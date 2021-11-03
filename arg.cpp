
#include "arg.h"

argument::argument(const std::string& ARG_NAME) : arg_name(ARG_NAME) {}

void argument::set_nargs(const int& N_ARGS)
{
    n_args = N_ARGS;
}

void argument::set_action(const int& ACTION)
{
    if (ACTION == STORE)
    {
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

void argument::set_help_message(const std::string& HELP_MESSAGE)
{
    help_message = HELP_MESSAGE;
}

void argument::set_requirement(const bool& REQUIREMENT)
{
    is_required = REQUIREMENT;
}

std::string argument::get_val()
{
    if (action != STORE)
    {
        std::cerr << "ERROR: " << arg_name << " does not have action STORE, cannot use get_val()" << std::endl;
        exit(-1);
    }
    
    else if (data.size() == 0)
        return NO_INPUT;

    return data[0];
}

std::vector<std::string> argument::get_vals()
{
    if (action != APPEND)
    {
        std::cerr << "ERROR: " << arg_name << " does not have action APPEND, cannot use get_vals()" << std::endl;
        exit(-1);
    }
    return data;
}

int argument::get_count()
{
    if (action != COUNT)
    {
        std::cerr << "ERROR: " << arg_name << " does not have COUNT action, cannot use get_count()" << std::endl;
        exit(-1);
    }
    return count;
}

bool argument::is_used()
{
    if (action == STORE_FALSE)
    {
        if (data[0] != STORE_F_DEFAULT)
            return true;
        return false;
    }
    else if (action == STORE_TRUE)
    {
        if (data[0] != STORE_T_DEFAULT)
            return true;
        return false;
    }
    else
    {
        std::cerr << "ERROR: " << arg_name << " does not have action STORE_FALSE or STORE_TRUE, cannot use is_used()" << std::endl;
        exit(-1);
    }
}

bool argument::is_empty()
{
    if (action == STORE )
    {
        if (get_val() == NO_INPUT)
            return true;
        return false;
    }
    else if (action == APPEND)
    {
        if (get_vals().size() == 0)
            return true;
        return false;
    }
    else 
    {
        std::cerr << "ERROR: is_empty() can only be used with actions STORE and APPEND, cannot use with " << arg_name << std::endl;
        exit(-1);
    }
}


