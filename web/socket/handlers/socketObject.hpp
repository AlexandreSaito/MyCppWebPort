#include <iostream>
#include <queue> // std::queue
#include "./../../../c_json.hpp"

#ifndef SOCKET_OBJECT_H
#define SOCKET_OBJECT_H

namespace web{
namespace web_socket{

struct SocketMessage{
	int clientFd;
	int op;
	std::string clientToken;
	bool reconnect;
	std::string message;

	bool parsed;
};

class SocketHandler{
protected:
	std::string socketName;
	std::queue<SocketMessage> messagesToHandle;
public:
	SocketHandler();
	void register_message(int clientFd, std::string message);
	SocketMessage parse_json_message(std::string message);
	virtual void read_message();
	virtual void handle(SocketMessage socketMessage);
};

}
}

#endif