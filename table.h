#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QMap>
#include <QVariant>

class Table : public QObject {
    Q_OBJECT
public:
    explicit Table(const QString& name,
                   const QList<QPair<QString, QString>>& columns,
                   QObject* parent = nullptr);

    void insert(const QVariantMap& data);
    QList<QVariantMap> selectAll() const;

private:
    QString name;
    QList<QPair<QString, QString>> columns;
};

#endif
