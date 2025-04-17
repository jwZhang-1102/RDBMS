#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 定义各种最大长度限制
#define MAX_NAME_LEN 50       // 名称最大长度
#define MAX_FIELDS 20         // 每个表最大字段数
#define MAX_TABLES 10         // 数据库最大表数
#define MAX_CONSTRAINTS 10    // 每个字段最大约束数

// 字段类型枚举，定义支持的字段数据类型
typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    BOOL_TYPE
} FieldType;

// 约束类型枚举，定义支持的约束类型
typedef enum {
    NOT_NULL,     // 非空约束
    UNIQUE,       // 唯一约束
    DEFAULT,      // 默认值约束
    CHECK,        // 检查约束
    FOREIGN_KEY   // 外键约束
} ConstraintType;

// 约束条件结构体
typedef struct {
    ConstraintType type;      // 约束类型
    char name[MAX_NAME_LEN];  // 约束名称

    // 联合体，用于存储不同类型的默认值(仅DEFAULT约束使用)
    union {
        int intValue;        // 整型默认值
        float floatValue;     // 浮点型默认值
        char stringValue[MAX_NAME_LEN]; // 字符串默认值
        bool boolValue;       // 布尔型默认值
    } defaultValue;

    // 外键约束专用字段
    char refTable[MAX_NAME_LEN];   // 引用表名
    char refField[MAX_NAME_LEN];   // 引用字段名

    // 检查约束专用字段
    char checkCondition[MAX_NAME_LEN * 2]; // 检查条件表达式
} Constraint;

// 字段结构体
typedef struct {
    char name[MAX_NAME_LEN];       // 字段名称
    FieldType type;                // 字段类型
    bool isPrimaryKey;             // 是否主键
    int length;                    // 字段长度(主要用于字符串类型)

    Constraint constraints[MAX_CONSTRAINTS]; // 字段约束数组
    int constraintCount;           // 当前约束数量
} Field;

// 表结构体
typedef struct {
    char name[MAX_NAME_LEN];       // 表名称
    Field fields[MAX_FIELDS];      // 字段数组
    int fieldCount;                // 当前字段数量
} Table;

// 数据库结构体
typedef struct {
    Table tables[MAX_TABLES];      // 表数组
    int tableCount;                // 当前表数量
} Database;

// 函数声明
Table* findTable(Database* db, const char* tableName);
const char* constraintToString(const Constraint* constraint);
bool addConstraint(Field* field, ConstraintType type, const char* name,
    const char* refTable, const char* refField, const char* checkCondition);
bool deleteConstraint(Field* field, const char* name);
bool validateForeignKey(Database* db, const char* tableName, const char* fieldName,
    const char* refTableName, const char* refFieldName);
bool addField(Table* table, const char* name, FieldType type, bool isPrimaryKey, int length);
bool deleteField(Table* table, const char* fieldName);
bool modifyField(Table* table, const char* fieldName, FieldType newType, bool newIsPrimaryKey, int newLength);
void printTableStructure(const Table* table);

#endif // DATABASE_H