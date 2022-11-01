#include "header.hpp"

http::Header::Header() {}
std::string http::Header::GetPath() const { return path; }
http::Method http::Header::GetMethod() const { return httpMethod; }
std::string http::Header::GetHTTPRepresentation(std::string name) {
  for (int i = 0; i < keys.size(); i++) {
    if (keys[i] == name)
      return keys[i] + ": " + values[i] + "\n";
  }
  return ": 0";
}
std::string http::Header::GetHeadersRepresentation() {
  std::string content = "";
  for (int i = 0; i < keys.size(); i++) {
    content += keys[i] + ": " + values[i] + "\r\n";
  }

  content += "\r\n";
  return content;
}
std::string http::Header::GetValueRepresentation(std::string name) {
	std::transform(name.begin(), name.end(), name.begin(), ::toupper);
	std::string value = "";
  for (int i = 0; i < keys.size(); i++) {
		std::string key = keys[i];
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    if (key.compare(name) == 0){
			value = values[i];
      break;
		}
  }
  return value;
}
void http::Header::AddHeader(std::string key, std::string value) {
  keys.push_back(key);
  values.push_back(value);
}
void http::Header::SetPathMethod(std::string path, Method method) {
  this->path = path;
  this->httpMethod = method;
}