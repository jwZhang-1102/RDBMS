#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QString>
#include <QFile>
#include <QDateTime>
#include <QMutex>

class LogManager {
public:
    static void logOperation(const QString& operation, const QString& dbName, const QString& tableName, const QString& sql);
    static const QString logFilePath;

private:
    static QMutex mutex;
};

#endif // LOGMANAGER_H
