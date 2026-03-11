#include "problemwidgets.h"
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <qdir.h>
#include <qstandardpaths.h>
#include "progressmanager.h"

// ==========================================
// TAG LABEL (The "Chip")
// ==========================================
// Colors are mapped per topic category for fast visual scanning
static QColor tagColorForTopic(const QString &topic)
{
    const QString t = topic.toLower();
    if (t.contains("array") || t.contains("string") || t.contains("list"))
        return QColor("#1a3a52"); // Blue family
    if (t.contains("tree") || t.contains("graph") || t.contains("bfs") || t.contains("dfs"))
        return QColor("#2a3a1a"); // Green family
    if (t.contains("dp") || t.contains("dynamic") || t.contains("memo"))
        return QColor("#3a1a3a"); // Purple family
    if (t.contains("hash") || t.contains("map") || t.contains("set"))
        return QColor("#3a2a1a"); // Orange family
    if (t.contains("sort") || t.contains("search") || t.contains("binary"))
        return QColor("#1a2a3a"); // Teal family
    return QColor("#252525");     // Default dark
}

static QColor tagTextColorForTopic(const QString &topic)
{
    const QString t = topic.toLower();
    if (t.contains("array") || t.contains("string") || t.contains("list"))
        return QColor("#58a6ff");
    if (t.contains("tree") || t.contains("graph") || t.contains("bfs") || t.contains("dfs"))
        return QColor("#7ee787");
    if (t.contains("dp") || t.contains("dynamic") || t.contains("memo"))
        return QColor("#bc8cff");
    if (t.contains("hash") || t.contains("map") || t.contains("set"))
        return QColor("#ffa657");
    if (t.contains("sort") || t.contains("search") || t.contains("binary"))
        return QColor("#39d0c8");
    return QColor("#888888");
}

TagLabel::TagLabel(const QString &text, QWidget *parent) : QLabel(text, parent)
{
    setFont(QFont("Consolas", 8));
    setAlignment(Qt::AlignCenter);

    QColor bg   = tagColorForTopic(text);
    QColor fg   = tagTextColorForTopic(text);
    // Build border color as slightly lighter version of bg
    QColor border = bg.lighter(150);

    setStyleSheet(QString(R"(
        QLabel {
            background-color: %1;
            color: %2;
            padding: 2px 7px;
            border-radius: 3px;
            border: 1px solid %3;
        }
        QLabel:hover {
            color: #ffffff;
            background-color: %4;
        }
    )").arg(bg.name(), fg.name(), border.name(), bg.lighter(130).name()));
}

// ==========================================
// PROBLEM CARD
// ==========================================
ProblemCard::ProblemCard(const ProblemData &data, QWidget *parent)
    : QFrame(parent)
{
    difficultyColor = getDifficultyColor(data.difficulty);
    m_data = data;
    setupUi(data);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(buildCardStyleSheet());
    setMinimumHeight(64);
}

QString ProblemCard::buildCardStyleSheet() const
{
    return R"(
        ProblemCard {
            background-color: #161616;
            border: 1px solid #222222;
            border-radius: 4px;
        }
        ProblemCard:hover {
            background-color: #1c1c1c;
            border: 1px solid #3a3a3a;
        }

        QLabel#Title {
            background: transparent;
            color: #e8e8e8;
            font-family: 'Segoe UI', 'Inter', sans-serif;
            font-size: 14px;
            font-weight: 600;
        }
        QLabel#ProblemId {
            background: transparent;
            color: #444;
            font-family: 'Consolas', monospace;
            font-size: 12px;
        }
        QLabel#DiffPill {
            font-family: 'Consolas', monospace;
            font-weight: bold;
            font-size: 10px;
            border-radius: 3px;
            padding: 2px 7px;
        }
    )";
}

void ProblemCard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // Left difficulty strip with rounded corner on left side
    QPainterPath strip;
    strip.addRoundedRect(QRectF(0, 0, 4, height()), 4, 4);
    painter.setBrush(difficultyColor);
    painter.drawPath(strip);

    // On hover: paint a very subtle difficulty-colored glow on the left border
    // (achieved via a second translucent rect slightly wider)
    if (underMouse()) {
        QColor glow = difficultyColor;
        glow.setAlpha(30);
        painter.setBrush(glow);
        QRectF glowRect(0, 0, 14, height());
        QPainterPath glowPath;
        glowPath.addRoundedRect(glowRect, 4, 4);
        painter.drawPath(glowPath);
    }
}

void ProblemCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit openRequested(m_data.path);
    }
    QFrame::mousePressEvent(event);
}

bool ProblemCard::event(QEvent *e)
{
    if (e->type() == QEvent::HoverEnter || e->type() == QEvent::HoverLeave) {
        update(); // repaint for the glow
    }
    return QFrame::event(e);
}

void ProblemCard::setupUi(const ProblemData &data)
{
    setAttribute(Qt::WA_Hover, true);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(16, 10, 14, 10);
    mainLayout->setSpacing(0);

    // ── LEFT: info column (stretches) ──
    QVBoxLayout *infoCol = new QVBoxLayout();
    infoCol->setSpacing(5);
    infoCol->setContentsMargins(0, 0, 0, 0);

    // Row 1: Title only (clean, prominent)
    QLabel *titleLabel = new QLabel(data.title, this);
    titleLabel->setObjectName("Title");
    infoCol->addWidget(titleLabel);

    // Row 2: Difficulty pill + topic tags (structured second line)
    QHBoxLayout *metaRow = new QHBoxLayout();
    metaRow->setSpacing(6);
    metaRow->setContentsMargins(0, 2, 0, 0);

    // Difficulty pill — always first on this row
    QLabel *diffPill = new QLabel(data.difficulty.toUpper(), this);
    diffPill->setObjectName("DiffPill");
    diffPill->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    diffPill->setStyleSheet(QString(
        "QLabel#DiffPill { "
        "color: %1; "
        "background-color: rgba(%2,%3,%4,35); "
        "border: 1px solid rgba(%2,%3,%4,90); "
        "border-radius: 3px; "
        "padding: 2px 7px; "
        "font-family: 'Consolas', monospace; "
        "font-weight: bold; "
        "font-size: 10px; }"
    ).arg(difficultyColor.name())
     .arg(difficultyColor.red())
     .arg(difficultyColor.green())
     .arg(difficultyColor.blue()));
    metaRow->addWidget(diffPill);

    // Topic tags follow the pill
    for (const QString &topic : data.topics)
        metaRow->addWidget(new TagLabel(topic, this));

    metaRow->addStretch();
    infoCol->addLayout(metaRow);

    // ── RIGHT: solved check circle, vertically centered ──
    statusLabel = new QLabel(this);
    statusLabel->setObjectName("Status");
    statusLabel->setFixedSize(24, 24);
    statusLabel->setAlignment(Qt::AlignCenter);
    updateSolvedState(data.isSolved);

    mainLayout->addLayout(infoCol, 1);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(statusLabel, 0, Qt::AlignVCenter);
}

void ProblemCard::updateSolvedState(bool solved)
{
    if (solved) {
        statusLabel->setText(" ✓ Solved");
        QFontMetrics fm(statusLabel->font());
        int textWidth = fm.horizontalAdvance(statusLabel->text());
        statusLabel->setFixedSize(textWidth+11, 22);
        statusLabel->setStyleSheet(R"(
            QLabel#Status {
            border-radius: 6px;
            font-size: 11px;
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.8px;
            background-color: hsla(90, 80%, 60%, 0.15);
            color: hsl(90, 80%, 70%);
            border-radius: 6px;
            font-size: 11px;
            font-weight: 500;
            border: 1px solid hsla(90, 80%, 60%, 0.15);
            }
        )");
    } else {
        statusLabel->setText("");
        statusLabel->setStyleSheet(R"(
            QLabel#Status {
                background: transparent;
                border: 1px solid #2a2a2a;
                border-radius: 11px;
            }
        )");
    }
}

QColor ProblemCard::getDifficultyColor(const QString &diff)
{
    const QString d = diff.toLower();
    if (d == "easy")   return QColor("#2cbb5d"); // Green
    if (d == "medium") return QColor("#ffc01e"); // Yellow/Amber
    if (d == "hard")   return QColor("#ef4743"); // Red
    return QColor("#666666");
}

