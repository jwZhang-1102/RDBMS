#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTableView>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QListWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void executeSql();
    void switchDatabase();
    void loadExistingDatabases();
    void refreshTableList();

private:
    QTextEdit* sqlEditor;
    QTableView* resultView;
    QComboBox* dbCombo;
    QPushButton* useDbBtn;
    QString currentDB;
    QListWidget* tableList;
    void setupUI();
};

#endif // MAINWINDOW_H
