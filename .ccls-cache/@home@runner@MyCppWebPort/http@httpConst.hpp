#include <map>
#include <string>

#ifndef HTTP_CONST_H

#define HTTP_CONST_H
namespace http {

enum Method { GET, POST };

// https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header
extern std::map<std::string, std::string> content_types;

void register_content_type();

std::string get_content_type(std::string content);

std::string get_name_of_content(std::string content_type);

}
#endif