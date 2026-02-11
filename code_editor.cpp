#include "code_editor.h"
#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged,
            this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest,
            this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged,
            this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    // Set tab stop width
    setTabWidth(4);

    // ═══════════════════════════════════════════════════════════════
    // MODERN DARK THEME STYLESHEET
    // ═══════════════════════════════════════════════════════════════
    setStyleSheet(R"(
        CodeEditor {
            background-color: #1e1e2e;
            color: #cdd6f4;
            border: none;
            selection-background-color: #45475a;
            selection-color: #cdd6f4;
        }

        QScrollBar:vertical {
            background: #1e1e2e;
            width: 12px;
            margin: 0;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background: #45475a;
            min-height: 30px;
            border-radius: 6px;
            margin: 2px;
        }

        QScrollBar::handle:vertical:hover {
            background: #585b70;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
        }

        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
        }

        QScrollBar:horizontal {
            background: #1e1e2e;
            height: 12px;
            margin: 0;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal {
            background: #45475a;
            min-width: 30px;
            border-radius: 6px;
            margin: 2px;
        }

        QScrollBar::handle:horizontal:hover {
            background: #585b70;
        }

        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal {
            width: 0;
        }
    )");

    // Set monospace font
    QFont font;
    font.setFamily("JetBrains Mono");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(11);
    setFont(font);

    // Cursor settings
    setCursorWidth(2);
}

int CodeEditor::lineNumberAreaWidth()
{
    if (!showLineNumbers) return 0;

    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    // Minimum 3 digits width + padding
    digits = qMax(3, digits);
    int space = 20 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                      lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    if (!highlightCurrentLineEnabled) return;

    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        // Current line highlight color
        QColor lineColor = QColor("#2a2a3d");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);

    // Line number area background
    painter.fillRect(event->rect(), QColor("#181825"));

    // Draw subtle border on the right
    painter.setPen(QColor("#313244"));
    painter.drawLine(event->rect().topRight(), event->rect().bottomRight());

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    // Current line number
    int currentLine = textCursor().blockNumber();

    // Font for line numbers
    QFont lineFont = font();
    lineFont.setPointSize(font().pointSize() - 1);
    painter.setFont(lineFont);

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);

            // Highlight current line number
            if (blockNumber == currentLine) {
                painter.setPen(QColor("#cdd6f4"));  // Bright for current line

                // Draw highlight background for current line number
                QRect highlightRect(0, top, lineNumberArea->width() - 2, fontMetrics().height());
                painter.fillRect(highlightRect, QColor("#2a2a3d"));
            } else {
                painter.setPen(QColor("#6c7086"));  // Muted for other lines
            }

            painter.drawText(0, top, lineNumberArea->width() - 10,
                             fontMetrics().height(),
                             Qt::AlignRight | Qt::AlignVCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    // Smart tab handling - insert spaces instead of tab
    if (event->key() == Qt::Key_Tab) {
        insertPlainText(QString(tabSpaces, ' '));
        return;
    }

    // Auto-indent on Enter
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        QTextCursor cursor = textCursor();
        QString currentLine = cursor.block().text();

        // Count leading whitespace
        int indent = 0;
        for (QChar c : currentLine) {
            if (c == ' ') indent++;
            else if (c == '\t') indent += tabSpaces;
            else break;
        }

        // Check if we need extra indent (after { or :)
        QString trimmed = currentLine.trimmed();
        if (trimmed.endsWith('{') || trimmed.endsWith(':')) {
            indent += tabSpaces;
        }

        QPlainTextEdit::keyPressEvent(event);
        insertPlainText(QString(indent, ' '));
        return;
    }

    // Auto-close brackets
    if (event->text() == "{") {
        insertPlainText("{}");
        moveCursor(QTextCursor::Left);
        return;
    }
    if (event->text() == "(") {
        insertPlainText("()");
        moveCursor(QTextCursor::Left);
        return;
    }
    if (event->text() == "[") {
        insertPlainText("[]");
        moveCursor(QTextCursor::Left);
        return;
    }
    if (event->text() == "\"") {
        insertPlainText("\"\"");
        moveCursor(QTextCursor::Left);
        return;
    }
    if (event->text() == "'") {
        insertPlainText("''");
        moveCursor(QTextCursor::Left);
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
}

void CodeEditor::setLineNumberVisible(bool visible)
{
    showLineNumbers = visible;
    lineNumberArea->setVisible(visible);
    updateLineNumberAreaWidth(0);
}

void CodeEditor::setCurrentLineHighlight(bool enabled)
{
    highlightCurrentLineEnabled = enabled;
    highlightCurrentLine();
}

void CodeEditor::setTabWidth(int spaces)
{
    tabSpaces = spaces;
    QFontMetrics fm(font());
    setTabStopDistance(fm.horizontalAdvance(' ') * spaces);
}
