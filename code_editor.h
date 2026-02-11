#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QPainter>
#include <QTextBlock>

class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setLineNumberVisible(bool visible);
    void setCurrentLineHighlight(bool enabled);
    void setTabWidth(int spaces);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    LineNumberArea *lineNumberArea;
    bool showLineNumbers = true;
    bool highlightCurrentLineEnabled = true;
    int tabSpaces = 4;
};

// ═══════════════════════════════════════════════════════════════
// LINE NUMBER AREA WIDGET
// ═══════════════════════════════════════════════════════════════

class LineNumberArea : public QWidget
{
public:
    explicit LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODE_EDITOR_H
