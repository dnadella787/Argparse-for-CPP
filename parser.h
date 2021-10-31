#ifndef PARSER_H
#define PARSER_H


#include "arg.h"

#define NO_DESCRIPTION "no_description"


class parser {
private:
    std::vector<argument> known_arguments;                      //holds all arguments
    std::string help_message;
    std::string prog_name = "PROG";
    std::string description = NO_DESCRIPTION;
    
    void print_help();

public:
    //constructors
    parser(){} 
    
    //public member functions
    template<typename ... T>
    void add_arguments(T... args);

    void set_prog_name(const std::string& PROG_NAME);
    void set_description(const std::string& DESCRIPTION);
    void parse_args(const int& argc, char** argv);         //parses args and assigns variables

    void print_arguments();
};


template<typename ... T>
void parser::add_arguments(T... args)
{
    (known_arguments.push_back(args), ...);
}



#endif 