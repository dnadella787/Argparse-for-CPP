THINGS TO DO:
- implement several template classes for STORE, STORE_APPEND, etc. for compile time error checking when using get() and also adding in multiple versions for the get<> function when specifically COUNT or STORE_TRUE are used (this will be implemented using enable_if(..))


Currently under some changes for efficiency sake and more modernizing. I am working on adding more overloads for the get() and I refactored much of the internals already but there is more work to do. The test suite also needs to be updated as a lot of the interface has changed so many of the tests do not work as they should.


An argparse implementation written in C++. This version is meant to use more object oriented design to do the parsing. Includes a test module that checks for proper behavior. The test module uses CMake with googletest to run.

This is no longer a work in progress since it works for my purposes, but, I will continue to add more functionality for negative numbers and variability of number of args for a flag. Also going to make it so all errors are outputted before exiting instead of just the first one. 


There is an argument class that is similar to the python add_argument method except that each argument must be initialized as an argument object first and then added to the parser object. 

To install, simply run:

    sudo make install 

To run tests:

    make tests

important to note that this will produce a "build" directry in run-tests which will have the test executable "./argparser" and all of the CMake dependencies.


Argument Class Functions:

    argument::argument(std::string)

initialize the argument object with a name as the parameter. The name is useful for the help message and error outputs but is not necessary.

    void set_flags(std::string ...)

Add all allowable command line flags to the argument class. You can add in as many at one time as needed.

    void argument::set_nargs(int)

Set the number of additional arguments that should be included after any of the allowable flags. Note that n_args is set to 1 by default, to change this you need to use this function or set_action will also automatically change it.

    void argument::set_requirement(bool)

Specified whether the argument must be specified on command line. The requirement is false by default but you can change it by setting it to set_requirement(true). 

    std::string argument::get_store()

Returns a string of the value specified for the flag. Can only be used for action STORE, else error will be raised. If there is no input, then the macro NO_INPUT is returned but it is best to check if this has ocurred using the function is_empty() 

    bool argument::get_store_tf()

Returns the value that is stored based on the actions STORE_TRUE or STORE_FALSE. If it is STORE_TRUE and the flag is specified, then it returns true, else false. The case for STORE_FALSE is similar. Can only be used with the action STORE_TRUE or STORE_FALSE, otherwise an error will happen.
    
    std::vector<std::string> argument::get_append()

Returns a vector of strings with all processed inputs. If no inputs are specified it will return an empty vector but it is best to check this using is_empty(). If the action of the argument is not APPEND then it will raise an error.

    int argument::get_count()

Returns the number of times an argument has been specified. Can only be used with action COUNT.

    bool argument::is_empty()

Used with the action STORE and APPEND. If no input is specified in either case the function returns true, else false.

    void argument::set_action(MACRO)

This defines how the argument will be parsed by the parser object. There are 5 different possible actions included below.

Possible set_action MACROS:

    STORE

n_args is set to 1 and exactly one variable after any of the allowable flags is allowed. If the flag is used more than once then it overwrites the variable. If you do not want overwrites to be allowed, use the action APPEND instead and check the size of the returned vector from get_append(). This is the only type of action that allows '=' in the flag like "-f=filename".

    STORE_TRUE or STORE_FALSE

If the flag is passed in, then the argument is stored as true and false for STORE_TRUE and STORE_FALSE respectively. n_args is set to 1 and any input after the flag will raise error.

    APPEND

n_args number of inputs will be taken in after the acceptable flag and stored. If you try to use the flag more than once it will raise an error, will not overwrite. If less than n_arg inputs or more are passed in, an error will be raised.

    COUNT

Counts the number of times the flag has been specified on the command line. It is 0 by default and if any inputs are specified after the flag, it will raise an error.


Parser Object Functions:

    parser::parser()

Default constructor for parser, there is no constructor that take parameters for this object.

    void parser::add_arguments(argument*...)

Allows you to add all arguments to the parser object with as many arguments as possible. NOTE: they must be pointers to arguments, not the actual argument.

    void parser::set_prog_name(std::string)

Allows user to set the program name used for the help message.

    void parser::set_description(std::string)

Allows user to set the description of the program, used in the help message.

    void parser::parse_args(int argc, char** argv)

This doeos the actual parsing. NOTE: you must call this function before using any of the get_[ACTION]() functions to get actual output. argc and argv are the same that are used for the main function when accessing command line arguments.

    
Example Code:

    #include <string>
    #include <iostream>
    #include <argparser/argparser.h>


    int main(int argc, char **argv) 
    {
        argument a("FIRST ARGUMENET");
        a.set_flags("-a", "--apple");
        a.set_help_message("generate full output");
        a.set_action(STORE);

        argument b("SECOND ARGUMENT");
        b.set_flags("-b", "--book");
        b.set_action(STORE_TRUE);

        argument c("THIRD ARGUMENT");
        c.set_flags("-c", "--cutlet");
        c.set_action(STORE_FALSE);

        argument d("FOURTH ARGUMENT");
        d.set_flags("-d", "--dracula");
        d.set_action(COUNT);
        d.set_requirement(true);

        argument e("FIFTH ARGUMENT");
        e.set_flags("-e", "--epsilon");
        e.set_action(APPEND);
        e.set_nargs(3);


        parser p;
        p.add_arguments(&a,&b,&c,&d, &e);
        p.set_description("this program does many many important things");
        p.parse_args(argc, argv);

        if (!a.is_empty)
            std::cout << "first argument value: " << a.get_store() << std::endl
        else
            std::cout << "first argument is empty" << std::endl;    

        if (b.get_store_tf())
            std::cout << "second argument was specified" << std::endl; 
        else
            std::cout << "second argument was not specified" << std::endl;
        
        if (d.get_count() > 0)
            std::cout << "fourth argument was specified at least once" << std::endl;
        else
        {
            std::cout << "fourth argument was not specified << std::endl;
            std::cout << but this is not possible since it is required << std::endl;
        }

        if (!e.is_empty())
        {
            std::cout << "fifth argument has values: " << std::endl;
            for (std::string s : e.get_append())
            {
                std::cout << s << std::endl;
            }
        }



    }






