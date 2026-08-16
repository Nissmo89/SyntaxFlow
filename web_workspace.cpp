#include "web_workspace.h"

#include <QVBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QUrl>
#include <QWebEngineSettings>
#include <QWebEnginePage>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "copilotmanager.h"

WebWorkspace::WebWorkspace(const QUrl &url, QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_netManager = new QNetworkAccessManager(this);

    m_view = new QWebEngineView(this);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    m_channel = new QWebChannel(m_view->page());
    m_bridge = new WorkspaceBridge(this);

    m_channel->registerObject("workspaceBridge", m_bridge);
    m_view->page()->setWebChannel(m_channel);

    connect(m_bridge, &WorkspaceBridge::workspaceReady, this, &WebWorkspace::onBridgeWorkspaceReady);
    connect(m_bridge, &WorkspaceBridge::textChanged, this, &WebWorkspace::onBridgeTextChanged);
    connect(m_bridge, &WorkspaceBridge::backRequested, this, &WebWorkspace::backRequested);
    connect(m_bridge, &WorkspaceBridge::runRequested, this, &WebWorkspace::runRequested);
    connect(m_bridge, &WorkspaceBridge::submitRequested, this, &WebWorkspace::submitRequested);
    connect(m_bridge, &WorkspaceBridge::languageChanged, this, &WebWorkspace::languageChanged);
    connect(m_bridge, &WorkspaceBridge::resetRequested, this, &WebWorkspace::resetRequested);
    connect(m_bridge, &WorkspaceBridge::fetchSolutionRequested, this, &WebWorkspace::onFetchSolutionRequested);

    layout->addWidget(m_view);

    m_view->setUrl(url);
}

void WebWorkspace::setCopilotManager(CopilotManager *manager) {
    if (m_channel && manager) {
        m_channel->registerObject("copilotManager", manager);
    }
}

WebWorkspace::~WebWorkspace() = default;

void WebWorkspace::runJs(const QString &script) {
    if (m_view && m_view->page()) {
        m_view->page()->runJavaScript(script);
    }
}

void WebWorkspace::applyTheme(const QString &script) {
    runJs(script);
}

void WebWorkspace::onBridgeWorkspaceReady() {
    m_isReady = true;
    if (!m_pendingProblemData.isEmpty()) {
        setProblemData(m_pendingProblemData);
        m_pendingProblemData = QJsonObject();
    }
    if (!m_pendingTestCases.isEmpty()) {
        setTestCases(m_pendingTestCases);
        m_pendingTestCases = QJsonArray();
    }
    if (!m_pendingLangs.isEmpty()) {
        setLanguages(m_pendingLangs, m_pendingCurrentLang);
        m_pendingLangs = QJsonArray();
    }
    if (!m_text.isEmpty()) {
        setCode(m_text);
    }
}

void WebWorkspace::onBridgeTextChanged(const QString &text) {
    if (m_text != text) {
        m_text = text;
        emit textChanged(text);
    }
}

void WebWorkspace::setProblemData(const QJsonObject &problemObj) {
    if (!m_isReady) {
        m_pendingProblemData = problemObj;
        return;
    }
    QString jsonStr = QJsonDocument(problemObj).toJson(QJsonDocument::Compact);
    runJs(QString("window.setProblemData(%1);").arg(jsonStr));
}

void WebWorkspace::setTestCases(const QJsonArray &testCases) {
    if (!m_isReady) {
        m_pendingTestCases = testCases;
        return;
    }
    QString jsonStr = QJsonDocument(testCases).toJson(QJsonDocument::Compact);
    runJs(QString("window.setTestCases(%1);").arg(jsonStr));
}

void WebWorkspace::setTestResult(int caseIndex, const QString &status, const QString &actualOutput, const QString &expected, qint64 elapsedMs, bool passed) {
    if (!m_isReady) return;
    QJsonObject obj;
    obj["index"] = caseIndex;
    obj["status"] = status;
    obj["actualOutput"] = actualOutput;
    obj["expected"] = expected;
    obj["elapsedMs"] = elapsedMs;
    obj["passed"] = passed;
    QString jsonStr = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    runJs(QString("window.setTestResult(%1);").arg(jsonStr));
}

