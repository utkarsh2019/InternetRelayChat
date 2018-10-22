#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>
#include "newaccount.h"
#include "login.h"

using namespace std;

class QPushButton;

class UserInfo:public QDialog,public talk_client
{
    Q_OBJECT

public:UserInfo();

protected:
    QPushButton * loginButton;
    QPushButton * newUserButton;
    NewAccount *newAcc;
    Login *log;

private slots:
    void loginAction();
    void newUserAction();

};

#endif // USERINFO_H
