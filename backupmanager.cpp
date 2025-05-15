#include "backupmanager.h"
#include <QFileInfo>
#include <QDateTime>
#include <algorithm>
#include <QDir>
#include <QFile>

bool BackupManager::copyDirectory(const QDir& from, const QDir& to) {
    if (!to.mkpath(".")) return false;

    foreach (const QFileInfo &info, from.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcPath = info.filePath();
        QString dstPath = to.filePath(info.fileName());

        if (info.isDir()) {
            if (!copyDirectory(QDir(srcPath), QDir(dstPath)))
                return false;
        } else {
            if (!QFile::copy(srcPath, dstPath))
                return false;
        }
    }
    return true;
}

bool BackupManager::createBackup(const QString& dbName) {
    QDir sourceDir(dbName);
    if (!sourceDir.exists()) return false;

    QString backupName = dbName + "_backup_" +
                         QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QDir backupDir(backupName);

    // 先创建目标目录
    if (!backupDir.mkpath(".")) return false;

    return copyDirectory(sourceDir, backupDir);
}

bool BackupManager::restoreBackup(const QString& dbName) {
    QString latestBackup = findLatestBackup(dbName);
    if (latestBackup.isEmpty()) return false;

    QDir targetDir(dbName);
    if (targetDir.exists()) {
        if (!targetDir.removeRecursively()) return false;
    }

    return copyDirectory(QDir(latestBackup), targetDir);
}

QString BackupManager::findLatestBackup(const QString& dbName) { // 移除static关键字（在cpp实现中不需要）
    QDir dir;
    QStringList backups = dir.entryList(QStringList(dbName + "_backup_*"), QDir::Dirs);
    std::sort(backups.begin(), backups.end(), std::greater<QString>());
    return backups.isEmpty() ? "" : backups.first();
}
