#include <iostream>
#include <fstream>

#include "web/http/content.hpp"
#include "web/server.hpp"
#include "web/webPages/registerPages.hpp"

#include "c_json.hpp"
// clang++ main.cpp -compiler_options_here -o main

// WEBSOCKET
// https://developer.mozilla.org/pt-BR/docs/Web/API/WebSockets_API/Writing_WebSocket_servers
// https://developer.mozilla.org/pt-BR/docs/Web/API/WebSockets_API
// https://developer.mozilla.org/pt-BR/docs/Web/API/WebSockets_API/Writing_WebSocket_client_applications

// CACHE CONTROL
// https://developer.mozilla.org/pt-BR/docs/Web/HTTP/Headers/Cache-Control

// ASCII
// https://www.rapidtables.com/code/text/ascii-table.html

// JSON
// https://github.com/Tencent/rapidjson

void start_socket_server() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Thread runned" << std::endl;
  }
}

int main() {
	web::http::register_content_type();
	web::register_pages();
	web::server::start_server();
	
	int command;
  while (web::server::is_running()) {
    std::cout << "Digite [0] para fechar o http Server." << std::endl;
    std::cin >> command;
    if (command == 0) {
      std::cout << "Fechando servidor..." << std::endl;
      web::server::close_server();
      std::cout << "Servidor fechado" << std::endl;
    }
	}
	
  return 0;
}
