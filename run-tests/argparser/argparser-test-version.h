#ifndef ARGPARSER_H
#define ARGPARSER_H 

#include <string>
#include <vector>
#include <iostream>
#include <string.h>

//action macros
#define STORE 1
#define STORE_TRUE 2
#define STORE_FALSE 3 
#define APPEND 4
#define COUNT 5
#define VAR_APPEND 6

//return_value() macros
#define NO_INPUT "NO_INPUT"

//STORE_TRUE/STORE_FALSE macros
#define STORE_T_DEFAULT "STORE_T_DEFAULT" 
#define STORE_F_DEFAULT "STORE_F_DEFAULT"

//macro for help message
#define NO_DESCRIPTION "no_description"


/*
THIS IS CODE FOR THE TEST SUITE EXCLUSIVELY, IT HAS SOME EXTRA HELPER FUNCTIONS
AND SCOPE CHANGES FOR TESTING PURPOSES, ALL OTHER CODE/FUNCTIONALITY IS EXACT 
SAME
*/


/*
*****************************
****** ARGUMENT OBJECT ******
*****************************
*/


class argument{
public:
    //variables
    std::string arg_name;                      //name of argument
    std::vector<std::string> accepted_flags;   //vector of recognizable flags
    std::vector<std::string> data;             //value to be stored
    std::string help_message;                  //help message for specific argument, used by parser to generate usage 
    int action = STORE;                        //action, check spec for specific information
    int n_args = 1;                            //number of arguments to be processed
    int count = 0;                             //number of times the flag has been specified 
    bool is_required = false;                  //if the flag is required or not, false by default

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
    std::string get_store();                                //for use on action STORE
    bool get_store_tf();                                    //for use on action STORE_FALSE or STORE_TRUE
    std::vector<std::string> get_append();                  //for use on action APPEND
    int get_count();                                        //for use on action COUNT
    bool is_empty();                                        //use to check if an argument of action STORE_APPEND has data inputted in or not

    friend class parser;                                    //parser can access accepted flags and store protocol to define appropriate behavior
};


/*
******* ARGUMENT FUNCTION DEFINITONS *******
*/

template<typename ... T>
inline void argument::set_flags(const T&... NAMES)
{
    (accepted_flags.push_back(NAMES), ...);
}


inline argument::argument(const std::string& ARG_NAME) : arg_name(ARG_NAME) {}


inline void argument::set_nargs(const int& N_ARGS)
{
    n_args = N_ARGS;
}


inline void argument::set_action(const int& ACTION)
{
    if (ACTION == STORE)
    {
        data.push_back(NO_INPUT);
        n_args = 1;
    }
    else if (ACTION == STORE_TRUE)
    {
        data.push_back(STORE_T_DEFAULT);
        n_args = 0;
    }
    else if (ACTION == STORE_FALSE)
    {
        data.push_back(STORE_F_DEFAULT);
        n_args = 0;
    }
    else if (ACTION == COUNT)
    {
        n_args = 0;
    }
    action = ACTION;
}


inline void argument::set_help_message(const std::string& HELP_MESSAGE)
{
    help_message = HELP_MESSAGE;
}


inline void argument::set_requirement(const bool& REQUIREMENT)
{
    is_required = REQUIREMENT;
}


