#include "code_editor.h"

#include <QScrollBar>
#include <QShortcut>
#include <QKeyEvent>
#include <QRegularExpression>
#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QStack>

// #include <libtcc.h>





void CodeEditor::registerExtraKeywords(const QString &lexerName,
                                       int set,
                                       const QStringList &words)
{
    keywordRegistry[lexerName][set].append(words);
}

void CodeEditor::setLexer(QsciLexer *lexer)
{
    QsciScintilla::setLexer(lexer);
    applyExtraKeywords();
}

void CodeEditor::applyExtraKeywords()
{
    QsciLexer *lexer = this->lexer();
    if (!lexer)
        return;

    QString name = lexer->language();  // IMPORTANT

    if (!keywordRegistry.contains(name))
        return;

    auto sets = keywordRegistry[name];

    for (auto it = sets.begin(); it != sets.end(); ++it)
    {
        int set = it.key();
        QStringList words = it.value();

        const char *existing = lexer->keywords(set);
        QString combined = existing ? QString(existing) : "";

        for (const QString &word : words)
        {
            if (!combined.contains(word))
                combined += " " + word;
        }

        QByteArray bytes = combined.toUtf8();

        SendScintilla(QsciScintilla::SCI_SETKEYWORDS,
                      set - 1,
                      bytes.constData());
    }

    recolor();
}


// ═══════════════════════════════════════════════════════════════════════════
// Theme Definitions
// ═══════════════════════════════════════════════════════════════════════════

EditorTheme EditorTheme::oneDarkPro() {
    EditorTheme t;
    t.name = "One Dark Pro";

    // Base
    t.background = QColor("#282c34");
    t.foreground = QColor("#abb2bf");
    t.lineHighlight = QColor("#2c313c");
    t.selection = QColor("#3e4451");
    t.selectionForeground = QColor("#abb2bf");
    t.caret = QColor("#528bff");
    t.invisibles = QColor("#3b4048");

    // Margins
    t.marginBackground = QColor("#282c34");
    t.marginForeground = QColor("#4b5263");
    t.foldMarginBackground = QColor("#282c34");

    // Syntax
    t.keyword = QColor("#c678dd");
    t.keywordSecondary = QColor("#e5c07b");
    t.string = QColor("#98c379");
    t.stringEscape = QColor("#56b6c2");
    t.comment = QColor("#5c6370");
    t.commentDoc = QColor("#5c6370");
    t.number = QColor("#d19a66");
    t.operator_ = QColor("#abb2bf");
    t.identifier = QColor("#e06c75");
    t.type = QColor("#e5c07b");
    t.function = QColor("#61afef");
    t.preprocessor = QColor("#c678dd");
    t.decorator = QColor("#e5c07b");
    t.className = QColor("#e5c07b");
    t.constant = QColor("#d19a66");
    t.error = QColor("#f44747");



    // Guides
    t.indentGuide = QColor("#3b4048");

    t.bracketColors = {
        QColor("#e06c75"), // red
        QColor("#61afef"), // blue
        QColor("#98c379")  // green
    };

    t.braceMatchBg   = QColor("#803D5A80");  // one dark blue-gray
    t.braceUnmatchBg = QColor("#80553333");  // deep neutral red



    return t;
}

EditorTheme EditorTheme::dracula() {
    EditorTheme t;
    t.name = "Dracula";

    t.background = QColor("#282a36");
    t.foreground = QColor("#f8f8f2");
    t.lineHighlight = QColor("#44475a");
    t.selection = QColor("#44475a");
    t.selectionForeground = QColor("#f8f8f2");
    t.caret = QColor("#f8f8f2");
    t.invisibles = QColor("#424450");

    t.marginBackground = QColor("#282a36");
    t.marginForeground = QColor("#6272a4");
    t.foldMarginBackground = QColor("#282a36");

    t.keyword = QColor("#ff79c6");
    t.keywordSecondary = QColor("#bd93f9");
    t.string = QColor("#f1fa8c");
    t.stringEscape = QColor("#ff79c6");
    t.comment = QColor("#6272a4");
    t.commentDoc = QColor("#6272a4");
    t.number = QColor("#bd93f9");
    t.operator_ = QColor("#ffffef");
    t.identifier = QColor("#f8f8f2");
    t.type = QColor("#8be9fd");
    t.function = QColor("#50fa7b");
    t.preprocessor = QColor("#ff79c6");
    t.decorator = QColor("#50fa7b");
    t.className = QColor("#8be9fd");
    t.constant = QColor("#bd93f9");
    t.error = QColor("#ff5555");



    t.indentGuide = QColor("#424450");

    t.bracketColors = {
        QColor("#ff79c6"), // pink
        QColor("#8be9fd"), // cyan
        QColor("#f1fa8c")  // yellow
    };

    t.braceMatchBg   = QColor("#803E68A8");  // muted dracula blue
    t.braceUnmatchBg = QColor("#805A1F2B");  // deep dracula red



    return t;
}

EditorTheme EditorTheme::tokyoNight() {
    EditorTheme t;
    t.name = "Tokyo Night";

    t.background = QColor("#1a1b26");
    t.foreground = QColor("#a9b1d6");
    t.lineHighlight = QColor("#292e42");
    t.selection = QColor("#33467c");
    t.selectionForeground = QColor("#c0caf5");
    t.caret = QColor("#c0caf5");
    t.invisibles = QColor("#363b54");

    t.marginBackground = QColor("#1a1b26");
    t.marginForeground = QColor("#3b4261");
    t.foldMarginBackground = QColor("#1a1b26");

    t.keyword = QColor("#bb9af7");
    t.keywordSecondary = QColor("#7dcfff");
    t.string = QColor("#9ece6a");
    t.stringEscape = QColor("#89ddff");
    t.comment = QColor("#565f89");
    t.commentDoc = QColor("#565f89");
    t.number = QColor("#ff9e64");
    t.operator_ = QColor("#ffffef");
    t.identifier = QColor("#c0caf5");
    t.type = QColor("#2ac3de");
    t.function = QColor("#7aa2f7");
    t.preprocessor = QColor("#bb9af7");
    t.decorator = QColor("#e0af68");
    t.className = QColor("#f7768e");
    t.constant = QColor("#ff9e64");
    t.error = QColor("#f7768e");



    t.indentGuide = QColor("#292e42");


    t.bracketColors = {
        QColor("#7aa2f7"), // blue
        QColor("#bb9af7"), // purple
        QColor("#9ece6a")  // green
    };

    t.braceMatchBg   = QColor("#803A5F8F");  // tokyo muted blue
    t.braceUnmatchBg = QColor("#80502A3A");  // violet-red tone


    return t;
}

EditorTheme EditorTheme::monokai() {
    EditorTheme t;
    t.name = "Monokai";

    t.background = QColor("#272822");
    t.foreground = QColor("#f8f8f2");
    t.lineHighlight = QColor("#3e3d32");
    t.selection = QColor("#49483e");
    t.selectionForeground = QColor("#f8f8f2");
    t.caret = QColor("#f8f8f0");
    t.invisibles = QColor("#3b3a32");

    t.marginBackground = QColor("#272822");
    t.marginForeground = QColor("#90908a");
    t.foldMarginBackground = QColor("#272822");

    t.keyword = QColor("#f92672");
    t.keywordSecondary = QColor("#66d9ef");
    t.string = QColor("#e6db74");
    t.stringEscape = QColor("#ae81ff");
    t.comment = QColor("#75715e");
    t.commentDoc = QColor("#75715e");
    t.number = QColor("#ae81ff");
    t.operator_ = QColor("#ffffef");
    t.identifier = QColor("#f8f8f2");
    t.type = QColor("#66d9ef");
    t.function = QColor("#a6e22e");
    t.preprocessor = QColor("#f92672");
    t.decorator = QColor("#66d9ef");
    t.className = QColor("#a6e22e");
    t.constant = QColor("#ae81ff");
    t.error = QColor("#f92672");



    t.indentGuide = QColor("#3b3a32");

    t.bracketColors = {
        QColor("#f92672"), // red
        QColor("#66d9ef"), // cyan
        QColor("#a6e22e")  // green
    };

    t.braceMatchBg   = QColor("#804E8070");  // desaturated teal-green
    t.braceUnmatchBg = QColor("#80602C2C");  // earthy red



    return t;
}

