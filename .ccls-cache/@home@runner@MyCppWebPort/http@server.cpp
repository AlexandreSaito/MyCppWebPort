#include "server.hpp"

http::Server *http::server = new http::Server();

void http::Server::start_server() {
  this->socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (this->socketfd < 0) {
    std::cout << "Unable to open socket.\n";
    return;
  }
  std::cout << "Socket opened.\n";

  in_addr inaddr;
  inaddr.s_addr = INADDR_ANY;

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr = inaddr;
  addr.sin_port = htons(PORT);

  int num = 1;
  // TCP sockets go into a TIME_WAIT state where they can't be used for about a
  // minute.
  setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, (char *)&num,
             sizeof(int));

  if (bind(this->socketfd, (const sockaddr *)&addr, sizeof(sockaddr_in)) < 0) {
    std::cerr << "Socket failed to bind.\n";
    return;
  };
  std::cerr << "Socket binded.\n";

  // Starts to listening requests
  listen(this->socketfd, 40);

  this->running = true;

  // Method to listen request
  this->responseThread = std::thread([this, addr]() {
    int bufferLength = 1024;

    while (this->is_running()) {
      // sockaddr *clientaddr = (sockaddr *)malloc(sizeof(sockaddr));
      // socklen_t *len = (socklen_t *)malloc(sizeof(socklen_t));

      int addrlen = sizeof(addr);
      // int req = accept(this->socketfd, clientaddr, len);
      int req = accept(this->socketfd, (struct sockaddr *)&addr,
                       (socklen_t *)&addrlen);
      // Free unused objects
      // free(clientaddr);
      // free(len);

      std::cout << "Server fd: " << this->socketfd << std::endl;
      std::cout << "Request fd: " << req << std::endl;
      if (req == -1) {
        // std::cout << "Server fd: " << this->socketfd << "Request fd: " << req
        // << std::endl;
        return;
      }

      char buf[bufferLength];
      int count = read(req, buf, bufferLength);

      std::string body = "";
      // Try get more content when buffer is full
      while (count == bufferLength) {
        body += buf;
        // Cleaning the array
        std::fill_n(buf, bufferLength, '\0');
        count = read(req, buf, bufferLength);
      }
      // Append the rest of content
      body += buf;

      std::cout << "\r\nCONTENT: " << std::endl;
      std::cout << body << std::endl << std::endl;

      http::Request *request = new http::Request();
      http::Response *response = new http::Response(&req);

      request->ParseRequest(body);

      http::Page *page;
      std::string path = request->GetHeader().GetPath();
      if (path == "/ws-connection") {
        const std::string keyHash = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        const std::string socketKey = request->GetHeader().GetValueRepresentation("Sec-Websocket-Key");

				std::cout << "ascii" << std::endl;
				for(int i=0; i < socketKey.length(); i ++){
					std::cout << (int)socketKey[i] << " = " << socketKey[i] << ", ";
				}
				std::cout << std::endl;
				std::cout << socketKey.substr(0, socketKey.length() - 1) + keyHash << std::endl;
				std::string socketKeyConst = socketKey.substr(0, socketKey.length() - 1);
				std::cout << socketKeyConst << std::endl;
				socketKeyConst += keyHash;
        macaron::Base64 base64;
        std::string keys = socketKeyConst;//socketKey + keyHash;

        std::cout << keys << " : " << keys.length() << std::endl;
        unsigned char hash[SHA_DIGEST_LENGTH];
        unsigned char hash2[SHA_DIGEST_LENGTH];
        SHA_CTX sha1;
        SHA1_Init(&sha1);
        SHA1_Update(&sha1, keys.c_str(), keys.length());
        SHA1_Final(hash, &sha1);

				//std::string key = "dGhlIHNhbXBsZSBub25jZQ==";
				//key += keyHash;
        //std::stringstream ss2;
        //SHA1_Init(&sha1);
        //SHA1_Update(&sha1, key.c_str(), key.length());
        //SHA1_Final(hash2, &sha1);
				
        std::stringstream ss;
        std::stringstream content;

        for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
          ss << hash[i];
          //ss2 << hash2[i];
        }
        //std::cout << ss2.str() << std::endl;
				//std::cout << key << std::endl;
        //std::cout << "Base64: " << base64.Encode(ss2.str()) << std::endl;

        std::cout << ss.str() << std::endl;
        content << "HTTP/1.1 101 Switching Protocols\r\n";
        content << "Connection: Upgrade\r\n";
        content << "Upgrade: websocket\r\n";
        //content << "Sec-WebSocket-Protocol: ws-connection\r\n";
        //content << "Sec-WebSocket-Version: 13\r\n";
        content << "Sec-WebSocket-Accept: ";
        content << base64.Encode(ss.str());
       	content << "\r\n";
				//content << "Strict-Transport-Security: max-age=7194270; includeSubDomains\r\n";
				//content << "Replit-Cluster: global\r\n";
        content << "\r\n";
        std::cout << content.str() << std::endl;
        //write(req, content.str().c_str(), content.str().length());
				//sendto(req, content.str().c_str(), content.str().length(), 0, NULL, NULL);
        send(req, content.str().c_str(), content.str().length(), 0);
        // std::string hello = "Hellow Mundo";
        // send(req, hello.c_str(), hello.length(), 0);
        // close(req);

      	//char bufRead[bufferLength];
				//int c = read(req, bufRead, bufferLength);
      	//std::string b = "";
				//b += bufRead;
				//std::cout << "Returned: " << std::endl;
				//std::cout << b << std::endl;
        continue;
      }
      int dotFileIndex = path.find_last_of(".");
      // send file content instead of search for a page
      if (dotFileIndex > -1 && dotFileIndex > (path.size() - 5)) {
        std::cout << "sending file " << path << "..." << std::endl;
        page = new http::SendFile(request, response, path);
      } else {
        std::cout << "searching for page " << path << "..." << std::endl;
        // find page by path
        int lastIndexSlash = path.find_last_of("/");
        auto pageInstancer = this->pages.find(path);

        while (pageInstancer == this->pages.end()) {
          std::string newPath = path.substr(0, lastIndexSlash);
          std::cout << "searching for page " << newPath << "..." << std::endl;
          if (lastIndexSlash < 1)
            break;
          pageInstancer = this->pages.find(newPath);
          lastIndexSlash = path.find_last_of("/", lastIndexSlash - 1);
        }

        if (pageInstancer != this->pages.end()) {
          std::cout << "Instancieting: " + pageInstancer->first << std::endl;
          page = pageInstancer->second(request, response);
          page->SetPageName(pageInstancer->first);
        } else {
          std::cout << "[Page not found]";
          page = new http::Page(request, response);
        }
      }

      page->Run();
      page->Send();

      delete (page);
      delete (request);
      delete (response);
    }
  });
}

void http::Server::close_server() {
  running = false;
  responseThread.detach();
  close(this->socketfd);
}

bool http::Server::is_running() { return running; }

void http::register_page(std::string pageName, http::create_page_method page) {
  http::server->pages.insert(
      std::pair<std::string, create_page_method>(pageName, page));
}
