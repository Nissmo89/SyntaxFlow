#include "backend.h"
#include "language_registry.h"
#include "code_runner.h"

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

Backend::Backend(QObject *parent) : QObject(parent) {
    m_registry = new LanguageRegistry(this);
    m_runner = new CodeRunner(m_registry, this);

    m_registry->initialize();

    // Connect runner signals
    connect(m_runner, &CodeRunner::testResult, this, &Backend::testResult);
    connect(m_runner, &CodeRunner::compilationError, this, &Backend::compilationError);
    connect(m_runner, &CodeRunner::systemError, this, &Backend::systemError);
    connect(m_runner, &CodeRunner::started, this, &Backend::executionStarted);
    connect(m_runner, &CodeRunner::finished, this, &Backend::executionFinished);
    connect(m_runner, &CodeRunner::progress, this, &Backend::progress);

    connect(m_registry, &LanguageRegistry::languagesChanged, this, &Backend::languagesChanged);

    qDebug() << "Backend initialized. Available:" << availableLanguages();
}

Backend::~Backend() = default;

QStringList Backend::availableLanguages() const {
    return m_registry->availableLanguages();
}

QStringList Backend::allLanguages() const {
    return m_registry->allLanguages();
}

LanguageConfig Backend::getLanguageConfig(const QString &id) const {
    return m_registry->getConfig(id);
}

QString Backend::getTemplate(const QString &languageId) const {
    return m_registry->getConfig(languageId).codeTemplate;
}

bool Backend::isLanguageAvailable(const QString &id) const {
    return m_registry->isLanguageAvailable(id);
}

bool Backend::isRunning() const {
    return m_runner->isRunning();
}

void Backend::runCode(const QString &code, const QString &languageId, const QString &problemId) {
    m_runner->runCode(code, languageId, problemId);
}

void Backend::runTestCase(const QString &code, const QString &languageId,
                          int testIndex, const QString &problemId) {
    m_runner->runSingleTest(code, languageId, testIndex, problemId);
}

// void Backend::stopExecution() {
//     if (m_currentProcess) {
//         m_currentProcess->kill();
//         m_currentProcess->waitForFinished(1000);
//     }
//     m_runner->stop();
// }

void Backend::requestTestCases(const QString &problemId) {
    QString relPath = "/data/problems/" + problemId + ".json";
    QStringList paths = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QCoreApplication::applicationDirPath() + "/../..",
        QCoreApplication::applicationDirPath() + "/../../.."
    };

    for (const QString &p : paths) {
        QString full = QDir(p).cleanPath(p + relPath);
        QFile file(full);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            emit testCasesReady(doc.object()["testCases"].toArray());
            return;
        }
    }

    emit systemError("Could not load test cases for: " + problemId);
}

bool Backend::addLanguage(const LanguageConfig &config) {
    return m_registry->addLanguage(config, true);
}

bool Backend::removeLanguage(const QString &id) {
    return m_registry->removeLanguage(id);
}

void Backend::reloadLanguages() {
    m_registry->reload();
}

void Backend::openConfigDirectory() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_registry->userConfigPath()));
}







