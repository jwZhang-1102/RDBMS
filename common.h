#pragma once
#include <string>
#include <stdexcept>

using namespace std;

//错误类型定义
class DatabaseError :public runtime_error {
public:
    explicit DatabaseError(const string& msg) : runtime_error(msg) {}
};

//数据类型定义
enum class DataType { INT, FLOAT, STRING };