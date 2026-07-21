#include "networkbrowser.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScrollBar>
#include <QTimer>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QDebug>

NetworkBrowser::NetworkBrowser(QWidget *parent)
    : QTextBrowser(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    
    // Create disk cache path for downloaded images
    m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/qtext_browser_cache";
    QDir().mkpath(m_cacheDir);

    // Load custom fonts dynamically (only once per application run)
    static bool fontsLoaded = false;
    if (!fontsLoaded) {
        QString fontsPath = QCoreApplication::applicationDirPath() + "/fonts";
        if (!QDir(fontsPath).exists()) {
            fontsPath = QDir::currentPath() + "/fonts";
        }
        if (!QDir(fontsPath).exists()) {
            fontsPath = QDir::currentPath() + "/libs/qtext_browser/fonts";
        }

        QVector<QString> fontFiles = {
            "/Inter[opsz,wght].ttf",
            "/JetBrainsMono-Regular.ttf",
            "/JetBrainsMono-Bold.ttf"
        };
        for (const QString &file : fontFiles) {
            QString fullPath = fontsPath + file;
            int fontId = QFontDatabase::addApplicationFont(fullPath);
            if (fontId == -1) {
                qDebug() << "Failed to load custom font file:" << fullPath;
            } else {
                qDebug() << "Loaded custom font file:" << fullPath;
            }
        }
        fontsLoaded = true;
    }

    // Locate style.css robustly
    m_cssPath = QCoreApplication::applicationDirPath() + "/style.css";
    if (!QFile::exists(m_cssPath)) {
        m_cssPath = QDir::currentPath() + "/style.css";
    }
    if (!QFile::exists(m_cssPath)) {
        m_cssPath = QDir::currentPath() + "/libs/qtext_browser/style.css";
    }

    loadStyleFile();

    // Setup style watcher for Hot Reload
    m_watcher = new QFileSystemWatcher(this);
    if (QFile::exists(m_cssPath)) {
        m_watcher->addPath(m_cssPath);
        connect(m_watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString &path) {
            QTimer::singleShot(100, this, [this, path]() {
                loadStyleFile();
                if (m_watcher) {
                    m_watcher->removePath(path);
                    m_watcher->addPath(path);
                }
            });
        });
    }
}

void NetworkBrowser::setProblemHtml(const QString &html)
{
    m_currentHtml = html;

    // Fix the missing CSS semicolon in border-radius
    m_currentHtml.replace("border-radius:3px margin-top: 0;", "border-radius:3px; margin-top: 0;");

    // Remove the ugly plain-text asterisks separators (5 or more asterisks)
    m_currentHtml.replace(QRegularExpression("\\*{5,}"), "");

    // Remove all original inline style blocks to avoid conflicts with our stylesheet
    m_currentHtml.replace(QRegularExpression("<style>.*?</style>", QRegularExpression::DotMatchesEverythingOption), "");

    // Add code-tag class to all <code> elements for stylesheet targeting
    m_currentHtml.replace(QRegularExpression("<code[^>]*>"), "<code class=\"code-tag\">");

    applyStyle(m_cssContent);
}

void NetworkBrowser::loadStyleFile()
{
    QFile cssFile(m_cssPath);
    if (cssFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_cssContent = QString::fromUtf8(cssFile.readAll());
        if (!m_currentHtml.isEmpty()) {
            applyStyle(m_cssContent);
        }
    } else {
        qDebug() << "Failed to open CSS file:" << m_cssPath;
    }
}

void NetworkBrowser::applyStyle(const QString &cssContent)
{
    QString qssPart = cssContent;
    QString htmlPart = "";
    
    int index = cssContent.indexOf("/* === HTML === */");
    if (index != -1) {
        qssPart = cssContent.left(index).trimmed();
        htmlPart = cssContent.mid(index + QString("/* === HTML === */").length()).trimmed();
    }
    
    // Apply widget QSS (handles rounded corners and border of the text browser widget itself)
    this->setStyleSheet(qssPart);
    
    // Set the HTML CSS part on the document's defaultStyleSheet before loading the HTML
    this->document()->setDefaultStyleSheet(htmlPart);
    
    // Save current scroll position
    int scrollValue = this->verticalScrollBar()->value();
    
    // Load the HTML content wrapped in body tags to ensure it is parsed as a full document
    this->setHtml("<html><body>" + m_currentHtml + "</body></html>");
    
    // Restore scroll position
    this->verticalScrollBar()->setValue(scrollValue);
}

QVariant NetworkBrowser::loadResource(int type, const QUrl &name)
{
    if (type == QTextDocument::ImageResource && name.scheme().startsWith("http")) {
        // 1. Check in-memory resource cache first
        if (m_loadedImages.contains(name)) {
            return m_loadedImages.value(name);
        }

        // 2. Check local disk cache
        QString cacheKey = QCryptographicHash::hash(name.toString().toUtf8(), QCryptographicHash::Sha256).toHex();
        QString cacheFilePath = m_cacheDir + "/" + cacheKey;
        
        if (QFile::exists(cacheFilePath)) {
            QFile file(cacheFilePath);
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray data = file.readAll();
                QImage img = QImage::fromData(data);
                if (!img.isNull()) {
                    m_loadedImages.insert(name, img);
                    document()->addResource(QTextDocument::ImageResource, name, img);
                    return img;
                }
            }
        }

        // 3. Download asynchronously if not already downloading
        if (!m_pendingDownloads.contains(name)) {
            m_pendingDownloads.insert(name);
            QNetworkRequest request(name);
            request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
            request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
            
            QNetworkReply *reply = m_networkManager->get(request);
            connect(reply, &QNetworkReply::finished, this, [this, name, reply, cacheFilePath]() {
                reply->deleteLater();
                m_pendingDownloads.remove(name);
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray data = reply->readAll();
                    QImage img = QImage::fromData(data);
                    if (!img.isNull()) {
                        // Save to disk cache
                        QFile file(cacheFilePath);
                        if (file.open(QIODevice::WriteOnly)) {
                            file.write(data);
                        }
                        // Cache locally
                        m_loadedImages.insert(name, img);
                        // Add to document resource
                        document()->addResource(QTextDocument::ImageResource, name, img);
                        
                        // Force a re-layout without altering scroll position
                        document()->setPageSize(document()->pageSize());
                        viewport()->update();
                    }
                } else {
                    qDebug() << "Failed to download image:" << name << reply->errorString();
                }
            });
        }

        // Return a 1x1 transparent placeholder so spacing is allocated
        static QImage placeholder;
        if (placeholder.isNull()) {
            placeholder = QImage(1, 1, QImage::Format_ARGB32);
            placeholder.fill(Qt::transparent);
        }
        return placeholder;
    }
    return QTextBrowser::loadResource(type, name);
}
