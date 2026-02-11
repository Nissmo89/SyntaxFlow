#ifndef CODE_RUNNER_H
#define CODE_RUNNER_H

#include <QObject>
#include <QProcess>
#include <QJsonArray>
#include "language_config.h"

class LanguageRegistry;

class CodeRunner : public QObject {
    Q_OBJECT

public:
    explicit CodeRunner(LanguageRegistry *registry, QObject *parent = nullptr);

    void runCode(const QString &code, const QString &languageId, const QString &problemId);
    void runSingleTest(const QString &code, const QString &languageId,
                       int testIndex, const QString &problemId);
    void stop();
    bool isRunning() const { return m_running; }

signals:
    void testResult(int testIndex, const QString &status, const QString &output,
                    const QString &expected, qint64 timeMs);
    void compilationError(const QString &error);
    void systemError(const QString &error);
    void started();
    void finished();
    void progress(int current, int total);

private:
    LanguageRegistry *m_registry;
    QProcess *m_currentProcess = nullptr;
    QString m_workDir;
    bool m_running = false;
    bool m_stopRequested = false;

    QString createWorkDir(const QString &langId);
    bool writeSource(const QString &dir, const QString &code, const LanguageConfig &cfg);
    bool compile(const QString &dir, const LanguageConfig &cfg, QString &error);
    void executeTest(const QString &dir, const LanguageConfig &cfg,
                     const QJsonObject &test, int index);
    void cleanup(const QString &dir);

    bool loadTestCases(const QString &problemId, QJsonArray &tests);
    QString getProblemsPath(const QString &problemId) const;
};

#endif // CODE_RUNNER_H
