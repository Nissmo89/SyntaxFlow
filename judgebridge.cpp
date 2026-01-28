#include "judgebridge.h"

JudgeBridge::JudgeBridge(QObject *parent) : QObject(parent) {}

void JudgeBridge::requestTestCases() {
    QString json = R"([
        {"input":"nums=[2,7,11,15], target=9","output":"[0,1]"},
        {"input":"nums=[3,2,4], target=6","output":"[1,2]"}
    ])";
    emit sendTestCases(json);
}

void JudgeBridge::runCode(QString code, int testIndex) {
    // 1️⃣ Save code to temp Solution.java
    // 2️⃣ Compile using javac
    // 3️⃣ Run with input
    // 4️⃣ Compare output
    // 5️⃣ Send verdict

    QString result = R"({
        "status":"Accepted",
        "stdout":"[0,1]",
        "time":"12ms"
    })";

    emit judgeResult(result);
}
