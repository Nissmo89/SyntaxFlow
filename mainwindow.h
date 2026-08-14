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


#ifdef SYNTAXFLOW_HAS_QWINDOWKIT
#include <QWKWidgets/widgetwindowagent.h>
namespace QWK { class WidgetWindowAgent; }
#endif

class QStackedLayout;
class WebWorkspace;
class ProblemBrowser;
class SidebarBridge;
class Backend;
class HttpServer;

#ifndef SYNTAXFLOW_HAS_QWINDOWKIT
class WidgetWindowAgent {
public:
    enum class SystemButton { Minimize, Maximize, Close, Unknown };
    explicit WidgetWindowAgent(QWidget *) {}
    void setup(QWidget *) {}
    void setHitTestVisible(QWidget *, bool) {}
    void setTitleBar(QWidget *) {}
    void setSystemButton(SystemButton, QWidget *) {}
};
#else
using WidgetWindowAgent = QWK::WidgetWindowAgent;
#endif

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

class WelcomeBridge : public QObject
{
    Q_OBJECT
public:
    explicit WelcomeBridge(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void navigateToBrowser() { emit requestedBrowser(); }

signals:
    void requestedBrowser();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void applyStyle(const QString &path);

    // App theme (dark/light) toggled from the title bar
    void onThemeToggle();
    void applyAppTheme();
    QString titleBarStyle() const;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;

public slots:
    // Navigation
    void onNavigateToEditor(const QString &path);
    void onNavigateToBrowser();
    void onNavigateToProfile();

private slots:

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
    void onResetCode();

private:
    // Setup
    void setupTitleBar();
    void setupUI();
    void setupWelcomePage();
    void setupBrowserPage();
    void setupEditorPage();
    void setupProfilePage();
    void setupSidebar();
    void setupConnections();
    void setupShortcuts();
    void setupBackend();

    // ─── Frameless Window & Titlebar ───
    static constexpr int m_titleBarHeight = 38;
    QWidget *m_titleBar = nullptr;
    QPushButton *m_btnMin = nullptr;
    QPushButton *m_btnMax = nullptr;
    QPushButton *m_btnClose = nullptr;
    QPushButton *m_btnTheme = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_iconLabel = nullptr;
    WidgetWindowAgent *m_windowAgent = nullptr;
    bool m_darkTheme = true;

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
    bool m_allTestsPassed = false;
    int m_testsCompleted = 0;

    // ─── Progress ───
    ProgressManager *progressManager = nullptr;

    // ─── Layout ───
    QStackedLayout *stack = nullptr;

    // ─── Welcome Page ───
    QWidget *welcomePage = nullptr;
    QWebEngineView *welcomeView = nullptr;
    WelcomeBridge *welcomeBridge = nullptr;

    // ─── Browser Page ───
    QWidget *browserPage = nullptr;
    ProblemBrowser *browser = nullptr;

    // ─── Profile Page ───
    QWidget *profilePage = nullptr;
    QWebEngineView *profileView = nullptr;

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

    // ─── Embedded Server ───
    HttpServer *m_server = nullptr;
};

#endif // MAINWINDOW_H
