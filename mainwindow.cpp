#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QWidgetAction>
#include <QComboBox>
#include <QTextEdit>
#include <QSplitter>
#include <QTableWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>

mainwindow::mainwindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("My DBMS(1.0)");
    setFixedSize(1100, 700);

    QGridLayout *layout = new QGridLayout(this);

    QMenuBar *menuBar = new QMenuBar(this);

// 文件菜单
    QMenu *fileMenu = menuBar->addMenu("文件");

    QAction *newProjectAction = new QAction("新建项目...", this);
    QAction *newConnectionAction = new QAction("新建连接...", this);
    QAction *openByMyDBMSAction = new QAction("使用MyDBMS打开...", this);
    QAction *importConnectionAction = new QAction("导入连接...", this);
    QAction *derivedConnectionAction = new QAction("导出连接...", this);
    QAction *managementAction = new QAction("管理云...", this);
    QAction *closewindowAction = new QAction("关闭窗口", this);
    QAction *closeTabAction = new QAction("关闭选项卡", this);
    QAction *exitMyDBMSAction = new QAction("退出MyDBMS", this);

    QMenu *newMenu = new QMenu("新建", this);
    QAction *newAction1 = new QAction("新建文件", this);
    QAction *newAction2 = new QAction("新建文件夹", this);
    newMenu->addAction(newAction1);
    newMenu->addAction(newAction2);

    QMenu *openExternalFileMenu = new QMenu("打开外部文件", this);
    QAction *newAction3 = new QAction("查询...", this);
    QAction *newAction4 = new QAction("模型工作区...", this);
    QAction *newAction5 = new QAction("BI 工作区", this);
    openExternalFileMenu->addAction(newAction3);
    openExternalFileMenu->addAction(newAction4);
    openExternalFileMenu->addAction(newAction5);

    QMenu *openRecentlyUsedMenu = new QMenu("打开最近使用过的", this);
    QAction *newAction6 = new QAction("打开表-recentlyUsed", this);
    openRecentlyUsedMenu->addAction(newAction6);

    fileMenu->addAction(newProjectAction);
    fileMenu->addAction(newConnectionAction);
    fileMenu->addMenu(newMenu);
    fileMenu->addAction(openByMyDBMSAction);
    fileMenu->addMenu(openExternalFileMenu);
    fileMenu->addMenu(openRecentlyUsedMenu);
    fileMenu->addAction(importConnectionAction);
    fileMenu->addAction(derivedConnectionAction);
    fileMenu->addAction(managementAction);
    fileMenu->addAction(closewindowAction);
    fileMenu->addAction(closeTabAction);
    fileMenu->addAction(exitMyDBMSAction);

//编辑菜单
    QMenu *editMenu = menuBar->addMenu("编辑");

    QAction *copyAction = new QAction("复制  Ctrl + C", this);
    QAction *pasteAction = new QAction("粘贴  Ctrl + V", this);
    QAction *allAction = new QAction("全选  Ctrl + A", this);

    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(allAction);

