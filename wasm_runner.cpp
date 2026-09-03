#include "wasm_runner.h"

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

QString WasmRunner::getWasiSdkCompiler() const {
    QString basePath = QCoreApplication::applicationDirPath();
    QString compilerName = m_isCpp ? "clang++" : "clang";
    
    QStringList searchPaths = {
        basePath + "/_deps/wasi_sdk-src/bin/" + compilerName,
        basePath + "/_deps/wasi_sdk-src/wasi-sdk-20.0/bin/" + compilerName,
        basePath + "/wasi-sdk/bin/" + compilerName,
        basePath + "/wasi-sdk/wasi-sdk-20.0/bin/" + compilerName,
        basePath + "/../wasi-sdk/bin/" + compilerName,
        basePath + "/../wasi-sdk/wasi-sdk-20.0/bin/" + compilerName,
        basePath + "/../../wasi-sdk/bin/" + compilerName,
        basePath + "/../../wasi-sdk/wasi-sdk-20.0/bin/" + compilerName,
        basePath + "/../../../wasi-sdk/bin/" + compilerName,
        basePath + "/../../../wasi-sdk/wasi-sdk-20.0/bin/" + compilerName
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
    
    return compilerName;
}

EmbeddedRunner::Result WasmRunner::execute(const QString &code,
                                             const QString &stdinInput,
                                             volatile bool *stopRequested,
                                             const QMap<QString, QString> &additionalFiles,
                                             int timeoutMs)
{
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

    // ── 1. Compile to WASM natively using wasi-sdk ──────────────────────
    QProcess compileProc;
    compileProc.setWorkingDirectory(tmpDir.path());
    
    QString basePath = QCoreApplication::applicationDirPath();
    QString includeDir = QDir::cleanPath(basePath + "/../resources/include");
    QString wasiSdkCompiler = getWasiSdkCompiler();
    
    QStringList compileArgs;
    compileArgs << "-I" + includeDir;
    
    if (m_isCpp) {
        QString pchPath = includeDir + "/stdcpp.h.pch";
        if (!QFile::exists(pchPath)) {
            QStringList pchArgs;
            pchArgs << "-x" << "c++-header" << "-fno-exceptions" << "-fno-rtti" 
                    << "-DJSON_HAS_FILESYSTEM=0" << "-DJSON_HAS_EXPERIMENTAL_FILESYSTEM=0"
                    << "-I" + includeDir << includeDir + "/stdcpp.h" << "-o" << pchPath;
            QProcess pchProc;
            pchProc.start(wasiSdkCompiler, pchArgs);
            pchProc.waitForFinished(-1);
        }
        
        compileArgs << "-fno-exceptions" << "-fno-rtti" << "-DJSON_HAS_FILESYSTEM=0" << "-DJSON_HAS_EXPERIMENTAL_FILESYSTEM=0";
        if (QFile::exists(pchPath)) {
            compileArgs << "-include-pch" << pchPath;
        }
    }
    
    compileArgs << "user_code" + ext << "-o" << "user_bin.wasm";
    
    qDebug() << "WasmRunner: starting compileProc... args:" << compileArgs;
    compileProc.start(wasiSdkCompiler, compileArgs);
    if (!compileProc.waitForStarted(2000)) {
        result.error = QStringLiteral("Failed to start compiler: ") + compileProc.errorString();
        result.exitCode = -1;
        qDebug() << "WasmRunner: Failed to start compileProc";
        return result;
    }
    compileProc.closeWriteChannel();
    QElapsedTimer compileTimer;
    compileTimer.start();
    qDebug() << "WasmRunner: waiting for compile finish...";
    while (!compileProc.waitForFinished(100)) {
        if (stopRequested && *stopRequested) {
            compileProc.kill();
            compileProc.waitForFinished(500);
            result.error    = QStringLiteral("Compilation stopped by user");
            result.exitCode = -1;
            qDebug() << "WasmRunner: compile aborted";
            return result;
        }
        if (compileTimer.elapsed() >= 300000) {
            compileProc.kill();
            compileProc.waitForFinished(500);
            result.error    = QStringLiteral("Compilation timed out (300 s)");
            result.exitCode = -1;
            qDebug() << "WasmRunner: compile timed out";
            return result;
        }
    }
    qDebug() << "WasmRunner: compile finished. exitCode:" << compileProc.exitCode();

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
    runArgs << "run" << "--mapdir" << "/src:." << "user_bin.wasm";
    
    qDebug() << "WasmRunner: starting wasmer... args:" << runArgs;
    runProc.start(wasmerExe, runArgs);
    if (!runProc.waitForStarted(2000)) {
        result.error = QStringLiteral("Failed to start engine: ") + runProc.errorString();
        result.exitCode = -1;
        qDebug() << "WasmRunner: Failed to start";
        return result;
    }

    if (!stdinInput.isEmpty()) {
        runProc.write(stdinInput.toUtf8());
    }
    runProc.closeWriteChannel();

    QElapsedTimer timer;
    timer.start();
    qDebug() << "WasmRunner: waiting for finish...";
        QByteArray outBuf;
    QByteArray errBuf;
    const int MAX_OUTPUT_SIZE = 10 * 1024 * 1024; // 1 MB

    while (!runProc.waitForFinished(100)) {
        outBuf.append(runProc.readAllStandardOutput());
        errBuf.append(runProc.readAllStandardError());
        if (outBuf.size() + errBuf.size() > MAX_OUTPUT_SIZE) {
            runProc.kill();
            runProc.waitForFinished(500);
            result.error = QString("Output limit exceeded (%1 MB)").arg(MAX_OUTPUT_SIZE / 1024.0 / 1024.0);
            result.exitCode = -1;
            result.timedOut = false;
            return result;
        }

        if (stopRequested && *stopRequested) {
            runProc.kill();
            runProc.waitForFinished(500);
            result.error    = QStringLiteral("Execution stopped by user");
            result.exitCode = -1;
            result.timedOut = false;
            qDebug() << "WasmRunner: aborted";
            return result;
        }
        if (timer.elapsed() >= timeoutMs) {
            runProc.kill();
            runProc.waitForFinished(500);
            result.error    = QString("Time limit exceeded (%1 s)").arg(timeoutMs / 1000.0);
            result.exitCode = -1;
            result.timedOut = true;
            qDebug() << "WasmRunner: timed out";
            return result;
        }
    }
    qDebug() << "WasmRunner: finished. exitCode:" << runProc.exitCode();
    
    outBuf.append(runProc.readAllStandardOutput());
    errBuf.append(runProc.readAllStandardError());
    if (outBuf.size() + errBuf.size() > MAX_OUTPUT_SIZE) {
        result.error = QString("Output limit exceeded (%1 MB)").arg(MAX_OUTPUT_SIZE / 1024.0 / 1024.0);
        result.exitCode = -1;
        result.timedOut = false;
        return result;
    }
    result.output = QString::fromUtf8(outBuf);
    result.error = QString::fromUtf8(errBuf);
    result.exitCode = runProc.exitCode();
    result.timedOut = false;
    qDebug() << "WasmRunner: returning result";
    return result;
}
