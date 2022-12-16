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
  int toParseLength = toParse.size();
  // opcode it's at index 1
  int opcode = (int)toParse.substr(0, 1)[0] & 0x7f;
  std::cout << "[WS_PARSE] TO PARSE LENGTH: " << toParseLength << std::endl;
  std::cout << "[WS_PARSE] OPCODE: " << opcode << std::endl;
  int contentLength = (int)toParse.substr(1, 1)[0] & 0x7f;
  std::cout << "[WS_PARSE] Content Length: " << contentLength << std::endl;

  readed = 2;
  if (readed >= toParse.length()) {
    std::cout << "[WS_PARSE] Content invalid" << std::endl;
    return "";
  }
  // content length it's at index 2
  // content length = (index2 - 128)
  if (contentLength == 126) {
    int countOf256 = (int)toParse.substr(readed, 1)[0] & 0x7f;
    readed++;
    int contentLength2 = (int)toParse.substr(readed, 1)[0] & 0x7f;
    readed++;
    std::cout << "[WS_PARSE] Cont of 256: " << countOf256 << std::endl;
    std::cout << "[WS_PARSE] Content Length2: " << contentLength2 << std::endl;
    contentLength = (countOf256 * 256) + contentLength2;
  }
  std::cout << "[WS_PARSE] Content Length Final: " << contentLength
            << std::endl;

  if (readed + 4 > toParse.length()) {
    std::cout << "[WS_PARSE] Content invalid - Count with mask key is higher "
                 "then actual content length."
              << std::endl;
    return "";
  }
  // mas key it's at 4 bytes after content length
  std::string maskKey = toParse.substr(readed, 4);

  readed += 4;
  if (readed + contentLength > toParse.length()) {
    std::cout << "[WS_PARSE] Content invalid - Content Length it's higher then "
                 "actual content length. "
              << toParse.length() << " - " << (readed + contentLength)
              << std::endl;
    contentLength = toParse.length() - readed;
    // return "";
  }
  // after mask key all of it is content
  std::string content = toParse.substr(readed, contentLength);

  std::cout << "[WS_PARSE] Content Length Before Parse: " << contentLength
            << std::endl;
  for (int i = 0; i < contentLength; i++) {
    char mask = maskKey[i % 4];
    // std::cout << "i - " << i % 4 << "mask: " << (int)mask << " - Content: "
    // << (int)content[i] << " - result: " << (char)(content[i] ^ mask) << ", ";
    content[i] = (char)(content[i] ^ mask);
  }
  std::cout << "[WS_PARSE] Content: " << content << std::endl;

  readed += contentLength;
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

std::string web::web_socket::parse_content(std::vector<char> toParse) {
  std::cout << "[WS_TESTE] TO PARSE LEN: " << toParse.size() << std::endl;
	return "";
}