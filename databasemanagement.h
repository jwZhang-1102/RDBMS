#ifndef DATABASEMANAGEMENT_H
#define DATABASEMANAGEMENT_H
#include <string>

class DataBaseManagement
{
public:
    DataBaseManagement();

    bool createDatabase(const std::string& dbName);

    bool deleteDatabase(const std::string& dbName);

    bool databaseExists(const std::string& dbName) const;//删除前先判断数据库是否存在

};

#endif // DATABASEMANAGEMENT_H
