#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private:

};

#endif // MAINWINDOW_H
