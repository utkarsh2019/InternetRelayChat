#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "roominfo.h"

using namespace std;

class QLineEdit;
class QPushButton;
class ControlWindow;

class Login : public QDialog, public talk_client
{

    Q_OBJECT

public:
    Login();

protected:
    QLineEdit *userName;
    QLineEdit *password;
    QPushButton *submitButton;
    RoomInfo *rooms;

private slots:
    void submitAction();
};

#endif // LOGIN_H
