#include "dml_processor.h"
#include "storage.h"
#include "table_meta.h"
#include <iostream>

int main() {
    // ��ʼ��Ԫ����
    std::vector<Column> columns = {
        {"id", DataType::INT, true, false},
        {"name", DataType::STRING, false, true},
        {"age", DataType::INT, false, true}
    };
    TableMeta user_meta("users", columns);

    // ��ʼ���洢�����DML������
    storage engine;
    engine.create_table("users");
    std::unordered_map<std::string, TableMeta> tables_meta;
    tables_meta["users"] = user_meta;
    dml_processor dml(engine, tables_meta);

    // ��������(insertʵ��)
    dml.insert("users", { "1", "Alice", "30" });
    dml.insert("users", { "2", "Bob", "25" });

    // ���Բ�ѯ(selectʵ��)
    std::cout << "All users:" << std::endl;
    auto all_users = dml.select("users", "");
    for (const auto& row : all_users) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // ����������ѯ
    std::cout << "\nUsers with age=30:" << std::endl;
    auto result = dml.select("users", "age=30");
    for (const auto& row : result) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // ���Ը���(updateʵ��)
    dml.update("users", "name=Alice", { {"age", "31"} });
    std::cout << "\nAfter updating Alice's age to 31:" << std::endl;
    all_users = dml.select("users", "");
    for (const auto& row : all_users) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // ����ɾ��(dropʵ��)
    dml.delete_from("users", "name=Bob");
    std::cout << "\nAfter deleting Bob:" << std::endl;
    all_users = dml.select("users", "");
    for (const auto& row : all_users) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}