//查看菜单
    QMenu *checkMenu = menuBar->addMenu("查看");

    QMenu *navigationPaneMenu = checkMenu->addMenu("导航窗格");
    QAction *newAction7 = new QAction("显示导航窗格", this);
    QAction *newAction8 = new QAction("隐藏连接组", this);
    QAction *newAction9 = new QAction("仅显示活跃对象  F12", this);
    QAction *newAction10 = new QAction("在顶部显示筛选", this);
    navigationPaneMenu->addAction(newAction7);
    navigationPaneMenu->addAction(newAction8);
    navigationPaneMenu->addAction(newAction9);
    navigationPaneMenu->addAction(newAction10);
    QMenu *informationPaneMenu = checkMenu->addMenu("信息窗格");
    QAction *newAction11 = new QAction("显示信息窗格", this);
    QAction *newAction12 = new QAction("常规", this);
    informationPaneMenu->addAction(newAction11);
    informationPaneMenu->addAction(newAction12);
    QAction *listAction = new QAction("列表", this);
    QAction *detailedInformationAction = new QAction("详细信息", this);
    QAction *erDiagramAction = new QAction("ER 图表", this);
    QAction *hiddenControlGroupAction = new QAction("隐藏对照组", this);
    checkMenu->addAction(listAction);
    checkMenu->addAction(detailedInformationAction);
    checkMenu->addAction(erDiagramAction);
    checkMenu->addAction(hiddenControlGroupAction );
    QMenu *sortMenu = checkMenu->addMenu("排序");
    QAction *nameAction = new QAction("名称", this);
    QAction *groupAction = new QAction("组", this);
    QAction *autoincrementAction = new QAction("自动递增值", this);
    QAction *rowFormatAction = new QAction("行格式", this);
    QAction *dataChangedAction = new QAction("修改日期", this);
    QAction *dataCreatedAction = new QAction("创建日期", this);
    QAction *indexLengthAction = new QAction("索引长度", this);
    QAction *dataLengthAction = new QAction("数据长度", this);
    sortMenu->addAction(nameAction);
    sortMenu->addAction(groupAction);
    sortMenu->addAction(autoincrementAction);
    sortMenu->addAction(rowFormatAction);
    sortMenu->addAction(dataChangedAction);
    sortMenu->addAction(dataCreatedAction);
    sortMenu->addAction(indexLengthAction);
    sortMenu->addAction(dataLengthAction);
    QAction *choseListAction = new QAction("选择列", this);
    QAction *showHiddenItemsAction = new QAction("显示隐藏的项目", this);
    QAction *showTableConfigurationFileAction = new QAction("显示表配置文件", this);
    checkMenu->addAction(choseListAction);
    checkMenu->addAction(showHiddenItemsAction);
    checkMenu->addAction(showTableConfigurationFileAction);
//收藏菜单
    QMenu *favoriteMenu = menuBar->addMenu("收藏");
    QAction *addToFavoriteAction = new QAction("添加到收藏夹", this);
    QAction *favorite1Action = new QAction("空的  Ctrl + 1", this);
    favoriteMenu->addAction(addToFavoriteAction);
    favoriteMenu->addAction(favorite1Action);

//工具菜单
    QMenu *toolMenu = menuBar->addMenu("工具");
    QAction *dataTransmissionAction = new QAction("数据传输...", this);
    QAction *dataGenerationAction = new QAction("数据生成...", this);
    QAction *dataDictionaryAction = new QAction("数据字典...", this);
    QAction *dataSynchronismAction = new QAction("数据同步...", this);
    QAction *structureSynchronismAction = new QAction("结构同步...", this);
    QAction *commandLineInterfaceAction = new QAction("命令列界面...", this);
    toolMenu->addAction(dataTransmissionAction);
    toolMenu->addAction(dataGenerationAction);
    toolMenu->addAction(dataDictionaryAction);
    toolMenu->addAction(dataSynchronismAction);
    toolMenu->addAction(structureSynchronismAction);
    toolMenu->addAction(commandLineInterfaceAction);
    QMenu *serverMonitoringMenu = toolMenu->addMenu("服务器监控");
    QAction *newAction13 = new QAction("MySQL", this);
    QAction *newAction14 = new QAction("SQL Server", this);
    QAction *newAction15 = new QAction("Oracle", this);
    serverMonitoringMenu->addAction(newAction13);
    serverMonitoringMenu->addAction(newAction14);
    serverMonitoringMenu->addAction(newAction15);
    QAction *searchInDatabaseOrSchemaAction = new QAction("在数据库或模式中查找...", this);
    QAction *historyLogAction = new QAction("历史日志...  Ctrl + L", this);
    QAction *optionAction = new QAction("选项...", this);
    toolMenu->addAction(searchInDatabaseOrSchemaAction);
    toolMenu->addAction(historyLogAction);
    toolMenu->addAction(optionAction);

