#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QWidgetAction>
#include <QComboBox>
#include <QTextEdit>
#include <QSplitter>

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

    // 创建 QSplitter 用于分隔左右两部分
    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal, this);
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);

    QWidget *leftWidget = new QWidget();
    QWidget *rightWidget = new QWidget();

    QTextEdit *leftTextEdit = new QTextEdit(leftWidget);
    QTextEdit *rightTextEdit = new QTextEdit(rightWidget);

    horizontalSplitter->addWidget(leftTextEdit);
    horizontalSplitter->addWidget(rightTextEdit);

    QWidget *midWidget = new QWidget();
    QHBoxLayout *midLayout = new QHBoxLayout();
    midLayout->addWidget(horizontalSplitter);
    midWidget->setLayout(midLayout);

    QVBoxLayout *downLayout = new QVBoxLayout();

    QTextEdit *dialogEdit = new QTextEdit();

    verticalSplitter->addWidget(midWidget);
    verticalSplitter->addWidget(dialogEdit);

    downLayout->addWidget(verticalSplitter);
    layout->addLayout(downLayout, 1, 0);
}

mainwindow::~mainwindow()
{

}
