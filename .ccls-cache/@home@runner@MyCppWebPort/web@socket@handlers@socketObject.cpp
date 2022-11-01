#include "socketObject.hpp"

web::web_socket::SocketHandler::SocketHandler() { this->socketName = ""; }

void web::web_socket::SocketHandler::register_message(int clientFd,
                                                      std::string message) {
  struct SocketMessage socMessage;
  socMessage = parse_json_message(message);
  socMessage.clientFd = clientFd;
  this->messagesToHandle.push(socMessage);
}

void web::web_socket::SocketHandler::read_message() {
  while (!this->messagesToHandle.empty()) {
    SocketMessage message = this->messagesToHandle.front();
    this->messagesToHandle.pop();

    if (!message.parsed)
      continue;

    std::cout << "Call handle to " << this->socketName
              << " client fd: " << message.clientFd
              << " message: " << message.message << std::endl;
    this->handle(message);
  }
}

void web::web_socket::SocketHandler::handle(SocketMessage socketMessage) {
  std::cout << "[SOCKET_HANDLER] NOT HANDLING ANYTHING" << std::endl;
}

web::web_socket::SocketMessage
web::web_socket::SocketHandler::parse_json_message(std::string message) {
  struct SocketMessage socMessage;
  socMessage.parsed = false;
  Document d;
  d.Parse(message.c_str());
  if (d.HasParseError()) {
    std::cout << "[ERROR] Failed to parse json message \r\nMessage: " << message
              << std::endl
              << std::endl;
    return socMessage;
  }

  if (c_json::check_value(&d, "reconnect") > 0)
    socMessage.reconnect = d["reconnect"].GetBool();

  if (c_json::check_value(&d, "message") > 0) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d["message"].Accept(writer);
    socMessage.message = buffer.GetString();
  }

  socMessage.parsed = true;
  return socMessage;
}