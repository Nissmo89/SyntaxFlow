#include "problem_panel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QPainter>
#include <QStyleOption>
#include <QScrollBar>
#include <QFrame>
#include <QRegularExpression>



ProblemPanel::ProblemPanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("problemPanel");
    setStyleSheet(buildStyleSheet());
    buildUI();
}

void ProblemPanel::buildUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ─── Main Scroll Area ───
    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setObjectName("problemScrollArea");

    contentWidget = new QWidget;
    contentWidget->setObjectName("problemContent");
    auto *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(0);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // ═══════════════════════════════════════════════════════════════════
    // HEADER SECTION
    // ═══════════════════════════════════════════════════════════════════
    auto *headerWidget = new QWidget;
    headerWidget->setObjectName("headerSection");
    auto *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(24, 24, 24, 20);
    headerLayout->setSpacing(12);

    // Title row
    auto *titleRow = new QHBoxLayout;
    titleRow->setSpacing(14);

    titleLabel = new QLabel("Problem Title");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setWordWrap(true);

    difficultyBadge = new QLabel("Easy");
    difficultyBadge->setObjectName("difficultyBadge");
    difficultyBadge->setProperty("difficulty", "easy");

    titleRow->addWidget(titleLabel, 1);
    titleRow->addWidget(difficultyBadge);

    // Meta row
    auto *metaRow = new QHBoxLayout;
    metaRow->setSpacing(12);

    idLabel = new QLabel("problem_id");
    idLabel->setObjectName("idLabel");

    categoryLabel = new QLabel("Category");
    categoryLabel->setObjectName("categoryLabel");

    metaRow->addWidget(idLabel);
    metaRow->addWidget(categoryLabel);
    metaRow->addStretch();

    // Tags row
    tagsContainer = new QWidget;
    tagsContainer->setObjectName("tagsContainer");
    auto *tagsLayout = new QHBoxLayout(tagsContainer);
    tagsLayout->setContentsMargins(0, 6, 0, 0);
    tagsLayout->setSpacing(8);

    headerLayout->addLayout(titleRow);
    headerLayout->addLayout(metaRow);
    headerLayout->addWidget(tagsContainer);

    // ═══════════════════════════════════════════════════════════════════
    // DESCRIPTION SECTION
    // ═══════════════════════════════════════════════════════════════════
    auto *descSection = new QWidget;
    descSection->setObjectName("section");
    auto *descLayout = new QVBoxLayout(descSection);
    descLayout->setContentsMargins(24, 20, 24, 20);
    descLayout->setSpacing(14);

    auto *descTitle = new QLabel("Description");
    descTitle->setObjectName("sectionTitle");

    descriptionLabel = new QLabel;
    descriptionLabel->setObjectName("contentLabel");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setTextFormat(Qt::RichText);
    descriptionLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    descriptionLabel->setOpenExternalLinks(true);

    descLayout->addWidget(descTitle);
    descLayout->addWidget(descriptionLabel);

    // ═══════════════════════════════════════════════════════════════════
    // TASK SECTION
    // ═══════════════════════════════════════════════════════════════════
    auto *taskOuter = new QWidget;
    taskOuter->setObjectName("taskOuter");
    auto *taskOuterLayout = new QVBoxLayout(taskOuter);
    taskOuterLayout->setContentsMargins(24, 0, 24, 20);

    auto *taskSection = new QWidget;
    taskSection->setObjectName("taskSection");
    auto *taskLayout = new QVBoxLayout(taskSection);
    taskLayout->setContentsMargins(18, 16, 18, 16);
    taskLayout->setSpacing(12);

    auto *taskHeader = new QHBoxLayout;
    auto *taskIcon = new QLabel("•");
    taskIcon->setObjectName("taskIcon");
    auto *taskTitle = new QLabel("Your Task");
    taskTitle->setObjectName("taskTitle");
    taskHeader->addWidget(taskIcon);
    taskHeader->addWidget(taskTitle);
    taskHeader->addStretch();

    taskLabel = new QLabel;
    taskLabel->setObjectName("taskLabel");
    taskLabel->setWordWrap(true);
    taskLabel->setTextFormat(Qt::RichText);
    taskLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    taskLayout->addLayout(taskHeader);
    taskLayout->addWidget(taskLabel);

    taskOuterLayout->addWidget(taskSection);

    // ═══════════════════════════════════════════════════════════════════
    // INPUT/OUTPUT FORMAT SECTION
    // ═══════════════════════════════════════════════════════════════════
    auto *formatSection = new QWidget;
    formatSection->setObjectName("section");
    auto *formatLayout = new QVBoxLayout(formatSection);
    formatLayout->setContentsMargins(24, 0, 24, 20);
    formatLayout->setSpacing(16);

    // Input Format
    auto *inputBox = new QWidget;
    inputBox->setObjectName("formatBox");
    auto *inputBoxLayout = new QVBoxLayout(inputBox);
    inputBoxLayout->setContentsMargins(16, 14, 16, 14);
    inputBoxLayout->setSpacing(10);

    auto *inputHeader = new QHBoxLayout;
    auto *inputIcon = new QLabel("•");
    auto *inputTitle = new QLabel("Input Format");
    inputTitle->setObjectName("formatTitle");
    inputHeader->addWidget(inputIcon);
    inputHeader->addWidget(inputTitle);
    inputHeader->addStretch();

    inputFormatLabel = new QLabel;
    inputFormatLabel->setObjectName("formatLabel");
    inputFormatLabel->setWordWrap(true);
    inputFormatLabel->setTextFormat(Qt::RichText);
    inputFormatLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    inputBoxLayout->addLayout(inputHeader);
    inputBoxLayout->addWidget(inputFormatLabel);

    // Output Format
    auto *outputBox = new QWidget;
    outputBox->setObjectName("formatBox");
    auto *outputBoxLayout = new QVBoxLayout(outputBox);
    outputBoxLayout->setContentsMargins(16, 14, 16, 14);
    outputBoxLayout->setSpacing(10);

    auto *outputHeader = new QHBoxLayout;
    auto *outputIcon = new QLabel("•");
    auto *outputTitle = new QLabel("Output Format");
    outputTitle->setObjectName("formatTitle");
    outputHeader->addWidget(outputIcon);
    outputHeader->addWidget(outputTitle);
    outputHeader->addStretch();

    outputFormatLabel = new QLabel;
    outputFormatLabel->setObjectName("formatLabel");
    outputFormatLabel->setWordWrap(true);
    outputFormatLabel->setTextFormat(Qt::RichText);
    outputFormatLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    outputBoxLayout->addLayout(outputHeader);
    outputBoxLayout->addWidget(outputFormatLabel);

    formatLayout->addWidget(inputBox);
    formatLayout->addWidget(outputBox);

    // ═══════════════════════════════════════════════════════════════════
    // CONSTRAINTS SECTION
    // ═══════════════════════════════════════════════════════════════════
    constraintsWidget = new QWidget;
    constraintsWidget->setObjectName("constraintsSection");
    auto *constraintsLayout = new QVBoxLayout(constraintsWidget);
    constraintsLayout->setContentsMargins(24, 0, 24, 20);
    constraintsLayout->setSpacing(10);

    auto *constraintsHeader = new QHBoxLayout;
    auto *constraintsIcon = new QLabel("•");
    auto *constraintsTitle = new QLabel("Constraints");
    constraintsTitle->setObjectName("sectionTitle");
    constraintsHeader->addWidget(constraintsIcon);
    constraintsHeader->addWidget(constraintsTitle);
    constraintsHeader->addStretch();

    constraintsLayout->addLayout(constraintsHeader);

    // ═══════════════════════════════════════════════════════════════════
    // SAMPLE I/O SECTION
    // ═══════════════════════════════════════════════════════════════════
    auto *sampleSection = new QWidget;
    sampleSection->setObjectName("section");
    auto *sampleLayout = new QVBoxLayout(sampleSection);
    sampleLayout->setContentsMargins(24, 0, 24, 20);
    sampleLayout->setSpacing(16);

    auto *sampleHeader = new QHBoxLayout;
    auto *sampleIcon = new QLabel("•");
    auto *sampleTitle = new QLabel("Example");
    sampleTitle->setObjectName("sectionTitle");
    sampleHeader->addWidget(sampleIcon);
    sampleHeader->addWidget(sampleTitle);
    sampleHeader->addStretch();

    // Sample I/O Container (side by side on wide screens, stacked on narrow)
    auto *ioContainer = new QWidget;
    auto *ioLayout = new QHBoxLayout(ioContainer);
    ioLayout->setContentsMargins(0, 0, 0, 0);
    ioLayout->setSpacing(16);

    // Sample Input Box
    auto *sampleInputBox = new QWidget;
    sampleInputBox->setObjectName("ioBox");
    auto *sampleInputBoxLayout = new QVBoxLayout(sampleInputBox);
    sampleInputBoxLayout->setContentsMargins(0, 0, 0, 0);
    sampleInputBoxLayout->setSpacing(0);

    auto *sampleInputHeader = new QWidget;
    sampleInputHeader->setObjectName("ioHeader");
    auto *sampleInputHeaderLayout = new QHBoxLayout(sampleInputHeader);
    sampleInputHeaderLayout->setContentsMargins(14, 10, 14, 10);
    auto *inputLabel = new QLabel("stdin");
    inputLabel->setObjectName("ioHeaderLabel");
    sampleInputHeaderLayout->addWidget(inputLabel);
    sampleInputHeaderLayout->addStretch();

    sampleInputLabel = new QLabel;
    sampleInputLabel->setObjectName("ioContent");
    sampleInputLabel->setTextFormat(Qt::RichText);
    sampleInputLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    sampleInputLabel->setWordWrap(true);

    sampleInputBoxLayout->addWidget(sampleInputHeader);
    sampleInputBoxLayout->addWidget(sampleInputLabel, 1);

    // Sample Output Box
    auto *sampleOutputBox = new QWidget;
    sampleOutputBox->setObjectName("ioBoxOutput");
    auto *sampleOutputBoxLayout = new QVBoxLayout(sampleOutputBox);
    sampleOutputBoxLayout->setContentsMargins(0, 0, 0, 0);
    sampleOutputBoxLayout->setSpacing(0);

    auto *sampleOutputHeader = new QWidget;
    sampleOutputHeader->setObjectName("ioHeaderOutput");
    auto *sampleOutputHeaderLayout = new QHBoxLayout(sampleOutputHeader);
    sampleOutputHeaderLayout->setContentsMargins(14, 10, 14, 10);
    auto *outputLabel = new QLabel("stdout");
    outputLabel->setObjectName("ioHeaderLabel");
    sampleOutputHeaderLayout->addWidget(outputLabel);
    sampleOutputHeaderLayout->addStretch();

    sampleOutputLabel = new QLabel;
    sampleOutputLabel->setObjectName("ioContentOutput");
    sampleOutputLabel->setTextFormat(Qt::RichText);
    sampleOutputLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    sampleOutputBoxLayout->addWidget(sampleOutputHeader);
    sampleOutputBoxLayout->addWidget(sampleOutputLabel, 1);

    ioLayout->addWidget(sampleInputBox, 1);
    ioLayout->addWidget(sampleOutputBox, 1);

    // Explanation
    sampleExplanationLabel = new QLabel;
    sampleExplanationLabel->setObjectName("explanationLabel");
    sampleExplanationLabel->setWordWrap(true);
    sampleExplanationLabel->setTextFormat(Qt::RichText);
    sampleExplanationLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    sampleExplanationLabel->setVisible(false);

    sampleLayout->addLayout(sampleHeader);
    sampleLayout->addWidget(ioContainer);
    sampleLayout->addWidget(sampleExplanationLabel);

    // ═══════════════════════════════════════════════════════════════════
    // HINTS SECTION
    // ═══════════════════════════════════════════════════════════════════
    hintsWidget = new QWidget;
    hintsWidget->setObjectName("hintsSection");
    hintsWidget->setVisible(false);
    auto *hintsLayout = new QVBoxLayout(hintsWidget);
    hintsLayout->setContentsMargins(24, 0, 24, 24);
    hintsLayout->setSpacing(12);

    auto *hintsHeader = new QWidget;
    auto *hintsHeaderLayout = new QHBoxLayout(hintsHeader);
    hintsHeaderLayout->setContentsMargins(0, 0, 0, 0);

    auto *hintsIcon = new QLabel("💡");
    auto *hintsTitle = new QLabel("Hints");
    hintsTitle->setObjectName("sectionTitle");

    hintsToggle = new QPushButton("Reveal");
    hintsToggle->setObjectName("hintsToggle");
    hintsToggle->setCursor(Qt::PointingHandCursor);
    hintsToggle->setCheckable(true);

    hintsHeaderLayout->addWidget(hintsIcon);
    hintsHeaderLayout->addWidget(hintsTitle);
    hintsHeaderLayout->addStretch();
    hintsHeaderLayout->addWidget(hintsToggle);

    hintsContent = new QWidget;
    hintsContent->setObjectName("hintsContent");
    hintsContent->setVisible(false);
    auto *hintsContentLayout = new QVBoxLayout(hintsContent);
    hintsContentLayout->setContentsMargins(16, 14, 16, 14);
    hintsContentLayout->setSpacing(0);

    connect(hintsToggle, &QPushButton::toggled, this, [this](bool checked) {
        hintsContent->setVisible(checked);
        hintsToggle->setText(checked ? "Hide" : "Reveal");
    });

    hintsLayout->addWidget(hintsHeader);
    hintsLayout->addWidget(hintsContent);

    // ═══════════════════════════════════════════════════════════════════
    // ASSEMBLE LAYOUT
    // ═══════════════════════════════════════════════════════════════════
    contentLayout->addWidget(headerWidget);
    contentLayout->addWidget(descSection);
    contentLayout->addWidget(taskOuter);
    contentLayout->addWidget(formatSection);
    contentLayout->addWidget(constraintsWidget);
    contentLayout->addWidget(sampleSection);
    contentLayout->addWidget(hintsWidget);
    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

