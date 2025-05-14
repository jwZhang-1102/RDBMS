#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <QString>
#include <QStringList>
#include <QRegularExpression>

struct CreateTableStatement {
    QString tableName;
    QList<QPair<QString, QString>> columns;
};

// ======================== [索引功能新增] ========================
struct CreateIndexStatement {
    QString indexName;
    QString tableName;
    QString fieldName;
};
// ===============================================================

class SqlParser {
public:
    // 原有方法
    static CreateTableStatement parseCreateTable(const QString& sql);
    static QString parseCreateDatabase(const QString& sql);
    static QString parseUseDatabase(const QString& sql);
    static QString parseDropDatabase(const QString& sql);
    static QString parseDropTable(const QString& sql);

    // ======================== [索引功能新增] ========================
    static CreateIndexStatement parseCreateIndex(const QString& sql);
    // ===============================================================
};

#endif // SQLPARSER_H
