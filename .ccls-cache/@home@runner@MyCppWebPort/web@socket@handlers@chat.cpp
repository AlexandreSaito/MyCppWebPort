#include "chat.hpp"

web::web_socket::ChatSocket web::web_socket::chat_socket;
// extern web::web_socket::ChatSocket chat_socket();
void web::web_socket::ChatSocket::handle(SocketMessage socketMessage) {
  std::cout << "[SOCKET_CHAT] Handle chat..." << std::endl;

  Document d;
  d.Parse(socketMessage.message.c_str());
  if (d.HasParseError()) {
    std::cout << "[ERROR-CHAT] Failed to parse SocketMessage \r\nMessage: "
              << socketMessage.message << std::endl
              << std::endl;
    return;
  }

  auto user_send = users.find(socketMessage.clientFd);
  if (user_send == users.end()) {
    if (!d.HasMember("userName"))
      return;

    users.insert(std::pair<int, std::string>(socketMessage.clientFd,
                                             d["userName"].GetString()));
    return;
  }

  if (!d.HasMember("message"))
    return;

  std::string message = d["message"].GetString();
  for (auto it : users) {
    if (it.first == socketMessage.clientFd) {
      web::web_socket::send_message(
          it.first, this->write_json(user_send->second, message, true));
      continue;
    }

    web::web_socket::send_message(
        it.first, this->write_json(user_send->second, message, false));
  }
}

std::string web::web_socket::ChatSocket::write_json(std::string name, std::string message,
                                                    bool accept) {
  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);

  writer.StartObject();
  writer.Key("name");
  writer.String(name.c_str());
  writer.Key("message");
  writer.String(message.c_str());
  writer.Key("accepted");
  writer.String(accept ? "true" : "false");
  writer.EndObject();
  return buffer.GetString();
}