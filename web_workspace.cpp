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
    if (slug.isEmpty() || !m_netManager) return;

    QUrl url("https://leetcode.com/graphql");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64)");
    request.setRawHeader("Referer", "https://leetcode.com");

    QString queryStr = QString(
        "query getSolutions($slug: String!) {"
        "  question(titleSlug: $slug) {"
        "    solution {"
        "      id title content paidOnly"
        "    }"
        "  }"
        "  questionSolutions(filters: {questionSlug: $slug, skip: 0, first: 10, orderBy: most_votes}) {"
        "    totalNum"
        "    solutions {"
        "      id title commentCount viewCount"
        "      post {"
        "        id content voteCount"
        "        author { username }"
        "      }"
        "    }"
        "  }"
        "}");

    QJsonObject queryObj;
    queryObj["query"] = queryStr;
    QJsonObject variables;
    variables["slug"] = slug;
    queryObj["variables"] = variables;

    QByteArray body = QJsonDocument(queryObj).toJson(QJsonDocument::Compact);

    QNetworkReply *reply = m_netManager->post(request, body);
    connect(reply, &QNetworkReply::finished, this, [this, reply, slug]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            QJsonObject res;
            res["status"] = "error";
            res["error"] = reply->errorString();
            QString slugJson = QString::fromUtf8(QJsonDocument(QJsonObject{{"slug", slug}}).toJson(QJsonDocument::Compact));
            QString resJson = QString::fromUtf8(QJsonDocument(res).toJson(QJsonDocument::Compact));
            runJs(QString("if (window.setSolutionResult) window.setSolutionResult(%1.slug, %2);").arg(slugJson, resJson));
            return;
        }

        QByteArray data = reply->readAll();
        QJsonParseError parseErr;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseErr);
        if (parseErr.error != QJsonParseError::NoError) {
            QJsonObject res;
            res["status"] = "error";
            res["error"] = "Failed to parse GraphQL response";
            QString slugJson = QString::fromUtf8(QJsonDocument(QJsonObject{{"slug", slug}}).toJson(QJsonDocument::Compact));
            QString resJson = QString::fromUtf8(QJsonDocument(res).toJson(QJsonDocument::Compact));
            runJs(QString("if (window.setSolutionResult) window.setSolutionResult(%1.slug, %2);").arg(slugJson, resJson));
            return;
        }

        QJsonObject root = doc.object();
        QJsonObject dataObj = root.value("data").toObject();
        QJsonObject question = dataObj.value("question").toObject();
        QJsonObject officialSol = question.value("solution").toObject();
        QJsonObject qSolutions = dataObj.value("questionSolutions").toObject();
        QJsonArray commSolutions = qSolutions.value("solutions").toArray();

        auto sanitizeContent = [](QString str) -> QString {
            if (str.contains("\\n")) {
                str.replace("\\r\\n", "\n").replace("\\n", "\n");
            }
            if (str.contains("\\t")) {
                str.replace("\\t", "    ");
            }
            if (str.contains("\\'")) {
                str.replace("\\'", "'");
            }
            if (str.contains("\\\"")) {
                str.replace("\\\"", "\"");
            }
            return str;
        };

        QJsonObject res;
        res["status"] = "loaded";

        // Official solution
        if (!officialSol.isEmpty() && !officialSol.value("paidOnly").toBool(false) && !officialSol.value("content").toString().isEmpty()) {
            QJsonObject offObj;
            offObj["id"] = officialSol.value("id").toString();
            offObj["title"] = officialSol.value("title").toString("Official Editorial");
            offObj["content"] = sanitizeContent(officialSol.value("content").toString());
            offObj["author"] = "LeetCode Editorial";
            offObj["isOfficial"] = true;
            res["official"] = offObj;
        }

        // Community solutions
        QJsonArray list;
        for (const QJsonValue &val : commSolutions) {
            QJsonObject s = val.toObject();
            QJsonObject p = s.value("post").toObject();
            QString content = sanitizeContent(p.value("content").toString());
            if (content.trimmed().isEmpty()) continue;

            QJsonObject item;
            item["id"] = QString::number(s.value("id").toInteger());
            item["title"] = s.value("title").toString();
            item["content"] = content;
            item["voteCount"] = p.value("voteCount").toInt();
            item["viewCount"] = s.value("viewCount").toInt();
            item["author"] = p.value("author").toObject().value("username").toString();
            item["isOfficial"] = false;
            list.append(item);
        }
        res["community"] = list;

        QString slugJson = QString::fromUtf8(QJsonDocument(QJsonObject{{"slug", slug}}).toJson(QJsonDocument::Compact));
        QString resJson = QString::fromUtf8(QJsonDocument(res).toJson(QJsonDocument::Compact));
        runJs(QString("if (window.setSolutionResult) window.setSolutionResult(%1.slug, %2);").arg(slugJson, resJson));
    });
}