EditorTheme EditorTheme::githubDark() {
    EditorTheme t;
    t.name = "GitHub Dark";

    t.background = QColor("#0d1117");
    t.foreground = QColor("#c9d1d9");
    t.lineHighlight = QColor("#161b22");
    t.selection = QColor("#264f78");
    t.selectionForeground = QColor("#ffffff");
    t.caret = QColor("#c9d1d9");
    t.invisibles = QColor("#484f58");

    t.marginBackground = QColor("#0d1117");
    t.marginForeground = QColor("#484f58");
    t.foldMarginBackground = QColor("#0d1117");

    t.keyword = QColor("#ff7b72");
    t.keywordSecondary = QColor("#79c0ff");
    t.string = QColor("#a5d6ff");
    t.stringEscape = QColor("#79c0ff");
    t.comment = QColor("#8b949e");
    t.commentDoc = QColor("#8b949e");
    t.number = QColor("#79c0ff");
    t.operator_ = QColor("#ffffef");
    t.identifier = QColor("#c9d1d9");
    t.type = QColor("#ffa657");
    t.function = QColor("#d2a8ff");
    t.preprocessor = QColor("#ff7b72");
    t.decorator = QColor("#d2a8ff");
    t.className = QColor("#ffa657");
    t.constant = QColor("#79c0ff");
    t.error = QColor("#f85149");



    t.indentGuide = QColor("#21262d");

    t.bracketColors = {
        QColor("#ff7b72"), // red
        QColor("#79c0ff"), // blue
        QColor("#ffa657")  // orange
    };

    t.braceMatchBg   = QColor("#804A6C9B");  // github steel blue
    t.braceUnmatchBg = QColor("#80582A2A");  // muted red


    return t;
}

// ═══════════════════════════════════════════════════════════════════════════
// Constructor / Destructor
// ═══════════════════════════════════════════════════════════════════════════

CodeEditor::CodeEditor(QWidget *parent)
    : QsciScintilla(parent)
{
    // Set default theme
    m_theme = EditorTheme::tokyoNight();

    // ── Core setup (no timers, no language yet) ───────────────────────
    setupEditor();
    createLexers();
    setupMargins();
    setupFolding();
    setupIndicators();
    setupShortcuts();

    // ── Connections ───────────────────────────────────────────────────
    connect(this, &QsciScintilla::cursorPositionChanged,
            this, &CodeEditor::onCursorPositionChanged);
    connect(this, &QsciScintilla::textChanged,
            this, &CodeEditor::onTextChanged);

    // ── Timers — create and CONNECT before anything starts them ───────
    m_idleProcessingTimer = new QTimer(this);
    m_idleProcessingTimer->setSingleShot(true);
    m_idleProcessingTimer->setInterval(60);
    connect(m_idleProcessingTimer, &QTimer::timeout,
            this, &CodeEditor::onIdleTimeout);   // ← connected BEFORE start

    m_highlightTimer = new QTimer(this);
    m_highlightTimer->setSingleShot(true);
    m_highlightTimer->setInterval(80);
    connect(m_highlightTimer, &QTimer::timeout,
            this, &CodeEditor::highlightVisibleBrackets);

    // ── Language + bracket indicators (after timers are ready) ────────
    setLanguage("cpp");
    setupBracketIndicators();

    // ── Kick off first highlight after everything is initialized ──────
    QTimer::singleShot(50, this, &CodeEditor::scheduleIdleProcessing);
}

CodeEditor::~CodeEditor() {
    // Lexers are children, Qt handles cleanup
}

// ═══════════════════════════════════════════════════════════════════════════
// Setup Methods
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::setupEditor() {
    setUtf8(true);
    setMouseTracking(true);

    // Font - Use fallback if JetBrains Mono not available
    QStringList fontFamilies = {"JetBrains Mono", "Fira Code", "Cascadia Code",
                                "Source Code Pro", "Consolas", "Monaco", "monospace"};
    QFont font;
    for (const QString &family : fontFamilies) {
        font.setFamily(family);
        if (QFontDatabase::hasFamily(family) || font.exactMatch()) {
            m_fontFamily = family;
            break;
        }
    }

    font.setFamily(m_fontFamily);
    font.setPointSize(m_fontSize);
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    setFont(font);

    // Tabs and indentation
    setIndentationsUseTabs(false);
    setIndentationWidth(m_tabSize);
    setTabWidth(m_tabSize);
    setAutoIndent(true);
    setBackspaceUnindents(true);
    setIndentationGuides(true);
    setTabIndents(true);



    // Visual settings
    setCaretLineVisible(true);
    setCaretWidth(2);
    setCaretForegroundColor(m_theme.caret);
    setCaretLineBackgroundColor(m_theme.lineHighlight);

    // Selection
    setSelectionBackgroundColor(m_theme.selection);
    setSelectionForegroundColor(m_theme.selectionForeground);

    // Whitespace (off by default, cleaner look)
    setWhitespaceVisibility(QsciScintilla::WsInvisible);
    setWhitespaceForegroundColor(m_theme.invisibles);
    setWhitespaceSize(1);

    // Edge line (column indicator)
    setEdgeColumn(100);
    setEdgeMode(QsciScintilla::EdgeNone);  // Disabled by default
    setEdgeColor(m_theme.indentGuide);

    // Wrapping
    setWrapMode(QsciScintilla::WrapNone);
    setWrapIndentMode(QsciScintilla::WrapIndentSame);

    // EOL
    setEolMode(QsciScintilla::EolUnix);
    setEolVisibility(false);

    // Scrolling
    SendScintilla(SCI_SETSCROLLWIDTHTRACKING, true);
    SendScintilla(SCI_SETSCROLLWIDTH, 1);

    // Smooth scrolling
    SendScintilla(SCI_SETMOUSEDWELLTIME, 500);

    // Extra styling
    setStyleSheet("QsciScintilla { border: none; }");

    // Paper (background)
    setPaper(m_theme.background);


}

void CodeEditor::setupMargins() {
    // Line numbers (margin 0)
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginWidth(0, "99999");  // Width for 5 digits
    setMarginsForegroundColor(m_theme.marginForeground);
    setMarginsBackgroundColor(m_theme.marginBackground);
    setMarginsFont(font());

    // Margin for symbols/breakpoints (margin 1)
    setMarginType(1, QsciScintilla::SymbolMargin);
    setMarginWidth(1, 0);  // Disabled by default
    setMarginSensitivity(1, true);

    // Folding margin (margin 2)
    setMarginType(2, QsciScintilla::SymbolMargin);
    setMarginWidth(2, 14);
    setMarginSensitivity(2, true);
}

void CodeEditor::setupFolding() {
    // Enable code folding
    setFolding(QsciScintilla::BoxedTreeFoldStyle, 2);

    // Fold margin colors
    setFoldMarginColors(m_theme.foldMarginBackground, m_theme.foldMarginBackground);

    // Fold markers
    SendScintilla(SCI_MARKERSETFORE, SC_MARKNUM_FOLDER, toScintillaColor(m_theme.marginForeground));
    SendScintilla(SCI_MARKERSETBACK, SC_MARKNUM_FOLDER, toScintillaColor(m_theme.marginBackground));
    SendScintilla(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPEN, toScintillaColor(m_theme.marginForeground));
    SendScintilla(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPEN, toScintillaColor(m_theme.marginBackground));
    SendScintilla(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPENMID, toScintillaColor(m_theme.marginForeground));
    SendScintilla(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPENMID, toScintillaColor(m_theme.marginBackground));
    SendScintilla(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERMIDTAIL, toScintillaColor(m_theme.marginForeground));
    SendScintilla(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, toScintillaColor(m_theme.marginBackground));
    SendScintilla(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERSUB, toScintillaColor(m_theme.marginForeground));
    SendScintilla(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, toScintillaColor(m_theme.marginBackground));
    SendScintilla(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERTAIL, toScintillaColor(m_theme.marginForeground));
    SendScintilla(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, toScintillaColor(m_theme.marginBackground));
    SendScintilla(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEREND, toScintillaColor(m_theme.marginForeground));
    SendScintilla(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEREND, toScintillaColor(m_theme.marginBackground));
}

