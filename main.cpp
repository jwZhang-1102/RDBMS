#include "widget.h"
#include "logui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogUI *log = new LogUI();
    log->show();
    return a.exec();
}
