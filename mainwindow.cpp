#include "mainwindow.h"
#include "database.h"
#include "sqlparser.h"
#include "storageengine.h"
#include "exceptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSplitter>
#include <QDir>

QMap<QString, Database*> databases;  // 全局数据库列表

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    sqlEditor = nullptr;
    resultView = nullptr;
    dbCombo = nullptr;
    useDbBtn = nullptr;
    setupUI();
    loadExistingDatabases(); // 新增初始化加载
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 数据库选择栏
    QHBoxLayout* dbLayout = new QHBoxLayout();
    dbCombo = new QComboBox();
    useDbBtn = new QPushButton("Use Database");
    connect(useDbBtn, &QPushButton::clicked, this, &MainWindow::switchDatabase);
    dbLayout->addWidget(new QLabel("Database:"));
    dbLayout->addWidget(dbCombo);
    dbLayout->addWidget(useDbBtn);

    // SQL编辑区
    sqlEditor = new QTextEdit();
    sqlEditor->setPlaceholderText("Enter SQL here...");

    // 执行按钮
    QPushButton* executeBtn = new QPushButton("Execute");
    connect(executeBtn, &QPushButton::clicked, this, &MainWindow::executeSql);

    // 结果显示
    resultView = new QTableView();

    // 组装布局
    mainLayout->addLayout(dbLayout);
    mainLayout->addWidget(sqlEditor);
    mainLayout->addWidget(executeBtn);
    mainLayout->addWidget(resultView);

    setCentralWidget(centralWidget);
    resize(800, 600);

    // 新增：左侧数据库面板
    QSplitter* splitter = new QSplitter(Qt::Horizontal, centralWidget);

    // 左侧面板
    QWidget* leftPanel = new QWidget;
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);

    // 数据库列表
    QLabel* dbLabel = new QLabel("数据库列表:");
    tableList = new QListWidget; // 需要添加成员变量 QListWidget* tableList;
    leftLayout->addWidget(dbLabel);
    leftLayout->addWidget(dbCombo);
    leftLayout->addWidget(useDbBtn);
    leftLayout->addWidget(tableList);

    // 右侧主区域
    QWidget* rightPanel = new QWidget;
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->addWidget(sqlEditor);
    rightLayout->addWidget(executeBtn);
    rightLayout->addWidget(resultView);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes({200, 600}); // 左侧宽度200px

    mainLayout->addWidget(splitter);
}

