#include <iostream>

#include "handlers/chat.hpp"
#include "handlers/reflect.hpp"

#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H
namespace web{
namespace web_socket{

void handle(int fdClient, std::string path, std::string message);

void handle_chat_reflect();

}
}
#endif