#include "c_json.hpp"

int c_json::check_value(Document *d, std::string param){
	if(!d->HasMember(param.c_str()))
		return -1;
	return d->GetType();
}
