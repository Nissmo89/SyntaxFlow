#include "javabridge.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QProcess>

JavaBridge::JavaBridge(QObject *parent) : QObject(parent) {}

void JavaBridge::runJava(const QString &code) {
    QString dir = QDir::tempPath() + "/java_runner";
    QDir().mkpath(dir);

    QFile file(dir + "/Main.java");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(code.toUtf8());
    file.close();

    qDebug() << "📄 Java file written:" << dir + "/Main.java";

    QProcess *compile = new QProcess(this);
    compile->setWorkingDirectory(dir);

    connect(compile, &QProcess::readyReadStandardError, this, [=]() {
        qDebug() << "❌ javac:" << compile->readAllStandardError();
    });

    connect(compile,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [=](int exitCode, QProcess::ExitStatus) {

                if (exitCode != 0) {
                    qDebug() << "❌ Compilation failed";
                    return;
                }

                qDebug() << "✅ Compilation successful";

                QProcess *run = new QProcess(this);
                run->setWorkingDirectory(dir);

                connect(run, &QProcess::readyReadStandardOutput, this, [=]() {
                    qDebug() << "▶️ java:" << run->readAllStandardOutput();
                });

                connect(run, &QProcess::readyReadStandardError, this, [=]() {
                    qDebug() << "❌ runtime:" << run->readAllStandardError();
                });

                run->start("java", {"-cp", ".", "Main"});
            });

    compile->start("javac", {"-d", ".", "Main.java"});
}



