#include "problemwidgets.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <qdir.h>
#include <qstandardpaths.h>
#include "progressmanager.h"
// ==========================================
// TAG LABEL (The "Chip")
// ==========================================
TagLabel::TagLabel(const QString &text, QWidget *parent) : QLabel(text, parent) {
    setFont(QFont("Consolas", 8));
    setAlignment(Qt::AlignCenter);

    // Minimalist Chip: Dark grey background, light grey text
    // This allows the Difficulty Color to be the main "pop" of color on the card
    setStyleSheet(R"(
        QLabel {
            background-color: #2a2a2a;
            color: #999999;
            padding: 3px 8px;
            border-radius: 2px; /* Slight roundness, but mostly square */
            border: 1px solid #333333;
        }
        QLabel:hover {
            color: #ffffff;
            border-color: #555555;
            background-color: #333333;
        }
    )");
}

// ==========================================
// PROBLEM CARD
// ==========================================
ProblemCard::ProblemCard(const ProblemData &data, QWidget *parent)
    : QFrame(parent)
{
    difficultyColor = getDifficultyColor(data.difficulty);

    setupUi(data);

    // UX INTERACTION:
    // PointingHand tells the user "This whole thing is clickable"
    setCursor(Qt::PointingHandCursor);

    // STYLE:
    // 1. Background is slightly lighter than the main window (#161616 vs #0f0f0f)
    // 2. Borders are sharp (0px radius) for that "Terminal/Zed" feel
    // 3. Hover effect brightens the border and background slightly
    setStyleSheet(R"(
        ProblemCard {
            background-color: #161616;
            border: 1px solid #2a2a2a;
            border-radius: 0px;
        }
        ProblemCard:hover {
            background-color: #1a1a1a;
            border: 1px solid #444444;
        }

        /* Title is distinct and bright */
        QLabel#Title {
            background: transparent;
            color: #eeeeee;
            font-family: 'Segoe UI', sans-serif;
            font-size: 15px;
            font-weight: 600;
        }

        /* Difficulty Text (e.g. "Easy") style */
        QLabel#DiffLabel {
            font-family: 'Consolas', monospace;
            font-weight: bold;
            font-size: 11px;
            background: transparent;
        }

        /* Status: Muted text */
        QLabel#Status {
            font-family: 'Consolas', monospace;
            font-size: 11px;
            font-weight: bold;
        }

        /* Button: "Ghost" style. Minimal distraction until hovered. */
        QPushButton {
            background-color: transparent;
            color: #666;
            border: 1px solid #333;
            font-family: 'Consolas', monospace;
            font-weight: bold;
            padding: 5px 15px;
            border-radius: 0px;
        }
        QPushButton:hover {
            color: #fff;
            border-color: #fff;
            background-color: #222;
        }
    )");
}

void ProblemCard::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event); // Draw CSS styles first

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(difficultyColor);

    // THE "STRIP":
    // A 3px wide colored bar on the far left.
    // This is the classic "Code Problem" indicator.
    painter.drawRect(0, 0, 3, height());
}

void ProblemCard::setupUi(const ProblemData &data) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 12, 15, 12); // Compact but comfortable
    mainLayout->setSpacing(15);

    // 1. INFO COLUMN (Left)
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(5);

    // -- Title Row --
    QHBoxLayout *titleRow = new QHBoxLayout();
    titleRow->setSpacing(8);

    // Difficulty Label (colored text) next to title
    QLabel *diffLabel = new QLabel(data.difficulty.toUpper(), this);
    diffLabel->setObjectName("DiffLabel");
    // Apply the specific Green/Yellow/Red color to the text
    QPalette p = diffLabel->palette();
    p.setColor(QPalette::WindowText, difficultyColor);
    diffLabel->setPalette(p);

    QLabel *titleLabel = new QLabel(data.title, this);
    titleLabel->setObjectName("Title");

    titleRow->addWidget(diffLabel);
    titleRow->addWidget(titleLabel);
    titleRow->addStretch();
    infoLayout->addLayout(titleRow);

    // -- Tags Row --
    QHBoxLayout *tagsLayout = new QHBoxLayout();
    tagsLayout->setSpacing(6);
    tagsLayout->setContentsMargins(0,2,0,0);
    for(const QString &topic : data.topics) {
        tagsLayout->addWidget(new TagLabel(topic, this));
    }
    tagsLayout->addStretch();
    infoLayout->addLayout(tagsLayout);

    // 2. ACTION COLUMN (Right)
    QVBoxLayout *actionLayout = new QVBoxLayout();
    actionLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    actionLayout->setSpacing(5);

    // Status Label
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("Status");
    if (data.isSolved) {
        statusLabel->setText("SOLVED");
        statusLabel->setStyleSheet("color: #4CAF50;"); // Green
    } else {
        statusLabel->setText("UNSOLVED");
        statusLabel->setStyleSheet("color: #555;"); // Dark Grey
    }
    statusLabel->setAlignment(Qt::AlignRight);

    // Code Button
    QPushButton *btnSolve = new QPushButton("/> CODE", this);
    btnSolve->setCursor(Qt::PointingHandCursor);
    connect(btnSolve, &QPushButton::clicked, [this, data]() {
        emit openRequested(data.path);
    });

    actionLayout->addWidget(statusLabel);
    actionLayout->addWidget(btnSolve);

    // Combine
    mainLayout->addLayout(infoLayout, 1); // Info takes available space
    mainLayout->addLayout(actionLayout);
}

