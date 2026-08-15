#include "githubauth.h"
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <keychain.h>

GithubAuth::GithubAuth(QObject *parent) : QObject(parent) {
    m_nam = new QNetworkAccessManager(this);
    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &GithubAuth::pollAccessToken);
    
    // Application-level Client ID for SyntaxFlow desktop app.
    // NOTE: This currently uses the GitHub CLI public Client ID so the auth flow 
    // works immediately for testing. For your final production build, you will 
    // want to substitute this with your own SyntaxFlow Client ID.
    m_clientId = "178c6fc778ccc68e1d6a"; 

    // Attempt to load existing token from secure OS keyring
    auto *job = new QKeychain::ReadPasswordJob("SyntaxFlow", this);
    job->setKey("github_access_token");
    connect(job, &QKeychain::Job::finished, this, [this, job]() {
        if (!job->error()) {
            QString savedToken = job->textData();
            fetchUserProfile(savedToken);
        }
    });
    job->start();
}

void GithubAuth::initiateAuth() {
    QNetworkRequest request(QUrl("https://github.com/login/device/code"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Accept", "application/json");

    QUrlQuery params;
    params.addQueryItem("client_id", m_clientId);
    params.addQueryItem("scope", "read:user");

    QNetworkReply *reply = m_nam->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, &GithubAuth::onDeviceCodeReply);
}

void GithubAuth::onDeviceCodeReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Failed to initiate GitHub login: " + reply->errorString());
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();

    if (obj.contains("error")) {
        emit errorOccurred(obj["error_description"].toString());
        return;
    }

    m_deviceCode = obj["device_code"].toString();
    QString userCode = obj["user_code"].toString();
    QString verificationUri = obj["verification_uri"].toString();
    int interval = obj["interval"].toInt(5);

    emit userCodeReceived(userCode, verificationUri);

    m_pollTimer->start(interval * 1000);
}

void GithubAuth::pollAccessToken() {
    QNetworkRequest request(QUrl("https://github.com/login/oauth/access_token"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Accept", "application/json");

    QUrlQuery params;
    params.addQueryItem("client_id", m_clientId);
    params.addQueryItem("device_code", m_deviceCode);
    params.addQueryItem("grant_type", "urn:ietf:params:oauth:grant-type:device_code");

    QNetworkReply *reply = m_nam->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, &GithubAuth::onAccessTokenReply);
}

void GithubAuth::onAccessTokenReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        // Ignore network errors during polling
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();

    if (obj.contains("error")) {
        QString errorType = obj["error"].toString();
        if (errorType == "authorization_pending") {
            // keep polling
        } else if (errorType == "slow_down") {
            m_pollTimer->setInterval(m_pollTimer->interval() + 5000);
        } else {
            m_pollTimer->stop();
            emit errorOccurred("GitHub login failed: " + obj["error_description"].toString());
        }
        return;
    }

    if (obj.contains("access_token")) {
        m_pollTimer->stop();
        QString token = obj["access_token"].toString();
        
        // Save the token securely to the OS keyring
        auto *job = new QKeychain::WritePasswordJob("SyntaxFlow", this);
        job->setKey("github_access_token");
        job->setTextData(token);
        connect(job, &QKeychain::Job::finished, this, [job]() {
            if (job->error()) {
                qWarning() << "Failed to save token to keyring:" << job->errorString();
            }
        });
        job->start();
        
        emit authenticated(token);
        fetchUserProfile(token);
    }
}

void GithubAuth::fetchUserProfile(const QString &token) {
    QNetworkRequest request(QUrl("https://api.github.com/user"));
    request.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    QNetworkReply *reply = m_nam->get(request);
    connect(reply, &QNetworkReply::finished, this, &GithubAuth::onProfileReply);
}

void GithubAuth::onProfileReply() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) return;
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        // Token is likely invalid or revoked. Delete it from keyring securely.
        auto *job = new QKeychain::DeletePasswordJob("SyntaxFlow", this);
        job->setKey("github_access_token");
        job->start();
        
        emit errorOccurred("Failed to fetch profile (or token invalid): " + reply->errorString());
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject obj = doc.object();

    QString avatarUrl = obj["avatar_url"].toString();
    QString login = obj["login"].toString();
    QString name = obj["name"].toString();
    if (name.isEmpty()) name = login;

    emit profileDataReceived(avatarUrl, login, name);
}
