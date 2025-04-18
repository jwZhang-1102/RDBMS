#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QMap>
#include <QStringList>

class Table;

class Database {
public:
    explicit Database(const QString& name);
    // 建立表
    void createTable(const QString& name,
                     const QList<QPair<QString, QString>>& columns);
    // 获取表名
    Table* getTable(const QString& name);
    bool hasTable(const QString& name);
    ~Database();

    QStringList tableNames() const;

private:
    QString name;
    QMap<QString, Table*> tables;
};

#endif // DATABASE_H
