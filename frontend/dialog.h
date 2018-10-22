#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QObject>
#include <string.h>
#include "talk_client.h"

using namespace std;

class QTimer;
class QLabel;
class QLineEdit;
class QPushButton;
class QListWidget;

class Dialog : public QDialog,public talk_client
{

    Q_OBJECT

public:
    Dialog(char * st1,char * str2,char *str3);

protected:

    QListWidget * userList;
    QListWidget * messagesList;
    QLineEdit * inputMessage;
    QPushButton * leaveRoomButton;
    QPushButton * sendButton;
    QTimer * timerDialog;

private slots:
    void leaveRoomAction();
    void sendAction();
    void timerDialogAction();
};


#endif // DIALOG_H
