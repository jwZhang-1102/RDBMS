#include "table_meta.h"
#include "common.h"
#include <algorithm>

using namespace std;

TableMeta::TableMeta(const string& name, const vector<Column>& cols)
    : table_name(name), columns(cols) {
    // ����������
    auto it = find_if(columns.begin(), columns.end(),
        [](const Column& col) { return col.is_primary_key; });
    if (it != columns.end()) {
        primary_key = *it;
    }
}

const Column& TableMeta::get_primary_key() const {
    return primary_key;
}

const vector<Column>& TableMeta::get_columns() const {
    return columns;
}

bool TableMeta::validate_record(const vector<string>& record) const {
    if (record.size() != columns.size()) {
        return false;
    }

    for (size_t i = 0; i < record.size(); ++i) {
        // TODO: ��Ӹ���ϸ������������֤
        if (!columns[i].is_nullable && record[i].empty()) {
            return false;
        }
    }

    return true;
}

TableMeta::~TableMeta() {
}
