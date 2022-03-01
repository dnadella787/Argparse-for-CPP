#ifndef ARGPARSER_H
#define ARGPARSER_H 

#include <string>
#include <vector>
#include <iostream>
#include <string.h>
#include <set>
#include <map>
#include <sstream>


//macro for help message
#define NO_DESCRIPTION "no_description"

/*
*****************************
******* ACTION OBJECTs ******
*****************************
*/

namespace ACTION 
{ 



template<typename T>
T convert(const std::string& orig)
{
    std::istringstream s(orig);
    T converted_val;
    s >> converted_val;
    return converted_val;
}


class COUNT 
{
private:
    int count = 0; 

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        count++;
        if (flag_num + 1 == argc)
            return;
        else if (argv[flag_num + 1][0] != '-')
        {
            std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        return;
    }

    void requirement_not_met(const std::string accepted_flag)
    {
        std::cerr << "ERROR: " << accepted_flag << " is a required COUNT action argument" << std::endl;
    }

    bool supports_equals() {return false;}

    template<typename T>
    std::vector<T> get_helper(std::vector<T> *)
    {
        static_assert(std::is_integral<T>::value ||
                      std::is_same<std::string,T>::value,
                      "ERROR: [get<std::vector<T>>()] T must be numeric, string, char, or bool for COUNT");
        return {convert<T>(std::to_string(count))};
    }

    template<typename T>
    T get_helper(T*)
    {
        static_assert(std::is_integral<T>::value ||
                      std::is_same<std::string,T>::value,
                      "ERROR: [get<std::vector<T>>()] T must be numeric, string, char, or bool for COUNT");
        return convert<T>(std::to_string(count));
    }

    friend class argument;
    friend class parser;
};


class STORE_TRUE 
{
private:
    bool supports_equals = false;
    bool found = false;

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        if (flag_num + 1 < argc && argv[flag_num + 1][0] != '-')
        {
            std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag, action STORE_TRUE" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        found = true;
    }

    bool supports_equals() {return false;}

    template<typename T>
    T get_helper(T*)
    {
        static_assert(std::is_arithmetic<T>::value ||
                      std::is_same<T, bool>::value,
                      "ERROR: [get<T>()] T must be numeric or bool for STORE_TRUE");
        return static_cast<T>(found);
    }

