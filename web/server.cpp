#include "server.hpp"

int web::server::fdServer = -1;
bool web::server::server_running = false;
std::unordered_map<int, std::string> web::server::fd_connections;
std::map<std::string, web::server::create_page_method> web::server::pages;
std::queue<web::http::Request> web::server::pages_to_handle;
std::queue<web::http::Request> web::server::new_web_socket_connections;

std::thread web::server::request_listener;
std::thread web::server::page_handler;
std::thread web::server::websocket_handler;

void web::server::start_server() {
  fdServer = socket(AF_INET, SOCK_STREAM, 0);

  if (fdServer < 0) {
    std::cout << "[SOCKET]Unable to open socket.\n";
    return;
  }
  std::cout << "[SOCKET]Socket opened.\n";

  in_addr inaddr;
  inaddr.s_addr = INADDR_ANY;

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr = inaddr;
  addr.sin_port = htons(PORT);

  int num = 1;
  // TCP sockets go into a TIME_WAIT state where they can't be used for about a
  // minute.
  setsockopt(fdServer, SOL_SOCKET, SO_REUSEADDR, (char *)&num, sizeof(int));

  // ioctl(fdServer, FIONBIO, (char *)&num);

  if (bind(fdServer, (const sockaddr *)&addr, sizeof(sockaddr_in)) < 0) {
    std::cerr << "[SOCKET]Socket failed to bind.\n";
    return;
  };
  std::cerr << "[SOCKET]Socket binded.\n";

  // Starts to listening requests
  listen(fdServer, 40);

  server_running = true;
  std::cout << "[SOCKET]Server fd: " << fdServer << std::endl;

  std::cout << "[SYS]Starting thread listen request..." << std::endl;
  request_listener = std::thread(listen_request);
  std::cout << "[SYS]Starting thread page..." << std::endl;
  page_handler = std::thread(handle_page);
  std::cout << "[SYS]Starting thread websocket..." << std::endl;
  websocket_handler = std::thread(handle_websocket);

  std::cout << "[SYS]Starting thread websocket chat/reflect..." << std::endl;
  web::web_socket::chat_reflect_handler = std::thread([]() {
    while (server_running) {
      web::web_socket::handle_chat_reflect();
    }
  });
}

void web::server::close_server() { server_running = false; }

void web::server::register_page(std::string pageName, create_page_method page) {
  web::server::pages.insert(
      std::pair<std::string, create_page_method>(pageName, page));
}

bool web::server::is_running() { return web::server::server_running; }

void web::server::listen_request() {
  std::cout << "[SYS]Starting to listen..." << std::endl;
  int bufferLength = 1024;

  while (server_running) {
    in_addr inaddr;
    inaddr.s_addr = INADDR_ANY;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr = inaddr;
    addr.sin_port = htons(PORT);
    int addrlen = sizeof(addr);

    int fdRequest =
        accept(fdServer, (struct sockaddr *)&addr, (socklen_t *)&addrlen);

    std::cout << "[LISTEN]Request fd: " << fdRequest << std::endl;
    if (fdRequest == -1) {
      return;
    }

    char buffer[bufferLength];
    std::stringstream body;
    int count = 0;
    // Try get more content when buffer is full
    do {
      count = read(fdRequest, buffer, bufferLength);
      body << buffer;
      // Cleaning the array
      std::fill_n(buffer, bufferLength, '\0');
    } while (count == bufferLength);

    // std::cout << "\r\nCONTENT: " << std::endl;
    // std::cout << body.str() << std::endl << std::endl;

    web::http::Request *request = new web::http::Request(fdRequest);

    request->ParseRequest(body.str());
    // clear stringstream, should't be used anymore
    body.str("");

    // Check if is a file request, socket request or PAGE request
    std::string path = request->GetHeader().GetPath();
    if (path.find("/ws-connection") != -1) {
      std::cout << "[LISTEN]Adding new webSocket to pull" << std::endl;
      // new_web_socket_connections.push(*request);
      web::web_socket::send_handshake(request);
      register_fd_connection(
          request->fdRequest,
          cstf::replace(request->GetHeader().GetPath(), "/ws-connection", ""));
      continue;
    }

    std::cout << "[LISTEN]Adding request to queue" << std::endl;
    pages_to_handle.push(*request);
  }

  std::cout << "[SYS]Stopped to listen..." << std::endl;
}

