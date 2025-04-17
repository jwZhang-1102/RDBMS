#include "logui.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QCryptographicHash>

LogUI::LogUI(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("登录");
    setFixedSize(300,200);

    user = new QLabel("账号:");
    pass = new QLabel("密码:");
    username = new QLineEdit();
    password = new QLineEdit();
    loginButton = new QPushButton("登录");
    logupButton = new QPushButton("注册");
    logoutButton = new QPushButton("退出");

    connect(loginButton, &QPushButton::clicked, this, &LogUI::onLoginButtonClicked);
    connect(logupButton, &QPushButton::clicked, this, &LogUI::onLogupButtonClicked);
    connect(logoutButton, &QPushButton::clicked, this, &LogUI::onLogoutButtonClicked);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    usernameLayout->addWidget(user);
    usernameLayout->addWidget(username);

    passwordLayout->addWidget(pass);
    passwordLayout->addWidget(password);

    layout->addLayout(usernameLayout);
    layout->addLayout(passwordLayout);
    layout->addWidget(loginButton);
    layout->addWidget(logupButton);
    layout->addWidget(logoutButton);
}

LogUI::~LogUI()
{
}

void LogUI::onLoginButtonClicked()
{
    QString inputUsername = username->text().trimmed();
    QString inputPassword = password->text().trimmed();

    if(inputUsername.isEmpty() || inputPassword.isEmpty()) {
        QMessageBox::warning(this, "警告", "账号和密码不能为空!");
        return;
    }

    QFile file("D:\\数据库实训\\DBMS\\user.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "用户数据库打开失败!");
        return;
    }

    QTextStream in(&file);
    bool userFound = false;

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(":");
        if(parts.size() == 2) {
            QString storedUsername = parts[0];
            QString storedPassword = parts[1];

            if(storedUsername == inputUsername && storedPassword == inputPassword) {
                userFound = true;
                break;
            }
        }
    }

    file.close();

    if(userFound) {
        this->close();
        mainwindow *win = new mainwindow(inputUsername);
        win->show();

    } else {
        QMessageBox::warning(this, "警告", "用户名或密码错误!");
    }
}

void LogUI::onLogupButtonClicked()
{
    QString inputUsername = username->text().trimmed();
    QString inputPassword = password->text().trimmed();

    if(inputUsername.isEmpty() || inputPassword.isEmpty()) {
        QMessageBox::warning(this, "警告", "账号和密码不能为空!");
        return;
    }

    if(inputUsername.contains('\\') || inputUsername.contains('/') ||
        inputUsername.contains(':') || inputUsername.contains('*') ||
        inputUsername.contains('?') || inputUsername.contains('"') ||
        inputUsername.contains('<') || inputUsername.contains('>') ||
        inputUsername.contains('|')) {
        QMessageBox::warning(this, "警告", "用户名包含非法字符！");
        return;
    }

    QFile checkFile("D:\\数据库实训\\DBMS\\user.txt");
    if(checkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&checkFile);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(":");
            if(parts.size() > 0 && parts[0] == inputUsername) {
                QMessageBox::warning(this, "警告", "用户名已存在!");
                checkFile.close();
                return;
            }
        }
        checkFile.close();
    }

    QFile userFile("D:\\数据库实训\\DBMS\\user.txt");
    if(!userFile.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", "无法打开用户数据库!");
        return;
    }

    QTextStream out(&userFile);
    out << inputUsername << ":" << inputPassword << "\n";
    userFile.close();

    QString userFolderPath = "D:\\数据库实训\\DBMS\\" + inputUsername + ".u";
    QDir userFolder;

    if(!userFolder.exists(userFolderPath)) {
        if(!userFolder.mkpath(userFolderPath)) {
            QMessageBox::warning(this, "警告", QString("无法创建用户文件夹: %1").arg(userFolderPath));
            return;
        }
    }

    QString dbFolderPath = userFolderPath + "\\ruanko.db";
    QDir dbFolder;

    if(!dbFolder.exists(dbFolderPath)) {
        if(!dbFolder.mkpath(dbFolderPath)) {
            QMessageBox::warning(this, "警告", QString("无法创建数据库文件夹: %1").arg(dbFolderPath));
            return;
        }
        QMessageBox::information(this, "成功", QString("注册成功！已创建用户文件夹和数据库文件夹。\n路径: %1").arg(dbFolderPath));
    } else {
        QMessageBox::warning(this, "警告", QString("数据库文件夹已存在: %1").arg(dbFolderPath));
    }
}

void LogUI::onLogoutButtonClicked()
{
    this->close();
}
