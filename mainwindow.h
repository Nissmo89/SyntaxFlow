#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

// #include "ProgressManager.h"
#include "progressmanager.h"

class QStackedLayout;
class CodeEditor;
class ProblemBrowser;
class ProblemPanel;
class TestCasePanel;
class HoverSidebar;
class Backend;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void applyStyle(const QString &path);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // Navigation
    void onNavigateToEditor(const QString &path);
    void onNavigateToBrowser();

    // Code Execution
    void onRunCurrentTest();
    void onRunAllTests();
    void onStopExecution();

    // Backend Results
    void onTestResult(int testIndex, const QString &status, const QString &output,
                      const QString &expected, qint64 timeMs);
    void onCompilationError(const QString &error);
    void onSystemError(const QString &error);
    void onExecutionStarted();
    void onExecutionFinished();

    // Language
    void onLanguageChanged(int index);
    void populateLanguages();

    // Editor
    void onEditorCursorChanged(int line, int column);

private:
    // Setup
    void setupUI();
    void setupBrowserPage();
    void setupEditorPage();
    void setupSidebar();
    void setupConnections();
    void setupShortcuts();
    void setupBackend();

    // UI State
    void setExecutionState(bool running);
    void updateLanguageIndicator();
    void updateStatusBar(int line, int column);

    // Solution persistence
    void saveSolution(const QString &problemId, const QString &languageId, const QString &code);
    QString loadSolution(const QString &problemId, const QString &languageId);
    QString extractProblemId(const QString &fullPath) const;

    // ─── Constants ───
    static constexpr int GlobalMargin = 50;
    QString ProblemsJsonPath;
    QString ProblemsBasePath;
    QString SolutionsBasePath;

    // ─── Backend ───
    Backend *m_backend = nullptr;
    QString m_currentProblemId;
    QString m_currentProblemPath;
    bool m_runningAllTests = false;

    // ─── Progress ───
    ProgressManager *progressManager = nullptr;

    // ─── Layout ───
    QStackedLayout *stack = nullptr;

    // ─── Browser Page ───
    QWidget *browserPage = nullptr;
    ProblemBrowser *browser = nullptr;

    // ─── Editor Page ───
    QWidget *editorPage = nullptr;
    QSplitter *mainHorizontalSplitter = nullptr;
    QSplitter *rightVerticalSplitter = nullptr;

    ProblemPanel *problemPanel = nullptr;
    CodeEditor *codeEditor = nullptr;
    TestCasePanel *testCasePanel = nullptr;

    // ─── Toolbar ───
    QComboBox  *languageCombo      = nullptr;
    QLabel     *langIndicator      = nullptr;
    QLabel     *cursorPosLabel     = nullptr;
    QLabel     *problemTitleLabel  = nullptr;
    QPushButton *backButton        = nullptr;
    QPushButton *runButton         = nullptr;
    QPushButton *stopButton        = nullptr;
    QPushButton *submitButton      = nullptr;

    // ─── Sidebar ───
    HoverSidebar *sidebar = nullptr;


    // 1. Move variables here
    QStringList m_themes = {"one dark pro", "dracula", "monokai", "github dark", "tokyo night"};
    int m_themeIndex = 0;
};

#endif // MAINWINDOW_H
