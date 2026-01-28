#pragma once
#include <QObject>

class JudgeBridge : public QObject {
    Q_OBJECT
public:
    explicit JudgeBridge(QObject *parent = nullptr);

public slots:
    void runCode(QString code, int testIndex);
    void requestTestCases();

signals:
    void sendTestCases(QString json);
    void judgeResult(QString json);
};