void CodeEditor::setupIndicators() {
    // Error indicator (red underline)
    SendScintilla(SCI_INDICSETSTYLE, IndicatorError, INDIC_SQUIGGLE);
    SendScintilla(SCI_INDICSETFORE, IndicatorError, toScintillaColor(m_theme.error));
    SendScintilla(SCI_INDICSETUNDER, IndicatorError, true);

    // Warning indicator (yellow underline)
    SendScintilla(SCI_INDICSETSTYLE, IndicatorWarning, INDIC_SQUIGGLE);
    SendScintilla(SCI_INDICSETFORE, IndicatorWarning, toScintillaColor(QColor("#e5c07b")));
    SendScintilla(SCI_INDICSETUNDER, IndicatorWarning, true);

    // Color block indicators
    for (int id = IndicatorColorBlockStart; id <= IndicatorColorBlockEnd; ++id) {
        SendScintilla(SCI_INDICSETSTYLE, id, INDIC_ROUNDBOX);
        SendScintilla(SCI_INDICSETALPHA, id, 60);
        SendScintilla(SCI_INDICSETOUTLINEALPHA, id, 100);
        SendScintilla(SCI_INDICSETFORE, id, toScintillaColor(m_theme.selection));
        SendScintilla(SCI_INDICSETUNDER, id, true);
    }
}

void CodeEditor::setupShortcuts() {
    // Duplicate line: Ctrl+Shift+D
    auto *dupShortcut = new QShortcut(QKeySequence("Ctrl+Shift+D"), this);
    connect(dupShortcut, &QShortcut::activated, this, &CodeEditor::duplicateLine);

    // Delete line: Ctrl+Shift+K
    auto *delShortcut = new QShortcut(QKeySequence("Ctrl+Shift+K"), this);
    connect(delShortcut, &QShortcut::activated, this, &CodeEditor::deleteLine);

    // Move line up: Alt+Up
    auto *moveUpShortcut = new QShortcut(QKeySequence("Alt+Up"), this);
    connect(moveUpShortcut, &QShortcut::activated, this, &CodeEditor::moveLineUp);

    // Move line down: Alt+Down
    auto *moveDownShortcut = new QShortcut(QKeySequence("Alt+Down"), this);
    connect(moveDownShortcut, &QShortcut::activated, this, &CodeEditor::moveLineDown);

    // // Toggle comment: Ctrl+/
    // auto *commentShortcut = new QShortcut(QKeySequence("Ctrl+/"), this);
    // connect(commentShortcut, &QShortcut::activated, this, &CodeEditor::toggleComment);
    connect(this,&CodeEditor::toggleCommentRequested,this,&CodeEditor::toggleComment);
}

void CodeEditor::setupAutocompletion() {
    QsciLexer *lex = lexer();
    if (!lex) return;

    QString lang = m_currentLanguage;

    // Create or reuse API
    if (!m_apis.contains(lang)) {
        QsciAPIs *apis = new QsciAPIs(lex);

        // Add language-specific keywords
        if (lang == "cpp" || lang == "c") {
            QStringList keywords = {
                "auto", "break", "case", "catch", "class", "const", "continue",
                "default", "delete", "do", "else", "enum", "explicit", "extern",
                "false", "for", "friend", "goto", "if", "inline", "mutable",
                "namespace", "new", "nullptr", "operator", "private", "protected",
                "public", "return", "sizeof", "static", "struct", "switch",
                "template", "this", "throw", "true", "try", "typedef", "typename",
                "union", "using", "virtual", "void", "volatile", "while",
                // STL
                "std::cout", "std::cin", "std::endl", "std::vector", "std::string",
                "std::map", "std::set", "std::unordered_map", "std::unordered_set",
                "std::sort", "std::find", "std::begin", "std::end",
                "std::make_pair", "std::make_unique", "std::make_shared",
                // Common includes
                "#include <iostream>", "#include <vector>", "#include <string>",
                "#include <algorithm>", "#include <map>", "#include <set>",
                "#include <bits/stdc++.h>"
            };
            for (const QString &kw : keywords) {
                apis->add(kw);
            }
        }
        else if (lang == "python") {
            QStringList keywords = {
                "and", "as", "assert", "async", "await", "break", "class",
                "continue", "def", "del", "elif", "else", "except", "False",
                "finally", "for", "from", "global", "if", "import", "in",
                "is", "lambda", "None", "nonlocal", "not", "or", "pass",
                "raise", "return", "True", "try", "while", "with", "yield",
                // Built-ins
                "print", "range", "len", "str", "int", "float", "list", "dict",
                "set", "tuple", "open", "input", "enumerate", "zip", "map",
                "filter", "sorted", "reversed", "any", "all", "sum", "min", "max"
            };
            for (const QString &kw : keywords) {
                apis->add(kw);
            }
        }
        else if (lang == "java") {
            QStringList keywords = {
                "abstract", "assert", "boolean", "break", "byte", "case",
                "catch", "char", "class", "const", "continue", "default",
                "do", "double", "else", "enum", "extends", "final", "finally",
                "float", "for", "goto", "if", "implements", "import",
                "instanceof", "int", "interface", "long", "native", "new",
                "package", "private", "protected", "public", "return", "short",
                "static", "strictfp", "super", "switch", "synchronized", "this",
                "throw", "throws", "transient", "try", "void", "volatile", "while",
                // Common
                "System.out.println", "System.out.print", "String", "Integer",
                "ArrayList", "HashMap", "Scanner"
            };
            for (const QString &kw : keywords) {
                apis->add(kw);
            }
        }
        else if (lang == "javascript" || lang == "js") {
            QStringList keywords = {
                "async", "await", "break", "case", "catch", "class", "const",
                "continue", "debugger", "default", "delete", "do", "else",
                "export", "extends", "false", "finally", "for", "function",
                "if", "import", "in", "instanceof", "let", "new", "null",
                "return", "static", "super", "switch", "this", "throw", "true",
                "try", "typeof", "undefined", "var", "void", "while", "with", "yield",
                // Common
                "console.log", "document", "window", "fetch", "JSON.parse",
                "JSON.stringify", "Array", "Object", "Promise", "setTimeout"
            };
            for (const QString &kw : keywords) {
                apis->add(kw);
            }
        }

        apis->prepare();
        m_apis[lang] = apis;
    }

    // Configure autocomplete
    setAutoCompletionThreshold(2);
    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionCaseSensitivity(true);
    setAutoCompletionReplaceWord(true);
    setAutoCompletionUseSingle(QsciScintilla::AcusExplicit);

    // Autocomplete popup style
    SendScintilla(SCI_AUTOCSETMAXHEIGHT, 10);
    SendScintilla(SCI_AUTOCSETMAXWIDTH, 50);
}

// ═══════════════════════════════════════════════════════════════════════════
// Lexer Management
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::createLexers() {
    // Lexers are created on demand in setLanguage()
}

