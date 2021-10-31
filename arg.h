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
    std::string arg_name;
    std::vector<std::string> accepted_flags;   //vector of recognizable flags
    std::string variable;                      //value
    std::string help_message;                  //help_message

public:
    //constructors
    argument(){}                               
    argument(const std::string& ARG_NAME);     

    //public member functions to set up appropriate behavior
    template<typename ... T>
    void set_flags(T... names);                //all acceptable cmd line flags for this argument

    void set_help_message(const std::string& HELP_MESSAGE);
   
    friend class parser;                       //parser can access the acceptable flags to return variables
};



template<typename ... T>
void argument::set_flags(T... names)
{
    (accepted_flags.push_back(names), ...);
}



#endif 