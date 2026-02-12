#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "progressmanager.h"
#include <QMainWindow>
#include <QSplitter>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QCoreApplication>
#include <QDir>

class QStackedLayout;
class CodeEditor;
class ProblemBrowser;
class ProblemPanel;
class TestCasePanel;
class HoverSidebar;
class TreeSitterHighlighter;
class QTextDocument;
class Backend;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void applyStyle(const QString &path);
    QString extractProblemId(const QString &fullPath) const;
    QString m_currentTemplateLanguage;
    bool m_isShowingTemplate = false;


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

private:
    // Setup
    void setupUI();
    void setupBrowserPage();
    void setupEditorPage();
    void setupSidebar();
    void setupConnections();
    void setupShortcuts();
    void setupBackend();

    // Factory
    CodeEditor* createEditor();
    TreeSitterHighlighter* createHighlighter(QTextDocument *document);

    // Update UI state
    void setExecutionState(bool running);
    void updateLanguageIndicator();

    // Constants
    static constexpr int GlobalMargin = 55;
    QString ProblemsBasePath; // Just declare
    QString ProblemsJsonPath; // Just declare


    // ─── Backend ───
    Backend *m_backend = nullptr;
    QString m_currentProblemPath;  // Full path to the problem JSON file
    bool m_runningAllTests = false;

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
    QComboBox *languageCombo = nullptr;
    QLabel *langIndicator = nullptr;
    QPushButton *runButton = nullptr;
    QPushButton *stopButton = nullptr;
    QPushButton *submitButton = nullptr;

    // ─── Sidebar ───
    HoverSidebar *sidebar = nullptr;

    ProgressManager *progressManager;
    QString m_currentProblemId;
};

#endif // MAINWINDOW_H
