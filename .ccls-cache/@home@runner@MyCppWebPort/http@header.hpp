#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "httpConst.hpp"
#include <algorithm> // string to uppercase

#ifndef HTTP_HEADER_H

#define HTTP_HEADER_H
namespace http {

class Header {
  std::vector<std::string> keys;
  std::vector<std::string> values;
  std::string path;
  Method httpMethod = GET;

public:
  Header();
  std::string GetPath() const;
  Method GetMethod() const;
  std::string GetHTTPRepresentation(std::string name);
  std::string GetHeadersRepresentation();
	std::string GetValueRepresentation(std::string name);
  void AddHeader(std::string key, std::string value);
  void SetPathMethod(std::string path, Method method);
};

}
#endif