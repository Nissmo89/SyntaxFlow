#include "mainwindow.h"
#include "code_editor.h"
#include "problem_panel.h"
#include "testcase_panel.h"
#include "problemwidgets.h"
#include "hoversidebar.h"
#include "backend.h"

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QShortcut>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>

// ═══════════════════════════════════════════════════════════════════════════
// Constructor / Destructor
// ═══════════════════════════════════════════════════════════════════════════

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1400, 900);

    // Setup paths
    const QString appDir = QCoreApplication::applicationDirPath();
    ProblemsBasePath = QDir(appDir).filePath("problems/");
    ProblemsJsonPath = QDir(appDir).filePath("problems.json");

    qDebug() << "Problems base path:" << ProblemsBasePath;
    qDebug() << "Problems JSON path:" << ProblemsJsonPath;

    // Progress manager
    progressManager = new ProgressManager(this);

    // Solutions directory
    SolutionsBasePath = QStandardPaths::writableLocation(
                            QStandardPaths::AppDataLocation) + "/solutions/";
    QDir().mkpath(SolutionsBasePath);

    // Setup
    setupBackend();
    setupUI();
    setupConnections();
    setupShortcuts();
    populateLanguages();
}

MainWindow::~MainWindow() = default;

// ═══════════════════════════════════════════════════════════════════════════
// Backend Setup
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::setupBackend()
{
    m_backend = new Backend(this);

    // Connect backend signals
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

    qDebug() << "Backend initialized. Available languages:"
             << m_backend->availableLanguages();
}

// ═══════════════════════════════════════════════════════════════════════════
// UI Setup
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::setupUI()
{
    auto *centralContainer = new QWidget(this);
    setCentralWidget(centralContainer);

    stack = new QStackedLayout(centralContainer);

    setupBrowserPage();
    setupEditorPage();
    setupSidebar();

    stack->setCurrentWidget(browserPage);
}

