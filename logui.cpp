#include "logui.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

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
    this->close();
    mainwindow *win = new mainwindow();
    win->show();

}

void LogUI::onLogupButtonClicked()
{

}

void LogUI::onLogoutButtonClicked()
{
    this->close();
}