// 数据库命令窗口
void MainWindow::executeSql() {
    // 获取输入内容
    QString sql = sqlEditor->toPlainText().trimmed();
    if (sql.isEmpty()) return;

    try {
        // 按分号分割语句
        QStringList statements = sql.split(';', Qt::SkipEmptyParts);

        foreach (const QString& stmt, statements) {
            QString cleanStmt = stmt.trimmed();
            if (cleanStmt.isEmpty()) continue;

            // 解析语句类型
            // 建库
            if (cleanStmt.startsWith("CREATE DATABASE", Qt::CaseInsensitive)) {
                QString dbName = SqlParser::parseCreateDatabase(cleanStmt); //获取库名
                StorageEngine::createDatabase(dbName); // 库的内存
                databases[dbName] = new Database(dbName); // 数据库新对象

               // 更新下拉列表并自动选中
                dbCombo->addItem(dbName);
                dbCombo->setCurrentText(dbName);
                currentDB = dbName; // 自动切换到新数据库

                QMessageBox::information(this, "Success", "Database created");
            }
            // 建表
            else if (cleanStmt.startsWith("CREATE TABLE", Qt::CaseInsensitive)) {
                if (currentDB.isEmpty()) throw SqlException("No database selected");

                CreateTableStatement tableStmt = SqlParser::parseCreateTable(cleanStmt);
                databases[currentDB]->createTable(tableStmt.tableName, tableStmt.columns);
                StorageEngine::createTable(currentDB, tableStmt.tableName, tableStmt.columns);
                QMessageBox::information(this, "Success", "Table created");

            }
            // 查询select
            else if (cleanStmt.startsWith("SELECT", Qt::CaseInsensitive)) {
                QRegularExpression selectRegex(
                    "SELECT\\s+\\*\\s+FROM\\s+(\\w+)",
                    QRegularExpression::CaseInsensitiveOption);

                QRegularExpressionMatch match = selectRegex.match(cleanStmt);
                if (!match.hasMatch()) throw SqlException("Invalid SELECT syntax");

                QString tableName = match.captured(1);
                QList<QVariantMap> results = StorageEngine::selectAll(currentDB, tableName);

                // 显示结果到TableView
                QStandardItemModel* model = new QStandardItemModel(this);
                if (!results.isEmpty()) {
                    // 添加表头
                    QStringList headers = results.first().keys();
                    model->setHorizontalHeaderLabels(headers);

                    // 填充数据
                    for (int row = 0; row < results.size(); ++row) {
                        const QVariantMap& rowData = results[row];
                        int col = 0;
                        foreach (const QString& key, headers) {
                            model->setItem(row, col++,
                                           new QStandardItem(rowData[key].toString()));
                        }
                    }
                }
                resultView->setModel(model);

            }
            // 选择数据库
            else if(cleanStmt.startsWith("USE DATABASE", Qt::CaseInsensitive)){
                QString dbName = SqlParser::parseUseDatabase(cleanStmt);

                if (!databases.contains(dbName)) {
                    throw SqlException("Database does not exist: " + dbName);
                }

                currentDB = dbName;
                dbCombo->setCurrentText(dbName);
                refreshTableList();
                QMessageBox::information(this, "Success", "Switched to database: " + dbName);
            }
            // 删除数据库
            else if(cleanStmt.startsWith("DROP DATABASE", Qt::CaseInsensitive)){
                QString dbName = SqlParser::parseDropDatabase(cleanStmt);

                if (!databases.contains(dbName)) {
                    throw SqlException("Database does not exist: " + dbName);
                }

                // 删除文件
                //StorageEngine::deleteDatabase(dbName);

                // 清理内存对象
                delete databases[dbName];
                databases.remove(dbName);

                // 更新UI
                int index = dbCombo->findText(dbName);
                if (index != -1) {
                    dbCombo->removeItem(index);
                }

                // 如果当前数据库是被删除的库
                if (currentDB == dbName) {
                    currentDB.clear();
                    refreshTableList(); // 清空表列表
                }

                QMessageBox::information(this, "Success", "Database dropped: " + dbName);
            }
            // 删除表
            else if(cleanStmt.startsWith("DROP TABLE", Qt::CaseInsensitive)){
                if (currentDB.isEmpty()) {
                    throw SqlException("No database selected");
                }

                QString tableName = SqlParser::parseDropTable(cleanStmt);
                Database* db = databases.value(currentDB);

                if (!db->hasTable(tableName)) {
                    throw SqlException("Table does not exist: " + tableName);
                }

                // 删除内存中的表
                //db->dropTable(tableName);
                // 删除物理文件
                //StorageEngine::deleteTable(currentDB, tableName);

                refreshTableList();
                QMessageBox::information(this, "Success", "Table dropped: " + tableName);
            }

            else {
                throw SqlException("Unsupported SQL statement");
            }
        }
    } catch (const SqlException& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::switchDatabase() {
    if (dbCombo->currentIndex() == -1) {
        QMessageBox::warning(this, "Error", "请先选择数据库");
        return;
    }

    currentDB = dbCombo->currentText();
    QMessageBox::information(this, "Database",
                             "已切换到数据库: " + currentDB);

    // 刷新表列表
    refreshTableList();
}

void MainWindow::refreshTableList() {
    if (tableList) {
        tableList->clear();
        Database* db = databases.value(currentDB, nullptr);
        if (db) {
            tableList->addItems(db->tableNames());
        }
    }
}

// 扫描数据库目录
void MainWindow::loadExistingDatabases() {
    QDir dir;
    foreach (const QString& dbName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (QFile::exists(dbName + "/.dbinfo")) {
            databases[dbName] = new Database(dbName);
            dbCombo->addItem(dbName);
        }
    }
}
