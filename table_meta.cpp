// table_meta.cpp
#include "table_meta.h"

TableMeta::TableMeta(const string& name, const vector<Column>& cols)
    : table_name(name), columns(cols) {
    for (const auto& col : cols) {
        if (col.is_primary_key) {
            primary_key = col;
            break;
        }
    }
}

const string& TableMeta::get_table_name() const {
    return table_name;
}

const Column& TableMeta::get_primary_key() const {
    return primary_key;
}

const vector<Column>& TableMeta::get_columns() const {
    return columns;
}

// 验证记录是否符合域完整性
bool TableMeta::validate_record(const vector<string>& record) const {
    if (record.size() != columns.size()) return false;
    for (size_t i = 0; i < columns.size(); ++i) {
        if (!columns[i].is_nullable && record[i].empty()) {
            return false;
        }
        if (columns[i].type == DataType::INT) {
            if (!all_of(record[i].begin(), record[i].end(), ::isdigit)) {
                return false;
            }
        }
        else if (columns[i].type == DataType::FLOAT) {
            bool dot = false;
            for (char c : record[i]) {
                if (c == '.') {
                    if (dot) return false;
                    dot = true;
                }
                else if (!isdigit(c)) return false;
            }
        }
        // STRING 不验证
    }
    return true;
}

TableMeta::~TableMeta() {}
