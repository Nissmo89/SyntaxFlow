#include "code_runner.h"
#include "language_registry.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDateTime>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QDebug>
#include "output_normalizer.h"
CodeRunner::CodeRunner(LanguageRegistry *registry, QObject *parent)
    : QObject(parent), m_registry(registry) {}

void CodeRunner::runCode(const QString &code, const QString &languageId, const QString &problemPath) {
    if (m_running) {
        emit systemError("Already running");
        return;
    }

    m_running = true;
    m_stopRequested = false;
    emit started();

    // Get config
    LanguageConfig cfg = m_registry->getConfig(languageId);
    if (!cfg.isValid()) {
        emit systemError("Invalid language: " + languageId);
        m_running = false;
        emit finished();
        return;
    }

    if (!m_registry->isLanguageAvailable(languageId)) {
        emit systemError(cfg.name + " not available. Install " +
                         (cfg.compiled ? cfg.compileCommand : cfg.runCommand));
        m_running = false;
        emit finished();
        return;
    }

    // Setup
    QString dir = createWorkDir(languageId);
    if (dir.isEmpty()) {
        emit systemError("Failed to create temp directory");
        m_running = false;
        emit finished();
        return;
    }
    m_workDir = dir;

    if (!writeSource(dir, code, cfg)) {
        emit systemError("Failed to write source file");
        cleanup(dir);
        m_running = false;
        emit finished();
        return;
    }

    // Compile
    if (cfg.compiled) {
        QString error;
        if (!compile(dir, cfg, error)) {
            emit compilationError(error);
            cleanup(dir);
            m_running = false;
            emit finished();
            return;
        }
    }

    // Load tests
    QJsonArray tests;
    if (!loadTestCases(problemPath, tests)) {
        emit systemError("Failed to load test cases from: " + problemPath);
        cleanup(dir);
        m_running = false;
        emit finished();
        return;
    }

    qDebug() << "Loaded" << tests.size() << "test cases";

    // Run tests
    for (int i = 0; i < tests.size() && !m_stopRequested; ++i) {
        emit progress(i + 1, tests.size());
        executeTest(dir, cfg, tests[i].toObject(), i);
    }

    cleanup(dir);
    m_running = false;
    emit finished();
}

void CodeRunner::runSingleTest(const QString &code, const QString &languageId,
                               int testIndex, const QString &problemPath) {
    if (m_running) {
        emit systemError("Already running");
        return;
    }

    m_running = true;
    m_stopRequested = false;
    emit started();

    LanguageConfig cfg = m_registry->getConfig(languageId);
    if (!cfg.isValid() || !m_registry->isLanguageAvailable(languageId)) {
        emit systemError("Language not available: " + languageId);
        m_running = false;
        emit finished();
        return;
    }

    QString dir = createWorkDir(languageId);
    if (dir.isEmpty() || !writeSource(dir, code, cfg)) {
        emit systemError("Setup failed");
        m_running = false;
        emit finished();
        return;
    }
    m_workDir = dir;

    if (cfg.compiled) {
        QString error;
        if (!compile(dir, cfg, error)) {
            emit compilationError(error);
            cleanup(dir);
            m_running = false;
            emit finished();
            return;
        }
    }

    QJsonArray tests;
    if (!loadTestCases(problemPath, tests)) {
        qDebug() << "Failed to load test cases from:" << problemPath;
        emit systemError("Failed to load test cases from: " + problemPath);
        cleanup(dir);
        m_running = false;
        emit finished();
        return;
    }

    if (testIndex < 0 || testIndex >= tests.size()) {
        qDebug() << "Test index" << testIndex << "out of range. Total tests:" << tests.size();
        emit systemError("Test index " + QString::number(testIndex) +
                         " out of range (0-" + QString::number(tests.size() - 1) + ")");
        cleanup(dir);
        m_running = false;
        emit finished();
        return;
    }

    executeTest(dir, cfg, tests[testIndex].toObject(), testIndex);

    cleanup(dir);
    m_running = false;
    emit finished();
}

void CodeRunner::stop() {
    m_stopRequested = true;
    if (m_currentProcess && m_currentProcess->state() == QProcess::Running) {
        m_currentProcess->kill();
    }
}

QString CodeRunner::createWorkDir(const QString &langId) {
    QString temp = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString dir = temp + "/CodeHour_" + langId + "_" +
                  QString::number(QDateTime::currentMSecsSinceEpoch());

    if (!QDir().mkpath(dir)) return "";
    return dir;
}

