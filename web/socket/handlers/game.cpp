#include "game.hpp"

web::web_socket::GameSocket web::web_socket::game_socket;
std::vector<web::web_socket::Lobby> web::web_socket::lobbyList;

void web::web_socket::GameSocket::handle(SocketMessage socketMessage){
	std::cout << "[SOCKET_GAME] Handle game..." << std::endl;

  Document document;
	if(socketMessage.message != ""){
	  document.Parse(socketMessage.message.c_str());
	  if (document.HasParseError()) {
	    std::cout << "[ERROR-GAME] Failed to parse SocketMessage \r\nMessage: "
	              << socketMessage.message << std::endl
	              << std::endl;
	    return;
	  }
	}

	if(socketMessage.op == -1){
    std::cout << "[ERROR-GAME] SYNTAX - MISSING 'op' PARAM" << std::endl;
		return;
	}
	/*
OP:
1 - ENTER USER
	Receive 'userName' Param
2 - CHANGE USER NAME
3 - LOBBY MANAGER
8 - IN GAME ACTIONS
9 - DISCONNECT
*/
	switch(socketMessage.op){
		case 1: // ENTER USER
			this->handleOP1(socketMessage.clientFd, document);
		break;
		case 8: // IN GAME ACTIONS
			this->handleOP8(socketMessage.clientFd, document);
		break;
		case 9: // DISCONNECT
		break;
	}
	
}

void web::web_socket::GameSocket::handleOP1(int fd, Document& d){
	if(lobbyList.size() == 0){
		Lobby nLobby;
		nLobby.id = 1;
		nLobby.name = "Um nome";
		lobbyList.insert(lobbyList.begin(), nLobby);
	}

	Lobby* lobby = &lobbyList[0];
	if(!d.HasMember("userName")){
		std::cout << "[SOCKET_GAME] not found 'userName' " << std::endl;
		return;
	}

	lobby->players.insert(std::make_pair(fd, d["userName"].GetString()));

	Document::AllocatorType& allocator = d.GetAllocator();
	d.AddMember("op", 1, allocator);
	d.AddMember("playerId", fd, allocator);
	
	for (auto it = lobby->players.begin(); it != lobby->players.end(); ++it){
		if (it->first == fd){
			// send confirmation
			d.AddMember("accept", "true", allocator);
			web::web_socket::send_message(it->first, this->parseDocument(d));
			d.RemoveMember("accept");
			continue;
		}
		// send message
		web::web_socket::send_message(it->first, this->parseDocument(d));
	}
	
}

void web::web_socket::GameSocket::handleOP8(int fd, Document& d){
	Lobby *lobby;
	bool hasLobby = false;
	for (int i=0; i < lobbyList.size(); i++){
		if(lobbyList[i].players.find(fd) != lobbyList[i].players.end()){
			lobby = &lobbyList[i];
			hasLobby = true;
		}
	}

	if(!hasLobby)
		return;
	
	Document::AllocatorType& allocator = d.GetAllocator();
	d.AddMember("playerId", fd, allocator);
	d.AddMember("op", 8, allocator);
	
	for (auto it = lobby->players.begin(); it != lobby->players.end(); ++it){
		if (it->first == fd){
			// send confirmation
			d.AddMember("accept", "true", allocator);
			web::web_socket::send_message(it->first, this->parseDocument(d));
			d.RemoveMember("accept");
			continue;
		}
		// send message
		web::web_socket::send_message(it->first, this->parseDocument(d));
	}
	
}

std::string web::web_socket::GameSocket::parseDocument(const Document& d){
  StringBuffer buffer;

  Writer<rapidjson::StringBuffer> writer(buffer);
  d.Accept(writer);

	return buffer.GetString();
}