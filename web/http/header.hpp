#include <iostream>
#include <string>
#include <map>
#include <algorithm> // string to uppercase
#include "content.hpp"

#ifndef WEB_HTTP_HEADER_H
#define WEB_HTTP_HEADER_H
namespace web{
namespace http {

using header_map = std::map<std::string, std::string>;

class Header {
  header_map headers;
  std::string path;
  web::http::Method httpMethod = GET;

public:
  Header();
  std::string GetPath() const;
  web::http::Method GetMethod() const;
  std::string GetHTTPRepresentation(std::string name) const;
  std::string GetHeadersRepresentation() const;
	std::string GetValueRepresentation(std::string name) const;
  void AddHeader(std::string key, std::string value);
  void SetPathMethod(std::string path, Method method);
};

}
}
#endif