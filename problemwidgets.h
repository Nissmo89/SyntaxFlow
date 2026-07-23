#ifndef PROBLEMWIDGETS_H
#define PROBLEMWIDGETS_H

#include "progressmanager.h"
#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class ProblemBrowserBridge : public QObject {
    Q_OBJECT
public:
    explicit ProblemBrowserBridge(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void openProblemRequested(const QString &path);
    void browserReady();

public slots:
    Q_INVOKABLE void openProblem(const QString &path) {
        qDebug() << ">>> ProblemBrowserBridge::openProblem CALLED WITH PATH:" << path;
        emit openProblemRequested(path);
    }

    Q_INVOKABLE void onBrowserReady() {
        qDebug() << ">>> ProblemBrowserBridge::onBrowserReady CALLED";
        emit browserReady();
    }
};

class ProblemBrowser : public QWidget {
    Q_OBJECT
public:
    explicit ProblemBrowser(ProgressManager *pm, QWidget *parent = nullptr);
    ~ProblemBrowser() override;

    void loadFromJson(const QString &filePath);

signals:
    void navigateToEditor(const QString &problemPath);

private slots:
    void onProgressChanged(const QString &problemId);
    void onBridgeBrowserReady();

private:
    void syncToWeb();
    QJsonObject getSolvedMap() const;

    QWebEngineView *m_view = nullptr;
    QWebChannel *m_channel = nullptr;
    ProblemBrowserBridge *m_bridge = nullptr;
    bool m_isWebReady = false;

    ProgressManager *progressManager = nullptr;
    QJsonArray m_problemsArray;
};

#endif // PROBLEMWIDGETS_H
