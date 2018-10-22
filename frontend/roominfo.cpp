#include "roominfo.h"
#include <QtWidgets>

void RoomInfo::enterRoomAction()
{
    printf("Enter Room Button\n");

    QString str1 = roomList->currentItem()->text();
    QByteArray ba1 = str1.toLatin1();
    char *c_str1 = ba1.data();

    timerRoomInfo->stop();

    if(enter_room(c_str1))
    {
        dialog = new Dialog(global_user,global_password,c_str1);
        dialog->exec();
    }
}

void RoomInfo::newRoomAction()
{
    printf("New Room Button\n");
    QString str1 = newRoom->text();
    QByteArray ba1 = str1.toLatin1();
    const char *c_str1 = ba1.data();

    if(add_room(c_str1))
        roomList->addItem(c_str1);

    newRoom->clear();
}

void RoomInfo::timerRoomInfoAction()
{
    printf("Timer wakeup\n");

    roomList->clear();

    //Adding pre-existing rooms
    char *allRooms;
    allRooms = get_rooms(allRooms);

        int i=0,j=0;
        string s1[50];
        while(allRooms[i] != '\0')
        {
        while(allRooms[i] != '\n' && allRooms[i] != '\0')
        {
            s1[j].push_back(allRooms[i]);
            i++;
        }

        j++;
        if(allRooms[i] != '\0')
            i++;
    }


    for(i=0; s1[i] != "\0" ;i++)
    {
        roomList->addItem(s1[i].c_str());
    }
}


void RoomInfo::logoutAction()
{
    printf("Logged out\n");
    timerRoomInfo->stop();
    close();
}

RoomInfo::RoomInfo(const char *str1,const char* str2)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    global_user = (char *)str1;
    global_password = (char *)str2;

    //Room list
    QLabel *roomLabel = new QLabel("Rooms");
    roomList = new QListWidget;

    //Adding pre-existing rooms
    char *allRooms;
    allRooms = get_rooms(allRooms);


        int i=0,j=0;
        string s1[50];
        while(allRooms[i] != '\0')
        {
            while(allRooms[i] != '\n' && allRooms[i] != '\0')
        {
            s1[j].push_back(allRooms[i]);
            i++;
        }

        j++;
        if(allRooms[i] != '\0')
            i++;
    }


    for(i=0; s1[i] != "\0" ;i++)
    {
        roomList->addItem(s1[i].c_str());
    }


    //Login and New account buttons
    enterRoomButton = new QPushButton("Enter Room");
    newRoomButton = new QPushButton("Create New Room");
    logoutButton = new QPushButton("Logout");

    // Setup actions for buttons
    connect(enterRoomButton, SIGNAL (released()), this, SLOT (enterRoomAction()));
    connect(newRoomButton, SIGNAL (released()), this, SLOT (newRoomAction()));
    connect(logoutButton, SIGNAL (released()), this, SLOT (logoutAction()));

    //New room
    newRoom = new QLineEdit;
    QLabel *newRoomLabel = new QLabel("New Room");
    QRegExp rr("(|\_|[0-9]|[A-Z]|[a-z]){10}");
    newRoom->setValidator(new QRegExpValidator(rr, this));

    mainLayout->addWidget(roomLabel);
    mainLayout->addWidget(roomList);
    mainLayout->addWidget(enterRoomButton);
    mainLayout->addWidget(newRoomLabel);
    mainLayout->addWidget(newRoom);
    mainLayout->addWidget(newRoomButton);
    mainLayout->addWidget(logoutButton);

    // Add layout to main window
    setLayout(mainLayout);

    setWindowTitle(tr("Room Info"));

    timerRoomInfo = new QTimer(this);
    timerRoomInfo->setInterval(5000);
    connect(timerRoomInfo, SIGNAL (timeout()), this, SLOT (timerRoomInfoAction()));
    timerRoomInfo->start(5000);
}
