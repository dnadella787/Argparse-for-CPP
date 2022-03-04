#ifndef ARGPARSER_H
#define ARGPARSER_H 

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include <type_traits>

template<typename>
struct is_std_vector : std::false_type {};

template<typename T, typename A>
struct is_std_vector<std::vector<T,A>> : std::true_type {};

/*
****** SETUP HELP MESSAGE ******
*/
namespace help 
{
std::string usage_message;
std::string description;
std::string options = "options : \n";
std::string prog_name = "PROG";
void print_help()
{
    std::cout << "usage : " << prog_name << " " << usage_message + '\n' + description + '\n' + options;
    std::cout << "-h, --help   ==>   generates this help/usage message" << std::endl;
}
}

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

template<>
bool convert<bool>(const std::string& s)
{
    return (s.size() > 0);
}

class action_base 
{
protected:
    action_base() {}
    virtual void do_nothing() {}
};

class COUNT : public action_base
{
public:
    int count; 

    COUNT() 
    {
        count = 0;
    }

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        count++;
        if (flag_num + 1 == argc)
            return;
        else if (argv[flag_num + 1][0] != '-')
        {
            std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag" << std::endl;
            help::print_help();
            exit(EXIT_FAILURE);
        }
        return;
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
};

class STORE_TRUE : public action_base 
{
public:
    bool found;

    STORE_TRUE()
    {
        found = false;
    }

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        if (flag_num + 1 < argc && argv[flag_num + 1][0] != '-')
        {
            std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag, action STORE_TRUE" << std::endl;
            help::print_help();
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
};

class STORE_FALSE : public action_base
{
public:
    bool found;

    STORE_FALSE() 
    {
        found = true;
    }

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {   
        if (flag_num + 1 < argc && argv[flag_num + 1][0] != '-')
        {
            std::cerr << "ERROR: no input expected for " << argv[flag_num] << " flag, action STORE_FALSE" << std::endl;
            help::print_help();
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
};

class STORE : public action_base 
{
public:
    std::string data;

    STORE()
    {
        data = "";
    }

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        if (flag_num + 1 == argc)
        {
            std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
            help::print_help();
            exit(EXIT_FAILURE);
        }
        else if (argv[flag_num + 1][0] == '-')
        {
            std::cerr << "ERROR: input expected for " << argv[flag_num] << " flag" << std::endl;
            help::print_help();
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
            help::print_help();
            exit(EXIT_FAILURE);
        }

    }

    bool check_empty()
    {
        return !(data.size() > 0);
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
                      "ERROR: [get<T>()] T must be numeric, string, bool, or char for STORE");

        return convert<T>(data);
    }
};

class STORE_APPEND : public action_base 
{
public:
    std::vector<std::string> data;
    int n_args;

    STORE_APPEND()
    {
        n_args = 1;
    }

