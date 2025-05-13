#include "connectdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonDocument>

ConnectDialog::ConnectDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("连接服务器");
    QVBoxLayout* layout = new QVBoxLayout(this);

    hostEdit = new QLineEdit("127.0.0.1");
    portEdit = new QLineEdit("12345");
    connectBtn = new QPushButton("连接");

    layout->addWidget(new QLabel("服务器地址:"));
    layout->addWidget(hostEdit);
    layout->addWidget(new QLabel("端口号:"));
    layout->addWidget(portEdit);
    layout->addWidget(connectBtn);

    connect(connectBtn, &QPushButton::clicked, this, &ConnectDialog::attemptConnection);
}

void ConnectDialog::attemptConnection() {
    // 直接触发参数信号
    emit connectionParamsRequested(getHost(), getPort());
}
