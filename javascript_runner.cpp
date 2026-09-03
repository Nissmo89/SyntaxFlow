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
                                             const QMap<QString, QString> &additionalFiles,
                                             int timeoutMs)
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
    QString toolsPath = QDir::cleanPath(basePath + "/tools");
    if (!QDir(toolsPath).exists()) {
        toolsPath = QDir::cleanPath(basePath + "/../tools");
    }
    if (!QDir(toolsPath).exists()) {
        toolsPath = QDir::cleanPath(basePath + "/../../tools");
    }

#ifdef Q_OS_WIN
    QString wasmerExe = QDir::cleanPath(toolsPath + "/wasmer/bin/wasmer.exe");
#else
    QString wasmerExe = QDir::cleanPath(toolsPath + "/wasmer/bin/wasmer");
#endif

    QStringList args;
    QString pkgPath = QDir::cleanPath(toolsPath + "/wasmer/packages/quickjs");
    args << "run" << "--mapdir" << "/src:." << pkgPath << "--command" << "qjs" << "--" << "/src/user_code.js";
    
    qDebug() << "JavascriptRunner: starting wasmer... args:" << args;
    runProc.start(wasmerExe, args);
    if (!runProc.waitForStarted(2000)) {
        result.error = QStringLiteral("Failed to start engine: ") + runProc.errorString();
        result.exitCode = -1;
        qDebug() << "JavascriptRunner: Failed to start";
        return result;
    }

    if (!stdinInput.isEmpty()) {
        runProc.write(stdinInput.toUtf8());
    }
    runProc.closeWriteChannel();

    QElapsedTimer timer;
    timer.start();
    qDebug() << "JavascriptRunner: waiting for finish...";
        QByteArray outBuf;
    QByteArray errBuf;
    const int MAX_OUTPUT_SIZE = 1 * 1024 * 1024; // 1 MB

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

        if (abort && *abort) {
            runProc.kill();
            result.error    = QStringLiteral("Execution stopped by user");
            result.exitCode = -1;
            result.timedOut = false;
            qDebug() << "JavascriptRunner: aborted";
            return result;
        }
        if (timer.elapsed() >= timeoutMs) {
            runProc.kill();
            result.error    = QString("Time limit exceeded (%1 s)").arg(timeoutMs / 1000.0);
            result.exitCode = -1;
            result.timedOut = true;
            qDebug() << "JavascriptRunner: timed out";
            return result;
        }
    }
    qDebug() << "JavascriptRunner: finished. exitCode:" << runProc.exitCode();
    
    outBuf.append(runProc.readAllStandardOutput());
    errBuf.append(runProc.readAllStandardError());
    if (outBuf.size() + errBuf.size() > MAX_OUTPUT_SIZE) {
        result.error = QString("Output limit exceeded (%1 MB)").arg(MAX_OUTPUT_SIZE / 1024.0 / 1024.0);
        result.exitCode = -1;
        result.timedOut = false;
        return result;
    }
    result.output = QString::fromLocal8Bit(outBuf);
    result.error = QString::fromLocal8Bit(errBuf);
    result.exitCode = runProc.exitCode();
    qDebug() << "JavascriptRunner: returning result";
    return result;
}