void MainWindow::setupBrowserPage()
{
    browserPage = new QWidget(this);
    auto *layout = new QVBoxLayout(browserPage);
    layout->setContentsMargins(GlobalMargin, 0, 0, 0);

    browser = new ProblemBrowser(progressManager, this);
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

    const QString splitterStyle = R"(
        QSplitter::handle {
            background: #2a2a2a;
        }
        QSplitter::handle:hover {
            background: #404040;
        }
        QSplitter::handle:horizontal {
            width: 2px;
        }
        QSplitter::handle:vertical {
            height: 2px;
        }
    )";

    // ─── Main Horizontal Splitter ───
    mainHorizontalSplitter = new QSplitter(Qt::Horizontal, this);
    mainHorizontalSplitter->setHandleWidth(2);
    mainHorizontalSplitter->setStyleSheet(splitterStyle);

    // ─── Right Side Vertical Splitter ───
    rightVerticalSplitter = new QSplitter(Qt::Vertical, this);
    rightVerticalSplitter->setHandleWidth(2);
    rightVerticalSplitter->setStyleSheet(splitterStyle);

    // ─── Problem Panel ───
    problemPanel = new ProblemPanel(this);
    problemPanel->setMinimumWidth(300);
    problemPanel->setMaximumWidth(600);

    // ─── Editor Container with Toolbar ───
    auto *editorContainer = new QWidget;
    editorContainer->setObjectName("editorContainer");
    auto *editorContainerLayout = new QVBoxLayout(editorContainer);
    editorContainerLayout->setContentsMargins(0, 0, 0, 0);
    editorContainerLayout->setSpacing(0);

    // ═══════════════════════════════════════════════════════════════════
    // Editor Toolbar
    // ═══════════════════════════════════════════════════════════════════
    auto *editorToolbar = new QWidget;
    editorToolbar->setObjectName("editorToolbar");
    editorToolbar->setFixedHeight(42);
    editorToolbar->setStyleSheet(R"(
        #editorToolbar {
            background: #1a1a1a;
            border-bottom: 1px solid #2a2a2a;
        }
    )");

    auto *toolbarLayout = new QHBoxLayout(editorToolbar);
    toolbarLayout->setContentsMargins(16, 0, 16, 0);
    toolbarLayout->setSpacing(10);

    // Language Selector ComboBox
    languageCombo = new QComboBox;
    languageCombo->setObjectName("languageCombo");
    languageCombo->setMinimumWidth(120);
    languageCombo->setCursor(Qt::PointingHandCursor);
    languageCombo->setStyleSheet(R"(
        #languageCombo {
            background: #252525;
            color: #d4d4d4;
            border: 1px solid #3a3a3a;
            border-radius: 5px;
            padding: 5px 10px;
            font-size: 13px;
        }
        #languageCombo:hover {
            border-color: #4a4a4a;
            background: #2a2a2a;
        }
        #languageCombo::drop-down {
            border: none;
            width: 20px;
        }
        #languageCombo::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 5px solid #888;
            margin-right: 5px;
        }
        #languageCombo QAbstractItemView {
            background: #252525;
            color: #d4d4d4;
            border: 1px solid #3a3a3a;
            selection-background-color: #3a3a3a;
        }
    )");

    // Language availability indicator
    langIndicator = new QLabel;
    langIndicator->setStyleSheet("color: #5c5; font-size: 14px;");

    // Cursor position label
    cursorPosLabel = new QLabel("Ln 1, Col 1");
    cursorPosLabel->setStyleSheet(R"(
        color: #666;
        font-size: 12px;
        padding: 0 10px;
    )");

    toolbarLayout->addWidget(languageCombo);
    toolbarLayout->addWidget(langIndicator);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(cursorPosLabel);

    // Separator
    auto *separator = new QFrame;
    separator->setFrameShape(QFrame::VLine);
    separator->setStyleSheet("color: #333;");
    toolbarLayout->addWidget(separator);

    // Run button
    runButton = new QPushButton("▶ Run");
    runButton->setObjectName("runBtn");
    runButton->setCursor(Qt::PointingHandCursor);
    runButton->setStyleSheet(R"(
        #runBtn {
            background: transparent;
            color: #8b949e;
            border: 1px solid #3a3a3a;
            border-radius: 5px;
            padding: 7px 18px;
            font-size: 13px;
            font-weight: 500;
        }
        #runBtn:hover {
            background: #2a2a2a;
            color: #fff;
            border-color: #4a4a4a;
        }
        #runBtn:pressed {
            background: #333;
        }
        #runBtn:disabled {
            color: #555;
            border-color: #2a2a2a;
        }
    )");

    // Stop button
    stopButton = new QPushButton("■ Stop");
    stopButton->setObjectName("stopBtn");
    stopButton->setCursor(Qt::PointingHandCursor);
    stopButton->setVisible(false);
    stopButton->setStyleSheet(R"(
        #stopBtn {
            background: #c53030;
            color: #fff;
            border: none;
            border-radius: 5px;
            padding: 7px 18px;
            font-size: 13px;
            font-weight: 500;
        }
        #stopBtn:hover {
            background: #e53e3e;
        }
        #stopBtn:pressed {
            background: #9b2c2c;
        }
    )");

    // Submit button
    submitButton = new QPushButton("Submit");
    submitButton->setObjectName("submitBtn");
    submitButton->setCursor(Qt::PointingHandCursor);
    submitButton->setStyleSheet(R"(
        #submitBtn {
            background: #238636;
            color: #fff;
            border: none;
            border-radius: 5px;
            padding: 7px 18px;
            font-size: 13px;
            font-weight: 600;
        }
        #submitBtn:hover {
            background: #2ea043;
        }
        #submitBtn:pressed {
            background: #1a7f37;
        }
        #submitBtn:disabled {
            background: #1a4d23;
            color: #888;
        }
    )");

    toolbarLayout->addWidget(runButton);
    toolbarLayout->addWidget(stopButton);
    toolbarLayout->addWidget(submitButton);

    // ═══════════════════════════════════════════════════════════════════
    // Code Editor - Using new CodeEditor class
    // ═══════════════════════════════════════════════════════════════════
    codeEditor = new CodeEditor(this);
    codeEditor->setMinimumHeight(200);

    // Apply theme
    codeEditor->setTheme("monokai");  // Options: "one dark pro", "dracula", "monokai", "github dark", "tokyo night"
    // also putting a shortcut to change theme for testing
    // Inside setupShortcuts() or constructor
    QShortcut *theme_change = new QShortcut(QKeySequence("Ctrl+K"), this);
    // CRITICAL: Set this to ApplicationShortcut
    theme_change->setContext(Qt::ApplicationShortcut);
    connect(theme_change, &QShortcut::activated, this, [this]() {
        // Increment index to cycle through
        m_themeIndex++;
        // Wrap around using modulo
        int index = m_themeIndex % m_themes.count();
        QString nextTheme = m_themes[index];
        qDebug() << "Switching to theme:" << nextTheme;
        // Apply
        codeEditor->setTheme(nextTheme); // Assuming you implemented setTheme in CustomQsciEditor
    });

    // Editor settings
    codeEditor->setFontSize(13);
    codeEditor->setTabSize(4);
    codeEditor->setShowWhitespace(false);
    codeEditor->setShowIndentGuides(true);
    codeEditor->setAutoCompleteEnabled(true);
    codeEditor->setBraceMatchingEnabled(true);

    // Set default language
    codeEditor->setLanguage("cpp");

    editorContainerLayout->addWidget(editorToolbar);
    editorContainerLayout->addWidget(codeEditor, 1);

    // Test Case Panel
    testCasePanel = new TestCasePanel(this);
    testCasePanel->setMinimumHeight(100);

    // ─── Assemble Right Splitter ───
    rightVerticalSplitter->addWidget(editorContainer);
    rightVerticalSplitter->addWidget(testCasePanel);
    rightVerticalSplitter->setSizes({450, 200});
    rightVerticalSplitter->setStretchFactor(0, 1);
    rightVerticalSplitter->setStretchFactor(1, 0);

    // ─── Assemble Main Splitter ───
    mainHorizontalSplitter->addWidget(problemPanel);
    mainHorizontalSplitter->addWidget(rightVerticalSplitter);
    mainHorizontalSplitter->setSizes({400, 900});
    mainHorizontalSplitter->setStretchFactor(0, 0);
    mainHorizontalSplitter->setStretchFactor(1, 1);

    layout->addWidget(mainHorizontalSplitter);
    stack->addWidget(editorPage);
}