//窗口菜单
    QMenu *windowMenu = menuBar->addMenu("窗口");
    QAction *MyDBMSAction = new QAction("MyDBMS  F8", this);
    QAction *nextWindowAction = new QAction("下一个窗口  Ctrl + Tab", this);
    windowMenu->addAction(MyDBMSAction);
    windowMenu->addAction(nextWindowAction);

//帮助菜单
    QMenu *helpMenu = menuBar->addMenu("帮助");
    QAction * MyDBMSHelpAction = new QAction("MyDBMS 帮助  F1", this);
    helpMenu->addAction(MyDBMSHelpAction);
    QMenu *onlineDocumentMenu = helpMenu->addMenu("在线文档");
    onlineDocumentMenu->addAction(newAction13);
    onlineDocumentMenu->addAction(newAction14);
    onlineDocumentMenu->addAction(newAction15);
    QAction * aboutAction = new QAction("关于...", this);
    helpMenu->addAction(aboutAction);

    layout->setMenuBar(menuBar);

    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal, this);
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);

    leftWidget = new QWidget();
    rightWidget = new QWidget();


    horizontalSplitter->addWidget(leftWidget);
    horizontalSplitter->addWidget(rightWidget);
    horizontalSplitter->setSizes({100,500});

    QWidget *midWidget = new QWidget();
    QHBoxLayout *midLayout = new QHBoxLayout();
    midLayout->addWidget(horizontalSplitter);
    midWidget->setLayout(midLayout);

    QVBoxLayout *downLayout = new QVBoxLayout();

    QTextEdit *dialogEdit = new QTextEdit();

    dialogEdit->setText("mysql<");

    verticalSplitter->addWidget(midWidget);
    verticalSplitter->addWidget(dialogEdit);
    verticalSplitter->setSizes({300,100});

    downLayout->addWidget(verticalSplitter);
    layout->addLayout(downLayout, 1, 0);

    //创建表示例1：student
    //attributes << "Sno VARCHAR(8) PRIMARY KEY" << "Sname VARCHAR(20)" << "age INT" << "gender VARCHAR(6)" << "ClassName VARCHAR(6)";

    //createTable("SchoolInfo", "student", attributes);

    //QStringList tuple1 = {"25301201", "Jose", "17", "M", "RJ2501"};
    //QStringList tuple2 = {"25301202", "Julio", "18", "M", "RJ2501"};
    //QStringList tuple3 = {"25301203", "Helena", "18", "F", "RJ2501"};
    //QStringList tuple4 = {"25301204", "Helena", "19", "F", "RJ2501"};


    //insertIntoTable("SchoolInfo", "student", tuple1);
    //insertIntoTable("SchoolInfo", "student", tuple2);
    //insertIntoTable("SchoolInfo", "student", tuple3);
    //insertIntoTable("SchoolInfo", "student", tuple4);

    //创建表示例2：class
    // attributes << "ClassName VARCHAR(6)" << "ClassMajor VARCHAR(20)" << "NOC INT";
    // createTable("SchoolInfo", "class", attributes);
    // QStringList tuple1 = {"RJ2501", "软件工程", "25"};
    // QStringList tuple2 = {"RJ2502", "软件工程", "27"};
    // QStringList tuple3 = {"RJ2503", "软件工程", "29"};
    // QStringList tuple4 = {"RJ2504", "软件工程", "24"};

    // insertIntoTable("SchoolInfo", "class", tuple1);
    // insertIntoTable("SchoolInfo", "class", tuple2);
    // insertIntoTable("SchoolInfo", "class", tuple3);
    // insertIntoTable("SchoolInfo", "class", tuple4);

    setupInLeftWidget(leftWidget);
    setupInRightWidget(rightWidget);

    //删除表示例
    //dropTableTEST("testDataBase1","testTable1");

}

mainwindow::~mainwindow()
{

}

