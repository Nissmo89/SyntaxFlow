#include "python_runner.h"

#include <QMutexLocker>
#include <QProcess>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>
#include <QDebug>

PythonRunner::PythonRunner()
{
    m_initialised = true;
}

PythonRunner::~PythonRunner()
{
}

EmbeddedRunner::Result PythonRunner::execute(const QString &code,
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

    const QString srcPath = tmpDir.filePath(QStringLiteral("user_code.py"));

    QFile srcFile(srcPath);
    if (!srcFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        result.error    = QStringLiteral("Failed to write source file");
        result.exitCode = -1;
        return result;
    }
    srcFile.write(code.toUtf8());
    srcFile.close();

    QProcess runProc;
    runProc.setWorkingDirectory(tmpDir.path());
#ifdef Q_OS_WIN
    QString pythonExe = QStringLiteral("python");
#else
    QString pythonExe = QStringLiteral("python3");
#endif
    runProc.start(pythonExe, {srcPath});

    if (!stdinInput.isEmpty()) {
        runProc.write(stdinInput.toUtf8());
    }
    runProc.closeWriteChannel();

    const int TIMEOUT_MS = 5000;
    QElapsedTimer timer;
    timer.start();

    while (!runProc.waitForFinished(100)) {
        if (abort && *abort) {
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
