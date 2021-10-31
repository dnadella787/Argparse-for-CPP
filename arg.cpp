
#include "arg.h"

argument::argument(const std::string& ARG_NAME) : arg_name(ARG_NAME) {}

void argument::set_nargs(const int& N_ARGS)
{
    n_args = N_ARGS;
}

void argument::set_action(const std::string& ACTION)
{
    action = ACTION;
}

void argument::set_help_message(const std::string& HELP_MESSAGE)
{
    help_message = HELP_MESSAGE;
}

