#ifndef WEB_WORKSPACE_H
#define WEB_WORKSPACE_H

#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

class WorkspaceBridge : public QObject {
    Q_OBJECT
public:
    explicit WorkspaceBridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void backRequested();
    void runRequested(int caseIndex);
    void submitRequested();
    void languageChanged(const QString &langId);
    void textChanged(const QString &text);
    void cursorPositionChanged(int line, int col);
    void workspaceReady();

public slots:
    Q_INVOKABLE void onBackRequested() { emit backRequested(); }
    Q_INVOKABLE void onRunRequested(int caseIndex) { emit runRequested(caseIndex); }
    Q_INVOKABLE void onSubmitRequested() { emit submitRequested(); }
    Q_INVOKABLE void onLanguageChanged(const QString &langId) { emit languageChanged(langId); }
    Q_INVOKABLE void onTextChanged(const QString &text) { emit textChanged(text); }
    Q_INVOKABLE void onCursorPositionChanged(int line, int col) { emit cursorPositionChanged(line, col); }
    Q_INVOKABLE void onWorkspaceReady() { emit workspaceReady(); }
};

class WebWorkspace : public QWidget {
    Q_OBJECT
public:
    explicit WebWorkspace(const QUrl &url, QWidget *parent = nullptr);
    ~WebWorkspace() override;

    void setProblemData(const QJsonObject &problemObj);
    void setTestCases(const QJsonArray &testCases);
    void setTestResult(int caseIndex, const QString &status, const QString &actualOutput, const QString &expected, qint64 elapsedMs, bool passed);
    void setTestRunning(int caseIndex);
    void clearAllResults();
    
    void setCode(const QString &code);
    QString code() const;
    
    void setLanguages(const QJsonArray &langs, const QString &currentLang);
    void setLanguage(const QString &langId);

signals:
    void backRequested();
    void runRequested(int caseIndex);
    void submitRequested();
    void languageChanged(const QString &langId);
    void textChanged(const QString &text);

private slots:
    void onBridgeWorkspaceReady();
    void onBridgeTextChanged(const QString &text);

private:
    void runJs(const QString &script);

    QWebEngineView *m_view = nullptr;
    QWebChannel *m_channel = nullptr;
    WorkspaceBridge *m_bridge = nullptr;
    
    bool m_isReady = false;
    QString m_text;
    QJsonObject m_pendingProblemData;
    QJsonArray m_pendingTestCases;
    QJsonArray m_pendingLangs;
    QString m_pendingCurrentLang;
};

#endif // WEB_WORKSPACE_H