QString ProblemPanel::buildStyleSheet()
{
    return R"(
        /* ═══════════════════════════════════════════════════════════════
           MODERN FLAT DESIGN - PROBLEM PANEL
           ═══════════════════════════════════════════════════════════════ */

        #problemPanel {
            background-color: #0a0a0a;
        }

        #problemScrollArea {
            background: transparent;
            border: none;
        }

        #problemScrollArea QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0;
        }

        #problemScrollArea QScrollBar::handle:vertical {
            background: rgba(255, 255, 255, 0.1);
            border-radius: 4px;
            min-height: 40px;
        }

        #problemScrollArea QScrollBar::handle:vertical:hover {
            background: rgba(255, 255, 255, 0.2);
        }

        #problemScrollArea QScrollBar::add-line:vertical,
        #problemScrollArea QScrollBar::sub-line:vertical,
        #problemScrollArea QScrollBar::add-page:vertical,
        #problemScrollArea QScrollBar::sub-page:vertical {
            background: transparent;
            height: 0;
            border: none;
        }

        #problemContent {
            background: #0a0a0a;
        }

        /* ─── Header ─── */
        #headerSection {
            background: #0a0a0a;
            border-bottom: 1px solid #1a1a1a;
        }

        #titleLabel {
            font-size: 22px;
            font-weight: 700;
            color: #f5f5f5;
            letter-spacing: -0.5px;
        }

        #idLabel {
            font-size: 12px;
            color: #555;
            font-family: 'SF Mono', 'JetBrains Mono', monospace;
        }

        #categoryLabel {
            font-size: 12px;
            color: #666;
            padding-left: 12px;
            border-left: 1px solid #333;
        }

        #difficultyBadge {
            padding: 5px 12px;
            border-radius: 6px;
            font-size: 11px;
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.8px;
        }

        #difficultyBadge[difficulty="easy"] {
            background: #052e16;
            color: #4ade80;
        }

        #difficultyBadge[difficulty="medium"] {
            background: #422006;
            color: #fb923c;
        }

        #difficultyBadge[difficulty="hard"] {
            background: #450a0a;
            color: #f87171;
        }

        /* ─── Tags ─── */
        #tagsContainer {
            min-height: 28px;
        }

        /* ─── Section Titles ─── */
        #sectionTitle {
            font-size: 14px;
            font-weight: 600;
            color: #888;
            letter-spacing: 0.3px;
        }

        /* ─── Content ─── */
        #contentLabel {
            color: #aaa;
            font-size: 14px;
            line-height: 24px;
        }

        /* ─── Task Section ─── */
        #taskSection {
            background: #0f1a0f;
            border: 1px solid #1a3a1a;
            border-radius: 10px;
        }

        #taskIcon {
            font-size: 16px;
        }

        #taskTitle {
            font-size: 14px;
            font-weight: 600;
            color: #4ade80;
        }

        #taskLabel {
            color: #9ca3af;
            font-size: 14px;
            line-height: 24px;
        }

        /* ─── Format Boxes ─── */
        #formatBox {
            background: #111;
            border: 1px solid #1a1a1a;
            border-radius: 10px;
        }

        #formatTitle {
            font-size: 13px;
            font-weight: 600;
            color: #777;
        }

        #formatLabel {
            color: #999;
            font-size: 13px;
            line-height: 22px;
        }

        /* ─── Constraints ─── */
        #constraintItem {
            color: #f59e0b;
            font-size: 13px;
            padding: 6px 0 6px 16px;
            font-family: 'SF Mono', 'JetBrains Mono', monospace;
        }

        /* ─── I/O Boxes ─── */
        #ioBox, #ioBoxOutput {
            background: #0f0f0f;
            border: 1px solid #1a1a1a;
            border-radius: 10px;
            overflow: hidden;
        }

        #ioHeader {
            background: #151515;
            border-bottom: 1px solid #1a1a1a;
        }

        #ioHeaderOutput {
            background: #0f1a0f;
            border-bottom: 1px solid #1a3a1a;
        }

        #ioHeaderLabel {
            font-size: 11px;
            font-weight: 600;
            color: #555;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        #ioContent {
            padding: 14px 16px;
            font-family: 'SF Mono', 'JetBrains Mono', 'Fira Code', monospace;
            font-size: 13px;
            line-height: 22px;
            color: #7dd3fc;
        }

        #ioContentOutput {
            padding: 14px 16px;
            font-family: 'SF Mono', 'JetBrains Mono', 'Fira Code', monospace;
            font-size: 13px;
            line-height: 22px;
            color: #4ade80;
        }

        /* ─── Explanation ─── */
        #explanationLabel {
            color: #666;
            font-size: 13px;
            line-height: 22px;
            padding: 12px 16px;
            background: #0f0f0f;
            border-radius: 8px;
            border-left: 3px solid #3b82f6;
        }

        /* ─── Hints ─── */
        #hintsToggle {
            background: #1a1a1a;
            color: #666;
            border: none;
            border-radius: 6px;
            padding: 6px 14px;
            font-size: 12px;
            font-weight: 500;
        }

        #hintsToggle:hover {
            background: #222;
            color: #888;
        }

        #hintsToggle:checked {
            background: #1e3a5f;
            color: #60a5fa;
        }

        #hintsContent {
            background: #0f0f0f;
            border: 1px solid #1a1a1a;
            border-radius: 10px;
        }

        #hintItem {
            color: #888;
            font-size: 13px;
            padding: 10px 0;
            border-bottom: 1px solid #1a1a1a;
            line-height: 20px;
        }
    )";
}