void CodeEditor::setLanguage(const QString &langId) {
    QString lang = langId.toLower();
    m_currentLanguage = lang;

    QsciLexer *activeLexer = nullptr;

    if (lang == "python" || lang == "py") {
        if (!m_lexerPython) {
            m_lexerPython = new QsciLexerPython(this);
        }
        themePythonLexer(m_lexerPython);
        activeLexer = m_lexerPython;
    }
    else if (lang == "javascript" || lang == "js" || lang == "typescript" || lang == "ts") {
        if (!m_lexerJavaScript) {
            m_lexerJavaScript = new QsciLexerJavaScript(this);
        }
        themeJavaScriptLexer(m_lexerJavaScript);
        activeLexer = m_lexerJavaScript;
    }
    else if (lang == "java") {
        if (!m_lexerJava) {
            m_lexerJava = new QsciLexerJava(this);
        }
        themeJavaLexer(m_lexerJava);
        activeLexer = m_lexerJava;
    }
    else if (lang == "c") {
        if (!m_lexerC) {
            m_lexerC = new QsciLexerCPP(this);
        }
        themeCppLexer(m_lexerC);
        activeLexer = m_lexerC;
    }
    else if (lang == "json") {
        if (!m_lexerJson) {
            m_lexerJson = new QsciLexerJSON(this);
            // Theme JSON lexer
            m_lexerJson->setPaper(m_theme.background);
            m_lexerJson->setDefaultPaper(m_theme.background);
            m_lexerJson->setDefaultColor(m_theme.foreground);
            m_lexerJson->setColor(m_theme.keyword, QsciLexerJSON::Keyword);
            m_lexerJson->setColor(m_theme.string, QsciLexerJSON::String);
            m_lexerJson->setColor(m_theme.number, QsciLexerJSON::Number);
        }
        activeLexer = m_lexerJson;
    }
    else {
        // Default to C++ (also handles Go, Rust with keyword injection)
        if (!m_lexerCpp) {
            m_lexerCpp = new QsciLexerCPP(this);
        }
        themeCppLexer(m_lexerCpp);
        activeLexer = m_lexerCpp;
    }

    // Apply lexer
    applyLexer(activeLexer);

    // Inject custom keywords for Rust/Go
    if (lang == "rust" || lang == "rs") {
        const char* rustKeywords =
            "as async await break const continue crate dyn else enum extern "
            "false fn for if impl in let loop match mod move mut pub ref "
            "return self Self static struct super trait true type unsafe use "
            "where while";
        // SendScintilla(SCI_SETKEYWORDS, 0, rustKeywords);
        // SendScintilla(SCI_SETKEYWORDS,
        //               static_cast<unsigned long>(0),
        //               rustKeywords);
        setKeywordSet(0, rustKeywords);



    }
    else if (lang == "go") {
        const char* goKeywords =
            "break case chan const continue default defer else fallthrough for "
            "func go goto if import interface map package range return select "
            "struct switch type var true false nil iota";
        // SendScintilla(SCI_SETKEYWORDS, static_cast<unsigned long>(0), goKeywords);
        setKeywordSet(0, goKeywords);

    }

    // Setup autocomplete for this language
    setupAutocompletion();

    emit languageChanged(lang);
}

void CodeEditor::applyLexer(QsciLexer *lexer) {
    if (!lexer) return;

    // Disconnect any existing lexer
    QsciLexer *oldLexer = this->lexer();
    if (oldLexer && oldLexer != lexer) {
        // Don't delete, they're cached
    }

    // Apply the new lexer
    setLexer(lexer);

    // setBraceMatching(QsciScintilla::NoBraceMatch);
    // Lock background
    lockBackground();

    // Re-apply margin settings
    setMarginsForegroundColor(m_theme.marginForeground);
    setMarginsBackgroundColor(m_theme.marginBackground);

    m_idleProcessingTimer->start();

    setupColorBlockIndicators(this);
    // setUnmatchedBraceBackgroundColor(QColor("#c0fe2909"));
    // setMatchedBraceBackgroundColor(QColor("#c0266f9f"));






}

// ═══════════════════════════════════════════════════════════════════════════
// Lexer Theming
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::themeCppLexer(QsciLexerCPP *lexer) {
    if (!lexer) return;

    QFont codeFont(m_fontFamily, m_fontSize);
    codeFont.setStyleHint(QFont::Monospace);

    lexer->setDefaultFont(codeFont);
    lexer->setFont(codeFont);
    lexer->setPaper(m_theme.background);
    lexer->setDefaultPaper(m_theme.background);
    lexer->setDefaultColor(m_theme.foreground);

    // Apply colors to all styles
    lexer->setColor(m_theme.foreground, QsciLexerCPP::Default);
    lexer->setColor(m_theme.comment, QsciLexerCPP::Comment);
    lexer->setColor(m_theme.comment, QsciLexerCPP::CommentLine);
    lexer->setColor(m_theme.commentDoc, QsciLexerCPP::CommentDoc);
    lexer->setColor(m_theme.commentDoc, QsciLexerCPP::CommentLineDoc);
    lexer->setColor(m_theme.number, QsciLexerCPP::Number);
    lexer->setColor(m_theme.keyword, QsciLexerCPP::Keyword);
    lexer->setColor(m_theme.keywordSecondary, QsciLexerCPP::KeywordSet2);
    lexer->setColor(m_theme.string, QsciLexerCPP::DoubleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerCPP::SingleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerCPP::RawString);
    lexer->setColor(m_theme.stringEscape, QsciLexerCPP::EscapeSequence);
    lexer->setColor(m_theme.preprocessor, QsciLexerCPP::PreProcessor);
    lexer->setColor(m_theme.operator_, QsciLexerCPP::Operator);
    lexer->setColor(m_theme.identifier, QsciLexerCPP::Identifier);
    lexer->setColor(m_theme.error, QsciLexerCPP::UnclosedString);
    lexer->setColor(m_theme.function, QsciLexerCPP::GlobalClass);
    lexer->setColor(m_theme.type, QsciLexerCPP::UserLiteral);

    // Apply paper to all styles
    for (int i = 0; i <= QsciLexerCPP::UserLiteral; ++i) {
        lexer->setPaper(m_theme.background, i);
    }

    // Style fonts
    QFont italicFont = codeFont;
    italicFont.setItalic(true);
    lexer->setFont(italicFont, QsciLexerCPP::Comment);
    lexer->setFont(italicFont, QsciLexerCPP::CommentLine);
    lexer->setFont(italicFont, QsciLexerCPP::CommentDoc);

    QFont boldFont = codeFont;
    boldFont.setBold(true);
    lexer->setFont(boldFont, QsciLexerCPP::Keyword);
}

void CodeEditor::themePythonLexer(QsciLexerPython *lexer) {
    if (!lexer) return;

    QFont codeFont(m_fontFamily, m_fontSize);
    codeFont.setStyleHint(QFont::Monospace);

    lexer->setDefaultFont(codeFont);
    lexer->setFont(codeFont);
    lexer->setPaper(m_theme.background);
    lexer->setDefaultPaper(m_theme.background);
    lexer->setDefaultColor(m_theme.foreground);

    lexer->setColor(m_theme.foreground, QsciLexerPython::Default);
    lexer->setColor(m_theme.comment, QsciLexerPython::Comment);
    lexer->setColor(m_theme.comment, QsciLexerPython::CommentBlock);
    lexer->setColor(m_theme.number, QsciLexerPython::Number);
    lexer->setColor(m_theme.keyword, QsciLexerPython::Keyword);
    lexer->setColor(m_theme.string, QsciLexerPython::DoubleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerPython::SingleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerPython::TripleDoubleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerPython::TripleSingleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerPython::DoubleQuotedFString);
    lexer->setColor(m_theme.string, QsciLexerPython::SingleQuotedFString);
    lexer->setColor(m_theme.function, QsciLexerPython::FunctionMethodName);
    lexer->setColor(m_theme.className, QsciLexerPython::ClassName);
    lexer->setColor(m_theme.decorator, QsciLexerPython::Decorator);
    lexer->setColor(m_theme.identifier, QsciLexerPython::Identifier);
    lexer->setColor(m_theme.operator_, QsciLexerPython::Operator);
    lexer->setColor(m_theme.error, QsciLexerPython::UnclosedString);

    lexer->setV3BinaryOctalAllowed(true);
    lexer->setV3BytesAllowed(true);



    // Apply paper
    for (int i = 0; i <= QsciLexerPython::HighlightedIdentifier; ++i) {
        lexer->setPaper(m_theme.background, i);
    }

    // Italic comments
    QFont italicFont = codeFont;
    italicFont.setItalic(true);
    lexer->setFont(italicFont, QsciLexerPython::Comment);
    lexer->setFont(italicFont, QsciLexerPython::CommentBlock);

    // Bold keywords
    QFont boldFont = codeFont;
    boldFont.setBold(true);
    lexer->setFont(boldFont, QsciLexerPython::Keyword);
}

