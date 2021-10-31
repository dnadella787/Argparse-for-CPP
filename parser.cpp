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

void parser::parse_args(const int& argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        
    }
}




void parser::print_arguments()
{
    for (auto x : known_arguments) std::cout << x.arg_name << std::endl;
}
