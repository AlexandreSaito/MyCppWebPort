#include "page.hpp"

std::pair<std::string, std::function<void()>>
web::http::create_method(std::string path, std::function<void()> method) {
  return std::pair<std::string, std::function<void()>>(path, method);
}

web::http::Page::Page(Request *req, Response *res) {
  request = req;
  response = res;
  this->methods.insert(create_method("", [this]() { this->PageLoad(); }));
	this->codStatus = 200;
  this->folderPath = "./htmlPages/";
  this->folderLayoutPath = "./htmlPages/";
  this->layoutFile = "_layout.html";
  this->partialLayoutFile = "_layout_partial.html";
	this->pageName = "";
	this->responseContentType = "";
  this->useLayout = true;
  this->partialLayout = false;
	this->sendHTML = true;
	this->sendJSON = false;
}
std::string web::http::Page::ReadFromFile(std::string fileName) {
  return cstf::readFile(this->folderPath + fileName);
}
void web::http::Page::Run() {
  if (request->GetHeader().GetMethod() == http::Method::GET) {
    std::cout << "[PAGE]Method GET Loading Page Load."
              << request->GetHeader().GetMethod() << std::endl;
    this->PageLoad();
    return;
  }

  std::string methodName =
      request->GetHeader().GetPath().replace(0, this->pageName.size(), "");
  std::cout << "[PAGE]Finding method: " << methodName << std::endl;
  auto method = this->methods.find(methodName);
  if (method == this->methods.end()) {
    std::cout << "[PAGE]Failed to find page method" << std::endl;
    PageLoad();
    return;
  }

  this->useLayout = false;
  this->sendHTML = false;
  std::cout << "Executing method: " << method->first << std::endl;
  method->second();
}
void web::http::Page::PageLoad() {
  std::cout << "PAGE LOAD WAS CALLED" << std::endl;
  this->codStatus = 404;
  std::string data =
      "<h1>Page not Found " + std::to_string(this->codStatus) + "</h1>";

  data += "<h3>Path: " + request->GetHeader().GetPath() + "</h3>";

  response->WriteToContent(data);
}
void web::http::Page::SetPageName(std::string name) { this->pageName = name; }
void web::http::Page::Send() {
  if (this->response->header.GetValueRepresentation("Content-Type") == "") {
    if (this->responseContentType == "") {
      if (this->sendHTML)
        this->responseContentType = "html";
      else if (!this->sendHTML && this->sendJSON)
        this->responseContentType = "json";
			else
				this->responseContentType = "plain";
    }
		std::cout << "[PAGE_SEND] Content type: " << this->responseContentType << "; Header Content-type: " << get_content_type(this->responseContentType) << std::endl;
    this->response->header.AddHeader("Content-Type", get_content_type(this->responseContentType));
  }

  if (this->sendHTML && this->useLayout && !this->partialLayout) {
    this->response->AddLayout(
        cstf::readFile(this->folderLayoutPath + this->layoutFile));
  }
	if(this->sendHTML && this->partialLayout){
    //this->response->AddLayout(cstf::readFile(this->folderLayoutPath + this->partialLayoutFile));
	}

  response->Respond(codStatus, response->header);
}

void web::http::SendFile::PageLoad() {
  this->useLayout = false;
  this->sendHTML = false;

  int dotIndex = this->filePath.find_last_of(".") + 1;
  std::string fileExt =
      this->filePath.substr(dotIndex, this->filePath.size() - dotIndex);

  this->responseContentType = fileExt;
	
  this->response->WriteToContent(cstf::readFile("." + this->filePath));
}
