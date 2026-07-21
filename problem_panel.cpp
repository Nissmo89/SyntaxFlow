#include "problem_panel.h"
#include "markdownrenderer.h"

#include <QVBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include <QStyleOption>
#include <QUrl>
#include <QDebug>

ProblemPanel::ProblemPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("problemPanel");
    buildUI();
}

void ProblemPanel::buildUI()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(0);

    markdownViewer = new NetworkBrowser(this);
    markdownViewer->setFrameShape(QFrame::NoFrame);
    markdownViewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    markdownViewer->setOpenExternalLinks(true);

    layout->addWidget(markdownViewer);
}

void ProblemPanel::loadFromJson(const QJsonObject &obj)
{
    // Store Problem Data
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

    // Check if the current problem is Two Sum to render custom Markdown and diagram
    QString rawMarkdown;
    QString htmlBody;
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
        htmlBody = MarkdownRenderer::toHtml(rawMarkdown);
    } else {
        QString badgeClass = "difficulty-easy";
        if (difficulty == "medium") badgeClass = "difficulty-medium";
        else if (difficulty == "hard") badgeClass = "difficulty-hard";

        QString difficultyText = obj["difficulty"].toString();

        QString tagsHtml;
        for (const QString &tag : tags) {
            tagsHtml += QString("<code class=\"code-tag\">%1</code> ").arg(tag.toHtmlEscaped());
        }

        htmlBody = QString(
            "<h1>%1</h1>"
            "<p><b>Difficulty:</b> <span class=\"%2\">%3</span> | <b>Category:</b> %4</p>"
            "<p><b>Tags:</b> %5</p>"
            "<hr>"
            "%6"
        )
        .arg(problemTitle.toHtmlEscaped())
        .arg(badgeClass)
        .arg(difficultyText.toHtmlEscaped())
        .arg(category.isEmpty() ? "General" : category.toHtmlEscaped())
        .arg(tagsHtml)
        .arg(obj["description"].toString());
    }
    markdownViewer->setProblemHtml(htmlBody);

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

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << error.errorString();
        return false;
    }

    if (!doc.isObject()) {
        qWarning() << "JSON root is not an object";
        return false;
    }

    loadFromJson(doc.object());
    return true;
}

void ProblemPanel::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