void web::server::handle_page() {
  std::cout << "[SYS]Starting to handle any request on queue..." << std::endl;

  while (server_running) {
    if (pages_to_handle.empty())
      continue;

    std::cout << "[HANDLE_PAGE]Request finded, starting to handle it!"
              << std::endl;
    web::http::Request req = pages_to_handle.front();
    pages_to_handle.pop();
    web::http::Request *request = &req;

    std::string path = request->GetHeader().GetPath();

    std::cout << "[HANDLE_PAGE]Requested path: " << path << std::endl;

    if (request->GetHeader().GetMethod() == web::http::Method::OPTIONS) {
      std::cout << "[HANDLE_PAGE]Handling ALLOW" << std::endl;
      std::string allowHeader =
          "HTTP/1.1 200 OK\r\nAllow: OPTIONS, GET, HEAD, POST";

      write(request->fdRequest, allowHeader.c_str(), allowHeader.length());
      close(request->fdRequest);
      continue;
    }

    int dotFileIndex = path.find_last_of(".");

    web::http::Page *page;
    web::http::Response *response = new web::http::Response(request->fdRequest);

    if (dotFileIndex > -1 && dotFileIndex > (path.size() - 5)) {
      std::cout << "[HANDLE_PAGE]sending file " << path << "..." << std::endl;
      page = new web::http::SendFile(request, response, path);
    } else {
      std::cout << "[HANDLE_PAGE]searching for page " << path << "..."
                << std::endl;
      // find page by path
      int lastIndexSlash = path.find_last_of("/");
      auto pageInstancer = pages.find(path);

      while (pageInstancer == pages.end()) {
        std::string newPath = path.substr(0, lastIndexSlash);
        std::cout << "[HANDLE_PAGE]searching for page " << newPath << "..."
                  << std::endl;
        if (lastIndexSlash < 1)
          break;
        pageInstancer = pages.find(newPath);
        lastIndexSlash = path.find_last_of("/", lastIndexSlash - 1);
      }

      if (pageInstancer != pages.end()) {
        std::cout << "[HANDLE_PAGE]Instancieting: " + pageInstancer->first
                  << std::endl;
        page = pageInstancer->second(request, response);
        page->SetPageName(pageInstancer->first);
      } else {
        std::cout << "[HANDLE_PAGE] [Page not found]" << std::endl;
        ;
        page = new web::http::Page(request, response);
      }
    }

    page->Run();
    page->Send();

    if (page != nullptr)
      delete (page);
    // if ( request != nullptr ) delete (request);
    if (response != nullptr)
      delete (response);
  }

  std::cout << "[SYS]Stopped to handle request..." << std::endl;
}

void web::server::handle_websocket() {
  std::cout << "[SYS]Starting to handle webSockets..." << std::endl;
  int bufferLength = 1024;
  char buffer[bufferLength];
  std::stringstream body;

  int nfds;
  while (server_running) {
    // std::cout << "+Checking new socket " <<
    // new_web_socket_connections.empty() << std::endl;

    /*while (!new_web_socket_connections.empty()) {
      web::http::Request req = new_web_socket_connections.front();
      new_web_socket_connections.pop();
      web::web_socket::send_handshake(&req);
      std::cout << "Socket URL: " << req.GetHeader().GetPath() << std::endl;
      register_fd_connection(
          req.fdRequest,
          cstf::replace(req.GetHeader().GetPath(), "/ws-connection", ""));
      //std::string helloMessage = "Hellow!!!";
      //web::web_socket::send_message(req.fdRequest, helloMessage);
    }*/

    if (fd_connections.size() == 0)
      continue;

    struct pollfd pollFds[fd_connections.size()];
    int pollFdCurrentSize = 0;

    for (auto it : fd_connections) {
      pollFds[pollFdCurrentSize].fd = it.first;
      pollFds[pollFdCurrentSize].events = POLLIN;
      pollFdCurrentSize++;
    }

    nfds = pollFdCurrentSize;
    int ready = poll(pollFds, nfds, 100);
    // std::cout << "+++++Poll ready: " << ready << std::endl;

    for (int j = 0; j < nfds; j++) {

      if (pollFds[j].revents != 0) {
        printf("+++++ fd=%d; events: %s%s%s\n", pollFds[j].fd,
               (pollFds[j].revents & POLLIN) ? "POLLIN " : "",
               (pollFds[j].revents & POLLHUP) ? "POLLHUP " : "",
               (pollFds[j].revents & POLLERR) ? "POLLERR " : "");

        if (pollFds[j].revents & POLLIN) {
          int count = 0;
          // Try get more content when buffer is full
          do {
            // Cleaning buffer array
            std::fill_n(buffer, bufferLength, '\0');
            count = read(pollFds[j].fd, buffer, bufferLength);
            body << buffer;
          } while (count == bufferLength);

          std::string content = body.str();
          body.str("");
          std::cout << "+++++ content body: " << content << std::endl;
          if (content == "") {
            std::cout << "+++++ Should close " << pollFds[j].fd << std::endl;
            // handle close fd
            remove_fd_connection(pollFds[j].fd);
            close(pollFds[j].fd);
            continue;
          } else {
            content = web::web_socket::parse_readed_content(content);
            std::cout << "+++++ parsed content: " << content << std::endl;

            // handle message
            auto user_pair = fd_connections.find(pollFds[j].fd);
            if (user_pair == fd_connections.end()) {
              std::cout << "{FAILED} Faild to find fd_connections pair"
                        << std::endl;
              continue;
            }
            web::web_socket::handle(user_pair->first, user_pair->second,
                                    content);
          }
        } else { /* POLLERR | POLLHUP */
          std::cout << "+++++ Should close " << pollFds[j].fd << std::endl;
          // handle close fd
          remove_fd_connection(pollFds[j].fd);
          close(pollFds[j].fd);
          continue;
        }
      }
    }

    // std::cout << "End of loop webSocket..." << std::endl;
  }

  std::cout << "[SYS]Out of loop WebSocket..." << std::endl;
}

void web::server::register_fd_connection(int fdClient, std::string alias) {
  fd_connections.insert(
      std::make_pair(fdClient, alias)); //.insert(std::pair(fdClient, alias));
}

void web::server::remove_fd_connection(int fdClient) {
  for (auto itr = fd_connections.begin(); itr != fd_connections.end(); ++itr) {
    if (itr->first == fdClient) {
      fd_connections.erase(itr);
      break;
    }
  }
}