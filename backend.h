#pragma once
#include <QObject>
#include <QProcess>

class Backend : public QObject {
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

public slots:

    Q_INVOKABLE void runJava(const QString &code, const QString &problemId);
    void runTestCase(const QString &code, int testIndex); // Keeping as is for now or update if needed
    void requestTestCases(const QString &problemId);

signals:
    void output(const QString &text);
    void sendTestCases(const QString &json);
    void judgeResult(const QString &json);

private:
    QString getProblemsFilePath(const QString &problemId);
    bool writeToFile(const QString &path, const QString &content);
};
