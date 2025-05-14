#include "database.h"

/**
 * ��Լ��ת��Ϊ�ɶ��ַ���
 * @param constraint Լ��ָ��
 * @return Լ�����ַ�����ʾ
 */
const char* constraintToString(const Constraint* constraint) {
    static char buf[256];  // ��̬�����������ڷ����ַ���

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
 * ���ֶ����Լ��
 * @param field �ֶ�ָ��
 * @param type Լ������
 * @param name Լ������
 * @param refTable ������ñ���(FOREIGN_KEYר��)
 * @param refField ��������ֶ���(FOREIGN_KEYר��)
 * @param checkCondition �������(CHECKר��)
 * @return �ɹ�����true��ʧ�ܷ���false
 */
bool addConstraint(Field* field, ConstraintType type, const char* name,
    const char* refTable, const char* refField,
    const char* checkCondition) {

    // ���Լ����������
    if (field->constraintCount >= MAX_CONSTRAINTS) {
        printf("Error: Maximum number of constraints reached for field %s\n", field->name);
        return false;
    }

    // ���Լ�����Ƿ��Ѵ���
    for (int i = 0; i < field->constraintCount; i++) {
        if (strcmp(field->constraints[i].name, name) == 0) {
            printf("Error: Constraint name '%s' already exists for field %s\n", name, field->name);
            return false;
        }
    }

    // ��ʼ����Լ��
    Constraint* newConstraint = &field->constraints[field->constraintCount];
    newConstraint->type = type;
    strncpy_s(newConstraint->name, name, MAX_NAME_LEN);

    // �����ض�Լ�����͵Ķ������
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
 * ���ֶ�ɾ��Լ��
 * @param field �ֶ�ָ��
 * @param name Ҫɾ����Լ����
 * @return �ɹ�����true��ʧ�ܷ���false
 */
bool deleteConstraint(Field* field, const char* name) {
    int index = -1;

    // ����Լ��λ��
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

    // �ƶ�����Լ����ǰ���Ǳ�ɾ����Լ��
    for (int i = index; i < field->constraintCount - 1; i++) {
        field->constraints[i] = field->constraints[i + 1];
    }

    field->constraintCount--;
    printf("Constraint '%s' deleted from field '%s'\n", name, field->name);
    return true;
}

/**
 * ��֤���Լ���Ƿ���Ч
 * @param db ���ݿ�ָ��
 * @param tableName ����
 * @param fieldName �ֶ���
 * @param refTableName ���ñ���
 * @param refFieldName �����ֶ���
 * @return ��Ч����true����Ч����false
 */
bool validateForeignKey(Database* db, const char* tableName, const char* fieldName,
    const char* refTableName, const char* refFieldName) {

    // ������ñ��Ƿ����
    Table* refTable = findTable(db, refTableName);
    if (refTable == NULL) {
        printf("Error: Referenced table '%s' does not exist\n", refTableName);
        return false;
    }

    // ��������ֶ��Ƿ������������
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

    // ����ֶ������Ƿ�ƥ��
    Table* table = findTable(db, tableName);
    if (table == NULL) {
        printf("Error: Table '%s' does not exist\n", tableName);
        return false;
    }

    // ����Դ�ֶ�
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

    // ����Ŀ���ֶ�
    Field* refField = NULL;
    for (int i = 0; i < refTable->fieldCount; i++) {
        if (strcmp(refTable->fields[i].name, refFieldName) == 0) {
            refField = &refTable->fields[i];
            break;
        }
    }

    // ��������Ƿ�ƥ��
    if (field->type != refField->type) {
        printf("Error: Field type mismatch. Field '%s' is %d but referenced field '%s' is %d\n",
            fieldName, field->type, refFieldName, refField->type);
        return false;
    }

    return true;
}