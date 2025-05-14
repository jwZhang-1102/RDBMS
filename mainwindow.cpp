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
    : QWidget(parent), userName(username),
    securityManager(SecurityManager::instance())
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

    fileMenu->addAction(newProjectAction);
    fileMenu->addAction(newConnectionAction);
    fileMenu->addAction(openByMyDBMSAction);
    fileMenu->addAction(importConnectionAction);
    fileMenu->addAction(derivedConnectionAction);
    fileMenu->addAction(managementAction);
    fileMenu->addAction(closewindowAction);
    fileMenu->addAction(closeTabAction);
    fileMenu->addAction(exitMyDBMSAction);

    // 安全菜单
    QMenu *securityMenu = menuBar->addMenu("安全");
    QAction *managePermissionsAction = new QAction("权限管理", this);
    connect(managePermissionsAction, &QAction::triggered, this, &mainwindow::showPermissionManager);
    securityMenu->addAction(managePermissionsAction);

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
    setupPermissionDialog();
}

mainwindow::~mainwindow()
{
}

void mainwindow::setupPermissionDialog() {
    permissionDialog = new QDialog(this);
    permissionDialog->setWindowTitle("权限管理");
    permissionDialog->setMinimumSize(400, 500);

    QVBoxLayout* layout = new QVBoxLayout(permissionDialog);

    // 数据库选择
    QComboBox* dbComboBox = new QComboBox(permissionDialog);
    for (const QString& db : databases) {
        dbComboBox->addItem(db);
    }
    connect(dbComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this, dbComboBox](int index) {
        currentSelectedDb = dbComboBox->itemText(index);
        updatePermissionList();
    });

    // 用户列表
    userListWidget = new QListWidget(permissionDialog);
    userListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(userListWidget, &QListWidget::itemSelectionChanged, [this]() {
        updatePermissionList();
    });

    // 权限列表
    permissionListWidget = new QListWidget(permissionDialog);
    permissionListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // 按钮
    grantButton = new QPushButton("授予权限", permissionDialog);
    revokeButton = new QPushButton("撤销权限", permissionDialog);

    connect(grantButton, &QPushButton::clicked, [this]() {
        if (userListWidget->currentItem() && !currentSelectedDb.isEmpty()) {
            QString selectedUser = userListWidget->currentItem()->text();
            for (QListWidgetItem* item : permissionListWidget->selectedItems()) {
                securityManager.grantPermission(userName, selectedUser, currentSelectedDb,
                                                static_cast<Permission>(item->data(Qt::UserRole).toInt()));
            }
            updatePermissionList();
        }
    });

    connect(revokeButton, &QPushButton::clicked, [this]() {
        if (userListWidget->currentItem() && !currentSelectedDb.isEmpty()) {
            QString selectedUser = userListWidget->currentItem()->text();
            for (QListWidgetItem* item : permissionListWidget->selectedItems()) {
                securityManager.revokePermission(userName, selectedUser, currentSelectedDb,
                                                 static_cast<Permission>(item->data(Qt::UserRole).toInt()));
            }
            updatePermissionList();
        }
    });

    databases.clear();
    QDir dbDir("D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db");
    QStringList dbFolders = dbDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &dbFolder : dbFolders) {
        if (dbFolder.endsWith(".db")) {
            databases.append(dbFolder.left(dbFolder.length() - 3));
        }
    };
    dbComboBox->clear();
    for (const QString& db : databases) {
        dbComboBox->addItem(db);
    }

    // 布局
    layout->addWidget(new QLabel("选择数据库:"));
    layout->addWidget(dbComboBox);
    layout->addWidget(new QLabel("用户列表:"));
    layout->addWidget(userListWidget);
    layout->addWidget(new QLabel("权限列表:"));
    layout->addWidget(permissionListWidget);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(grantButton);
    buttonLayout->addWidget(revokeButton);
    layout->addLayout(buttonLayout);
}

void mainwindow::showPermissionManager() {
    // 加载用户列表
    userListWidget->clear();
    QFile userFile("D:\\数据库实训\\DBMS\\user.txt");
    if (userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&userFile);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QString username = line.split(":").first();
            if (username != userName) { // 不显示当前用户自己
                userListWidget->addItem(username);
            }
        }
        userFile.close();
    }

    if (!databases.isEmpty()) {
        currentSelectedDb = databases.first();
    }

    updatePermissionList();
    permissionDialog->exec();
}

