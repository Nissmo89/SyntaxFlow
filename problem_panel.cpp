#include "problem_panel.h"
#include "markdownrenderer.h"

#include <QVBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QCoreApplication>
#include <QWebEngineSettings>
#include <QDebug>

ProblemPanel::ProblemPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("problemPanel");
    buildUI();
}

ProblemPanel::~ProblemPanel() = default;

void ProblemPanel::buildUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_view = new QWebEngineView(this);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    m_channel = new QWebChannel(m_view->page());
    m_bridge = new ProblemPanelBridge(this);

    m_channel->registerObject("panelBridge", m_bridge);
    m_view->page()->setWebChannel(m_channel);

    layout->addWidget(m_view);

    const QString appDir = QCoreApplication::applicationDirPath();
    QString htmlPath = QDir(appDir).filePath("web_editor/panel.html");
    if (!QFile::exists(htmlPath)) {
        htmlPath = QDir(appDir + "/../web_editor").filePath("panel.html");
    }

    connect(m_view, &QWebEngineView::loadFinished, this, [this](bool ok) {
        if (ok) {
            m_isWebReady = true;
            syncToWeb();
        }
    });

    m_view->setUrl(QUrl::fromLocalFile(htmlPath));
}

void ProblemPanel::onWebReady()
{
    m_isWebReady = true;
    syncToWeb();
}

void ProblemPanel::syncToWeb()
{
    if (!m_isWebReady) return;

    QJsonObject obj;
    obj["title"] = problemTitle;
    obj["difficulty"] = difficulty;
    obj["category"] = category;
    
    QJsonArray tagsArr;
    for (const QString &t : tags) {
        tagsArr.append(t);
    }
    obj["tags"] = tagsArr;
    obj["markdown"] = rawMarkdown;

    QString jsonStr = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    m_view->page()->runJavaScript(QString("window.setProblemData(%1);").arg(jsonStr));
}

void ProblemPanel::loadFromJson(const QJsonObject &obj)
{
    problemId = obj["id"].toString();
    problemTitle = obj["title"].toString();
    difficulty = obj["difficulty"].toString().toLower();
    category = obj["category"].toString();

    tags.clear();
    QJsonArray tagsArray = obj["tags"].toArray();
    for (const auto &tag : tagsArray) {
        tags << tag.toString();
    }

    cachedTestCases = obj["testCases"].toArray();

    if (problemId == "two_sum") {
        QString imagePath = QFileInfo(currentFilePath).dir().absoluteFilePath("../two_sum_diagram.png");
        if (!QFile::exists(imagePath)) {
            imagePath = QFileInfo(currentFilePath).dir().absoluteFilePath("two_sum_diagram.png");
        }
        if (!QFile::exists(imagePath)) {
            imagePath = "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/two_sum_diagram.png";
        }
        QString imageUrl = QUrl::fromLocalFile(imagePath).toString();

        rawMarkdown = QString(R"(# 1. Two Sum

Given an array of integers `nums` and an integer `target`, return _indices of the two numbers such that they add up to `target`_.

You may assume that each input would have **_exactly_ one solution**, and you may not use the _same_ element twice.

You can return the answer in any order.

![Two Sum Diagram](%1)

**Example 1:**

```cpp
Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].
```

**Example 2:**

```cpp
Input: nums = [3,2,4], target = 6
Output: [1,2]
```

**Example 3:**

```cpp
Input: nums = [3,3], target = 6
Output: [0,1]
```

**Constraints:**

  * `2 <= nums.length <= 104`
  * `-109 <= nums[i] <= 109`
  * `-109 <= target <= 109`
  * **Only one valid answer exists.**

**Follow-up:** Can you come up with an algorithm that is less than `O(n2)` time complexity?
)").arg(imageUrl);
    } else {
        rawMarkdown = obj["description"].toString();
    }

    syncToWeb();

    emit problemLoaded(problemId);
    emit testCasesAvailable(cachedTestCases);
}

bool ProblemPanel::loadFromFile(const QString &filePath)
{
    currentFilePath = filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "JSON parse error or invalid root object";
        return false;
    }

    loadFromJson(doc.object());
    return true;
}
