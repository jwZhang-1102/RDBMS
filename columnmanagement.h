#ifndef COLUMNMANAGEMENT_H
#define COLUMNMANAGEMENT_H
#include <string>

class ColumnManagement
{
public:
    ColumnManagement();

    bool addColumn(const std::string& columnName, const std::string& columnType);

    bool removeColumn(const std::string& columnName);

    bool modifyColumn(const std::string& oldColumnName, const std::string& newColumnName, const std::string& newColumnType);
};

#endif // COLUMNMANAGEMENT_H
