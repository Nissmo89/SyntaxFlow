#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QColor>
#include <QVector>
#include <QMap>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

// ═══════════════════════════════════════════════════════════════════════════
// Theme Definition (Maintained for compatibility)
// ═══════════════════════════════════════════════════════════════════════════

struct EditorTheme {
    QString name;

    // Base colors
    QColor background;
    QColor foreground;
    QColor lineHighlight;
    QColor selection;
    QColor selectionForeground;
    QColor caret;
    QColor invisibles;

    // Margin colors
    QColor marginBackground;
    QColor marginForeground;
    QColor foldMarginBackground;

    // Syntax colors
    QColor keyword;
    QColor keywordSecondary;
    QColor string;
    QColor stringEscape;
    QColor comment;
    QColor commentDoc;
    QColor number;
    QColor operator_;
    QColor identifier;
    QColor type;
    QColor function;
    QColor preprocessor;
    QColor decorator;
    QColor className;
    QColor constant;
    QColor error;

    // Brace matching
    QColor braceMatchBg;
    QColor braceUnmatchBg;

    QVector<QColor> bracketColors;

    // Indentation guides
    QColor indentGuide;

    static EditorTheme oneDarkPro();
    static EditorTheme dracula();
    static EditorTheme monokai();
    static EditorTheme githubDark();
    static EditorTheme tokyoNight();
};

// ═══════════════════════════════════════════════════════════════════════════
// WebChannel Communication Bridge
// ═══════════════════════════════════════════════════════════════════════════

class CodeEditorBridge : public QObject {
    Q_OBJECT
public:
    explicit CodeEditorBridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void textChanged(const QString &text);
    void cursorPositionChanged(int line, int col);
    void editorReady();

public slots:
    void onTextChanged(const QString &text) {
        emit textChanged(text);
    }

    void onCursorPositionChanged(int line, int col) {
        emit cursorPositionChanged(line, col);
    }

    void onEditorReady() {
        emit editorReady();
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// WebEngine + CodeMirror Code Editor Widget
// ═══════════════════════════════════════════════════════════════════════════

class CodeEditor : public QWidget {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor() override;

    // Language support
    void setLanguage(const QString &langId);
    QString currentLanguage() const { return m_currentLanguage; }

    // Text content
    void setText(const QString &text);
    QString text() const { return m_text; }

    // Theming
    void setTheme(const EditorTheme &theme);
    void setTheme(const QString &themeName);
    EditorTheme currentTheme() const { return m_theme; }

    // Editor settings
    void setFontFamily(const QString &family);
    void setFontSize(int size);
    void setTabSize(int size);
    void setShowWhitespace(bool show);
    void setShowIndentGuides(bool show);
    void setWordWrap(bool wrap);
    void setReadOnly(bool readOnly);
    void setMinimap(bool show);

    // Code intelligence
    void setAutoCompleteEnabled(bool enabled);
    void setBraceMatchingEnabled(bool enabled);
    void setAutoIndentEnabled(bool enabled);

    // Utilities
    void goToLine(int line);
    void duplicateLine();
    void deleteLine();
    void moveLineUp();
    void moveLineDown();
    void toggleComment();
    void setFocus();

signals:
    void languageChanged(const QString &langId);
    void cursorPositionChanged(int line, int column);
    void toggleCommentRequested();
    void textChanged();

private slots:
    void onBridgeTextChanged(const QString &text);
    void onBridgeCursorPositionChanged(int line, int col);
    void onBridgeEditorReady();

private:
    void runJs(const QString &script);
    void syncStateToJs();

    QWebEngineView *m_view = nullptr;
    QWebChannel *m_channel = nullptr;
    CodeEditorBridge *m_bridge = nullptr;

    bool m_isReady = false;
    QString m_text;
    QString m_currentLanguage = "cpp";
    EditorTheme m_theme;
    QString m_fontFamily = "JetBrains Mono";
    int m_fontSize = 13;
    int m_tabSize = 4;
    bool m_wordWrap = false;
    bool m_showWhitespace = false;
    bool m_showIndentGuides = true;
};

// Alias for backward compatibility
using CustomQsciEditor = CodeEditor;

#endif // CODE_EDITOR_H
