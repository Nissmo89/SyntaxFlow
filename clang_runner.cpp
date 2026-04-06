// clang_runner.cpp  —  macOS QProcess-based C/C++ runner (BUILD-05)
// ─────────────────────────────────────────────────────────────────────────────
// Compiles the given source via clang/clang++, runs the resulting binary,
// and captures its stdout/stderr.  Used on Apple platforms where TCC is
// unavailable (no macOS / Apple Silicon backend).
// ─────────────────────────────────────────────────────────────────────────────
#ifdef SF_USE_CLANG_RUNNER

#include "clang_runner.h"

#include <QMutexLocker>
#include <QProcess>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>

ClangRunner::ClangRunner(bool isCpp)
    : m_isCpp(isCpp)
{}

EmbeddedRunner::Result ClangRunner::execute(const QString &code,
                                             const QString &stdinInput,
                                             volatile bool *stopRequested)
{
    QMutexLocker lock(&m_mutex);
    Result result;

    // ── 1. Write source to a temp file ───────────────────────────────────────
    QTemporaryDir tmpDir;
    if (!tmpDir.isValid()) {
        result.error    = QStringLiteral("Failed to create temp directory");
        result.exitCode = -1;
        return result;
    }

    const QString ext     = m_isCpp ? QStringLiteral(".cpp") : QStringLiteral(".c");
    const QString srcPath = tmpDir.filePath(QStringLiteral("user_code") + ext);
    const QString binPath = tmpDir.filePath(QStringLiteral("user_bin"));

    QFile srcFile(srcPath);
    if (!srcFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        result.error    = QStringLiteral("Failed to write source file");
        result.exitCode = -1;
        return result;
    }
    srcFile.write(code.toUtf8());
    srcFile.close();

    // ── 2. Compile ────────────────────────────────────────────────────────────
    const QString compiler = m_isCpp ? QStringLiteral("clang++") : QStringLiteral("clang");
    QProcess compileProc;
    compileProc.start(compiler, {srcPath, QStringLiteral("-o"), binPath,
                                 QStringLiteral("-std="), m_isCpp ? QStringLiteral("c++17") : QStringLiteral("c11")});
    compileProc.waitForFinished(15000); // 15 s compile timeout

    if (compileProc.exitCode() != 0) {
        result.error    = QString::fromUtf8(compileProc.readAllStandardError());
        result.exitCode = compileProc.exitCode();
        return result;
    }

    // ── 3. Run binary ─────────────────────────────────────────────────────────
    QProcess runProc;
    runProc.setWorkingDirectory(tmpDir.path());
    runProc.start(binPath, {});
    if (!stdinInput.isEmpty())
        runProc.write(stdinInput.toUtf8());
    runProc.closeWriteChannel();

    const int TIMEOUT_MS = 5000;
    QElapsedTimer timer;
    timer.start();

    while (!runProc.waitForFinished(100)) {
        if (stopRequested && *stopRequested) {
            runProc.kill();
            result.error    = QStringLiteral("Execution stopped by user");
            result.exitCode = -1;
            result.timedOut = false;
            return result;
        }
        if (timer.elapsed() >= TIMEOUT_MS) {
            runProc.kill();
            result.error    = QStringLiteral("Time limit exceeded (5 s)");
            result.exitCode = -1;
            result.timedOut = true;
            return result;
        }
    }

    result.output   = QString::fromLocal8Bit(runProc.readAllStandardOutput());
    result.error    = QString::fromLocal8Bit(runProc.readAllStandardError());
    result.exitCode = runProc.exitCode();
    return result;
}

#endif // SF_USE_CLANG_RUNNER
