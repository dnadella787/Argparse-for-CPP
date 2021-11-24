#include "parser.h"

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


int parser::match(const std::string& flag)
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


int parser::find_equal(const std::string& input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == '=')
            return i;
    }
    return -1;
}


void parser::action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv)
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


void parser::equal_action_store(const int& arg_num, const int& flag_num, const int& argc, char** argv, const int& equal_iter)
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


void parser::action_store_true(const int& arg_num, const int& flag_num, const int& argc, char** argv)
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


void parser::action_store_false(const int& arg_num, const int& flag_num, const int& argc, char** argv)
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


void parser::action_append(const int& arg_num, const int& flag_num, const int& argc, char** argv)
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


void parser::action_count(const int& arg_num, const int& flag_num, const int& argc, char** argv)
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


void parser::do_action(const int& arg_num, const int& flag_num, const int& argc, char** argv, const int& ACTION)
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


void parser::parse_args(const int& argc, char** argv)
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
                    if (known_arguments[arg_num]->action == STORE)
                        equal_action_store(arg_num, i, argc, argv, equal_iter);
                    else 
                    {
                        std::cerr << "ERROR: '=' can only be used for single input arguments like -f=file" << std::endl;
                        print_help();
                        exit(EXIT_FAILURE);
                    }
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
        if (a->is_required)
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
                std::cerr << "ERROR: " << a->accepted_flags[0] << " is a required argument." << std::endl;
                print_help();
                exit(EXIT_FAILURE);
            }
        }
    }
}
