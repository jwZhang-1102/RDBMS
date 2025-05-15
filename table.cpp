#include "table.h"
#include "storageengine.h"
#include <QDebug>

Table::Table(const QString& name,
             const QList<QPair<QString, QString>>& columns,
             QObject* parent)
    : QObject(parent), name(name), columns(columns) {
}

Table::Table(const QString& name,
             const QStringList& columnNames,
             QObject* parent)
    : QObject(parent), name(name) {
    // 将列名列表转换为 QPair 格式，列类型默认为 "STRING"
    for (const QString& columnName : columnNames) {
        columns.append(QPair<QString, QString>(columnName, "STRING"));  // 假设所有列类型为 "STRING"
    }
    qDebug() << "Table created: " << name << " with attributes: " << columnNames;
}

// 插入数据
void Table::insert(const QVariantMap& data) {
    StorageEngine::insertRecord("当前数据库名", this->name, data);

    // ======================== [索引功能新增] ========================
    // 插入时更新所有索引
    QString primaryKey = data.value("id").toString(); // 假设主键字段为 "id"
    for (auto it = indices.begin(); it != indices.end(); ++it) {
        QString indexName = it.key();
        QString fieldName = it.value();
        QString fieldValue = data.value(fieldName).toString();
        indexData[fieldName][fieldValue] = primaryKey;
        qDebug() << "[索引已更新]" << fieldName << "=" << fieldValue << "->" << primaryKey;
    }
    // ===============================================================
}

// 简化实现：返回空
QList<QVariantMap> Table::selectAll() const {
    return {};
}

// ======================== [索引功能新增] ========================
void Table::createIndex(const QString& indexName, const QString& fieldName) {
    if (indices.contains(indexName)) {
        qDebug() << "[索引已存在]" << indexName;  // [调试]
        return;
    }
    indices[indexName] = fieldName;
    indexData[fieldName].clear();  // 初始化索引表
    qDebug() << "[索引创建成功]" << indexName << "字段:" << fieldName;
}


bool Table::hasIndex(const QString& indexName) const {
    return indices.contains(indexName);
}

void Table::dropIndex(const QString& indexName) {
    if (indices.contains(indexName)) {
        indices.remove(indexName);
        indexData.remove(indexName);  // 移除索引数据
        qDebug() << "[索引已删除]" << indexName;
    } else {
        qDebug() << "[删除失败] 索引不存在" << indexName;
    }
}

QString Table::indexField(const QString& indexName) const {
    return indices.value(indexName);
}
// ===============================================================
