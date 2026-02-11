#ifndef LANGUAGE_REGISTRY_H
#define LANGUAGE_REGISTRY_H

#include <QObject>
#include <QMap>
#include "language_config.h"

class LanguageRegistry : public QObject {
    Q_OBJECT

public:
    explicit LanguageRegistry(QObject *parent = nullptr);

    void initialize();

    LanguageConfig getConfig(const QString &id) const;
    bool hasLanguage(const QString &id) const;
    QStringList allLanguages() const;
    QStringList availableLanguages() const;

    bool isLanguageAvailable(const QString &id) const;
    bool addLanguage(const LanguageConfig &config, bool save = false);
    bool removeLanguage(const QString &id);

    QString userConfigPath() const;
    void reload();

signals:
    void languagesChanged();

private:
    QMap<QString, LanguageConfig> m_languages;
    QString m_userConfigPath;

    void loadBuiltinDefaults();
    void loadFromDirectory(const QString &path);
    void loadFromFile(const QString &filePath);
    bool saveConfig(const LanguageConfig &config);
    bool checkCommandExists(const QString &command) const;

    // Built-in configs
    static QJsonObject builtinC();
    static QJsonObject builtinCpp();
    static QJsonObject builtinPython();
    static QJsonObject builtinJava();
    static QJsonObject builtinJavaScript();
    static QJsonObject builtinGo();
    static QJsonObject builtinRust();
};

#endif // LANGUAGE_REGISTRY_H
