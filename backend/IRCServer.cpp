
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "IRCServer.h"

Room a[100];
int i=0;
int QueueLength = 5;

//test

int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress;
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);

	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1;
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR,
			     (char *) &optval, sizeof( int ) );

	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}

	// Put socket in listening mode and set the
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();

	while ( 1 ) {

		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);

		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}

		// Process request.
		processRequest( slaveSocket );
	}

}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}

	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);

}

//
// Commands:
//   Commands are started y the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;

	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;

	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}

		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}

	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

	//The commandLine has the following format:
	//COMMAND <user> <password> <arguments>. See below.

	//Separating commandLine components

	int i=0,j=0;
	string s1[5];
	while(commandLine[i] != '\0')
	{
		if(j<4)
		{
			while(commandLine[i] != ' ' && commandLine[i] != '\0')
			{
				s1[j].push_back(commandLine[i]);
				i++;
			}
		}
		else if(j == 4)
		{
			while(commandLine[i] != '\0')
                        {
                                s1[j].push_back(commandLine[i]);
                                i++;
                        }
		}
		j++;
		if(commandLine[i] != '\0')
			i++;
	}

	const char * command = s1[0].c_str();
	const char * user = s1[1].c_str();
	const char * password = s1[2].c_str();
	const char * args;
	const char * message;
	if (s1[0] != "GET-MESSAGES")
	{
		args = s1[3].c_str();
		message = s1[4].c_str();
	}
	else
	{
		args = s1[4].c_str();
		message = s1[3].c_str();
	}

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);
	printf("message=%s\n", message);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "LOGIN")) {
		login(fd, user, password, args);
	}
        else if (!strcmp(command, "CREATE-ROOM")) {
                createRoom(fd, user, password, args);
        }
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args, message);
	}
	else if (!strcmp(command, "GET-ALL-ROOMS")) {
		getAllRooms(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args, message);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	//Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);
}

void
IRCServer::initialize()
{
	// Opening password file

	// Initialize users in room

	// Initalize message list
}

bool
IRCServer::checkPassword(int fd, const char * user, const char * password) {
	string str1(user);
	string str2(password);

	//Finding if user exists
	if(users.find(str1) == users.end())
		return false;

	//Checking user's password
	if (users[str1] == password)
		return true;
	else
		return false;
}

void
IRCServer::addUser(int fd, const char * user, const char * password, const char * args)
{
	string str1(user);
	string str2(password);

	//Checking if user already exists
	if(users.find(str1) != users.end())
	{
	        const char * msg =  "DENIED\r\n";
        	write(fd, msg, strlen(msg));
		return;
	}

        FILE *file = fopen(PASSWORD_FILE,"w+");

	fseek(file,-1,SEEK_END);

	fputs(str1.c_str(),file);
	fputs("\n",file);
	fputs(str2.c_str(),file);
	fputs("\n",file);

	fclose(file);

	//Adding user
	users[str1] = str2;
	const char * msg =  "OK\r\n";
	write(fd, msg, strlen(msg));

	return;
}

void
IRCServer::login(int fd,const char * user,const char * password,const char *args)
{
	string str1(user);
	string str2(password);

	//Checking password
        if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "DENIED\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	else
	{
		const char * msg = "OK\r\n";
	        write(fd, msg, strlen(msg));
		return;
	}
}

void
IRCServer::createRoom(int fd,const char * user,const char * password,const char * args)
{
        string str1(user);
        string str2(password);
        string str3(args);

	//Checking password
        if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "DENIED\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if room already exists
	else if(rooms.find(str3) != rooms.end())
	{
		const char * msg =  "DENIED\r\n";
                write(fd, msg, strlen(msg));
                return;
	}

	//Creating room
	rooms[str3] = a[i++];
        const char * msg =  "OK\r\n";
        write(fd, msg, strlen(msg));

        return;

}

void
IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args)
{
        string str1(user);
        string str2(password);
	string str3(args);

	//Checking password
	if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if room doesnt exist
	else if(rooms.find(str3) == rooms.end())
	{
                const char * msg =  "ERROR (No room)\r\n";
                write(fd, msg, strlen(msg));
                return;
	}

	//Adding user to room
	rooms[str3].user_in_room[str1] = str2;
        const char * msg =  "OK\r\n";
        write(fd, msg, strlen(msg));

        return;
}

