#include "./../server.hpp"
#include "rootPages.cpp"

#ifndef REGISTER_PAGES_H
#define REGISTER_PAGES_H
namespace web{

void register_pages(){
	web::server::register_page("/index", &web::server::create_page<web::root::IndexPage>);
	web::server::register_page("/teste", &web::server::create_page<web::root::TestePage>);
	web::server::register_page("/indexTeste", &web::server::create_page<web::root::IndexTestePage>);
}

}
#endif