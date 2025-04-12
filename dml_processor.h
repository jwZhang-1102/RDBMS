#pragma once
#include "storage.h"
#include "table_meta.h"
#include <unordered_map>

using namespace std;

class dml_processor
{
private:
    storage& engine_;
    unordered_map<std::string, TableMeta> tables_meta_;

    // ¸¨Öú·½·¨
    string get_primary_key_value(const vector<string>& record, const string& table_name) const;
    bool evaluate_condition(const vector<string>& record, const string& condition) const;
public:
    dml_processor(storage& engine, const unordered_map<string, TableMeta>& tables_meta);
    ~dml_processor();

    // DML ²Ù×÷
    void insert(const string& table_name, const vector<string>& values);
    vector<vector<string>> select(const string& table_name, const string& condition) const;
    void update(const string& table_name, const string& condition, const vector<pair<string, string>>& updates);
    void delete_from(const string& table_name, const string& condition);
};
