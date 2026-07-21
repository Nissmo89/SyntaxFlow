#include "markdownrenderer.h"
#include "code_editor.h"
#include <QRegularExpression>
#include <QStringList>
#include <tree_sitter/api.h>
#include <vector>
#include <string>

extern "C" const TSLanguage *tree_sitter_cpp(void);

static const char *HIGHLIGHTS_SCM = R"(
[
  "break"
  "case"
  "class"
  "const"
  "continue"
  "default"
  "delete"
  "do"
  "else"
  "enum"
  "extern"
  "for"
  "goto"
  "if"
  "inline"
  "namespace"
  "new"
  "operator"
  "private"
  "protected"
  "public"
  "return"
  "sizeof"
  "static"
  "struct"
  "switch"
  "template"
  "this"
  "throw"
  "try"
  "typedef"
  "union"
  "using"
  "virtual"
  "void"
  "volatile"
  "while"
] @keyword

[
  "#define"
  "#elif"
  "#else"
  "#endif"
  "#if"
  "#ifdef"
  "#ifndef"
  "#include"
] @preproc

(number_literal) @number
(string_literal) @string
(system_lib_string) @string
(escape_sequence) @string.escape

(comment) @comment

(type_identifier) @type
(primitive_type) @type

(call_expression
  function: (identifier) @function)
(call_expression
  function: (field_expression
    field: (field_identifier) @function))

(function_declarator
  declarator: (identifier) @function)

(identifier) @variable
(field_identifier) @property
)";

static QString processInlineElements(QString text)
{
    // Inline code: `code`
    static const QRegularExpression inlineCodeRx(R"(`([^`]+)`)");
    text.replace(inlineCodeRx, "<code>\\1</code>");

    // Bold: **text**
    static const QRegularExpression boldRx(R"(\*\*([^*]+)\*\*)");
    text.replace(boldRx, "<b>\\1</b>");

    // Italics: *text*
    static const QRegularExpression italicRx(R"(\*([^*]+)\*)");
    text.replace(italicRx, "<i>\\1</i>");

    // Unescape markdown escaped characters
    text.replace(QLatin1String("\\_"), QLatin1String("_"));
    text.replace(QLatin1String("\\*"), QLatin1String("*"));
    text.replace(QLatin1String("\\`"), QLatin1String("`"));
    text.replace(QLatin1String("\\\\"), QLatin1String("\\"));

    return text;
}