bool CodeRunner::writeSource(const QString &dir, const QString &code, const LanguageConfig &cfg) {
    QFile file(dir + "/" + cfg.sourceFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    file.write(code.toUtf8());
    file.close();
    return true;
}

bool CodeRunner::compile(const QString &dir, const LanguageConfig &cfg, QString &error) {
    QProcess compiler;
    compiler.setWorkingDirectory(dir);

    QString cmd = cfg.expand(cfg.compileCommand, dir);
    QStringList args = cfg.expandArgs(cfg.compileArgs, dir);

    qDebug() << "Compiling:" << cmd << args;

    compiler.start(cmd, args);

    if (!compiler.waitForStarted(5000)) {
        error = "Failed to start compiler: " + cmd;
        return false;
    }

    if (!compiler.waitForFinished(cfg.compileTimeout)) {
        compiler.kill();
        error = "Compilation timed out";
        return false;
    }

    if (compiler.exitCode() != 0) {
        error = QString(compiler.readAllStandardError());
        if (error.isEmpty()) error = QString(compiler.readAllStandardOutput());
        return false;
    }

    qDebug() << "Compilation successful";
    return true;
}

void CodeRunner::executeTest(const QString &dir, const LanguageConfig &cfg,
                             const QJsonObject &test, int index) {
    // Handle both array and string input formats
    QString inputStr;

    if (test.contains("input")) {
        QJsonValue inputVal = test["input"];
        if (inputVal.isArray()) {
            QJsonArray inputs = inputVal.toArray();
            for (const auto &arg : inputs) {
                inputStr += arg.toString() + "\n";
            }
        } else if (inputVal.isString()) {
            inputStr = inputVal.toString();
            if (!inputStr.endsWith("\n")) {
                inputStr += "\n";
            }
        }
    }

    QString expected = test["output"].toString();

    qDebug() << "Test" << index << "- Input:" << inputStr.trimmed() << "Expected:" << expected;

    QProcess runner;
    m_currentProcess = &runner;
    runner.setWorkingDirectory(dir);

    // Setup environment
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    for (auto it = cfg.environment.begin(); it != cfg.environment.end(); ++it) {
        env.insert(it.key(), cfg.expand(it.value(), dir));
    }
    runner.setProcessEnvironment(env);

    QString cmd = cfg.expand(cfg.runCommand, dir);
    QStringList args = cfg.expandArgs(cfg.runArgs, dir);

    qDebug() << "Running:" << cmd << args;

    QElapsedTimer timer;
    timer.start();

    runner.start(cmd, args);

    if (!runner.waitForStarted(5000)) {
        emit testResult(index, "Runtime Error", "Failed to start: " + runner.errorString(), expected, 0);
        m_currentProcess = nullptr;
        return;
    }

    if (!inputStr.isEmpty()) {
        runner.write(inputStr.toUtf8());
        runner.closeWriteChannel();
    }

    bool finished = runner.waitForFinished(cfg.timeout);
    qint64 timeTaken = timer.elapsed();

    QString status;
    QString output;

    if (m_stopRequested) {
        runner.kill();
        status = "Stopped";
        output = "Stopped by user";
    } else if (!finished || runner.state() == QProcess::Running) {
        runner.kill();
        runner.waitForFinished(1000);
        status = "Time Limit Exceeded";
        output = "";
    } else if (runner.exitCode() != 0) {
        status = "Runtime Error";
        output = QString(runner.readAllStandardError());
        if (output.isEmpty()) output = QString(runner.readAllStandardOutput());
    } else {
        QString actual = QString(runner.readAllStandardOutput()).trimmed();
        // if (actual == expected.trimmed()) {
        if (OutputNormalizer::equals(actual, expected)) {
            status = "Accepted";
        } else {
            status = "Wrong Answer";
        }
        output = actual;
    }

    qDebug() << "Test" << index << "result:" << status << "in" << timeTaken << "ms";

    m_currentProcess = nullptr;
    emit testResult(index, status, output, expected, timeTaken);
}

void CodeRunner::cleanup(const QString &dir) {
    if (!dir.isEmpty() && dir.contains("CodeHour_")) {
        QDir(dir).removeRecursively();
    }
    m_workDir.clear();
}

bool CodeRunner::loadTestCases(const QString &problemPath, QJsonArray &tests) {
    qDebug() << "Loading test cases from:" << problemPath;

    // Try the path directly first (it might be a full path)
    if (QFile::exists(problemPath)) {
        QFile file(problemPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            tests = doc.object()["testCases"].toArray();
            qDebug() << "Loaded" << tests.size() << "test cases from:" << problemPath;
            return !tests.isEmpty();
        }
    }

    // Try as a problem ID with various base paths
    QString resolvedPath = getProblemsPath(problemPath);
    if (!resolvedPath.isEmpty()) {
        QFile file(resolvedPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            tests = doc.object()["testCases"].toArray();
            qDebug() << "Loaded" << tests.size() << "test cases from:" << resolvedPath;
            return !tests.isEmpty();
        }
    }

    qDebug() << "Could not find problem file for:" << problemPath;
    return false;
}

QString CodeRunner::getProblemsPath(const QString &problemId) const {
    // Try multiple path patterns
    QStringList relPaths = {
        "/data/problems/" + problemId + ".json",
        "/problems/" + problemId + ".json",
        "/problems/easy/" + problemId + ".json",
        "/problems/medium/" + problemId + ".json",
        "/problems/hard/" + problemId + ".json",
        problemId + ".json",
        problemId
    };

    QStringList basePaths = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QCoreApplication::applicationDirPath() + "/..",
        QCoreApplication::applicationDirPath() + "/../..",
        QCoreApplication::applicationDirPath() + "/../../.."
    };

    for (const QString &base : basePaths) {
        for (const QString &rel : relPaths) {
            QString full = QDir(base).cleanPath(base + rel);
            if (QFile::exists(full)) {
                qDebug() << "Found problem file at:" << full;
                return full;
            }
        }
    }

    return "";
}
