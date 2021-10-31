#ifndef PARSER_H
#define PARSER_H


#include "arg.h"

#define NO_DESCRIPTION "no_description"


class parser {
private:

    //member variables
    std::vector<argument> known_arguments;                      //holds all arguments
    std::string prog_name = "PROG";                             //program name, for help message
    std::string description = NO_DESCRIPTION;                   //description, for help message
    
    //private helper functions used internally
    void print_help();                                          //prints help message for -h, --help
    int match(const std::string& flag);                         //internal matching function checks if flag is legal in parse_args()
    void parse_arg_helper();                                    //helps to store the actual variables


public:
    //constructors
    parser(){} 
    
    //public member functions 
    template<typename ... T>
    void add_arguments(T... args);                              //add all argument objects to parser object

    //help message related functions
    void set_prog_name(const std::string& PROG_NAME);           //set the program name
    void set_description(const std::string& DESCRIPTION);       //set the description name

    //output related function
    void parse_args(const int& argc, char** argv);              //parses args and assigns variables, whole point of this
};


template<typename ... T>
void parser::add_arguments(T... args)
{
    (known_arguments.push_back(args), ...);
}



#endif 