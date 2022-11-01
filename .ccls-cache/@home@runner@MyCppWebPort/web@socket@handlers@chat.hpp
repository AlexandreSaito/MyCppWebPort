#include <iostream>
#include <map>
#include "socketObject.hpp"
#include "./../socket.hpp"

#ifndef SOCKET_CHAT_H
#define SOCKET_CHAT_H

namespace web{
namespace web_socket{

class ChatSocket : public web::web_socket::SocketHandler{
protected:
	std::map<int, std::string> users;
public:
	explicit ChatSocket() : SocketHandler() {
		this->socketName = "Chat";
	}
	void handle(SocketMessage socketMessage);
	std::string write_json(std::string name, std::string message, bool accept);
};

extern ChatSocket chat_socket;

}
}

#endif