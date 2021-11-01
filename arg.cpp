
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
    action = ACTION;
}

void argument::set_help_message(const std::string& HELP_MESSAGE)
{
    help_message = HELP_MESSAGE;
}

