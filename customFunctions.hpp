#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
//#include "json.hpp"

//using json = nlohmann::json;

#ifndef CUSTOMFUNCTIONS_HPP
#define CUSTOMFUNCTIONS_HPP
#define SIZE 1000

namespace cstf {

std::vector<std::string> splitString(std::string content,
                                     std::string splitter);

bool string_contains(std::string content, std::string key);

std::string replace(std::string content, std::string tag, std::string data);

std::string readFile(std::string filePath);

char* base64Encoder(char input_str[], int len_str);

} 


/*template<typename T>
json to_json(const T);
template<typename T>
T from_json(const json);*/

#endif