#include "./../http/server.hpp"
#include "rootPages.cpp"

void registerPages(){
	//http::register_page("/", &http::create_page<IndexPage>);
	http::register_page("/index", &http::create_page<IndexPage>);
	http::register_page("/index-socket", &http::create_page<IndexSocketPage>);
	http::register_page("/teste", &http::create_page<TestePage>);
}