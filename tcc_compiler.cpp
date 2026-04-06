// tcc_compiler.cpp  —  TccRunner implementation
// ─────────────────────────────────────────────────────────────────────────────
// Stdout/stderr capture:
//   1. Open two OS pipes (stdout, stderr).
//   2. dup2() write-ends over STDOUT_FILENO / STDERR_FILENO.
//   3. Open a third pipe for stdin, write the test input into it.
//   4. Call the compiled entry point via tcc_get_symbol().
//   5. Restore original fds; drainer thread collects captured bytes.
//
// exit() interception:
//   tcc_add_symbol() shadows libc's exit() — must be called BEFORE
//   tcc_relocate() so TCC binds our hook, not libc's.  Our replacement
//   longjmp()s back so the user's exit(0) never terminates the Qt process.
//
// Pipe deadlock prevention (BUG-02):
//   A background drainer thread reads from the pipe concurrently with fn().
//   This prevents the 64 KB pipe-buffer deadlock on heavy-output programs.
//
// Qt thread isolation (BUG-03):
//   A null message handler is installed before dup2 and restored after so
//   Qt's own qDebug/qWarning calls don't pollute the captured pipe buffer.
//
// Segfault sandboxing (DESIGN-02):
//   A SIGSEGV/SIGFPE handler longjmps out of user code rather than
//   terminating the Qt process.  This is best-effort — a corrupt stack
//   may still crash; QProcess is the proper long-term fix.
//
// Watchdog (BUG-05):
//   A second background thread kills the run after TIMEOUT_MS if the user
//   code has not finished (e.g. infinite loop).
//
// Thread safety:
//   m_mutex serialises runs because dup2 is process-global.
// ─────────────────────────────────────────────────────────────────────────────

#include "tcc_compiler.h"
#include <libtcc.h>
#include <QCoreApplication>
#include <QMutexLocker>
#include <QThread>
#include <cstdio>
#include <csetjmp>
#include <atomic>

#ifndef Q_OS_WIN
#  include <signal.h>
#endif

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

// ── Crash handler (DESIGN-02) ────────────────────────────────────────────────
#ifndef Q_OS_WIN
static sigjmp_buf s_crashJmp;
static std::atomic<bool> s_crashHandlerActive{false};

static void crash_signal_handler(int sig)
{
    if (s_crashHandlerActive.load()) {
        s_crashHandlerActive = false;
        siglongjmp(s_crashJmp, sig);
    }
}
#endif

// ── TCC error collector ──────────────────────────────────────────────────────
struct TccErrCtx { QString msg; };
static void tcc_err_cb(void *op, const char *m)
{
    static_cast<TccErrCtx*>(op)->msg += QString::fromUtf8(m) + QLatin1Char('\n');
}

