#include "socket.hpp"

// https://stackoverflow.com/questions/8125507/how-can-i-send-and-receive-websocket-messages-on-the-server-side/27442080#27442080

std::thread web::web_socket::chat_reflect_handler;

void web::web_socket::send_handshake(web::http::Request *request) {
  std::cout << "/+NEW WEBSOCKET CONNECTION: " << std::endl;

  const std::string socketKey =
      request->GetHeader().GetValueRepresentation("Sec-Websocket-Key");

  std::string key =
      socketKey.substr(0, socketKey.length() - 1) + websocket_hash;
  std::cout << "/+" << key << " : " << key.length() << std::endl;
  macaron::Base64 base64;

  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA_CTX sha1;
  SHA1_Init(&sha1);
  SHA1_Update(&sha1, key.c_str(), key.length());
  SHA1_Final(hash, &sha1);

  std::stringstream ss;
  std::stringstream content;

  for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    ss << hash[i];
  }

  std::cout << ss.str() << std::endl;
  content << "HTTP/1.1 101 Switching Protocols\r\n";
  content << "Connection: Upgrade\r\n";
  content << "Upgrade: websocket\r\n";
  // content << "Sec-WebSocket-Protocol: ws-connection\r\n";
  // content << "Sec-WebSocket-Version: 13\r\n";
  content << "Sec-WebSocket-Accept: ";
  content << base64.Encode(ss.str());
  content << "\r\n";
  content << "\r\n";
  std::cout << "/+		HEADER\r\n"
            << content.str() << "+/" << std::endl;

  send(request->fdRequest, content.str().c_str(), content.str().length(), 0);
  std::cout << "/+END OF NEW WEBSOCKET CONNECTION. " << std::endl;
}

std::string web::web_socket::parse_readed_content(std::string toParse) {
  int readed = 1;
  // opcode it's at index 1
  int opcode = (int)toParse.substr(0, 1)[0];
  std::cout << "+parse_content+opcode: " << opcode << std::endl;

  readed = 2;
  if (readed > toParse.length()) {
    std::cout << "[PARSE_WEBSOCKET]Content invalid" << std::endl;
    return "";
  }
  // content length it's at index 2
  // content length = (index2 - 128)
  int contentLength = (int)toParse.substr(1, 1)[0] & 0x7f;
  // std::cout << "+parse_content+contentLength: " << contentLength <<
  // std::endl;

  readed = 6;
  if (readed > toParse.length()) {
    std::cout << "[PARSE_WEBSOCKET]Content invalid" << std::endl;
    return "";
  }
  // mas key it's at index 3~6
  std::string maskKey = toParse.substr(2, 4);
  // std::cout << "+parse_content+maskKey: " << maskKey << std::endl;

  readed = 6 + contentLength;
  if (readed > toParse.length()) {
    std::cout << "[PARSE_WEBSOCKET]Content invalid" << std::endl;
    return "";
  }
  // from 6 it's only content
  std::string content = toParse.substr(6, contentLength);
  // std::cout << "+parse_content+contents" << contents << std::endl;

  for (int i = 0; i < contentLength; i++) {
    char mask = maskKey[i % 4];
    content[i] = (char)(content[i] ^ mask);
  }

  if (readed < toParse.length()) {
    content +=
        parse_readed_content(toParse.substr(readed, toParse.length() - readed));
  }

  return content;
}

std::string web::web_socket::encrypt_message(std::string message) {
  std::stringstream messageMasked;
  // Add handle to message length > 127
  messageMasked << (char)129;
  messageMasked << (char)message.length();
  messageMasked << message;

  return messageMasked.str();
}

void web::web_socket::send_message(int fdClient, std::string message) {
  std::string encryptMessage = encrypt_message(message);
  send(fdClient, encryptMessage.c_str(), encryptMessage.length(), 0);
}

void web::web_socket::send_messaeg(int listFdClient[], std::string message) {
  std::string encryptMessage = encrypt_message(message);
  int clientsLength = *(&listFdClient + 1) - listFdClient;
  for (int i = 0; i < clientsLength; i++) {
    send(listFdClient[i], encryptMessage.c_str(), encryptMessage.length(), 0);
  }
}
