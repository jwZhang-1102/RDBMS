#include "database.h"

/**
 * 将约束转换为可读字符串
 * @param constraint 约束指针
 * @return 约束的字符串表示
 */
const char* constraintToString(const Constraint* constraint) {
    static char buf[256];  // 静态缓冲区，用于返回字符串

    switch (constraint->type) {
    case NOT_NULL:
        snprintf(buf, sizeof(buf), "NOT NULL");
        break;
    case UNIQUE:
        snprintf(buf, sizeof(buf), "UNIQUE");
        break;
    case DEFAULT:
        snprintf(buf, sizeof(buf), "DEFAULT");
        break;
    case CHECK:
        snprintf(buf, sizeof(buf), "CHECK(%s)", constraint->checkCondition);
        break;
    case FOREIGN_KEY:
        snprintf(buf, sizeof(buf), "FOREIGN KEY REFERENCES %s(%s)",
            constraint->refTable, constraint->refField);
        break;
    default:
        snprintf(buf, sizeof(buf), "UNKNOWN");
    }
    return buf;
}

/**
 * 向字段添加约束
 * @param field 字段指针
 * @param type 约束类型
 * @param name 约束名称
 * @param refTable 外键引用表名(FOREIGN_KEY专用)
 * @param refField 外键引用字段名(FOREIGN_KEY专用)
 * @param checkCondition 检查条件(CHECK专用)
 * @return 成功返回true，失败返回false
 */
bool addConstraint(Field* field, ConstraintType type, const char* name,
    const char* refTable, const char* refField,
    const char* checkCondition) {

    // 检查约束数量限制
    if (field->constraintCount >= MAX_CONSTRAINTS) {
        printf("Error: Maximum number of constraints reached for field %s\n", field->name);
        return false;
    }

    // 检查约束名是否已存在
    for (int i = 0; i < field->constraintCount; i++) {
        if (strcmp(field->constraints[i].name, name) == 0) {
            printf("Error: Constraint name '%s' already exists for field %s\n", name, field->name);
            return false;
        }
    }

    // 初始化新约束
    Constraint* newConstraint = &field->constraints[field->constraintCount];
    newConstraint->type = type;
    strncpy_s(newConstraint->name, name, MAX_NAME_LEN);

    // 处理特定约束类型的额外参数
    if (type == FOREIGN_KEY) {
        if (refTable == NULL || refField == NULL) {
            printf("Error: FOREIGN_KEY constraint requires reference table and field\n");
            return false;
        }
        strncpy_s(newConstraint->refTable, refTable, MAX_NAME_LEN);
        strncpy_s(newConstraint->refField, refField, MAX_NAME_LEN);
    }
    else if (type == CHECK) {
        if (checkCondition == NULL) {
            printf("Error: CHECK constraint requires a condition\n");
            return false;
        }
        strncpy_s(newConstraint->checkCondition, checkCondition, MAX_NAME_LEN * 2);
    }

    field->constraintCount++;
    printf("Constraint '%s' added to field '%s'\n", name, field->name);
    return true;
}

/**
 * 从字段删除约束
 * @param field 字段指针
 * @param name 要删除的约束名
 * @return 成功返回true，失败返回false
 */
bool deleteConstraint(Field* field, const char* name) {
    int index = -1;

    // 查找约束位置
    for (int i = 0; i < field->constraintCount; i++) {
        if (strcmp(field->constraints[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Constraint '%s' not found in field '%s'\n", name, field->name);
        return false;
    }

    // 移动后续约束向前覆盖被删除的约束
    for (int i = index; i < field->constraintCount - 1; i++) {
        field->constraints[i] = field->constraints[i + 1];
    }

    field->constraintCount--;
    printf("Constraint '%s' deleted from field '%s'\n", name, field->name);
    return true;
}

/**
 * 验证外键约束是否有效
 * @param db 数据库指针
 * @param tableName 表名
 * @param fieldName 字段名
 * @param refTableName 引用表名
 * @param refFieldName 引用字段名
 * @return 有效返回true，无效返回false
 */
bool validateForeignKey(Database* db, const char* tableName, const char* fieldName,
    const char* refTableName, const char* refFieldName) {

    // 检查引用表是否存在
    Table* refTable = findTable(db, refTableName);
    if (refTable == NULL) {
        printf("Error: Referenced table '%s' does not exist\n", refTableName);
        return false;
    }

    // 检查引用字段是否存在且是主键
    bool refFieldFound = false;
    for (int i = 0; i < refTable->fieldCount; i++) {
        if (strcmp(refTable->fields[i].name, refFieldName) == 0) {
            refFieldFound = true;
            if (!refTable->fields[i].isPrimaryKey) {
                printf("Error: Referenced field '%s' in table '%s' must be a primary key\n",
                    refFieldName, refTableName);
                return false;
            }
            break;
        }
    }

    if (!refFieldFound) {
        printf("Error: Referenced field '%s' does not exist in table '%s'\n",
            refFieldName, refTableName);
        return false;
    }

    // 检查字段类型是否匹配
    Table* table = findTable(db, tableName);
    if (table == NULL) {
        printf("Error: Table '%s' does not exist\n", tableName);
        return false;
    }

    // 查找源字段
    Field* field = NULL;
    for (int i = 0; i < table->fieldCount; i++) {
        if (strcmp(table->fields[i].name, fieldName) == 0) {
            field = &table->fields[i];
            break;
        }
    }

    if (field == NULL) {
        printf("Error: Field '%s' does not exist in table '%s'\n", fieldName, tableName);
        return false;
    }

    // 查找目标字段
    Field* refField = NULL;
    for (int i = 0; i < refTable->fieldCount; i++) {
        if (strcmp(refTable->fields[i].name, refFieldName) == 0) {
            refField = &refTable->fields[i];
            break;
        }
    }

    // 检查类型是否匹配
    if (field->type != refField->type) {
        printf("Error: Field type mismatch. Field '%s' is %d but referenced field '%s' is %d\n",
            fieldName, field->type, refFieldName, refField->type);
        return false;
    }

    return true;
}