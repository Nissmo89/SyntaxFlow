#ifndef COPILOTMANAGER_H
#define COPILOTMANAGER_H

#include <QObject>
#include <QProcess>
#include <QJsonObject>
#include <QMap>
#include <QTimer>

class CopilotManager : public QObject {
    Q_OBJECT
public:
    explicit CopilotManager(QObject *parent = nullptr);
    ~CopilotManager();

    // Start the Node.js bridge process
    void startBridge(const QString &githubToken);
    
    // Check if ready
    bool isReady() const { return m_ready; }

signals:
    // Signals to frontend JS via QWebChannel
    void completionReceived(const QString &requestId, const QString &completionText);
    void chatReceived(const QString &requestId, const QString &response);
    void errorOccurred(const QString &errorMessage);
    void bridgeReady();

public slots:
    // Slot called by Frontend JS to request a completion
    void requestCompletion(const QString &requestId, const QString &code, const QString &language, int cursorOffset);
    // Slot called by Frontend JS to request a chat response
    void requestChat(const QString &requestId, const QString &prompt, const QJsonObject &context);

private slots:
    void onProcessReadyRead();
    void onProcessError(QProcess::ProcessError error);
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void sendRequest(const QString &method, const QJsonObject &params, const QString &id);

    QProcess *m_process;
    bool m_ready = false;
    QString m_sessionId;
    QString m_lastError;
};

#endif // COPILOTMANAGER_H
