#ifndef GITHUBAUTH_H
#define GITHUBAUTH_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>

class GithubAuth : public QObject {
    Q_OBJECT
public:
    explicit GithubAuth(QObject *parent = nullptr);
    void initiateAuth();
    void fetchUserProfile(const QString &token);

signals:
    void userCodeReceived(const QString &userCode, const QString &verificationUri);
    void authenticated(const QString &accessToken);
    void profileDataReceived(const QString &avatarUrl, const QString &login, const QString &name);
    void errorOccurred(const QString &errorMsg);

private slots:
    void onDeviceCodeReply();
    void pollAccessToken();
    void onAccessTokenReply();
    void onProfileReply();

private:
    QNetworkAccessManager *m_nam;
    QString m_clientId;
    QString m_deviceCode;
    QTimer *m_pollTimer;
};

#endif // GITHUBAUTH_H
