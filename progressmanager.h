#ifndef PROGRESSMANAGER_H
#define PROGRESSMANAGER_H


#include <QObject>
#include <QJsonObject>
#include <QString>

class ProgressManager : public QObject
{
    Q_OBJECT

public:
    explicit ProgressManager(QObject *parent = nullptr);

    void load();
    void save();

    bool isSolved(const QString &id) const;
    void markSolved(const QString &id, bool solved = true);

    bool isStarred(const QString &id) const;
    void toggleStar(const QString &id);

signals:
    void progressChanged(const QString &problemId);

private:
    void ensureProblemExists(const QString &id);

private:
    QJsonObject progressData;
    QString filePath;
};


#endif // PROGRESSMANAGER_H
