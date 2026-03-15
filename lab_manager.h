#ifndef LAB_MANAGER_H
#define LAB_MANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>

struct CodeLab {
    QString id;              // Unique identifier
    QString title;           // User-given name
    QString language;        // Language ID (cpp, python, etc.)
    QString code;            // Source code
    QDateTime created;       // Creation timestamp
    QDateTime modified;      // Last modified timestamp
    QString description;     // Optional description
    bool isStarred;         // Favorite flag
    QStringList tags;       // User tags

    QJsonObject toJson() const;
    static CodeLab fromJson(const QJsonObject &obj);
};

class LabManager : public QObject
{
    Q_OBJECT

public:
    explicit LabManager(const QString &storageDir, QObject *parent = nullptr);

    // CRUD operations
    QString createLab(const QString &title, const QString &language);
    bool saveLab(const CodeLab &lab);
    CodeLab loadLab(const QString &id) const;
    bool deleteLab(const QString &id);
    bool renameLab(const QString &id, const QString &newTitle);

    // Query operations
    QList<CodeLab> getAllLabs() const;
    QList<CodeLab> getLabsByLanguage(const QString &language) const;
    QList<CodeLab> getStarredLabs() const;
    QList<CodeLab> searchLabs(const QString &query) const;

    // Bulk operations
    bool exportLab(const QString &id, const QString &filePath) const;
    QString importLab(const QString &filePath);

    QString getStorageDir() const { return m_storageDir; }

signals:
    void labCreated(const QString &id);
    void labUpdated(const QString &id);
    void labDeleted(const QString &id);

private:
    QString m_storageDir;
    QString m_indexPath;

    void saveIndex();
    void loadIndex();
    QJsonArray m_index;

    QString generateId() const;
    QString getLabPath(const QString &id) const;
};

#endif // LAB_MANAGER_H
