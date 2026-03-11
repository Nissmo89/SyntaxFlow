#include "testcase_panel.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QStyle>
#include <QTimer>

TestCasePanel::TestCasePanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("testCasePanel");
    setStyleSheet(buildStyleSheet());
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    buildUI();
}

QSize TestCasePanel::sizeHint() const
{
    // Header (38) + caseTabBar + current content view's natural height
    int h = 38; // header fixed height
    if (caseTabBar)
        h += caseTabBar->sizeHint().height();

    // Get the currently visible content widget's ideal height
    QWidget *visibleContent = contentStack->currentWidget();
    if (visibleContent)
        h += visibleContent->sizeHint().height();

    return QSize(QWidget::sizeHint().width(), h);
}

void TestCasePanel::buildUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ─── Header: Testcase | Result tabs + summary ───
    headerWidget = new QWidget;
    headerWidget->setObjectName("header");
    headerWidget->setFixedHeight(38);

    auto *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 0, 12, 0);
    headerLayout->setSpacing(0);

    testcaseTab = new QPushButton("Testcase");
    testcaseTab->setObjectName("modeTab");
    testcaseTab->setCursor(Qt::PointingHandCursor);
    testcaseTab->setCheckable(true);
    testcaseTab->setChecked(true);

    resultTab = new QPushButton("Result");
    resultTab->setObjectName("modeTab");
    resultTab->setCursor(Qt::PointingHandCursor);
    resultTab->setCheckable(true);

    connect(testcaseTab, &QPushButton::clicked, this, &TestCasePanel::showTestcaseView);
    connect(resultTab,   &QPushButton::clicked, this, &TestCasePanel::showResultView);

    headerLayout->addWidget(testcaseTab);
    headerLayout->addWidget(resultTab);
    headerLayout->addStretch();

    // Summary chip (hidden by default, shown after running)
    m_summaryLabel = new QLabel(this);
    m_summaryLabel->setObjectName("summaryLabel");
    m_summaryLabel->setVisible(false);
    headerLayout->addWidget(m_summaryLabel);

    // ─── Case Tab Bar ───
    caseTabBar = new QTabBar;
    caseTabBar->setObjectName("caseTabBar");
    caseTabBar->setExpanding(false);
    caseTabBar->setDrawBase(false);
    connect(caseTabBar, &QTabBar::currentChanged, this, &TestCasePanel::onTabChanged);

    // ─── Content Stack ───
    contentStack = new QStackedWidget;
    contentStack->setObjectName("contentStack");

    // ─── Testcase View ───
    testcaseView = new QWidget;
    testcaseView->setObjectName("contentView");
    auto *testcaseLayout = new QVBoxLayout(testcaseView);
    testcaseLayout->setContentsMargins(16, 12, 16, 16);
    testcaseLayout->setSpacing(10);

    inputTitleLabel = new QLabel("Input");
    inputTitleLabel->setObjectName("fieldTitle");

    inputValueLabel = new QLabel;
    inputValueLabel->setObjectName("fieldValue");
    inputValueLabel->setProperty("accent", "blue");
    inputValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    inputValueLabel->setWordWrap(false);

    expectedTitleLabel = new QLabel("Expected Output");
    expectedTitleLabel->setObjectName("fieldTitle");

    expectedValueLabel = new QLabel;
    expectedValueLabel->setObjectName("fieldValue");
    expectedValueLabel->setProperty("accent", "green");
    expectedValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    testcaseLayout->addWidget(inputTitleLabel);
    testcaseLayout->addWidget(inputValueLabel);
    testcaseLayout->addSpacing(4);
    testcaseLayout->addWidget(expectedTitleLabel);
    testcaseLayout->addWidget(expectedValueLabel);

    // ─── Result View ───
    resultView = new QWidget;
    resultView->setObjectName("contentView");
    auto *resultLayout = new QVBoxLayout(resultView);
    resultLayout->setContentsMargins(16, 12, 16, 16);
    resultLayout->setSpacing(10);

    // Status bar area
    auto *statusRow = new QHBoxLayout;
    statusRow->setContentsMargins(0, 0, 0, 0);
    resultStatusLabel = new QLabel;
    resultStatusLabel->setObjectName("resultStatus");
    statusRow->addWidget(resultStatusLabel);
    statusRow->addStretch();

    outputTitleLabel = new QLabel("Your Output");
    outputTitleLabel->setObjectName("fieldTitle");

    outputValueLabel = new QLabel;
    outputValueLabel->setObjectName("fieldValue");
    outputValueLabel->setProperty("accent", "blue");
    outputValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    expectedResultTitleLabel = new QLabel("Expected Output");
    expectedResultTitleLabel->setObjectName("fieldTitle");

    expectedResultValueLabel = new QLabel;
    expectedResultValueLabel->setObjectName("fieldValue");
    expectedResultValueLabel->setProperty("accent", "green");
    expectedResultValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    resultLayout->addLayout(statusRow);
    resultLayout->addSpacing(4);
    resultLayout->addWidget(outputTitleLabel);
    resultLayout->addWidget(outputValueLabel);
    resultLayout->addSpacing(4);
    resultLayout->addWidget(expectedResultTitleLabel);
    resultLayout->addWidget(expectedResultValueLabel);

    contentStack->addWidget(testcaseView);
    contentStack->addWidget(resultView);

    // ─── Scroll Area ───
    auto *scrollArea = new QScrollArea;
    scrollArea->setObjectName("contentScrollArea");
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(contentStack);


    // ─── Assemble ───
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(caseTabBar);
    mainLayout->addWidget(scrollArea, 1);

    // Running pulse timer
    m_pulseTimer = new QTimer(this);
    m_pulseTimer->setInterval(600);
    connect(m_pulseTimer, &QTimer::timeout, this, [this]() {
        m_pulseBright = !m_pulseBright;
        if (m_isRunning) {
            resultStatusLabel->setStyleSheet(m_pulseBright
                ? "color: #ffc01e; font-size: 14px; font-weight: 600;"
                : "color: #886800; font-size: 14px; font-weight: 600;");
        }
    });
}