void mainwindow::setupInLeftWidget(QWidget *leftWidget)
{
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    QTreeWidget *treeWidget = new QTreeWidget(leftWidget);
    treeWidget->setHeaderHidden(true);

    treeWidget->setIndentation(15);
    treeWidget->setAnimated(true);
    connect(treeWidget, &QTreeWidget::itemClicked, this, &mainwindow::onTreeItemClicked);

    QTreeWidgetItem *dbRootItem = new QTreeWidgetItem(treeWidget);
    dbRootItem->setText(0, "我的连接222");
    dbRootItem->setIcon(0, QIcon(":/icons/database.png"));
    dbRootItem->setExpanded(true);

    QDir rootDir("D:\\数据库实训\\DBMS\\ruanko.db");
    QStringList dbFolders = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QString &dbFolder, dbFolders) {
        if (dbFolder.endsWith(".db")) {
            QString dbName = dbFolder.left(dbFolder.length() - 3);
            QTreeWidgetItem *dbItem = new QTreeWidgetItem(dbRootItem);
            dbItem->setText(0, dbName);
            dbItem->setIcon(0, QIcon(":/icons/database.png"));

            QDir tableDir(rootDir.path() + "\\" + dbFolder);
            QStringList tableFiles = tableDir.entryList(QStringList() << "*.table", QDir::Files);

            foreach (const QString &tableFile, tableFiles) {
                QString tableName = tableFile.left(tableFile.length() - 6);
                QTreeWidgetItem *tableItem = new QTreeWidgetItem(dbItem);
                tableItem->setText(0, tableName);
                tableItem->setIcon(0, QIcon(":/icons/table.png"));
            }
        }
    }

    leftLayout->addWidget(treeWidget);
    leftWidget->setLayout(leftLayout);
}

void mainwindow::createTable(QString dbName, QString tableName, QStringList attributes)
{
    // 检查并创建数据库文件夹
    QDir dbDir("D:\\数据库实训\\DBMS\\ruanko.db");
    if (!dbDir.exists()) {
        dbDir.mkpath(".");
    }

    QDir dbSubDir("D:\\数据库实训\\DBMS\\ruanko.db\\" + dbName + ".db");
    if (!dbSubDir.exists()) {
        if (dbSubDir.mkpath(".")) {
            databases.append(dbName);
            qDebug() << "成功创建数据库目录:" << dbName;
        } else {
            qDebug() << "无法创建数据库目录:" << dbName;
        }
    } else if (!databases.contains(dbName)) {
        databases.append(dbName);
    }

    QString tableFilePath = "D:\\数据库实训\\DBMS\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";
    QFile tableFile(tableFilePath);

    if (tableFile.exists()) {
        qDebug() << "名为" << tableName << "的表已经存在，请换一个名字再试试";
    } else {
        if (tableFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&tableFile);

            QJsonObject tableInfo;
            tableInfo["tableName"] = tableName;
            tableInfo["createTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);

            QJsonArray attributesArray;
            for (const QString &attr : attributes) {
                attributesArray.append(attr);
            }
            tableInfo["attributes"] = attributesArray;

            tableInfo["data"] = QJsonArray();

            QJsonDocument doc(tableInfo);
            out << doc.toJson();

            tableFile.close();
            tables.append(tableName);
            qDebug() << "成功创建表:" << tableName << "，包含属性:" << attributes;
        } else {
            qDebug() << "无法创建表文件:" << tableName;
        }
    }
}

void mainwindow::setupInRightWidget(QWidget *rightWidget)
{
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    QTableWidget *table = new QTableWidget(rightWidget);

    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rightLayout->addWidget(table);
    rightWidget->setLayout(rightLayout);
}

