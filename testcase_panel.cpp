#include "testcase_panel.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QStyle>

TestCasePanel::TestCasePanel(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("testCasePanel");
    setStyleSheet(buildStyleSheet());
    buildUI();
}

void TestCasePanel::buildUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ─── Header: Testcase | Result tabs ───
    headerWidget = new QWidget;
    headerWidget->setObjectName("header");
    headerWidget->setFixedHeight(36);

    auto *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(12, 0, 12, 0);
    headerLayout->setSpacing(0);

    testcaseTab = new QPushButton("Testcase");
    testcaseTab->setObjectName("modeTab");
    testcaseTab->setProperty("active", true);
    testcaseTab->setCursor(Qt::PointingHandCursor);
    testcaseTab->setCheckable(true);
    testcaseTab->setChecked(true);

    resultTab = new QPushButton("Result");
    resultTab->setObjectName("modeTab");
    resultTab->setProperty("active", false);
    resultTab->setCursor(Qt::PointingHandCursor);
    resultTab->setCheckable(true);

    connect(testcaseTab, &QPushButton::clicked, this, &TestCasePanel::showTestcaseView);
    connect(resultTab, &QPushButton::clicked, this, &TestCasePanel::showResultView);

    headerLayout->addWidget(testcaseTab);
    headerLayout->addWidget(resultTab);
    headerLayout->addStretch();

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
    testcaseLayout->setSpacing(12);

    // Input section
    inputTitleLabel = new QLabel("Input =");
    inputTitleLabel->setObjectName("fieldTitle");

    inputValueLabel = new QLabel;
    inputValueLabel->setObjectName("fieldValue");
    inputValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    inputValueLabel->setWordWrap(false);

    // Expected section
    expectedTitleLabel = new QLabel("Expected =");
    expectedTitleLabel->setObjectName("fieldTitle");

    expectedValueLabel = new QLabel;
    expectedValueLabel->setObjectName("fieldValueExpected");
    expectedValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    testcaseLayout->addWidget(inputTitleLabel);
    testcaseLayout->addWidget(inputValueLabel);
    testcaseLayout->addSpacing(4);
    testcaseLayout->addWidget(expectedTitleLabel);
    testcaseLayout->addWidget(expectedValueLabel);
    testcaseLayout->addStretch();

    // ─── Result View ───
    resultView = new QWidget;
    resultView->setObjectName("contentView");
    auto *resultLayout = new QVBoxLayout(resultView);
    resultLayout->setContentsMargins(16, 12, 16, 16);
    resultLayout->setSpacing(10);

    // Status
    resultStatusLabel = new QLabel;
    resultStatusLabel->setObjectName("resultStatus");

    // Output
    outputTitleLabel = new QLabel("Output =");
    outputTitleLabel->setObjectName("fieldTitle");

    outputValueLabel = new QLabel;
    outputValueLabel->setObjectName("fieldValue");
    outputValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // Expected
    expectedResultTitleLabel = new QLabel("Expected =");
    expectedResultTitleLabel->setObjectName("fieldTitle");

    expectedResultValueLabel = new QLabel;
    expectedResultValueLabel->setObjectName("fieldValueExpected");
    expectedResultValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    resultLayout->addWidget(resultStatusLabel);
    resultLayout->addSpacing(6);
    resultLayout->addWidget(outputTitleLabel);
    resultLayout->addWidget(outputValueLabel);
    resultLayout->addSpacing(4);
    resultLayout->addWidget(expectedResultTitleLabel);
    resultLayout->addWidget(expectedResultValueLabel);
    resultLayout->addStretch();

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
}

