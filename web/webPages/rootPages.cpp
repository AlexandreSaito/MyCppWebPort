#include "./../http/page.hpp"

namespace web{
namespace root{

class IndexPage : public web::http::Page {
public:
  explicit IndexPage(web::http::Request *req, web::http::Response *res)
      : Page(req, res) {}
  void PageLoad() {
    response->WriteToContent(this->ReadFromFile("index.html"));
    response->ReplaceContent("<[URLPAGE]>", request->GetHeader().GetPath());
  }
};

class TodoPage : public web::http::Page {
public:
  explicit TodoPage(web::http::Request *req, web::http::Response *res)
      : Page(req, res) {}
  void PageLoad() {
    response->WriteToContent(this->ReadFromFile("todo.html"));
  }
};

class GamePage : public web::http::Page {
public:
  explicit GamePage(web::http::Request *req, web::http::Response *res)
      : Page(req, res) {}
  void PageLoad() {
    response->WriteToContent(this->ReadFromFile("game.html"));
  }
};

class Game2Page : public web::http::Page {
public:
  explicit Game2Page(web::http::Request *req, web::http::Response *res)
      : Page(req, res) {}
  void PageLoad() {
    response->WriteToContent(this->ReadFromFile("game2.html"));
  }
};

class IndexTestePage : public web::http::Page {
public:
  explicit IndexTestePage(web::http::Request *req, web::http::Response *res)
      : Page(req, res) {
				this->layoutFile = "_layout2-0.html";
			}
  void PageLoad() {
    response->WriteToContent(this->ReadFromFile("index_teste.html"));
    response->ReplaceContent("<[URLPAGE]>", request->GetHeader().GetPath());
  }
};

class IndexSocketPage : public web::http::Page {
public:
  explicit IndexSocketPage(web::http::Request *req, web::http::Response *res)
      : Page(req, res) {}
  void PageLoad() {
		this->layoutFile = "_layout2-0.html";
    response->WriteToContent(this->ReadFromFile("indexsocket.html"));
    response->ReplaceContent("<[URLPAGE]>", request->GetHeader().GetPath());
  }
};

class TestePage : public web::http::Page {
public:
  explicit TestePage(web::http::Request *req, web::http::Response *res) : Page(req, res) {
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

}
}