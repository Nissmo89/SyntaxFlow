#include "code_editor.h"

#include <QVBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QUrl>
#include <QWebEngineSettings>
#include <QWebEnginePage>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

// ═══════════════════════════════════════════════════════════════════════════
// EditorTheme Static Factory Implementations
// ═══════════════════════════════════════════════════════════════════════════

EditorTheme EditorTheme::oneDarkPro() {
    EditorTheme t;
    t.name = "one dark pro";
    t.background = QColor("#282c34");
    t.foreground = QColor("#abb2bf");
    return t;
}

EditorTheme EditorTheme::dracula() {
    EditorTheme t;
    t.name = "dracula";
    t.background = QColor("#282a36");
    t.foreground = QColor("#f8f8f2");
    return t;
}

EditorTheme EditorTheme::monokai() {
    EditorTheme t;
    t.name = "monokai";
    t.background = QColor("#272822");
    t.foreground = QColor("#f8f8f2");
    return t;
}

EditorTheme EditorTheme::githubDark() {
    EditorTheme t;
    t.name = "github dark";
    t.background = QColor("#0d1117");
    t.foreground = QColor("#c9d1d9");
    return t;
}

EditorTheme EditorTheme::tokyoNight() {
    EditorTheme t;
    t.name = "tokyo night";
    t.background = QColor("#1a1b26");
    t.foreground = QColor("#a9b1d6");
    return t;
}

// ═══════════════════════════════════════════════════════════════════════════
// CodeEditor Implementation
// ═══════════════════════════════════════════════════════════════════════════

CodeEditor::CodeEditor(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_view = new QWebEngineView(this);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    m_view->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    m_channel = new QWebChannel(m_view->page());
    m_bridge = new CodeEditorBridge(this);

    m_channel->registerObject("editorBridge", m_bridge);
    m_view->page()->setWebChannel(m_channel);

    connect(m_bridge, &CodeEditorBridge::textChanged,
            this, &CodeEditor::onBridgeTextChanged);
    connect(m_bridge, &CodeEditorBridge::cursorPositionChanged,
            this, &CodeEditor::onBridgeCursorPositionChanged);
    connect(m_bridge, &CodeEditorBridge::editorReady,
            this, &CodeEditor::onBridgeEditorReady);

    layout->addWidget(m_view);

    // Locate index.html
    const QString appDir = QCoreApplication::applicationDirPath();
    QString htmlPath = QDir(appDir).filePath("web_editor/index.html");
    if (!QFile::exists(htmlPath)) {
        // Fallback to source directory if running uninstalled
        htmlPath = QDir(appDir + "/../web_editor").filePath("index.html");
    }

    qDebug() << "Loading WebEngine CodeMirror from:" << htmlPath;
    m_view->setUrl(QUrl::fromLocalFile(htmlPath));

    m_theme = EditorTheme::monokai();
}

CodeEditor::~CodeEditor() = default;

void CodeEditor::runJs(const QString &script) {
    if (m_view && m_view->page()) {
        m_view->page()->runJavaScript(script);
    }
}

void CodeEditor::onBridgeEditorReady() {
    m_isReady = true;
    syncStateToJs();
}

void CodeEditor::syncStateToJs() {
    if (!m_isReady) return;
    setText(m_text);
    setLanguage(m_currentLanguage);
    setTheme(m_theme.name);
    setFontSize(m_fontSize);
    setTabSize(m_tabSize);
    setWordWrap(m_wordWrap);
}

void CodeEditor::setText(const QString &text) {
    m_text = text;
    if (m_isReady) {
        QString jsonStr = QJsonDocument(QJsonArray{text}).toJson(QJsonDocument::Compact);
        QString val = jsonStr.mid(1, jsonStr.length() - 2);
        runJs(QString("window.setCode(%1);").arg(val));
    }
}

void CodeEditor::setLanguage(const QString &langId) {
    m_currentLanguage = langId;
    if (m_isReady) {
        runJs(QString("window.setLanguage('%1');").arg(langId));
    }
    emit languageChanged(langId);
}

void CodeEditor::setTheme(const EditorTheme &theme) {
    m_theme = theme;
    setTheme(theme.name);
}

void CodeEditor::setTheme(const QString &themeName) {
    m_theme.name = themeName;
    if (m_isReady) {
        runJs(QString("window.setTheme('%1');").arg(themeName));
    }
}

void CodeEditor::setFontFamily(const QString &family) {
    m_fontFamily = family;
}

void CodeEditor::setFontSize(int size) {
    m_fontSize = size;
    if (m_isReady) {
        runJs(QString("window.setFontSize(%1);").arg(size));
    }
}

void CodeEditor::setTabSize(int size) {
    m_tabSize = size;
    if (m_isReady) {
        runJs(QString("window.setTabSize(%1);").arg(size));
    }
}

void CodeEditor::setShowWhitespace(bool show) {
    m_showWhitespace = show;
    if (m_isReady) {
        runJs(QString("window.setShowWhitespace(%1);").arg(show ? "true" : "false"));
    }
}

void CodeEditor::setShowIndentGuides(bool show) {
    m_showIndentGuides = show;
    if (m_isReady) {
        runJs(QString("window.setShowIndentGuides(%1);").arg(show ? "true" : "false"));
    }
}

void CodeEditor::setWordWrap(bool wrap) {
    m_wordWrap = wrap;
    if (m_isReady) {
        runJs(QString("window.setWordWrap(%1);").arg(wrap ? "true" : "false"));
    }
}

void CodeEditor::setReadOnly(bool readOnly) {
    if (m_isReady) {
        runJs(QString("window.setReadOnly(%1);").arg(readOnly ? "true" : "false"));
    }
}

void CodeEditor::setMinimap(bool) {}
void CodeEditor::setAutoCompleteEnabled(bool) {}
void CodeEditor::setBraceMatchingEnabled(bool) {}
void CodeEditor::setAutoIndentEnabled(bool) {}

void CodeEditor::goToLine(int line) {
    if (m_isReady) {
        runJs(QString("window.goToLine(%1);").arg(line));
    }
}

void CodeEditor::duplicateLine() {
    if (m_isReady) {
        runJs("window.duplicateLine();");
    }
}

void CodeEditor::deleteLine() {
    if (m_isReady) {
        runJs("window.deleteLine();");
    }
}

void CodeEditor::moveLineUp() {
    if (m_isReady) {
        runJs("window.moveLineUp();");
    }
}

void CodeEditor::moveLineDown() {
    if (m_isReady) {
        runJs("window.moveLineDown();");
    }
}

void CodeEditor::toggleComment() {
    if (m_isReady) {
        runJs("window.toggleComment();");
    }
    emit toggleCommentRequested();
}

void CodeEditor::setFocus() {
    QWidget::setFocus();
    if (m_view) {
        m_view->setFocus();
    }
    if (m_isReady) {
        runJs("window.focusEditor();");
    }
}

void CodeEditor::onBridgeTextChanged(const QString &text) {
    if (m_text != text) {
        m_text = text;
        emit textChanged();
    }
}

void CodeEditor::onBridgeCursorPositionChanged(int line, int col) {
    emit cursorPositionChanged(line, col);
}
