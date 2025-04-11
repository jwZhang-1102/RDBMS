#include "database.h"

/**
 * ���ݱ������ұ�
 * @param db ���ݿ�ָ��
 * @param tableName Ҫ���ҵı���
 * @return �ҵ����ر�ָ�룬���򷵻�NULL
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
 * �������ֶ�
 * @param table ��ָ��
 * @param name �ֶ���
 * @param type �ֶ�����
 * @param isPrimaryKey �Ƿ�����
 * @param length �ֶγ���(�ַ�������ʹ��)
 * @return �ɹ�����true��ʧ�ܷ���false
 */
bool addField(Table* table, const char* name, FieldType type, bool isPrimaryKey, int length) {
    // ����ֶ���������
    if (table->fieldCount >= MAX_FIELDS) {
        printf("Error: Maximum number of fields reached for table %s\n", table->name);
        return false;
    }

    // ����ֶ����Ƿ��Ѵ���
    for (int i = 0; i < table->fieldCount; i++) {
        if (strcmp(table->fields[i].name, name) == 0) {
            printf("Error: Field name '%s' already exists in table %s\n", name, table->name);
            return false;
        }
    }

    // ��ʼ�����ֶ�
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
 * ɾ�����е��ֶ�
 * @param table ��ָ��
 * @param fieldName Ҫɾ�����ֶ���
 * @return �ɹ�����true��ʧ�ܷ���false
 */
bool deleteField(Table* table, const char* fieldName) {
    int index = -1;

    // �����ֶ�λ��
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

    // ɾ���ֶΣ�ͨ�����Ǻ����ֶ�ʵ��
    for (int i = index; i < table->fieldCount - 1; i++) {
        table->fields[i] = table->fields[i + 1];
    }

    table->fieldCount--;
    printf("Field '%s' deleted from table '%s'\n", fieldName, table->name);
    return true;
}

/**
 * �޸ı��е��ֶ�����
 * @param table ��ָ��
 * @param fieldName Ҫ�޸ĵ��ֶ���
 * @param newType �µ���������
 * @param newIsPrimaryKey �Ƿ�Ϊ����
 * @param newLength �³���(�����ַ���������Ч)
 * @return �ɹ�����true��ʧ�ܷ���false
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

    // �޸��ֶ�����
    field->type = newType;
    field->isPrimaryKey = newIsPrimaryKey;
    field->length = newLength;

    printf("Field '%s' in table '%s' modified\n", fieldName, table->name);
    return true;
}
