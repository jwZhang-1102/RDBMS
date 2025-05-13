#ifndef DBSERVER_H
#define DBSERVER_H

#include <QTcpServer>
#include <QMutex>
#include "database.h"
#include <QMap>

class DBServer : public QTcpServer {
    Q_OBJECT
public:
    explicit DBServer(QObject* parent = nullptr);
    void handleClientRequest(QTcpSocket* client);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void handleDisconnected();
    void processRequest();

private:

    QMutex mutex;
    QMap<QString, Database*> databases;
    QMap<QTcpSocket*, QString> m_clientCurrentDB;
    QMutex m_clientDBMutex;

    QString executeCommand(QTcpSocket* client, const QString& sql);
};

#endif // DBSERVER_H
