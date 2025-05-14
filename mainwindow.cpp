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
#include <QRegularExpression>

mainwindow::mainwindow(const QString &username, QWidget *parent)
    : QWidget(parent), userName(username)  // 初始化user成员
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

    refreshButton = new QPushButton("刷新", this);
    refreshButton->setFixedSize(80, 30);
    connect(refreshButton, &QPushButton::clicked, this, &mainwindow::onRefreshButtonClicked);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();
    layout->addLayout(buttonLayout, 0, 0);

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

    QTextCursor cursor = dialogEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    dialogEdit->setTextCursor(cursor);

    connect(dialogEdit, &QTextEdit::textChanged, this, &mainwindow::onTextChanged);

    verticalSplitter->addWidget(midWidget);
    verticalSplitter->addWidget(dialogEdit);
    verticalSplitter->setSizes({300,100});

    downLayout->addWidget(verticalSplitter);
    layout->addLayout(downLayout, 1, 0);

    setupInLeftWidget(leftWidget);
    setupInRightWidget(rightWidget);


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
    dbRootItem->setText(0, "我的连接");
    dbRootItem->setIcon(0, QIcon(":/icons/database.png"));
    dbRootItem->setExpanded(true);

    QDir rootDir("D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db");
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
    qDebug() << "Current user:" << userName;
    qDebug() << "Loading from:" << "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db";
}

void mainwindow::createDataBase(QString dbName)
{
    QDir dbDir("D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db");
    if (!dbDir.exists()) {
        dbDir.mkpath(".");
    }

    QDir dbSubDir("D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db");
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
}

void mainwindow::createEmptyTable(QString dbName, QString tableName)
{
    //确保数据库存在
    createDataBase(dbName);

    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";
    QFile tableFile(tableFilePath);

    if (tableFile.exists()) {
        qDebug() << "名为" << tableName << "的表已经存在，请换一个名字再试试";
    } else {
        if (tableFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&tableFile);

            QJsonObject tableInfo;
            tableInfo["tableName"] = tableName;
            tableInfo["createTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
            tableInfo["attributes"] = QJsonArray();
            tableInfo["data"] = QJsonArray();

            QJsonDocument doc(tableInfo);
            out << doc.toJson();

            tableFile.close();
            tables.append(tableName);
            qDebug() << "成功创建表:" << tableName;
        } else {
            qDebug() << "无法创建表文件:" << tableName;
        }
    }
}

void mainwindow::createTable(QString dbName, QString tableName, QStringList attributes)
{
    // 首先确保数据库存在
    createDataBase(dbName);

    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";
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

void mainwindow::insertAttribute(QString dbName, QString tableName, QStringList attributes)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";

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

    for (const QString &newAttr : attributes) {
        bool attrExists = false;
        for (const QJsonValue &existingAttr : attributesArray) {
            if (existingAttr.toString() == newAttr) {
                attrExists = true;
                break;
            }
        }
        if (attrExists) {
            qDebug() << "属性" << newAttr << "已存在，跳过添加";
            continue;
        }
        attributesArray.append(newAttr);
    }

    for (int i = 0; i < dataArray.size(); ++i) {
        QJsonObject row = dataArray[i].toObject();
        for (const QString &newAttr : attributes) {
            if (!row.contains(newAttr)) {
                row[newAttr] = "";
            }
        }
        dataArray[i] = row;
    }

    tableObj["attributes"] = attributesArray;
    tableObj["data"] = dataArray;

    if (tableFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&tableFile);
        QJsonDocument newDoc(tableObj);
        out << newDoc.toJson();
        tableFile.close();
        qDebug() << "成功添加属性到表:" << tableName;
    } else {
        qDebug() << "无法写入表文件:" << tableFilePath;
    }
}

void mainwindow::alterAttribute(QString dbName, QString tableName, QString attribute)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";

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

    QStringList parts = attribute.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 2) {
        qDebug() << "无效的属性定义格式，应为'属性名 类型定义'";
        return;
    }

    QString attrName = parts[0];
    QString newTypeDef = parts.mid(1).join(' ');

    bool attrFound = false;
    for (int i = 0; i < attributesArray.size(); ++i) {
        QString currentAttr = attributesArray[i].toString();
        QStringList currentParts = currentAttr.split(' ', Qt::SkipEmptyParts);
        if (currentParts.size() >= 1 && currentParts[0] == attrName) {
            QString newAttrDef = attrName + " " + newTypeDef;
            attributesArray[i] = newAttrDef;
            attrFound = true;
            break;
        }
    }

    if (!attrFound) {
        qDebug() << "属性" << attrName << "不存在";
        return;
    }

    tableObj["attributes"] = attributesArray;

    if (tableFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&tableFile);
        QJsonDocument newDoc(tableObj);
        out << newDoc.toJson();
        tableFile.close();
        qDebug() << "成功修改属性定义:" << attrName << "->" << attribute;
    } else {
        qDebug() << "无法写入表文件:" << tableFilePath;
    }
}

