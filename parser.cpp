#include "parser.h"

void parser::add_arguments_helper(){return;}

void parser::print_arguments(){
    for (int i = 0; i < known_arguments.size(); i++){
        std::cout << known_arguments.at(i).variable << std::endl;
    }
    
}