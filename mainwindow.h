#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QTreeWidgetItem>
#include <QKeyEvent>
#include <QDialog>
#include <QListWidget>
#include "security.h"

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    mainwindow(const QString &username, QWidget *parent = nullptr);
    ~mainwindow();

    void setupInLeftWidget(QWidget *leftWidget);
    void createDataBase(QString dbName);
    void createEmptyTable(QString dbName, QString tableName);
    void createTable(QString dbName, QString tableName, QStringList attributes);
    void setupInRightWidget(QWidget *rightWidget);
    void insertAttribute(QString dbName, QString tableName, QStringList attributes);
    void alterAttribute(QString dbName, QString tableName, QString attribute);
    void deleteAttribute(QString dbName, QString tableName, QString attribute);
    void insertIntoTable(QString dbName, QString tableName, QStringList tuples);
    void showTable(QString dbName, QString tableName);
    void dropDataBase(QString dbName);
    void dropTableTEST(QString dbName, QString tableName);
    void processDDL();
    void useDatabase(QString database);

    QString userName;
    QString dataBase;

private slots:
    void onTextChanged();
    void onTreeItemClicked(QTreeWidgetItem *item, int column);
    void onRefreshButtonClicked();
    void showPermissionManager();

private:
    QStringList databases;
    QStringList tables;
    QStringList attributes;
    QStringList attributesToDelete;
    QWidget *leftWidget;
    QWidget *rightWidget;
    int rowNum = 0;
    int columnNum = 0;
    QStringList columnNames = {"ID", "Name", "Salary", "DOB", "Position"};
    QPushButton *refreshButton;

    SecurityManager& securityManager;
    QString currentSelectedDb;

    // 权限管理对话框相关
    QDialog* permissionDialog;
    QListWidget* userListWidget;
    QListWidget* permissionListWidget;
    QPushButton* grantButton;
    QPushButton* revokeButton;

    void setupPermissionDialog();
    void updatePermissionList();
};

#endif // MAINWINDOW_H
