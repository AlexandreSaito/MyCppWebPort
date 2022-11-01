#include "formParser.hpp"

std::map<std::string, std::string>
web::http::parse_form_urlenconded(std::string content) {
  std::map<std::string, std::string> form;
  std::vector<std::string> lines = cstf::splitString(content, "&");
  //std::cout << "count: " << lines.size() << std::endl;
  for (std::string line : lines) {
    //std::cout << "parsing line: " << line << std::endl;
    int indexEqual = line.find("=");
    if (indexEqual < 1)
      continue;

    form.insert(std::pair<std::string, std::string>(
        line.substr(0, indexEqual),
        line.substr((indexEqual + 1), line.size() - (indexEqual + 1))));
  }

  return form;
}
