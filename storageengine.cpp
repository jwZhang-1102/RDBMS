#include "storageengine.h"
#include <QDir>
#include "exceptions.h"
#include <QFile>
#include <QDataStream>

void StorageEngine::createDatabase(const QString& dbName) {
    QDir dir;
    if (!dir.mkpath(dbName)) {
        throw SqlException("Failed to create database directory");
    }
}

void StorageEngine::createTable(const QString& dbName, const QString& tableName,
                                const QList<QPair<QString, QString>>& columns) {
    QString path = QString("%1/%2.tbl").arg(dbName, tableName);
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        throw SqlException("Cannot create table file");
    }

    QDataStream out(&file);
    out << columns;
    file.close();
}

QList<QVariantMap> StorageEngine::selectAll(const QString& dbName, const QString& tableName) {
    QList<QVariantMap> results;
    QString path = QString("%1/%2.tbl").arg(dbName, tableName);
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        throw SqlException("Table not found or cannot be opened");
    }

    QDataStream in(&file);
    QList<QPair<QString, QString>> columns; // 读取表结构
    in >> columns;

    // 读取所有记录
    while (!in.atEnd()) {
        QVariantMap record;
        in >> record;
        results.append(record);
    }

    file.close();
    return results;
}

void StorageEngine::insertRecord(const QString& dbName,
                                 const QString& tableName,
                                 const QVariantMap& data) {
    QString path = QString("%1/%2.tbl").arg(dbName, tableName);
    QFile file(path);
    if (!file.open(QIODevice::Append)) {
        throw SqlException("Cannot open table file: " + tableName);
    }
    QDataStream out(&file);
    out << data; // 确保序列化操作有效
    file.close();
}
