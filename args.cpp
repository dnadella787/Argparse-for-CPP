#include "args.h"

void argument::set_help_message(const std::string& HELP_MESSAGE){
	help_message = HELP_MESSAGE;
}

void argument::set_flag_helper() {return;}

void argument::set_action(const std::string& ACTION){
	action = ACTION;
}

void argument::set_nargs(const int& NARGS){
	nargs = NARGS;
}

void argument::set_default(const std::string& DEFAULT_VALUE){
	variable = DEFAULT_VALUE;
}

void argument::set_variable_type(const std::string& VAR_TYPE){
	type = VAR_TYPE;
}

void argument::print(){
	std::cout << "name: " << arg_name << std::endl;
	for(size_t i = 0; i < flags.size(); i++){
		std::cout << flags.at(i) << std::endl;
	}

	std::cout << "help message: " << help_message << std::endl;
	std::cout << "action: " << action << std::endl;
	std::cout << "nargs: " << nargs << std::endl;
	std::cout << "default: " << variable << std::endl;
	std::cout << "type: " << type << std::endl;
}



