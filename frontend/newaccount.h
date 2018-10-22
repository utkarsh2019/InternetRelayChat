#ifndef NEWACCOUNT_H
#define NEWACCOUNT_H

#include <QDialog>
#include <string.h>
#include "talk_client.h"

using namespace std;

class QLineEdit;
class QPushButton;
class QLabel;

class NewAccount : public QDialog,public talk_client
{
    Q_OBJECT

public:
    NewAccount();

protected:
    QLineEdit *userName;
    QLineEdit *password;
    QPushButton *submitButton;

private slots:
    void submitAction();
};

#endif // NEWACCOUNT_H
