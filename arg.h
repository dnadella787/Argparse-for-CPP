#ifndef ARGS_H
#define ARGS_H

#include <string>
#include <vector>
#include <iostream>

//action macros
#define STORE 1
#define STORE_TRUE 2
#define STORE_FALSE 3 
#define APPEND 4
#define COUNT 5

//return_value() macros
#define SINGLE 1
#define VECTOR 'v'
#define NO_INPUT "NO_INPUT"

//STORE_TRUE/STORE_FALSE macros
#define STORE_T_DEFAULT "STORE_T_DEFAULT" 
#define STORE_F_DEFAULT "STORE_F_DEFAULT"


class argument{
    private:
        //variables
        std::string arg_name;                      //name of argument
        std::vector<std::string> accepted_flags;   //vector of recognizable flags
        std::vector<std::string> data;             //value to be stored
        std::string help_message;                  //help message for specific argument, used by parser to generate usage 
        int action = STORE;                        //action, check spec for specific information
        int n_args = 1;                            //number of arguments to be processed
        int count = 0;                             //number of times the flag has been specified 
        bool is_required = false;                  //if the flag is required or not, false by default

    public:
        //constructors
        argument(){}                               //default constructor
        argument(const std::string& ARG_NAME);     //constructor that also takes in the argument, use this one

        //public member functions to set up appropriate behavior
        template<typename ... T>
        void set_flags(const T&... NAMES);                      //all acceptable cmd line flags for this argument
        void set_nargs(const int& N_ARGS);                      //set the number of arguments to be processed
        void set_action(const int& ACTION);                     //set the type of action, README has the allowable macros
        void set_help_message(const std::string& HELP_MESSAGE); //set help message for the parser to use
        void set_requirement(const bool& REQUIREMENT);          //set the requirement, causes error if a required flag is not passed in
    
        //public member functions to get the desired value based on type
        std::string get_val();                                  //for use on action STORE
        bool is_used();                                         //for use on action STORE_FALSE or STORE_TRUE
        std::vector<std::string> get_vals();                    //for use on action APPEND
        int get_count();                                        //for use on action COUNT
        bool is_empty();                                        //use to check if an argument of action STORE_APPEND has data inputted in or not

        friend class parser;                                    //parser can access accepted flags and store protocol to define appropriate behavior
};




template<typename ... T>
void argument::set_flags(const T&... NAMES)
{
    (accepted_flags.push_back(NAMES), ...);
}



#endif 