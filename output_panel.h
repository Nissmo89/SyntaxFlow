#ifndef OUTPUT_PANEL_H
#define OUTPUT_PANEL_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

class OutputPanel : public QWidget
{
    Q_OBJECT

public:
    explicit OutputPanel(QWidget *parent = nullptr);

    // Output methods
    void appendOutput(const QString &text);
    void appendError(const QString &text);
    void appendInfo(const QString &text);
    void appendInput(const QString &text);
    void clear();

    // State
    void setRunning(bool running);
    bool isRunning() const { return m_isRunning; }

    // Input
    void setInputEnabled(bool enabled);
    void focusInput();

signals:
    void runRequested();
    void stopRequested();
    void inputSubmitted(const QString &input);
    void clearRequested();

private slots:
    void onInputSubmit();
    void onRunClicked();
    void onStopClicked();
    void updateElapsedTime();

private:
    void setupUI();
    void setupToolbar();
    void setupOutputArea();
    void setupInputArea();

    // UI Components
    QPlainTextEdit *m_outputArea = nullptr;
    QLineEdit *m_inputField = nullptr;
    QPushButton *m_runButton = nullptr;
    QPushButton *m_stopButton = nullptr;
    QPushButton *m_clearButton = nullptr;
    QLabel *m_statusLabel = nullptr;
    QLabel *m_timeLabel = nullptr;

    // State
    bool m_isRunning = false;
    QTimer *m_elapsedTimer = nullptr;
    qint64 m_startTime = 0;

    // Styling
    QString m_outputStyle;
    QString m_errorStyle;
    QString m_infoStyle;
    QString m_inputStyle;
};

#endif // OUTPUT_PANEL_H
