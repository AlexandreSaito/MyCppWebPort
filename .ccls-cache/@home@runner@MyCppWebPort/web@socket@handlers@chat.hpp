#include <iostream>
#include <map>
#include "socketObject.hpp"
#include "./../socket.hpp"

#ifndef SOCKET_CHAT_H
#define SOCKET_CHAT_H

namespace web{
namespace web_socket{

/*
OP:
1 - ENTER USER
	Receive 'userName' Param
2 - CHANGE USER NAME
3 - SEND MESSAGE
4 - REQUEST USER LIST
5 - SEND ONLY TO CLIENT SIDE -> SEND NEW USER WHEN USER ADDED
*/

class ChatSocket : public web::web_socket::SocketHandler{
protected:
	std::map<int, std::string> users;
public:
	explicit ChatSocket() : SocketHandler() {
		this->socketName = "Chat";
	}
	void handle(SocketMessage socketMessage);
	void handleOP1(int fd, const Document& d);
	void handleOP3(int fd, const Document& d);
	std::string write_user_name_json(bool accepted, std::string complement);
	std::string write_message_json(std::string senderName, std::string message, bool accept);
	std::string write_user_name_list(int fd);
	std::string write_new_user(int id, std::string name);
};

extern ChatSocket chat_socket;

}
}

#endif