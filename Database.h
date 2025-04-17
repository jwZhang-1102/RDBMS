#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ���������󳤶�����
#define MAX_NAME_LEN 50       // ������󳤶�
#define MAX_FIELDS 20         // ÿ��������ֶ���
#define MAX_TABLES 10         // ���ݿ�������
#define MAX_CONSTRAINTS 10    // ÿ���ֶ����Լ����

// �ֶ�����ö�٣�����֧�ֵ��ֶ���������
typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    BOOL_TYPE
} FieldType;

// Լ������ö�٣�����֧�ֵ�Լ������
typedef enum {
    NOT_NULL,     // �ǿ�Լ��
    UNIQUE,       // ΨһԼ��
    DEFAULT,      // Ĭ��ֵԼ��
    CHECK,        // ���Լ��
    FOREIGN_KEY   // ���Լ��
} ConstraintType;

// Լ�������ṹ��
typedef struct {
    ConstraintType type;      // Լ������
    char name[MAX_NAME_LEN];  // Լ������

    // �����壬���ڴ洢��ͬ���͵�Ĭ��ֵ(��DEFAULTԼ��ʹ��)
    union {
        int intValue;        // ����Ĭ��ֵ
        float floatValue;     // ������Ĭ��ֵ
        char stringValue[MAX_NAME_LEN]; // �ַ���Ĭ��ֵ
        bool boolValue;       // ������Ĭ��ֵ
    } defaultValue;

    // ���Լ��ר���ֶ�
    char refTable[MAX_NAME_LEN];   // ���ñ���
    char refField[MAX_NAME_LEN];   // �����ֶ���

    // ���Լ��ר���ֶ�
    char checkCondition[MAX_NAME_LEN * 2]; // ����������ʽ
} Constraint;

// �ֶνṹ��
typedef struct {
    char name[MAX_NAME_LEN];       // �ֶ�����
    FieldType type;                // �ֶ�����
    bool isPrimaryKey;             // �Ƿ�����
    int length;                    // �ֶγ���(��Ҫ�����ַ�������)

    Constraint constraints[MAX_CONSTRAINTS]; // �ֶ�Լ������
    int constraintCount;           // ��ǰԼ������
} Field;

// ��ṹ��
typedef struct {
    char name[MAX_NAME_LEN];       // ������
    Field fields[MAX_FIELDS];      // �ֶ�����
    int fieldCount;                // ��ǰ�ֶ�����
} Table;

// ���ݿ�ṹ��
typedef struct {
    Table tables[MAX_TABLES];      // ������
    int tableCount;                // ��ǰ������
} Database;

// ��������
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