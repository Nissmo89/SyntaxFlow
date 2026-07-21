#ifndef NETWORKBROWSER_H
#define NETWORKBROWSER_H

#include <QTextBrowser>
#include <QNetworkAccessManager>
#include <QSet>
#include <QUrl>
#include <QHash>
#include <QImage>
#include <QFileSystemWatcher>

class NetworkBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit NetworkBrowser(QWidget *parent = nullptr);
    ~NetworkBrowser() override = default;

    void setProblemHtml(const QString &html);

protected:
    QVariant loadResource(int type, const QUrl &name) override;

private:
    void loadStyleFile();
    void applyStyle(const QString &cssContent);

    QNetworkAccessManager *m_networkManager;
    QSet<QUrl> m_pendingDownloads;
    QHash<QUrl, QImage> m_loadedImages;
    QString m_cacheDir;

    QFileSystemWatcher *m_watcher;
    QString m_cssPath;
    QString m_cssContent;
    QString m_currentHtml;
};

#endif // NETWORKBROWSER_H