QString ProblemPanel::buildHtmlWrapper(const QString &content, const QString &additionalCss)
{
    // Base CSS for HTML content
    QString baseCss = R"(
        * { margin: 0; padding: 0; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            line-height: 1.7;
            color: #aaa;
        }
        code {
            background: #1a1a1a;
            padding: 2px 7px;
            border-radius: 4px;
            color: #7dd3fc;
            font-family: 'SF Mono', 'JetBrains Mono', monospace;
            font-size: 0.9em;
        }
        strong, b { color: #ddd; font-weight: 600; }
        em, i { color: #bbb; }

        .io-line {
            padding: 2px 0;
            font-family: 'SF Mono', 'JetBrains Mono', monospace;
        }
        .io-line.comment {
            color: #555;
            font-style: italic;
            font-size: 0.85em;
        }
        .io-line.highlight-green {
            color: #4ade80;
            font-weight: 600;
        }
        .io-line.highlight-red {
            color: #f87171;
            font-weight: 600;
        }
        .io-line.output {
            color: #4ade80;
        }

        .input-table {
            width: 100%;
            border-collapse: collapse;
        }
        .input-table td {
            padding: 6px 0;
            vertical-align: top;
        }
        .input-table .line-num {
            color: #666;
            font-family: 'SF Mono', monospace;
            font-size: 0.9em;
            width: 100px;
            padding-right: 12px;
        }
    )";

    return QString("<html><head><style>%1 %2</style></head><body>%3</body></html>")
        .arg(baseCss, additionalCss, content);
}

QString ProblemPanel::formatText(const QString &text)
{
    QString result = text;

    // Replace \n with <br> for line breaks
    result.replace("\\n", "<br>");
    result.replace("\n", "<br>");

    return result;
}

void ProblemPanel::loadFromJson(const QJsonObject &obj)
{
    // ─── Store Problem Data ───
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

    // ─── Update Header ───
    titleLabel->setText(problemTitle);
    idLabel->setText(problemId);
    categoryLabel->setText(category.isEmpty() ? "General" : category);

    // Difficulty badge
    difficultyBadge->setText(obj["difficulty"].toString());
    difficultyBadge->setProperty("difficulty", difficulty);
    difficultyBadge->setAlignment(Qt::AlignCenter);

    QFontMetrics fm(difficultyBadge->font());
    int textWidth = fm.horizontalAdvance(difficultyBadge->text());
    difficultyBadge->setFixedSize(textWidth + 24, 24);
    difficultyBadge->style()->unpolish(difficultyBadge);
    difficultyBadge->style()->polish(difficultyBadge);

    // ─── Update Tags ───
    QLayout *tagsLayout = tagsContainer->layout();
    QLayoutItem *item;
    while ((item = tagsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    auto *tagsHLayout = qobject_cast<QHBoxLayout*>(tagsLayout);
    for (const QString &tag : tags) {
        auto *tagLabel = new QLabel(tag);
        tagLabel->setStyleSheet(R"(
            background-color: hsla(90, 80%, 60%, 0.15);
            color: hsl(90, 80%, 70%);
            padding: 4px 10px;
            border-radius: 6px;
            font-size: 11px;
            font-weight: 500;
            border: 1px solid hsla(90, 80%, 60%, 0.15);

        )");
        tagsHLayout->addWidget(tagLabel);
    }
    tagsHLayout->addStretch();

    // ─── Update Description ───
    QString descText = obj["description"].toString();
    descriptionLabel->setText(buildHtmlWrapper(formatText(descText)));

    // ─── Update Task ───
    QString taskText = obj["task"].toString();
    taskLabel->setText(buildHtmlWrapper(formatText(taskText), "body { color: #9ca3af; }"));

    // ─── Update Input Format ───
    QString inputText = obj["inputFormat"].toString();
    inputFormatLabel->setText(buildHtmlWrapper(formatText(inputText), "body { color: #999; }"));

    // ─── Update Output Format ───
    QString outputText = obj["outputFormat"].toString();
    outputFormatLabel->setText(buildHtmlWrapper(formatText(outputText), "body { color: #999; }"));

    // ─── Update Constraints ───
    QJsonArray constraints = obj["constraints"].toArray();
    QLayout *constraintsLayout = constraintsWidget->layout();

    // Remove old items (keep header at index 0)
    while (constraintsLayout->count() > 1) {
        QLayoutItem *item = constraintsLayout->takeAt(1);
        if (item->widget()) delete item->widget();
        delete item;
    }

    if (constraints.isEmpty()) {
        constraintsWidget->setVisible(false);
    } else {
        constraintsWidget->setVisible(true);

        auto *constraintsList = new QWidget;
        constraintsList->setObjectName("constraintsList");
        auto *listLayout = new QVBoxLayout(constraintsList);
        listLayout->setContentsMargins(16, 8, 0, 0);
        listLayout->setSpacing(0);

        for (const auto &constraint : constraints) {
            auto *constraintLabel = new QLabel;
            constraintLabel->setObjectName("constraintItem");
            constraintLabel->setTextFormat(Qt::RichText);
            constraintLabel->setText(QString("• %1").arg(constraint.toString()));
            constraintLabel->setWordWrap(true);
            listLayout->addWidget(constraintLabel);
        }

        qobject_cast<QVBoxLayout*>(constraintsLayout)->addWidget(constraintsList);
    }

    // ─── Update Sample I/O ───
    QString sampleInput = obj["sampleInput"].toString();
    QString sampleOutput = obj["sampleOutput"].toString();
    QString sampleExplanation = obj["sampleExplanation"].toString();

    // Build I/O HTML
    QString ioCss = R"(
        body {
            font-family: 'SF Mono', 'JetBrains Mono', monospace;
            font-size: 13px;
            line-height: 1.7;
        }
        .io-line { padding: 1px 0; }
        .io-line.comment { color: #555; font-style: italic; font-size: 0.9em; }
        .io-line.highlight-green {
            display: inline-block;
            background: rgba(74, 222, 128, 0.15);
            color: #4ade80;
            padding: 2px 8px;
            border-radius: 4px;
            font-weight: 600;
        }
        .io-line.highlight-red {
            display: inline-block;
            background: rgba(248, 113, 113, 0.15);
            color: #f87171;
            padding: 2px 8px;
            border-radius: 4px;
            font-weight: 600;
        }
        .io-line.output { color: #4ade80; }
    )";

    sampleInputLabel->setText(buildHtmlWrapper(sampleInput, ioCss + " body { color: #7dd3fc; }"));
    sampleOutputLabel->setText(buildHtmlWrapper(sampleOutput, ioCss + " body { color: #4ade80; }"));

    // Explanation
    if (sampleExplanation.isEmpty()) {
        sampleExplanationLabel->setVisible(false);
    } else {
        sampleExplanationLabel->setVisible(true);
        QString explainCss = "body { color: #888; } code { background: #1a1a1a; color: #7dd3fc; }";
        sampleExplanationLabel->setText(buildHtmlWrapper(
            QString("💡 %1").arg(formatText(sampleExplanation)),
            explainCss
            ));
    }

    // ─── Update Hints ───
    QJsonArray hints = obj["hints"].toArray();

    if (hints.isEmpty()) {
        hintsWidget->setVisible(false);
    } else {
        hintsWidget->setVisible(true);
        hintsToggle->setChecked(false);
        hintsContent->setVisible(false);

        QLayout *hintsContentLayout = hintsContent->layout();

        QLayoutItem *hintItem;
        while ((hintItem = hintsContentLayout->takeAt(0)) != nullptr) {
            if (hintItem->widget()) delete hintItem->widget();
            delete hintItem;
        }

        int hintNum = 1;
        for (const auto &hint : hints) {
            auto *hintLabel = new QLabel;
            hintLabel->setObjectName("hintItem");
            hintLabel->setWordWrap(true);
            hintLabel->setTextFormat(Qt::RichText);
            hintLabel->setText(QString("<span style='color:#60a5fa;font-weight:600;'>%1.</span> %2")
                                   .arg(hintNum++)
                                   .arg(hint.toString()));
            qobject_cast<QVBoxLayout*>(hintsContentLayout)->addWidget(hintLabel);
        }
    }

    // ─── Emit Signals ───
    emit problemLoaded(problemId);
    emit testCasesAvailable(cachedTestCases);
}

bool ProblemPanel::loadFromFile(const QString &filePath)
{
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
