#include "problemwidgets.h"
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QUrl>
#include <QWebEngineSettings>
#include <QDebug>

ProblemBrowser::ProblemBrowser(const QUrl &url, ProgressManager *pm, QWidget *parent)
    : QWidget(parent)
    , progressManager(pm)
{
    if (progressManager) {
        progressManager->load();
    }

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_view = new QWebEngineView(this);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    m_channel = new QWebChannel(m_view->page());
    m_bridge = new ProblemBrowserBridge(this);

    m_channel->registerObject("browserBridge", m_bridge);
    m_view->page()->setWebChannel(m_channel);

    layout->addWidget(m_view);

    connect(m_bridge, &ProblemBrowserBridge::openProblemRequested,
            this, &ProblemBrowser::navigateToEditor);
    connect(m_bridge, &ProblemBrowserBridge::browserReady,
            this, &ProblemBrowser::onBridgeBrowserReady);

    connect(m_view, &QWebEngineView::loadFinished, this, [this](bool ok) {
        if (ok) {
            m_isWebReady = true;
            syncToWeb();
        }
    });

    if (progressManager) {
        connect(progressManager, &ProgressManager::progressChanged,
                this, &ProblemBrowser::onProgressChanged);
    }

    m_view->setUrl(url);
}

ProblemBrowser::~ProblemBrowser() = default;

void ProblemBrowser::onBridgeBrowserReady()
{
    m_isWebReady = true;
    syncToWeb();
}

QJsonObject ProblemBrowser::getSolvedMap() const
{
    QJsonObject solvedMap;
    if (!progressManager) return solvedMap;

    for (const QJsonValue &val : m_problemsArray) {
        QString id = val.toObject()["id"].toString();
        if (!id.isEmpty()) {
            solvedMap[id] = progressManager->isSolved(id);
        }
    }
    return solvedMap;
}

void ProblemBrowser::syncToWeb()
{
    if (!m_isWebReady) return;

    QString problemsJson = QJsonDocument(m_problemsArray).toJson(QJsonDocument::Compact);
    QString solvedJson = QJsonDocument(getSolvedMap()).toJson(QJsonDocument::Compact);

    m_view->page()->runJavaScript(
        QString("window.loadProblems(%1, %2);").arg(problemsJson, solvedJson)
    );
}

void ProblemBrowser::loadFromJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    m_problemsArray = doc.array();

    syncToWeb();
}

void ProblemBrowser::onProgressChanged(const QString &)
{
    if (!m_isWebReady) return;
    QString solvedJson = QJsonDocument(getSolvedMap()).toJson(QJsonDocument::Compact);
    m_view->page()->runJavaScript(
        QString("window.updateProgress(%1);").arg(solvedJson)
    );
}
