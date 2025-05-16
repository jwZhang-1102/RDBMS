#include "database.h"
#include <cstdio>
#include <cstring>

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
bool deleteField(Database* db, Table* table, const char* fieldName) {
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

    // 检查是否被其他表外键引用
    for (int t = 0; t < db->tableCount; t++) {
        Table* otherTable = &db->tables[t];
        for (int f = 0; f < otherTable->fieldCount; f++) {
            Field* otherField = &otherTable->fields[f];
            for (int c = 0; c < otherField->constraintCount; c++) {
                Constraint* con = &otherField->constraints[c];
                if (con->type == FOREIGN_KEY &&
                    strcmp(con->refTable, table->name) == 0 &&
                    strcmp(con->refField, fieldName) == 0) {
                    printf("Error: Cannot delete field '%s' in table '%s' — it is referenced by FOREIGN_KEY '%s' in table '%s', field '%s'\n",
                        fieldName, table->name, con->name, otherTable->name, otherField->name);
                    return false;
                }
            }
        }
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

    // 检查现有关联约束
    for (int i = 0; i < field->constraintCount; i++) {
        Constraint* c = &field->constraints[i];
        if (c->type == FOREIGN_KEY) {
            printf("Warning: Field type change may invalidate FOREIGN_KEY constraint '%s'\n", c->name);
        }
        if (c->type == DEFAULT) {
            // 检查默认值类型是否匹配
            switch (newType) {
            case INT_TYPE:
                // 合法，intValue
                break;
            case FLOAT_TYPE:
                break;
            case STRING_TYPE:
                break;
            case BOOL_TYPE:
                break;
            default:
                printf("Warning: Unknown type in DEFAULT constraint '%s'\n", c->name);
            }
        }
        if (c->type == CHECK) {
            printf("Warning: Field type change may invalidate CHECK constraint '%s'\n", c->name);
        }
    }

    // 修改字段属性
    field->type = newType;
    field->isPrimaryKey = newIsPrimaryKey;
    field->length = newLength;

    printf("Field '%s' in table '%s' modified\n", fieldName, table->name);
    return true;
}
