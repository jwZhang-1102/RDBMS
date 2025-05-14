#include "sqlparser.h"
#include "exceptions.h"

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

QString SqlParser::parseCreateDatabase(const QString& sql) {
    QRegularExpression regex("CREATE\\s+DATABASE\\s+(\\w+)",
                             QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid CREATE DATABASE syntax");
    }
    return match.captured(1);
}

QString SqlParser::parseUseDatabase(const QString& sql) {
    QRegularExpression regex("USE\\s+DATABASE\\s+(\\w+)",
                             QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid USE DATABASE syntax");
    }
    return match.captured(1);
}

QString SqlParser::parseDropDatabase(const QString& sql) {
    QRegularExpression regex("DROP\\s+DATABASE\\s+(\\w+)",
                             QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid DROP DATABASE syntax");
    }
    return match.captured(1);
}

QString SqlParser::parseDropTable(const QString& sql) {
    QRegularExpression regex("DROP\\s+TABLE\\s+(\\w+)",
                             QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid DROP TABLE syntax");
    }
    return match.captured(1);
}

// ======================== [索引功能新增] ========================
CreateIndexStatement SqlParser::parseCreateIndex(const QString& sql) {
    QRegularExpression regex(
        "CREATE\\s+INDEX\\s+(\\w+)\\s+ON\\s+(\\w+)\\s*\\((\\w+)\\)",
        QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = regex.match(sql);
    if (!match.hasMatch()) {
        throw SqlException("Invalid CREATE INDEX syntax");
    }

    CreateIndexStatement stmt;
    stmt.indexName = match.captured(1);
    stmt.tableName = match.captured(2);
    stmt.fieldName = match.captured(3);
    return stmt;
}
// ===============================================================