void MainWindow::setupSidebar()
{
    sidebar = new HoverSidebar(this);
    sidebar->setGeometry(0, 0, 50, height());
}

void MainWindow::setupConnections()
{
    // Navigation: Browser → Editor
    connect(browser, &ProblemBrowser::navigateToEditor,
            this, &MainWindow::onNavigateToEditor);

    // Problem Panel → Test Case Panel
    connect(problemPanel, &ProblemPanel::testCasesAvailable,
            testCasePanel, &TestCasePanel::loadTestCases);

    // Toolbar buttons
    connect(runButton, &QPushButton::clicked,
            this, &MainWindow::onRunCurrentTest);
    connect(stopButton, &QPushButton::clicked,
            this, &MainWindow::onStopExecution);
    connect(submitButton, &QPushButton::clicked,
            this, &MainWindow::onRunAllTests);

    // Language selection
    connect(languageCombo, QOverload<int>::of(&QComboBox::activated),
            this, &MainWindow::onLanguageChanged);

    // Editor cursor position
    connect(codeEditor, &CodeEditor::cursorPositionChanged,
            this, &MainWindow::onEditorCursorChanged);

    // Auto-save solution on text change (debounced internally by editor)
    connect(codeEditor, &CodeEditor::textChanged, this, [this]() {
        if (m_currentProblemId.isEmpty())
            return;

        QString langId = languageCombo->currentData().toString();
        saveSolution(m_currentProblemId, langId, codeEditor->text());
    });
}

void MainWindow::setupShortcuts()
{
    // Return to browser: Ctrl+B
    auto *backToBrowser = new QShortcut(QKeySequence("Ctrl+B"), this);
    connect(backToBrowser, &QShortcut::activated,
            this, &MainWindow::onNavigateToBrowser);

    // Run current test: Ctrl+Enter
    auto *runCurrentTest = new QShortcut(QKeySequence("Ctrl+Return"), this);
    connect(runCurrentTest, &QShortcut::activated,
            this, &MainWindow::onRunCurrentTest);

    // Run all tests (Submit): Ctrl+Shift+Enter
    auto *runAllTests = new QShortcut(QKeySequence("Ctrl+Shift+Return"), this);
    connect(runAllTests, &QShortcut::activated,
            this, &MainWindow::onRunAllTests);

    // Stop execution: Escape
    auto *stopExec = new QShortcut(QKeySequence("Escape"), this);
    connect(stopExec, &QShortcut::activated,
            this, &MainWindow::onStopExecution);

    // Focus editor: Ctrl+E
    auto *focusEditor = new QShortcut(QKeySequence("Ctrl+E"), this);
    connect(focusEditor, &QShortcut::activated, this, [this]() {
        if (codeEditor) {
            codeEditor->setFocus();
        }
    });

    // Go to line: Ctrl+G
    auto *gotoLine = new QShortcut(QKeySequence("Ctrl+G"), this);
    connect(gotoLine, &QShortcut::activated, this, [this]() {
        // Could show a dialog here
        // For now, just focus the editor
        codeEditor->setFocus();
    });

    // Open language config: Ctrl+,
    auto *openConfig = new QShortcut(QKeySequence("Ctrl+,"), this);
    connect(openConfig, &QShortcut::activated, m_backend, &Backend::openConfigDirectory);

    // Toggle comment: Already handled in CodeEditor (Ctrl+/)
    // Duplicate line: Already handled in CodeEditor (Ctrl+Shift+D)
    // Delete line: Already handled in CodeEditor (Ctrl+Shift+K)
    // Move line up/down: Already handled in CodeEditor (Alt+Up/Down)
}

