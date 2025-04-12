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
    // ��ʵ��: ֻ֧�ּ򵥵�����=ֵ����
    size_t eq_pos = condition.find('=');
    if (eq_pos == string::npos) {
        throw DatabaseError("Invalid condition format");
    }

    string col_name = condition.substr(0, eq_pos);
    string value = condition.substr(eq_pos + 1);

    // �ֶ�ȥ���ո�
    col_name.erase(remove_if(col_name.begin(), col_name.end(),
        [](char c) { return isspace(c); }), col_name.end());
    value.erase(remove_if(value.begin(), value.end(),
        [](char c) { return isspace(c); }), value.end());

    // ����������
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

    // ��֤��¼��ʽ
    if (!it->second.validate_record(values)) {
        throw DatabaseError("Invalid record format");
    }

    // ��ȡ����ֵ
    string pk_value = get_primary_key_value(values, table_name);

    // �����¼
    engine_.insert(table_name, pk_value, values);
}

std::vector<std::vector<std::string>> dml_processor::select(const std::string& table_name,
    const std::string& condition) const {
    auto it = tables_meta_.find(table_name);
    if (it == tables_meta_.end()) {
        throw DatabaseError("Table not found");
    }

    // ��ȡ���м�¼
    vector<vector<string>> all_records = engine_.scan(table_name);
    vector<vector<string>> result;

    // ���û���������������м�¼
    if (condition.empty()) {
        return all_records;
    }

    // �����������˼�¼
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

    // ��ȡ���з��������ļ�¼
    vector<vector<string>> records = select(table_name, condition);
    const Column& pk = it->second.get_primary_key();

    // ����ÿ����¼
    for (auto& record : records) {
        string old_pk = get_primary_key_value(record, table_name);
        bool pk_updated = false;

        // Ӧ�ø���
        for (const auto& [col_name, new_value] : updates) {
            // ����������
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

            // ����Ƿ�����������
            if (cols[col_idx].name == pk.name) {
                pk_updated = true;
            }

            record[col_idx] = new_value;
        }

        // ��֤���º�ļ�¼
        if (!it->second.validate_record(record)) {
            throw DatabaseError("Invalid record after update");
        }

        // ������������
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

    // ��ȡ���з��������ļ�¼
    vector<vector<string>> records = select(table_name, condition);

    // ɾ��ÿ����¼
    for (const auto& record : records) {
        string pk_value = get_primary_key_value(record, table_name);
        engine_.delete_record(table_name, pk_value);
    }
}