void mainwindow::updatePermissionList() {
    permissionListWidget->clear();

    if (currentSelectedDb.isEmpty() || !securityManager.isDbOwner(userName, currentSelectedDb)) {
        permissionListWidget->setEnabled(false);
        grantButton->setEnabled(false);
        revokeButton->setEnabled(false);
        return;
    }

    permissionListWidget->setEnabled(true);
    grantButton->setEnabled(true);
    revokeButton->setEnabled(true);

    // 添加所有权限项
    QList<QPair<QString, Permission>> permissions = {
        {"删除数据库", Permission::DROP_DB},
        {"创建表", Permission::CREATE_TABLE},
        {"删除表", Permission::DROP_TABLE},
        {"修改表结构", Permission::ALTER_TABLE},
        {"插入数据", Permission::INSERT_DATA},
        {"查询数据", Permission::SELECT_DATA},
        {"更新数据", Permission::UPDATE_DATA},
        {"删除数据", Permission::DELETE_DATA},
        {"查看数据库", Permission::VIEW_DB},
         {"使用数据库", Permission::USE_DB}
    };

    for (const auto& perm : permissions) {
        QListWidgetItem* item = new QListWidgetItem(perm.first);
        item->setData(Qt::UserRole, static_cast<int>(perm.second));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        permissionListWidget->addItem(item);
    }

    // 标记已授予的权限
    if (userListWidget->currentItem()) {
        QString selectedUser = userListWidget->currentItem()->text();
        for (int i = 0; i < permissionListWidget->count(); ++i) {
            QListWidgetItem* item = permissionListWidget->item(i);
            Permission perm = static_cast<Permission>(item->data(Qt::UserRole).toInt());
            if (securityManager.hasPermission(selectedUser, currentSelectedDb, perm)) {
                item->setCheckState(Qt::Checked);
            } else {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
}


void mainwindow::setupInLeftWidget(QWidget *leftWidget) {
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

    // 获取所有数据库
    QList<QString> allDatabases = securityManager.getAllDatabases();

    foreach (const QString &dbName, allDatabases) {
        // 检查是否有查看权限才显示数据库
        if (securityManager.hasPermission(userName, dbName, Permission::VIEW_DB)) {
            QTreeWidgetItem *dbItem = new QTreeWidgetItem(dbRootItem);
            dbItem->setText(0, dbName);
            dbItem->setIcon(0, QIcon(":/icons/database.png"));

            // 检查是否有查看权限才显示表
            if (securityManager.hasPermission(userName, dbName, Permission::VIEW_DB)) {
                QString dbPath = "D:\\数据库实训\\DBMS\\Users\\" + userName + ".u\\ruanko.db\\" + dbName + ".db";
                QDir tableDir(dbPath);
                QStringList tableFiles = tableDir.entryList(QStringList() << "*.table", QDir::Files);

                foreach (const QString &tableFile, tableFiles) {
                    QString tableName = tableFile.left(tableFile.length() - 6);
                    QTreeWidgetItem *tableItem = new QTreeWidgetItem(dbItem);
                    tableItem->setText(0, tableName);
                    tableItem->setIcon(0, QIcon(":/icons/table.png"));
                }
            }
        }
    }

    leftLayout->addWidget(treeWidget);
    leftWidget->setLayout(leftLayout);
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
            securityManager.setDbOwner(dbName, userName); // 设置数据库所有者
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
    if (!securityManager.hasPermission(userName, dbName, Permission::CREATE_TABLE)) {
        QMessageBox::warning(this, "权限不足", "您没有创建表的权限");
        return;
    }
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
    if (!securityManager.hasPermission(userName, dbName, Permission::CREATE_TABLE)) {
        QMessageBox::warning(this, "权限不足", "您没有权限");
        return;
    }
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

    if (tuples.size() != attributesArray.size()) {
        qDebug() << "插入的数据数量与表属性数量不匹配";
        return;
    }

    // 解析字段名（移除类型信息）
    QStringList attributeNames;
    for (const QJsonValue& attr : attributesArray) {
        QString attrStr = attr.toString();
        QString fieldName = attrStr.split(' ').first(); // 只取字段名
        attributeNames.append(fieldName);
    }

    QJsonObject newRow;
    for (int i = 0; i < attributeNames.size(); ++i) {
        newRow[attributeNames[i]] = tuples[i];
    }

    dataArray.append(newRow);
    tableObj["data"] = dataArray;

    // 修复：重新打开文件以写入
    if (!tableFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qDebug() << "无法打开表文件进行写入:" << tableFilePath;
        return;
    }

    QTextStream out(&tableFile);
    QJsonDocument newDoc(tableObj);
    out << newDoc.toJson();
    tableFile.close();

    qDebug() << "成功插入数据到表:" << tableName;
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
    if (!securityManager.isDbOwner(userName, dbName)) {
        QMessageBox::warning(this, "权限不足", "只有数据库所有者可以删除数据库");
        return;
    }
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
            // 检查数据库是否存在
            if (!securityManager.getAllDatabases().contains(dbName)) {
                dialogEdit->append("ERROR: Database '" + dbName + "' does not exist");
            } else {
                useDatabase(dbName);
                if (dataBase == dbName) { // 只有权限检查通过才会设置dataBase
                    dialogEdit->append("Database changed");
                }
            }
        } else {
            dialogEdit->append("ERROR: Database name cannot be empty");
        }

        QTextCursor cursor = dialogEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        dialogEdit->setTextCursor(cursor);
        dialogEdit->append("mysql<");
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
    // 检查是否有USE权限
    if (!securityManager.hasPermission(userName, database, Permission::USE_DB)) {
        QMessageBox::warning(this, "权限不足", "您没有使用此数据库的权限");
        return;
    }
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

        // 检查是否有查看权限
        if (!securityManager.hasPermission(userName, dbName, Permission::VIEW_DB)) {
            QMessageBox::warning(this, "权限不足", "您没有查看此数据库的权限");
            return;
        }

        showTable(dbName, tableName);
    }
}

void mainwindow::onRefreshButtonClicked()
{
    setupInLeftWidget(leftWidget);

    qDebug() << "Database tree view refreshed";
}
