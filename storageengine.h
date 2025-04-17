#ifndef STORAGEENGINE_H
#define STORAGEENGINE_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QMap>
#include <QVariant>

class StorageEngine : public QObject {
public:
    static void createDatabase(const QString& dbName);
    static void createTable(const QString& dbName, const QString& tableName,
                            const QList<QPair<QString, QString>>& columns);
    static void insertRecord(const QString& dbName, const QString& tableName,
                             const QVariantMap& data);
    static QList<QVariantMap> selectAll(const QString& dbName, const QString& tableName);
};

#endif // STORAGEENGINE_H
