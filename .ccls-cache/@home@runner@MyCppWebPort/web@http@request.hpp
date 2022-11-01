#include <iostream>
#include <string>
#include <map>
#include "header.hpp"
#include "formParser.hpp"

#ifndef WEB_HTTP_REQUEST_H
#define WEB_HTTP_REQUEST_H
namespace web{
namespace http{

class Request {
  Header header;

public:
	int fdRequest;
  std::string queryString;
  std::string formString;
  std::map<std::string, std::string> queryData;
  std::map<std::string, std::string> formData;
  Request(int fdRequest);
  Header GetHeader() const;
  void ParseRequest(std::string data);
  void ParseQueryString();
  void ParseFormString();
};

}
}
#endif