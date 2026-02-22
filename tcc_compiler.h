#pragma once
#include <QObject>
#include <QString>
#include <functional>

class Compiler : public QObject
{
    Q_OBJECT
public:
    explicit Compiler(QObject *parent = nullptr);
    ~Compiler() = default;

    bool compileAndRun(const QString &code,
                       std::function<void(const QString &)> outputCallback,
                       std::function<void(const QString &)> errorCallback);

signals:
    void finished();
};
