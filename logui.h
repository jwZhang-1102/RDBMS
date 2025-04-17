#ifndef LOGUI_H
#define LOGUI_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LogUI : public QWidget
{
    Q_OBJECT

public:
    LogUI(QWidget *parent = nullptr);
    ~LogUI();

private slots:
    void onLoginButtonClicked();
    void onLogupButtonClicked();
    void onLogoutButtonClicked();

private:
    QLineEdit *username;
    QLineEdit *password;
    QPushButton *loginButton;
    QPushButton *logupButton;
    QPushButton *logoutButton;
    QLabel *user;
    QLabel *pass;
};

#endif // LOGUI_H
