#include "database.h"
#include "table.h"


Database::Database(const QString& name) : name(name) {
    // 初始化
}


void Database::createTable(const QString& name,
                           const QList<QPair<QString, QString>>& columns) {
    tables[name] = new Table(name, columns);
}

Table* Database::getTable(const QString& name) {
    return tables.value(name, nullptr);
}

QStringList Database::tableNames() const {
    return tables.keys();  // 直接返回键列表
}

// 判断有没有这样的表
bool Database::hasTable(const QString& name){
    return tables.contains(name);
}

Database::~Database() {
    qDeleteAll(tables);
    tables.clear();
}

