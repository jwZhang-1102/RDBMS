#pragma once
#include "common.h"
#include <vector>

using namespace std;

// 数据列结构体
struct Column
{
    string name;
    DataType type;
    bool is_primary_key;
    bool is_nullable;

    bool is_foreign_key = false;
    string ref_table;
    string ref_column;
};

// 表元数据管理类
class TableMeta
{
private:
    string table_name;
    vector<Column> columns;
    Column primary_key;

public:
    TableMeta() = default;
    TableMeta(const string& name, const vector<Column>& columns);

    const string& get_table_name() const;
    const Column& get_primary_key() const;
    const vector<Column>& get_columns() const;

    bool validate_record(const vector<string>& record) const;

    ~TableMeta();
};
