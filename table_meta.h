#pragma once
#include "common.h"
#include <vector>

using namespace std;

// �����нṹ��
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

// ��Ԫ���ݹ�����
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
