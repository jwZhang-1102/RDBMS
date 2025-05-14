#pragma once
#include "common.h"
#include <vector>

using namespace std;

//结构体定义
struct Column
{
    string name;
    DataType type;
    bool is_primary_key;
    bool is_nullable;
};

//表元数据管理
class TableMeta
{
private:
    string table_name;
    vector<Column> columns;
    Column primary_key;
public:
    TableMeta() = default;
    TableMeta(const string& name, const vector<Column>& columns);
    const Column& get_primary_key() const;
    const vector<Column>& get_columns() const;
    bool validate_record(const vector<string>& record) const;
    ~TableMeta();
};
