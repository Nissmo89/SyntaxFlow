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
    addBuiltin(builtinJavaScript(), "javascript");
}

QJsonObject LanguageRegistry::builtinC() {
    return QJsonObject{
        {"name", "C"},
        {"extension", ".c"},
        {"sourceFile", "solution.c"},
        {"isEmbedded", true},
        {"compiled", false},
        {"timeout", 2000},
        {"template", "#include <stdio.h>\n\nint main() {\n    \n    return 0;\n}\n"}
    };
}

QJsonObject LanguageRegistry::builtinCpp() {
    return QJsonObject{
        {"name", "C++"},
        {"extension", ".cpp"},
        {"sourceFile", "solution.cpp"},
        {"isEmbedded", false},
        {"compiled", true},
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
#ifdef SF_PYTHON_ENABLED
        {"isEmbedded", true},
#else
        {"isEmbedded", false},
#endif
        {"timeout", 5000},
        {"template", "# Your code here\n"}
    };
}


QJsonObject LanguageRegistry::builtinJavaScript() {
    return QJsonObject{
        {"name", "JavaScript"},
        {"extension", ".js"},
        {"sourceFile", "solution.js"},
        {"isEmbedded", true},
        {"compiled", false},
        {"timeout", 3000},
        {"template", "// Your code here\n"}
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

    // We only support embedded compilers now.
    return m_languages[id].isEmbedded;
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
