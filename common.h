#pragma once
#include <string>
#include <stdexcept>

using namespace std;

//�������Ͷ���
class DatabaseError :public runtime_error {
public:
    explicit DatabaseError(const string& msg) : runtime_error(msg) {}
};

//�������Ͷ���
enum class DataType { INT, FLOAT, STRING };