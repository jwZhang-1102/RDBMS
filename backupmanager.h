#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QString>
#include <QDir>

class BackupManager {
public:
    static bool createBackup(const QString& dbName);
    static bool restoreBackup(const QString& dbName);

private:
    static bool copyDirectory(const QDir& from, const QDir& to);
    static QString findLatestBackup(const QString& dbName);
};

#endif // BACKUPMANAGER_H
