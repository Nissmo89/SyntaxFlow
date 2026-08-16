#include "copilotmanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QDebug>

CopilotManager::CopilotManager(QObject *parent) : QObject(parent) {
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &CopilotManager::onProcessReadyRead);
    connect(m_process, &QProcess::readyReadStandardError, this, [this]() {
        qWarning() << "[Copilot Bridge stderr]" << m_process->readAllStandardError().trimmed();
    });
    connect(m_process, &QProcess::errorOccurred, this, &CopilotManager::onProcessError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &CopilotManager::onProcessFinished);
}

CopilotManager::~CopilotManager() {
    if (m_process->state() == QProcess::Running) {
        sendRequest("stop", QJsonObject(), "shutdown");
        m_process->waitForFinished(1000);
        m_process->kill();
    }
}

void CopilotManager::startBridge(const QString &githubToken) {
    if (m_process->state() != QProcess::NotRunning) return;

    QString appDir = QCoreApplication::applicationDirPath();
    
    QString nodeExecutable = "node";
#ifdef Q_OS_WIN
    QString localNode = QDir(appDir).filePath("node.exe");
    if (QFile::exists(localNode)) {
        nodeExecutable = localNode;
    }
    QString scriptPath = QDir(appDir).filePath("copilot_bridge/bridge.js");
#else
    QString scriptPath = QDir(appDir).filePath("../copilot_bridge/bridge.js");
    if (!QFile::exists(scriptPath)) {
        scriptPath = QDir(appDir).filePath("copilot_bridge/bridge.js");
    }
#endif

    m_process->start(nodeExecutable, QStringList() << scriptPath);
    if (!m_process->waitForStarted()) {
        qWarning() << "Failed to start Node.js for Copilot bridge!";
        emit errorOccurred("Node.js is not installed or not in PATH.");
        return;
    }

    // Initialize the SDK with the token
    QJsonObject params;
    params["token"] = githubToken;
    sendRequest("init", params, "init_req");
}

void CopilotManager::sendRequest(const QString &method, const QJsonObject &params, const QString &id) {
    QJsonObject req;
    req["id"] = id;
    req["method"] = method;
    req["params"] = params;

    QJsonDocument doc(req);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + "\n";
    m_process->write(data);
}

void CopilotManager::requestCompletion(const QString &requestId, const QString &code, const QString &language, int cursorOffset) {
    if (!m_ready) {
        emit errorOccurred("Copilot is not ready yet.");
        return;
    }

    QJsonObject params;
    params["code"] = code;
    params["language"] = language;
    params["cursorOffset"] = cursorOffset;
    if (!m_sessionId.isEmpty()) {
        params["sessionId"] = m_sessionId;
    }

    sendRequest("get_completion", params, requestId);
}

void CopilotManager::requestChat(const QString &requestId, const QString &prompt, const QString &codeContext, const QString &languageContext) {
    if (!m_ready) {
        emit errorOccurred("Copilot is not ready yet.");
        return;
    }

    QJsonObject params;
    params["prompt"] = prompt;
    params["codeContext"] = codeContext;
    params["languageContext"] = languageContext;
    if (!m_sessionId.isEmpty()) {
        params["sessionId"] = m_sessionId;
    }

    sendRequest("chat", params, requestId);
}

void CopilotManager::onProcessReadyRead() {
    while (m_process->canReadLine()) {
        QByteArray line = m_process->readLine().trimmed();
        if (line.isEmpty()) continue;

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError) {
            qWarning() << "CopilotManager failed to parse JSON:" << err.errorString() << line;
            continue;
        }

        QJsonObject obj = doc.object();
        QString id = obj["id"].toString();
        
        if (obj.contains("error") && !obj["error"].isNull()) {
            emit errorOccurred(obj["error"].toString());
            continue;
        }

        QJsonObject result = obj["result"].toObject();

        if (id == "init_req") {
            // Once initialized, create a session
            sendRequest("create_session", QJsonObject(), "session_req");
        } else if (id == "session_req") {
            m_sessionId = result["sessionId"].toString();
            m_ready = true;
            emit bridgeReady();
        } else {
            // It's a completion or chat request response
            if (result.contains("completion")) {
                QString completionText = "";
                if (result["completion"].isObject()) {
                    QJsonObject compObj = result["completion"].toObject();
                }
                emit completionReceived(id, completionText);
            } else if (result.contains("chat")) {
                emit chatReceived(id, result["chat"].toString());
            }
        }
    }
}

void CopilotManager::onProcessError(QProcess::ProcessError error) {
    qWarning() << "Copilot bridge process error:" << error;
}

void CopilotManager::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    qWarning() << "Copilot bridge process finished:" << exitCode << exitStatus;
    m_ready = false;
}
