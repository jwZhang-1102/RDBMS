#ifndef DBCLIENT_H
#define DBCLIENT_H

#include <QTcpSocket>
#include <QJsonDocument>

class DBClient : public QObject {
    Q_OBJECT
public:
    explicit DBClient(QObject* parent = nullptr);

    // 连接管理
    void connectToServer(const QString& host, quint16 port);
    void disconnect();
    bool isConnected() const;

    // 查询操作
    void executeSQL(const QString& sql);

signals:
    void connectionStatusChanged(bool connected);
    void queryResultReceived(const QJsonDocument& result);
    void errorOccurred(const QString& error);

private slots:
    void handleSocketConnected();
    void handleSocketDisconnected();
    void readResponse();

private:
    QTcpSocket m_socket;
    void setupConnections();
};

#endif // DBCLIENT_H