inline std::string argument::get_store()
{
    if (action != STORE)
    {
        std::cerr << "ERROR: " << arg_name << " does not have action STORE, cannot use get_store()" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    return data[0];
}


inline std::vector<std::string> argument::get_append()
{
    if (action != APPEND)
    {
        std::cerr << "ERROR: " << arg_name << " does not have action APPEND, cannot use get_append()" << std::endl;
        exit(EXIT_FAILURE);
    }
    return data;
}


inline int argument::get_count()
{
    if (action != COUNT)
    {
        std::cerr << "ERROR: " << arg_name << " does not have COUNT action, cannot use get_count()" << std::endl;
        exit(EXIT_FAILURE);
    }
    return count;
}


inline bool argument::get_store_tf()
{
    if (action == STORE_FALSE)
        return (data[0] == STORE_F_DEFAULT);
    else if (action == STORE_TRUE)
        return (data[0] != STORE_T_DEFAULT);
    else
    {
        std::cerr << "ERROR: " << arg_name << " does not have action STORE_FALSE or STORE_TRUE, cannot use get_store_tf()" << std::endl;
        exit(EXIT_FAILURE);
    }
}


inline bool argument::is_empty()
{
    if (action == STORE)
    {
        if (data[0] == NO_INPUT)
            return true;
        return false;
    }
    else if (action == APPEND)
    {
        if (get_append().size() == 0)
            return true;
        return false;
    }
    else 
    {
        std::cerr << "ERROR: is_empty() can only be used with actions STORE and APPEND, cannot use with " << arg_name << std::endl;
        exit(EXIT_FAILURE);
    }
}


/*
******************************
******* PARSER OBJECT ********
******************************
*/

class parser {
public:

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


inline void parser::set_prog_name(const std::string& PROG_NAME)
{
    prog_name = PROG_NAME;
}

inline void parser::set_description(const std::string& DESCRIPTION)
{
    description = DESCRIPTION;
}


inline void parser::print_help()
{
    std::cout << "usage: " << prog_name << " ";
    for (argument *x : known_arguments)
    {
        std::cout << "[";
        for (int i = 0; i < x->accepted_flags.size() - 1; i++)
        {
            std::cout << " " << x->accepted_flags[i] << " |";
        }
        std::cout << " " << x->accepted_flags.back();
        std::cout << " ] ";
    }
    
    std::cout << std::endl << std::endl;
    if (description != NO_DESCRIPTION)
    {
        std::cout << "description:" << std::endl;
        std::cout << "  " << description << std::endl;
        std::cout << std::endl << std::endl;
    }


    std::cout << "options:" << std::endl;
    for (argument *x : known_arguments)
    {
        std::cout << "  ";
        for (int i = 0; i < x->accepted_flags.size() - 1; i++)
        {
            std::cout << x->accepted_flags[i] << ", ";
        }
        std::cout << x->accepted_flags.back() << "   ==>   "; 
        if (x->action == STORE)
        {
            std::cout << x->help_message << " (usage: ";
            std::cout << x->accepted_flags[0] << "=[input] or ";
            std::cout << x->accepted_flags[0] << " [input])" << std::endl << std::endl;
        }
        else if (x->action == APPEND)
        {
            std::cout << x->help_message << " (usage: ";
            std::cout << x->accepted_flags[0] << " [input1] ... [inputk])" << std::endl << std::endl;
        }
        else 
            std::cout << x->help_message << std::endl << std::endl;
    }

    std::cout << "  " << "-h, --help   ==>   generates this help/usage message" << std::endl;
}


inline int parser::match(const std::string& flag)
{
    for (int i = 0; i < known_arguments.size(); i++)
    {
        for (std::string arg : known_arguments[i]->accepted_flags)
        {
            if (arg == flag)
                return i;
        }
    }
    return -1;
}


inline int parser::find_equal(const std::string& input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == '=')
            return i;
    }
    return -1;
}


