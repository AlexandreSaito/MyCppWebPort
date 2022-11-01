#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <vector>
#include <thread>
#include "./../customFunctions.hpp"
#include "page.hpp"
#include <unordered_map>
#include <openssl/sha.h>
#include <iomanip>
#include "./../base64.hpp"

#ifndef PORT
//#define PORT 8080
//#define PORT 443
#define PORT 3000
#endif

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

namespace http{

template<typename T> Page * create_page(Request *req, Response *res) { return new T(req, res); }
using create_page_method = Page * (*)(Request *req, Response *res);

class Server{
private:
bool running;
int socketfd;
std::thread responseThread;

public:
std::map<std::string, create_page_method> pages;
void start_server();
void close_server();
bool is_running();
};

extern Server *server;

void register_page(std::string pageName, create_page_method page);
//void listen_request(Server*server, int socketfd);

} // namespace http

#endif