    friend class argument;
    friend class parser;
};


class STORE_FALSE 
{
private:
    bool supports_equals = false;
    bool found = true;

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {   
        if (flag_num + 1 < argc && argv[flag_num + 1][0] != '-')
        {
            std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag, action STORE_FALSE" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        found = false;
    }

    bool supports_equals() {return false;}

    template<typename T>
    T get_helper(T*)
    {
        static_assert(std::is_arithmetic<T>::value ||
                      std::is_same<T, bool>::value,
                      "ERROR: [get<T>()] T must be numeric or bool for STORE_FALSE");
        return static_cast<T>(found);
    }

    friend class argument;
    friend class parser;
};


class STORE 
{
private:
    bool supports_equals = true;
    std::string data = "";

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        if (flag_num + 1 == argc)
        {
            std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        else if (argv[flag_num + 1][0] == '-')
        {
            std::cerr << "ERROR: input expected for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        else
        {
            data = argv[flag_num + 1];
        }
        if (flag_num + 2 == argc)
        {
            return;
        }
        else if (argv[flag_num + 2][0] != '-')
        {
            std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }

    }

    void parse_equal(const int& flag_num, const int& argc, char** argv, const int& equal_iter)
    {

        if (argv[flag_num][equal_iter + 1] == '\0')
        {
            std::cerr << "ERROR: no input after = for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        else if (flag_num + 1 < argc && argv[flag_num + 1][0] != '-')
        {
            std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        data = &argv[flag_num][equal_iter + 1];
    }

    void requirement_not_met(const std::string accepted_flag)
    {
        std::cerr << "ERROR: " << accepted_flag << " is a required STORE action argument" << std::endl;
    }

    bool check_empty()
    {
        if (data.size())
            return true;
        return false;
    }

    bool supports_equals() {return true;}

    template<typename T>
    std::vector<T> get_helper(std::vector<T> *)
    {
        static_assert(std::is_integral<T>::value ||
                      std::is_same<std::string,T>::value,
                      "ERROR: [get<std::vector<T>>()] T must be numeric, string, bool, or char for STORE");
        return {convert<T>(data)};
    }

    template<typename T>
    T get_helper(T*)
    {
        static_assert(std::is_integral<T>::value ||
                      std::is_same<std::string,T>::value,
                      "ERROR: [get<std::vector<T>>()] T must be numeric, string, bool, or char for STORE");

        return convert<T>(data);
    }

    bool get_helper(bool*)
    {
        return (data.size() > 0);
    }

    friend class argument;
    friend class parser;
};


class STORE_APPEND 
{
private:
    bool supports_equals = false;
    std::vector<std::string> data;
    int n_args = 1;

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        // if the max number of arguments have been gotten already 
        if (data.size() == n_args)
        {
            std::cerr << "ERROR: " << argv[flag_num] << " already has input, cannot input twice" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }

        // if the sufficient number of args surpasses the available num of args
        if (n_args + flag_num >= argc)
        {
            std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
        // otherwise iterate over the next n_args in char* argv[]
        for (int i = 1; i < n_args + 1; i++)
        {
            if (argv[flag_num + i][0] == '-')
            {
                std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
                // print_help();
                exit(EXIT_FAILURE);
            }
            else
            {
                data.push_back(argv[flag_num + i]);
            }
        }
        // if the sufficient number of args till the end
        if (flag_num + n_args + 1 == argc)
        {
            return;
        }
        //if there are more args after, the next one must start with '-'
        else if (argv[flag_num + a->n_args + 1][0] != '-')
        {
            std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
            // print_help();
            exit(EXIT_FAILURE);
        }
    }

    void requirement_not_met(const std::string accepted_flag)
    {
        std::cerr << "ERROR: " << accepted_flag << " is a required STORE_APPEND action argument" << std::endl;
    }


    bool check_empty()
    {
        if (!data.empty())
            return true;
        return false;
    }

    bool supports_equals() {return false;}

    template<typename T>
    std::vector<T> get_helper(std::vector<T> *)
    {
        static_assert(std::is_integral<T>::value ||
                      std::is_same<std::string,T>::value,
                      "ERROR: [get<std::vector<T>>()] T must be numeric, string, bool, or char for STORE_APPEND");
        std::vector<T> ret_vector(data.size());
        for (int i = 0; i < data.size(); ++i)
            ret_vector[i] = convert<T>(data[i]);

        return ret_vector;
    }

    template<typename T>
    T get_helper(T*)
    {
        static_assert(std::false_type::value, "ERROR: [get<T>()] T must be a vector for STORE_APPEND");
    }

    friend class argument;
    friend class parser; 
};

}

/*
*****************************
****** ARGUMENT OBJECT ******
*****************************
*/

namespace argparser 
{

class arg_base 
{
private:
    arg_base(const std::string& n) : arg_name(n) {}

    std::string arg_name;                      //name of argument
    std::vector<std::string> accepted_flags;   //vector of recognizable flags
    std::string help_message;                  //help message for specific argument, used by parser to generate usage 
    bool is_required = false;                  //if the flag is required or not, false by default

    virtual void equals_support() {}
    virtual void do_action(const int& flag_num, const int& argc, char** argv) {}
    virtual void do_equals_action(const int& flag_num, const int& argc, char** argv, const int& equal_ind) {}
    virtual void need_requirement() {}

};

template<typename action>
class argument : public arg_base 
{
private:
    static_assert(std::is_same<action, ACTION::COUNT>::value || 
                  std::is_same<action, ACTION::STORE_TRUE>::value ||
                  std::is_same<action, ACTION::STORE_FALSE>::value ||
                  std::is_same<action, ACTION::STORE>::value ||
                  std::is_same<action, ACTION::STORE_APPEND>::value,
                  "action must be 'COUNT', 'STORE_TRUE', 'STORE_FALSE', 'STORE', or 'STORE_APPEND'" );

    action* action_type;
    std::string help_message;
    bool is_required = false;

    virtual void do_action(const int& flag_num, const int& argc, char** argv)
    {
        action_type->parse_input(flag_num, argc, argv)
    }

    virtual bool equals_support()
    {
        return action_type->supports_equals();
    }

    virtual void do_equals_action(const int& flag_num, const int& argc, char** argv, const int& equal_ind)
    {
        action_type->parse_equal(flag_num, argc, argv, equal_ind);
    }

    virtual void need_requirement()
    {
        action_type->requirement_not_met(accepted_flags[0]);
    }


public:
    //constructors
    argument(const std::string& ARG_NAME);     //constructor that also takes in the argument, use this one
    ~argument();                               //destructor deallocates the action_type

    //public member functions to set up appropriate behavior
    template<typename ... T>
    void set_flags(const T&... NAMES);                      //all acceptable cmd line flags for this argument
    void set_nargs(const int& N_ARGS);                      //set the number of arguments to be processed
    void set_help_message(const std::string& HELP_MESSAGE); //set help message for the parser to use
    void set_requirement(const bool& REQUIREMENT);          //set the requirement, causes error if a required flag is not passed in

    bool is_empty();                                        //use to check if an argument of action STORE_APPEND has data inputted in or not

    template<typename T>
    T get()
    {
        return action_type->get_helper((T*)0);
    }


    friend class parser;                                    //parser can access accepted flags and store protocol to define appropriate behavior
};


/*
******* ARGUMENT FUNCTION DEFINITONS *******
*/

template<typename ... S>
void argument<T>::set_flags(const S&... NAMES)
{
    (accepted_flags.push_back(NAMES), ...);
}

template<typename T>
argument<T>::argument(const std::string& ARG_NAME) : arg_base(ARG_NAME) 
{
    action_type = new T;
}

template<typename T>
argument<T>::~argument()
{
    delete action_type;
}

template<typename T>
inline void argument<T>::set_nargs(const int& N_ARGS)
{
    static_assert(std::is_same<T, ACTION::STORE_APPEND>::value, "changing nargs only allowed for action STORE_APPEND");
    action_type->n_args = N_ARGS;
}

template<typename T>
void argument<T>::set_help_message(const std::string& HELP_MESSAGE)
{
    help_message = HELP_MESSAGE;
}

template<typename T>
void argument<T>::set_requirement(const bool& REQUIREMENT)
{
    static_assert(std::is_same<T, ACTION::STORE_APPEND>::value ||
                  std::is_same<T, ACTION::STORE>::value || 
                  std::is_same<T, ACTION::COUNT>::value,
                  "ERROR: set_requirement() can only be used on actions 'COUNT', 'STORE', or 'STORE_APPEND'");
    is_required = REQUIREMENT;
}

template<typename T>
bool argument<T>::is_empty()
{
    static_assert(std::is_same<T, ACTION::STORE_APPEND>::value ||
                  std::is_same<T, ACTION::STORE>::value,
                  "ERROR: is_empty() can only be used with actions 'STORE' or 'STORE_APPEND'");
    return action_type->check_empty();
}

template <typename T>
T argument::get()
{
    return get_helper((T*)0); 
}

//return a single value
template <typename T>
T argument::get_helper(T*)
{
    return convert<T>(data[0]);
}

//return vector of values 
template <typename T>
std::vector<T> argument::get_helper(std::vector<T> *)
{
    std::vector<T> ret_vector(data.size());
    for (size_t i = 0; i < data.size(); ++i)
        ret_vector[i] = convert<T>(data[i]);

    return ret_vector;
}

template<typename T>
T convert(const std::string& s)
{
    std::istringstream ss(s);
    T converted_val;
    ss >> converted_val;
    return converted_val;
}



/*
******************************
******* PARSER OBJECT ********
******************************
*/

class parser 
{
private:
    //member variables
    std::map<std::string, arg_base*> known_arguments;           //holds pointers to all arguments
    std::string prog_name = "PROG";                         //program name, for help message
    std::string description = NO_DESCRIPTION;               //description, for help message
    std::set<arg_base*> required_args;                          //all the arguments that are required
    
    //private helper functions used internally
    void print_help();                                      //prints help message for -h, --help



public:
    //constructors
    parser(){} 
    
    //public member functions 
    template <typename Arg>
    void add_arguments(Arg&& arg);                              //variadic template, base case

    template <typename Arg, typename ...Args>
    void add_arguments(Arg&& arg, Args&& ...args);              //variadic template to add any number of arguments

    //help message related functions
    void set_prog_name(const std::string& PROG_NAME);           //set the program name
    void set_description(const std::string& DESCRIPTION);       //set the description name

    //output related function
    void parse_args(const int& argc, char** argv);              //parses args and assigns variables, whole point of this
};


/*
******* PARSER FUNCTION DEFINITONS *******
*/

template <typename Arg>
void parser::add_arguments(Arg&& arg)
{
    if (arg.is_required)
        required_args.insert(&std::forward<Arg>(arg));

    for (const std::string& flag : arg.accepted_flags)
        known_arguments[flag] = &std::forward<Arg>(arg);
}

template <typename Arg, typename ...Args>
void parser::add_arguments(Arg&& arg, Args&& ...args) 
{
    if (arg.is_required)
        required_args.insert(&std::forward<Arg>(arg));
        
    for (const std::string& flag : arg.accepted_flags)
        known_arguments[flag] = &std::forward<Arg>(arg);
    
    add_arguments(std::forward<Args>(args)...);
}

inline void parser::set_prog_name(const std::string& PROG_NAME)
{
    prog_name = PROG_NAME;
}

inline void parser::set_description(const std::string& DESCRIPTION)
{
    description = DESCRIPTION;
}

inline void parser::parse_args(const int& argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            // print_help();
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            std::string curr_flag(argv[i]);
            size_t ind = curr_flag.find("=")
            if (ind != std::string::npos)  //if there is an '=' in the current arg
            {
                auto iter = known_arguments.find(curr_flag.substr(0, ind));
                if (iter != known_arguments.end()) //if this flag is recognized 
                {
                    if (iter->second->equals_support()) //if this flag supports '='
                        iter->second->do_equals_action(i, argc, argv, ind);
                    else    //otherwise this flag doesnt support '=' assignment, error
                    {
                        std::cerr << "ERROR: " << curr_flag.substr(0,ind) << "does not have action STORE, doesnt accept '='" << std::endl;
                        // print_help();
                        exit(EXIT_FAILURE);
                    }
                }
                else //flag is not recognized
                {
                    std::cerr << "ERROR: " << curr_flag.substr(0,ind) << " is not a recognized flag." << std::endl;
                    // print_help();
                    exit(EXIT_FAILURE);
                }
            }
            else //no equals parse normally
            {
                auto iter = known_arguments.find(argv[i]);
                if (iter != known_arguments.end()) //if flag is recognized
                {
                    iter->second->do_action(i, argc, argv);
                    if (iter->second->is_required)
                        required_args.erase(iter->second);
                }
                else //otherwise its an error 
                {
                    std::cerr << "ERROR: " << argv[i] << " is not a recognized argument" << std::endl;
                    // print_help();
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    
    //there should be no required arguments remaining
    if (!required_args.empty())
    {
        while (!required_args.empty())
        {
            base* b = *required_args.begin();
            b->need_requirement();
            required_args.erase(a);
        }
        print_help();
        exit(EXIT_FAILURE);
    }
}


// inline void parser::print_help()
// {
//     std::cout << "usage: " << prog_name << " ";


//     for (auto iter = known_arguments.begin(); iter != known_arguments.end(); ++iter)
//     {
//         if (iter->second->printed_already)
//             continue;

//         std::cout << "[";
//         for (int i = 0; i < iter->second->accepted_flags.size() - 1; i++)
//         {
//             std::cout << " " << iter->second->accepted_flags[i] << " |";
//         }
//         std::cout << " " << iter->second->accepted_flags.back();
//         std::cout << " ] ";
//         iter->second->printed_already = true;
//     }
    
//     std::cout << std::endl << std::endl;
//     if (description != NO_DESCRIPTION)
//     {
//         std::cout << "description:" << std::endl;
//         std::cout << "  " << description << std::endl;
//         std::cout << std::endl << std::endl;
//     }


//     std::cout << "options:" << std::endl;
//     for (auto iter = known_arguments.begin(); iter != known_arguments.end(); ++iter)
//     {
//         if (!iter->second->printed_already)
//             continue;

//         argument* x = iter->second;
//         std::cout << "  ";
//         for (int i = 0; i < x->accepted_flags.size() - 1; i++)
//         {
//             std::cout << x->accepted_flags[i] << ", ";
//         }
//         std::cout << x->accepted_flags.back() << "   ==>   "; 
//         if (x->action == STORE)
//         {
//             std::cout << x->help_message << " (usage: ";
//             std::cout << x->accepted_flags[0] << "=[input] or ";
//             std::cout << x->accepted_flags[0] << " [input])" << std::endl << std::endl;
//         }
//         else if (x->action == APPEND)
//         {
//             std::cout << x->help_message << " (usage: ";
//             std::cout << x->accepted_flags[0] << " [input1] ... [inputk])" << std::endl << std::endl;
//         }
//         else 
//             std::cout << x->help_message << std::endl << std::endl;
        
//         x->printed_already = false; 
//     }

//     std::cout << "  " << "-h, --help   ==>   generates this help/usage message" << std::endl;
// }

}

#endif 