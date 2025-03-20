#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H
#include <string>

class Datamanagement
{
public:
    Datamanagement();

    bool insertData(const std::string& columnName, const std::string& value);

    bool updateData(const std::string& columnName, const std::string& oldValue, const std::string& newValue);

    bool deleteData(const std::string& columnName, const std::string& value);

    std::vector<std::string> queryData(const std::string& columnName, const std::string& value);
};

#endif // DATAMANAGEMENT_H
