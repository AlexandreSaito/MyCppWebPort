#include "socketHandler.hpp"

void web::web_socket::handle(int fdClient, std::string path, std::string message) {
  std::cout << "[WEBSOCKET_HANDLER] FD: " << fdClient << " Path: " << path << " Message: " << message
            << std::endl;

	if(path == "/" || path == ""){
		web::web_socket::reflect_socket.register_message(fdClient, message);
	}else if(path == "/chat"){
		web::web_socket::chat_socket.register_message(fdClient, message);
	}else if(path == "/game"){
		web::web_socket::game_socket.register_message(fdClient, message);
	}
}

void web::web_socket::handle_chat_reflect(){
	web::web_socket::chat_socket.read_message();
	web::web_socket::reflect_socket.read_message();
	// renove later
	web::web_socket::game_socket.read_message();
}
