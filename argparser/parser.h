#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include "arg.h"


/*
******************************
******* PARSER OBJECT ********
******************************
*/

class parser {
private:

    //member variables
    std::vector<argument*> known_arguments;                     //holds pointers to all arguments
    std::string prog_name = "PROG";                             //program name, for help message
    std::string description = NO_DESCRIPTION;                   //description, for help message
    
    //private helper functions used internally
    void print_help();                                          //prints help message for -h, --help

    //parsing helper functions based on the type of action to be done
    void setup_parsing();
    void do_action(const int& arg_num, const int& flag_num, const int& argc, char** argv, const int& ACTION);
    void action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void equal_action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv, const int& equal_iter);
    void action_store_true(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void action_store_false(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void action_append(const int& arg_num, const int& flag_num, const int& argc, char** argv);
    void action_count(const int& arg_num, const int& flag_num, const int& argc, char** argv);

    int match(const std::string& flag);                         //internal matching function checks if flag is legal in parse_args()
    int find_equal(const std::string& input);                   //checks if there is = present in the flag
    
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


/*
******* PARSER FUNCTION DEFINITONS *******
*/


template<typename ... T>
inline void parser::add_arguments(T... args)
{
    (known_arguments.push_back(args), ...);
}




#endif 
