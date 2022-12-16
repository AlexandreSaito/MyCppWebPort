#include "response.hpp"

web::http::Response::Response(int fdRequest) { this->fdRequest = fdRequest; }
void web::http::Response::WriteToContent(std::string data) {
  content << (data + "\n");
}
void web::http::Response::Respond(int status, Header headers) {
  int contentLength = content.str().length();
  if (contentLength == 0) {
    std::string header = "HTTP/1.1 500 OK\r\n";
    write(this->fdRequest, header.c_str(), header.length());
    close(this->fdRequest);
    return;
  }

  std::string header = "HTTP/1.1 " + std::to_string(status) + " OK\r\n";
  this->header.AddHeader("Content-Length", std::to_string(contentLength));

  std::string headerRepresentation = this->header.GetHeadersRepresentation();
  write(this->fdRequest, header.c_str(), header.length());
  write(this->fdRequest, headerRepresentation.c_str(),
        headerRepresentation.length());
  write(this->fdRequest, content.str().c_str(), contentLength);

  // std::cout << "Responding: \r\n\r\n" << header << headerRepresentation <<
  // content.str();

  std::cout << "Fd Request: " << this->fdRequest << std::endl;
  close(this->fdRequest);
}
void web::http::Response::ReplaceContent(std::string tag,
                                         const std::string data) {
  this->content.str(cstf::replace(this->content.str(), tag, data));
}
void web::http::Response::AddLayout(std::string layout) {
  std::string content = this->content.str();

  int indexScriptTag = content.find(this->startContentScriptTag);
  while (indexScriptTag > -1) {
    int indexEndScriptTag =
        content.find(this->endContentScriptTag, indexScriptTag) + 9;
    std::string script = content.substr(
        (indexScriptTag + 2), indexEndScriptTag - (indexScriptTag + 2));

    content.erase(indexScriptTag, (indexEndScriptTag + 2) - indexScriptTag);
    layout.insert(layout.find(this->layoutScriptTag), script);

    indexScriptTag = content.find(this->startContentScriptTag);
  }

  layout = cstf::replace(layout, layoutScriptTag, "");
  this->content.str(cstf::replace(layout, this->layoutContentTag, content));
}