void mainwindow::deleteAttribute(QString dbName, QString tableName, QString attribute)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";

    QFile tableFile(tableFilePath);
    if (!tableFile.exists()) {
        qDebug() << "ERROR: Table file does not exist:" << tableFilePath;
        return;
    }

    if (!tableFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot open table file:" << tableFilePath;
        return;
    }

    QByteArray jsonData = tableFile.readAll();
    tableFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qDebug() << "ERROR: Invalid JSON format in table file:" << tableFilePath;
        return;
    }

    QJsonObject tableObj = doc.object();
    QJsonArray attributesArray = tableObj["attributes"].toArray();
    QJsonArray dataArray = tableObj["data"].toArray();

    bool attrFound = false;
    for (int i = 0; i < attributesArray.size(); ++i) {
        QString fullAttribute = attributesArray[i].toString();
        QString attributeName = fullAttribute.split(' ').first();

        if (attributeName.compare(attribute, Qt::CaseInsensitive) == 0) {
            attributesArray.removeAt(i);
            attrFound = true;
            break;
        }
    }

    if (!attrFound) {
        qDebug() << "ERROR: Column" << attribute << "does not exist in table";
        return;
    }

    // 从数据中删除该属性
    for (int i = 0; i < dataArray.size(); ++i) {
        QJsonObject row = dataArray[i].toObject();
        for(const QString& key : row.keys()) {
            if(key.compare(attribute, Qt::CaseInsensitive) == 0) {
                row.remove(key);
                dataArray[i] = row;
                break;
            }
        }
    }

    tableObj["attributes"] = attributesArray;
    tableObj["data"] = dataArray;

    if (tableFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&tableFile);
        QJsonDocument newDoc(tableObj);
        out << newDoc.toJson();
        tableFile.close();
    } else {
        qDebug() << "ERROR: Cannot write to table file:" << tableFilePath;
    }
}



void mainwindow::insertIntoTable(QString dbName, QString tableName, QStringList tuples)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";

    QFile tableFile(tableFilePath);
    if (!tableFile.exists()) {
        qDebug() << "The table file does not exist:" << tableFilePath;
        return;
    }

    if (!tableFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open the table file:" << tableFilePath;
        return;
    }

    QByteArray jsonData = tableFile.readAll();
    tableFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qDebug() << "Invalid JSON format:" << tableFilePath;
        return;
    }

    QJsonObject tableObj = doc.object();
    QJsonArray attributesArray = tableObj["attributes"].toArray();
    QJsonArray dataArray = tableObj["data"].toArray();

    if (tuples.size() != attributesArray.size()) {
        qDebug() << "The number of inserted data does not match the number of table attributes";
        return;
    }

    QStringList attributeNames;
    for (const QJsonValue& attr : attributesArray) {
        QString attrStr = attr.toString();
        QString fieldName = attrStr.split(' ').first();
        attributeNames.append(fieldName);
    }

    QJsonObject newRow;
    for (int i = 0; i < attributeNames.size(); ++i) {
        newRow[attributeNames[i]] = tuples[i];
    }

    dataArray.append(newRow);
    tableObj["data"] = dataArray;

    if (!tableFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "The table file cannot be opened for writing:" << tableFilePath;
        return;
    }

    QTextStream out(&tableFile);
    QJsonDocument newDoc(tableObj);
    out << newDoc.toJson();
    tableFile.close();

    qDebug() << "Successfully inserted data into the table:" << tableName;
}



void mainwindow::showTable(QString dbName, QString tableName)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";

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

void mainwindow::dropDataBase(QString dbName)
{
    QString dbPath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db";
    QDir dbDir(dbPath);

    if (dbDir.exists()) {

        dbDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        foreach(QString dirFile, dbDir.entryList()) {
            dbDir.remove(dirFile);
        }

        if (dbDir.rmdir(dbPath)) {
            qDebug() << "成功删除数据库:" << dbName;

            if (databases.contains(dbName)) {
                databases.removeOne(dbName);
            }

            setupInLeftWidget(leftWidget);
        } else {
            qDebug() << "删除数据库" << dbName << "失败，可能文件夹不为空或没有权限";
        }
    } else {
        qDebug() << "数据库" << dbName << "不存在，无法删除";
    }
}

