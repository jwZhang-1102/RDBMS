#include "mainwindow.h"
#include "dbclient.h"
#include "connectdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QHeaderView>
#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextEdit>
#include <QTableView>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_dbClient(new DBClient(this))
{
    setupUI();
    setupConnections();

    // 直接连接
    m_dbClient->connectToServer("127.0.0.1", 12345);

    // 输入端口号连接
    //showConnectionDialog();
}

void MainWindow::setupUI() {
    // 主容器
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);

    // SQL编辑区
    m_sqlEditor = new QTextEdit();
    m_sqlEditor->setPlaceholderText("输入SQL语句...");
    m_sqlEditor->setFont(QFont("Consolas", 12));

    // 执行按钮
    m_executeBtn = new QPushButton("执行查询");
    m_executeBtn->setEnabled(false);

    // 结果展示
    m_resultView = new QTableView();
    m_resultView->setSelectionBehavior(QTableView::SelectRows);
    m_resultView->horizontalHeader()->setStretchLastSection(true);

    // 布局安排
    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->addWidget(m_sqlEditor);
    topBar->addWidget(m_executeBtn);

    mainLayout->addLayout(topBar);
    mainLayout->addWidget(m_resultView);

    setCentralWidget(mainWidget);

    // 状态栏
    m_statusLabel = new QLabel("未连接");
    statusBar()->addPermanentWidget(m_statusLabel);

    // 日志和备份
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_restoreBtn = new QPushButton("恢复备份");
    m_viewLogBtn = new QPushButton("查看日志");
    buttonLayout->addWidget(m_restoreBtn);
    buttonLayout->addWidget(m_viewLogBtn);
    mainLayout->addLayout(buttonLayout);
}

void MainWindow::setupConnections() {
    connect(m_executeBtn, &QPushButton::clicked,
            this, &MainWindow::executeQuery);
    connect(m_dbClient, &DBClient::queryResultReceived,
            this, &MainWindow::handleQueryResult);
    connect(m_dbClient, &DBClient::errorOccurred,
            this, &MainWindow::handleError);
    connect(m_dbClient, &DBClient::connectionStatusChanged,
            this, &MainWindow::updateConnectionStatus);
    // 日志和备份
    connect(m_restoreBtn, &QPushButton::clicked, this, &MainWindow::restoreBackup);
    connect(m_viewLogBtn, &QPushButton::clicked, this, &MainWindow::viewLog);
}

void MainWindow::showConnectionDialog() {
    ConnectDialog dlg;
    connect(&dlg, &ConnectDialog::connectionParamsRequested,
            m_dbClient, [this](const QString& host, quint16 port) {
                m_dbClient->connectToServer(host, port);
            });

    dlg.exec();
}

// sql查询返回图标可视化结果
void MainWindow::executeQuery() {
    const QString sql = m_sqlEditor->toPlainText().trimmed();
    if(!sql.isEmpty()) {
        m_dbClient->executeSQL(sql);
        statusBar()->showMessage("正在执行查询...", 2000);
    }
}

void MainWindow::handleQueryResult(const QJsonDocument& result) {
    QStandardItemModel* model = new QStandardItemModel(this);
    // 日志
    if (result.object().contains("log")) {
        QString logContent = result.object()["log"].toString();
        QDialog* logDialog = new QDialog(this);
        QTextEdit* logView = new QTextEdit(logDialog);
        logView->setPlainText(logContent);
        QVBoxLayout* layout = new QVBoxLayout(logDialog);
        layout->addWidget(logView);
        logDialog->resize(600, 400);
        logDialog->exec();
    }

    //  查询
    if (result.isArray()) {
        QJsonArray rows = result.array();
        if (!rows.isEmpty()) {
            // 处理表头
            QJsonObject firstRow = rows.first().toObject();
            QStringList headers = firstRow.keys();
            model->setHorizontalHeaderLabels(headers);

            // 处理数据
            for (const QJsonValue& row : rows) {
                QJsonObject obj = row.toObject();
                QList<QStandardItem*> items;
                for (const QString& header : headers) {
                    items << new QStandardItem(obj[header].toString());
                }
                model->appendRow(items);
            }
        }
    }

    m_resultView->setModel(model);
    m_resultView->resizeColumnsToContents();
    statusBar()->showMessage("查询完成", 2000);
}

void MainWindow::handleError(const QString& error) {
    statusBar()->showMessage("错误: " + error, 5000);
    QMessageBox::critical(this, "错误", error);
}

void MainWindow::updateConnectionStatus(bool connected) {
    m_statusLabel->setText(connected ? "已连接" : "未连接");
    m_executeBtn->setEnabled(connected);
    if(!connected) {
        m_resultView->setModel(nullptr);
    }
}

void MainWindow::restoreBackup() {
    m_dbClient->executeSQL("RESTORE");
}

void MainWindow::viewLog() {
    m_dbClient->executeSQL("SHOW_LOG");
}
