#ifndef PROBLEM_PANEL_H
#define PROBLEM_PANEL_H

#include <QWidget>
#include <QTextBrowser>
#include "networkbrowser.h"
#include <QJsonObject>
#include <QJsonArray>

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

    NetworkBrowser *markdownViewer;

    // Problem data
    QString problemId;
    QString problemTitle;
    QString difficulty;
    QString category;
    QStringList tags;
    QString currentFilePath;

    QJsonArray cachedTestCases;
};

#endif // PROBLEM_PANEL_H
