#ifndef CODE_RUNNER_H
#define CODE_RUNNER_H

#include <QObject>
#include <QJsonArray>
#include "language_config.h"
#include "tcc_compiler.h"
#ifdef SF_PYTHON_ENABLED
#include "python_runner.h"
#endif
#include "quickjs_runner.h"
#include "driver_generator.h"

class LanguageRegistry;

class CodeRunner : public QObject {
    Q_OBJECT

public:
    explicit CodeRunner(LanguageRegistry *registry, QObject *parent = nullptr);
    ~CodeRunner() override;

    void runCode(const QString &code, const QString &languageId, const QString &problemId);
    void runSingleTest(const QString &code, const QString &languageId,
                       int testIndex, const QString &problemId);
    void stop();
    bool isRunning() const { return m_running; }
    
    QString getProblemsPath(const QString &problemId) const;

    void executeTestRaw(EmbeddedRunner* runner, const QString &code,
                        const QJsonObject &test,
                        int index);

    void runFreeCode(const QString &code, const QString &languageId);

signals:
    void testResult(int testIndex, const QString &status, const QString &output,
                    const QString &expected, qint64 timeMs);
    void compilationError(const QString &error);
    void systemError(const QString &error);
    void started();
    void finished();
    void progress(int current, int total);

    void freeCodeOutput(const QString &output);
    void freeCodeError(const QString &error);
    void freeCodeFinished(int exitCode, qint64 ms);

private:
    LanguageRegistry *m_registry;
    bool m_running = false;
    bool m_stopRequested = false;


    bool loadTestCases(const QString &problemId, QJsonArray &tests, MethodSchema &schema);

    EmbeddedRunner* getRunner(const QString &languageId);

    TccRunner *m_tccRunner = nullptr;
    QuickJsRunner *m_quickjsRunner = nullptr;
#ifdef SF_PYTHON_ENABLED
    PythonRunner *m_pythonRunner = nullptr;
#endif
};

#endif // CODE_RUNNER_H
