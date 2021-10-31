
#include "arg.h"

argument::argument(const std::string& ARG_NAME) : arg_name(ARG_NAME) {}


void argument::set_help_message(const std::string& HELP_MESSAGE)
{
    help_message = HELP_MESSAGE;
}

