#include "mainwindow.h"
#include "web_workspace.h"
#include "problemwidgets.h"
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QNetworkInterface>
#include "backend.h"
#include "httpserver.h"

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QShortcut>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1400, 900);
    setWindowIcon(QIcon(":/icons/app_icon.svg"));

#ifdef SYNTAXFLOW_HAS_QWINDOWKIT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
#endif

    const QString appDir = QCoreApplication::applicationDirPath();
    ProblemsBasePath = QDir(appDir).filePath("problems/");
    ProblemsJsonPath = QDir(appDir).filePath("problems.json");

    progressManager = new ProgressManager(this);

    SolutionsBasePath = QStandardPaths::writableLocation(
                            QStandardPaths::AppDataLocation) + "/solutions/";
    QDir().mkpath(SolutionsBasePath);

    m_server = new HttpServer(this);

    setupBackend();
    setupUI();
    setupConnections();
    setupShortcuts();
    populateLanguages();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupBackend()
{
    m_backend = new Backend(this);

    connect(m_backend, &Backend::testResult,
            this, &MainWindow::onTestResult);
    connect(m_backend, &Backend::compilationError,
            this, &MainWindow::onCompilationError);
    connect(m_backend, &Backend::systemError,
            this, &MainWindow::onSystemError);
    connect(m_backend, &Backend::executionStarted,
            this, &MainWindow::onExecutionStarted);
    connect(m_backend, &Backend::executionFinished,
            this, &MainWindow::onExecutionFinished);
    connect(m_backend, &Backend::languagesChanged,
            this, &MainWindow::populateLanguages);
}

