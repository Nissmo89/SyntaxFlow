// process manager , for user Dynamic data
#include "progressmanager.h"
#include <qdir.h>
#include <qstandardpaths.h>

ProgressManager::ProgressManager(QObject *parent)
    : QObject(parent)
{
    // Get app data location
    QString appDataPath =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(appDataPath);

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    filePath = dir.filePath("user_progress.json");
}

void ProgressManager::load()
{
    QFile file(filePath);

    if (!file.exists()) {
        qDebug() << "Progress file does not exist. Creating new one.";
        progressData = QJsonObject();
        save();
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open progress file.";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        qWarning() << "Invalid progress file format.";
        progressData = QJsonObject();
        return;
    }

    progressData = doc.object();
}

void ProgressManager::save()
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to save progress file.";
        return;
    }

    QJsonDocument doc(progressData);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void ProgressManager::ensureProblemExists(const QString &id)
{
    if (!progressData.contains(id)) {
        QJsonObject obj;
        obj["solved"] = false;
        obj["starred"] = false;
        obj["attempts"] = 0;
        progressData[id] = obj;
    }
}

bool ProgressManager::isSolved(const QString &id) const
{
    if (!progressData.contains(id))
        return false;

    QJsonObject obj = progressData.value(id).toObject();
    return obj.value("solved").toBool(false);
}

void ProgressManager::markSolved(const QString &id, bool solved)
{
    ensureProblemExists(id);

    QJsonObject obj = progressData.value(id).toObject();
    obj["solved"] = solved;

    progressData[id] = obj;

    save();
    emit progressChanged(id);
}

bool ProgressManager::isStarred(const QString &id) const
{
    if (!progressData.contains(id))
        return false;

    QJsonObject obj = progressData.value(id).toObject();
    return obj.value("starred").toBool(false);
}

void ProgressManager::toggleStar(const QString &id)
{
    ensureProblemExists(id);

    QJsonObject obj = progressData.value(id).toObject();
    bool current = obj.value("starred").toBool(false);

    obj["starred"] = !current;
    progressData[id] = obj;

    save();
    emit progressChanged(id);
}
