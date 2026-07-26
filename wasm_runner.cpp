#include "wasm_runner.h"

#include <QMutexLocker>
#include <QProcess>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QStandardPaths>

WasmRunner::WasmRunner(bool isCpp)
    : m_isCpp(isCpp)
{}

QString WasmRunner::getWasmerExecutable() const {
    // Try to find the downloaded Wasmer executable
    QString basePath = QCoreApplication::applicationDirPath();
    QStringList searchPaths = {
        basePath + "/tools/wasmer/bin/wasmer",
        basePath + "/../tools/wasmer/bin/wasmer",
        basePath + "/../../tools/wasmer/bin/wasmer",
        basePath + "/../../../tools/wasmer/bin/wasmer"
    };

#ifdef Q_OS_WIN
    for (int i = 0; i < searchPaths.size(); ++i) {
        searchPaths[i] += ".exe";
    }
#endif

    for (const QString &path : searchPaths) {
        if (QFile::exists(path)) {
            return path;
        }
    }
    
    // Fallback to system PATH
    return "wasmer";
}

EmbeddedRunner::Result WasmRunner::execute(const QString &code,
                                             const QString &stdinInput,
                                             volatile bool *stopRequested,
                                             const QMap<QString, QString> &additionalFiles)
{
    QMutexLocker lock(&m_mutex);
    Result result;

    QTemporaryDir tmpDir;
    if (!tmpDir.isValid()) {
        result.error    = QStringLiteral("Failed to create temp directory");
        result.exitCode = -1;
        return result;
    }

    const QString ext     = m_isCpp ? QStringLiteral(".cpp") : QStringLiteral(".c");
    const QString srcPath = tmpDir.filePath(QStringLiteral("user_code") + ext);
    const QString binPath = tmpDir.filePath(QStringLiteral("user_bin.wasm"));

    QFile srcFile(srcPath);
    if (!srcFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        result.error    = QStringLiteral("Failed to write source file");
        result.exitCode = -1;
        return result;
    }
    srcFile.write(code.toUtf8());
    srcFile.close();

    for (auto it = additionalFiles.begin(); it != additionalFiles.end(); ++it) {
        QFile file(tmpDir.filePath(it.key()));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(it.value().toUtf8());
            file.close();
        }
    }

    QString wasmerExe = getWasmerExecutable();

    // ── 1. Compile to WASM using clang/clang package ────────────────────────
    QProcess compileProc;
    compileProc.setWorkingDirectory(tmpDir.path());
    
    QString basePath = QCoreApplication::applicationDirPath();
    QString includeDir = QDir::cleanPath(basePath + "/../resources/include");
    
    QStringList compileArgs;
    compileArgs << "run" << "--dir=." << ("--dir=" + includeDir) << "clang/clang" << "--";
    compileArgs << "-I" + includeDir;
    
    // Target wasm32-wasi
    if (m_isCpp) {
        compileArgs << "-xc++";
        // C++ compiling might need additional flags for WASI libc++ if supported,
        // but clang/clang generally defaults to C/C++ capable.
    } else {
        compileArgs << "-xc";
    }
    
    compileArgs << "user_code" + ext << "-o" << "user_bin.wasm";
    
    compileProc.start(wasmerExe, compileArgs);
    if (!compileProc.waitForFinished(20000)) { // 20s compile timeout
        compileProc.kill();
        compileProc.waitForFinished(500);
        result.error    = QStringLiteral("Compilation timed out");
        result.exitCode = -1;
        return result;
    }

    if (compileProc.exitStatus() != QProcess::NormalExit || compileProc.exitCode() != 0) {
        result.error    = QString::fromUtf8(compileProc.readAllStandardError());
        if (result.error.isEmpty()) {
            result.error = QString::fromUtf8(compileProc.readAllStandardOutput());
        }
        result.exitCode = compileProc.exitCode() != 0 ? compileProc.exitCode() : -1;
        return result;
    }

    // ── 2. Run the generated WASM binary ────────────────────────────────────
    QProcess runProc;
    runProc.setWorkingDirectory(tmpDir.path());
    
    QStringList runArgs;
    runArgs << "run" << "--dir=." << "user_bin.wasm";
    
    runProc.start(wasmerExe, runArgs);
    if (!stdinInput.isEmpty()) {
        runProc.write(stdinInput.toUtf8());
    }
    runProc.closeWriteChannel();

    const int TIMEOUT_MS = 5000;
    QElapsedTimer timer;
    timer.start();

    while (!runProc.waitForFinished(100)) {
        if (stopRequested && *stopRequested) {
            runProc.kill();
            runProc.waitForFinished(500);
            result.error    = QStringLiteral("Execution stopped by user");
            result.exitCode = -1;
            result.timedOut = false;
            return result;
        }
        if (timer.elapsed() >= TIMEOUT_MS) {
            runProc.kill();
            runProc.waitForFinished(500);
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
