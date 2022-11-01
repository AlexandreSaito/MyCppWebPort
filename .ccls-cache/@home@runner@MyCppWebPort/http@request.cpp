#include "request.hpp"

std::map<std::string, std::string> http::parse_form_urlenconded(std::string content) {
  std::map<std::string, std::string> form;
  std::vector<std::string> lines = cstf::splitString(content, "&");
  std::cout << "count: " << lines.size() << std::endl;
  for (std::string line : lines) {
    std::cout << "parsing line: " << line << std::endl;
    int indexEqual = line.find("=");
    if (indexEqual < 1)
      continue;

    form.insert(std::pair<std::string, std::string>(
        line.substr(0, indexEqual),
        line.substr((indexEqual + 1), line.size() - (indexEqual + 1))));
  }

  return form;
}

http::Request::Request() {}
http::Header http::Request::GetHeader() const { return header; }
void http::Request::ParseRequest(std::string data) {
  std::vector<std::string> lines = cstf::splitString(data, "\n");
  std::cout << "\n\nParser: " << std::endl;
  int bodyRow = 0;

  for (int i = 0; i < lines.size(); i++) {
    std::string line = lines[i];

    if (bodyRow > 0) {
      if (header.GetMethod() == Method::POST) {
        this->formString.append(line);
        continue;
      }
    }

    int indexDoubleDot = line.find(":");
    if (indexDoubleDot > 0) {
			std::string key = line.substr(0, indexDoubleDot).c_str();
			std::string std_method = line.substr(indexDoubleDot + 2, (indexDoubleDot + 2) - line.length()).c_str();
      this->header.AddHeader(key, std_method);
      continue;
    }

    // Validate method and url string
    if (i == 0) {
      Method method;
      std::string path;
      if (line.find("GET") != -1) {
        method = Method::GET;
      } else if (line.find("POST") != -1) {
        method = Method::POST;
      }

      int indexUrl = line.find("/");
      int indexSpace = line.find(" ", indexUrl);
      path = line.substr(indexUrl, indexSpace - indexUrl);

      if (path == "/") {
        path = "/index";
      }

      int indexQueryString = path.find("?");
      if (indexQueryString != -1) {
        // skip '?' char
        ++indexQueryString;
        this->queryString =
            path.substr(indexQueryString, indexQueryString - path.length());

        path = path.substr(0, indexQueryString - 1);

        this->ParseQueryString();
      }

      header.SetPathMethod(path, method);
      continue;
    }

    if (line.size() == 1) {
      bodyRow = i;
    }
  }

  this->ParseFormString();

  std::cout << "\nHTTP Method" << header.GetMethod() << std::endl;
  std::cout << "URL Path: " << header.GetPath() << std::endl;
  std::cout << header.GetHeadersRepresentation() << std::endl;
}
void http::Request::ParseQueryString() {
  queryData = parse_form_urlenconded(this->queryString);
}
void http::Request::ParseFormString() {

  std::string contentType = this->header.GetValueRepresentation("Content-Type");
  std::cout << this->formString << std::endl;
  std::cout << contentType << std::endl;

  if (cstf::string_contains(contentType, "application/json")) {
    return;
  }

  if (cstf::string_contains(contentType, "multipart/form-data")) {
    int bondaryIndex = this->formString.find("boundary=");
    if (bondaryIndex == -1) {
      std::cout << "Fail to find bondary to parte form-data";
      return;
    }

    // Adding number of characters on 'boundary='
    bondaryIndex += 9;
    // Get Bondary
    std::string bondary = this->formString.substr(
        bondaryIndex, this->formString.size() - bondaryIndex);

    std::vector<std::string> contents =
        cstf::splitString(this->formString, bondary);

    for (std::string content : contents) {
      if (content.find("--") == 0)
        break;

      int nameIndex = content.find("name=\"");
      int valueIndex = content.find_last_of("\n");
      if (nameIndex == -1 || valueIndex == -1)
        continue;
      // Adding number of characters on 'name="'
      nameIndex += 6;
      // Adding number of characters on '\n'
      valueIndex += 1;

      int contentLength = content.size();
      std::string name = content.substr(nameIndex, contentLength - nameIndex);
      std::string value =
          content.substr(valueIndex, contentLength - valueIndex);

      formData.insert(std::pair<std::string, std::string>(name, value));
    }
    return;
  }

  if (cstf::string_contains(contentType, "application/x-www-form-urlencoded")) {
    this->formData = parse_form_urlenconded(this->formString);
    return;
  }
}
