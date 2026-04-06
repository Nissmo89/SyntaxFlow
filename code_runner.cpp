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
#include "driver_generator.h"

CodeRunner::CodeRunner(LanguageRegistry *registry, QObject *parent)
    : QObject(parent), m_registry(registry) {

    m_tccRunner     = new TccRunner();
    m_quickjsRunner = new QuickJsRunner();
#ifdef SF_PYTHON_ENABLED
    m_pythonRunner = new PythonRunner();
#endif
}

CodeRunner::~CodeRunner() {
    delete m_tccRunner;
    delete m_quickjsRunner;
#ifdef SF_PYTHON_ENABLED
    delete m_pythonRunner;
#endif
}

EmbeddedRunner* CodeRunner::getRunner(const QString &languageId) {
    // libtcc — C only (C89/C99, no C++ support)
    if (languageId == "c") return m_tccRunner;

    // C++ is not supported in embedded mode: TCC cannot compile C++
    // and no external compiler fallback exists (by design).
    if (languageId == "cpp") return nullptr;

    // QuickJS — JavaScript (works on all platforms, no external tools)
    if (languageId == "javascript" || languageId == "js") return m_quickjsRunner;

#ifdef SF_PYTHON_ENABLED
    // CPython — Python (optional, requires python3-dev at build time)
    if (languageId == "python") return m_pythonRunner;
#endif
    return nullptr;
}

void CodeRunner::runCode(const QString &code, const QString &languageId, const QString &problemPath) {
    if (m_running) {
        emit systemError("Already running");
        return;
    }

    m_running = true;
    m_stopRequested = false;
    emit started();

    QJsonArray tests;
    MethodSchema schema;
    if (!loadTestCases(problemPath, tests, schema)) {
        emit systemError("Failed to load test cases from: " + problemPath);
        m_running = false;
        emit finished();
        return;
    }

    qDebug() << "Loaded" << tests.size() << "test cases";

    EmbeddedRunner *runner = getRunner(languageId);
    if (!runner) {
        QString msg = (languageId == "cpp")
            ? QStringLiteral("C++ is not supported. TCC only compiles C, and no external compiler is used.")
            : (QStringLiteral("Language not yet configured for embedded compilation: ") + languageId);
        emit systemError(msg);
        m_running = false;
        emit finished();
        return;
    }
    
    QString fullCode = code;
    if (schema.isValid()) {
        fullCode += DriverGenerator::generateDriver(schema, languageId);
    }

    for (int i = 0; i < tests.size() && !m_stopRequested; ++i) {
        emit progress(i + 1, tests.size());
        executeTestRaw(runner, fullCode, tests[i].toObject(), i);
    }

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

    QJsonArray tests;
    MethodSchema schema;
    if (!loadTestCases(problemPath, tests, schema)) {
        qDebug() << "Failed to load test cases from:" << problemPath;
        emit systemError("Failed to load test cases from: " + problemPath);
        m_running = false;
        emit finished();
        return;
    }

    if (testIndex < 0 || testIndex >= tests.size()) {
        qDebug() << "Test index" << testIndex << "out of range. Total tests:" << tests.size();
        emit systemError("Test index " + QString::number(testIndex) +
                         " out of range (0-" + QString::number(tests.size() - 1) + ")");
        m_running = false;
        emit finished();
        return;
    }

    EmbeddedRunner *runner = getRunner(languageId);
    if (!runner) {
        QString msg = (languageId == "cpp")
            ? QStringLiteral("C++ is not supported. TCC only compiles C, and no external compiler is used.")
            : (QStringLiteral("Language not yet configured for embedded compilation: ") + languageId);
        emit systemError(msg);
        m_running = false;
        emit finished();
        return;
    }

    QString fullCode = code;
    if (schema.isValid()) {
        fullCode += DriverGenerator::generateDriver(schema, languageId);
    }

    executeTestRaw(runner, fullCode, tests[testIndex].toObject(), testIndex);

    m_running = false;
    emit finished();
}







