#include "mainwindow.h"
#include "code_editor.h"
#include "problem_panel.h"
#include "testcase_panel.h"
#include "problemwidgets.h"
#include "hoversidebar.h"
#include "TreeSitterHighlighter.h"
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
// Tree-sitter C++ Parser
// ═══════════════════════════════════════════════════════════════════════════

extern "C" {
const TSLanguage* tree_sitter_cpp(void);
}

namespace {

const std::string CppHighlightQuery = R"(
["extern" "catch" "class" "delete" "namespace" "new" "private" "protected" "public"
 "throw" "try" "requires" "virtual" "break" "case" "const" "continue" "do" "else"
 "enum" "for" "if"  "inline" "return" "static" "struct" "switch" "typedef" "while" ] @keyword

["#define" "#elif" "#else" "#endif" "#if" "#ifdef" "#ifndef" "#include" ] @preproc
(preproc_directive) @preproc

(number_literal) @number

(true) @bool
(false) @bool

(string_literal) @string.std
(raw_string_literal) @string.raw

(type_identifier) @type
(primitive_type) @type
(sized_type_specifier) @type

(field_identifier) @variable
(identifier) @variable

(call_expression
    function: (identifier) @function)
(call_expression
    function: (field_expression
    field: (field_identifier) @function))
(function_declarator
    declarator: (identifier) @function)

(comment) @comment
)";

FormatMap createFormatMap()
{
    FormatMap map;

    map[""].setForeground(QColor("#ABB2BF"));
    map["keyword"].setForeground(QColor("#C678DD"));
    map["preproc"].setForeground(QColor("#E5C07B"));
    map["number"].setForeground(QColor("#D19A66"));
    map["bool"].setForeground(QColor("#56B6C2"));
    map["bool"].setFontWeight(QFont::Bold);
    map["string"].setForeground(QColor("#98C379"));
    map["string.std"].setForeground(QColor("#98C379"));
    map["string.raw"].setForeground(QColor("#7EC07B"));
    map["type"].setForeground(QColor("#E5C07B"));
    map["variable"].setForeground(QColor("#ABB2BF"));
    map["function"].setForeground(QColor("#61AFEF"));
    map["comment"].setForeground(QColor("#5C6370"));
    map["comment"].setFontItalic(true);

    return map;
}

} // anonymous namespace

// ═══════════════════════════════════════════════════════════════════════════
// Constructor / Destructor
// ═══════════════════════════════════════════════════════════════════════════

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1400, 900);
    const QString appDir = QCoreApplication::applicationDirPath();
        ProblemsBasePath = QDir(appDir).filePath("problems/");
        ProblemsJsonPath = QDir(appDir).filePath("problems.json");

        // 2. Now you can safely use qDebug
        qDebug() << ProblemsBasePath << "-- [the ProblemsBasePath]";
        qDebug() << ProblemsJsonPath << "-- [the ProblemsJsonPath]";
        progressManager = new ProgressManager();


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

    browser = new ProblemBrowser(progressManager,this);
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

    toolbarLayout->addWidget(languageCombo);
    toolbarLayout->addWidget(langIndicator);
    toolbarLayout->addStretch();

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

    // Code Editor
    codeEditor = createEditor();
    codeEditor->setMinimumHeight(200);

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


    connect(codeEditor, &QPlainTextEdit::textChanged, this, [this]() {
        m_isShowingTemplate = false;
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

    // Open language config: Ctrl+,
    auto *openConfig = new QShortcut(QKeySequence("Ctrl+,"), this);
    connect(openConfig, &QShortcut::activated, m_backend, &Backend::openConfigDirectory);
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
    updateLanguageIndicator();

    QString newLangId = languageCombo->currentData().toString();
    QString newTemplate = m_backend->getTemplate(newLangId);

    if (!m_isShowingTemplate)
        return;

    if (m_currentTemplateLanguage == newLangId)
        return;


    if (newTemplate.isEmpty())
        return;

    codeEditor->blockSignals(true);
    codeEditor->setPlainText(newTemplate);
    codeEditor->blockSignals(false);

    m_currentTemplateLanguage = newLangId;
    m_isShowingTemplate = true;
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
// Factory Methods
// ═══════════════════════════════════════════════════════════════════════════

CodeEditor* MainWindow::createEditor()
{
    auto *editor = new CodeEditor(this);

    createHighlighter(editor->document());

    editor->setStyleSheet(R"(
        CodeEditor {
            background-color: #0d0d0d;
            color: #d4d4d4;
            border: none;
            font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', monospace;
            font-size: 14px;
        }
    )");

    // Set default C++ template
    QString defaultLang = "cpp";
    QString defaultTemplate = m_backend->getTemplate(defaultLang);

    if (!defaultTemplate.isEmpty()) {
        editor->setPlainText(defaultTemplate);
    } else {
        editor->setPlainText(R"(#include <iostream>
using namespace std;

int main() {
    return 0;
}
)");
    }

    m_isShowingTemplate = true;
    m_currentTemplateLanguage = defaultLang;

    editor->setContentsMargins(0, 0, 0, 0);

    return editor;
}

TreeSitterHighlighter* MainWindow::createHighlighter(QTextDocument *document)
{
    return new TreeSitterHighlighter(
        tree_sitter_cpp(),
        CppHighlightQuery,
        createFormatMap(),
        document
        );
}

// ═══════════════════════════════════════════════════════════════════════════
// Navigation Slots
// ═══════════════════════════════════════════════════════════════════════════

void MainWindow::onNavigateToEditor(const QString &path)
{
    const QString filePath = ProblemsBasePath + path;
    m_currentProblemId = extractProblemId(path);

    qDebug() << ">>> Opening editor for:" << filePath;

    if (!problemPanel->loadFromFile(filePath)) {
        qWarning() << "Failed to load problem:" << filePath;
        return;
    }

    // Store the full path to the problem file
    m_currentProblemPath = filePath;
    qDebug() << "Problem path:" << m_currentProblemPath;

    // Reset test results
    testCasePanel->clearAllResults();

    // Switch to editor view
    stack->setCurrentWidget(editorPage);

    QString langId = languageCombo->currentData().toString();
    QString tpl = m_backend->getTemplate(langId);

    codeEditor->blockSignals(true);
    codeEditor->setPlainText(tpl);
    codeEditor->blockSignals(false);

    m_isShowingTemplate = true;
    m_currentTemplateLanguage = langId;

    // Focus the code editor
    codeEditor->setFocus();

}

void MainWindow::onNavigateToBrowser()
{
    // Stop any running execution
    if (m_backend->isRunning()) {
        m_backend->stopExecution();
    }

    m_currentProblemPath.clear();  // Clear the path
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
        codeEditor->toPlainText(),
        langId,
        currentIndex,
        m_currentProblemPath  // Pass full path
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
        codeEditor->toPlainText(),
        langId,
        m_currentProblemPath  // Pass full path
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

    // Also show a message box for visibility
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

        // ✅ Mark solved only if ALL passed
        if (total > 0 && passed == total) {
            qDebug() << "All test cases passed. Marking as solved.";

            progressManager->markSolved(m_currentProblemId, true);

            qDebug() << "Current ID:" << extractProblemId(m_currentProblemId);
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

    // Change cursor on code editor during execution
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

QString MainWindow::extractProblemId(const QString &fullPath) const
{
    QFileInfo info(fullPath);
    return info.completeBaseName();
}
