#include <iostream>
#include <map>
#include "socketObject.hpp"
#include "./../socket.hpp"

#ifndef SOCKET_GAME_H
#define SOCKET_GAME_H

namespace web{
namespace web_socket{

struct Lobby{
	int id;
	std::string name;
	std::string description;
 	std::map<int, std::string> players;	
};

/*
OP:
1 - ENTER USER
	Receive 'userName' Param
2 - CHANGE USER NAME
3 - LOBBY MANAGER
8 - IN GAME ACTIONS
9 - DISCONNECT
*/

class GameSocket : public web::web_socket::SocketHandler{
protected:
public:
	explicit GameSocket() : SocketHandler() {
		this->socketName = "Game";
	}
	void handle(SocketMessage socketMessage);
	void handleOP1(int fd, Document& d);
	void handleOP8(int fd, Document& d);
	std::string parseDocument(const Document& d);
};

extern GameSocket game_socket;

extern std::vector<Lobby> lobbyList;

}
}

#endif