    void parse_input(const int& flag_num, const int& argc, char** argv)
    {
        // if the max number of arguments have been gotten already 
        if (data.size() == n_args)
        {
            std::cerr << "ERROR: " << argv[flag_num] << " already has input, cannot input twice" << std::endl;
            help::print_help();
            exit(EXIT_FAILURE);
        }

        // if the sufficient number of args surpasses the available num of args
        if (n_args + flag_num >= argc)
        {
            std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
            help::print_help();
            exit(EXIT_FAILURE);
        }
        // otherwise iterate over the next n_args in char* argv[]
        for (int i = 1; i < n_args + 1; i++)
        {
            if (argv[flag_num + i][0] == '-')
            {
                std::cerr << "ERROR: not enough inputs for " << argv[flag_num] << " flag" << std::endl;
                help::print_help();
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
        else if (argv[flag_num + n_args + 1][0] != '-')
        {
            std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
            help::print_help();
            exit(EXIT_FAILURE);
        }
    }


    bool check_empty()
    {
        return data.empty();
    }

    bool supports_equals() {return false;}

    template<typename T>
    T get_helper(T*)
    {
        static_assert(is_std_vector<T>::value, 
                       "ERROR: [get<T>()] T must be a vector for STORE_APPEND");
        using S = typename T::value_type;
        static_assert(std::is_integral<S>::value ||
                      std::is_same<std::string,S>::value,
                      "ERROR: [get<std::vector<T>>()] T must be numeric, string, bool, or char for STORE_APPEND");

        std::vector<S> ret_vector;
        std::transform(std::begin(data), 
                       std::end(data), 
                       std::back_inserter(ret_vector), 
                       [](const std::string& s) { return convert<S>(s); });

        return ret_vector;
    }
};

/*
***** PUBLIC HELPER FUNCTIONS *****
*/

void requirement_not_met(const std::string accepted_flag)
{
    std::cerr << "ERROR: " << accepted_flag << " is a required argument" << std::endl;
}

void parse_equal(STORE* argument, const int& flag_num, const int& argc, char** argv, const int& equal_iter)
{

    if (argv[flag_num][equal_iter + 1] == '\0')
    {
        std::cerr << "ERROR: no input after = for " << argv[flag_num] << " flag" << std::endl;
        help::print_help();
        exit(EXIT_FAILURE);
    }
    else if (flag_num + 1 < argc && argv[flag_num + 1][0] != '-')
    {
        std::cerr << "ERROR: too many inputs for " << argv[flag_num] << " flag" << std::endl;
        help::print_help();
        exit(EXIT_FAILURE);
    }
    argument->data = &argv[flag_num][equal_iter + 1];
}


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
protected:
    arg_base(const std::string& n) : arg_name(n) {}

    std::string arg_name;                      //name of argument
    std::vector<std::string> accepted_flags;   //vector of recognizable flags
    std::string help_message;                  //help message for specific argument, used by parser to generate usage 
    bool is_required = false;                  //if the flag is required or not, false by default

    virtual bool equals_support() {return true;}
    virtual void do_action(const int& flag_num, const int& argc, char** argv) {}
    virtual void need_requirement() {}
    // virtual std::string get_first_flag() {return "";}
    virtual ACTION::action_base* get_action_type() {return nullptr;}

    friend class parser;
};

template<typename action>
class argument : public arg_base 
{
private:
    /*
    ******* CHECK ACTION IS SUPPORTED *******
    */
    static_assert(std::is_same<action, ACTION::COUNT>::value || 
                  std::is_same<action, ACTION::STORE_TRUE>::value ||
                  std::is_same<action, ACTION::STORE_FALSE>::value ||
                  std::is_same<action, ACTION::STORE>::value ||
                  std::is_same<action, ACTION::STORE_APPEND>::value,
                  "action must be 'COUNT', 'STORE_TRUE', 'STORE_FALSE', 'STORE', or 'STORE_APPEND'" );

    /*
    ******* BASIC INTERNAL DATA *******
    */
    action* action_type;                      //pointer to the action type, houses parser and get functions


    /*
    ******* INTERNAL VIRTUAL FUNCTIONS FOR PARSING/INFO FROM PARSER OBJ *******
    */
    virtual bool equals_support()
    {
        return action_type->supports_equals();
    }

    virtual void do_action(const int& flag_num, const int& argc, char** argv)
    {
        action_type->parse_input(flag_num, argc, argv);
    }

    // virtual std::string get_first_flag()
    // {
    //     return accepted_flags
    // }

    virtual action* get_action_type()
    {
        return action_type;
    }

public:
    /*
    ******* CONSTRUCTOR/DESTRUCTOR *******
    */
    argument(const std::string& ARG_NAME) : arg_base(ARG_NAME) 
    {
        action_type = new action;
    }
    ~argument()                             
    {
        delete action_type;
    }


    /*
    ******* PUBLIC FUNCTIONS TO SETUP DESIRED BEHAVIOR *******
    */
    template<typename ... T>
    void set_flags(const T&... NAMES)                       //all acceptable cmd line flags for this argument
    {
        (accepted_flags.push_back(NAMES), ...);
    }

    void set_nargs(const int& N_ARGS)                       //set the number of arguments to be processed
    {
        static_assert(std::is_same<action, ACTION::STORE_APPEND>::value, "changing nargs only allowed for action STORE_APPEND");
        action_type->n_args = N_ARGS;
    }
    void set_help_message(const std::string& HELP_MESSAGE)  //set help message for the parser to use
    {
        help_message = HELP_MESSAGE;
    }
    void set_requirement(const bool& REQUIREMENT)           //set the requirement, causes error if a required flag is not passed in
    { 
        static_assert(std::is_same<action, ACTION::STORE_APPEND>::value ||
                      std::is_same<action, ACTION::STORE>::value || 
                      std::is_same<action, ACTION::COUNT>::value,
                      "ERROR: set_requirement() can only be used on actions 'COUNT', 'STORE', or 'STORE_APPEND'");
        is_required = REQUIREMENT;
    }

