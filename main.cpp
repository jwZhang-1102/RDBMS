#include "dbserver.h"
#include <QCoreApplication>
#include <QApplication>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    DBServer server;
    if (!server.listen(QHostAddress::Any, 12345)) {
        qCritical() << "Failed to start server:" << server.errorString();
        return 1;
    }
    qInfo() << "Server started on port 12345";

    return a.exec();
}