// ==========================================
// PROBLEM BROWSER
// ==========================================
ProblemBrowser::ProblemBrowser(ProgressManager *pm, QWidget *parent)
    : QWidget(parent)
    , progressManager(pm)
{
    progressManager->load();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── HEADER BAR ──
    setupHeader();
    mainLayout->addWidget(m_headerWidget);

    // ── SCROLL AREA ──
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea { border: none; background: #0f0f0f; }

        QScrollBar:vertical {
            border: none;
            background: #0f0f0f;
            width: 8px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #2e2e2e;
            min-height: 30px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: #4a4a4a;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");

    scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: #0f0f0f;");

    listLayout = new QVBoxLayout(scrollContent);
    listLayout->setSpacing(6);
    listLayout->setContentsMargins(20, 16, 20, 32); // extra bottom padding
    listLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea, 1);

    connect(progressManager, &ProgressManager::progressChanged,
            this, &ProblemBrowser::onProgressChanged);
    connect(progressManager, &ProgressManager::progressChanged,
            this, &ProblemBrowser::updateHeaderStats);
}

void ProblemBrowser::setupHeader()
{
    m_headerWidget = new QWidget(this);
    m_headerWidget->setObjectName("browserHeader");
    m_headerWidget->setFixedHeight(64);
    m_headerWidget->setStyleSheet(R"(
        #browserHeader {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #111111, stop:1 #0f0f0f);
            border-bottom: 1px solid #1e1e1e;
        }
    )");

    QHBoxLayout *hLayout = new QHBoxLayout(m_headerWidget);
    hLayout->setContentsMargins(24, 0, 24, 0);
    hLayout->setSpacing(16);

    // App name / logo text
    QLabel *appName = new QLabel("SyntaxFlow", m_headerWidget);
    appName->setStyleSheet(R"(
        color: #e8e8e8;
        font-family: 'Segoe UI', 'Inter', sans-serif;
        font-size: 18px;
        font-weight: 700;
        letter-spacing: 0.5px;
        background: transparent;
    )");

    // Accent dot
    QLabel *dot = new QLabel("•", m_headerWidget);
    dot->setStyleSheet("color: #58a6ff; font-size: 20px; background: transparent;");

    // Subtitle
    QLabel *subtitle = new QLabel("Problem Set", m_headerWidget);
    subtitle->setStyleSheet(R"(
        color: #555;
        font-family: 'Segoe UI', sans-serif;
        font-size: 13px;
        background: transparent;
    )");

    hLayout->addWidget(appName);
    hLayout->addWidget(dot);
    hLayout->addWidget(subtitle);
    hLayout->addStretch();

    // Stats label
    m_statsLabel = new QLabel("— / — Solved", m_headerWidget);
    m_statsLabel->setObjectName("statsLabel");
    m_statsLabel->setStyleSheet(R"(
        #statsLabel {
            color: #888;
            font-family: 'Consolas', monospace;
            font-size: 12px;
            background: rgba(255,255,255,5);
            border: 1px solid #2a2a2a;
            border-radius: 4px;
            padding: 4px 12px;
        }
    )");
    hLayout->addWidget(m_statsLabel);
}

void ProblemBrowser::updateHeaderStats()
{
    int total  = cardMap.size();
    int solved = 0;
    for (const auto &id : cardMap.keys()) {
        if (progressManager->isSolved(id)) solved++;
    }
    m_statsLabel->setText(QString("%1 / %2 Solved").arg(solved).arg(total));
    // Color the stats based on progress
    QString color = (total > 0 && solved == total) ? "#7ee787" :
                    (solved > 0 ? "#ffc01e" : "#888");
    m_statsLabel->setStyleSheet(QString(R"(
        #statsLabel {
            color: %1;
            font-family: 'Consolas', monospace;
            font-size: 12px;
            background: rgba(255,255,255,5);
            border: 1px solid #2a2a2a;
            border-radius: 4px;
            padding: 4px 12px;
        }
    )").arg(color));
}

void ProblemBrowser::loadFromJson(const QString &filePath)
{
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
        data.id         = obj["id"].toString();
        data.title      = obj["title"].toString();
        data.difficulty = obj["difficulty"].toString();
        data.path       = obj["path"].toString();
        data.isSolved   = progressManager->isSolved(data.id);

        QJsonArray topicArray = obj["topics"].toArray();
        for (const QJsonValue &t : topicArray) data.topics.append(t.toString());

        ProblemCard *card = new ProblemCard(data, scrollContent);
        cardMap[data.id] = card;

        connect(card, &ProblemCard::openRequested,
                this, &ProblemBrowser::navigateToEditor);

        listLayout->addWidget(card);
    }

    updateHeaderStats();
}

void ProblemBrowser::onProgressChanged(const QString &problemId)
{
    if (!cardMap.contains(problemId)) return;
    ProblemCard *card = cardMap.value(problemId);
    bool solved = progressManager->isSolved(problemId);
    card->updateSolvedState(solved);
}