QString TestCasePanel::buildStyleSheet()
{
    return R"(
        #testCasePanel {
            background: #161616;
            border-top: 1px solid #252525;
        }

        #header {
            background: #161616;
            border-bottom: 1px solid #222;
        }

        /* Mode tabs: Testcase / Result */
        #modeTab {
            background: transparent;
            border: none;
            border-bottom: 2px solid transparent;
            color: #555;
            font-size: 13px;
            font-weight: 500;
            padding: 8px 16px;
            margin-right: 2px;
            font-family: 'Segoe UI', sans-serif;
        }
        #modeTab:hover {
            color: #888;
        }
        #modeTab:checked {
            color: #e8e8e8;
            border-bottom: 2px solid #58a6ff;
        }

        /* Summary chip */
        #summaryLabel {
            font-family: 'Consolas', monospace;
            font-size: 11px;
            padding: 3px 10px;
            border-radius: 3px;
            margin-right: 4px;
        }

        /* Case tab bar */
        QTabBar#caseTabBar {
            background: #161616;
            border-bottom: 1px solid #222;
        }
        QTabBar#caseTabBar::tab {
            background: transparent;
            color: #555;
            border: none;
            border-bottom: 2px solid transparent;
            padding: 7px 16px;
            margin: 0 1px;
            font-size: 12px;
            font-family: 'Consolas', monospace;
        }
        QTabBar#caseTabBar::tab:selected {
            color: #ccc;
            border-bottom: 2px solid #3a3a3a;
        }
        QTabBar#caseTabBar::tab:hover:!selected {
            color: #888;
        }

        #contentView {
            background: #161616;
        }
        #contentStack {
            background: #161616;
        }
        QScrollArea#contentScrollArea {
            background: #161616;
            border: none;
        }

        /* Section label above each code box */
        #fieldTitle {
            color: #555;
            font-size: 11px;
            font-weight: 600;
            font-family: 'Segoe UI', sans-serif;
            letter-spacing: 0.5px;
            text-transform: uppercase;
            padding: 0 2px;
            background: transparent;
        }

        /* Code boxes */
        #fieldValue {
            font-family: 'JetBrains Mono', 'Consolas', monospace;
            font-size: 13px;
            background: #0d0d0d;
            padding: 10px 14px;
            border-radius: 5px;
            border: 1px solid #252525;
        }
        #fieldValue[accent="blue"] {
            color: #79c0ff;
            border-left: 3px solid #1d4e7e;
        }
        #fieldValue[accent="green"] {
            color: #7ee787;
            border-left: 3px solid #1a5c2a;
        }
        #fieldValue[accent="red"] {
            color: #f85149;
            border-left: 3px solid #6b1a1a;
        }

        /* Result status label */
        #resultStatus {
            font-size: 15px;
            font-weight: 700;
            padding: 4px 0;
            font-family: 'Segoe UI', sans-serif;
        }
        #resultStatus[status="passed"] { color: #7ee787; }
        #resultStatus[status="failed"]  { color: #f85149; }
        #resultStatus[status="running"] { color: #ffc01e; }
        #resultStatus[status="pending"] { color: #555; }
    )";
}

