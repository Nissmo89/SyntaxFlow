#ifndef TESTCASE_PANEL_H
#define TESTCASE_PANEL_H

#include <QWidget>
#include <QTabBar>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QMap>

struct TestCaseData {
    QString input;
    QString expectedOutput;
    QString actualOutput;
    enum Status { Pending, Running, Passed, Failed } status = Pending;
};

// ═══════════════════════════════════════════════════════════════════════════
// TestCasePanel - Minimal LeetCode-style panel
// ═══════════════════════════════════════════════════════════════════════════

class TestCasePanel : public QWidget {
    Q_OBJECT
public:
    explicit TestCasePanel(QWidget *parent = nullptr);

    void loadTestCases(const QJsonArray &testCases);
    void clearTestCases();

    void setTestResult(int caseIndex, const QString &actualOutput, bool passed);
    void setTestRunning(int caseIndex);
    void clearAllResults();
    void resetTestResult(int index);

    int getTestCaseCount() const { return testCaseData.size(); }
    int getCurrentIndex() const;
    TestCaseData getTestCase(int index) const;
    TestCaseData getCurrentTestCase() const;
    bool isTestPassed(int index) const;

signals:
    void runRequested();
    void submitRequested();

private slots:
    void onTabChanged(int index);

private:
    void buildUI();
    QString buildStyleSheet();
    void updateContent(int index);
    void showTestcaseView();
    void showResultView();

    // Mode: Testcase or Result
    enum Mode { TestcaseMode, ResultMode };
    Mode currentMode = TestcaseMode;

    // Header
    QWidget *headerWidget;
    QPushButton *testcaseTab;
    QPushButton *resultTab;

    // Case tabs
    QTabBar *caseTabBar;

    // Content
    QStackedWidget *contentStack;

    // Testcase view
    QWidget *testcaseView;
    QLabel *inputTitleLabel;
    QLabel *inputValueLabel;
    QLabel *expectedTitleLabel;
    QLabel *expectedValueLabel;

    // Result view
    QWidget *resultView;
    QLabel *resultStatusLabel;
    QLabel *outputTitleLabel;
    QLabel *outputValueLabel;
    QLabel *expectedResultTitleLabel;
    QLabel *expectedResultValueLabel;

    // Data
    QMap<int, TestCaseData> testCaseData;
    int currentCaseIndex = 0;
};

#endif // TESTCASE_PANEL_H