// ═══════════════════════════════════════════════════════════════════════════
// Language Management
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::populateLanguages()
{
    languageCombo->blockSignals(true);
    languageCombo->clear();

    QStringList all = m_backend->allLanguages();

    for (const QString &id : all) {
        LanguageConfig cfg = m_backend->getLanguageConfig(id);
        bool available = m_backend->isLanguageAvailable(id);

        QString display = cfg.name;
        if (!available) {
            display += " ✗";
        }

        languageCombo->addItem(display, id);

        // Gray out unavailable languages
        if (!available) {
            int idx = languageCombo->count() - 1;
            languageCombo->setItemData(idx, QBrush(Qt::gray), Qt::ForegroundRole);
        }
    }

    // Default to C++ if available
    int cppIndex = languageCombo->findData("cpp");
    if (cppIndex >= 0) {
        languageCombo->setCurrentIndex(cppIndex);
    }

    languageCombo->blockSignals(false);
    updateLanguageIndicator();
}

void MainWindow::onLanguageChanged(int)
{
    QString newLangId = languageCombo->currentData().toString();

    updateLanguageIndicator();

    // Update editor syntax highlighting
    codeEditor->setLanguage(newLangId);

    if (m_currentProblemId.isEmpty())
        return;

    // Load saved solution for this language
    QString saved = loadSolution(m_currentProblemId, newLangId);

    codeEditor->blockSignals(true);

    if (!saved.isEmpty()) {
        codeEditor->setText(saved);
    } else {
        // Load template for new language
        QString templ = m_backend->getTemplate(newLangId);
        if (!templ.isEmpty()) {
            codeEditor->setText(templ);
        }
    }

    codeEditor->blockSignals(false);
}

