#include "dml_processor.h"

dml_processor::dml_processor(storage& engine,
    const std::unordered_map<std::string, TableMeta>& tables_meta)
    : engine_(engine), tables_meta_(tables_meta) {
}

dml_processor::~dml_processor() {
}

std::string dml_processor::get_primary_key_value(const std::vector<std::string>& record,
    const std::string& table_name) const {
    auto it = tables_meta_.find(table_name);
    if (it == tables_meta_.end()) {
        throw DatabaseError("Table not found");
    }

    const Column& pk = it->second.get_primary_key();
    const vector<Column>& cols = it->second.get_columns();

    for (size_t i = 0; i < cols.size(); ++i) {
        if (cols[i].name == pk.name) {
            return record[i];
        }
    }

    throw DatabaseError("Primary key column not found");
}

bool dml_processor::evaluate_condition(const std::vector<std::string>& record,
    const std::string& condition) const {
    // 简化实现: 只支持简单的列名=值条件
    size_t eq_pos = condition.find('=');
    if (eq_pos == string::npos) {
        throw DatabaseError("Invalid condition format");
    }

    string col_name = condition.substr(0, eq_pos);
    string value = condition.substr(eq_pos + 1);

    // 手动去除空格
    col_name.erase(remove_if(col_name.begin(), col_name.end(),
        [](char c) { return isspace(c); }), col_name.end());
    value.erase(remove_if(value.begin(), value.end(),
        [](char c) { return isspace(c); }), value.end());

    // 查找列索引
    const vector<Column>& cols = tables_meta_.begin()->second.get_columns();
    size_t col_idx = 0;
    for (; col_idx < cols.size(); ++col_idx) {
        if (cols[col_idx].name == col_name) {
            break;
        }
    }

    if (col_idx >= cols.size()) {
        throw DatabaseError("Column not found in condition");
    }

    return record[col_idx] == value;
}

void dml_processor::insert(const std::string& table_name,
    const std::vector<std::string>& values) {
    auto it = tables_meta_.find(table_name);
    if (it == tables_meta_.end()) {
        throw DatabaseError("Table not found");
    }

    // 验证记录格式
    if (!it->second.validate_record(values)) {
        throw DatabaseError("Invalid record format");
    }

    // 获取主键值
    string pk_value = get_primary_key_value(values, table_name);

    // 插入记录
    engine_.insert(table_name, pk_value, values);
}

std::vector<std::vector<std::string>> dml_processor::select(const std::string& table_name,
    const std::string& condition) const {
    auto it = tables_meta_.find(table_name);
    if (it == tables_meta_.end()) {
        throw DatabaseError("Table not found");
    }

    // 获取所有记录
    vector<vector<string>> all_records = engine_.scan(table_name);
    vector<vector<string>> result;

    // 如果没有条件，返回所有记录
    if (condition.empty()) {
        return all_records;
    }

    // 根据条件过滤记录
    for (const auto& record : all_records) {
        if (evaluate_condition(record, condition)) {
            result.push_back(record);
        }
    }

    return result;
}

void dml_processor::update(const std::string& table_name,
    const std::string& condition,
    const std::vector<std::pair<std::string, std::string>>& updates) {
    auto it = tables_meta_.find(table_name);
    if (it == tables_meta_.end()) {
        throw DatabaseError("Table not found");
    }

    // 获取所有符合条件的记录
    vector<vector<string>> records = select(table_name, condition);
    const Column& pk = it->second.get_primary_key();

    // 更新每条记录
    for (auto& record : records) {
        string old_pk = get_primary_key_value(record, table_name);
        bool pk_updated = false;

        // 应用更新
        for (const auto& [col_name, new_value] : updates) {
            // 查找列索引
            const vector<Column>& cols = it->second.get_columns();
            size_t col_idx = 0;
            for (; col_idx < cols.size(); ++col_idx) {
                if (cols[col_idx].name == col_name) {
                    break;
                }
            }

            if (col_idx >= cols.size()) {
                throw DatabaseError("Column not found: " + col_name);
            }

            // 检查是否是主键更新
            if (cols[col_idx].name == pk.name) {
                pk_updated = true;
            }

            record[col_idx] = new_value;
        }

        // 验证更新后的记录
        if (!it->second.validate_record(record)) {
            throw DatabaseError("Invalid record after update");
        }

        // 处理主键更新
        if (pk_updated) {
            string new_pk = get_primary_key_value(record, table_name);
            if (engine_.has_record(table_name, new_pk)) {
                throw DatabaseError("Duplicate primary key");
            }
            engine_.delete_record(table_name, old_pk);
            engine_.insert(table_name, new_pk, record);
        }
        else {
            engine_.delete_record(table_name, old_pk);
            engine_.insert(table_name, old_pk, record);
        }
    }
}

void dml_processor::delete_from(const std::string& table_name,
    const std::string& condition) {
    auto it = tables_meta_.find(table_name);
    if (it == tables_meta_.end()) {
        throw DatabaseError("Table not found");
    }

    // 获取所有符合条件的记录
    vector<vector<string>> records = select(table_name, condition);

    // 删除每条记录
    for (const auto& record : records) {
        string pk_value = get_primary_key_value(record, table_name);
        engine_.delete_record(table_name, pk_value);
    }
}
