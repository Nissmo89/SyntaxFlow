#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QStringList>
#include "language_registry.h"

class CodeRunner;

class Backend : public QObject
{
    Q_OBJECT

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    // Existing methods...
    QStringList availableLanguages() const;
    QStringList allLanguages() const;
    LanguageConfig getLanguageConfig(const QString &id) const;
    QString getTemplate(const QString &languageId) const;
    QString getProblemTemplate(const QString &languageId, const QString &problemId) const;
    bool isLanguageAvailable(const QString &id) const;
    bool isRunning() const;

    void runCode(const QString &code, const QString &languageId, const QString &problemId);
    void runTestCase(const QString &code, const QString &languageId,
                     int testIndex, const QString &problemId);
    void stopExecution();
    void requestTestCases(const QString &problemId);

    bool addLanguage(const LanguageConfig &config);
    bool removeLanguage(const QString &id);
    void reloadLanguages();
    void openConfigDirectory();

    // ═══════════════════════════════════════════════════════════════════
    // NEW: For Code Labs free execution
    // ═══════════════════════════════════════════════════════════════════
    void runFreeCode(const QString &code, const QString &languageId);

signals:
    // Existing signals
    void testResult(int testIndex, const QString &status,
                    const QString &output, const QString &expected, qint64 timeMs);
    void compilationError(const QString &error);
    void systemError(const QString &error);
    void executionStarted();
    void executionFinished();
    void languagesChanged();
    void testCasesReady(const QJsonArray &testCases);
    void progress(int current, int total);

    // ═══════════════════════════════════════════════════════════════════
    // NEW: Signals for interactive I/O (Code Labs)
    // ═══════════════════════════════════════════════════════════════════
    void outputReceived(const QString &output);
    void errorReceived(const QString &error);
    void programFinished(int exitCode, qint64 elapsedMs);

public slots:
    // ═══════════════════════════════════════════════════════════════════
    // NEW: Send input to running process
    // ═══════════════════════════════════════════════════════════════════
    void sendInput(const QString &input);

private:
    LanguageRegistry *m_registry;
    CodeRunner *m_runner;

};

#endif // BACKEND_H
