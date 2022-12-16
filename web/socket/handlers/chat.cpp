#include "chat.hpp"

web::web_socket::ChatSocket web::web_socket::chat_socket;
// extern web::web_socket::ChatSocket chat_socket();
void web::web_socket::ChatSocket::handle(SocketMessage socketMessage) {
  std::cout << "[SOCKET_CHAT] Handle chat..." << std::endl;

  Document document;
	if(socketMessage.message != ""){
	  document.Parse(socketMessage.message.c_str());
	  if (document.HasParseError()) {
	    std::cout << "[ERROR-CHAT] Failed to parse SocketMessage \r\nMessage: "
	              << socketMessage.message << std::endl
	              << std::endl;
	    return;
	  }
	}

	if(socketMessage.op == -1){
    std::cout << "[ERROR-CHAT] SYNTAX - MISSING 'op' PARAM" << std::endl;
		return;
	}

	switch(socketMessage.op){
		case 1: // Create User
  		this->handleOP1(socketMessage.clientFd, document);
		break;
		case 3: // Send message
			this->handleOP3(socketMessage.clientFd, document);
		break;
		case 4: // Send users list
			web::web_socket::send_message(socketMessage.clientFd, this->write_user_name_list(socketMessage.clientFd));
		break;
	}
	
}

void web::web_socket::ChatSocket::handleOP1(int fd, const Document& d){
	auto user_send = this->users.find(fd);
	if (user_send != this->users.end()){
		web::web_socket::send_message(fd, this->write_user_name_json(false, "Já adicionado!"));
		return;
	}
	if (!d.HasMember("userName")){
		web::web_socket::send_message(fd, this->write_user_name_json(false, "Faltando 'userName' param!"));
		return;
	}
	
	std::string userName = d["userName"].GetString();

	bool canAdd = true;
	for (auto it = this->users.begin(); it != this->users.end(); ++it){
		if (it->second == userName){
			canAdd = false;
			break;
		}
	}
	
	if(canAdd){
		this->users.insert(std::pair<int, std::string>(fd, userName));
		web::web_socket::send_message(fd, this->write_user_name_json(true, ""));
	  for (auto it : users) {
			if(it.first == fd)
				continue;
			web::web_socket::send_message(it.first, write_new_user(fd, userName));
		}
		return;
	}
	
	web::web_socket::send_message(fd, this->write_user_name_json(false, "Nome de usuario já está sendo utilizado!"));
}

void web::web_socket::ChatSocket::handleOP3(int fd, const Document& d){
  if (!d.HasMember("message")){
    std::cout << "[ERROR-CHAT] SYNTAX - MISSING 'message' PARAM" << std::endl;
    return;
	}
	auto user_send = this->users.find(fd);
	if (user_send == this->users.end()){
    std::cout << "[ERROR-CHAT] User not added yet" << std::endl;
		return;
	}

  std::string message = d["message"].GetString();
  for (auto it : users) {
    if (it.first == fd) {
      web::web_socket::send_message(it.first, this->write_message_json(user_send->second, message, true));
      continue;
    }

    web::web_socket::send_message(it.first, this->write_message_json(user_send->second, message, false));
  }
}

std::string web::web_socket::ChatSocket::write_user_name_json(bool accepted, std::string complement){
	StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  writer.StartObject();
  writer.Key("op");
  writer.Int(1);
  writer.Key("accepted");
  writer.String(accepted ? "true" : "false");
  writer.Key("complement");
  writer.String(complement.c_str());
  writer.EndObject();
  return buffer.GetString();
}

std::string web::web_socket::ChatSocket::write_message_json(std::string senderName, std::string message, bool accept) {
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  writer.StartObject();
  writer.Key("op");
  writer.Int(3);
  writer.Key("name");
  writer.String(senderName.c_str());
  writer.Key("message");
  writer.String(message.c_str());
  writer.Key("accepted");
  writer.String(accept ? "true" : "false");
  writer.EndObject();
  return buffer.GetString();
}

std::string web::web_socket::ChatSocket::write_user_name_list(int fd) {
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  writer.StartObject();
  writer.Key("op");
  writer.Int(4);
  writer.Key("users");
	writer.StartArray();
	
  for (auto it : users) {
		if(it.first == fd)
			continue;
  	writer.StartObject();
	  writer.Key("id");
	  writer.Int(it.first);
	  writer.Key("name");
	  writer.String(it.second.c_str());
  	writer.EndObject();
	}
	
	writer.EndArray();
  writer.EndObject();
  return buffer.GetString();
}

std::string web::web_socket::ChatSocket::write_new_user(int id, std::string name) {
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  writer.StartObject();
  writer.Key("op");
  writer.Int(5);
  writer.Key("user");
	
	writer.StartObject();
	writer.Key("id");
	writer.Int(id);
	writer.Key("name");
	writer.String(name.c_str());
	writer.EndObject();
		
  writer.EndObject();
  return buffer.GetString();
}