void CodeEditor::themeJavaScriptLexer(QsciLexerJavaScript *lexer) {
    if (!lexer) return;

    QFont codeFont(m_fontFamily, m_fontSize);
    codeFont.setStyleHint(QFont::Monospace);

    lexer->setDefaultFont(codeFont);
    lexer->setFont(codeFont);
    lexer->setPaper(m_theme.background);
    lexer->setDefaultPaper(m_theme.background);
    lexer->setDefaultColor(m_theme.foreground);

    lexer->setColor(m_theme.foreground, QsciLexerJavaScript::Default);
    lexer->setColor(m_theme.comment, QsciLexerJavaScript::Comment);
    lexer->setColor(m_theme.comment, QsciLexerJavaScript::CommentLine);
    lexer->setColor(m_theme.commentDoc, QsciLexerJavaScript::CommentDoc);
    lexer->setColor(m_theme.number, QsciLexerJavaScript::Number);
    lexer->setColor(m_theme.keyword, QsciLexerJavaScript::Keyword);
    lexer->setColor(m_theme.keywordSecondary, QsciLexerJavaScript::KeywordSet2);
    lexer->setColor(m_theme.string, QsciLexerJavaScript::DoubleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerJavaScript::SingleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerJavaScript::VerbatimString);
    lexer->setColor(m_theme.identifier, QsciLexerJavaScript::Identifier);
    lexer->setColor(m_theme.operator_, QsciLexerJavaScript::Operator);
    lexer->setColor(m_theme.error, QsciLexerJavaScript::UnclosedString);
    lexer->setColor(m_theme.constant, QsciLexerJavaScript::Regex);

    // Apply paper
    for (int i = 0; i <= 25; ++i) {  // JS lexer has ~25 styles
        lexer->setPaper(m_theme.background, i);
    }

    // Italic comments
    QFont italicFont = codeFont;
    italicFont.setItalic(true);
    lexer->setFont(italicFont, QsciLexerJavaScript::Comment);
    lexer->setFont(italicFont, QsciLexerJavaScript::CommentLine);

    // Bold keywords
    QFont boldFont = codeFont;
    boldFont.setBold(true);
    lexer->setFont(boldFont, QsciLexerJavaScript::Keyword);
}

void CodeEditor::themeJavaLexer(QsciLexerJava *lexer) {
    if (!lexer) return;

    QFont codeFont(m_fontFamily, m_fontSize);
    codeFont.setStyleHint(QFont::Monospace);

    lexer->setDefaultFont(codeFont);
    lexer->setFont(codeFont);
    lexer->setPaper(m_theme.background);
    lexer->setDefaultPaper(m_theme.background);
    lexer->setDefaultColor(m_theme.foreground);

    lexer->setColor(m_theme.foreground, QsciLexerJava::Default);
    lexer->setColor(m_theme.comment, QsciLexerJava::Comment);
    lexer->setColor(m_theme.comment, QsciLexerJava::CommentLine);
    lexer->setColor(m_theme.commentDoc, QsciLexerJava::CommentDoc);
    lexer->setColor(m_theme.number, QsciLexerJava::Number);
    lexer->setColor(m_theme.keyword, QsciLexerJava::Keyword);
    lexer->setColor(m_theme.keywordSecondary, QsciLexerJava::KeywordSet2);
    lexer->setColor(m_theme.string, QsciLexerJava::DoubleQuotedString);
    lexer->setColor(m_theme.string, QsciLexerJava::SingleQuotedString);
    lexer->setColor(m_theme.identifier, QsciLexerJava::Identifier);
    lexer->setColor(m_theme.operator_, QsciLexerJava::Operator);
    lexer->setColor(m_theme.error, QsciLexerJava::UnclosedString);

    // Apply paper
    for (int i = 0; i <= 25; ++i) {
        lexer->setPaper(m_theme.background, i);
    }

    // Italic comments
    QFont italicFont = codeFont;
    italicFont.setItalic(true);
    lexer->setFont(italicFont, QsciLexerJava::Comment);
    lexer->setFont(italicFont, QsciLexerJava::CommentLine);
    lexer->setFont(italicFont, QsciLexerJava::CommentDoc);

    // Bold keywords
    QFont boldFont = codeFont;
    boldFont.setBold(true);
    lexer->setFont(boldFont, QsciLexerJava::Keyword);
}

// ═══════════════════════════════════════════════════════════════════════════
// Theme Management
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::setTheme(const EditorTheme &theme) {
    m_theme = theme;
    applyTheme();
}

void CodeEditor::setTheme(const QString &themeName) {
    QString name = themeName.toLower();

    if (name == "dracula") {
        setTheme(EditorTheme::dracula());
    } else if (name == "monokai") {
        setTheme(EditorTheme::monokai());
    } else if (name == "github" || name == "github dark") {
        setTheme(EditorTheme::githubDark());
    } else if (name == "tokyo night" || name == "tokyonight") {
        setTheme(EditorTheme::tokyoNight());
    } else {
        setTheme(EditorTheme::oneDarkPro());
    }
}

void CodeEditor::applyTheme()
{
    // Base
    setPaper(m_theme.background);
    setColor(m_theme.foreground);

    // Caret
    setCaretForegroundColor(m_theme.caret);
    setCaretLineBackgroundColor(m_theme.lineHighlight);

    // Selection
    setSelectionBackgroundColor(m_theme.selection);
    setSelectionForegroundColor(m_theme.selectionForeground);

    // Margins
    setMarginsForegroundColor(m_theme.marginForeground);
    setMarginsBackgroundColor(m_theme.marginBackground);

    // Whitespace
    setWhitespaceForegroundColor(m_theme.invisibles);

    // Indentation guides
    setIndentationGuidesForegroundColor(m_theme.indentGuide);
    setIndentationGuidesBackgroundColor(m_theme.background);

    // Folding
    setFoldMarginColors(m_theme.foldMarginBackground,
                        m_theme.foldMarginBackground);

    // IMPORTANT: Re-theme lexer FIRST
    setLanguage(m_currentLanguage);



    lockBackground();

    setupBracketIndicators();

    highlightVisibleBrackets();

}

// ═══════════════════════════════════════════════════════════════════════════
// Editor Settings
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::setFontFamily(const QString &family) {
    m_fontFamily = family;
    QFont newFont(family, m_fontSize);
    newFont.setStyleHint(QFont::Monospace);
    setFont(newFont);

    // Re-apply to lexer
    setLanguage(m_currentLanguage);
}

void CodeEditor::setFontSize(int size) {
    if (size <= 0) size = 12;  // Prevent the QFont::setPointSize error
    m_fontSize = size;

    QFont newFont(m_fontFamily, size);
    newFont.setStyleHint(QFont::Monospace);
    setFont(newFont);

    // Re-apply to lexer
    setLanguage(m_currentLanguage);
}

void CodeEditor::setTabSize(int size) {
    m_tabSize = size;
    setIndentationWidth(size);
    setTabWidth(size);
}

void CodeEditor::setShowWhitespace(bool show) {
    setWhitespaceVisibility(show ? QsciScintilla::WsVisible : QsciScintilla::WsInvisible);
}

void CodeEditor::setShowIndentGuides(bool show) {
    setIndentationGuides(show);
}

void CodeEditor::setWordWrap(bool wrap) {
    setWrapMode(wrap ? QsciScintilla::WrapWord : QsciScintilla::WrapNone);
}

void CodeEditor::setMinimap(bool show) {
    // QScintilla doesn't have native minimap support
    // Would need custom implementation
    Q_UNUSED(show);
}

void CodeEditor::setAutoCompleteEnabled(bool enabled) {
    if (enabled) {
        setAutoCompletionSource(QsciScintilla::AcsAll);
        setAutoCompletionThreshold(2);
    } else {
        setAutoCompletionSource(QsciScintilla::AcsNone);
    }
}

void CodeEditor::setBraceMatchingEnabled(bool enabled) {
    setBraceMatching(enabled ? QsciScintilla::SloppyBraceMatch : QsciScintilla::NoBraceMatch);
}

