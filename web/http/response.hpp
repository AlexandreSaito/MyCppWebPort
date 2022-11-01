#include <iostream>
#include <string>
#include <unistd.h> // read/write/close from fd
#include "header.hpp"
#include "./../../customFunctions.hpp"

#ifndef WEB_HTTP_RESPONSE_H
#define WEB_HTTP_RESPONSE_H
namespace web{
namespace http{

class Response {
  int fdRequest;
  std::ostringstream content;

public:
  Header header;
  Response(int fdRequest);
  void WriteToContent(std::string data);
  void ReplaceContent(std::string tag, const std::string data);
  void Respond(int status, Header headers);
  void AddLayout(std::string layout);
};

}
}
#endif