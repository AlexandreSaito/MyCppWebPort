#include "./../http/page.hpp"

class IndexPage : public http::Page {
public:
  explicit IndexPage(http::Request *req, http::Response *res)
      : Page(req, res) {}
  void PageLoad() {
    response->WriteToContent(this->ReadFromFile("index.html"));
    response->ReplaceContent("<[URLPAGE]>", request->GetHeader().GetPath());
  }
};

class IndexSocketPage : public http::Page {
public:
  explicit IndexSocketPage(http::Request *req, http::Response *res)
      : Page(req, res) {}
  void PageLoad() {
		this->layoutFile = "_layout2-0.html";
    response->WriteToContent(this->ReadFromFile("indexsocket.html"));
    response->ReplaceContent("<[URLPAGE]>", request->GetHeader().GetPath());
  }
};

class TestePage : public http::Page {
public:
  explicit TestePage(http::Request *req, http::Response *res) : Page(req, res) {
    this->methods.insert(
        http::create_method("/testando", [this]() { this->PostMethod1(); }));
    this->methods.insert(
        http::create_method("/testaFetch", [this]() { this->Fetch(); }));
  }
  void PageLoad() {
    response->WriteToContent(this->ReadFromFile("teste.html"));
    std::string methodType = "";
    if (request->GetHeader().GetMethod() == http::Method::GET) {
      methodType = "Acessando por GET";
    } else {
      methodType = "Acessando por POST";
    }
    response->ReplaceContent("<[METHODTYPE]>", methodType);

    std::stringstream queryValues;
    queryValues << "<ul>";
    for (auto const &imap : this->request->queryData) {
      queryValues << "<li>" << imap.first << ": " << imap.second << "</li>";
    }
    queryValues << "</ul>";

    std::stringstream formValues;
    formValues << "<ul>";
    for (auto const &imap : this->request->formData) {
      formValues << "<li>" << imap.first << ": " << imap.second << "</li>";
    }
    formValues << "</ul>";
    response->ReplaceContent("<[QUERY]>", queryValues.str());
    response->ReplaceContent("<[FORM]>", formValues.str());
  }
  void PostMethod1() {
    this->sendHTML = true;
    this->useLayout = true;
    response->WriteToContent("<h1>Testando</h1>");
  }
  void Fetch() {this->sendHTML = false; response->WriteToContent("Fetched!"); }
};
