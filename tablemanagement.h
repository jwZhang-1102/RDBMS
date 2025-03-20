#ifndef TABLEMANAGEMENT_H
#define TABLEMANAGEMENT_H
#include <string>

class TableManagement
{
public:
    TableManagement();

    bool createTable(const std::string& tableName, const std::vector<std::string>& columns);

    bool alterTable(const std::string& tableName, const std::string& columnDefinition);

    bool dropTable(const std::string& tableName);
};

#endif // TABLEMANAGEMENT_H
