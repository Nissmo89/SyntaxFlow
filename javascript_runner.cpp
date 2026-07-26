#include "javascript_runner.h"

#include <QMutexLocker>
#include <QProcess>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>
#include <QDebug>
#include <QCoreApplication>

JavascriptRunner::JavascriptRunner()
{
    m_initialised = true;
}

JavascriptRunner::~JavascriptRunner()
{
}

EmbeddedRunner::Result JavascriptRunner::execute(const QString &code,
                                             const QString &stdinInput,
                                             volatile bool *abort,
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

    const QString srcPath = tmpDir.filePath(QStringLiteral("user_code.js"));

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

    QProcess runProc;
    runProc.setWorkingDirectory(tmpDir.path());

    QString basePath = QCoreApplication::applicationDirPath();
#ifdef Q_OS_WIN
    QString wasmerExe = QDir::cleanPath(basePath + "/../tools/wasmer/bin/wasmer.exe");
#else
    QString wasmerExe = QDir::cleanPath(basePath + "/../tools/wasmer/bin/wasmer");
#endif

    QStringList args;
    args << "run" << "--dir=." << "syrusakbary/quickjs" << "--command" << "qjs" << "--" << "user_code.js";
    
    runProc.start(wasmerExe, args);

    if (!stdinInput.isEmpty()) {
        runProc.write(stdinInput.toUtf8());
    }
    runProc.closeWriteChannel();

    const int TIMEOUT_MS = 15000;
    QElapsedTimer timer;
    timer.start();

    while (!runProc.waitForFinished(100)) {
        QCoreApplication::processEvents();
        if (abort && *abort) {
            runProc.kill();
            result.error    = QStringLiteral("Execution stopped by user");
            result.exitCode = -1;
            result.timedOut = false;
            return result;
        }
        if (timer.elapsed() >= TIMEOUT_MS) {
            runProc.kill();
            result.error    = QStringLiteral("Time limit exceeded (15 s)");
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