void CodeEditor::setAutoIndentEnabled(bool enabled) {
    setAutoIndent(enabled);
}

// ═══════════════════════════════════════════════════════════════════════════
// Utilities
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::goToLine(int line) {
    setCursorPosition(line - 1, 0);
    ensureLineVisible(line - 1);
    setFocus();
}

void CodeEditor::duplicateLine() {
    int line, index;
    getCursorPosition(&line, &index);

    QString lineText = text(line);
    if (!lineText.endsWith('\n')) {
        lineText += '\n';
    }

    // Insert duplicate
    int lineStart = positionFromLineIndex(line + 1, 0);
    insertAt(lineText, line + 1, 0);

    // Move cursor to same position in new line
    setCursorPosition(line + 1, index);
}

void CodeEditor::deleteLine() {
    int line, index;
    getCursorPosition(&line, &index);

    setSelection(line, 0, line + 1, 0);
    removeSelectedText();
}

void CodeEditor::moveLineUp() {
    int line, index;
    getCursorPosition(&line, &index);

    if (line == 0) return;

    beginUndoAction();

    QString currentLine = text(line);
    QString aboveLine = text(line - 1);

    // Remove current line
    setSelection(line, 0, line + 1, 0);
    if (line == lines() - 1) {
        setSelection(line - 1, text(line - 1).length(), line, text(line).length());
    }
    removeSelectedText();

    // Insert above
    int insertLine = line - 1;
    if (insertLine < 0) insertLine = 0;

    QString insertText = currentLine;
    if (!insertText.endsWith('\n') && insertLine < lines() - 1) {
        insertText += '\n';
    }
    insertAt(insertText, insertLine, 0);

    endUndoAction();

    setCursorPosition(line - 1, index);
}

void CodeEditor::moveLineDown() {
    int line, index;
    getCursorPosition(&line, &index);

    if (line >= lines() - 1) return;

    beginUndoAction();

    QString currentLine = text(line);

    // Remove current line
    setSelection(line, 0, line + 1, 0);
    removeSelectedText();

    // Insert below
    int insertLine = line + 1;
    if (insertLine > lines()) insertLine = lines();

    QString insertText = currentLine;
    if (!insertText.endsWith('\n')) {
        insertText += '\n';
    }

    if (insertLine >= lines()) {
        append("\n" + currentLine.trimmed());
    } else {
        insertAt(insertText, insertLine, 0);
    }

    endUndoAction();

    setCursorPosition(line + 1, index);
}

void CodeEditor::toggleComment() {
    QString prefix = getCommentPrefix();
    if (prefix.isEmpty()) return;

    // just capture position of cursor on same line so after comment in/out the cursor remains at same position


    int lineFrom, lineTo, indexFrom, indexTo;
    getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);

    if (lineFrom == -1) {
        getCursorPosition(&lineFrom, &indexFrom);
        lineTo = lineFrom;
    }

    // If selection ends at column 0 of last line, don't include that line
    if (lineTo > lineFrom && indexTo == 0) {
        lineTo--;
    }

    beginUndoAction();

    // Check if all lines are already commented
    bool allCommented = true;
    for (int i = lineFrom; i <= lineTo; ++i) {
        QString lineText = text(i).trimmed();
        if (!lineText.isEmpty() && !lineText.startsWith(prefix)) {
            allCommented = false;
            break;
        }
    }

    for (int i = lineFrom; i <= lineTo; ++i) {
        QString lineText = text(i);

        if (allCommented) {
            // Uncomment
            int commentPos = lineText.indexOf(prefix);
            if (commentPos != -1) {
                setSelection(i, commentPos, i, commentPos + prefix.length());
                // Remove space after comment if exists
                if (commentPos + prefix.length() < lineText.length() &&
                    lineText[commentPos + prefix.length()] == ' ') {
                    setSelection(i, commentPos, i, commentPos + prefix.length() + 1);
                }
                removeSelectedText();
            }
        } else {
            // Comment
            if (!lineText.trimmed().isEmpty()) {
                // Find first non-whitespace
                int firstNonSpace = 0;
                while (firstNonSpace < lineText.length() && lineText[firstNonSpace].isSpace()) {
                    firstNonSpace++;
                }
                insertAt(prefix + " ", i, firstNonSpace);
            }
        }
    }
    endUndoAction();
}

QString CodeEditor::getCommentPrefix() const {
    static QMap<QString, QString> commentMap = {
        {"cpp", "//"},
        {"c", "//"},
        {"java", "//"},
        {"javascript", "//"},
        {"js", "//"},
        {"typescript", "//"},
        {"ts", "//"},
        {"rust", "//"},
        {"rs", "//"},
        {"go", "//"},
        {"python", "#"},
        {"py", "#"},
        {"ruby", "#"},
        {"bash", "#"},
        {"sh", "#"}
    };


    return commentMap.value(m_currentLanguage, "//");
}

// ═══════════════════════════════════════════════════════════════════════════
// Helpers
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::lockBackground() {
    long scBg = toScintillaColor(m_theme.background);
    long scFg = toScintillaColor(m_theme.foreground);

    // Lock BACKGROUND only across all styles — prevents white flicker
    // Do NOT set foreground here, it would overwrite lexer syntax colors
    for (int i = 0; i <= 255; ++i) {
        SendScintilla(SCI_STYLESETBACK, i, scBg);
    }

    // Set foreground only on the true default/fallback styles
    // These are the ones the lexer doesn't explicitly colorize
    SendScintilla(SCI_STYLESETBACK, STYLE_DEFAULT,    scBg);
    SendScintilla(SCI_STYLESETFORE, STYLE_DEFAULT,    scFg);

    SendScintilla(SCI_STYLESETBACK, STYLE_LINENUMBER, toScintillaColor(m_theme.marginBackground));
    SendScintilla(SCI_STYLESETFORE, STYLE_LINENUMBER, toScintillaColor(m_theme.marginForeground));

    // These fallback styles should also match the theme
    SendScintilla(SCI_STYLESETBACK, STYLE_BRACELIGHT, scBg);
    SendScintilla(SCI_STYLESETBACK, STYLE_BRACEBAD,   scBg);
    SendScintilla(SCI_STYLESETBACK, STYLE_INDENTGUIDE,scBg);
    SendScintilla(SCI_STYLESETBACK, STYLE_CALLTIP,    scBg);
}

long CodeEditor::toScintillaColor(const QColor &color) {
    return (color.blue() << 16) | (color.green() << 8) | color.red();
}

// ═══════════════════════════════════════════════════════════════════════════
// Event Handlers
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::keyPressEvent(QKeyEvent *event) {

    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Slash) {
        qDebug() << "CustomQsciEditor caught Ctrl+/ ! Emitting signal...";
        emit toggleCommentRequested(); // Emit our new signal
        event->accept();                   // Mark the event as handled
        return;                        // Stop processing
    }        


        QString text = event->text();
        if (text.isEmpty()) {
            QsciScintilla::keyPressEvent(event); // Handle non-character keys like Shift, Ctrl
            return;
        }

        QChar typedChar = text.at(0);

        // --- Case 1: User types an OPENING bracket ---
        if (bracketPairs.contains(typedChar)) {
            QChar closingChar = bracketPairs.value(typedChar);

            // Feature: Wrap selection
            if (hasSelectedText()) {
                QString selected = selectedText();
                replaceSelectedText(QString(typedChar) + selected + QString(closingChar));
                event->accept();
                return;
            }

            // --- THE CRITICAL FIX FOR CURSOR POSITION ---
            // 1. Get the current position BEFORE we insert anything.
            int currentPos = SendScintilla(SCI_GETCURRENTPOS);

            // 2. Insert both brackets. The cursor will move to the end.
            insert(QString(typedChar) + closingChar);

            // 3. Set the cursor's absolute position to be exactly in the middle.
            // This is far more reliable than relative movement.
            SendScintilla(SCI_GOTOPOS, currentPos + 1);
            // --- END FIX ---

            event->accept(); // We handled the event
            return;
        }

        // --- Case 2: User types a CLOSING bracket ---
        if (bracketPairs.key(typedChar) != Q_NULLPTR) {
            int currentPos = SendScintilla(SCI_GETCURRENTPOS);
            char nextChar = SendScintilla(SCI_GETCHARAT, currentPos);

            // Feature: Smart Overtype for closing brackets
            if (nextChar == typedChar.toLatin1()) {
                // The character to the right is what we just typed.
                // So, instead of inserting, just move the cursor past it.
                SendScintilla(SCI_CHARRIGHT);
                event->accept();
                return;
            }
        }

        // --- Case 3: Any other character ---
        // Let the base class handle all other keys (a, b, c, backspace, etc.)



    QsciScintilla::keyPressEvent(event);
}

