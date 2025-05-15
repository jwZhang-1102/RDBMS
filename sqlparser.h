#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>

struct CreateTableStatement {
    QString tableName;
    QList<QPair<QString, QString>> columns;
};

class SqlParser {
public:
    // 原有方法
    static CreateTableStatement parseCreateTable(const QString& sql); // 建表
    static QString parseCreateDatabase(const QString& sql); // 建库

    static QString parseUseDatabase(const QString& sql);    // 选择数据库
    static QString parseDropDatabase(const QString& sql);  // 删除数据库
    static QString parseDropTable(const QString& sql);      // 删除表

    // 新增备份恢复和日志相关解析方法
    static QString parseBackupDatabase(const QString& sql);
    static QString parseRestoreDatabase(const QString& sql);
    static bool parseShowLog(const QString& sql);
};

#endif // SQLPARSER_H
