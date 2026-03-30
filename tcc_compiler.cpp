// tcc_compiler.cpp  —  TccRunner implementation
// ─────────────────────────────────────────────────────────────────────────────
// Stdout/stderr capture:
//   1. Open two OS pipes (stdout, stderr).
//   2. dup2() write-ends over STDOUT_FILENO / STDERR_FILENO.
//   3. Open a third pipe for stdin, write the test input into it.
//   4. Call tcc_run().
//   5. Restore original fds, drain captured bytes from read-ends.
//
// exit() interception:
//   tcc_add_symbol() shadows libc's exit().  Our replacement longjmp()s
//   back so the user's exit(0) never terminates the Qt process.
//
// Thread safety:
//   m_mutex serialises runs because dup2 is process-global.
//   Pipe buffer is 64 KB on Linux which covers all competitive-programming
//   output sizes.  Add a reader thread if you need more.
// ─────────────────────────────────────────────────────────────────────────────

#include "tcc_compiler.h"
#include <libtcc.h>
#include <QCoreApplication>
#include <QMutexLocker>
#include <cstdio>
#include <csetjmp>

#ifdef Q_OS_WIN
#  include <io.h>
#  include <fcntl.h>
#  define SF_PIPE(fds)       ::_pipe(fds, 65536, _O_BINARY)
#  define SF_DUP(fd)         ::_dup(fd)
#  define SF_DUP2(s,d)       ::_dup2(s,d)
#  define SF_CLOSE(fd)       ::_close(fd)
#  define SF_READ(fd,b,n)    ::_read(fd,b,(unsigned)(n))
#  define SF_WRITE(fd,b,n)   ::_write(fd,b,(unsigned)(n))
#  define SF_STDOUT          _fileno(stdout)
#  define SF_STDERR          _fileno(stderr)
#  define SF_STDIN           _fileno(stdin)
#else
#  include <unistd.h>
#  include <fcntl.h>
#  define SF_PIPE(fds)       ::pipe(fds)
#  define SF_DUP(fd)         ::dup(fd)
#  define SF_DUP2(s,d)       ::dup2(s,d)
#  define SF_CLOSE(fd)       ::close(fd)
#  define SF_READ(fd,b,n)    ::read(fd,b,n)
#  define SF_WRITE(fd,b,n)   ::write(fd,b,n)
#  define SF_STDOUT          STDOUT_FILENO
#  define SF_STDERR          STDERR_FILENO
#  define SF_STDIN           STDIN_FILENO
#endif

// ── exit() interception ──────────────────────────────────────────────────────
static thread_local jmp_buf  s_exitJmp;
static thread_local int      s_exitCode = 0;
static thread_local bool     s_inRun    = false;

static void tcc_fake_exit(int code)
{
    if (s_inRun) { s_exitCode = code; longjmp(s_exitJmp, 1); }
    ::exit(code);
}

// ── TCC error collector ──────────────────────────────────────────────────────
struct TccErrCtx { QString msg; };
static void tcc_err_cb(void *op, const char *m)
{
    static_cast<TccErrCtx*>(op)->msg += QString::fromUtf8(m) + QLatin1Char('\n');
}

// ── Drain a non-blocking read-end ────────────────────────────────────────────
static QString drain(int fd)
{
    QString out;
    char buf[4096];
#ifndef Q_OS_WIN
    ::fcntl(fd, F_SETFL, O_NONBLOCK);
#endif
    for (;;) {
        auto n = SF_READ(fd, buf, sizeof(buf));
        if (n <= 0) break;
        out += QString::fromLocal8Bit(buf, static_cast<int>(n));
    }
    return out;
}

// ─────────────────────────────────────────────────────────────────────────────
TccRunner::TccRunner()
    : m_libPath(QCoreApplication::applicationDirPath() + QLatin1String("/tcc"))
{}

