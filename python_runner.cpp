#include "python_runner.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <fstream>
#include <string>

PythonRunner::PythonRunner() { m_initialised = true; }

PythonRunner::~PythonRunner() {}

EmbeddedRunner::Result PythonRunner::execute(
    const QString &code, const QString &stdinInput, volatile bool *abort,
    const QMap<QString, QString> &additionalFiles, int timeoutMs) {
  Result result;

    std::string x_code = code.toStdString();

  std::ofstream out("test_text_python_outcome.py");
  out << x_code;
  out.close();

  qDebug() << "EmbeddedRunner::Result:" << code;
  

  QTemporaryDir tmpDir;
  if (!tmpDir.isValid()) {
    result.error = QStringLiteral("Failed to create temp directory");
    result.exitCode = -1;
    return result;
  }

  const QString srcPath = tmpDir.filePath(QStringLiteral("user_code.py"));

  QFile srcFile(srcPath);
  if (!srcFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    result.error = QStringLiteral("Failed to write source file");
    result.exitCode = -1;
    return result;
  }
  srcFile.write(code.toUtf8());
  srcFile.close();

  for (auto it = additionalFiles.begin(); it != additionalFiles.end(); ++it) {
    QFile file(tmpDir.filePath(it.key()));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      file.write(it.value().toUtf8());
      // qDebug() << "some other info -> " << file.readAll();
      file.close();
    }
  }

  QProcess runProc;
  runProc.setWorkingDirectory(tmpDir.path());

  // qDebug() << "some other info -> " ;

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
  QString pkgPath = QDir::cleanPath(toolsPath + "/wasmer/packages/python");
  args << "run" << "--mapdir" << "/src:." << pkgPath << "--"
       << "/src/user_code.py";

  // QString final_solution = QFile("/src/user_code.py").;

  QFile file(tmpDir.path() + "/user_code.py");

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QString final_solution = QString::fromUtf8(file.readAll());

      qDebug().noquote() << "Final code being executed:";
      qDebug().noquote() << final_solution;
  } else {
      qWarning() << "Failed to read final solution:"
                 << file.errorString();
  }

  // qDebug() << 

  qDebug() << "PythonRunner: starting wasmer... args:" << args;
  runProc.start(wasmerExe, args);
  if (!runProc.waitForStarted(2000)) {
    result.error =
        QStringLiteral("Failed to start engine: ") + runProc.errorString();
    result.exitCode = -1;
    qDebug() << "PythonRunner: Failed to start";
    // qDebug() << "with this error -> " + runProc.errorString();
    return result;
  }

  if (!stdinInput.isEmpty()) {
    runProc.write(stdinInput.toUtf8());
  }
  runProc.closeWriteChannel();

  QElapsedTimer timer;
  timer.start();
  qDebug() << "PythonRunner: waiting for finish...";
  QByteArray outBuf;
  QByteArray errBuf;
  const int MAX_OUTPUT_SIZE = 100 * 1024 * 1024; // 100 MB

  while (!runProc.waitForFinished(100)) {
    outBuf.append(runProc.readAllStandardOutput());
    errBuf.append(runProc.readAllStandardError());
    if (outBuf.size() + errBuf.size() > MAX_OUTPUT_SIZE) {
      runProc.kill();
      runProc.waitForFinished(500);
      result.error = QString("Output limit exceeded (%1 MB)")
                         .arg(MAX_OUTPUT_SIZE / 1024.0 / 1024.0);
      result.exitCode = -1;
      result.timedOut = false;
      return result;
    }

    if (abort && *abort) {
      runProc.kill();
      result.error = QStringLiteral("Execution stopped by user");
      result.exitCode = -1;
      result.timedOut = false;
      qDebug() << "PythonRunner: aborted";
      return result;
    }
    if (timer.elapsed() >= timeoutMs) {
      runProc.kill();
      result.error =
          QString("Time limit exceeded (%1 s)").arg(timeoutMs / 1000.0);
      result.exitCode = -1;
      result.timedOut = true;
      qDebug() << "PythonRunner: timed out";
      return result;
    }
  }
  qDebug() << "PythonRunner: finished. exitCode:" << runProc.exitCode();
  qDebug() << "with this error -> " + runProc.errorString();


  outBuf.append(runProc.readAllStandardOutput());
  errBuf.append(runProc.readAllStandardError());
  if (outBuf.size() + errBuf.size() > MAX_OUTPUT_SIZE) {
    result.error = QString("Output limit exceeded (%1 MB)")
                       .arg(MAX_OUTPUT_SIZE / 1024.0 / 1024.0);
    result.exitCode = -1;
    result.timedOut = false;
    return result;
  }
  result.output = QString::fromLocal8Bit(outBuf);
  result.error = QString::fromLocal8Bit(errBuf);
  result.exitCode = runProc.exitCode();
  qDebug() << "PythonRunner: returning result";
  return result;
}
