#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>

class QTextEdit;
class QTableView;
class QPushButton;
class QLabel;
class DBClient;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void executeQuery();
    void showConnectionDialog();
    void handleQueryResult(const QJsonDocument& result);
    void handleError(const QString& error);
    void updateConnectionStatus(bool connected);

    // 日志和备份
    void restoreBackup();
    void viewLog();

private:
    void setupUI();
    void setupConnections();

    // UI Components
    QTextEdit* m_sqlEditor;
    QTableView* m_resultView;
    QPushButton* m_executeBtn;
    QLabel* m_statusLabel;
    QPushButton* m_restoreBtn;
    QPushButton* m_viewLogBtn;

    // Core
    DBClient* m_dbClient;
};

#endif // MAINWINDOW_H