void MainWindow::setupTitleBar()
{
    m_titleBar = new QWidget(this);
    m_titleBar->setObjectName("titleBar");
    m_titleBar->setFixedHeight(m_titleBarHeight);

    auto *layout = new QHBoxLayout(m_titleBar);
    layout->setContentsMargins(12, 0, 8, 0);
    layout->setSpacing(8);

    // App Icon
    m_iconLabel = new QLabel(m_titleBar);
    m_iconLabel->setObjectName("appIcon");
    m_iconLabel->setFixedSize(20, 20);
    m_iconLabel->setPixmap(QIcon(":/icons/app_icon.svg").pixmap(20, 20));
    m_iconLabel->setScaledContents(true);

    // App Title
    m_titleLabel = new QLabel("SyntaxFlow", m_titleBar);
    m_titleLabel->setObjectName("appTitle");

    // Left side: Icon + Title
    layout->addWidget(m_iconLabel, 0, Qt::AlignVCenter);
    layout->addWidget(m_titleLabel, 0, Qt::AlignVCenter);
    layout->addStretch(1);

    // App theme toggle (dark/light)
    m_btnTheme = new QPushButton(m_titleBar);
    m_btnTheme->setObjectName("btnTheme");
    m_btnTheme->setIcon(QIcon(":/icons/theme-sun.svg"));
    m_btnTheme->setIconSize(QSize(14, 14));
    m_btnTheme->setFixedSize(36, 26);
    m_btnTheme->setToolTip("Switch to light theme");
    connect(m_btnTheme, &QPushButton::clicked, this, &MainWindow::onThemeToggle);

    // Window control buttons
    m_btnMin = new QPushButton(m_titleBar);
    m_btnMin->setObjectName("btnMin");
    m_btnMin->setIcon(QIcon(":/icons/min.svg"));
    m_btnMin->setIconSize(QSize(14, 14));
    m_btnMin->setFixedSize(36, 26);
    m_btnMin->setToolTip("Minimize");

    m_btnMax = new QPushButton(m_titleBar);
    m_btnMax->setObjectName("btnMax");
    m_btnMax->setIcon(QIcon(":/icons/max.svg"));
    m_btnMax->setIconSize(QSize(14, 14));
    m_btnMax->setFixedSize(36, 26);
    m_btnMax->setToolTip("Maximize");

    m_btnClose = new QPushButton(m_titleBar);
    m_btnClose->setObjectName("btnClose");
    m_btnClose->setIcon(QIcon(":/icons/close.svg"));
    m_btnClose->setIconSize(QSize(14, 14));
    m_btnClose->setFixedSize(36, 26);
    m_btnClose->setToolTip("Close");

    layout->addWidget(m_btnTheme, 0, Qt::AlignVCenter);
    layout->addWidget(m_btnMin, 0, Qt::AlignVCenter);
    layout->addWidget(m_btnMax, 0, Qt::AlignVCenter);
    layout->addWidget(m_btnClose, 0, Qt::AlignVCenter);

    connect(m_btnMin, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(m_btnMax, &QPushButton::clicked, this, [this]() {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
        m_btnMax->setIcon(isMaximized() ? QIcon(":/icons/restore.svg") : QIcon(":/icons/max.svg"));
    });
    connect(m_btnClose, &QPushButton::clicked, this, &MainWindow::close);

    // Setup QWindowKit Agent
    m_windowAgent = new WidgetWindowAgent(this);
    m_windowAgent->setup(this);
    m_windowAgent->setTitleBar(m_titleBar);
    m_windowAgent->setSystemButton(WidgetWindowAgent::SystemButton::Minimize, m_btnMin);
    m_windowAgent->setSystemButton(WidgetWindowAgent::SystemButton::Maximize, m_btnMax);
    m_windowAgent->setSystemButton(WidgetWindowAgent::SystemButton::Close, m_btnClose);
    m_windowAgent->setHitTestVisible(m_btnTheme, true);
    m_windowAgent->setHitTestVisible(m_btnMin, true);
    m_windowAgent->setHitTestVisible(m_btnMax, true);
    m_windowAgent->setHitTestVisible(m_btnClose, true);

    // Apply TitleBar stylesheet
    m_titleBar->setStyleSheet(titleBarStyle());
}

QString MainWindow::titleBarStyle() const
{
    if (m_darkTheme) {
        return
            "QWidget#titleBar {"
            "  background-color: #0f1015;"
            "  border-bottom: 1px solid #1e2029;"
            "}"
            "QLabel#appTitle {"
            "  color: #e2e8f0;"
            "  font-size: 12px;"
            "  font-weight: 600;"
            "  letter-spacing: 0.5px;"
            "  padding-left: 2px;"
            "}"
            "QPushButton#btnMin, QPushButton#btnMax, QPushButton#btnClose, QPushButton#btnTheme {"
            "  background-color: transparent;"
            "  border: 1px solid transparent;"
            "  border-radius: 5px;"
            "  padding: 0px;"
            "}"
            "QPushButton#btnMin:hover, QPushButton#btnMax:hover, QPushButton#btnTheme:hover {"
            "  background-color: rgba(255, 255, 255, 0.08);"
            "  border: 1px solid rgba(255, 255, 255, 0.12);"
            "}"
            "QPushButton#btnMin:pressed, QPushButton#btnMax:pressed, QPushButton#btnTheme:pressed {"
            "  background-color: rgba(255, 255, 255, 0.15);"
            "}"
            "QPushButton#btnClose:hover {"
            "  background-color: #e81123;"
            "  border: 1px solid #e81123;"
            "}"
            "QPushButton#btnClose:pressed {"
            "  background-color: #bf101d;"
            "}";
    }

    return
        "QWidget#titleBar {"
        "  background-color: #f6f7f9;"
        "  border-bottom: 1px solid #e2e5e9;"
        "}"
        "QLabel#appTitle {"
        "  color: #1c1f24;"
        "  font-size: 12px;"
        "  font-weight: 600;"
        "  letter-spacing: 0.5px;"
        "  padding-left: 2px;"
        "}"
        "QPushButton#btnMin, QPushButton#btnMax, QPushButton#btnClose, QPushButton#btnTheme {"
        "  background-color: transparent;"
        "  border: 1px solid transparent;"
        "  border-radius: 5px;"
        "  padding: 0px;"
        "}"
        "QPushButton#btnMin:hover, QPushButton#btnMax:hover, QPushButton#btnTheme:hover {"
        "  background-color: rgba(0, 0, 0, 0.06);"
        "  border: 1px solid rgba(0, 0, 0, 0.10);"
        "}"
        "QPushButton#btnMin:pressed, QPushButton#btnMax:pressed, QPushButton#btnTheme:pressed {"
        "  background-color: rgba(0, 0, 0, 0.12);"
        "}"
        "QPushButton#btnClose:hover {"
        "  background-color: #e81123;"
        "  border: 1px solid #e81123;"
        "}"
        "QPushButton#btnClose:pressed {"
        "  background-color: #bf101d;"
        "}";
}

void MainWindow::onThemeToggle()
{
    m_darkTheme = !m_darkTheme;
    applyAppTheme();
}

void MainWindow::applyAppTheme()
{
    const QString theme = m_darkTheme ? "dark" : "light";

    if (m_btnTheme) {
        m_btnTheme->setIcon(QIcon(m_darkTheme ? ":/icons/theme-sun.svg" : ":/icons/theme-moon.svg"));
        m_btnTheme->setToolTip(m_darkTheme ? "Switch to light theme" : "Switch to dark theme");
    }
    if (m_titleBar) {
        m_titleBar->setStyleSheet(titleBarStyle());
    }

    // Pages share the same origin (same embedded server), so they keep a
    // single localStorage key; push the change to every loaded page.
    const QString js = QStringLiteral("try { AppTheme.set('%1'); } catch (e) {}").arg(theme);
    if (browser) browser->applyTheme(js);
    if (m_workspace) m_workspace->applyTheme(js);
    if (profileView) profileView->page()->runJavaScript(js);
    if (sidebarView) sidebarView->page()->runJavaScript(js);
}

void MainWindow::setupUI()
{
    auto *centralContainer = new QWidget(this);
    setCentralWidget(centralContainer);

    auto *rootLayout = new QVBoxLayout(centralContainer);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    setupTitleBar();
    rootLayout->addWidget(m_titleBar);

    auto *contentWidget = new QWidget(centralContainer);
    stack = new QStackedLayout(contentWidget);
    rootLayout->addWidget(contentWidget, 1);

    QFont appFont("Segoe UI", 10);
    appFont.setStyleHint(QFont::SansSerif);
    qApp->setFont(appFont);

    setupBrowserPage();
    setupEditorPage();
    setupProfilePage();
    setupSidebar();

    stack->setCurrentWidget(browserPage);
}

void MainWindow::setupBrowserPage()
{
    browserPage = new QWidget(this);
    auto *layout = new QVBoxLayout(browserPage);
    layout->setContentsMargins(GlobalMargin, 0, 0, 0);

    browser = new ProblemBrowser(QUrl(m_server->getUrlFor("/browser.html")), progressManager, this);
    browser->loadFromJson(ProblemsJsonPath);

    layout->addWidget(browser);
    stack->addWidget(browserPage);
}

void MainWindow::setupEditorPage()
{
    editorPage = new QWidget(this);
    auto *layout = new QVBoxLayout(editorPage);
    layout->setContentsMargins(GlobalMargin, 0, 0, 0);
    layout->setSpacing(0);

    m_workspace = new WebWorkspace(QUrl(m_server->getUrlFor("/workspace.html")), this);
    layout->addWidget(m_workspace);

    stack->addWidget(editorPage);
}

void MainWindow::setupProfilePage()
{
    profilePage = new QWidget(this);
    auto *layout = new QVBoxLayout(profilePage);
    layout->setContentsMargins(GlobalMargin, 0, 0, 0);

    profileView = new QWebEngineView(this);
    profileView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    profileView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    profileView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    QString macAddress = "default-mac";
    for(const QNetworkInterface& interface : QNetworkInterface::allInterfaces()) {
        if(!(interface.flags() & QNetworkInterface::IsLoopBack) && interface.hardwareAddress().length() > 0) {
            macAddress = interface.hardwareAddress();
            break;
        }
    }

    QUrl url(m_server->getUrlFor("/profile.html", "mac=" + QUrl::toPercentEncoding(macAddress)));
    profileView->setUrl(url);

    layout->addWidget(profileView);
    stack->addWidget(profilePage);
}

void MainWindow::setupSidebar()
{
    sidebarView = new QWebEngineView(this);
    sidebarView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    sidebarView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    sidebarView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    sidebarView->setGeometry(0, m_titleBarHeight, 50, height() - m_titleBarHeight); // Width matching sidebar.html collapsed state
    sidebarView->setContextMenuPolicy(Qt::NoContextMenu);

    sidebarBridge = new SidebarBridge(this);
    sidebarChannel = new QWebChannel(this);
    sidebarChannel->registerObject(QStringLiteral("sidebarBridge"), sidebarBridge);
    sidebarView->page()->setWebChannel(sidebarChannel);

    connect(sidebarBridge, &SidebarBridge::navChanged, this, [this](int index) {
        switch (index) {
        case 0: /* editor */ break;
        case 1: onNavigateToBrowser(); break;
        }
    });

    connect(sidebarBridge, &SidebarBridge::bottomClicked, this, [this](int index) {
        switch (index) {
        case 0: /* settings */ break;
        case 1: onNavigateToProfile(); break;
        }
    });

    connect(sidebarBridge, &SidebarBridge::hoverChanged, this, [this](bool isHovered) {
        sidebarView->setGeometry(0, m_titleBarHeight, isHovered ? 220 : 50, height() - m_titleBarHeight);
        if (isHovered) sidebarView->raise();
    });

    // Get MAC address for Avatar
    QString macAddress = "default-mac";
    for(const QNetworkInterface& interface : QNetworkInterface::allInterfaces()) {
        if(!(interface.flags() & QNetworkInterface::IsLoopBack) && interface.hardwareAddress().length() > 0) {
            macAddress = interface.hardwareAddress();
            break;
        }
    }

    QUrl url(m_server->getUrlFor("/sidebar.html", "mac=" + QUrl::toPercentEncoding(macAddress)));
    sidebarView->setUrl(url);

    // Sync the title bar theme with the theme persisted by the web pages.
    connect(sidebarView, &QWebEngineView::loadFinished, this, [this](bool ok) {
        if (!ok) return;
        sidebarView->page()->runJavaScript(
            "try { localStorage.getItem('sf-theme') || 'dark' } catch (e) { 'dark' }",
            [this](const QVariant &value) {
                m_darkTheme = (value.toString() != "light");
                applyAppTheme();
            });
    });
}

void MainWindow::setupConnections()
{
    connect(browser, &ProblemBrowser::navigateToEditor,
            this, &MainWindow::onNavigateToEditor);

    connect(m_workspace, &WebWorkspace::backRequested,
            this, &MainWindow::onNavigateToBrowser);
    connect(m_workspace, &WebWorkspace::runRequested,
            this, &MainWindow::onRunCurrentTest);
    connect(m_workspace, &WebWorkspace::submitRequested,
            this, &MainWindow::onRunAllTests);
    connect(m_workspace, &WebWorkspace::languageChanged,
            this, &MainWindow::onLanguageChanged);
    connect(m_workspace, &WebWorkspace::resetRequested,
            this, &MainWindow::onResetCode);

    connect(m_workspace, &WebWorkspace::textChanged, this, [this](const QString &text) {
        if (m_currentProblemId.isEmpty()) return;
        saveSolution(m_currentProblemId, m_currentLangId, text);
    });
}

void MainWindow::setupShortcuts()
{
    auto *backToBrowser = new QShortcut(QKeySequence("Ctrl+B"), this);
    connect(backToBrowser, &QShortcut::activated,
            this, &MainWindow::onNavigateToBrowser);

    auto *runCurrentTest = new QShortcut(QKeySequence("Ctrl+Return"), this);
    connect(runCurrentTest, &QShortcut::activated, this, [this]() {
        onRunCurrentTest(0); // Default to case 0 via shortcut if not known
    });

    auto *runAllTests = new QShortcut(QKeySequence("Ctrl+Shift+Return"), this);
    connect(runAllTests, &QShortcut::activated,
            this, &MainWindow::onRunAllTests);

    auto *stopExec = new QShortcut(QKeySequence("Escape"), this);
    connect(stopExec, &QShortcut::activated,
            this, &MainWindow::onStopExecution);

    auto *openConfig = new QShortcut(QKeySequence("Ctrl+,"), this);
    connect(openConfig, &QShortcut::activated, m_backend, &Backend::openConfigDirectory);
}

void MainWindow::populateLanguages()
{
    QStringList all = m_backend->allLanguages();
    QJsonArray arr;
    for (const QString &id : all) {
        if (m_backend->isLanguageAvailable(id)) {
            arr.append(id);
        }
    }
    if (!arr.contains(m_currentLangId) && arr.size() > 0) {
        m_currentLangId = arr[0].toString();
    }
    if (m_workspace) {
        m_workspace->setLanguages(arr, m_currentLangId);
    }
}

void MainWindow::onLanguageChanged(const QString &langId)
{
    m_currentLangId = langId;
    if (m_currentProblemId.isEmpty()) return;
    
    QString saved = loadSolution(m_currentProblemId, langId);
    if (!saved.isEmpty()) {
        m_workspace->setCode(saved);
    } else {
        QString templ = m_backend->getProblemTemplate(langId, m_currentProblemId);
        m_workspace->setCode(templ);
    }
}

void MainWindow::onResetCode()
{
    if (m_currentProblemId.isEmpty()) return;
    if (!m_backend->isLanguageAvailable(m_currentLangId)) return;
    QString templ = m_backend->getProblemTemplate(m_currentLangId, m_currentProblemId);
    m_workspace->setCode(templ);
}

void MainWindow::onNavigateToEditor(const QString &path)
{
    const QString filePath = QDir(ProblemsBasePath).filePath(path);
    m_currentProblemId = extractProblemId(path);
    m_currentProblemPath = filePath;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            obj["problem_id"] = m_currentProblemId;
            obj["problem_rel_path"] = path;
            m_workspace->setProblemData(obj);
            m_workspace->setTestCases(obj["testCases"].toArray());
        }
    }

    m_workspace->clearAllResults();
    stack->setCurrentWidget(editorPage);
    onLanguageChanged(m_currentLangId);
}

