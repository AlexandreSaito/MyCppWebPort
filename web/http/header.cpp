#include "header.hpp"

web::http::Header::Header() {}
std::string web::http::Header::GetPath() const { return path; }
web::http::Method web::http::Header::GetMethod() const { return httpMethod; }
std::string web::http::Header::GetHTTPRepresentation(std::string name) const {
  std::transform(name.begin(), name.end(), name.begin(), ::toupper);
  std::string value = "";
  for (auto it = headers.begin(); it != headers.end(); ++it) {
    std::string key = it->first;
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    if (key.compare(name) == 0) {
      value = it->first + ": " + it->second;
      break;
    }
  }
  return value;
}

std::string web::http::Header::GetHeadersRepresentation() const {
  std::string content = "";
  for (auto it = headers.begin(); it != headers.end(); ++it) {
    content += it->first + ": " + it->second + "\r\n";
  }
  content += "\r\n";
  return content;
}

std::string web::http::Header::GetValueRepresentation(std::string name) const {
  std::transform(name.begin(), name.end(), name.begin(), ::toupper);
  std::string value = "";
  for (auto it = headers.begin(); it != headers.end(); ++it) {
    std::string key = it->first;
    std::transform(key.begin(), key.end(), key.begin(), ::toupper);
    if (key.compare(name) == 0) {
      value = it->second;
      break;
    }
  }
  return value;
}

void web::http::Header::AddHeader(std::string key, std::string value) {
  headers.insert(std::pair<std::string, std::string>(key, value));
}

void web::http::Header::SetPathMethod(std::string path, Method method) {
  this->path = path;
  this->httpMethod = method;
}
