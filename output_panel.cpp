#include "output_panel.h"
#include <QScrollBar>
#include <QDateTime>
#include <QFontDatabase>
#include <QApplication>
#include <QTextCursor>
#include <QDebug>

OutputPanel::OutputPanel(QWidget *parent)
    : QWidget(parent)
    , m_isRunning(false)
    , m_startTime(0)
{
    // ✅ Init timer FIRST — setupUI() → setRunning(false) → m_elapsedTimer->stop()
    m_elapsedTimer = new QTimer(this);
    m_elapsedTimer->setSingleShot(false);
    m_elapsedTimer->setInterval(100);
    connect(m_elapsedTimer, &QTimer::timeout, this, &OutputPanel::updateElapsedTime);

    setupUI();

    m_outputStyle = "color: #d4d4d4;";
    m_errorStyle  = "color: #f44747;";
    m_infoStyle   = "color: #6a9955; font-style: italic;";
    m_inputStyle  = "color: #dcdcaa;";
}

void OutputPanel::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupToolbar();
    setupOutputArea();
    setupInputArea();

    // Initial state - don't call setRunning here to avoid issues
    m_runButton->setVisible(true);
    m_runButton->setEnabled(true);
    m_stopButton->setVisible(false);
    m_inputField->setEnabled(false);
    m_statusLabel->setText("● Ready");
    m_statusLabel->setStyleSheet("color: #6a9955; font-size: 11px;");
}

void OutputPanel::setupToolbar()
{
    auto *toolbar = new QWidget;
    toolbar->setObjectName("outputToolbar");
    toolbar->setFixedHeight(40);
    toolbar->setStyleSheet(R"(
        #outputToolbar {
            background: #1e1e1e;
            border-bottom: 1px solid #333;
        }
    )");

    auto *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(10, 0, 10, 0);
    toolbarLayout->setSpacing(8);

    // Title
    auto *titleLabel = new QLabel("Console");
    titleLabel->setStyleSheet("color: #888; font-weight: bold; font-size: 12px;");

    // Status indicator
    m_statusLabel = new QLabel("● Ready");
    m_statusLabel->setStyleSheet("color: #6a9955; font-size: 11px;");

    // Elapsed time
    m_timeLabel = new QLabel("");
    m_timeLabel->setStyleSheet("color: #666; font-size: 11px;");

    // Run button
    m_runButton = new QPushButton("▶ Run");
    m_runButton->setObjectName("runBtn");
    m_runButton->setCursor(Qt::PointingHandCursor);
    m_runButton->setFixedHeight(28);
    m_runButton->setStyleSheet(R"(
        #runBtn {
            background: #1a6330;
            color: #d4f5de;
            border: 1px solid #2ea043;
            border-radius: 4px;
            padding: 4px 12px;
            font-size: 11px;
            font-weight: 600;
        }
        #runBtn:hover {
            background: #238636;
            color: #fff;
        }
        #runBtn:disabled {
            background: #0e2e1a;
            color: #3a5a3a;
            border-color: #1a3a1a;
        }
    )");

    // Stop button
    m_stopButton = new QPushButton("■ Stop");
    m_stopButton->setObjectName("stopBtn");
    m_stopButton->setCursor(Qt::PointingHandCursor);
    m_stopButton->setFixedHeight(28);
    m_stopButton->setVisible(false);
    m_stopButton->setStyleSheet(R"(
        #stopBtn {
            background: #6b1a1a;
            color: #fca5a5;
            border: 1px solid #c53030;
            border-radius: 4px;
            padding: 4px 12px;
            font-size: 11px;
            font-weight: 600;
        }
        #stopBtn:hover {
            background: #9b2c2c;
            color: #fff;
        }
    )");

    // Clear button
    m_clearButton = new QPushButton("🗑 Clear");
    m_clearButton->setObjectName("clearBtn");
    m_clearButton->setCursor(Qt::PointingHandCursor);
    m_clearButton->setFixedHeight(28);
    m_clearButton->setStyleSheet(R"(
        #clearBtn {
            background: transparent;
            color: #888;
            border: 1px solid #444;
            border-radius: 4px;
            padding: 4px 12px;
            font-size: 11px;
        }
        #clearBtn:hover {
            background: #333;
            color: #ccc;
            border-color: #555;
        }
    )");

    toolbarLayout->addWidget(titleLabel);
    toolbarLayout->addWidget(m_statusLabel);
    toolbarLayout->addWidget(m_timeLabel);
    toolbarLayout->addStretch();
    toolbarLayout->addWidget(m_runButton);
    toolbarLayout->addWidget(m_stopButton);
    toolbarLayout->addWidget(m_clearButton);

    // Connections
    connect(m_runButton, &QPushButton::clicked, this, &OutputPanel::onRunClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &OutputPanel::onStopClicked);
    connect(m_clearButton, &QPushButton::clicked, this, &OutputPanel::clear);
    connect(m_clearButton, &QPushButton::clicked, this, &OutputPanel::clearRequested);

    static_cast<QVBoxLayout*>(layout())->addWidget(toolbar);
}

