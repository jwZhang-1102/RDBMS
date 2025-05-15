#include "dbclient.h"
#include <QDebug>
#include <QJsonObject>

DBClient::DBClient(QObject* parent) : QObject(parent) {
    setupConnections();
}

void DBClient::setupConnections() {
    connect(&m_socket, &QTcpSocket::connected,
            this, &DBClient::handleSocketConnected);
    connect(&m_socket, &QTcpSocket::disconnected,
            this, &DBClient::handleSocketDisconnected);
    connect(&m_socket, &QTcpSocket::readyRead,
            this, &DBClient::readResponse);
    connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            [this](QAbstractSocket::SocketError error) {
                emit errorOccurred(m_socket.errorString());
            });
}

void DBClient::connectToServer(const QString& host, quint16 port) {
    if(m_socket.state() != QAbstractSocket::UnconnectedState) return;

    qDebug() << "Connecting to" << host << ":" << port;
    m_socket.connectToHost(host, port);
}

void DBClient::disconnect() {
    if(m_socket.state() == QAbstractSocket::ConnectedState) {
        m_socket.disconnectFromHost();
    }
}

bool DBClient::isConnected() const {
    return m_socket.state() == QAbstractSocket::ConnectedState;
}

void DBClient::executeSQL(const QString& sql) {
    if(!isConnected()) {
        emit errorOccurred("Not connected to server");
        return;
    }

    const QByteArray data = sql.toUtf8() + '\n';
    m_socket.write(data);
    qDebug() << "Sent SQL:" << sql;
}

void DBClient::handleSocketConnected() {
    qDebug() << "Connected to server";
    emit connectionStatusChanged(true);
}

void DBClient::handleSocketDisconnected() {
    qDebug() << "Disconnected from server";
    emit connectionStatusChanged(false);
}

void DBClient::readResponse() {
    while(m_socket.canReadLine()) {
        QByteArray rawData = m_socket.readLine().trimmed();
        qDebug() << "Raw response:" << rawData;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(rawData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {

            if (rawData.endsWith("}") == false) {
                rawData += "}";
                doc = QJsonDocument::fromJson(rawData, &parseError);
            }
            if (parseError.error != QJsonParseError::NoError) {
                emit errorOccurred("Invalid JSON: " + parseError.errorString());
                continue;
            }
        }

        if(doc.isObject() && doc.object().contains("error")) {
            emit errorOccurred(doc.object()["error"].toString());
        } else {
            emit queryResultReceived(doc);
        }
    }
}