EmbeddedRunner::Result TccRunner::execute(const QString &code,
                                           const QString &stdinInput,
                                           volatile bool *)
{
    QMutexLocker lock(&m_mutex);
    Result result;

    // 1. Create pipes
    int outP[2], errP[2], inP[2];
    if (SF_PIPE(outP) < 0 || SF_PIPE(errP) < 0 || SF_PIPE(inP) < 0) {
        result.error = QStringLiteral("pipe() failed");
        result.exitCode = -1;
        return result;
    }

    // 2. Save original fds
    int savOut = SF_DUP(SF_STDOUT);
    int savErr = SF_DUP(SF_STDERR);
    int savIn  = SF_DUP(SF_STDIN);

    // 3. Redirect stdout / stderr
    SF_DUP2(outP[1], SF_STDOUT); SF_CLOSE(outP[1]);
    SF_DUP2(errP[1], SF_STDERR); SF_CLOSE(errP[1]);

    // 4. Inject stdin (write input then close write-end to signal EOF)
    {
        QByteArray b = stdinInput.toUtf8();
        if (!b.isEmpty()) SF_WRITE(inP[1], b.constData(), b.size());
        SF_CLOSE(inP[1]);
    }
    SF_DUP2(inP[0], SF_STDIN); SF_CLOSE(inP[0]);

    // 5. Compile
    TccErrCtx ec;
    TCCState *s = tcc_new();
    tcc_set_error_func(s, &ec, tcc_err_cb);
    tcc_set_lib_path(s, m_libPath.toUtf8().constData());
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    auto restore = [&]() {
        fflush(stdout); fflush(stderr);
        SF_DUP2(savOut, SF_STDOUT); SF_CLOSE(savOut);
        SF_DUP2(savErr, SF_STDERR); SF_CLOSE(savErr);
        SF_DUP2(savIn,  SF_STDIN);  SF_CLOSE(savIn);
        SF_CLOSE(outP[0]); SF_CLOSE(errP[0]);
    };

    if (tcc_compile_string(s, code.toUtf8().constData()) < 0) {
        restore(); tcc_delete(s);
        result.error    = ec.msg.isEmpty() ? QStringLiteral("Compilation failed") : ec.msg;
        result.exitCode = 1;
        return result;
    }
    if (tcc_relocate(s) < 0) {
        restore(); tcc_delete(s);
        result.error    = QStringLiteral("Relocation failed\n") + ec.msg;
        result.exitCode = 1;
        return result;
    }

    // 6. Look up the custom entry point (never call tcc_run — it crashes trying to load libtcc1.a)
    //    The DriverGenerator generates a function named _sf_main instead of main.
    //    For free-code runs (no driver), we fall back to looking for main.
    typedef int (*MainFn)();
    MainFn fn = (MainFn)tcc_get_symbol(s, "_sf_main");
    if (!fn) fn = (MainFn)tcc_get_symbol(s, "main");

    if (!fn) {
        restore(); tcc_delete(s);
        result.error    = QStringLiteral("No entry point found (_sf_main or main)");
        result.exitCode = -1;
        return result;
    }

    // 7. Call the entry point directly (intercept exit() to prevent process termination)
    s_inRun    = true;
    s_exitCode = 0;
    int ret    = 0;
    tcc_add_symbol(s, "exit",  reinterpret_cast<void*>(&tcc_fake_exit));
    tcc_add_symbol(s, "_exit", reinterpret_cast<void*>(&tcc_fake_exit));
    if (setjmp(s_exitJmp) == 0)
        ret = fn();
    else
        ret = s_exitCode;
    s_inRun = false;

    // 8. Restore fds and collect output
    fflush(stdout); fflush(stderr);
    SF_DUP2(savOut, SF_STDOUT); SF_CLOSE(savOut);
    SF_DUP2(savErr, SF_STDERR); SF_CLOSE(savErr);
    SF_DUP2(savIn,  SF_STDIN);  SF_CLOSE(savIn);

    result.output   = drain(outP[0]); SF_CLOSE(outP[0]);
    result.error    = drain(errP[0]); SF_CLOSE(errP[0]);
    result.exitCode = ret;

    if (!ec.msg.isEmpty())
        result.error = ec.msg + result.error;   // prepend warnings

    tcc_delete(s);
    return result;
}
