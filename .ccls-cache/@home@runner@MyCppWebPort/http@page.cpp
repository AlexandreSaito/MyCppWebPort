#include "page.hpp"

std::pair<std::string, std::function<void()>>
http::create_method(std::string path, std::function<void()> method) {
  return std::pair<std::string, std::function<void()>>(path, method);
}

http::Page::Page(Request *req, Response *res) {
  request = req;
  response = res;
  this->methods.insert(create_method("", [this]() { this->PageLoad(); }));
}
std::string http::Page::ReadFromFile(std::string fileName) {
  return cstf::readFile(this->folderPath + fileName);
}
void http::Page::Run() {
  if (request->GetHeader().GetMethod() == http::Method::GET) {
    std::cout << "Method GET Loading Page Load."
              << request->GetHeader().GetMethod() << std::endl;
    this->PageLoad();
    return;
  }

  std::string methodName =
      request->GetHeader().GetPath().replace(0, this->pageName.size(), "");
  std::cout << "Finding method: " << methodName << std::endl;
  auto method = this->methods.find(methodName);
  if (method == this->methods.end()) {
    std::cout << "Failed to find page method" << std::endl;
    PageLoad();
    return;
  }

  this->useLayout = false;
  this->sendHTML = false;
  std::cout << "Executing method: " << method->first << std::endl;
  method->second();
}
void http::Page::PageLoad() {
  std::cout << "RUN WAS CALLED" << std::endl;
  this->codStatus = 404;
  std::string data =
      "<h1>Page not Found " + std::to_string(this->codStatus) + "</h1>";

  data += "<h3>Path: " + request->GetHeader().GetPath() + "</h3>";

  response->WriteToContent(data);
}
void http::Page::SetPageName(std::string name) { this->pageName = name; }
void http::Page::Send() {
  if (this->response->header.GetValueRepresentation("Content-Type") == "") {
    if (this->responseContentType == "") {
      if (this->sendHTML)
        this->responseContentType = "html";
      else if (!this->sendHTML && this->sendJSON)
        this->responseContentType = "json";
			else
				this->responseContentType = "plain";
    }
    this->response->header.AddHeader("Content-Type", get_content_type(this->responseContentType));
  }

  if (this->sendHTML && this->useLayout) {
    this->response->AddLayout(
        cstf::readFile(this->folderLayoutPath + this->layoutFile));
  }

  response->Respond(codStatus, response->header);
}

void http::SendFile::PageLoad() {
  this->useLayout = false;
  this->sendHTML = false;

  int dotIndex = this->filePath.find_last_of(".") + 1;
  std::string fileExt =
      this->filePath.substr(dotIndex, this->filePath.size() - dotIndex);

  this->responseContentType = fileExt;
	
  this->response->WriteToContent(cstf::readFile("." + this->filePath));
}
