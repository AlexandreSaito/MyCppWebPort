#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "./../../customFunctions.hpp"

#ifndef WEB_HTTP_FORMPARSER_H
#define WEB_HTTP_FORMPARSER_H
namespace web{
namespace http{

	std::map<std::string, std::string> parse_form_urlenconded(std::string content);

}
}
#endif