#include "login.h"
#include <QtWidgets>

void Login::submitAction()
{
    printf("submit button\n");

    QString str1 = userName->text();
    QString str2 = password->text();
    QByteArray ba1 = str1.toLatin1();
    QByteArray ba2 = str2.toLatin1();
    const char *c_str1 = ba1.data();
    const char *c_str2 = ba2.data();


    if(login(c_str1,c_str2))
    {
        rooms = new RoomInfo(c_str1,c_str2);
        rooms->exec();
        close();
    }
    else
        submitButton->setToolTip("Wrong User Name or Password");
}

Login::Login()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    //Adding usename and password
    QLabel *userLabel = new QLabel("User name :");
    userName = new QLineEdit;
    QRegExp ru("(|\_|[0-9]|[A-Z]|[a-z]){8}");
    userName->setValidator(new QRegExpValidator(ru, this));
    QLabel *passwordLabel = new QLabel("Password :");
    password = new QLineEdit;
    QRegExp rp("(|\_|[0-9]|[A-Z]|[a-z]|\!){20}");
    password->setValidator(new QRegExpValidator(rp, this));
    password->setEchoMode(QLineEdit::Password);

    //Adding submit button
    submitButton = new QPushButton("Submit");


    // Setup action for button
    connect(submitButton, SIGNAL (released()), this, SLOT (submitAction()));

    mainLayout->addWidget(userLabel);
    mainLayout->addWidget(userName);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(password);
    mainLayout->addWidget(submitButton);

    // Add layout to main window
    setLayout(mainLayout);

    setWindowTitle(tr("Login"));
}
