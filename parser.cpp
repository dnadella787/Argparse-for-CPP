#include "parser.h"
#include <string.h>


void parser::set_prog_name(const std::string& PROG_NAME)
{
    prog_name = PROG_NAME;
}

void parser::set_description(const std::string& DESCRIPTION)
{
    description = DESCRIPTION;
}


void parser::print_help()
{
    std::cout << "usage: " << prog_name << " ";
    for (auto x : known_arguments)
    {
        std::cout << "[";
        for (int i = 0; i < x.accepted_flags.size() - 1; i++)
        {
            std::cout << " " << x.accepted_flags[i] << " |";
        }
        std::cout << " " << x.accepted_flags.back();
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
    for (auto x : known_arguments)
    {
        std::cout << "  ";
        for (int i = 0; i < x.accepted_flags.size() - 1; i++)
        {
            std::cout << x.accepted_flags[i] << ", ";
        }
        std::cout << x.accepted_flags.back() << "    ";
        std::cout << x.help_message << std::endl;
    }

    std::cout << "  " << "-h, --help    generates this help/usage message" << std::endl;
}


int parser::match(const std::string& flag)
{
    for (int i = 0; i < known_arguments.size(); i++)
    {
        for (std::string arg : known_arguments[i].accepted_flags)
        {
            if (arg == flag)
                return i;
        }
    }
    return -1;
}


void parser::action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{
    if (flag_num + 1 == argc)
    {
        std::cerr << "not enough inputs for " << argv[flag_num] << " flag" << std::endl;
        exit(-1);
    }
    else if (argv[flag_num + 1][0] == '-')
    {
        std::cerr << "input expected for " << argv[flag_num] << " flag" << std::endl;
        exit(-1);
    }
    else
    {
        known_arguments[arg_num].data.push_back(argv[flag_num + 1]);
    }
    if (flag_num + 2 == argc)
    {
        return;
    }
    else if (argv[flag_num + 2][0] != '-')
    {
        std::cerr << "too many inputs for " << argv[flag_num] << " flag" << std::endl;
        exit(-1);
    }

}


void parser::action_store_true(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{   
    if (flag_num + 1 == argc)
    {
        known_arguments[arg_num].data[0] = STORE_F_DEFAULT;
        return;
    }
    if (argv[flag_num + 1][0] != '-')
    {
        std::cerr << "no input expected for " << argv[flag_num] << " flag, action STORE_TRUE" << std::endl;
        exit(-1);
    }
    else 
    {
        known_arguments[arg_num].data[0] = STORE_F_DEFAULT;
    }
}


void parser::action_store_false(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{   
    if (flag_num + 1 == argc)
    {
        known_arguments[arg_num].data[0] = STORE_T_DEFAULT;
        return;
    }
    if (argv[flag_num + 1][0] != '-')
    {
        std::cerr << "no input expected for " << argv[flag_num] << " flag, action STORE_FALSE" << std::endl;
        exit(-1);
    }
    else 
    {
        known_arguments[arg_num].data[0] = STORE_T_DEFAULT;
    }
}


void action_append(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{

}


void action_count(const int& arg_num, const int& flag_num, const int& argc, char** argv)
{

}

void parser::parse_args(const int& argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_help();
            return;
        }
        if (argv[i][0] == '-')
        {
            int arg_num = match(argv[i]);
            if (arg_num >= 0)
            {
                if (known_arguments[arg_num].action == STORE)
                {
                    action_store(arg_num, i, argc, argv);
                }
                else if (known_arguments[arg_num].action == STORE_TRUE)
                {
                    action_store_true(arg_num, i, argc, argv);
                }
                else if (known_arguments[arg_num].action == STORE_FALSE)
                {
                    action_store_false(arg_num, i, argc, argv);
                }
                // else if (known_arguments[arg_num].action == APPEND)
                    
                // else if (known_arguments[arg_num].action == COUNT)
                // else
                // {
                //     std::cerr << "ERROR: " << known_arguments[arg_num].arg_name << " has an invalid action, use STORE, STORE_TRUE, STORE_FALSE, APPEND, COUNT" << std::endl;
                //     exit(-1);
                // }
            }
            else
            {
                std::cerr << argv[i] << " is not a recognized argument" << std::endl;
                exit(-1);
            }
        }
    }
    for (argument a : known_arguments)
    {
        std::cout << a.arg_name << std::endl;
        for (std::string x : a.data)
            std::cout << x << std::endl;
    }

}
