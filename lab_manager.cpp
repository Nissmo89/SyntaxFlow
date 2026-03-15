#include "lab_manager.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QUuid>
#include <QDebug>

// ═══════════════════════════════════════════════════════════════════════════
// CodeLab Implementation
// ═══════════════════════════════════════════════════════════════════════════

QJsonObject CodeLab::toJson() const
{
    return QJsonObject{
        {"id", id},
        {"title", title},
        {"language", language},
        {"code", code},
        {"created", created.toString(Qt::ISODate)},
        {"modified", modified.toString(Qt::ISODate)},
        {"description", description},
        {"isStarred", isStarred},
        {"tags", QJsonArray::fromStringList(tags)}
    };
}

CodeLab CodeLab::fromJson(const QJsonObject &obj)
{
    CodeLab lab;
    lab.id = obj["id"].toString();
    lab.title = obj["title"].toString();
    lab.language = obj["language"].toString();
    lab.code = obj["code"].toString();
    lab.created = QDateTime::fromString(obj["created"].toString(), Qt::ISODate);
    lab.modified = QDateTime::fromString(obj["modified"].toString(), Qt::ISODate);
    lab.description = obj["description"].toString();
    lab.isStarred = obj["isStarred"].toBool();

    QJsonArray tagsArray = obj["tags"].toArray();
    for (const auto &tag : tagsArray) {
        lab.tags << tag.toString();
    }

    return lab;
}

// ═══════════════════════════════════════════════════════════════════════════
// LabManager Implementation
// ═══════════════════════════════════════════════════════════════════════════

LabManager::LabManager(const QString &storageDir, QObject *parent)
    : QObject(parent)
    , m_storageDir(storageDir)
    , m_indexPath(storageDir + "/labs_index.json")
{
    QDir().mkpath(m_storageDir);
    loadIndex();
}

QString LabManager::createLab(const QString &title, const QString &language)
{
    CodeLab lab;
    lab.id = generateId();
    lab.title = title.isEmpty() ? "Untitled Lab" : title;
    lab.language = language;
    lab.code = "";  // Will be filled with template by caller
    lab.created = QDateTime::currentDateTime();
    lab.modified = lab.created;
    lab.isStarred = false;

    if (saveLab(lab)) {
        emit labCreated(lab.id);
        return lab.id;
    }

    return QString();
}

bool LabManager::saveLab(const CodeLab &lab)
{
    QString path = getLabPath(lab.id);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to save lab:" << path;
        return false;
    }

    QJsonDocument doc(lab.toJson());
    file.write(doc.toJson());
    file.close();

    // Update index
    bool found = false;
    for (int i = 0; i < m_index.size(); ++i) {
        QJsonObject obj = m_index[i].toObject();
        if (obj["id"].toString() == lab.id) {
            obj["title"] = lab.title;
            obj["language"] = lab.language;
            obj["modified"] = lab.modified.toString(Qt::ISODate);
            obj["isStarred"] = lab.isStarred;
            m_index[i] = obj;
            found = true;
            break;
        }
    }

    if (!found) {
        m_index.append(QJsonObject{
            {"id", lab.id},
            {"title", lab.title},
            {"language", lab.language},
            {"created", lab.created.toString(Qt::ISODate)},
            {"modified", lab.modified.toString(Qt::ISODate)},
            {"isStarred", lab.isStarred}
        });
    }

    saveIndex();
    emit labUpdated(lab.id);
    return true;
}

CodeLab LabManager::loadLab(const QString &id) const
{
    QString path = getLabPath(id);

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to load lab:" << path;
        return CodeLab();
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    return CodeLab::fromJson(doc.object());
}

bool LabManager::deleteLab(const QString &id)
{
    QString path = getLabPath(id);

    if (!QFile::remove(path)) {
        qWarning() << "Failed to delete lab file:" << path;
        return false;
    }

    // Remove from index
    for (int i = 0; i < m_index.size(); ++i) {
        if (m_index[i].toObject()["id"].toString() == id) {
            m_index.removeAt(i);
            break;
        }
    }

    saveIndex();
    emit labDeleted(id);
    return true;
}

bool LabManager::renameLab(const QString &id, const QString &newTitle)
{
    CodeLab lab = loadLab(id);
    if (lab.id.isEmpty()) {
        return false;
    }

    lab.title = newTitle;
    lab.modified = QDateTime::currentDateTime();
    return saveLab(lab);
}

QList<CodeLab> LabManager::getAllLabs() const
{
    QList<CodeLab> labs;

    for (const auto &item : m_index) {
        QString id = item.toObject()["id"].toString();
        CodeLab lab = loadLab(id);
        if (!lab.id.isEmpty()) {
            labs << lab;
        }
    }

    // Sort by modified date (newest first)
    std::sort(labs.begin(), labs.end(), [](const CodeLab &a, const CodeLab &b) {
        return a.modified > b.modified;
    });

    return labs;
}

QList<CodeLab> LabManager::getLabsByLanguage(const QString &language) const
{
    QList<CodeLab> labs = getAllLabs();
    labs.erase(std::remove_if(labs.begin(), labs.end(), [&](const CodeLab &lab) {
                   return lab.language != language;
               }), labs.end());
    return labs;
}

QList<CodeLab> LabManager::getStarredLabs() const
{
    QList<CodeLab> labs = getAllLabs();
    labs.erase(std::remove_if(labs.begin(), labs.end(), [](const CodeLab &lab) {
                   return !lab.isStarred;
               }), labs.end());
    return labs;
}

QList<CodeLab> LabManager::searchLabs(const QString &query) const
{
    if (query.isEmpty()) {
        return getAllLabs();
    }

    QList<CodeLab> labs = getAllLabs();
    QString q = query.toLower();

    labs.erase(std::remove_if(labs.begin(), labs.end(), [&](const CodeLab &lab) {
                   return !lab.title.toLower().contains(q) &&
                          !lab.description.toLower().contains(q) &&
                          !lab.tags.join(" ").toLower().contains(q);
               }), labs.end());

    return labs;
}
bool LabManager::exportLab(const QString &id, const QString &filePath) const
{
    CodeLab lab = loadLab(id);
    if (lab.id.isEmpty()) {
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(lab.toJson());
    file.write(doc.toJson());
    file.close();
    return true;
}

QString LabManager::importLab(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    CodeLab lab = CodeLab::fromJson(doc.object());

    // Generate new ID to avoid conflicts
    lab.id = generateId();
    lab.created = QDateTime::currentDateTime();
    lab.modified = lab.created;

    if (saveLab(lab)) {
        return lab.id;
    }

    return QString();
}

void LabManager::saveIndex()
{
    QFile file(m_indexPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to save index:" << m_indexPath;
        return;
    }

    QJsonDocument doc(m_index);
    file.write(doc.toJson());
    file.close();
}

void LabManager::loadIndex()
{
    QFile file(m_indexPath);
    if (!file.exists()) {
        m_index = QJsonArray();
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to load index:" << m_indexPath;
        m_index = QJsonArray();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    m_index = doc.array();
    file.close();
}

QString LabManager::generateId() const
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString LabManager::getLabPath(const QString &id) const
{
    return m_storageDir + "/" + id + ".json";
}