QString TestCasePanel::buildStyleSheet()
{
    return R"(
        #testCasePanel {
            background: #1a1a1a;
            border-top: 1px solid #333;
        }

        #header {
            background: #1a1a1a;
            border-bottom: 1px solid #2a2a2a;
        }

        #modeTab {
            background: transparent;
            border: none;
            color: #666;
            font-size: 13px;
            font-weight: 500;
            padding: 8px 16px;
            margin-right: 4px;
        }

        #modeTab:hover {
            color: #999;
        }

        #modeTab:checked {
            color: #fff;
        }

        #caseTabBar {
            background: #1a1a1a;
            border-bottom: 1px solid #2a2a2a;
        }

        #caseTabBar::tab {
            background: transparent;
            color: #666;
            border: none;
            padding: 8px 16px;
            margin: 0 2px;
            font-size: 12px;
        }

        #caseTabBar::tab:selected {
            color: #fff;
            background: #2a2a2a;
            border-radius: 4px 4px 0 0;
        }

        #caseTabBar::tab:hover:!selected {
            color: #999;
        }

        #contentView {
            background: #1a1a1a;
        }

        #contentStack {
            background: #1a1a1a;
        }

        QScrollArea#contentScrollArea {
            background: #1a1a1a;
            border: none;
        }

        #fieldTitle {
            color: #666;
            font-size: 12px;
            font-weight: 500;
        }

        #fieldValue {
            color: #79c0ff;
            font-family: 'JetBrains Mono', 'SF Mono', 'Consolas', monospace;
            font-size: 13px;
            line-height: 1.5;
            background: #0d0d0d;
            padding: 10px 12px;
            border-radius: 6px;
        }

        #fieldValueExpected {
            color: #7ee787;
            font-family: 'JetBrains Mono', 'SF Mono', 'Consolas', monospace;
            font-size: 13px;
            line-height: 1.5;
            background: #0d0d0d;
            padding: 10px 12px;
            border-radius: 6px;
        }

        #resultStatus {
            font-size: 14px;
            font-weight: 600;
            padding: 8px 0;
        }

        #resultStatus[status="passed"] {
            color: #7ee787;
        }

        #resultStatus[status="failed"] {
            color: #f85149;
        }

        #resultStatus[status="running"] {
            color: #d29922;
        }

        #resultStatus[status="pending"] {
            color: #666;
        }
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

    // Format text for display (convert \n to visual line breaks)
    auto formatCode = [](const QString &text) -> QString {
        QString html = text;
        html.replace("&", "&amp;");
        html.replace("<", "&lt;");
        html.replace(">", "&gt;");
        html.replace(" ", "&nbsp;");
        html.replace("\n", "<br>");
        return QString("<div style='white-space:pre;'>%1</div>").arg(html);
    };

    // Update testcase view
    inputValueLabel->setText(formatCode(data.input));
    inputValueLabel->setTextFormat(Qt::RichText);

    expectedValueLabel->setText(formatCode(data.expectedOutput));
    expectedValueLabel->setTextFormat(Qt::RichText);

    // Update result view
    expectedResultValueLabel->setText(formatCode(data.expectedOutput));
    expectedResultValueLabel->setTextFormat(Qt::RichText);

    // Reset output style
    outputValueLabel->setStyleSheet("");

    if (data.status == TestCaseData::Pending) {
        resultStatusLabel->setText("Click Run to see result");
        resultStatusLabel->setProperty("status", "pending");
        outputValueLabel->setText("—");
    } else if (data.status == TestCaseData::Running) {
        resultStatusLabel->setText("⟳ Running...");
        resultStatusLabel->setProperty("status", "running");
        outputValueLabel->setText("...");
    } else if (data.status == TestCaseData::Passed) {
        resultStatusLabel->setText("✓ Accepted");
        resultStatusLabel->setProperty("status", "passed");
        outputValueLabel->setText(formatCode(data.actualOutput));
        outputValueLabel->setTextFormat(Qt::RichText);
    } else {
        resultStatusLabel->setText("✗ Wrong Answer");
        resultStatusLabel->setProperty("status", "failed");
        outputValueLabel->setText(formatCode(data.actualOutput));
        outputValueLabel->setTextFormat(Qt::RichText);
        outputValueLabel->setStyleSheet("#fieldValue { color: #f85149; }");
    }

    // Refresh styles
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
        data.input = testCase["input"].toString();
        data.expectedOutput = testCase["output"].toString();
        data.status = TestCaseData::Pending;

        testCaseData[caseNum] = data;
        caseTabBar->addTab(QString("Case %1").arg(caseNum + 1));

        caseNum++;
    }

    if (caseNum > 0) {
        caseTabBar->setCurrentIndex(0);
        updateContent(0);
    }
}

void TestCasePanel::clearTestCases()
{
    while (caseTabBar->count() > 0) {
        caseTabBar->removeTab(0);
    }
    testCaseData.clear();
    currentCaseIndex = 0;
}

void TestCasePanel::setTestResult(int caseIndex, const QString &actualOutput, bool passed)
{
    if (!testCaseData.contains(caseIndex)) return;

    testCaseData[caseIndex].actualOutput = actualOutput;
    testCaseData[caseIndex].status = passed ? TestCaseData::Passed : TestCaseData::Failed;

    // Update tab text with status
    QString tabText = passed
                          ? QString("✓ Case %1").arg(caseIndex + 1)
                          : QString("✗ Case %1").arg(caseIndex + 1);
    caseTabBar->setTabText(caseIndex, tabText);

    // If viewing this case, update display
    if (caseIndex == currentCaseIndex) {
        updateContent(caseIndex);
    }

    // Auto-switch to Result view
    showResultView();
}

void TestCasePanel::setTestRunning(int caseIndex)
{
    if (!testCaseData.contains(caseIndex)) return;

    testCaseData[caseIndex].status = TestCaseData::Running;
    testCaseData[caseIndex].actualOutput.clear();
    caseTabBar->setTabText(caseIndex, QString("◌ Case %1").arg(caseIndex + 1));

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
    }

    updateContent(currentCaseIndex);
    showTestcaseView();
}

void TestCasePanel::resetTestResult(int index)
{
    if (!testCaseData.contains(index)) return;

    testCaseData[index].actualOutput.clear();
    testCaseData[index].status = TestCaseData::Pending;
    caseTabBar->setTabText(index, QString("Case %1").arg(index + 1));

    if (index == currentCaseIndex) {
        updateContent(index);
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
    if (testCaseData.contains(index)) {
        return testCaseData[index].status == TestCaseData::Passed;
    }
    return false;
}
