#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciapis.h>

#include <QMap>
#include <QTimer>

// --- Central Indicator Definitions ---
enum EditorIndicator {
    // ... your other indicators ...
    INDICATOR_BRACE_MATCH = 12,

    // NEW: Indicators for warnings and errors
    INDICATOR_WARNING = 2,
    INDICATOR_ERROR   = 1,

    INDICATOR_BRACKET_BASE = 17,
    // ...
};

enum CustomIndicator {
    // We can define other indicators here in the future
    INDICATOR_COLORBLOCK_BASE = 20, // As used by your colorBlockRx

    // Let's reserve the highest available indicators for brackets to be safe.
    // INDICATOR_BRACKET_BASE = 17, // Using 29, 30, 31
    MAX_BRACKET_COLORS = 3
};


// ═══════════════════════════════════════════════════════════════════════════
// Theme Definition
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
    QColor braceUnmatchBg;  // no foreground since we use Rainbow bracket

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
// Custom QScintilla Editor
// ═══════════════════════════════════════════════════════════════════════════

class CodeEditor : public QsciScintilla {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor() override;

    void setLexer(QsciLexer *lexer) override;

    void registerExtraKeywords(const QString &lexerName,
                               int set,
                               const QStringList &words);

    // Language support
    void setLanguage(const QString &langId);
    QString currentLanguage() const { return m_currentLanguage; }

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

    void setKeywordSet(int set, const char* words);

    // Data for keyPressEvent
    const QMap<QChar, QChar> bracketPairs = {
        {'(', ')'}, {'{', '}'}, {'[', ']'}, {'"', '"'}, {'\'', '\''}
    };

    // rainbow bracket

    // void setupIndicators_bracket();  // good at public
    void setupBracketIndicators();   // good at public
    QTimer *m_highlightTimer;
    QVector<QColor> m_bracketColors;
    QVector<QColor> bracketColors;
    QTimer *m_idleProcessingTimer; // Single timer for all idle tasks
    void setupColorBlockIndicators(QsciScintilla* editor);









signals:
    void languageChanged(const QString &langId);
    void cursorPositionChanged(int line, int column);
    void toggleCommentRequested();


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void onCursorPositionChanged(int line, int index);
    void onTextChanged();
    void updateColorBlocks();

    void onIdleTimeout();              // private slot
    void highlightVisibleBrackets();  // private slot
    void scheduleIdleProcessing();


private:

    void applyExtraKeywords();

    QMap<QString, QMap<int, QStringList>> keywordRegistry;

    // Setup methods
    void setupEditor();
    void setupMargins();
    void setupFolding();
    void setupIndicators();
    void setupShortcuts();
    void setupAutocompletion();

    // Lexer management
    void createLexers();
    void applyLexer(QsciLexer *lexer);
    void themeLexer(QsciLexer *lexer);


    // Specific lexer theming
    void themeCppLexer(QsciLexerCPP *lexer);
    void themePythonLexer(QsciLexerPython *lexer);
    void themeJavaScriptLexer(QsciLexerJavaScript *lexer);
    // Helper methods
    void lockBackground();
    void applyTheme();
    long toScintillaColor(const QColor &color);
    QString getCommentPrefix() const;

    // Lexer instances (lazy-loaded)
    QsciLexerCPP *m_lexerCpp = nullptr;
    QsciLexerCPP *m_lexerC = nullptr;
    QsciLexerPython *m_lexerPython = nullptr;
    QsciLexerJavaScript *m_lexerJavaScript = nullptr;
    // API instances for autocomplete
    QMap<QString, QsciAPIs*> m_apis;

    // State
    QString m_currentLanguage = "cpp";
    EditorTheme m_theme;
    QString m_fontFamily = "JetBrains Mono";
    int m_fontSize = 12;
    int m_tabSize = 4;

    // Timers
    // QTimer *m_colorBlockTimer = nullptr;

    // Indicator IDs
    static const int IndicatorError = 8;
    static const int IndicatorWarning = 9;
    static const int IndicatorColorBlockStart = 10;
    static const int IndicatorColorBlockEnd = 19;
};

// Alias for backward compatibility
using CustomQsciEditor = CodeEditor;

#endif // CODE_EDITOR_H
