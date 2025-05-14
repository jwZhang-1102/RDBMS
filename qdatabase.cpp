#include "QDatabase.h"
#include "table.h"

QDatabase::QDatabase(const QString& name) : name(name) {
    // 初始化
}

void QDatabase::createTable(const QString& name,
                            const QList<QPair<QString, QString>>& columns) {
    tables[name] = new Table(name, columns);
}

Table* QDatabase::getTable(const QString& name) {
    return tables.value(name, nullptr);
}

QStringList QDatabase::tableNames() const {
    return tables.keys();
}

bool QDatabase::hasTable(const QString& name){
    return tables.contains(name);
}

// ======================== [索引功能新增] ========================
bool QDatabase::createIndex(const QString& tableName, const QString& indexName, const QString& fieldName) {
    Table* table = getTable(tableName);
    if (!table) return false;

    if (table->hasIndex(indexName)) {
        return false; // 索引已存在
    }

    table->createIndex(indexName, fieldName);
    return true;
}

bool QDatabase::hasIndex(const QString& tableName, const QString& indexName) const {
    Table* table = tables.value(tableName, nullptr);
    if (!table) return false;
    return table->hasIndex(indexName);
}

bool QDatabase::dropIndex(const QString& tableName, const QString& indexName) {
    Table* table = getTable(tableName);
    if (!table) return false;

    if (!table->hasIndex(indexName)) {
        return false; // 索引不存在
    }

    table->dropIndex(indexName);  // 删除索引
    return true;
}
// ===============================================================

QDatabase::~QDatabase() {
    qDeleteAll(tables);
    tables.clear();
}