static QString highlightCodeBlock(const QString &code, const QString &langName)
{
    Q_UNUSED(langName);
    if (code.isEmpty()) {
        return QLatin1String("<pre><code></code></pre>");
    }

    QByteArray utf8 = code.toUtf8();
    const char *sourceStr = utf8.constData();
    uint32_t sourceLen = utf8.length();

    TSParser *parser = ts_parser_new();
    if (!parser) {
        return QLatin1String("<pre><code>") + code.toHtmlEscaped() + QLatin1String("</code></pre>");
    }

    const TSLanguage *tsLang = tree_sitter_cpp();

    if (!tsLang || !ts_parser_set_language(parser, tsLang)) {
        ts_parser_delete(parser);
        return QLatin1String("<pre><code>") + code.toHtmlEscaped() + QLatin1String("</code></pre>");
    }

    TSTree *tree = ts_parser_parse_string(parser, nullptr, sourceStr, sourceLen);
    if (!tree) {
        ts_parser_delete(parser);
        return QLatin1String("<pre><code>") + code.toHtmlEscaped() + QLatin1String("</code></pre>");
    }

    uint32_t error_offset = 0;
    TSQueryError error_type = TSQueryErrorNone;
    TSQuery *query = ts_query_new(tsLang, HIGHLIGHTS_SCM, strlen(HIGHLIGHTS_SCM), &error_offset, &error_type);
    if (!query) {
        ts_tree_delete(tree);
        ts_parser_delete(parser);
        return QLatin1String("<pre><code>") + code.toHtmlEscaped() + QLatin1String("</code></pre>");
    }

    TSQueryCursor *cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, query, ts_tree_root_node(tree));

    // Vector to store the capture name for each byte in the UTF-8 source string
    std::vector<std::string> tokenTypes(sourceLen, std::string());

    TSQueryMatch match;
    uint32_t capture_index;
    while (ts_query_cursor_next_capture(cursor, &match, &capture_index)) {
        TSQueryCapture capture = match.captures[capture_index];
        uint32_t start_byte = ts_node_start_byte(capture.node);
        uint32_t end_byte = ts_node_end_byte(capture.node);
        uint32_t capture_id = capture.index;

        uint32_t name_len = 0;
        const char *name_ptr = ts_query_capture_name_for_id(query, capture_id, &name_len);
        if (!name_ptr) continue;
        std::string name(name_ptr, name_len);

        for (uint32_t i = start_byte; i < end_byte && i < sourceLen; ++i) {
            tokenTypes[i] = name;
        }
    }

    // Retrieve active theme mapping
    EditorTheme editorTheme = EditorTheme::tokyoNight();

    QString html = QLatin1String("<pre><code>");
    
    uint32_t runStart = 0;
    while (runStart < sourceLen) {
        std::string currentType = tokenTypes[runStart];
        uint32_t runEnd = runStart + 1;
        while (runEnd < sourceLen && tokenTypes[runEnd] == currentType) {
            runEnd++;
        }

        // Extract run slice and convert to QString
        QByteArray slice = utf8.mid(runStart, runEnd - runStart);
        QString text = QString::fromUtf8(slice);
        QString escaped = text.toHtmlEscaped();

        if (currentType.empty()) {
            html += escaped;
        } else {
            // Map the token type to style attributes
            QColor color = editorTheme.foreground;
            bool isBold = false;
            bool isItalic = false;

            if (currentType.rfind("keyword", 0) == 0) {
                color = editorTheme.keyword;
            } else if (currentType.rfind("preproc", 0) == 0) {
                color = editorTheme.preprocessor;
            } else if (currentType.rfind("operator", 0) == 0) {
                color = editorTheme.operator_;
            } else if (currentType.rfind("punctuation", 0) == 0) {
                color = editorTheme.foreground;
            } else if (currentType.rfind("string", 0) == 0) {
                if (currentType.find("escape") != std::string::npos) {
                    color = editorTheme.stringEscape;
                } else {
                    color = editorTheme.string;
                }
            } else if (currentType.rfind("comment", 0) == 0) {
                color = editorTheme.comment;
            } else if (currentType.rfind("number", 0) == 0) {
                color = editorTheme.number;
            } else if (currentType.rfind("boolean", 0) == 0) {
                color = editorTheme.number;
            } else if (currentType.rfind("constant", 0) == 0) {
                color = editorTheme.constant;
            } else if (currentType.rfind("variable", 0) == 0) {
                color = editorTheme.identifier;
            } else if (currentType.rfind("function", 0) == 0) {
                color = editorTheme.function;
            } else if (currentType.rfind("type", 0) == 0) {
                color = editorTheme.type;
            } else if (currentType.rfind("property", 0) == 0) {
                color = editorTheme.identifier;
            }

            if (!color.isValid()) {
                color = editorTheme.foreground;
            }

            QString styleString = QStringLiteral("color: %1;").arg(color.name(QColor::HexRgb));
            if (isBold) {
                styleString += QLatin1String(" font-weight: bold;");
            }
            if (isItalic) {
                styleString += QLatin1String(" font-style: italic;");
            }

            html += QStringLiteral("<span style=\"%1\">%2</span>").arg(styleString, escaped);
        }

        runStart = runEnd;
    }

    html += QLatin1String("</code></pre>");

    // Clean up
    ts_query_cursor_delete(cursor);
    ts_query_delete(query);
    ts_tree_delete(tree);
    ts_parser_delete(parser);

    return html;
}

