#include "backend.h"
#include "language_registry.h"
#include "code_runner.h"

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

Backend::Backend(QObject *parent) : QObject(parent) {
    m_registry = new LanguageRegistry(this);
    m_runner = new CodeRunner(m_registry, this);

    m_registry->initialize();

    // Connect runner signals
    connect(m_runner, &CodeRunner::testResult, this, &Backend::testResult);
    connect(m_runner, &CodeRunner::compilationError, this, &Backend::compilationError);
    connect(m_runner, &CodeRunner::systemError, this, &Backend::systemError);
    connect(m_runner, &CodeRunner::started, this, &Backend::executionStarted);
    connect(m_runner, &CodeRunner::finished, this, &Backend::executionFinished);
    connect(m_runner, &CodeRunner::progress, this, &Backend::progress);

    connect(m_registry, &LanguageRegistry::languagesChanged, this, &Backend::languagesChanged);

    qDebug() << "Backend initialized. Available:" << availableLanguages();
}

Backend::~Backend() = default;

QStringList Backend::availableLanguages() const {
    return m_registry->availableLanguages();
}

QStringList Backend::allLanguages() const {
    return m_registry->allLanguages();
}

LanguageConfig Backend::getLanguageConfig(const QString &id) const {
    return m_registry->getConfig(id);
}

QString Backend::getTemplate(const QString &languageId) const {
    return m_registry->getConfig(languageId).codeTemplate;
}

bool Backend::isLanguageAvailable(const QString &id) const {
    return m_registry->isLanguageAvailable(id);
}

bool Backend::isRunning() const {
    return m_runner->isRunning();
}

void Backend::runCode(const QString &code, const QString &languageId, const QString &problemId) {
    m_runner->runCode(code, languageId, problemId);
}

void Backend::runTestCase(const QString &code, const QString &languageId,
                          int testIndex, const QString &problemId) {
    m_runner->runSingleTest(code, languageId, testIndex, problemId);
}

void Backend::stopExecution() {
    m_runner->stop();
}

void Backend::requestTestCases(const QString &problemId) {
    QString relPath = "/data/problems/" + problemId + ".json";
    QStringList paths = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath(),
        QCoreApplication::applicationDirPath() + "/../..",
        QCoreApplication::applicationDirPath() + "/../../.."
    };

    for (const QString &p : paths) {
        QString full = QDir(p).cleanPath(p + relPath);
        QFile file(full);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            emit testCasesReady(doc.object()["testCases"].toArray());
            return;
        }
    }

    emit systemError("Could not load test cases for: " + problemId);
}

bool Backend::addLanguage(const LanguageConfig &config) {
    return m_registry->addLanguage(config, true);
}

bool Backend::removeLanguage(const QString &id) {
    return m_registry->removeLanguage(id);
}

void Backend::reloadLanguages() {
    m_registry->reload();
}

void Backend::openConfigDirectory() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_registry->userConfigPath()));
}
