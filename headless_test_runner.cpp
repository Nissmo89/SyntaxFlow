#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include "code_runner.h"
#include "language_registry.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <problem_id_or_path> <language_id> <code_file_path>\n";
        return 1;
    }
    
    QCoreApplication app(argc, argv);
    
    QString problemPath = argv[1];
    QString languageId = argv[2];
    QString codePath = argv[3];
    
    QFile file(codePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Failed to open code file: " << codePath.toStdString() << "\n";
        return 1;
    }
    QString code = file.readAll();
    file.close();
    
    LanguageRegistry registry;
    CodeRunner runner(&registry);
    
    bool hasError = false;
    
    QObject::connect(&runner, &CodeRunner::testResult, [](int testIndex, const QString &status, const QString &output, const QString &expected, qint64 timeMs) {
        std::cout << "TEST_RESULT|" << testIndex << "|" << status.toStdString() << "|" << timeMs << "\n";
        if (status != "Accepted") {
            std::cout << "OUTPUT|" << output.toStdString() << "\nEXPECTED|" << expected.toStdString() << "\n";
        }
    });
    
    QObject::connect(&runner, &CodeRunner::compilationError, [&](const QString &error) {
        std::cout << "COMPILATION_ERROR|" << error.toStdString() << "\n";
        hasError = true;
    });
    
    QObject::connect(&runner, &CodeRunner::systemError, [&](const QString &error) {
        std::cout << "SYSTEM_ERROR|" << error.toStdString() << "\n";
        hasError = true;
    });
    
    QObject::connect(&runner, &CodeRunner::finished, [&]() {
        app.quit();
    });
    
    runner.runCode(code, languageId, problemPath);
    
    int exitCode = app.exec();
    return hasError ? 1 : exitCode;
}
