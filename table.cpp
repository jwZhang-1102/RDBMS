#include "table.h"
#include "storageengine.h"

// 实现构造函数
Table::Table(const QString& name,
             const QList<QPair<QString, QString>>& columns,
             QObject* parent)
    : QObject(parent), name(name), columns(columns) {
}

QList<QVariantMap> Table::selectAll() const {
    // 这里添加查询数据的具体逻辑
    return {};
}

// 插入
void Table::insert(const QVariantMap& data) {
    StorageEngine::insertRecord("当前数据库名", this->name, data);
}