void TestCasePanel::showTestcaseView()
{
    currentMode = TestcaseMode;
    testcaseTab->setChecked(true);
    resultTab->setChecked(false);
    contentStack->setCurrentWidget(testcaseView);
}

void TestCasePanel::showResultView()
{
    currentMode = ResultMode;
    testcaseTab->setChecked(false);
    resultTab->setChecked(true);
    contentStack->setCurrentWidget(resultView);
}

void TestCasePanel::onTabChanged(int index)
{
    if (index >= 0 && index < testCaseData.size()) {
        currentCaseIndex = index;
        updateContent(index);
    }
}

void TestCasePanel::updateContent(int index)
{
    if (!testCaseData.contains(index)) return;

    const TestCaseData &data = testCaseData[index];

    auto formatCode = [](const QString &text) -> QString {
        QString html = text;
        html.replace("&", "&amp;");
        html.replace("<", "&lt;");
        html.replace(">", "&gt;");
        html.replace(" ", "&nbsp;");
        html.replace("\n", "<br>");
        return QString("<div style='white-space:pre;'>%1</div>").arg(html);
    };

    inputValueLabel->setText(formatCode(data.input));
    inputValueLabel->setTextFormat(Qt::RichText);

    expectedValueLabel->setText(formatCode(data.expectedOutput));
    expectedValueLabel->setTextFormat(Qt::RichText);

    expectedResultValueLabel->setText(formatCode(data.expectedOutput));
    expectedResultValueLabel->setTextFormat(Qt::RichText);

    // Reset output
    outputValueLabel->setProperty("accent", "blue");
    outputValueLabel->style()->unpolish(outputValueLabel);
    outputValueLabel->style()->polish(outputValueLabel);

    m_isRunning = false;
    m_pulseTimer->stop();

    if (data.status == TestCaseData::Pending) {
        resultStatusLabel->setText("Click Run to see result");
        resultStatusLabel->setProperty("status", "pending");
        outputValueLabel->setText("—");
    } else if (data.status == TestCaseData::Running) {
        resultStatusLabel->setText("⟳  Running...");
        resultStatusLabel->setProperty("status", "running");
        outputValueLabel->setText("…");
        m_isRunning = true;
        m_pulseTimer->start();
    } else if (data.status == TestCaseData::Passed) {
        resultStatusLabel->setText("✓  Accepted");
        resultStatusLabel->setProperty("status", "passed");
        outputValueLabel->setProperty("accent", "green");
        outputValueLabel->style()->unpolish(outputValueLabel);
        outputValueLabel->style()->polish(outputValueLabel);
        outputValueLabel->setText(formatCode(data.actualOutput));
        outputValueLabel->setTextFormat(Qt::RichText);
    } else {
        resultStatusLabel->setText("✗  Wrong Answer");
        resultStatusLabel->setProperty("status", "failed");
        outputValueLabel->setProperty("accent", "red");
        outputValueLabel->style()->unpolish(outputValueLabel);
        outputValueLabel->style()->polish(outputValueLabel);
        outputValueLabel->setText(formatCode(data.actualOutput));
        outputValueLabel->setTextFormat(Qt::RichText);
    }

    // Refresh status label style
    resultStatusLabel->style()->unpolish(resultStatusLabel);
    resultStatusLabel->style()->polish(resultStatusLabel);
}

void TestCasePanel::loadTestCases(const QJsonArray &testCases)
{
    clearTestCases();

    int caseNum = 0;
    for (const auto &tc : testCases) {
        QJsonObject testCase = tc.toObject();

        TestCaseData data;
        data.input          = testCase["input"].toString();
        data.expectedOutput = testCase["output"].toString();
        data.status         = TestCaseData::Pending;

        testCaseData[caseNum] = data;
        caseTabBar->addTab(QString("Case %1").arg(caseNum + 1));
        caseNum++;
    }

    if (caseNum > 0) {
        caseTabBar->setCurrentIndex(0);
        updateContent(0);
    }

    m_summaryLabel->setVisible(false);
    updateGeometry();
}

