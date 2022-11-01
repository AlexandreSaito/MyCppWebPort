#include <iostream>
#include <vector>
#include "socketObject.hpp"
#include "./../socket.hpp"

#ifndef SOCKET_REFLECT_H
#define SOCKET_REFLECT_H

namespace web{
namespace web_socket{

class ReflectSocket : public web::web_socket::SocketHandler{
protected:
	std::vector<int> fds;
public:
	explicit ReflectSocket() : SocketHandler() {}
	void handle(SocketMessage socketMessage);
};

extern ReflectSocket reflect_socket;

}
}

#endif