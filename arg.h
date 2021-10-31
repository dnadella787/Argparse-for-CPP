#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <vector>
#include <iostream>

//action macros
#define STORE "store"
#define STORE_TRUE "true" 
#define STORE_FALSE "false" 
#define APPEND "append"
#define COUNT "count"

//variable type macros
#define STRING "string"
#define INT "int"
#define CHAR "char"
#define BOOL "bool"
#define NONE "none"


class argument{
private:
    //variables
    std::string arg_name;                      //name of argument
    std::vector<std::string> accepted_flags;   //vector of recognizable flags
    std::vector<std::string> variable;         //value to be stored
    std::string help_message;                  //help message for specific argument, used by parser to generate usage 
    std::string action = STORE;                //action, check spec for specific information
    int n_args = 1;                            //number of arguments to be processed


public:
    //constructors
    argument(){}                               
    argument(const std::string& ARG_NAME);     


    //public member functions to set up appropriate behavior
    template<typename ... T>
    void set_flags(T... names);                             //all acceptable cmd line flags for this argument
    void set_nargs(const int& N_ARGS);                      //set the number of arguments to be processed
    void set_action(const std::string& ACTION);             //
    void set_help_message(const std::string& HELP_MESSAGE); //set help message for the parser to use
   

    friend class parser;                                    //parser can access accepted flags and store protocol to define appropriate behavior
};



template<typename ... T>
void argument::set_flags(T... names)
{
    (accepted_flags.push_back(names), ...);
}



#endif 