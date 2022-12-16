#include <iostream>
#include <map>
#include <string>
#include <functional> // std::function
#include "header.hpp"
#include "request.hpp"
#include "response.hpp"

#ifndef WEB_HTTP_H
#define WEB_HTTP_H
namespace web{
namespace http{

std::pair<std::string, std::function<void()>> create_method(std::string path, std::function<void()> method);

class Page {

protected:
	std::map<std::string, std::function<void()>> methods;
  int codStatus;
  std::string folderPath;
  std::string folderLayoutPath;
  std::string layoutFile;
	std::string partialLayoutFile;
	std::string pageName;
	std::string responseContentType;
  bool useLayout;
	bool sendHTML;
	bool sendJSON;

public:
  bool partialLayout;
  Response *response;
  Request *request;
  Page(Request *req, Response *res);
  std::string ReadFromFile(std::string fileName);
	void SetPageName(std::string name);
  virtual void Run();
	virtual void PageLoad();
  void Send();
};


class SendFile : public web::http::Page {
std::string filePath;
public:
  explicit SendFile(http::Request *req, http::Response *res, std::string filePath)
      : Page(req, res) {
				this->filePath = filePath;
				this->sendHTML = false;
				this->useLayout = false;
			}
  void PageLoad();
};

}
}
#endif