// ── Drain a blocking read-end (used from drainer thread) ─────────────────────
static QString drain(int fd)
{
    QString out;
    char buf[4096];
    // Blocking reads — the thread itself is the blocking entity.
    // EOF is signalled when the write-end of the pipe is closed.
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

    // ── 1. Create pipes ───────────────────────────────────────────────────────
    int outP[2], errP[2], inP[2];
    if (SF_PIPE(outP) < 0 || SF_PIPE(errP) < 0 || SF_PIPE(inP) < 0) {
        result.error = QStringLiteral("pipe() failed");
        result.exitCode = -1;
        return result;
    }

    // ── 2. Save original fds ─────────────────────────────────────────────────
    int savOut = SF_DUP(SF_STDOUT);
    int savErr = SF_DUP(SF_STDERR);
    int savIn  = SF_DUP(SF_STDIN);

    // ── 3. Suppress Qt debug output during the dup2 window (BUG-03) ──────────
    QtMessageHandler oldHandler = qInstallMessageHandler(
        [](QtMsgType, const QMessageLogContext&, const QString&) {}
    );

    // ── 4. Redirect stdout / stderr ───────────────────────────────────────────
    SF_DUP2(outP[1], SF_STDOUT); SF_CLOSE(outP[1]);
    SF_DUP2(errP[1], SF_STDERR); SF_CLOSE(errP[1]);

    // ── 5. Inject stdin ───────────────────────────────────────────────────────
    {
        QByteArray b = stdinInput.toUtf8();
        if (!b.isEmpty()) SF_WRITE(inP[1], b.constData(), b.size());
        SF_CLOSE(inP[1]);
    }
    SF_DUP2(inP[0], SF_STDIN); SF_CLOSE(inP[0]);

    // ── 6. Compile ────────────────────────────────────────────────────────────
    TccErrCtx ec;
    TCCState *s = tcc_new();
    tcc_set_error_func(s, &ec, tcc_err_cb);
    tcc_set_lib_path(s, m_libPath.toUtf8().constData());
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

    auto restoreFds = [&]() {
        fflush(stdout); fflush(stderr);
        SF_DUP2(savOut, SF_STDOUT); SF_CLOSE(savOut);
        SF_DUP2(savErr, SF_STDERR); SF_CLOSE(savErr);
        SF_DUP2(savIn,  SF_STDIN);  SF_CLOSE(savIn);
    };

    if (tcc_compile_string(s, code.toUtf8().constData()) < 0) {
        restoreFds();
        SF_CLOSE(outP[0]); SF_CLOSE(errP[0]);
        qInstallMessageHandler(oldHandler);
        tcc_delete(s);
        result.error    = ec.msg.isEmpty() ? QStringLiteral("Compilation failed") : ec.msg;
        result.exitCode = 1;
        return result;
    }

    // ── 7. Register exit hooks BEFORE tcc_relocate (BUG-01) ──────────────────
    tcc_add_symbol(s, "exit",  reinterpret_cast<void*>(&tcc_fake_exit));
    tcc_add_symbol(s, "_exit", reinterpret_cast<void*>(&tcc_fake_exit));

    if (tcc_relocate(s) < 0) {
        restoreFds();
        SF_CLOSE(outP[0]); SF_CLOSE(errP[0]);
        qInstallMessageHandler(oldHandler);
        tcc_delete(s);
        result.error    = QStringLiteral("Relocation failed\n") + ec.msg;
        result.exitCode = 1;
        return result;
    }

    // ── 8. Look up entry point ────────────────────────────────────────────────
    typedef int (*MainFn)();
    MainFn fn = (MainFn)tcc_get_symbol(s, "_sf_main");
    if (!fn) fn = (MainFn)tcc_get_symbol(s, "main");

    if (!fn) {
        restoreFds();
        SF_CLOSE(outP[0]); SF_CLOSE(errP[0]);
        qInstallMessageHandler(oldHandler);
        tcc_delete(s);
        result.error    = QStringLiteral("No entry point found (_sf_main or main)");
        result.exitCode = -1;
        return result;
    }

    // ── 9. Start background drainer thread (BUG-02) ───────────────────────────
    QString capturedOut;
    QString capturedErr;
    std::atomic<bool> drainDone{false};

    QThread* drainerThread = QThread::create([&]() {
        capturedOut = drain(outP[0]);
        capturedErr = drain(errP[0]);
        drainDone = true;
    });
    drainerThread->start();

    // ── 10. Watchdog thread (BUG-05) ──────────────────────────────────────────
    const int TIMEOUT_MS = 5000;
    std::atomic<bool> timedOut{false};

    QThread* watchdog = QThread::create([&]() {
        int elapsed = 0;
        while (elapsed < TIMEOUT_MS && !drainDone.load()) {
            QThread::msleep(50);
            elapsed += 50;
            if (watchdog->isInterruptionRequested()) return;
        }
        if (!drainDone.load()) {
            timedOut = true;
            s_inRun  = false; // trigger longjmp path in fn()
        }
    });
    watchdog->start();

    // ── 11. Install crash signal handler (DESIGN-02) ──────────────────────────
    int ret = 0;
#ifndef Q_OS_WIN
    struct sigaction sa{}, old_sa_segv, old_sa_fpe;
    sa.sa_handler = crash_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGSEGV, &sa, &old_sa_segv);
    sigaction(SIGFPE,  &sa, &old_sa_fpe);
    s_crashHandlerActive = true;

    int crash_sig = 0;
    if ((crash_sig = sigsetjmp(s_crashJmp, 1)) == 0) {
#endif
        // ── 12. Run user code ─────────────────────────────────────────────────
        s_inRun    = true;
        s_exitCode = 0;
        if (setjmp(s_exitJmp) == 0)
            ret = fn();
        else
            ret = s_exitCode;
        s_inRun = false;
#ifndef Q_OS_WIN
    } else {
        // Signal was caught — user code crashed
        s_inRun = false;
        s_crashHandlerActive = false;
        result.error    = QString("Runtime crash: signal %1 (segfault/div-by-zero)").arg(crash_sig);
        result.exitCode = -1;
    }

    // Restore signal handlers
    s_crashHandlerActive = false;
    sigaction(SIGSEGV, &old_sa_segv, nullptr);
    sigaction(SIGFPE,  &old_sa_fpe,  nullptr);
#endif

    // ── 13. Stop watchdog ─────────────────────────────────────────────────────
    watchdog->requestInterruption();
    watchdog->wait(500);
    delete watchdog;

    // ── 14. Restore fds — this closes write ends, signalling EOF to drainer ───
    restoreFds();
    qInstallMessageHandler(oldHandler);

    // ── 15. Wait for drainer to finish ────────────────────────────────────────
    drainerThread->wait(3000);
    delete drainerThread;

    SF_CLOSE(outP[0]);
    SF_CLOSE(errP[0]);

    // ── 16. Collect results ───────────────────────────────────────────────────
    if (timedOut) {
        result.timedOut = true;
        result.output   = capturedOut;
        result.exitCode = -1;
        if (result.error.isEmpty())
            result.error = QStringLiteral("Time limit exceeded (5 s)");
    } else if (result.exitCode != -1 || result.error.isEmpty()) {
        // Only overwrite if we didn't already set a crash error
        result.output   = capturedOut;
        if (result.error.isEmpty())
            result.error = capturedErr;
        else
            result.error = capturedErr + result.error;
        result.exitCode = ret;
    }

    if (!ec.msg.isEmpty())
        result.error = ec.msg + result.error;   // prepend compiler warnings

    tcc_delete(s);
    return result;
}