void OutputPanel::setupOutputArea()
{
    m_outputArea = new QPlainTextEdit;
    m_outputArea->setObjectName("outputArea");
    m_outputArea->setReadOnly(true);
    m_outputArea->setWordWrapMode(QTextOption::WrapAnywhere);

    // Monospace font
    QFont font("JetBrains Mono", 11);
    font.setStyleHint(QFont::Monospace);
    if (!QFontDatabase::hasFamily("JetBrains Mono")) {
        font.setFamily("Consolas");
    }
    m_outputArea->setFont(font);

    m_outputArea->setStyleSheet(R"(
        #outputArea {
            background: #0d0d0d;
            color: #d4d4d4;
            border: none;
            padding: 10px;
            selection-background-color: #264f78;
        }
    )");

    m_outputArea->setPlaceholderText("Program output will appear here...");

    static_cast<QVBoxLayout*>(layout())->addWidget(m_outputArea, 1);
}

void OutputPanel::setupInputArea()
{
    auto *inputContainer = new QWidget;
    inputContainer->setObjectName("inputContainer");
    inputContainer->setStyleSheet(R"(
        #inputContainer {
            background: #1a1a1a;
            border-top: 1px solid #333;
        }
    )");

    auto *inputLayout = new QHBoxLayout(inputContainer);
    inputLayout->setContentsMargins(10, 8, 10, 8);
    inputLayout->setSpacing(8);

    // Input prompt
    auto *promptLabel = new QLabel(">");
    promptLabel->setStyleSheet("color: #569cd6; font-weight: bold; font-size: 14px;");

    // Input field
    m_inputField = new QLineEdit;
    m_inputField->setObjectName("inputField");
    m_inputField->setPlaceholderText("Run program to enable input...");
    m_inputField->setEnabled(false);

    QFont font("JetBrains Mono", 11);
    font.setStyleHint(QFont::Monospace);
    if (!QFontDatabase::hasFamily("JetBrains Mono")) {
        font.setFamily("Consolas");
    }
    m_inputField->setFont(font);

    m_inputField->setStyleSheet(R"(
        #inputField {
            background: #252525;
            color: #d4d4d4;
            border: 1px solid #3a3a3a;
            border-radius: 4px;
            padding: 6px 10px;
        }
        #inputField:focus {
            border-color: #569cd6;
        }
        #inputField:disabled {
            background: #1a1a1a;
            color: #555;
            border-color: #2a2a2a;
        }
    )");

    // Submit button
    auto *submitBtn = new QPushButton("Send");
    submitBtn->setObjectName("submitInputBtn");
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setFixedSize(60, 30);
    submitBtn->setStyleSheet(R"(
        #submitInputBtn {
            background: #0e639c;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 11px;
            font-weight: 600;
        }
        #submitInputBtn:hover {
            background: #1177bb;
        }
        #submitInputBtn:disabled {
            background: #1a3a4a;
            color: #555;
        }
    )");

    inputLayout->addWidget(promptLabel);
    inputLayout->addWidget(m_inputField, 1);
    inputLayout->addWidget(submitBtn);

    // Connections
    connect(m_inputField, &QLineEdit::returnPressed, this, &OutputPanel::onInputSubmit);
    connect(submitBtn, &QPushButton::clicked, this, &OutputPanel::onInputSubmit);

    static_cast<QVBoxLayout*>(layout())->addWidget(inputContainer);
}

// ═══════════════════════════════════════════════════════════════════════════
// Output Methods
// ═══════════════════════════════════════════════════════════════════════════