void mainwindow::insertIntoTable(QString dbName, QString tableName, QStringList tuples)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\ruanko.db\\" +
                            dbName + ".db\\" +
                            tableName + ".table";

    QFile tableFile(tableFilePath);
    if (!tableFile.exists()) {
        qDebug() << "表文件不存在:" << tableFilePath;
        return;
    }

    if (!tableFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开表文件:" << tableFilePath;
        return;
    }

    QByteArray jsonData = tableFile.readAll();
    tableFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qDebug() << "无效的JSON格式:" << tableFilePath;
        return;
    }

    QJsonObject tableObj = doc.object();

    QJsonArray attributesArray = tableObj["attributes"].toArray();
    QJsonArray dataArray = tableObj["data"].toArray();

    if (tuples.size() != attributesArray.size()) {
        qDebug() << "插入的数据数量与表属性数量不匹配";
        return;
    }

    QJsonObject newRow;
    for (int i = 0; i < attributesArray.size(); ++i) {
        QString attr = attributesArray[i].toString();
        newRow[attr] = tuples[i];
    }

    dataArray.append(newRow);

    tableObj["data"] = dataArray;

    if (tableFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&tableFile);
        QJsonDocument newDoc(tableObj);
        out << newDoc.toJson();
        tableFile.close();
        qDebug() << "成功插入数据到表:" << tableName;
    } else {
        qDebug() << "无法写入表文件:" << tableFilePath;
    }
}

void mainwindow::showTable(QString dbName, QString tableName)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\ruanko.db\\" +
                            dbName + ".db\\" +
                            tableName + ".table";

    QFile tableFile(tableFilePath);
    if (!tableFile.exists()) {
        qDebug() << "表文件不存在:" << tableFilePath;
        return;
    }

    if (!tableFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开表文件:" << tableFilePath;
        return;
    }

    QByteArray jsonData = tableFile.readAll();
    tableFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qDebug() << "无效的JSON格式:" << tableFilePath;
        return;
    }

    QJsonObject tableObj = doc.object();

    QJsonArray attributesArray = tableObj["attributes"].toArray();
    QStringList attributes;
    for (const QJsonValue &attr : attributesArray) {
        attributes.append(attr.toString());
    }

    QJsonArray dataArray = tableObj["data"].toArray();

    QTableWidget *tableWidget = rightWidget->findChild<QTableWidget *>();
    if (!tableWidget) {
        qDebug() << "找不到右侧的表格控件";
        return;
    }

    tableWidget->setColumnCount(attributes.size());
    tableWidget->setHorizontalHeaderLabels(attributes);

    tableWidget->setRowCount(1 + dataArray.size());

    for (int col = 0; col < attributes.size(); ++col) {
        QTableWidgetItem *attrItem = new QTableWidgetItem(attributes[col]);
        attrItem->setBackground(QBrush(Qt::lightGray));
        attrItem->setFlags(attrItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(0, col, attrItem);
    }

    for (int row = 0; row < dataArray.size(); ++row) {
        QJsonObject rowData = dataArray[row].toObject();
        for (int col = 0; col < attributes.size(); ++col) {
            QString value = rowData[attributes[col]].toString();
            QTableWidgetItem *dataItem = new QTableWidgetItem(value);
            tableWidget->setItem(row + 1, col, dataItem);
        }
    }

    tableWidget->resizeColumnsToContents();
}

void mainwindow::dropTableTEST(QString dbName, QString tableName)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\ruanko.db\\" +
                            dbName + ".db\\" +
                            tableName + ".table";

    QFile tableFile(tableFilePath);

    if (tableFile.exists()) {
        if (tableFile.remove()) {
            qDebug() << "成功删除表:" << tableName;
        } else {
            qDebug() << "删除表" << tableName << "失败，可能没有权限或文件被锁定";
        }
    } else {
        qDebug() << "表" << tableName << "不存在，无法删除";
    }
}

void mainwindow::onTreeItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    // 检查是否是表节点（表节点有父节点且父节点是数据库节点）
    if (item->parent() && item->parent()->parent()) {
        QString dbName = item->parent()->text(0);
        QString tableName = item->text(0);

        showTable(dbName, tableName);
    }
}