void Backend::runFreeCode(const QString &code, const QString &languageId)
{
    qDebug() << "=== runFreeCode START ===";
    qDebug() << "Language:" << languageId;
    qDebug() << "Code length:" << code.length();

    // Clean up any previous process
    if (m_currentProcess) {
        qDebug() << "Killing previous process...";
        disconnect(m_currentProcess, nullptr, this, nullptr);  // Disconnect all signals first
        m_currentProcess->kill();
        m_currentProcess->waitForFinished(1000);
        m_currentProcess->deleteLater();
        m_currentProcess = nullptr;
    }

    if (!isLanguageAvailable(languageId)) {
        qDebug() << "ERROR: Language not available!";
        emit errorReceived("[Error] Language not available: " + languageId + "\n");
        emit programFinished(1, 0);
        return;
    }

    LanguageConfig cfg = getLanguageConfig(languageId);
    qDebug() << "Config - compiled:" << cfg.compiled;
    qDebug() << "Config - extension:" << cfg.extension;
    qDebug() << "Config - compileCommand:" << cfg.compileCommand;
    qDebug() << "Config - runCommand:" << cfg.runCommand;

    // Create temp directory
    QString tempDir = QDir::tempPath() + "/codelabs_" +
                      QString::number(QCoreApplication::applicationPid());
    QDir().mkpath(tempDir);

    QString sourceFile = tempDir + "/main" + cfg.extension;
    QString execFile = tempDir + "/program";

#ifdef Q_OS_WIN
    execFile += ".exe";
#endif

    qDebug() << "Temp dir:" << tempDir;
    qDebug() << "Source file:" << sourceFile;
    qDebug() << "Exec file:" << execFile;

    // Write source code
    QFile file(sourceFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot create source file!";
        emit errorReceived("[Error] Failed to create source file\n");
        emit programFinished(1, 0);
        return;
    }
    file.write(code.toUtf8());
    file.close();
    qDebug() << "Source file written successfully";

    emit executionStarted();
    emit outputReceived("");

    // ═══════════════════════════════════════════════════════════════════
    // COMPILATION (for compiled languages)
    // ═══════════════════════════════════════════════════════════════════
    if (cfg.compiled) {
        emit outputReceived("[Compiling...]\n");

        QProcess compiler;
        compiler.setWorkingDirectory(tempDir);

        QString compileCmd = cfg.compileCommand;
        compileCmd.replace("{source}", sourceFile);
        compileCmd.replace("{output}", execFile);

        qDebug() << "Compile command:" << compileCmd;

        QStringList parts = compileCmd.split(' ', Qt::SkipEmptyParts);
        if (parts.isEmpty()) {
            emit errorReceived("[Error] Invalid compile command\n");
            emit programFinished(1, 0);
            emit executionFinished();
            return;
        }

        QString program = parts.takeFirst();
        qDebug() << "Compiler program:" << program;
        qDebug() << "Compiler args:" << parts;

        compiler.start(program, parts);

        if (!compiler.waitForStarted(5000)) {
            qDebug() << "ERROR: Compiler failed to start!";
            emit errorReceived("[Error] Failed to start compiler: " + program + "\n");
            emit programFinished(1, 0);
            emit executionFinished();
            return;
        }

        if (!compiler.waitForFinished(30000)) {
            qDebug() << "ERROR: Compilation timeout!";
            compiler.kill();
            emit errorReceived("[Error] Compilation timeout (30s)\n");
            emit programFinished(1, 0);
            emit executionFinished();
            return;
        }

        QString stdErr = QString::fromUtf8(compiler.readAllStandardError());
        QString stdOut = QString::fromUtf8(compiler.readAllStandardOutput());

        qDebug() << "Compiler exit code:" << compiler.exitCode();

        if (compiler.exitCode() != 0) {
            emit errorReceived("[Compilation Error]\n");
            if (!stdErr.isEmpty()) emit errorReceived(stdErr);
            if (!stdOut.isEmpty()) emit errorReceived(stdOut);
            emit programFinished(1, 0);
            emit executionFinished();
            return;
        }

        emit outputReceived("[Compiled successfully]\n\n");
    }

    // ═══════════════════════════════════════════════════════════════════
    // EXECUTION
    // ═══════════════════════════════════════════════════════════════════
    m_currentProcess = new QProcess(this);
    m_currentProcess->setWorkingDirectory(tempDir);

    // Store process pointer for lambda captures
    QProcess *proc = m_currentProcess;

    // Connect output signals
    connect(proc, &QProcess::readyReadStandardOutput, this, [this, proc]() {
        if (proc != m_currentProcess) return;  // Stale connection
        QByteArray data = proc->readAllStandardOutput();
        QString text = QString::fromUtf8(data);
        qDebug() << "STDOUT:" << text;
        emit outputReceived(text);
    });

    connect(proc, &QProcess::readyReadStandardError, this, [this, proc]() {
        if (proc != m_currentProcess) return;  // Stale connection
        QByteArray data = proc->readAllStandardError();
        QString text = QString::fromUtf8(data);
        qDebug() << "STDERR:" << text;
        emit errorReceived(text);
    });

    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, proc](int exitCode, QProcess::ExitStatus status) {
                qDebug() << "Process finished with exit code:" << exitCode;

                if (status == QProcess::CrashExit && exitCode != 9) {
                    // exitCode 9 is SIGKILL (user stopped), don't show as crash
                    emit errorReceived("\n[Program crashed]\n");
                }

                emit programFinished(exitCode, 0);
                emit executionFinished();

                if (proc == m_currentProcess) {
                    m_currentProcess = nullptr;
                }
                proc->deleteLater();
            });

    connect(proc, &QProcess::errorOccurred, this, [this, proc](QProcess::ProcessError error) {
        if (proc != m_currentProcess) return;

        // Don't report error for intentional kill
        if (error == QProcess::Crashed) {
            return;  // Will be handled by finished signal
        }

        qDebug() << "Process error occurred:" << error;
        QString msg;
        switch (error) {
        case QProcess::FailedToStart: msg = "Failed to start"; break;
        case QProcess::Timedout: msg = "Timed out"; break;
        case QProcess::WriteError: msg = "Write error"; break;
        case QProcess::ReadError: msg = "Read error"; break;
        default: msg = "Unknown error";
        }
        emit errorReceived("[Error] " + msg + "\n");
    });

    // Build run command
    QString runProgram;
    QStringList runArgs;

    if (cfg.compiled) {
        runProgram = execFile;
        qDebug() << "Running compiled executable:" << runProgram;
    } else {
        QString runCmd = cfg.runCommand;

        // ═══════════════════════════════════════════════════════════════
        // FIX: Handle missing {source} in run command
        // ═══════════════════════════════════════════════════════════════
        if (runCmd.contains("{source}")) {
            runCmd.replace("{source}", sourceFile);
        } else {
            // Append source file if not specified in command
            runCmd = runCmd.trimmed() + " " + sourceFile;
        }

        qDebug() << "Final run command:" << runCmd;

        QStringList parts = runCmd.split(' ', Qt::SkipEmptyParts);
        if (parts.isEmpty()) {
            emit errorReceived("[Error] Invalid run command\n");
            emit programFinished(1, 0);
            emit executionFinished();
            return;
        }

        runProgram = parts.takeFirst();
        runArgs = parts;
        qDebug() << "Running interpreter:" << runProgram;
        qDebug() << "Args:" << runArgs;
    }

    // Start execution
    // emit outputReceived("[Running...]\n");
    emit outputReceived("");

    m_currentProcess->start(runProgram, runArgs);

    if (!m_currentProcess->waitForStarted(5000)) {
        qDebug() << "ERROR: Failed to start program!";
        emit errorReceived("[Error] Failed to start: " + runProgram + "\n");
        emit programFinished(1, 0);
        emit executionFinished();
        m_currentProcess->deleteLater();
        m_currentProcess = nullptr;
        return;
    }

    qDebug() << "Program started successfully!";
}

void Backend::sendInput(const QString &input)
{
    qDebug() << "sendInput called:" << input;

    if (m_currentProcess && m_currentProcess->state() == QProcess::Running) {
        QByteArray data = (input + "\n").toUtf8();
        qint64 written = m_currentProcess->write(data);
        qDebug() << "Bytes written:" << written;
    } else {
        qDebug() << "No process running to send input to";
    }
}

void Backend::stopExecution()
{
    qDebug() << "stopExecution called";

    if (m_currentProcess) {
        qDebug() << "Killing current process...";

        // Disconnect signals first to prevent callbacks after deletion
        disconnect(m_currentProcess, nullptr, this, nullptr);

        m_currentProcess->kill();
        m_currentProcess->waitForFinished(1000);
        m_currentProcess->deleteLater();
        m_currentProcess = nullptr;

        // Manually emit finished since we disconnected
        emit programFinished(9, 0);  // 9 = SIGKILL
        emit executionFinished();
    }

    m_runner->stop();
}
