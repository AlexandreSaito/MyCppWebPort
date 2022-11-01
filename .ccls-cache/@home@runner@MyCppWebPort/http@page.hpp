#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>
#include "httpConst.hpp"
#include "header.hpp"
#include "request.hpp"
#include "response.hpp"
#include "./../customFunctions.hpp"

#ifndef HTTP_H

#define HTTP_H

namespace http {

std::pair<std::string, std::function<void()>> create_method(std::string path, std::function<void()> method);

class Page {

protected:
	std::map<std::string, std::function<void()>> methods;
  int codStatus = 200;
  std::string folderPath = "./htmlPages/";
  std::string folderLayoutPath = "./htmlPages/";
  std::string layoutFile = "_layout2-0.html";
	std::string pageName = "";
	std::string responseContentType = "";
  bool useLayout = true;
	bool sendHTML = true;
	bool sendJSON = false;

public:
  Response *response;
  Request *request;
  Page(Request *req, Response *res);
  std::string ReadFromFile(std::string fileName);
	void SetPageName(std::string name);
  virtual void Run();
	virtual void PageLoad();
  void Send();
};


class SendFile : public http::Page {
std::string filePath;
public:
  explicit SendFile(http::Request *req, http::Response *res, std::string filePath)
      : Page(req, res) {
				this->filePath = filePath;
			}
  void PageLoad();
};


} // namespace http

#endif