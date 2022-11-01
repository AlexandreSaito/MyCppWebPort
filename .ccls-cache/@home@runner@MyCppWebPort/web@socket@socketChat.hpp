//#include "./../server.hpp"

#include "socketHandler.hpp"

#ifndef SOCKET_CHAT_H
#define SOCKET_CHAT_H

namespace web{
namespace web_socket{

class ChatSocket : public web::web_socket::SocketHandler{
public:
	explicit ChatSocket() : SocketHandler() {}
	void handle();
};

const ChatSocket chat_socket;

}
}

#endif