void MainWindow::onNavigateToBrowser()
{
    if (m_backend->isRunning()) {
        m_backend->stopExecution();
    }
    m_currentProblemPath.clear();
    m_currentProblemId.clear();
    stack->setCurrentWidget(browserPage);
}

void MainWindow::onNavigateToProfile()
{
    if (m_backend->isRunning()) {
        m_backend->stopExecution();
    }
    m_currentProblemPath.clear();
    m_currentProblemId.clear();
    stack->setCurrentWidget(profilePage);
}

void MainWindow::onRunCurrentTest(int index)
{
    if (!m_backend->isLanguageAvailable(m_currentLangId)) {
        LanguageConfig cfg = m_backend->getLanguageConfig(m_currentLangId);
        QMessageBox::warning(this, "Language Not Available",
                             cfg.name + " is not installed.");
        return;
    }
    if (m_currentProblemPath.isEmpty()) return;

    m_runningAllTests = false;
    m_workspace->setTestRunning(index);
    m_backend->runTestCase(m_workspace->code(), m_currentLangId, index, m_currentProblemPath);
}

void MainWindow::onRunAllTests()
{
    if (!m_backend->isLanguageAvailable(m_currentLangId)) {
        LanguageConfig cfg = m_backend->getLanguageConfig(m_currentLangId);
        QMessageBox::warning(this, "Language Not Available",
                             cfg.name + " is not installed.");
        return;
    }
    if (m_currentProblemPath.isEmpty()) return;

    m_runningAllTests = true;
    m_allTestsPassed = true;
    m_testsCompleted = 0;
    m_backend->runCode(m_workspace->code(), m_currentLangId, m_currentProblemPath);
}

