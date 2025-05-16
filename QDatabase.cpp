#include "table.h"
#include "QDatabase.h"


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
    return tables.keys();  // 直接返回键列表
}

// 判断有没有这样的表
bool QDatabase::hasTable(const QString& name){
    return tables.contains(name);
}

// ======================== [索引功能新增] ========================
bool QDatabase::createIndex(const QString& tableName, const QString& indexName, const QString& fieldName) {
    qDebug() << "QDatabase::createIndex(tableName=" << tableName
             << ", indexName=" << indexName << ", fieldName=" << fieldName << ")";

    Table* table = getTable(tableName);
    Q_ASSERT(table != nullptr);  // 确保 table 不是 nullptr
    if (!table) {
        qDebug() << "Error: getTable() returned nullptr for " << tableName;
        return false;
    }

    if (table->hasIndex(indexName)) {
        qDebug() << "Error: Index " << indexName << " already exists on table " << tableName;
        return false; // 索引已存在
    }

    table->createIndex(indexName, fieldName);
    return true;
}

bool QDatabase::dropIndex(const QString& tableName, const QString& indexName) {
    qDebug() << "QDatabase::dropIndex(tableName=" << tableName
             << ", indexName=" << indexName << ")";

    Table* table = getTable(tableName);
    Q_ASSERT(table != nullptr);
    if (!table) {
        qDebug() << "Error: getTable() returned nullptr for " << tableName;
        return false;
    }

    if (!table->hasIndex(indexName)) {
        qDebug() << "Error: Index " << indexName << " not found on table " << tableName;
        return false;
    }

    table->dropIndex(indexName);
    return true;
}

bool QDatabase::hasIndex(const QString& tableName, const QString& indexName) const {
    qDebug() << "QDatabase::hasIndex(tableName=" << tableName << ", indexName=" << indexName << ")";
    Table* table = tables.value(tableName, nullptr);
    if (!table) {
        qDebug() << "  Table not found in tables.";
        return false;
    }
    bool indexExists = table->hasIndex(indexName);
    qDebug() << "  Index exists: " << indexExists;
    return indexExists;
}

void QDatabase::addTable(const QString& tableName, Table* table) {
    if (!tables.contains(tableName)) {
        tables.insert(tableName, table);  // 使用 insert 方法将表添加到 QMap 中
        qDebug() << "表 '" << tableName << "' 已添加到数据库中";  // 打印调试信息
    }
}


// ===============================================================

QDatabase::~QDatabase() {
    qDeleteAll(tables);
    tables.clear();
}