void mainwindow::dropTableTEST(QString dbName, QString tableName)
{
    QString tableFilePath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db\\" + tableName + ".table";

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

void mainwindow::processDDL()
{
    QTextEdit *dialogEdit = findChild<QTextEdit *>();
    if (!dialogEdit) {
        qDebug() << "找不到对话框编辑器";
        return;
    }

    QString currentText = dialogEdit->toPlainText();
    int lastPromptPos = currentText.lastIndexOf("mysql<");
    if (lastPromptPos == -1) {
        qDebug() << "未找到命令提示符";
        return;
    }

    QString command = currentText.mid(lastPromptPos + 6).trimmed();
    command = command.left(command.length() - 1);

    QString lowerCommand = command.toLower();

    // 解析SQL命令
    // 选择数据库
    if(lowerCommand.startsWith("use")) {
        QString dbName = command.mid(3).trimmed();
        if (!dbName.isEmpty()) {
            QString dbPath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db";

            QFile dbFile(dbPath);
            if (dbFile.exists()) {
                useDatabase(dbName);
                dialogEdit->append("Database changed");
            } else {
                dialogEdit->append("ERROR: Database '" + dbName + "' does not exist");
            }

            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
        } else {
            dialogEdit->append("ERROR: Database name cannot be empty");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
        }
        return;
    }
    // 检查是否已选择数据库
    else if (dataBase.isEmpty() && !lowerCommand.startsWith("create database")) {
        dialogEdit->append("ERROR: No database selected. Please use 'USE database' first or Create a new database.");
        QTextCursor cursor = dialogEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        dialogEdit->setTextCursor(cursor);
        dialogEdit->append("mysql<");
        return;
    }
    //创建数据库
    else if (lowerCommand.startsWith("create database")) {
        //解析 CREATE DATABASE语句
        QString dbName = command.mid(15).trimmed();
        if (!dbName.isEmpty()) {
            createDataBase(dbName);
            dialogEdit->append("Query OK, 1 row affected");
            setupInLeftWidget(leftWidget); // 刷新左侧树形视图
        } else {
            dialogEdit->append("ERROR: Database name cannot be empty");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
        }
    }
    //删除数据库
    else if (lowerCommand.startsWith("drop database")) {
        // 解析 DROP DATABASE 语句
        QString dbName = command.mid(13).trimmed();
        if (!dbName.isEmpty()) {
            dropDataBase(dbName);
            dialogEdit->append("Query OK, database dropped");
            setupInLeftWidget(leftWidget); // 刷新左侧树形视图
        } else {
            dialogEdit->append("ERROR: Database name cannot be empty");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
        }
    }
    //创建表
    else if (lowerCommand.startsWith("create table")) {
        //解析 CREATE TABLE语句
        int openParen = command.indexOf('(');
        if (openParen == -1) {
            dialogEdit->append("ERROR: Invalid CREATE TABLE syntax");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
            return;
        }

        QString tableName = command.mid(12, openParen - 12).trimmed();
        if (tableName.isEmpty()) {
            dialogEdit->append("ERROR: Table name cannot be empty");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
            return;
        }

        QString attrPart = command.mid(openParen + 1);
        attrPart = attrPart.left(attrPart.lastIndexOf(')')).trimmed();
        QStringList attrList = attrPart.split(',', Qt::SkipEmptyParts);

        QStringList attributes;
        for (QString attr : attrList) {
            attributes.append(attr.trimmed());
        }

        createTable(dataBase, tableName, attributes);
        dialogEdit->append("Query OK, 0 rows affected");
        setupInLeftWidget(leftWidget);
    }
    //删除表
    else if (lowerCommand.startsWith("drop table")) {
        // 解析 DROP TABLE 语句
        QString tableName = command.mid(10).trimmed();
        if (tableName.isEmpty()) {
            dialogEdit->append("ERROR: Table name cannot be empty");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
            return;
        }

        dropTableTEST(dataBase, tableName);
        dialogEdit->append("Query OK, 0 rows affected");
        setupInLeftWidget(leftWidget);
    }
    //修改表
    else if (lowerCommand.startsWith("alter table")) {
        //解析ALTER TABLE语句
        //支持 ADD COLUMN, MODIFY COLUMN, DROP COLUMN
        QString alterPart = command.mid(11).trimmed();
        QStringList parts = alterPart.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (parts.size() < 2) {
            dialogEdit->append("ERROR: Invalid ALTER TABLE syntax");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
            return;
        }

        QString tableName = parts[0];
        QString operation = parts[1].toLower();

        if (operation == "add") {
            if (parts.size() < 4 || parts[2].toLower() != "column") {
                dialogEdit->append("ERROR: Invalid ADD COLUMN syntax");
                QTextCursor cursor = dialogEdit->textCursor();
                cursor.movePosition(QTextCursor::End);
                dialogEdit->setTextCursor(cursor);
                dialogEdit->append("mysql<");
                return;
            }
            QString columnDef = parts.mid(3).join(" ");
            QStringList attributes;
            attributes.append(columnDef);
            insertAttribute(dataBase, tableName, attributes);
            dialogEdit->append("Query OK, 0 rows affected");
        }
        else if (operation == "modify") {
            if (parts.size() < 4 || parts[2].toLower() != "column") {
                dialogEdit->append("ERROR: Invalid MODIFY COLUMN syntax");
                QTextCursor cursor = dialogEdit->textCursor();
                cursor.movePosition(QTextCursor::End);
                dialogEdit->setTextCursor(cursor);
                dialogEdit->append("mysql<");
                return;
            }
            QString newColumnDef = parts.mid(3).join(" ");
            alterAttribute(dataBase, tableName, newColumnDef);
            dialogEdit->append("Query OK, 0 rows affected");
        }
        else if (operation == "drop") {
            if (parts.size() < 4 || parts[2].toLower() != "column") {
                dialogEdit->append("ERROR: Invalid DROP COLUMN syntax");
                QTextCursor cursor = dialogEdit->textCursor();
                cursor.movePosition(QTextCursor::End);
                dialogEdit->setTextCursor(cursor);
                dialogEdit->append("mysql<");
                return;
            }
            QString columnName = parts[3];
            deleteAttribute(dataBase, tableName, columnName);
            dialogEdit->append("Query OK, 0 rows affected");
        }
        else {
            dialogEdit->append("ERROR: Unsupported ALTER TABLE operation");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
        }

        setupInLeftWidget(leftWidget);
    }
    //插入数据
    else if (lowerCommand.startsWith("insert into")) {
        //解析INSERT INTO语句

        int valuesPos = lowerCommand.indexOf("values");
        if (valuesPos == -1) {
            dialogEdit->append("ERROR: Invalid INSERT syntax - missing VALUES");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
            return;
        }

        QString tableName = command.mid(11, valuesPos - 11).trimmed();
        if (tableName.isEmpty()) {
            dialogEdit->append("ERROR: Table name cannot be empty");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
            return;
        }

        QString valuesPart = command.mid(valuesPos + 6).trimmed();
        if (!valuesPart.startsWith('(') || !valuesPart.endsWith(')')) {
            dialogEdit->append("ERROR: Invalid VALUES syntax");
            QTextCursor cursor = dialogEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            dialogEdit->setTextCursor(cursor);
            dialogEdit->append("mysql<");
            return;
        }

        QString valuesContent = valuesPart.mid(1, valuesPart.length() - 2).trimmed();
        QStringList values = valuesContent.split(',', Qt::SkipEmptyParts);

        QStringList tuples;
        for (QString value : values) {
            tuples.append(value.trimmed().remove('\'').remove('"'));
        }

        insertIntoTable(dataBase, tableName, tuples);
        dialogEdit->append("Query OK, 1 row affected");
    }
    else {
        dialogEdit->append("ERROR: Unsupported SQL command");
        QTextCursor cursor = dialogEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        dialogEdit->setTextCursor(cursor);
        dialogEdit->append("mysql<");
    }

    QTextCursor cursor = dialogEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    dialogEdit->setTextCursor(cursor);
    dialogEdit->append("mysql<");
}

void mainwindow::useDatabase(QString database)
{
    dataBase = database;
}

void mainwindow::onTextChanged()
{
    QTextEdit *dialogEdit = findChild<QTextEdit *>();
    if (!dialogEdit) {
        return;
    }

    QString text = dialogEdit->toPlainText();
    if (text.isEmpty()) {
        return;
    }

    if (text.endsWith(';')) {
        int lastPromptPos = text.lastIndexOf("mysql<");
        if (lastPromptPos == -1) {
            return;
        }

        QString userInput = text.mid(lastPromptPos + 6).trimmed();
        if (userInput.endsWith(';')) {
            processDDL();
        }
    }
}

void mainwindow::onTreeItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    if (item->parent() && item->parent()->parent()) {
        QString dbName = item->parent()->text(0);
        QString tableName = item->text(0);

        showTable(dbName, tableName);
    }
}

void mainwindow::onRefreshButtonClicked()
{
    setupInLeftWidget(leftWidget);

    qDebug() << "Database tree view refreshed";
}
