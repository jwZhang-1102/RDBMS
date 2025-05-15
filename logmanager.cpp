#include "logmanager.h"

QMutex LogManager::mutex;
const QString LogManager::logFilePath = "db_operations.log";

void LogManager::logOperation(const QString& operation, const QString& dbName,
                              const QString& tableName, const QString& sql) {
    QMutexLocker locker(&mutex);
    QFile file(logFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " | "
               << operation << " | "
               << "DB:" << dbName << " | "
               << "Table:" << tableName << " | "
               << "SQL:" << sql << "\n";
    }
}
