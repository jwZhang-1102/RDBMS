#include "storage.h"
#include "common.h"

storage::storage() = default;

storage::~storage() = default;

void storage::create_table(const std::string& table_name) {
    if (tables_.find(table_name) != tables_.end()) {
        throw DatabaseError("Table already exists");
    }
    tables_[table_name] = {};
}

void storage::insert(const std::string& table_name, const std::string& primary_key, const std::vector<std::string>& record) {
    auto& table = tables_[table_name];
    if (table.find(primary_key) != table.end()) {
        throw DatabaseError("Duplicate primary key");
    }
    table[primary_key] = record;
}

std::vector<std::vector<std::string>> storage::scan(const std::string& table_name) const {
    std::vector<std::vector<std::string>> records;
    auto it = tables_.find(table_name);
    if (it == tables_.end()) {
        throw DatabaseError("Table not found");
    }
    for (const auto& [_, record] : it->second) {
        records.push_back(record);
    }
    return records;
}

bool storage::has_record(const std::string& table_name, const std::string& primary_key) const {
    auto it = tables_.find(table_name);
    if (it == tables_.end()) return false;
    return it->second.find(primary_key) != it->second.end();
}

void storage::delete_record(const std::string& table_name, const std::string& primary_key) {
    auto& table = tables_[table_name];
    if (table.erase(primary_key) == 0) {
        throw DatabaseError("Record not found");
    }
}
