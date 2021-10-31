#ifndef ARGS_H
#define ARGS_H

//action macros
#define STORE "store"
#define STORE_TRUE "true" 
#define STORE_FALSE "false" 
#define APPEND "append"
#define COUNT "count"

//nargs macros
#define VARIABLE -1

//variable type macros
#define STRING "string"
#define INT "int"
#define CHAR "char"
#define BOOL "bool"
#define NONE "none"

#include <string>
#include <vector>
#include <iostream>


class argument {
protected:
    std::string arg_name;                      //name of argument for help message purposes
    std::string help_message;                  //help message used by parser
    std::vector<std::string> flags;            //vector of recognizable flags
    std::string action = NONE;                 //how to handle the argument
    int nargs = 0;                             //how many arguments to store
    std::string variable = "";                 //default value
    bool required = false;
    std::string type = NONE;                   //variable type


    //protected helper functions
    template<typename T, typename ... S>       //recursive helper function for set_flags()
    void set_flag_helper(T first_flag, S ... rest_flags);

    void set_flag_helper();                    //base case overloaded function
public:
    //constructors
    argument(){}
    argument(const std::string& ARG_NAME) {}

    //public member functions
    template<typename ... T>                   //adds all flags to flags vector
    void set_flags(T ... flag);
    void set_help_message(const std::string& HELP_MESSAGE);
    void set_action(const std::string& ACTION);
    void set_nargs(const int& NARGS);
    void set_default(const std::string& DEFAULT_VALUE);
    void set_required(const bool& REQUIRED);
    void set_variable_type(const std::string& VAR_TYPE);

    template<typename Q>
    T operator()();

    void print();

    friend class parser;
};

template<typename T, typename ... S>
void argument::set_flag_helper(T first_flag, S ... rest_flags) {
    flags.push_back(first_flag);
    set_flag_helper(rest_flags...);
}


template<typename ... T>
void argument::set_flags(T ... flags) {
	set_flag_helper(flags...);
}

#endif 