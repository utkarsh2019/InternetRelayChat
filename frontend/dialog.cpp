#include <QtWidgets>
#include "dialog.h"

void Dialog::leaveRoomAction()
{
    printf("Leave Room Button\n");
    char * s = "----left room----";
    send_message(s);
    leave_room();
    timerDialog->stop();
    close();
}


void Dialog::sendAction()
{
    printf("Send Button\n");
    QString str1 = inputMessage->text();
    QByteArray ba1 = str1.toLatin1();
    const char *c_str1 = ba1.data();

    send_message(c_str1);
    inputMessage->clear();
}

void Dialog::timerDialogAction()
{
    printf("Timer wakeup\n");

    messagesList->clear();

    char *allMessages;
    allMessages = get_messages(allMessages);

    int i=0,j=0;  
    string s2[100];
    while(allMessages[i] != '\0')
    {
        while(allMessages[i] != '\n' && allMessages[i] != '\0')
        {
            s2[j].push_back(allMessages[i]);
            i++;
        }

        j++;
        if(allMessages[i] != '\0')
            i++;
    }


    for(i=0; s2[i] != "\0" ;i++)
    {
        messagesList->addItem(s2[i].c_str());
    }

    i = 0;
    j = 0;

    userList->clear();

    char *allUsers;
    allUsers = get_users_in_room(allUsers);

    string s1[50];
    while(allUsers[i] != '\0')
    {
        while(allUsers[i] != '\n' && allUsers[i] != '\0')
    {
        s1[j].push_back(allUsers[i]);
        i++;
    }

    j++;
    if(allUsers[i] != '\0')
        i++;
    }


    for(i=0; s1[i] != "\0" ;i++)
    {
        userList->addItem(s1[i].c_str());
    }
}

Dialog::Dialog(char *str1,char *str2,char *str3)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    global_user = str1;
    global_password = str2;
    room_name = str3;

    char * s = "----entered room----";
    send_message(s);

    // Users List
    QVBoxLayout * userLayout = new QVBoxLayout();
    QLabel * userLabel = new QLabel("Users in Room");
    userList = new QListWidget();


    char *allUsers;
    allUsers = get_users_in_room(allUsers);

    int i=0,j=0;
    string s1[50];
    while(allUsers[i] != '\0')
    {
        while(allUsers[i] != '\n' && allUsers[i] != '\0')
    {
        s1[j].push_back(allUsers[i]);
        i++;
    }

    j++;
    if(allUsers[i] != '\0')
        i++;
    }


    for(i=0; s1[i] != "\0" ;i++)
    {
        userList->addItem(s1[i].c_str());
    }

    userLayout->addWidget(userLabel);
    userLayout->addWidget(userList);

    // Layout for rooms and users
    QVBoxLayout *layoutRoomsUsers = new QVBoxLayout;
    layoutRoomsUsers->addLayout(userLayout);

    // Textbox for all messages
    QVBoxLayout * messagesLayout = new QVBoxLayout();
    QLabel * messagesLabel = new QLabel("Messages");
    messagesList = new QListWidget;

    char *allMessages;
    allMessages = get_messages(allMessages);

    i=0;
    j=0;
    string s2[100];
    while(allMessages[i] != '\0')
    {
        while(allMessages[i] != '\n' && allMessages[i] != '\0')
        {
            s2[j].push_back(allMessages[i]);
            i++;
        }

        j++;
        if(allMessages[i] != '\0')
            i++;
    }


    for(i=0; s2[i] != "\0" ;i++)
    {
        messagesList->addItem(s2[i].c_str());
    }


    messagesLayout->addWidget(messagesLabel);
    messagesLayout->addWidget(messagesList);

    // Textbox for input message
    QVBoxLayout * inputMessagesLayout = new QVBoxLayout();
    QLabel * inputMessagesLabel = new QLabel("Type your message:");
    inputMessage = new QLineEdit;
    inputMessage->setMaxLength(25);
    inputMessagesLayout->addWidget(inputMessagesLabel);
    inputMessagesLayout->addWidget(inputMessage);

    // Send and new account buttons
    QHBoxLayout *layoutButtons = new QHBoxLayout;
    leaveRoomButton = new QPushButton("Leave Room");
    sendButton = new QPushButton("Send");
    layoutButtons->addWidget(sendButton);
    layoutButtons->addWidget(leaveRoomButton);

    // Setup actions for buttons
    connect(leaveRoomButton, SIGNAL (released()), this, SLOT (leaveRoomAction()));
    connect(sendButton, SIGNAL (released()), this, SLOT (sendAction()));

    // Add all widgets to window
    mainLayout->addLayout(layoutRoomsUsers);
    mainLayout->addLayout(messagesLayout);
    mainLayout->addLayout(inputMessagesLayout);
    mainLayout->addLayout(layoutButtons);

    // Add layout to main window
    setLayout(mainLayout);

    setWindowTitle(tr(room_name));
    //timer->setInterval(5000);

    timerDialog = new QTimer(this);
    timerDialog->setInterval(5000);
    connect(timerDialog, SIGNAL (timeout()), this, SLOT (timerDialogAction()));
    timerDialog->start(5000);
}
