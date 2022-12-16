#include <iostream>
#include <map> // std::map
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector
#include <thread> // std::thread
#include <queue> // std::queue
#include <netinet/in.h> // socket
#include <unistd.h> // read/write/close from fd
#include <sstream> // std::stringstream
#include "http/request.hpp" // Request class
#include "http/page.hpp" // Page class
#include <sys/socket.h>
#include "socket/socket.hpp"
#include "socket/socketHandler.hpp"
#include <sys/poll.h> // poll socket

#ifndef PORT
//#define PORT 8080
//#define PORT 443
#define PORT 3000
#endif

#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP
namespace web{
namespace server{

template<typename T> web::http::Page * create_page(web::http::Request *req, web::http::Response *res) { return new T(req, res); }
using create_page_method = web::http::Page * (*)(web::http::Request *req, web::http::Response *res);

extern int fdServer;
extern bool server_running;
// fd_connections <fd_client, operation>
extern std::unordered_map<int, std::string> fd_connections;
extern std::queue<web::http::Request> new_web_socket_connections;
extern std::map<std::string, create_page_method> pages;
extern std::queue<web::http::Request> pages_to_handle;

extern  std::thread request_listener;
extern  std::thread page_handler;
extern  std::thread websocket_handler;

void start_server();
void close_server();
bool is_running();

void register_fd_connection(int fdClient, std::string alias);
void remove_fd_connection(int fdClient);

void listen_request();
void handle_websocket();
void handle_page();

void register_page(std::string pageName, create_page_method page);

}
}
#endif