#ifndef QDATABASE_H
#define QDATABASE_H

#include <QString>
#include <QMap>
#include <QStringList>
#include "table.h"  // [索引功能新增] 确保包含 Table 类声明

class Table;

class QDatabase {
public:
    explicit QDatabase(const QString& name);

    void createTable(const QString& name,
                     const QList<QPair<QString, QString>>& columns);
    Table* getTable(const QString& name);
    bool hasTable(const QString& name);
    QStringList tableNames() const;

    // ======================== [索引功能新增] ========================
    bool createIndex(const QString& tableName, const QString& indexName, const QString& fieldName);
    bool hasIndex(const QString& tableName, const QString& indexName) const;
    bool dropIndex(const QString& tableName, const QString& indexName);
    // ===============================================================

    ~QDatabase();

private:
    QString name;
    QMap<QString, Table*> tables;
};

#endif // QDATABASE_H