void
IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{
        string str1(user);
        string str2(password);
        string str3(args);

	//Checking Password
        if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if room doesnt exist
        else if(rooms.find(str3) == rooms.end())
        {
                const char * msg =  "ERROR (No room)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if user isnt in room
	else if(rooms[str3].user_in_room.find(str1) == rooms[str3].user_in_room.end())
        {
                const char * msg =  "ERROR (No user in room)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }

	//Removing user from room
	rooms[str3].user_in_room.erase(str1);
        const char * msg =  "OK\r\n";
        write(fd, msg, strlen(msg));

        return;
}

void
IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args, const char * message)
{
        string str1(user);
        string str2(password);
        string str3(args);
	string str4(message);

	//Checking password
        if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if room doesnt exist
        else if(rooms.find(str3) == rooms.end())
        {
                const char * msg =  "ERROR (No room)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if user isnt in room
        else if(rooms[str3].user_in_room.find(str1) == rooms[str3].user_in_room.end())
        {
                const char * msg =  "ERROR (user not in room)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }

	//Adding message to room
	int j = (rooms[str3].n)++;
	rooms[str3].messages[j].message_user = str1;
	rooms[str3].messages[j].sentence = str4;

	//Checking if message is full
	if(rooms[str3].n == 100)
		rooms[str3].n = 0;
	const char * msg =  "OK\r\n";
	write(fd, msg, strlen(msg));

}

void
IRCServer::getMessages(int fd, const char * user, const char * password, const char * args,const char * message)
{
        string str1(user);
        string str2(password);
        string str3(args);
        int n = atoi(message);

	//Checking password
        if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if room doesnt exist
        else if(rooms.find(str3) == rooms.end())
        {
                const char * msg =  "ERROR (No room)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if user isnt in room
        else if(rooms[str3].user_in_room.find(str1) == rooms[str3].user_in_room.end())
        {
                const char * msg =  "ERROR (User not in room)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }
	//Checking if last_message_no is out of bounds
	else if(n+1 > rooms[str3].n)
        {
                const char * msg =  "NO-NEW-MESSAGES\r\n";
                write(fd, msg, strlen(msg));
                return;
        }

	//Printing messages from last_message_no to end
	int j;
	for(j = n+1;rooms[str3].messages[j].sentence != "\0";j++)
	{
		char str[12];
		sprintf(str,"%d",j);
		string msg(str);
		msg.push_back(' ');
		msg.append(rooms[str3].messages[j].message_user);
		msg.push_back(' ');
		msg.append(rooms[str3].messages[j].sentence);
		msg.append("\r\n");
		write(fd, msg.c_str(), msg.length());
	}

	const char * msg =  "\r\n";
	write(fd, msg, strlen(msg));
}

void
IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{
	//Checking password
        if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }

	string str3(args);

	//Printing all users in room
        for(map<string,string>::iterator it = rooms[str3].user_in_room.begin();it != rooms[str3].user_in_room.end(); ++it)
        {
                string msg =  it->first;
                msg.push_back('\r');
                msg.push_back('\n');
                write(fd, msg.c_str(), msg.length());
        }

	const char * msg =  "\r\n";
	write(fd, msg, strlen(msg));
}

void
IRCServer::getAllUsers(int fd, const char * user, const char * password,const  char * args)
{
	//Checking password
	if(!(checkPassword(fd,user,password)))
	{
                const char * msg =  "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
		return;
	}

	//Printing all users
	for(map<string,string>::iterator it = users.begin();it != users.end(); ++it)
	{
                string msg =  it->first;
		msg.push_back('\r');
		msg.push_back('\n');
                write(fd, msg.c_str(), msg.length());
	}

	const char* msg =  "\r\n";
	write(fd, msg, strlen(msg));
}

void
IRCServer::getAllRooms(int fd, const char * user, const char * password, const char * args)
{
        string str3(args);

        //Checking password
        if(!(checkPassword(fd,user,password)))
        {
                const char * msg =  "ERROR (Wrong password)\r\n";
                write(fd, msg, strlen(msg));
                return;
        }

	map<string,Room>::iterator it=rooms.begin();
	if(it == rooms.end())
	{
		const char * msg = "No rooms\r\n";
		write(fd, msg, strlen(msg));
                return;
	}

        //Printing all users
        for(it = rooms.begin();it != rooms.end(); ++it)
        {
                string msg =  it->first;
                msg.push_back('\n');
                write(fd, msg.c_str(), msg.length());
        }

        const char* msg =  "\r\n";
        write(fd, msg, strlen(msg));
}
