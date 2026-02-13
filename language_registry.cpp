#include "language_registry.h"
#include <QStandardPaths>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDirIterator>
#include <QProcess>
#include <QDebug>
#include <QCoreApplication>

LanguageRegistry::LanguageRegistry(QObject *parent) : QObject(parent) {
    m_userConfigPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
    + "/languages";
}

void LanguageRegistry::initialize() {
    qDebug() << "Initializing Language Registry...";

    loadBuiltinDefaults();

    // System paths
    QStringList systemPaths = {
        "/usr/share/codehour/languages",
        "/usr/local/share/codehour/languages",
        QCoreApplication::applicationDirPath() + "/languages"
    };
    for (const QString &path : systemPaths) {
        loadFromDirectory(path);
    }

    // User configs
    QDir().mkpath(m_userConfigPath);
    loadFromDirectory(m_userConfigPath);

    qDebug() << "Available languages:" << availableLanguages();
}

void LanguageRegistry::loadBuiltinDefaults() {
    auto addBuiltin = [this](const QJsonObject &json, const QString &id) {
        LanguageConfig config = LanguageConfig::fromJson(json, id);
        config.isBuiltin = true;
        m_languages[id] = config;
    };

    addBuiltin(builtinC(), "c");
    addBuiltin(builtinCpp(), "cpp");
    addBuiltin(builtinPython(), "python");
    addBuiltin(builtinJava(), "java");
    addBuiltin(builtinJavaScript(), "javascript");
    addBuiltin(builtinGo(), "go");
    addBuiltin(builtinRust(), "rust");
}

QJsonObject LanguageRegistry::builtinC() {
    return QJsonObject{
        {"name", "C"},
        {"extension", ".c"},
        {"sourceFile", "solution.c"},
        {"compiled", true},
        {"compileCommand", "gcc"},
        {"compileArgs", QJsonArray{
                            "-O2",
                            "-Wall",
                            "{source}",
                            "-o",
                            "{output}",
                            "-lm"
                        }},
        {"runCommand", "{workdir}/{output}"},
        {"timeout", 2000},
        {"template", "#include <stdio.h>\n\nint main() {\n    \n    return 0;\n}\n"}
    };
}

QJsonObject LanguageRegistry::builtinCpp() {
    return QJsonObject{
        {"name", "C++"},
        {"extension", ".cpp"},
        {"sourceFile", "solution.cpp"},
        {"compiled", true},
        {"compileCommand", "g++"},
        {"compileArgs", QJsonArray{"-std=c++17", "-O2", "-Wall", "-o", "{output}", "{source}"}},
        {"runCommand", "{workdir}/{output}"},
        {"timeout", 2000},
        {"template", "#include <bits/stdc++.h>\nusing namespace std;\n\nint main() {\n    \n    return 0;\n}\n"}
    };
}

QJsonObject LanguageRegistry::builtinPython() {
    return QJsonObject{
        {"name", "Python"},
        {"extension", ".py"},
        {"sourceFile", "solution.py"},
        {"compiled", false},
#ifdef Q_OS_WIN
        {"runCommand", "python"},
#else
        {"runCommand", "python3"},
#endif
        {"runArgs", QJsonArray{"-u", "{source}"}},
        {"timeout", 5000},
        {"template", "# Your code here\n"}
    };
}

QJsonObject LanguageRegistry::builtinJava() {
    return QJsonObject{
        {"name", "Java"},
        {"extension", ".java"},
        {"sourceFile", "Main.java"},
        {"compiled", true},
        {"compileCommand", "javac"},
        {"compileArgs", QJsonArray{"{source}"}},
        {"runCommand", "java"},
        {"runArgs", QJsonArray{"-cp", "{workdir}", "Main"}},
        {"timeout", 3000},
        {"template", "import java.util.*;\n\npublic class Main {\n    public static void main(String[] args) {\n        \n    }\n}\n"}
    };
}

QJsonObject LanguageRegistry::builtinJavaScript() {
    return QJsonObject{
        {"name", "JavaScript"},
        {"extension", ".js"},
        {"sourceFile", "solution.js"},
        {"compiled", false},
        {"runCommand", "node"},
        {"runArgs", QJsonArray{"{source}"}},
        {"timeout", 3000},
        {"template", "// Your code here\n"}
    };
}

