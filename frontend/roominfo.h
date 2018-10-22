#ifndef ROOMINFO_H
#define ROOMINFO_H

#include <QDialog>
//#include "newroom.h"
#include "dialog.h"

using namespace std;

class QPushButton;
class QListWidget;
class QLabel;
class QLineEdit;
class QTimer;

class RoomInfo:public QDialog, public talk_client
{
    Q_OBJECT

public:RoomInfo(const char* str1,const char *str2);

protected:
    QListWidget * roomList;
    QPushButton * newRoomButton;
    QPushButton * enterRoomButton;
    QPushButton * logoutButton;
    QLineEdit *newRoom;
    Dialog *dialog;
    QTimer * timerRoomInfo;

private slots:
    void enterRoomAction();
    void newRoomAction();
    void logoutAction();
    void timerRoomInfoAction();
};

#endif // ROOMINFO_H