void CodeEditor::focusInEvent(QFocusEvent *event) {
    QsciScintilla::focusInEvent(event);
    // Could emit a signal here if needed
}

void CodeEditor::wheelEvent(QWheelEvent *event) {
    // Ctrl+Wheel = zoom
    if (event->modifiers() & Qt::ControlModifier) {
        int delta = event->angleDelta().y();
        if (delta > 0) {
            zoomIn();
        } else if (delta < 0) {
            zoomOut();
        }
        event->accept();
        return;
    }

    QsciScintilla::wheelEvent(event);
}

// ═══════════════════════════════════════════════════════════════════════════
// Slots
// ═══════════════════════════════════════════════════════════════════════════

void CodeEditor::onCursorPositionChanged(int line, int index) {
    emit cursorPositionChanged(line + 1, index + 1);  // 1-based for UI
}

void CodeEditor::onTextChanged() {
    // Debounced color block update
    // m_colorBlockTimer->start();

    // --- ADD THIS LINE ---
    // Restart the idle timer to trigger rainbow brackets
    // and other analysis after the user stops typing for 400ms
    m_idleProcessingTimer->start();
}

void CodeEditor::updateColorBlocks() {
    static const QRegularExpression rx(R"(\{#([0-9A-Fa-f]{3,6})(?:,(\d+))?\})");
    const int docLen = length();

    // Clear existing indicators
    for (int id = IndicatorColorBlockStart; id <= IndicatorColorBlockEnd; ++id) {
        SendScintilla(SCI_SETINDICATORCURRENT, id);
        SendScintilla(SCI_INDICATORCLEARRANGE, 0, docLen);
    }

    const int totalLines = lines();
    int indicatorId = IndicatorColorBlockStart;

    for (int line = 0; line < totalLines && indicatorId <= IndicatorColorBlockEnd; ++line) {
        QString lineStr = text(line);
        int posComment = lineStr.indexOf("//");
        if (posComment < 0) {
            posComment = lineStr.indexOf("#");  // Python comments
        }
        if (posComment < 0) continue;

        QString comment = lineStr.mid(posComment);
        QRegularExpressionMatchIterator mit = rx.globalMatch(comment);

        while (mit.hasNext() && indicatorId <= IndicatorColorBlockEnd) {
            QRegularExpressionMatch m = mit.next();
            QString hex = m.captured(1);
            QString countStr = m.captured(2);

            // Expand 3-char hex to 6-char
            if (hex.length() == 3) {
                hex = QString("%1%1%2%2%3%3").arg(hex[0]).arg(hex[1]).arg(hex[2]);
            }
            QColor c("#" + hex);

            int matchCharInLine = posComment + m.capturedStart(0);
            int lineStartPos = SendScintilla(SCI_POSITIONFROMLINE, line);
            QByteArray prefixUtf8 = lineStr.left(matchCharInLine).toUtf8();
            int startPos = lineStartPos + prefixUtf8.size();

            int lastLine = line;
            if (!countStr.isEmpty()) {
                lastLine = qMin(line + countStr.toInt(), totalLines - 1);
            } else {
                for (int L = line + 1; L < totalLines; ++L) {
                    if (text(L).trimmed().isEmpty()) break;
                    lastLine = L;
                }
            }

            int endPos = (lastLine + 1 < totalLines)
                             ? SendScintilla(SCI_POSITIONFROMLINE, lastLine + 1)
                             : docLen;

            SendScintilla(SCI_SETINDICATORCURRENT, indicatorId);
            SendScintilla(SCI_INDICSETFORE, indicatorId, toScintillaColor(c));
            SendScintilla(SCI_INDICSETALPHA, indicatorId, 60);
            SendScintilla(SCI_INDICATORFILLRANGE, startPos, endPos - startPos);

            ++indicatorId;
        }
    }
}

void CodeEditor::setKeywordSet(int set, const char* words)
{
    SendScintilla(
        SCI_SETKEYWORDS,
        static_cast<unsigned long>(set),
        words
        );
}



// This is the new, robust implementation
// This is the final, robust implementation

// ============================================================================================================
                                            // RAINBOW BRACKET
// ============================================================================================================

// --- Bracket Coloring Functions (Unchanged, already good) ---
// void CodeEditor::setupBracketIndicators()
// {
//     auto toScintillaColor = [] (const QColor &c) { return (c.blue() << 16) | (c.green() << 8) | c.red(); };
//     for (int i = 0; i < MAX_BRACKET_COLORS; ++i) {
//         int indicatorId = INDICATOR_BRACKET_BASE + i;
//         SendScintilla(SCI_INDICSETSTYLE, indicatorId, IndicatorStyle::TextColorIndicator);
//         SendScintilla(SCI_INDICSETFORE, indicatorId, toScintillaColor(m_theme.bracketColors[i]));
//         SendScintilla(SCI_INDICSETALPHA, indicatorId, 255);
//         SendScintilla(SCI_INDICSETUNDER, indicatorId, false);
//     }
// }



// ---------------------------------------------
// Ignore < > for bracket matching
// ---------------------------------------------
// inline bool isRealBracket(char ch)
// {
//     return (ch == '(' || ch == ')' ||
//             ch == '{' || ch == '}' ||
//             ch == '[' || ch == ']');
// }

// void CodeEditor::highlightVisibleBrackets()
// {
//     // --- 1. Determine the visible range ---
//     const int firstVisibleLine = SendScintilla(SCI_GETFIRSTVISIBLELINE);
//     const int linesOnScreen = SendScintilla(SCI_LINESONSCREEN);
//     const int lastVisibleLine = firstVisibleLine + linesOnScreen;
//     const int startPos = SendScintilla(SCI_POSITIONFROMLINE, firstVisibleLine);
//     const int endPos = SendScintilla(SCI_POSITIONFROMLINE, lastVisibleLine + 1);
//     const int docLength = SendScintilla(SCI_GETTEXTLENGTH);
//     const int highlightEndPos = (endPos == -1) ? docLength : endPos;

//     // --- 2. Clear indicators ONLY from the visible area ---
//     for (int i = 0; i < MAX_BRACKET_COLORS; ++i) {
//         SendScintilla(SCI_SETINDICATORCURRENT, INDICATOR_BRACKET_BASE + i);
//         SendScintilla(SCI_INDICATORCLEARRANGE, startPos, highlightEndPos - startPos);
//     }

//     // --- 3. Parse the document, but only apply indicators to the visible part ---
//     QStack<int> nestingLevelStack;

//     // We must scan from the start to correctly calculate nesting levels.
//     // However, we avoid the slow memory copy of editor->text().
//     for (int pos = 0; pos < docLength; ++pos) {
//         char ch = SendScintilla(SCI_GETCHARAT, pos);

//         // Optimization: If we are past the visible area and have no open brackets, we can stop.
//         if (pos > highlightEndPos && nestingLevelStack.isEmpty()) {
//             break;
//         }

//         int styleId = SendScintilla(SCI_GETSTYLEAT, pos);
//         bool isAComment = (styleId == QsciLexerCPP::Comment || styleId == QsciLexerCPP::CommentLine);
//         bool isAString = (styleId == QsciLexerCPP::DoubleQuotedString || styleId == QsciLexerCPP::SingleQuotedString || styleId == QsciLexerCPP::UnclosedString);

//         if (isAComment || isAString) {
//             continue;
//         }

