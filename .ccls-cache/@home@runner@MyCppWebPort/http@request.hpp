#include <iostream>
#include <string>
#include <map>
#include "header.hpp"
#include "./../customFunctions.hpp"

#ifndef HTTP_REQUEST_H

#define HTTP_REQUEST_H
namespace http {

std::map<std::string, std::string> parse_form_urlenconded(std::string content);

class Request {
  Header header;

public:
  std::string queryString;
  std::string formString;
  std::map<std::string, std::string> queryData;
  std::map<std::string, std::string> formData;
  Request();
  Header GetHeader() const;
  void ParseRequest(std::string data);
  void ParseQueryString();
  void ParseFormString();
};

}
#endif