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

void MainWindow::setupUI()
{
    auto *centralContainer = new QWidget(this);
    setCentralWidget(centralContainer);

    stack = new QStackedLayout(centralContainer);

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
    sidebarView->setGeometry(0, 0, 50, height()); // Width matching sidebar.html collapsed state
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
        sidebarView->setFixedWidth(isHovered ? 220 : 50);
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

void MainWindow::onNavigateToEditor(const QString &path)
{
    const QString filePath = QDir(ProblemsBasePath).filePath(path);
    m_currentProblemId = extractProblemId(path);
    m_currentProblemPath = filePath;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) {
            m_workspace->setProblemData(doc.object());
            m_workspace->setTestCases(doc.object()["testCases"].toArray());
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
    Q_UNUSED(expected);
    bool passed = (status == "Accepted");
    QString displayOutput = output;
    if (status == "Time Limit Exceeded") displayOutput = "[TLE] Execution timed out";
    else if (status == "Runtime Error") displayOutput = "[RE] " + output;
    else if (status == "Compile Error") displayOutput = "[CE] " + output;
    
    m_workspace->setTestResult(testIndex, displayOutput, passed);
}

void MainWindow::onCompilationError(const QString &error)
{
    QMessageBox::critical(this, "Compilation Error", "Failed to compile your code:\n\n" + error.left(500));
}

void MainWindow::onSystemError(const QString &error)
{
    QMessageBox::critical(this, "System Error", error);
    m_workspace->clearAllResults();
}

void MainWindow::onExecutionStarted()
{
    setExecutionState(true);
}

void MainWindow::onExecutionFinished()
{
    setExecutionState(false);
}

void MainWindow::setExecutionState(bool running)
{
    // The workspace could potentially disable UI here if needed via JS
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (sidebarView) sidebarView->setFixedHeight(height());
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
