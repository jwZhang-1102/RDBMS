#include "dml_processor.h"
#include "storage.h"
#include "table_meta.h"
#include <iostream>

int main() {
    // 初始化元数据
    std::vector<Column> columns = {
        {"id", DataType::INT, true, false},
        {"name", DataType::STRING, false, true},
        {"age", DataType::INT, false, true}
    };
    TableMeta user_meta("users", columns);

    // 初始化存储引擎和DML处理器
    storage engine;
    engine.create_table("users");
    std::unordered_map<std::string, TableMeta> tables_meta;
    tables_meta["users"] = user_meta;
    dml_processor dml(engine, tables_meta);

    // 插入数据(insert实现)
    dml.insert("users", { "1", "Alice", "30" });
    dml.insert("users", { "2", "Bob", "25" });

    // 测试查询(select实现)
    std::cout << "All users:" << std::endl;
    auto all_users = dml.select("users", "");
    for (const auto& row : all_users) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 测试条件查询
    std::cout << "\nUsers with age=30:" << std::endl;
    auto result = dml.select("users", "age=30");
    for (const auto& row : result) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 测试更新(update实现)
    dml.update("users", "name=Alice", { {"age", "31"} });
    std::cout << "\nAfter updating Alice's age to 31:" << std::endl;
    all_users = dml.select("users", "");
    for (const auto& row : all_users) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 测试删除(drop实现)
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