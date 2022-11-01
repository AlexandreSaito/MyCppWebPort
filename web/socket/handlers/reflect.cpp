#include "reflect.hpp"

web::web_socket::ReflectSocket web::web_socket::reflect_socket;

void web::web_socket::ReflectSocket::handle(SocketMessage socketMessage) {
	bool isNewFD = true;
  for (auto &fd : fds) {
    if (fd == socketMessage.clientFd) {
			isNewFD = false;
      continue;
    }
    web::web_socket::send_message(fd, socketMessage.message);
  }

	if(isNewFD)
		fds.push_back(socketMessage.clientFd);
	
  std::cout << "Handle reflect..." << std::endl;
}