inline void parser::action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{
    if (flag_num + 1 == argc)
    {
        std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    else if (argv[flag_num + 1][0] == '-')
    {
        std::cerr << "ERROR: input expected for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    else
    {
        known_arguments[arg_num]->data[0] = argv[flag_num + 1];
    }
    if (flag_num + 2 == argc)
    {
        return;
    }
    else if (argv[flag_num + 2][0] != '-')
    {
        std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
}


inline void parser::equal_action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv, const int& equal_iter)
{
    if (known_arguments[arg_num]->action != STORE)
    {
        std::cerr << "ERROR: " << argv[flag_num] << " flag is not action STORE, can't use '='" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    if (argv[flag_num][equal_iter + 1] == '\0')
    {
        std::cerr << "ERROR: no input after = for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    if (flag_num + 1 < argc && argv[flag_num + 1][0] != '-')
    {
        std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    known_arguments[arg_num]->data[0] = &argv[flag_num][equal_iter + 1];
    
    if (flag_num + 1 == argc)
        return;
}


inline void parser::action_store_true(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{   
    if (flag_num + 1 == argc)
    {
        known_arguments[arg_num]->data[0] = STORE_F_DEFAULT;
        return;
    }
    if (argv[flag_num + 1][0] != '-')
    {
        std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag, action STORE_TRUE" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    else 
    {
        known_arguments[arg_num]->data[0] = STORE_F_DEFAULT;
    }
}


inline void parser::action_store_false(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{   
    if (flag_num + 1 == argc)
    {
        known_arguments[arg_num]->data[0] = STORE_T_DEFAULT;
        return;
    }
    if (argv[flag_num + 1][0] != '-')
    {
        std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag, action STORE_FALSE" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    else 
    {
        known_arguments[arg_num]->data[0] = STORE_T_DEFAULT;
    }
}


inline void parser::action_append(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{
    if (known_arguments[arg_num]->data.size() == known_arguments[arg_num]->n_args)
    {
        std::cerr << "ERROR: " << argv[flag_num] << " already has input, cannot input twice" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }

    if (known_arguments[arg_num]->n_args + flag_num >= argc)
    {
        std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < known_arguments[arg_num]->n_args + 1; i++)
    {
        if (argv[flag_num + i][0] == '-')
        {
            std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
            print_help();
            exit(EXIT_FAILURE);
        }
        else
        {
            known_arguments[arg_num]->data.push_back(argv[flag_num + i]);
        }
    }
    if (flag_num + known_arguments[arg_num]->n_args + 1 == argc)
    {
        return;
    }
    else if (argv[flag_num + known_arguments[arg_num]->n_args + 1][0] != '-')
    {
        std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
}


inline void parser::action_count(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{
    known_arguments[arg_num]->count++;
    if (flag_num + 1 == argc)
        return;
    else if (argv[flag_num + 1][0] != '-')
    {
        std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
    return;
}


inline void parser::do_action(const int& arg_num, const int& flag_num, const int& argc, char** argv, const int& ACTION)
{
    if (known_arguments[arg_num]->action == STORE)
        action_store(arg_num, flag_num, argc, argv);
    else if (known_arguments[arg_num]->action == STORE_TRUE)
    {
        action_store_true(arg_num, flag_num, argc, argv);
    }
    else if (known_arguments[arg_num]->action == STORE_FALSE)
    {
        action_store_false(arg_num, flag_num, argc, argv);
    }
    else if (known_arguments[arg_num]->action == APPEND)
    {
        action_append(arg_num, flag_num, argc, argv);
    }
    else if (known_arguments[arg_num]->action == COUNT)
    {
        action_count(arg_num, flag_num, argc, argv);
    }
    else
    {
        std::cerr << "ERROR: " << known_arguments[arg_num]->arg_name << " has an invalid action, use STORE, STORE_TRUE, STORE_FALSE, APPEND, COUNT" << std::endl;
        print_help();
        exit(EXIT_FAILURE);
    }
}


inline void parser::parse_args(const int& argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_help();
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            int equal_iter = find_equal(argv[i]);
            if (equal_iter > 0)
            {
                std::string actual_flag(argv[i]);
                int arg_num = match(actual_flag.substr(0, equal_iter));
                if (arg_num >= 0)
                {
                    equal_action_store(arg_num, i, argc, argv, equal_iter);
                }
                else
                {
                    std::cerr << "ERROR: " << actual_flag << " is not a recognized flag." << std::endl;
                    print_help();
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                int arg_num = match(argv[i]);
                if (arg_num >= 0)
                {
                    do_action(arg_num, i, argc, argv, known_arguments[arg_num]->action);
                }
                else
                {
                    std::cerr << "ERROR: " << argv[i] << " is not a recognized argument" << std::endl;
                    print_help();
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    for (argument *a : known_arguments)
    {
        if (a->is_required && a->action != STORE_FALSE && a->action != STORE_TRUE)
        {
            if (a->action == COUNT && a->count == 0)
            {
                std::cerr << "ERROR: " << a->accepted_flags[0] << " is a required COUNT action argument" << std::endl;
                print_help();
                exit(EXIT_FAILURE);
            }
            else if (a->action == STORE && a->data[0] == NO_INPUT)
            {
                std::cerr << "ERROR: " << a->accepted_flags[0] << " is a required STORE argument." << std::endl;
                print_help();
                exit(EXIT_FAILURE);
            }
            else if (a->data.size() == 0)
            {
                std::cerr << "ERROR: " << a->accepted_flags[0] << " is a required APPEND argument." << std::endl;
                print_help();
                exit(EXIT_FAILURE);
            }
        }
    }
}


#endif 