void MainWindow::updateLanguageIndicator()
{
    QString langId = languageCombo->currentData().toString();
    bool available = m_backend->isLanguageAvailable(langId);

    if (available) {
        langIndicator->setText("✓");
        langIndicator->setStyleSheet("color: #5c5; font-size: 14px;");
        langIndicator->setToolTip("Language available");
    } else {
        LanguageConfig cfg = m_backend->getLanguageConfig(langId);
        langIndicator->setText("✗");
        langIndicator->setStyleSheet("color: #c55; font-size: 14px;");
        langIndicator->setToolTip("Not installed. Requires: " +
                                  (cfg.compiled ? cfg.compileCommand : cfg.runCommand));
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// Editor Events
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::onEditorCursorChanged(int line, int column)
{
    cursorPosLabel->setText(QString("Ln %1, Col %2").arg(line).arg(column));
}

void MainWindow::updateStatusBar(int line, int column)
{
    cursorPosLabel->setText(QString("Ln %1, Col %2").arg(line).arg(column));
}

// ═══════════════════════════════════════════════════════════════════════════
// Navigation Slots
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::onNavigateToEditor(const QString &path)
{
    const QString filePath = ProblemsBasePath + path;
    m_currentProblemId = extractProblemId(path);
    m_currentProblemPath = filePath;

    qDebug() << ">>> Opening editor for:" << filePath;
    qDebug() << ">>> Problem ID:" << m_currentProblemId;

    if (!problemPanel->loadFromFile(filePath)) {
        qWarning() << "Failed to load problem:" << filePath;
        return;
    }

    // Reset test results
    testCasePanel->clearAllResults();

    // Switch to editor view
    stack->setCurrentWidget(editorPage);

    // Get current language
    QString langId = languageCombo->currentData().toString();

    // Update editor language
    codeEditor->setLanguage(langId);

    // Load saved solution or template
    QString saved = loadSolution(m_currentProblemId, langId);

    codeEditor->blockSignals(true);

    if (!saved.isEmpty()) {
        codeEditor->setText(saved);
    } else {
        QString templ = m_backend->getTemplate(langId);
        if (!templ.isEmpty()) {
            codeEditor->setText(templ);
        }
    }

    codeEditor->blockSignals(false);

    // Focus the code editor
    codeEditor->setFocus();

    // Reset cursor position display
    cursorPosLabel->setText("Ln 1, Col 1");
}

void MainWindow::onNavigateToBrowser()
{
    // Stop any running execution
    if (m_backend->isRunning()) {
        m_backend->stopExecution();
    }

    m_currentProblemPath.clear();
    m_currentProblemId.clear();
    stack->setCurrentWidget(browserPage);
}

// ═══════════════════════════════════════════════════════════════════════════
// Code Execution
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::onRunCurrentTest()
{
    QString langId = languageCombo->currentData().toString();

    // Check language availability
    if (!m_backend->isLanguageAvailable(langId)) {
        LanguageConfig cfg = m_backend->getLanguageConfig(langId);
        QMessageBox::warning(this, "Language Not Available",
                             cfg.name + " is not installed.\n\n"
                                        "Please install: " + (cfg.compiled ? cfg.compileCommand : cfg.runCommand) + "\n\n"
                                                                                          "Or add a custom configuration via Ctrl+,");
        return;
    }

    if (m_currentProblemPath.isEmpty()) {
        QMessageBox::warning(this, "No Problem Selected",
                             "Please select a problem first.");
        return;
    }

    int currentIndex = testCasePanel->getCurrentIndex();
    m_runningAllTests = false;

    qDebug() << ">>> Running test case:" << currentIndex << "with" << langId;

    testCasePanel->setTestRunning(currentIndex);

    m_backend->runTestCase(
        codeEditor->text(),
        langId,
        currentIndex,
        m_currentProblemPath
        );
}

void MainWindow::onRunAllTests()
{
    QString langId = languageCombo->currentData().toString();

    // Check language availability
    if (!m_backend->isLanguageAvailable(langId)) {
        LanguageConfig cfg = m_backend->getLanguageConfig(langId);
        QMessageBox::warning(this, "Language Not Available",
                             cfg.name + " is not installed.\n\n"
                                        "Please install: " + (cfg.compiled ? cfg.compileCommand : cfg.runCommand));
        return;
    }

    if (m_currentProblemPath.isEmpty()) {
        QMessageBox::warning(this, "No Problem Selected",
                             "Please select a problem first.");
        return;
    }

    m_runningAllTests = true;
    int totalTests = testCasePanel->getTestCaseCount();

    qDebug() << ">>> Running all" << totalTests << "test cases with" << langId;

    // Mark all tests as running
    for (int i = 0; i < totalTests; ++i) {
        testCasePanel->setTestRunning(i);
    }

    m_backend->runCode(
        codeEditor->text(),
        langId,
        m_currentProblemPath
        );
}

void MainWindow::onStopExecution()
{
    if (m_backend->isRunning()) {
        qDebug() << ">>> Stopping execution";
        m_backend->stopExecution();
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// Backend Result Handlers
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::onTestResult(int testIndex, const QString &status,
                              const QString &output, const QString &expected,
                              qint64 timeMs)
{
    Q_UNUSED(expected);
    qDebug() << "Test" << testIndex << ":" << status << "(" << timeMs << "ms)";

    bool passed = (status == "Accepted");

    // Format output with status info
    QString displayOutput = output;
    if (status == "Time Limit Exceeded") {
        displayOutput = "[TLE] Execution timed out";
    } else if (status == "Runtime Error") {
        displayOutput = "[RE] " + output;
    } else if (status == "Compile Error") {
        displayOutput = "[CE] " + output;
    }

    testCasePanel->setTestResult(testIndex, displayOutput, passed);
}

void MainWindow::onCompilationError(const QString &error)
{
    qDebug() << "Compilation error:" << error;

    // Show error in all running tests
    int count = testCasePanel->getTestCaseCount();
    for (int i = 0; i < count; ++i) {
        testCasePanel->setTestResult(i, "[Compile Error]\n" + error, false);
    }

    // Show message box
    QMessageBox::critical(this, "Compilation Error",
                          "Failed to compile your code:\n\n" + error.left(500));
}

void MainWindow::onSystemError(const QString &error)
{
    qDebug() << "System error:" << error;

    QMessageBox::critical(this, "System Error", error);

    // Reset test states
    testCasePanel->clearAllResults();
}

void MainWindow::onExecutionStarted()
{
    qDebug() << "Execution started";
    setExecutionState(true);
}

void MainWindow::onExecutionFinished()
{
    qDebug() << "Execution finished";
    setExecutionState(false);

    if (m_runningAllTests) {
        int passed = 0;
        int total = testCasePanel->getTestCaseCount();

        for (int i = 0; i < total; ++i) {
            if (testCasePanel->isTestPassed(i)) {
                passed++;
            }
        }

        qDebug() << "Results:" << passed << "/" << total << "passed";

        // Mark solved if ALL passed
        if (total > 0 && passed == total) {
            qDebug() << "All test cases passed. Marking as solved.";
            progressManager->markSolved(m_currentProblemId, true);
        }
    }

    m_runningAllTests = false;
}

// ═══════════════════════════════════════════════════════════════════════════
// UI State Management
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::setExecutionState(bool running)
{
    runButton->setVisible(!running);
    runButton->setEnabled(!running);
    stopButton->setVisible(running);
    submitButton->setEnabled(!running);
    languageCombo->setEnabled(!running);

    // Editor state
    if (running) {
        codeEditor->setReadOnly(true);
        codeEditor->setCursor(Qt::WaitCursor);
    } else {
        codeEditor->setReadOnly(false);
        codeEditor->setCursor(Qt::IBeamCursor);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// Event Handlers
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (sidebar) {
        sidebar->setFixedHeight(height());
    }
}

void MainWindow::applyStyle(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Failed to load stylesheet:" << path;
        return;
    }

    qApp->setStyleSheet(QString::fromUtf8(file.readAll()));
}

// ═══════════════════════════════════════════════════════════════════════════
// Solution Persistence
// ═══════════════════════════════════════════════════════════════════════════

QString MainWindow::extractProblemId(const QString &fullPath) const
{
    QFileInfo info(fullPath);
    return info.completeBaseName();
}

void MainWindow::saveSolution(const QString &problemId,
                              const QString &languageId,
                              const QString &code)
{
    if (problemId.isEmpty() || languageId.isEmpty())
        return;

    QString path = SolutionsBasePath + problemId + ".json";

    QJsonObject root;

    // Load existing solutions
    QFile file(path);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        root = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }

    // Update this language's solution
    root[languageId] = code;

    // Save
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
    if (problemId.isEmpty() || languageId.isEmpty())
        return QString();

    QString path = SolutionsBasePath + problemId + ".json";

    QFile file(path);
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
        return QString();

    QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    return root.value(languageId).toString();
}



// ```

// ## Key Changes:

// 1. **Removed Tree-sitter** - The new `CodeEditor` uses QScintilla's built-in lexers
// 2. **Removed manual Scintilla background hacks** - Now handled internally by `CodeEditor::lockBackground()`
// 3. **Added theme support** - `codeEditor->setTheme("tokyo night")`
// 4. **Added editor settings** - Font size, tab size, whitespace visibility, etc.
// 5. **Connected new signals** - `cursorPositionChanged` for cursor position display
// 6. **Added cursor position label** - Shows "Ln X, Col Y" in toolbar
// 7. **Language switching updates editor** - `codeEditor->setLanguage(langId)` now properly updates syntax highlighting
// 8. **Cleaner code** - Removed unused includes and simplified setup

// ## Available Themes:
// - `"one dark pro"` - Atom One Dark
// - `"dracula"` - Dracula
// - `"monokai"` - Monokai
// - `"github dark"` - GitHub Dark Dimmed
// - `"tokyo night"` - Tokyo Night

// ## Available Editor Features:
// - **Ctrl+/** - Toggle comment
// - **Ctrl+Shift+D** - Duplicate line
// - **Ctrl+Shift+K** - Delete line
// - **Alt+Up/Down** - Move line up/down
// - **Ctrl+Scroll** - Zoom in/out
// - **Ctrl+k**      - change theme [just shortcut not Gui in testing state]
// - Auto-close brackets `{}`, `()`, `[]`, `""`, `''`
// - Code folding
// - Brace matching
// - Autocomplete

