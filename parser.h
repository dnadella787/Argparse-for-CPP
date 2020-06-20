#ifndef PARSER_H
#define PARSER_H

#include "args.h"

class parser {
private:
    std::vector<argument> known_arguments;                      //holds all arguments

    //private helper functions
    template<typename T, typename ... S>                        //recursive helper function
    void add_arguments_helper(T first_arg, S ... rest_args);  

    void add_arguments_helper();                                //base case
public:
    //constructors
    parser(){} 
    
    //public member functions
    template<typename ... T>                                    //adds all arguments to parser
    void add_arguments(T ... arguments);

    void parse_args(const int& argc, const char* argv);         //parses args and assigns variables

    void print_arguments();
};


template<typename T, typename ... S>
void parser::add_arguments_helper(T first_arg, S ... rest_args){
    known_arguments.push_back(first_arg);
    add_arguments_helper(rest_args...);
}

template<typename ... T>
void parser::add_arguments(T ... arguments){
    add_arguments_helper(arguments...);
}

#endif 