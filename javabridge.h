#pragma once
#include <QObject>

class JavaBridge : public QObject {
    Q_OBJECT
public:
    explicit JavaBridge(QObject *parent = nullptr);

public slots:
    void runJava(const QString &code);
};
