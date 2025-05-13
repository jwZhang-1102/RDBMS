#include "dbserver.h"
#include "sqlparser.h"
#include "storageengine.h"
#include "exceptions.h"
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "table.h"

DBServer::DBServer(QObject* parent) : QTcpServer(parent) {}

// 服务端处理功能
void DBServer::incomingConnection(qintptr socketDescriptor) {
    QThread* thread = new QThread(this);
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    socket->moveToThread(thread);

    connect(socket, &QTcpSocket::readyRead, this, &DBServer::processRequest);
    connect(socket, &QTcpSocket::disconnected, this, &DBServer::handleDisconnected);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, socket, &QTcpSocket::deleteLater);

    thread->start();
}

void DBServer::processRequest() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    while (client->canReadLine()) {
        QString command = QString::fromUtf8(client->readLine()).trimmed();
        QString response = executeCommand(client, command);
        client->write(response.toUtf8() + "\n");
    }
}

// 获取客户端输入并执行SQL语句后返回结果
QString DBServer::executeCommand(QTcpSocket* client, const QString& sql) {
    QMutexLocker locker(&mutex);
    try {
        QString operation = sql.section(' ', 0, 0, QString::SectionSkipEmpty).toUpper();

        if (operation == "CREATE") {
            if (sql.contains(" DATABASE ", Qt::CaseInsensitive)) {
                QString dbName = SqlParser::parseCreateDatabase(sql);
                StorageEngine::createDatabase(dbName);
                databases.insert(dbName, new Database(dbName));
                QJsonObject responseObj;
                responseObj["status"] = "OK";
                responseObj["message"] = "Database created: " + dbName;
                return QJsonDocument(responseObj).toJson(QJsonDocument::Compact);
            } else if (sql.contains(" TABLE ", Qt::CaseInsensitive)) {
                CreateTableStatement stmt = SqlParser::parseCreateTable(sql);
                QMutexLocker dbLocker(&m_clientDBMutex);
                QString currentDB = m_clientCurrentDB.value(client, "");
                if (currentDB.isEmpty()) throw SqlException("No database selected");
                Database* db = databases.value(currentDB);
                if (!db) throw SqlException("Database not found");
                db->createTable(stmt.tableName, stmt.columns);
                QJsonObject responseObj;
                responseObj["status"] = "OK";
                responseObj["message"] = "Table created: " + stmt.tableName;
                return QJsonDocument(responseObj).toJson(QJsonDocument::Compact);
            }
        } else if (operation == "USE") {
            QString dbName = SqlParser::parseUseDatabase(sql);
            if (!databases.contains(dbName)) throw SqlException("Database not found");
            QMutexLocker dbLocker(&m_clientDBMutex);
            m_clientCurrentDB[client] = dbName;
            QJsonObject responseObj;
            responseObj["status"] = "OK";
            responseObj["message"] = "Using database: " + dbName;
            return QJsonDocument(responseObj).toJson(QJsonDocument::Compact);
        } else if (operation == "INSERT") {
            // 解析INSERT语句并插入数据
            QRegularExpression insertRegex(R"(INSERT INTO (\w+) \((.+?)\) VALUES \((.+?)\))", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = insertRegex.match(sql);
            if (!match.hasMatch()) throw SqlException("Invalid INSERT syntax");
            QString tableName = match.captured(1);
            QStringList columns = match.captured(2).split(',', Qt::SkipEmptyParts);
            QStringList values = match.captured(3).split(',', Qt::SkipEmptyParts);

            QMutexLocker dbLocker(&m_clientDBMutex);
            QString currentDB = m_clientCurrentDB.value(client, "");
            if (currentDB.isEmpty()) throw SqlException("No database selected");
            Database* db = databases.value(currentDB);
            if (!db) throw SqlException("Database not found");
            Table* table = db->getTable(tableName);
            if (!table) throw SqlException("Table not found");

            QVariantMap data;
            for (int i = 0; i < columns.size(); ++i) {
                data[columns[i].trimmed()] = values[i].trimmed().replace("'", "");
            }
            table->insert(data);

            QJsonObject responseObj;
            responseObj["status"] = "OK";
            responseObj["message"] = "Insert successful";
            return QJsonDocument(responseObj).toJson(QJsonDocument::Compact);
        } else if (operation == "SELECT") {
            QRegularExpression selectRegex(R"(SELECT \* FROM (\w+))", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = selectRegex.match(sql);
            if (!match.hasMatch()) throw SqlException("Invalid SELECT syntax");
            QString tableName = match.captured(1);

            QMutexLocker dbLocker(&m_clientDBMutex);
            QString currentDB = m_clientCurrentDB.value(client, "");
            if (currentDB.isEmpty()) throw SqlException("No database selected");
            Database* db = databases.value(currentDB);
            if (!db) throw SqlException("Database not found");
            Table* table = db->getTable(tableName);
            if (!table) throw SqlException("Table not found");

            QList<QVariantMap> results = table->selectAll();
            QJsonArray jsonArray;
            for (const QVariantMap& row : results) {
                QJsonObject jsonRow;
                for (auto it = row.begin(); it != row.end(); ++it) {
                    jsonRow[it.key()] = QJsonValue::fromVariant(it.value());
                }
                jsonArray.append(jsonRow);
            }
            return QJsonDocument(jsonArray).toJson(QJsonDocument::Compact);
        } else {
            throw SqlException("Unsupported operation");
        }
    } catch (const SqlException& e) {
        QJsonObject errorObj;
        errorObj["error"] = e.what();
        return QJsonDocument(errorObj).toJson(QJsonDocument::Compact);
    }
}

// 断开连接
void DBServer::handleDisconnected() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        QMutexLocker locker(&m_clientDBMutex);
        m_clientCurrentDB.remove(client);
        client->deleteLater();
        QThread::currentThread()->quit();
    }
}