    /*
    ******* INFO GATHERING PUBLIC FUNCTIONS FOR POST PARSING *******
    */
    bool is_empty()                                         
    {
        static_assert(std::is_same<action, ACTION::STORE_APPEND>::value ||
                    std::is_same<action, ACTION::STORE>::value,
                    "ERROR: is_empty() can only be used with actions 'STORE' or 'STORE_APPEND'");
        return action_type->check_empty();
    }

    template<typename T>
    T get()
    {
        return action_type->get_helper((T*)0);
    }

    friend class parser;                                    //parser can access accepted flags and store protocol to define appropriate behavior
};



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
    std::string prog_name = "PROG";                             //program name, for help message
    bool no_description = true;                                 //tell if the description has been set or not
    std::set<arg_base*> required_args;                          //all the arguments that are required
    
public:
    //constructors
    parser(){} 
    
    //public member functions 
    template <typename Arg>
    void add_arguments(Arg&& arg)                               //variadic template, base case
    {
        if (arg.is_required)
            required_args.insert(&std::forward<Arg>(arg));
        
        help::usage_message += "[ ";
        std::string temp_options = "";
        for (const std::string& flag : arg.accepted_flags)
        {
            help::usage_message += flag + " | ";
            temp_options += ", " + flag;

            known_arguments[flag] = &std::forward<Arg>(arg);
        }

        help::options += temp_options.substr(2, temp_options.size()) + "  ==>  " + arg.help_message + '\n';
        help::usage_message[help::usage_message.size()-2] = ']';
    }

    template <typename Arg, typename ...Args>
    void add_arguments(Arg&& arg, Args&& ...args)               //variadic template to add any number of arguments
    {
        if (arg.is_required)
            required_args.insert(&std::forward<Arg>(arg));
            
        help::usage_message += "[ ";
        std::string temp_options = "";
        for (const std::string& flag : arg.accepted_flags)
        {
            help::usage_message += flag + " | ";
            temp_options += ", " + flag;

            known_arguments[flag] = &std::forward<Arg>(arg);
        }

        help::options += temp_options.substr(2, temp_options.size()) + "  ==>  " + arg.help_message + '\n';
        help::usage_message[help::usage_message.size()-2] = ']';
        
        add_arguments(std::forward<Args>(args)...);
    }

    //help message related functions
    void set_prog_name(const std::string& PROG_NAME)            //set the program name
    {
        help::prog_name = PROG_NAME;
    }

    void set_description(const std::string& DESCRIPTION)        //set the description name
    {
        help::description = "description : " + DESCRIPTION;
    }

    //output related function
    void parse_args(const int& argc, char** argv)               //parses args and assigns variables, whole point of this
    {
        for (int i = 1; i < argc; i++)
        {
            std::string curr(argv[i]);
            if (curr == "-h" || curr == "--help")
            {
                help::print_help();
                exit(EXIT_SUCCESS);
            }
            if (argv[i][0] == '-')
            {
                std::string curr_flag(argv[i]);
                size_t ind = curr_flag.find("=");
                if (ind != std::string::npos)  //if there is an '=' in the current arg
                {
                    auto iter = known_arguments.find(curr_flag.substr(0, ind));
                    if (iter != known_arguments.end()) //if this flag is recognized 
                    {
                        if (iter->second->equals_support()) //if this flag supports '='
                            ACTION::parse_equal(dynamic_cast<ACTION::STORE*>(iter->second->get_action_type()), 
                                                i,
                                                argc, 
                                                argv, 
                                                ind);
                        else    //otherwise this flag doesnt support '=' assignment, error
                        {
                            std::cerr << "ERROR: " << curr_flag.substr(0,ind) << " does not have action STORE, doesn't accept '='" << std::endl;
                            help::print_help();
                            exit(EXIT_FAILURE);
                        }
                    }
                    else //flag is not recognized
                    {
                        std::cerr << "ERROR: " << curr_flag.substr(0,ind) << " is not a recognized flag." << std::endl;
                        help::print_help();
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
                        help::print_help();
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
                arg_base* b = *required_args.begin();
                ACTION::requirement_not_met(b->accepted_flags[0]);
                required_args.erase(b);
            }
            help::print_help();
            exit(EXIT_FAILURE);
        }
    }
};


}

#endif 