void CodeRunner::runFreeCode(const QString &code, const QString &languageId) {
    // BUG-09: guard against concurrent execution (same guard as runCode/runSingleTest)
    if (m_running) {
        emit systemError("Already running");
        return;
    }
    m_running = true;
    m_stopRequested = false;

    EmbeddedRunner *runner = getRunner(languageId);
    if (!runner) {
        QString msg = (languageId == "cpp")
            ? QStringLiteral("C++ is not supported. TCC only compiles C, and no external compiler is used.")
            : (QStringLiteral("Language not yet configured for free execution: ") + languageId);
        emit systemError(msg);
        m_running = false;
        emit finished();
        return;
    }

    QElapsedTimer timer;
    timer.start();
    EmbeddedRunner::Result r = runner->execute(code, "", &m_stopRequested);
    qint64 ms = timer.elapsed();

    if (!r.output.isEmpty()) emit freeCodeOutput(r.output);
    if (!r.error.isEmpty())  emit freeCodeError(r.error);
    emit freeCodeFinished(r.exitCode, ms);

    m_running = false;
    emit finished();
}

void CodeRunner::stop() {
    m_stopRequested = true;
}

bool CodeRunner::loadTestCases(const QString &problemPath, QJsonArray &tests, MethodSchema &schema) {
    qDebug() << "Loading test cases from:" << problemPath;
    
    auto parseDoc = [&](const QJsonDocument& doc) {
        tests = doc.object()["testCases"].toArray();
        if (doc.object().contains("method")) {
            schema = MethodSchema::fromJson(doc.object()["method"].toObject());
        }
        return !tests.isEmpty();
    };

    // Try the path directly first (it might be a full path)
    if (QFile::exists(problemPath)) {
        QFile file(problemPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            bool hasTests = parseDoc(doc);
            qDebug() << "Loaded" << tests.size() << "test cases from:" << problemPath;
            return hasTests;
        }
    }

    QString resolvedPath = getProblemsPath(problemPath);
    if (!resolvedPath.isEmpty()) {
        QFile file(resolvedPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            bool hasTests = parseDoc(doc);
            qDebug() << "Loaded" << tests.size() << "test cases from:" << resolvedPath;
            return hasTests;
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


void CodeRunner::executeTestRaw(EmbeddedRunner* runner, const QString &code,
                                const QJsonObject &test,
                                int index)
{
    QString input    = test["input"].toString();
    QString expected = test["expected"].toString().trimmed();

    QElapsedTimer timer;
    timer.start();
    
    EmbeddedRunner::Result r = runner->execute(code, input, &m_stopRequested);
    
    qint64 elapsedMs = timer.elapsed();

    QString actual = r.output.trimmed();
    QString status;

    if (r.exitCode != 0 && actual.isEmpty()) {
        status = "Runtime Error";
        actual = r.error.trimmed();
        if (actual.isEmpty()) {
            actual = QString("Process exited with code %1").arg(r.exitCode);
            if (!r.error.isEmpty()) {
                actual += "\nError: " + r.error;
            } else if (r.exitCode == -1 && runner->languageId() == "c") {
                actual += "\n(Check if 'main' function is defined, or if there was a relocation error)";
            }
        }
    } else if (r.timedOut) {
        status = "Time Limit Exceeded";
    } else {
        // BUG-10: surface compile/parse errors for any language, not just C.
        // Heuristic: non-zero exit with no stdout and non-empty stderr = compile error.
        if (r.exitCode != 0 && actual.isEmpty() && !r.error.isEmpty()) {
            emit compilationError(r.error);
            return;
        }

        if (r.exitCode != 0) {
            status = "Runtime Error";
        } else {
            status = OutputNormalizer::equals(actual, expected) ? "Accepted" : "Wrong Answer";
        }
    }

    emit testResult(index, status, actual, expected, elapsedMs);
}
