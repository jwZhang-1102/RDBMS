#pragma once
#include "common.h"
#include <vector>
#include <unordered_map>

using namespace std;

class storage
{
private:
    unordered_map<string, unordered_map<string, vector<string>>> tables_;
public:
    storage(/* args */);
    ~storage();
    void create_table(const string& table_name);
    void insert(const string& table_name, const string& primary_key, const vector<string>& record);
    vector<vector<string>> scan(const string& table_name) const;
    bool has_record(const string& table_name, const string& primary_key) const;
    void delete_record(const string& table_name, const string& primary_key);
};
