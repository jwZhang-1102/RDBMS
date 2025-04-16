#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QTreeWidgetItem>

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
    void setupInLeftWidget(QWidget *leftWidget);
    void createTable(QString dbName, QString tableName, QStringList attributes);
    void setupInRightWidget(QWidget *rightWidget);
    void insertIntoTable(QString dbName, QString tableName, QStringList tuples);
    void showTable(QString dbName, QString tableName);
    void dropTableTEST(QString dbName, QString tableName);

private slots:
    void onTreeItemClicked(QTreeWidgetItem *item, int column);

private:
    QStringList databases;// = {"testDataBase"};
    QStringList tables;// = {"testTable"};

    QStringList attributes;

    QWidget *leftWidget;
    QWidget *rightWidget;

    int rowNum = 0;
    int columnNum = 0;
    QStringList columnNames = {"ID", "Name", "Salary", "DOB","Position"};
};

#endif // MAINWINDOW_H
