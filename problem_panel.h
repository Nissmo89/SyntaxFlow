#ifndef PROBLEM_PANEL_H
#define PROBLEM_PANEL_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>

class ProblemPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ProblemPanel(QWidget *parent = nullptr);

    void loadFromJson(const QJsonObject &obj);
    bool loadFromFile(const QString &filePath);

    QString getProblemId() const { return problemId; }
    QString getTitle() const { return problemTitle; }
    QString getDifficulty() const { return difficulty; }
    QJsonArray getTestCases() const { return cachedTestCases; }

signals:
    void problemLoaded(const QString &problemId);
    void testCasesAvailable(const QJsonArray &testCases);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void buildUI();
    QString buildStyleSheet();
    QString buildHtmlWrapper(const QString &content, const QString &additionalCss = "");
    QString formatText(const QString &text);

    QScrollArea *scrollArea;
    QWidget *contentWidget;

    // Header
    QLabel *titleLabel;
    QLabel *idLabel;
    QLabel *difficultyBadge;
    QLabel *categoryLabel;
    QWidget *tagsContainer;

    // Content sections
    QLabel *descriptionLabel;
    QLabel *taskLabel;
    QLabel *inputFormatLabel;
    QLabel *outputFormatLabel;
    QWidget *constraintsWidget;

    // Sample I/O
    QLabel *sampleInputLabel;
    QLabel *sampleOutputLabel;
    QLabel *sampleExplanationLabel;

    // Hints
    QWidget *hintsWidget;
    QWidget *hintsContent;
    QPushButton *hintsToggle;

    // Problem data
    QString problemId;
    QString problemTitle;
    QString difficulty;
    QString category;
    QStringList tags;

    QJsonArray cachedTestCases;
};

#endif // PROBLEM_PANEL_H
