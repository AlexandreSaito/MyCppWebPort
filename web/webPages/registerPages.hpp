#include "./../server.hpp"
#include "rootPages.cpp"

#ifndef REGISTER_PAGES_H
#define REGISTER_PAGES_H
namespace web{

void register_pages(){
	web::server::register_page("/index", &web::server::create_page<web::root::IndexPage>);
	web::server::register_page("/teste", &web::server::create_page<web::root::TestePage>);
	web::server::register_page("/todo", &web::server::create_page<web::root::TodoPage>);
	web::server::register_page("/game", &web::server::create_page<web::root::GamePage>);
	web::server::register_page("/game2", &web::server::create_page<web::root::Game2Page>);
	web::server::register_page("/indexTeste", &web::server::create_page<web::root::IndexTestePage>);
}

}
#endif