QString MarkdownRenderer::toHtml(const QString &markdown)
{
    QString normalized = markdown;
    normalized.replace(QLatin1String("\r\n"), QLatin1String("\n"));
    normalized.replace('\r', '\n');
    QStringList lines = normalized.split('\n');

    QString html;
    bool inCodeBlock = false;
    QString codeBlockContent;
    QString codeBlockLanguage;

    for (int i = 0; i < lines.size(); ++i) {
        QString line = lines[i];
        
        // Handle code blocks
        if (line.trimmed().startsWith(QLatin1String("```"))) {
            if (inCodeBlock) {
                // End of code block
                html += highlightCodeBlock(codeBlockContent, codeBlockLanguage);
                codeBlockContent.clear();
                codeBlockLanguage.clear();
                inCodeBlock = false;
            } else {
                // Start of code block
                inCodeBlock = true;
                codeBlockLanguage = line.trimmed().mid(3).trimmed().toLower();
            }
            continue;
        }

        if (inCodeBlock) {
            if (!codeBlockContent.isEmpty()) {
                codeBlockContent += QLatin1String("\n");
            }
            codeBlockContent += line;
            continue;
        }

        // Process a normal line
        QString trimmed = line.trimmed();

        // Horizontal Rule
        if (trimmed == QLatin1String("---") || trimmed == QLatin1String("___") || trimmed == QLatin1String("***")) {
            html += QLatin1String("<hr>");
            continue;
        }

        // Image: ![alt](url)
        if (trimmed.startsWith(QLatin1String("![")) && trimmed.contains(QLatin1String("](")) && trimmed.endsWith(QLatin1String(")"))) {
            int altStart = trimmed.indexOf('[');
            int altEnd = trimmed.indexOf(']');
            int urlStart = trimmed.indexOf('(', altEnd);
            int urlEnd = trimmed.lastIndexOf(')');
            if (altStart != -1 && altEnd != -1 && urlStart != -1 && urlEnd != -1) {
                QString alt = trimmed.mid(altStart + 1, altEnd - altStart - 1);
                QString url = trimmed.mid(urlStart + 1, urlEnd - urlStart - 1);
                html += QStringLiteral("<p><img src=\"%1\" alt=\"%2\" width=\"100%\" /></p>").arg(url, alt);
                continue;
            }
        }

        // Headers
        if (trimmed.startsWith('#')) {
            int level = 0;
            while (level < trimmed.size() && trimmed.at(level) == '#') {
                level++;
            }
            if (level > 0 && level < trimmed.size() && trimmed.at(level).isSpace()) {
                QString headerText = trimmed.mid(level).trimmed().toHtmlEscaped();
                headerText = processInlineElements(headerText);
                html += QStringLiteral("<h%1>%2</h%1>").arg(qMin(level, 6)).arg(headerText);
                continue;
            }
        }

        // Bullet points / lists
        if (trimmed.startsWith(QLatin1String("- ")) || trimmed.startsWith(QLatin1String("* "))) {
            QString itemText = trimmed.mid(2).trimmed().toHtmlEscaped();
            itemText = processInlineElements(itemText);
            html += QStringLiteral("<ul><li>%1</li></ul>").arg(itemText);
            continue;
        }

        // Normal line
        QString processedLine = line.toHtmlEscaped();
        processedLine = processInlineElements(processedLine);
        
        if (!html.isEmpty() && 
            !html.endsWith(QLatin1String("<hr>")) && 
            !html.endsWith(QLatin1String("</ul>")) && 
            !html.endsWith(QLatin1String("</h3>")) && 
            !html.endsWith(QLatin1String("</h2>")) && 
            !html.endsWith(QLatin1String("</h1>")) && 
            !html.endsWith(QLatin1String("</pre>"))) {
            html += QLatin1String("<br>");
        }
        html += processedLine;
    }

    // Clean up if code block wasn't closed
    if (inCodeBlock) {
        html += highlightCodeBlock(codeBlockContent, codeBlockLanguage);
    }

    return html;
}
