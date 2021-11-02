
Currently working on:
 - getting parser/argument objects to return value to be used
 - updating function descriptions on README
 - update the print_help function
 - work on efficiency

Parsing itself is done.

An argparse implementation written in C++. This version is meant to use more object oriented design to do the parsing.


There is an argument class that is similar to the python add_argument method except that each argument must be initialized as an argument object first and then added to the parser object. 

Important Notes:
To create a positional argument you must set the nargs to be 0 (which it is by default) and add the name of the positional argument via set_flags(). You must also set the action to be STORE. 

The relavent function calls for the argument class are:

| Function | Description |
|------|------|
|argument(const std::string& ARG_NAME) | Constructor that takes in a string parameter and sets it to the name of the argument. The name of the argument is used to build the help message|
|void set_flags(const std::string& ...) | Allows you to add both relavent flags to an argument but also the positional argument name. This function uses variadic templates so any number of names can be added in at once. Also, the function must be called or otherwise the parser will return an error |
|void set_help_message(const std::string& HELP_MESSAGE) | Function takes in a string that is set to be the help message. The help message is displayed when the -h flag is passed in and is automatically put together by the parser object. |
|void set_action(const std::string ACTION)| This function sets how the variable is stored in the argument class. The macros to specify this are below but by default it is set to be NONE meaning nothing is stored.|
|void set_nargs(const int& NARGS)| Tells the parser exactly how many arguments after the flag to store in. If another flag is within the range after, then the parser will return an error. In addition, the default is set to be 0 meaning nothing is stored. You must change this if you choose the STORE action. |
|void set_default(const std::string& DEFAULT_VALUE)| Passed in is the default value, as a string, that is converted by the argument into the appropriate type. The default value is the value that is stored normally if no flag is recieved.|
|void set_required(const bool& REQUIRED)| This function tells the parser whether or not the argument is required. By default, it is set that the argument is not required.|
|void set_variable_type(const std::string& VAR_TYPE)| This function tells the argument which type the arguments should be stored as. This is not impacted by the action type. This only impacts whether each argument that is captured is taking in as a string or int but not as a vector or an individual variable.|



