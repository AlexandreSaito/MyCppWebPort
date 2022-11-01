#include <iostream>
#include <string>
#include <unistd.h>
#include "header.hpp"
#include "./../customFunctions.hpp"

#ifndef HTTP_RESPONSE_H

#define HTTP_RESPONSE_H
namespace http {

class Response {
  int *responseId;
  std::ostringstream content;

public:
  Header header;
  Response(int *id);
  void WriteToContent(std::string data);
  void ReplaceContent(std::string tag, const std::string data);
  void Respond(int status, Header headers);
  void AddLayout(std::string layout);
};

}
#endif