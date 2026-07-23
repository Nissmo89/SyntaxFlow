#ifndef PROBLEM_PANEL_H
#define PROBLEM_PANEL_H

#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QJsonObject>
#include <QJsonArray>

class ProblemPanelBridge : public QObject {
    Q_OBJECT
public:
    explicit ProblemPanelBridge(QObject *parent = nullptr) : QObject(parent) {}
};

class ProblemPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ProblemPanel(QWidget *parent = nullptr);
    ~ProblemPanel() override;

    void loadFromJson(const QJsonObject &obj);
    bool loadFromFile(const QString &filePath);

    QString getProblemId() const { return problemId; }
    QString getTitle() const { return problemTitle; }
    QString getDifficulty() const { return difficulty; }
    QJsonArray getTestCases() const { return cachedTestCases; }

signals:
    void problemLoaded(const QString &problemId);
    void testCasesAvailable(const QJsonArray &testCases);

private slots:
    void onWebReady();

private:
    void buildUI();
    void syncToWeb();

    QWebEngineView *m_view = nullptr;
    QWebChannel *m_channel = nullptr;
    ProblemPanelBridge *m_bridge = nullptr;
    bool m_isWebReady = false;

    // Problem data
    QString problemId;
    QString problemTitle;
    QString difficulty;
    QString category;
    QStringList tags;
    QString currentFilePath;
    QString rawMarkdown;

    QJsonArray cachedTestCases;
};

#endif // PROBLEM_PANEL_H
