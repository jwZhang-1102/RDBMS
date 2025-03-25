#ifndef TABLEMANAGEMENT_H
#define TABLEMANAGEMENT_H

#include <string>
#include <vector>

class TableManagement
{
public:
    TableManagement();

    // 创建表函数
    bool createTable(const std::string& tableName, const std::vector<std::string>& columns);

    // 修改表结构函数（添加列等）
    bool alterTable(const std::string& tableName, const std::string& columnDefinition);

    // 删除表函数
    bool dropTable(const std::string& tableName);
};

#endif // TABLEMANAGEMENT_H
