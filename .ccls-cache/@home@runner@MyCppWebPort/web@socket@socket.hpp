#include <iostream>
#include <netinet/in.h> // socket
#include <unistd.h> // read/write/close from fd
#include <sstream> // std::stringstream
#include <thread> // std::thread
#include <sys/socket.h>
#include <openssl/sha.h> // sha1 cryptography
#include "./../../base64.hpp" // encode/decode base64
#include "./../http/request.hpp"

#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H
namespace web{
namespace web_socket{

const std::string websocket_hash = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

extern std::thread chat_reflect_handler;

void send_handshake(web::http::Request *request);

std::string parse_readed_content(std::string toParse);

std::string encrypt_message(std::string message);

void send_message(int fdClient, std::string message);

void send_messaeg(int listFdClient[], std::string message);

std::string parse_content(std::vector<char> toParse);

}
}
#endif