void MainWindow::onStopExecution()
{
    if (m_backend->isRunning()) {
        m_backend->stopExecution();
    }
}

void MainWindow::onTestResult(int testIndex, const QString &status,
                              const QString &output, const QString &expected,
                              qint64 timeMs)
{
    bool passed = (status == "Accepted");
    if (m_runningAllTests) {
        m_testsCompleted++;
        if (!passed) {
            m_allTestsPassed = false;
        }
    }

    QString displayOutput = output;
    if (status == "Time Limit Exceeded") displayOutput = "[TLE] Execution timed out";
    else if (status == "Runtime Error") displayOutput = "[RE] " + output;
    else if (status == "Compile Error") displayOutput = "[CE] " + output;
    
    m_workspace->setTestResult(testIndex, status, displayOutput, expected, timeMs, passed);
}

void MainWindow::onCompilationError(const QString &error)
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Compilation Error");
    msgBox.setText("Failed to compile your code:");
    msgBox.setDetailedText(error);
    msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgBox.exec();
}

void MainWindow::onSystemError(const QString &error)
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("System Error");
    msgBox.setText("A system error occurred.");
    msgBox.setDetailedText(error);
    msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgBox.exec();
    m_workspace->clearAllResults();
}

void MainWindow::onExecutionStarted()
{
    setExecutionState(true);
}

