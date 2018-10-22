#include <QtWidgets>
#include "userinfo.h"

void UserInfo::loginAction()
{
    printf("Login Button\n");
    log = new Login;
    log->exec();
}

void UserInfo::newUserAction()
{
    printf("New User Button\n");
    newAcc = new NewAccount;
    newAcc->exec();
}

UserInfo::UserInfo() : talk_client()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    //Welcome Message
    QVBoxLayout *welcome = new QVBoxLayout;
    QLabel *welcomeMessage = new QLabel("\t           Hello.\nWelcome to Internet Relay Chatroom.\n\t        Have fun!\n");
    welcome->addWidget(welcomeMessage);

    //Login and New account buttons
    QHBoxLayout *layoutButtons = new QHBoxLayout;
    QPushButton * sendButton = new QPushButton("Login");
    QPushButton * newUserButton = new QPushButton("Create New Account");
    layoutButtons->addWidget(sendButton);
    layoutButtons->addWidget(newUserButton);

    // Setup actions for buttons
    connect(sendButton, SIGNAL (released()), this, SLOT (loginAction()));
    connect(newUserButton, SIGNAL (released()), this, SLOT (newUserAction()));

    mainLayout->addLayout(welcome);
    mainLayout->addLayout(layoutButtons);

    // Add layout to main window
    setLayout(mainLayout);

    setWindowTitle(tr("Internet Relay Chatroom"));
}