//         // if (ch == '(' || ch == '[' || ch == '{') {
//         // === Ignore < and > ===
//         if (!isRealBracket(ch))
//             continue;

//         if (ch == '(' || ch == '[' || ch == '{') {
//             int currentNesting = nestingLevelStack.isEmpty() ? 0 : nestingLevelStack.top() + 1;
//             nestingLevelStack.push(currentNesting);

//             // ONLY apply the indicator if the bracket is visible
//             if (pos >= startPos && pos < highlightEndPos) {
//                 int colorIndex = currentNesting % MAX_BRACKET_COLORS;
//                 SendScintilla(SCI_SETINDICATORCURRENT, INDICATOR_BRACKET_BASE + colorIndex);
//                 SendScintilla(SCI_INDICATORFILLRANGE, pos, 1);
//             }
//         }
//         else if (ch == ')' || ch == ']' || ch == '}') {
//             if (!nestingLevelStack.isEmpty()) {
//                 // To keep the logic simple, we don't check for matching pairs here,
//                 // as that can be complex. We just pop. A more advanced implementation
//                 // would track the character type.
//                 int currentNesting = nestingLevelStack.pop();

//                 // ONLY apply the indicator if the bracket is visible
//                 if (pos >= startPos && pos < highlightEndPos) {
//                     int colorIndex = currentNesting % MAX_BRACKET_COLORS;
//                     SendScintilla(SCI_SETINDICATORCURRENT, INDICATOR_BRACKET_BASE + colorIndex);
//                     SendScintilla(SCI_INDICATORFILLRANGE, pos, 1);
//                 }
//             }
//         }
//     }

//     setUnmatchedBraceBackgroundColor(QColor(m_theme.braceUnmatchBg));
//     setMatchedBraceBackgroundColor(QColor(m_theme.braceMatchBg));
// }

void CodeEditor::onIdleTimeout()
{
    // When the user has been idle, run ALL background tasks.
    highlightVisibleBrackets();

    // CRITICAL FIX: The slow text() copy now happens right before emitting the signal,
    // and only when we are sure we need to do the analysis.
}

void CodeEditor::scheduleIdleProcessing()
{
    // This is called on every text or scroll change. It just resets the single timer.
    m_idleProcessingTimer->start();
}

void CodeEditor::setupColorBlockIndicators(QsciScintilla* editor) {
    for (int id = 20; id <= 30; ++id) {
        editor->SendScintilla(QsciScintilla::SCI_INDICSETSTYLE, id, CustomQsciEditor::INDIC_ROUNDBOX);
        editor->SendScintilla(QsciScintilla::SCI_INDICSETALPHA, id, 100);
        editor->SendScintilla(QsciScintilla::SCI_INDICSETFORE, id, toScintillaColor(QColor("#ee882f"))); //88c0ff
        editor->SendScintilla(QsciScintilla::SCI_INDICSETUNDER, id, long(0));
    }
}






// ============================================================================================================
//                                      RAINBOW BRACKET - WINDOWS FIX
// ============================================================================================================

void CodeEditor::setupBracketIndicators()
{
    auto toScintillaColor = [](const QColor &c) {
        return (c.blue() << 16) | (c.green() << 8) | c.red();
    };

    for (int i = 0; i < MAX_BRACKET_COLORS; ++i) {
        int indicatorId = INDICATOR_BRACKET_BASE + i;

        // ══════════════════════════════════════════════════════════════
        // FIX FOR WINDOWS: Use FULLBOX instead of TextColorIndicator
        // FULLBOX draws BEHIND the text, so it survives re-styling
        // ══════════════════════════════════════════════════════════════
        SendScintilla(SCI_INDICSETSTYLE, indicatorId, INDIC_FULLBOX);
        SendScintilla(SCI_INDICSETFORE, indicatorId, toScintillaColor(m_theme.bracketColors[i]));
        SendScintilla(SCI_INDICSETALPHA, indicatorId, 80);     // Semi-transparent background
        SendScintilla(SCI_INDICSETOUTLINEALPHA, indicatorId, 255);  // Solid outline
        SendScintilla(SCI_INDICSETUNDER, indicatorId, true);   // Draw behind text
    }
}

// Remove this old function - not needed anymore
// void CodeEditor::setupIndicators_bracket() { ... }
// void CodeEditor::scheduleBracketHighlight() { ... }

inline bool isRealBracket(char ch)
{
    return (ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' ||
            ch == '[' || ch == ']');
}

void CodeEditor::highlightVisibleBrackets()
{
    // ═══════════════════════════════════════════════════════════════
    // 1. Determine visible range
    // ═══════════════════════════════════════════════════════════════
    const int firstVisibleLine = SendScintilla(SCI_GETFIRSTVISIBLELINE);
    const int linesOnScreen = SendScintilla(SCI_LINESONSCREEN);
    const int lastVisibleLine = firstVisibleLine + linesOnScreen;
    const int startPos = SendScintilla(SCI_POSITIONFROMLINE, firstVisibleLine);
    const int endPos = SendScintilla(SCI_POSITIONFROMLINE, lastVisibleLine + 1);
    const int docLength = SendScintilla(SCI_GETTEXTLENGTH);
    const int highlightEndPos = (endPos == -1) ? docLength : endPos;

    // ═══════════════════════════════════════════════════════════════
    // 2. Clear indicators ONLY in visible area
    // ═══════════════════════════════════════════════════════════════
    for (int i = 0; i < MAX_BRACKET_COLORS; ++i) {
        SendScintilla(SCI_SETINDICATORCURRENT, INDICATOR_BRACKET_BASE + i);
        SendScintilla(SCI_INDICATORCLEARRANGE, startPos, highlightEndPos - startPos);
    }

    // ═══════════════════════════════════════════════════════════════
    // 3. Parse and colorize brackets
    // ═══════════════════════════════════════════════════════════════
    QStack<int> nestingStack;

    for (int pos = 0; pos < docLength; ++pos) {
        // Early exit optimization
        if (pos > highlightEndPos && nestingStack.isEmpty()) {
            break;
        }

        char ch = SendScintilla(SCI_GETCHARAT, pos);

        if (!isRealBracket(ch)) {
            continue;
        }

        // Skip brackets in comments and strings
        int styleId = SendScintilla(SCI_GETSTYLEAT, pos);
        bool isComment = (styleId == QsciLexerCPP::Comment ||
                          styleId == QsciLexerCPP::CommentLine ||
                          styleId == QsciLexerCPP::CommentDoc);
        bool isString = (styleId == QsciLexerCPP::DoubleQuotedString ||
                         styleId == QsciLexerCPP::SingleQuotedString ||
                         styleId == QsciLexerCPP::RawString ||
                         styleId == QsciLexerCPP::UnclosedString);

        if (isComment || isString) {
            continue;
        }

        // Opening bracket
        if (ch == '(' || ch == '[' || ch == '{') {
            int level = nestingStack.isEmpty() ? 0 : nestingStack.top() + 1;
            nestingStack.push(level);

            // Apply color only if visible
            if (pos >= startPos && pos < highlightEndPos) {
                int colorIndex = level % MAX_BRACKET_COLORS;
                SendScintilla(SCI_SETINDICATORCURRENT, INDICATOR_BRACKET_BASE + colorIndex);
                SendScintilla(SCI_INDICATORFILLRANGE, pos, 1);
            }
        }
        // Closing bracket
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (!nestingStack.isEmpty()) {
                int level = nestingStack.pop();

                // Apply color only if visible
                if (pos >= startPos && pos < highlightEndPos) {
                    int colorIndex = level % MAX_BRACKET_COLORS;
                    SendScintilla(SCI_SETINDICATORCURRENT, INDICATOR_BRACKET_BASE + colorIndex);
                    SendScintilla(SCI_INDICATORFILLRANGE, pos, 1);
                }
            }
        }
    }

    // Brace matching background colors
    setUnmatchedBraceBackgroundColor(QColor(m_theme.braceUnmatchBg));
    setMatchedBraceBackgroundColor(QColor(m_theme.braceMatchBg));
}
