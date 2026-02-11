#include "language_config.h"
#include <QJsonArray>
#include <QFileInfo>

LanguageConfig LanguageConfig::fromJson(const QJsonObject &json, const QString &id) {
    LanguageConfig config;

    config.id = json.value("id").toString(id);
    config.name = json.value("name").toString(config.id);
    config.version = json.value("version").toString();

    config.extension = json.value("extension").toString();
    config.sourceFile = json.value("sourceFile").toString();

    if (config.sourceFile.isEmpty() && !config.extension.isEmpty()) {
        config.sourceFile = "solution" + config.extension;
    }

    config.compiled = json.value("compiled").toBool(true);
    config.compileCommand = json.value("compileCommand").toString();

    for (const auto &arg : json.value("compileArgs").toArray()) {
        config.compileArgs << arg.toString();
    }

    config.compileTimeout = json.value("compileTimeout").toInt(30000);
    config.runCommand = json.value("runCommand").toString();

    for (const auto &arg : json.value("runArgs").toArray()) {
        config.runArgs << arg.toString();
    }

    config.timeout = json.value("timeout").toInt(2000);
    config.memoryLimitMB = json.value("memoryLimit").toInt(256);

    QJsonObject envObj = json.value("environment").toObject();
    for (auto it = envObj.begin(); it != envObj.end(); ++it) {
        config.environment[it.key()] = it.value().toString();
    }

    config.codeTemplate = json.value("template").toString();
    config.commentPrefix = json.value("commentPrefix").toString("//");

    return config;
}

QJsonObject LanguageConfig::toJson() const {
    QJsonObject json;

    json["id"] = id;
    json["name"] = name;
    if (!version.isEmpty()) json["version"] = version;

    json["extension"] = extension;
    json["sourceFile"] = sourceFile;
    json["compiled"] = compiled;

    if (!compileCommand.isEmpty()) json["compileCommand"] = compileCommand;
    if (!compileArgs.isEmpty()) json["compileArgs"] = QJsonArray::fromStringList(compileArgs);
    if (compileTimeout != 30000) json["compileTimeout"] = compileTimeout;

    json["runCommand"] = runCommand;
    if (!runArgs.isEmpty()) json["runArgs"] = QJsonArray::fromStringList(runArgs);
    json["timeout"] = timeout;
    json["memoryLimit"] = memoryLimitMB;

    if (!codeTemplate.isEmpty()) json["template"] = codeTemplate;
    json["commentPrefix"] = commentPrefix;

    return json;
}

bool LanguageConfig::isValid() const {
    if (id.isEmpty() || extension.isEmpty()) return false;
    if (!isEmbedded) {
        if (compiled && compileCommand.isEmpty()) return false;
        if (runCommand.isEmpty()) return false;
    }
    return true;
}

QString LanguageConfig::outputName() const {
    QString base = QFileInfo(sourceFile).baseName();
#ifdef Q_OS_WIN
    if (compiled) base += ".exe";
#endif
    return base;
}

QString LanguageConfig::expand(const QString &str, const QString &workDir) const {
    QString result = str;

    result.replace("{source}", sourceFile);
    result.replace("{output}", outputName());
    result.replace("{workdir}", workDir);
    result.replace("{filename}", QFileInfo(sourceFile).baseName());

#ifdef Q_OS_WIN
    result.replace("{sep}", "\\");
    result.replace("{pathsep}", ";");
#else
    result.replace("{sep}", "/");
    result.replace("{pathsep}", ":");
#endif

    return result;
}

QStringList LanguageConfig::expandArgs(const QStringList &args, const QString &workDir) const {
    QStringList result;
    for (const QString &arg : args) {
        result << expand(arg, workDir);
    }
    return result;
}
