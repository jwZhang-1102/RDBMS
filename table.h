#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QFile>
#include <QDataStream>

class Table : public QObject {
    Q_OBJECT
public:
    explicit Table(const QString& name,
                   const QList<QPair<QString, QString>>& columns,
                   QObject* parent = nullptr);

    explicit Table(const QString& name,
                   const QStringList& columns,
                   QObject* parent = nullptr);

    void insert(const QVariantMap& data);
    QList<QVariantMap> selectAll() const;

    // ======================== [索引功能新增] ========================
    void createIndex(const QString& indexName, const QString& fieldName);
    bool hasIndex(const QString& indexName) const;
    void dropIndex(const QString& indexName);
    QString indexField(const QString& indexName) const;
    const QMap<QString, QString>& getIndexMap() const { return indices; }
    // ===============================================================

private:
    QString name;
    QList<QPair<QString, QString>> columns;

    // ======================== [索引功能新增] ========================
    QMap<QString, QString> indices; // 索引名 -> 字段名映射
    QMap<QString, QMap<QString, QString>> indexData; // 字段值 -> 主键，模拟哈希索引
    // ===============================================================
};

#endif