void TestCasePanel::clearTestCases()
{
    while (caseTabBar->count() > 0)
        caseTabBar->removeTab(0);
    testCaseData.clear();
    currentCaseIndex = 0;
}

void TestCasePanel::setTestResult(int caseIndex, const QString &actualOutput, bool passed)
{
    if (!testCaseData.contains(caseIndex)) return;

    testCaseData[caseIndex].actualOutput = actualOutput;
    testCaseData[caseIndex].status = passed ? TestCaseData::Passed : TestCaseData::Failed;

    // Update tab label
    QString tabText = passed
        ? QString("✓ Case %1").arg(caseIndex + 1)
        : QString("✗ Case %1").arg(caseIndex + 1);
    caseTabBar->setTabText(caseIndex, tabText);

    // Color the tab
    caseTabBar->setTabTextColor(caseIndex,
        passed ? QColor("#7ee787") : QColor("#f85149"));

    if (caseIndex == currentCaseIndex) {
        updateContent(caseIndex);
    }

    showResultView();
    updateSummary();
    updateGeometry();
}

void TestCasePanel::setTestRunning(int caseIndex)
{
    if (!testCaseData.contains(caseIndex)) return;

    testCaseData[caseIndex].status = TestCaseData::Running;
    testCaseData[caseIndex].actualOutput.clear();
    caseTabBar->setTabText(caseIndex, QString("◌ Case %1").arg(caseIndex + 1));
    caseTabBar->setTabTextColor(caseIndex, QColor("#ffc01e"));

    if (caseIndex == currentCaseIndex) {
        updateContent(caseIndex);
    }
    showResultView();
}

void TestCasePanel::clearAllResults()
{
    for (int i = 0; i < testCaseData.size(); ++i) {
        testCaseData[i].actualOutput.clear();
        testCaseData[i].status = TestCaseData::Pending;
        caseTabBar->setTabText(i, QString("Case %1").arg(i + 1));
        caseTabBar->setTabTextColor(i, QColor("#555"));
    }
    m_summaryLabel->setVisible(false);
    m_isRunning = false;
    m_pulseTimer->stop();
    updateContent(currentCaseIndex);
    showTestcaseView();
}

void TestCasePanel::resetTestResult(int index)
{
    if (!testCaseData.contains(index)) return;

    testCaseData[index].actualOutput.clear();
    testCaseData[index].status = TestCaseData::Pending;
    caseTabBar->setTabText(index, QString("Case %1").arg(index + 1));
    caseTabBar->setTabTextColor(index, QColor("#555"));

    if (index == currentCaseIndex) {
        updateContent(index);
    }
}

void TestCasePanel::updateSummary()
{
    int total  = testCaseData.size();
    int passed = 0;
    int done   = 0;

    for (int i = 0; i < total; ++i) {
        auto s = testCaseData[i].status;
        if (s == TestCaseData::Passed)  { passed++; done++; }
        if (s == TestCaseData::Failed)  { done++; }
    }

    if (done == 0) {
        m_summaryLabel->setVisible(false);
        return;
    }

    m_summaryLabel->setText(QString("%1 / %2 Passed").arg(passed).arg(total));
    m_summaryLabel->setVisible(true);

    if (passed == total) {
        m_summaryLabel->setStyleSheet(R"(
            #summaryLabel {
                color: #7ee787;
                background: rgba(46,160,67,18);
                border: 1px solid rgba(46,160,67,50);
                border-radius: 3px;
                font-family: 'Consolas', monospace;
                font-size: 11px;
                padding: 3px 10px;
            }
        )");
    } else {
        m_summaryLabel->setStyleSheet(R"(
            #summaryLabel {
                color: #f85149;
                background: rgba(248,81,73,12);
                border: 1px solid rgba(248,81,73,40);
                border-radius: 3px;
                font-family: 'Consolas', monospace;
                font-size: 11px;
                padding: 3px 10px;
            }
        )");
    }
}

int TestCasePanel::getCurrentIndex() const
{
    return currentCaseIndex;
}

TestCaseData TestCasePanel::getTestCase(int index) const
{
    return testCaseData.value(index);
}

TestCaseData TestCasePanel::getCurrentTestCase() const
{
    return testCaseData.value(currentCaseIndex);
}

bool TestCasePanel::isTestPassed(int index) const
{
    if (testCaseData.contains(index))
        return testCaseData[index].status == TestCaseData::Passed;
    return false;
}
