#include "jsonutils.h"

#include <QFile>
#include <QJsonDocument>
#include <QDebug>

QJsonObject loadJsonFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open JSON:" << path;
        return {};
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format:" << path;
        return {};
    }

    return doc.object();
}
