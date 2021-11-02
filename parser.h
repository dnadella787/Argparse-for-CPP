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

    //parsing helper functions based on the type of action to be done
    void setup_parsing();
    void action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void action_store_true(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void action_store_false(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void action_append(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void action_count(const int& arg_num);

    int match(const std::string& flag);                         //internal matching function checks if flag is legal in parse_args()


public:
    //constructors
    parser(){} 
    
    //public member functions 
    template<typename ... T>
    void add_arguments(const T&... args);                       //add all argument objects to parser object

    //help message related functions
    void set_prog_name(const std::string& PROG_NAME);           //set the program name
    void set_description(const std::string& DESCRIPTION);       //set the description name

    //output related function
    void parse_args(const int& argc, char** argv);              //parses args and assigns variables, whole point of this
};


template<typename ... T>
void parser::add_arguments(const T&... args)
{
    (known_arguments.push_back(args), ...);
}



#endif 