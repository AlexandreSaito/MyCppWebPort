#include <map>
#include <string>

#ifndef WEB_HTTP_CONST_H
#define WEB_HTTP_CONST_H

namespace web {
namespace http {

// OPTUONS METHOD
// https://developer.mozilla.org/pt-BR/docs/Web/HTTP/Methods/OPTIONS
enum Method { GET, POST, OPTIONS };

// https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header
extern std::map<std::string, std::string> content_types;

void register_content_type();

const std::string get_content_type(std::string content);

const std::string get_name_of_content(std::string content_type);

}
}
#endif