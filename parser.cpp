#include "parser.h"
#include <iomanip>



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


void parser::parse_arg_helper()
{
    
}

void parser::parse_args(const int& argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            int arg_num = match(argv[i]);
            if (arg_num >= 0)
            {
                std::cout << known_arguments.at(arg_num).arg_name << std::endl;
                
                //actually do the proper storage protocols and stuff
            }
            else
            {
                std::cout << argv[i] << " is not a recognized" << std::endl;
                return;
            }
        }
        else
        {
            continue;
        }
    }
}