void OutputPanel::appendOutput(const QString &text)
{
    if (text.isEmpty()) return;

    QTextCursor cursor = m_outputArea->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format;
    format.setForeground(QColor("#d4d4d4"));

    cursor.insertText(text, format);

    m_outputArea->verticalScrollBar()->setValue(
        m_outputArea->verticalScrollBar()->maximum());
}

void OutputPanel::appendError(const QString &text)
{
    if (text.isEmpty()) return;

    QTextCursor cursor = m_outputArea->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format;
    format.setForeground(QColor("#f44747"));

    cursor.insertText(text, format);

    m_outputArea->verticalScrollBar()->setValue(
        m_outputArea->verticalScrollBar()->maximum());
}

void OutputPanel::appendInfo(const QString &text)
{
    if (text.isEmpty()) return;

    QTextCursor cursor = m_outputArea->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format;
    format.setForeground(QColor("#6a9955"));
    format.setFontItalic(true);

    cursor.insertText(text, format);

    m_outputArea->verticalScrollBar()->setValue(
        m_outputArea->verticalScrollBar()->maximum());
}

void OutputPanel::appendInput(const QString &text)
{
    if (text.isEmpty()) return;

    QTextCursor cursor = m_outputArea->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format;
    format.setForeground(QColor("#dcdcaa"));

    cursor.insertText("> " + text + "\n", format);

    m_outputArea->verticalScrollBar()->setValue(
        m_outputArea->verticalScrollBar()->maximum());
}

void OutputPanel::clear()
{
    m_outputArea->clear();
    m_timeLabel->clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// State Management
// ═══════════════════════════════════════════════════════════════════════════

void OutputPanel::setRunning(bool running)
{
    m_isRunning = running;

    if (m_runButton) {
        m_runButton->setVisible(!running);
        m_runButton->setEnabled(!running);
    }

    if (m_stopButton) {
        m_stopButton->setVisible(running);
    }

    if (running) {
        if (m_statusLabel) {
            m_statusLabel->setText("● Running");
            m_statusLabel->setStyleSheet("color: #dcdcaa; font-size: 11px;");
        }
        if (m_inputField) {
            m_inputField->setEnabled(true);
            m_inputField->setPlaceholderText("Enter input and press Enter...");
        }
        m_startTime = QDateTime::currentMSecsSinceEpoch();
        if (m_elapsedTimer) {
            m_elapsedTimer->start();
        }
    } else {
        if (m_statusLabel) {
            m_statusLabel->setText("● Finished");
            m_statusLabel->setStyleSheet("color: #6a9955; font-size: 11px;");
        }
        if (m_inputField) {
            m_inputField->setEnabled(false);
            m_inputField->setPlaceholderText("Run program to enable input...");
            m_inputField->clear();
        }
        if (m_elapsedTimer) {
            m_elapsedTimer->stop();
        }
    }
}

void OutputPanel::setInputEnabled(bool enabled)
{
    if (m_inputField) {
        m_inputField->setEnabled(enabled);
    }
}

void OutputPanel::focusInput()
{
    if (m_inputField) {
        m_inputField->setFocus();
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// Slots
// ═══════════════════════════════════════════════════════════════════════════

void OutputPanel::onInputSubmit()
{
    if (!m_inputField) return;

    QString input = m_inputField->text();

    // Allow empty input when running (some programs need just Enter)
    if (!m_isRunning) return;

    qDebug() << ">>> OutputPanel: Input submitted:" << input;

    // Echo input to output area
    appendInput(input);

    // Clear input field
    m_inputField->clear();

    // Emit signal to send to backend
    emit inputSubmitted(input);
}

void OutputPanel::onRunClicked()
{
    qDebug() << ">>> OutputPanel: Run clicked";
    clear();
    appendInfo("[Starting execution...]\n");
    emit runRequested();
}

void OutputPanel::onStopClicked()
{
    qDebug() << ">>> OutputPanel: Stop clicked";
    appendInfo("\n[Execution stopped by user]\n");
    emit stopRequested();
}

void OutputPanel::updateElapsedTime()
{
    if (!m_timeLabel) return;

    qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - m_startTime;
    double seconds = elapsed / 1000.0;
    m_timeLabel->setText(QString("%1s").arg(seconds, 0, 'f', 1));
}
