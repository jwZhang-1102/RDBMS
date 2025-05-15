#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "dbclient.h"

class ConnectDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget* parent = nullptr);
    QString getHost() const { return hostEdit->text(); }
    quint16 getPort() const { return portEdit->text().toUShort(); }

signals:
    void connectionEstablished(DBClient* client); // 新增连接成功信号
    void connectionParamsRequested(const QString& host, quint16 port);

private slots:
    void attemptConnection();

private:
    QLineEdit* hostEdit;
    QLineEdit* portEdit;
    QPushButton* connectBtn;
};

#endif // CONNECTDIALOG_H
