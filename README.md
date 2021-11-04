
Currently working on:
 - portability as a library
 - updating function descriptions on README
 - work on efficiency


THESE FUNCTIONS ARE NOT UP TO DATE ANYMORE, this has had a complete overhaul I will update soon :)
Parsing itself is done.

An argparse implementation written in C++. This version is meant to use more object oriented design to do the parsing.


There is an argument class that is similar to the python add_argument method except that each argument must be initialized as an argument object first and then added to the parser object. 

Important Notes:
To create a positional argument you must set the nargs to be 0 (which it is by default) and add the name of the positional argument via set_flags(). You must also set the action to be STORE. 

Argument Class Functions:

    argument(std::string)

initialize the argument object with a name as the parameter. The name is useful for the help message and error outputs but is not necessary.

    void set_flags(std::string ...)

Add all allowable command line flags to the argument class. You can add in as many at one time as needed.

    void set_nargs(int)

Set the number of additional arguments that should be included after any of the allowable flags. Note that n_args is set to 1 by default, to change this you need to use this function or set_action will also automatically change it.

    void set_action(MACRO)

This defines how the argument will be parsed by the parser object. There are 5 different possible actions:

    STORE

n_args is set to 1 and exactly one variable after any of the allowable flags is allowed. If the flag is used more than once then it overwrites the variable. If you do not want overwrites to be allowed, use the action APPEND instead and check the size of the returned vector from get_append()

    STORE_TRUE or STORE_FALSE

If the flag is passed in, then the argument is stored as true and false for STORE_TRUE and STORE_FALSE respectively. n_args is set to 1 and any input after the flag will raise error.

    APPEND

n_args number of inputs will be taken in after the acceptable flag and stored. If you try to use the flag more than once it will raise an error, will not overwrite. If less than n_arg inputs or more are passed in, an error will be raised.

    COUNT

Counts the number of times the flag has been specified on the command line. It is 0 by default and if any inputs are specified after the flag, it will raise an error.

    void set_requirement(bool)

Specified whether the argument must be specified on command line. The requirement is false by default but you can change it by setting it to set_requirement(true). 

    std::string get_store()

Returns a string of the value specified for the flag. Can only be used for action STORE, else error will be raised. If there is no input, then the macro NO_INPUT is returned but it is best to check if this has ocurred using the function is_empty() 

    bool get_store_tf()

Returns the value that is stored based on the actions STORE_TRUE or STORE_FALSE. If it is STORE_TRUE and the flag is specified, then it returns true, else false. The case for STORE_FALSE is similar. Can only be used with the action STORE_TRUE or STORE_FALSE, otherwise an error will happen.
    
    std::vector<std::string> get_append()

Returns a vector of strings with all processed inputs. If no inputs are specified it will return an empty vector but it is best to check this using is_empty(). If the action of the argument is not APPEND then it will raise an error.

    int get_count()

Returns the number of times an argument has been specified. Can only be used with action COUNT.

    bool is_empty()

Used with the action STORE and APPEND. If no input is specified in either case the function returns true, else false.







