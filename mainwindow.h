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
#include <QWebEngineView>
#include <QWebChannel>

// #include "ProgressManager.h"
#include "progressmanager.h"


class QStackedLayout;
class WebWorkspace;
class ProblemBrowser;
class SidebarBridge;
class Backend;

class SidebarBridge : public QObject
{
    Q_OBJECT
public:
    explicit SidebarBridge(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void navigationChanged(int index) { emit navChanged(index); }
    void bottomItemClicked(int index) { emit bottomClicked(index); }
    void hovered(bool isHovered) { emit hoverChanged(isHovered); }

signals:
    void navChanged(int index);
    void bottomClicked(int index);
    void hoverChanged(bool isHovered);
};

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
    void onRunCurrentTest(int index);
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
    void onLanguageChanged(const QString &langId);
    void populateLanguages();

    // Editor (Removed unused cursor changed)

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
    QString m_currentLangId = "cpp";
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
    WebWorkspace *m_workspace = nullptr;

    // ─── Toolbar (Kept for Browser View compatibility if needed, or removed if handled inside WebWorkspace)
    // Wait, the UnifiedEditor handles the toolbar inside the Web UI. We don't need Qt toolbar buttons.
    // I'll keep the variables just in case they are referenced in setupBrowserPage, but they shouldn't be.
    // Actually, submitButton and runButton are referenced in onNavigateToBrowser. 
    // I will remove them and handle it cleanly.

    // ─── Sidebar ───
    QWebEngineView *sidebarView = nullptr;
    QWebChannel *sidebarChannel = nullptr;
    SidebarBridge *sidebarBridge = nullptr;


    // 1. Move variables here
    QStringList m_themes = {"one dark pro", "dracula", "monokai", "github dark", "tokyo night"};
    int m_themeIndex = 0;

};

#endif // MAINWINDOW_H
