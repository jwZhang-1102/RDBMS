#include "sqlparser.h"
#include "exceptions.h"

// 建立表
CreateTableStatement SqlParser::parseCreateTable(const QString& sql) {
    CreateTableStatement stmt;

    QRegularExpression regex(
        "CREATE\\s+TABLE\\s+(\\w+)\\s*\\((.+)\\)",
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid CREATE TABLE syntax");
    }

    stmt.tableName = match.captured(1);
    QString columnsStr = match.captured(2);

    foreach (const QString& column, columnsStr.split(',')) {
        QStringList parts = column.trimmed().split(' ');
        if (parts.size() < 2) continue;
        stmt.columns.append(qMakePair(parts[0], parts[1].toUpper()));
    }

    return stmt;
}

// 建立数据库
QString SqlParser::parseCreateDatabase(const QString& sql) {
    QRegularExpression regex(
        "CREATE\\s+DATABASE\\s+(\\w+)",
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid CREATE DATABASE syntax");
    }

    return match.captured(1);
}

// 选择数据库
QString SqlParser::parseUseDatabase(const QString& sql) {
    QRegularExpression regex(
        "USE\\s+DATABASE\\s+(\\w+)",
        QRegularExpression::CaseInsensitiveOption
        );

    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid USE DATABASE syntax");
    }
    return match.captured(1);
}

// 完善DROP DATABASE解析
QString SqlParser::parseDropDatabase(const QString& sql) {
    QRegularExpression regex(
        R"(DROP\s+DATABASE\s+(\w+)\s*;?)",
        QRegularExpression::CaseInsensitiveOption
        );

    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid DROP DATABASE syntax");
    }
    return match.captured(1).trimmed();
}

// 完善DROP TABLE解析
QString SqlParser::parseDropTable(const QString& sql) {
    QRegularExpression regex(
        R"(DROP\s+TABLE\s+(\w+)\s*;?)",
        QRegularExpression::CaseInsensitiveOption
        );

    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid DROP TABLE syntax");
    }
    return match.captured(1).trimmed();
}

// 新增备份命令解析
QString SqlParser::parseBackupDatabase(const QString& sql) {
    QRegularExpression regex(
        R"(BACKUP\s+DATABASE\s+(\w+)\s*;?)",
        QRegularExpression::CaseInsensitiveOption
        );

    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid BACKUP syntax. Usage: BACKUP DATABASE <dbname>");
    }
    return match.captured(1).trimmed();
}

// 新增恢复命令解析
QString SqlParser::parseRestoreDatabase(const QString& sql) {
    QRegularExpression regex(
        R"(RESTORE\s+DATABASE\s+(\w+)\s*;?)",
        QRegularExpression::CaseInsensitiveOption
        );

    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid RESTORE syntax. Usage: RESTORE DATABASE <dbname>");
    }
    return match.captured(1).trimmed();
}

// 新增查看日志命令解析
bool SqlParser::parseShowLog(const QString& sql) {
    QRegularExpression regex(
        R"(SHOW_LOG\s*;?)", // 匹配SHOW_LOG或SHOW_LOG;
        QRegularExpression::CaseInsensitiveOption
        );
    return regex.match(sql).hasMatch();
}


