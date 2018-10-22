#include "talk_client.h"

talk_client::talk_client()
{
    lastMessage = 0;
    host = "sslab10.cs.purdue.edu";
    port = 12019;
}

int talk_client::open_client_socket(char * host, int port) {
    // Initialize socket address structure
    struct  sockaddr_in socketAddress;

    // Clear sockaddr structure
    memset((char *)&socketAddress,0,sizeof(socketAddress));

    // Set family to Internet
    socketAddress.sin_family = AF_INET;

    // Set port
    socketAddress.sin_port = htons((u_short)port);

    // Get host table entry for this host
    struct  hostent  *ptrh = gethostbyname(host);
    if ( ptrh == NULL ) {
        perror("gethostbyname");
        exit(1);
    }

    // Copy the host ip address to socket address structure
    memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);

    // Get TCP transport protocol entry
    struct  protoent *ptrp = getprotobyname("tcp");
    if ( ptrp == NULL ) {
        perror("getprotobyname");
        exit(1);
    }

    // Create a tcp socket
    int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    // Connect the socket to the specified server
    if (connect(sock, (struct sockaddr *)&socketAddress,
            sizeof(socketAddress)) < 0) {
        perror("connect");
        exit(1);
    }

    return sock;
}

int talk_client::sendCommand(char * host, int port, char * command,const char * _user,const char * _password,const char * args, const char * message,char * response)
{
    int sock = open_client_socket( host, port);

    // Send command
    write(sock, command, strlen(command));
    write(sock, " ", 1);
    write(sock, _user, strlen(_user));
    write(sock, " ", 1);
    write(sock, _password, strlen(_password));
    write(sock, " ", 1);
    write(sock, args, strlen(args));
    write(sock, " ", 1);
    write(sock, message, strlen(message));
    write(sock, "\r\n",2);

    // Keep reading until connection is closed or MAX_REPONSE
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    *(response+len) = '\0';

    close(sock);
    return 1;
}

void talk_client::printUsage()
{
    printf("Usage: talk-client host port user password\n");
    exit(1);
}

int talk_client::add_user(const char *_user,const char *_password) {
    // Try first to add user in case it does not exist.
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "ADD-USER", _user, _password, "", "", response);

    if (!strcmp(response,"OK\r\n")) {
        printf("User %s added\n", _user);
        return 1;
    }
    else {
        printf("User %s already exists\n", _user);
        return 0;
    }
}

int talk_client::login(const char*_user,const char *_password)
{
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "LOGIN", _user, _password, "", "", response);

    if (!strcmp(response,"OK\r\n")) {
        printf("Logged in\n");
        return 1;
    }
    else {
        printf("Wrong password\n");
        return 0;
    }
}

int talk_client::add_room(const char *room)
{
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "CREATE-ROOM", global_user, global_password, room, "", response);

    if (!strcmp(response,"OK\r\n")) {
        printf("Room %s added\n", room);
        return 1;
    }
    else {
        printf("Room %s already exists\n",room);
        return 0;
    }
}

char * talk_client::get_rooms(char *allRooms) {
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "GET-ALL-ROOMS", global_user, global_password, "", "", response);

    if(!strcmp(response,"ERROR (Wrong password)\r\n") || !strcmp(response,"No rooms\r\n"))
        allRooms = "";
    else
    {
        int len = strlen(response);
        *(response+len-2) = '\0';
        allRooms = response;
    }

    return allRooms;
}

char * talk_client::get_users_in_room(char *allUsers) {
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "GET-USERS-IN-ROOM", global_user, global_password, room_name, "", response);

    int len = strlen(response);
    *(response+len-2) = '\0';
    allUsers = response;

    return allUsers;
}

int talk_client::send_message(const char * message) {
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "SEND-MESSAGE", global_user, global_password, room_name, message, response);

    return 1;
}

char * talk_client::get_messages(char *allMessages) {
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "GET-MESSAGES", global_user, global_password, "-1", room_name, response);

    if(!strcmp(response,""))
        allMessages = "";
    else
    {
        int len = strlen(response);
        *(response+len-2) = '\0';
        allMessages = response;
    }

    return allMessages;
}

int talk_client::enter_room(const char *room) {
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "ENTER-ROOM", global_user, global_password, room, "", response);

    if (!strcmp(response,"OK\r\n")) {
        printf("Entered room %s\n", room);
        return 1;
    }
    else {
        return 0;
    }
}

int talk_client::leave_room() {
    char response[ MAX_RESPONSE ];
    sendCommand(host, port, "LEAVE-ROOM", global_user, global_password, room_name, "", response);

    if (!strcmp(response,"OK\r\n")) {
        printf("Left room %s\n", room_name);
        return 1;
    }
    else {
        return 0;
    }
}

void talk_client::printPrompt() {
    printf("talk> ");
    fflush(stdout);
}

void talk_client::printHelp() {
    printf("Commands:\n");
    printf(" -who   - Gets users in room\n");
    printf(" -users - Prints all registered users\n");
    printf(" -help  - Prints this help\n");
    printf(" -quit  - Leaves the room\n");
    printf("Anything that does not start with \"-\" will be a message to the chat room\n");
}

void * talk_client::getMessagesThread(void * arg) {
    // This code will be executed simultaneously with main()
    // Get messages to get last message number. Discard the initial Messages

    while (1) {
        // Get messages after last message number received.

        // Print messages

        // Sleep for ten seconds
        usleep(2*1000*1000);
    }
}

void talk_client::startGetMessageThread()
{
    //pthread_create(NULL, NULL, getMessagesThread, NULL);
}