QJsonObject LanguageRegistry::builtinGo() {
    return QJsonObject{
        {"name", "Go"},
        {"extension", ".go"},
        {"sourceFile", "solution.go"},
        {"compiled", true},
        {"compileCommand", "go"},
        {"compileArgs", QJsonArray{"build", "-o", "{output}", "{source}"}},
        {"runCommand", "{workdir}/{output}"},
        {"timeout", 2000},
        {"template", "package main\n\nimport \"fmt\"\n\nfunc main() {\n    \n}\n"}
    };
}

QJsonObject LanguageRegistry::builtinRust() {
    return QJsonObject{
        {"name", "Rust"},
        {"extension", ".rs"},
        {"sourceFile", "solution.rs"},
        {"compiled", true},
        {"compileCommand", "rustc"},
        {"compileArgs", QJsonArray{"-O", "-o", "{output}", "{source}"}},
        {"runCommand", "{workdir}/{output}"},
        {"timeout", 2000},
        {"template", "use std::io;\n\nfn main() {\n    \n}\n"}
    };
}

void LanguageRegistry::loadFromDirectory(const QString &path) {
    QDir dir(path);
    if (!dir.exists()) return;

    QDirIterator it(path, {"*.json"}, QDir::Files);
    while (it.hasNext()) {
        loadFromFile(it.next());
    }
}

void LanguageRegistry::loadFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON error in" << filePath << ":" << error.errorString();
        return;
    }

    QString id = QFileInfo(filePath).baseName();
    LanguageConfig config = LanguageConfig::fromJson(doc.object(), id);
    config.configPath = filePath;

    if (config.isValid()) {
        m_languages[config.id] = config;
        qDebug() << "Loaded language:" << config.name;
    }
}

LanguageConfig LanguageRegistry::getConfig(const QString &id) const {
    return m_languages.value(id, LanguageConfig{});
}

bool LanguageRegistry::hasLanguage(const QString &id) const {
    return m_languages.contains(id);
}

QStringList LanguageRegistry::allLanguages() const {
    return m_languages.keys();
}

QStringList LanguageRegistry::availableLanguages() const {
    QStringList available;
    for (auto it = m_languages.begin(); it != m_languages.end(); ++it) {
        if (isLanguageAvailable(it.key())) {
            available << it.key();
        }
    }
    return available;
}

bool LanguageRegistry::isLanguageAvailable(const QString &id) const {
    if (!m_languages.contains(id)) return false;

    const LanguageConfig &config = m_languages[id];
    if (config.isEmbedded) return true;

    QString cmd = config.compiled ? config.compileCommand : config.runCommand;
    return checkCommandExists(cmd);
}

bool LanguageRegistry::checkCommandExists(const QString &command) const {
    if (command.isEmpty()) return false;
    if (QFile::exists(command)) return true;

    QProcess proc;
#ifdef Q_OS_WIN
    proc.start("where", {command});
#else
    proc.start("which", {command});
#endif
    proc.waitForFinished(2000);
    return proc.exitCode() == 0;
}

bool LanguageRegistry::addLanguage(const LanguageConfig &config, bool save) {
    if (!config.isValid()) return false;

    m_languages[config.id] = config;

    if (save) {
        saveConfig(config);
    }

    emit languagesChanged();
    return true;
}

bool LanguageRegistry::saveConfig(const LanguageConfig &config) {
    QString filePath = m_userConfigPath + "/" + config.id + ".json";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QJsonDocument doc(config.toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool LanguageRegistry::removeLanguage(const QString &id) {
    if (!m_languages.contains(id)) return false;

    const LanguageConfig &config = m_languages[id];
    if (config.isBuiltin && config.configPath.isEmpty()) return false;

    if (!config.configPath.isEmpty()) {
        QFile::remove(config.configPath);
    }

    m_languages.remove(id);
    emit languagesChanged();
    return true;
}

QString LanguageRegistry::userConfigPath() const {
    return m_userConfigPath;
}

void LanguageRegistry::reload() {
    m_languages.clear();
    initialize();
    emit languagesChanged();
}
