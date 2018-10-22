#ifndef TALK_CLIENT_H
#define TALK_CLIENT_H

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 300
#define MAX_RESPONSE (20 * 1024)


class talk_client
{

public:
    talk_client();
    int open_client_socket(char * host, int port);
    int sendCommand(char * host, int port, char * command,const char * _user,const char * _password,const char * args, const char * message, char * response);
    void printUsage();
    int add_user(const char *_user,const char *_password);
    int login(const char *_user, const char *_password);
    int add_room(const char *room);
    char * get_rooms(char * allRooms);
    int enter_room(const char *room);
    int leave_room();
    char * get_users_in_room(char * allUsers);
    int send_message(const char * message);
    char * get_messages(char * allMessages);
    void printPrompt();
    void printHelp();
    void * getMessagesThread(void * arg);
    void startGetMessageThread();

protected:
    int lastMessage;
    char * host;
    char * global_user;
    char * global_password;
    char * room_name;
    char * sport;
    int port;

};

#endif // TALK_CLIENT_H
