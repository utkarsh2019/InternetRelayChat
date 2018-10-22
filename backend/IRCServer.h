#include <map>
#include <string>
#ifndef IRC_SERVER
#define IRC_SERVER

#define PASSWORD_FILE "password.txt"

using namespace std;

struct Message {
	string sentence;
	string message_user;
};

class Room {
	map <string,string> user_in_room;
	Message messages[100];
	int n;
	friend class IRCServer;
   public:
	Room();
};

Room::Room() : n(0)
{
}

class IRCServer {
	map <string,string> users;
	map <string,Room> rooms;

private:
	int open_server_socket(int port);

public:
	void initialize();
	bool checkPassword(int fd, const char * user, const char * password);
	void processRequest( int socket );
	void login(int fd, const char * user,const char * password,const char * args);
	void addUser(int fd, const char * user, const char * password, const char * args);
	void createRoom(int fd, const char * user, const char * password, const char * args);
	void enterRoom(int fd, const char * user, const char * password, const char * args);
	void leaveRoom(int fd, const char * user, const char * password, const char * args);
	void getAllRooms(int fd, const char * user, const char * password, const char * args);
	void sendMessage(int fd, const char * user, const char * password, const char * args, const char * message);
	void getMessages(int fd, const char * user, const char * password, const char * args, const char * message);
	void getUsersInRoom(int fd, const char * user, const char * password, const char * args);
	void getAllUsers(int fd, const char * user, const char * password, const char * args);
	void runServer(int port);
};

#endif