void MainWindow::onExecutionFinished()
{
    setExecutionState(false);
    if (m_runningAllTests) {
        if (m_allTestsPassed && m_testsCompleted > 0) {
            if (progressManager) {
                progressManager->markSolved(m_currentProblemId, true);
            }
        }
        m_runningAllTests = false;
    }
}

void MainWindow::setExecutionState(bool running)
{
    // The workspace could potentially disable UI here if needed via JS
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (sidebarView) {
        sidebarView->setGeometry(0, m_titleBarHeight, sidebarView->width(), height() - m_titleBarHeight);
    }
    if (m_btnMax) {
        m_btnMax->setIcon(isMaximized() ? QIcon(":/icons/restore.svg") : QIcon(":/icons/max.svg"));
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        if (m_btnMax) {
            m_btnMax->setIcon(isMaximized() ? QIcon(":/icons/restore.svg") : QIcon(":/icons/max.svg"));
        }
    }
}

void MainWindow::applyStyle(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;
    qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
}

QString MainWindow::extractProblemId(const QString &fullPath) const
{
    QFileInfo info(fullPath);
    return info.completeBaseName();
}

void MainWindow::saveSolution(const QString &problemId,
                              const QString &languageId,
                              const QString &code)
{
    if (problemId.isEmpty() || languageId.isEmpty()) return;
    QString path = SolutionsBasePath + problemId + ".json";
    QJsonObject root;
    QFile file(path);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }
    root[languageId] = code;
    QSaveFile saveFile(path);
    if (saveFile.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(root);
        saveFile.write(doc.toJson());
        saveFile.commit();
    }
}

QString MainWindow::loadSolution(const QString &problemId,
                                 const QString &languageId)
{
    if (problemId.isEmpty() || languageId.isEmpty()) return QString();
    QString path = SolutionsBasePath + problemId + ".json";
    QFile file(path);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return QString();
    QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    return root.value(languageId).toString();
}
