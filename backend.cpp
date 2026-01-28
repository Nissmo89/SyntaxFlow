#include "backend.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDateTime>
#include <QElapsedTimer>


#include <QCoreApplication>

Backend::Backend(QObject *parent) : QObject(parent) {}

QString Backend::getProblemsFilePath(const QString &problemId) {
    // Try multiple locations to find the data file
    QString relPath = "/data/problems/" + problemId + ".json";
    QStringList searchPaths = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QCoreApplication::applicationDirPath() + "/../..", // Typical Qt Creator build (build/Debug -> ../..)
        QCoreApplication::applicationDirPath() + "/../../.."
    };

    for (const QString &path : searchPaths) {
        QString fullPath = QDir(path).cleanPath(path + relPath);
        if (QFile::exists(fullPath)) {
            qDebug() << "✅ Found problems.json at:" << fullPath;
            return fullPath;
        }
    }
    
    qDebug() << "❌ Could not find problems.json in any search path.";
    return "";
}


bool Backend::writeToFile(const QString &path, const QString &content) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "❌ Failed to write file:" << path;
        return false;
    }
    file.write(content.toUtf8());
    file.close();
    return true;
}

void Backend::runJava(const QString &code, const QString &problemId) {
    // 1. Setup Environment
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString dirPath = tempPath + "/CodeHour_Java_" + QString::number(QDateTime::currentMSecsSinceEpoch());
    QDir().mkpath(dirPath);

    qDebug() << "📂 Working dir:" << dirPath;

    // 2. Write User Code
    // Assuming the user submits a class named 'Main'. 
    // In the future, we might want to parse the class name or wrap it.
    if (!writeToFile(dirPath + "/Main.java", code)) {
        emit output("System Error: Could not write Main.java");
        return;
    }

    // 3. Compile
    QProcess builder;
    builder.setWorkingDirectory(dirPath);
    builder.start("javac", {"Main.java"});
    builder.waitForFinished();

    if (builder.exitCode() != 0) {
        QString error = builder.readAllStandardError();
        qDebug() << "❌ Compile Error:" << error;
        QJsonObject result;
        result["status"] = "Compile Error";
        result["stdout"] = error;
        result["testIndex"] = -1; // Global error
        emit judgeResult(QJsonDocument(result).toJson());
        return; 
    }

    // 4. Load Test Cases
    QString problemsPath = getProblemsFilePath(problemId);
    QFile problemsFile(problemsPath);
    if (problemsPath.isEmpty() || !problemsFile.open(QIODevice::ReadOnly)) {
        qDebug() << "❌ Failed to load problems.json";
        emit output("System Error: Problems file not found.");
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(problemsFile.readAll());
    QJsonArray testCases = doc.object()["testCases"].toArray();
    problemsFile.close();

    // 5. Run Test Cases
    for (int i = 0; i < testCases.size(); ++i) {
        QJsonObject testCase = testCases[i].toObject();
        QJsonArray inputs = testCase["input"].toArray();
        QString expected = testCase["output"].toString();

        // Prepare input string (joined by newlines)
        QString inputString;
        for(const auto &arg : inputs) {
             inputString += arg.toString() + "\n";
        }

        QProcess runner;
        runner.setWorkingDirectory(dirPath);
        
        QElapsedTimer timer;
        timer.start();
        
        // Start the process
        runner.start("java", QStringList() << "-cp" << "." << "Main");
        if (!runner.waitForStarted()) {
             QJsonObject result;
             result["status"] = "Runtime Error";
             result["stdout"] = "Failed to start Java process.";
             result["testIndex"] = i;
             emit judgeResult(QJsonDocument(result).toJson());
             continue;
        }

        // Write to stdin
        if (!inputString.isEmpty()) {
            runner.write(inputString.toUtf8());
            runner.closeWriteChannel(); // Important: Close stdin so the program knows input ended
        }

        runner.waitForFinished(2000); // 2 sec timeout
        qint64 timeTaken = timer.elapsed();

        QJsonObject result;
        
        if (runner.state() == QProcess::Running) {
             runner.kill();
             result["status"] = "Time Limit Exceeded";
        } else if (runner.exitCode() != 0) {
             result["status"] = "Runtime Error";
             result["stdout"] = QString(runner.readAllStandardError());
        } else {
             QString actual = QString(runner.readAllStandardOutput()).trimmed();
             // Normalize expected output as well if needed
             if (actual == expected.trimmed()) {
                 result["status"] = "Accepted";
             } else {
                 result["status"] = "Wrong Answer";
             }
             result["stdout"] = actual;
             result["expected"] = expected;
        }
        
        result["time"] = QString::number(timeTaken) + "ms";
        result["testIndex"] = i;
        
        emit judgeResult(QJsonDocument(result).toJson());
    }

    // Cleanup
     QDir(dirPath).removeRecursively();
}

void Backend::requestTestCases(const QString &problemId) {
    QString problemsPath = getProblemsFilePath(problemId);
    QFile file(problemsPath);
    if (!problemsPath.isEmpty() && file.open(QIODevice::ReadOnly)) {
         QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
         emit sendTestCases(QJsonDocument(obj["testCases"].toArray()).toJson());
    } else {
        qDebug() << "❌ Failed to request test cases from:" << problemsPath;
    }
}

void Backend::runTestCase(const QString &code, int testIndex) {
    // Reuse runJava logic or implement single test run
    // TODO: Need to handle single test case execution more gracefully with ID
    runJava(code, "two_sum"); 
}
