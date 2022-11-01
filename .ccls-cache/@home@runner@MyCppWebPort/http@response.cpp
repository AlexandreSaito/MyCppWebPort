#include "response.hpp"

http::Response::Response(int *id) { responseId = id; }
void http::Response::WriteToContent(std::string data) {
  content << (data + "\n");
}
void http::Response::Respond(int status, Header headers) {
  int contentLength = content.str().length();
  if (contentLength == 0) {
    std::string header = "HTTP/1.1 500 OK\r\n";
    write(*responseId, header.c_str(), header.length());
    close(*responseId);
    return;
  }

  std::string header = "HTTP/1.1 " + std::to_string(status) + " OK\r\n";
  this->header.AddHeader("Content-Length", std::to_string(contentLength));

  std::string headerRepresentation = this->header.GetHeadersRepresentation();
  write(*responseId, header.c_str(), header.length());
  write(*responseId, headerRepresentation.c_str(), headerRepresentation.length());
  write(*responseId, content.str().c_str(), contentLength);

  //std::cout << "Responding: \r\n\r\n" << header << headerRepresentation << content.str();

  close(*responseId);
}
void http::Response::ReplaceContent(std::string tag, const std::string data) {
  this->content.str(cstf::replace(this->content.str(), tag, data));
}
void http::Response::AddLayout(std::string layout) {
	std::string layoutScriptTag = "<[SCRIPT]>";
	std::string contentScriptTag = "<[<script";
	std::string content = this->content.str();
	
	int indexScriptTag = content.find(contentScriptTag);
	while(indexScriptTag > -1){
		int indexEndScriptTag = content.find("</script>]>", indexScriptTag) + 9;
		std::string script = content.substr((indexScriptTag+ 2), indexEndScriptTag - (indexScriptTag + 2));
		
		content.erase(indexScriptTag, (indexEndScriptTag+ 2) - indexScriptTag );
		layout.insert(layout.find(layoutScriptTag), script);
		
		indexScriptTag = content.find(contentScriptTag);
	}
	
	layout = cstf::replace(layout, layoutScriptTag, "");
  this->content.str(cstf::replace(layout, "<[CONTENT]>", content));
}
