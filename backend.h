#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "language_config.h"

class LanguageRegistry;
class CodeRunner;

class Backend : public QObject {
    Q_OBJECT

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    // Language info
    QStringList availableLanguages() const;
    QStringList allLanguages() const;
    LanguageConfig getLanguageConfig(const QString &id) const;
    QString getTemplate(const QString &languageId) const;
    bool isLanguageAvailable(const QString &id) const;

    // State
    bool isRunning() const;

public slots:
    // Execution
    void runCode(const QString &code, const QString &languageId, const QString &problemPath);
    void runTestCase(const QString &code, const QString &languageId,
                     int testIndex, const QString &problemPath);
    void stopExecution();

    // Test cases
    void requestTestCases(const QString &problemId);

    // Language management
    bool addLanguage(const LanguageConfig &config);
    bool removeLanguage(const QString &id);
    void reloadLanguages();
    void openConfigDirectory();

signals:
    // Execution results
    void testResult(int testIndex, const QString &status, const QString &output,
                    const QString &expected, qint64 timeMs);
    void compilationError(const QString &error);
    void systemError(const QString &error);
    void executionStarted();
    void executionFinished();
    void progress(int current, int total);

    // Data
    void testCasesReady(const QJsonArray &testCases);

    // Config
    void languagesChanged();

private:
    LanguageRegistry *m_registry;
    CodeRunner *m_runner;
};

#endif // BACKEND_H
