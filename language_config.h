#ifndef LANGUAGE_CONFIG_H
#define LANGUAGE_CONFIG_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QMap>

struct LanguageConfig {
    // Identity
    QString id;
    QString name;
    QString version;

    // File Settings
    QString extension;
    QString sourceFile;

    // Compilation
    bool compiled = true;
    QString compileCommand;
    QStringList compileArgs;
    int compileTimeout = 30000;

    // Execution
    QString runCommand;
    QStringList runArgs;
    int timeout = 2000;
    int memoryLimitMB = 256;

    // Environment
    QMap<QString, QString> environment;

    // Optional
    QString codeTemplate;
    QString commentPrefix;

    // Metadata
    bool isBuiltin = false;
    bool isEmbedded = false;
    QString configPath;

    // Methods
    static LanguageConfig fromJson(const QJsonObject &json, const QString &id = "");
    QJsonObject toJson() const;
    bool isValid() const;
    QString expand(const QString &str, const QString &workDir) const;
    QStringList expandArgs(const QStringList &args, const QString &workDir) const;
    QString outputName() const;
};

#endif // LANGUAGE_CONFIG_H
