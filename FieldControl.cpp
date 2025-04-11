#include "database.h"

/**
 * 根据表名查找表
 * @param db 数据库指针
 * @param tableName 要查找的表名
 * @return 找到返回表指针，否则返回NULL
 */
Table* findTable(Database* db, const char* tableName) {
    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i].name, tableName) == 0) {
            return &db->tables[i];
        }
    }
    return NULL;
}

/**
 * 向表添加字段
 * @param table 表指针
 * @param name 字段名
 * @param type 字段类型
 * @param isPrimaryKey 是否主键
 * @param length 字段长度(字符串类型使用)
 * @return 成功返回true，失败返回false
 */
bool addField(Table* table, const char* name, FieldType type, bool isPrimaryKey, int length) {
    // 检查字段数量限制
    if (table->fieldCount >= MAX_FIELDS) {
        printf("Error: Maximum number of fields reached for table %s\n", table->name);
        return false;
    }

    // 检查字段名是否已存在
    for (int i = 0; i < table->fieldCount; i++) {
        if (strcmp(table->fields[i].name, name) == 0) {
            printf("Error: Field name '%s' already exists in table %s\n", name, table->name);
            return false;
        }
    }

    // 初始化新字段
    Field* newField = &table->fields[table->fieldCount];
    strncpy_s(newField->name, name, MAX_NAME_LEN);
    newField->type = type;
    newField->isPrimaryKey = isPrimaryKey;
    newField->length = length;
    newField->constraintCount = 0;

    table->fieldCount++;
    printf("Field '%s' added to table '%s'\n", name, table->name);
    return true;
}
/**
 * 删除表中的字段
 * @param table 表指针
 * @param fieldName 要删除的字段名
 * @return 成功返回true，失败返回false
 */
bool deleteField(Table* table, const char* fieldName) {
    int index = -1;

    // 查找字段位置
    for (int i = 0; i < table->fieldCount; i++) {
        if (strcmp(table->fields[i].name, fieldName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Field '%s' not found in table '%s'\n", fieldName, table->name);
        return false;
    }

    // 删除字段，通过覆盖后续字段实现
    for (int i = index; i < table->fieldCount - 1; i++) {
        table->fields[i] = table->fields[i + 1];
    }

    table->fieldCount--;
    printf("Field '%s' deleted from table '%s'\n", fieldName, table->name);
    return true;
}

/**
 * 修改表中的字段属性
 * @param table 表指针
 * @param fieldName 要修改的字段名
 * @param newType 新的数据类型
 * @param newIsPrimaryKey 是否为主键
 * @param newLength 新长度(仅对字符串类型有效)
 * @return 成功返回true，失败返回false
 */
bool modifyField(Table* table, const char* fieldName, FieldType newType, bool newIsPrimaryKey, int newLength) {
    Field* field = NULL;

    for (int i = 0; i < table->fieldCount; i++) {
        if (strcmp(table->fields[i].name, fieldName) == 0) {
            field = &table->fields[i];
            break;
        }
    }

    if (!field) {
        printf("Error: Field '%s' not found in table '%s'\n", fieldName, table->name);
        return false;
    }

    // 修改字段属性
    field->type = newType;
    field->isPrimaryKey = newIsPrimaryKey;
    field->length = newLength;

    printf("Field '%s' in table '%s' modified\n", fieldName, table->name);
    return true;
}