void WebWorkspace::setTestRunning(int caseIndex) {
    if (!m_isReady) return;
    runJs(QString("window.setTestRunning(%1);").arg(caseIndex));
}

void WebWorkspace::clearAllResults() {
    if (!m_isReady) return;
    runJs("window.clearAllResults();");
}

void WebWorkspace::setCode(const QString &code) {
    m_text = code;
    if (m_isReady) {
        QString jsonStr = QJsonDocument(QJsonArray{code}).toJson(QJsonDocument::Compact);
        QString val = jsonStr.mid(1, jsonStr.length() - 2);
        runJs(QString("window.setCode(%1);").arg(val));
    }
}

QString WebWorkspace::code() const {
    return m_text;
}

void WebWorkspace::setLanguages(const QJsonArray &langs, const QString &currentLang) {
    if (!m_isReady) {
        m_pendingLangs = langs;
        m_pendingCurrentLang = currentLang;
        return;
    }
    QJsonObject obj;
    obj["languages"] = langs;
    obj["current"] = currentLang;
    QString jsonStr = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    runJs(QString("window.setLanguages(%1);").arg(jsonStr));
}

void WebWorkspace::setLanguage(const QString &langId) {
    m_pendingCurrentLang = langId;
    if (m_isReady) {
        runJs(QString("window.setLanguage('%1');").arg(langId));
    }
}

void WebWorkspace::onFetchSolutionRequested(const QString &slug) {
    if (slug.isEmpty()) return;

    QString underscoreSlug = QString(slug).replace('-', '_').toLower();
    QString hyphenSlug = QString(slug).replace('_', '-').toLower();

    // Check all potential project and executable root directories
    QStringList baseDirs = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QDir(QCoreApplication::applicationDirPath()).filePath(".."),
        QDir(QCoreApplication::applicationDirPath()).filePath("../..")
    };

    QStringList candidateRelPaths;
    QStringList subDirs = {"solutions/easy", "solutions/medium", "solutions/hard", "solutions"};

    for (const QString &sub : subDirs) {
        candidateRelPaths << QString("%1/%2.md").arg(sub, underscoreSlug);
        candidateRelPaths << QString("%1/%2.md").arg(sub, hyphenSlug);
    }

    for (const QString &base : baseDirs) {
        for (const QString &rel : candidateRelPaths) {
            QString fullPath = QDir(base).filePath(rel);
            QFile file(fullPath);
            if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString content = QString::fromUtf8(file.readAll());
                file.close();
                if (!content.trimmed().isEmpty()) {
                    QJsonObject res;
                    res["status"] = "loaded";
                    QJsonObject offObj;
                    offObj["title"] = "AI Editorial Solution & Explanation";
                    offObj["content"] = content;
                    offObj["author"] = "SyntaxFlow AI";
                    offObj["isOfficial"] = true;
                    res["official"] = offObj;
                    res["community"] = QJsonArray();

                    QString slugJson = QString::fromUtf8(QJsonDocument(QJsonObject{{"slug", slug}}).toJson(QJsonDocument::Compact));
                    QString resJson = QString::fromUtf8(QJsonDocument(res).toJson(QJsonDocument::Compact));
                    runJs(QString("if (window.setSolutionResult) window.setSolutionResult(%1.slug, %2);").arg(slugJson, resJson));
                    return;
                }
            }
        }
    }

    // No local solution file found
    QJsonObject res;
    res["status"] = "not_found";
    res["error"] = QString("No solution markdown found in solutions/ for '%1'.").arg(slug);
    QString slugJson = QString::fromUtf8(QJsonDocument(QJsonObject{{"slug", slug}}).toJson(QJsonDocument::Compact));
    QString resJson = QString::fromUtf8(QJsonDocument(res).toJson(QJsonDocument::Compact));
    runJs(QString("if (window.setSolutionResult) window.setSolutionResult(%1.slug, %2);").arg(slugJson, resJson));
}