QColor ProblemCard::getDifficultyColor(const QString &diff) {
    // YOUR REQUESTED COLOR SCHEME:
    QString d = diff.toLower();

    if (d == "easy")   return QColor("#4CAF50"); // Green
    if (d == "medium") return QColor("#FFC107"); // Yellow/Amber
    if (d == "hard")   return QColor("#F44336"); // Red

    return QColor("#666666"); // Fallback Grey
}

// ==========================================
// PROBLEM BROWSER
// ==========================================
ProblemBrowser::ProblemBrowser(ProgressManager *pm, QWidget *parent)
    : QWidget(parent),
    progressManager(pm)   // ✅ STORE IT HERE
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    progressManager->load();
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    // CUSTOM SCROLLBAR (VSCode Style)
    // Minimalist, square, dark
    scrollArea->setStyleSheet(R"(
        QScrollArea { border: none; background: #0f0f0f; }

        QScrollBar:vertical {
            border: none;
            background: #0f0f0f;
            width: 10px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #333;
            min-height: 20px;
            border-radius: 0px;
        }
        QScrollBar::handle:vertical:hover {
            background: #555;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");

    scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: #0f0f0f;"); // Deep black background

    listLayout = new QVBoxLayout(scrollContent);
    listLayout->setSpacing(10); // Consistent gap between cards
    listLayout->setContentsMargins(20, 20, 20, 20);
    listLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    connect(progressManager, &ProgressManager::progressChanged,
            this, &ProblemBrowser::onProgressChanged);


}

// Logic loadFromJson remains the same as requested...
void ProblemBrowser::loadFromJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray array = doc.array();
    cardMap.clear();

    // Clear old widgets
    QLayoutItem *child;
    while ((child = listLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    for (const QJsonValue &val : array) {
        QJsonObject obj = val.toObject();
        ProblemData data;
        data.id = obj["id"].toString();
        data.title = obj["title"].toString();
        data.difficulty = obj["difficulty"].toString();
        data.path = obj["path"].toString();
        QJsonObject statusObj = obj["status"].toObject();
        // data.isSolved = statusObj["solved"].toBool(); this change from this to
        data.isSolved = progressManager->isSolved(data.id);
        QJsonArray topicArray = obj["topics"].toArray();
        for (const QJsonValue &t : topicArray) data.topics.append(t.toString());

        ProblemCard *card = new ProblemCard(data, scrollContent);

        cardMap[data.id] = card;   // ✅ STORE POINTER

        connect(card, &ProblemCard::openRequested,
                this, &ProblemBrowser::navigateToEditor);

        listLayout->addWidget(card);

    }
}

void ProblemBrowser::onProgressChanged(const QString &problemId)
{
    if (!cardMap.contains(problemId))
        return;

    ProblemCard *card = cardMap.value(problemId);

    bool solved = progressManager->isSolved(problemId);

    card->updateSolvedState(solved);
}

void ProblemCard::updateSolvedState(bool solved)
{
    if (solved) {
        statusLabel->setText("SOLVED");
        statusLabel->setStyleSheet("color: #4CAF50;");
    } else {
        statusLabel->setText("UNSOLVED");
        statusLabel->setStyleSheet("color: #555;");
    }
}

