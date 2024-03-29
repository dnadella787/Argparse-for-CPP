An argparse implementation written in C++. This version is meant to use more object oriented design to do the parsing.


There is an argument class that is similar to the python add_argument method except that each argument must be initialized as an argument object first and then added to the parser object. 

To install, simply run:

    sudo make install 

Namespaces: 

Both the argument and the parser functions are in the argparser namespace but all of the actions (COUNT, STORE, etc.) are in the ACTION namespace. Example below will exemplify this.

Argument Class Functions:

    argparser::argument<ACTION>(std::string)

initialize the argument object with a name as the parameter. The name is useful for the help message and error outputs but is not necessary.

    void set_flags(std::string ...)

Add all allowable command line flags to the argument class. You can add in as many at one time as needed.

    void argument::set_nargs(int)

Set the number of additional arguments that should be included after any of the allowable flags. Note that n_args is set to 1 by default, to change this you need to use this function or set_action will also automatically change it. Compiler will only allow you to set nargs for the action APPEND

    void argument::set_requirement(bool)

Specified whether the argument must be specified on command line. The requirement is false by default but you can change it by setting it to set_requirement(true). Compiler

    std::string argument::get<T>()

Gives you the relavent output based on the initial action type specified. So if its ACTION::COUNT then get<int>() will return an integer of the count value. If it was ACTION::STORE get<std::string>() will return a string of the input for that flag. Note that for ACTION::APPEND the get function call must be of the form get<std::vector<T>>(). Not all types are allowed but for the ones that are not allowed the compiler will throw you warnings. 

    bool argument::is_empty()

Used with the action STORE and APPEND. If no input is specified in either case the function returns true, else false.


Possible ACTIONs:

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

    void parser::add_arguments(argument...)

Allows you to add all arguments to the parser object with as many arguments as possible. NOTE: they must be pointers to arguments, not the actual argument.

    void parser::set_prog_name(std::string)

Allows user to set the program name used for the help message.

    void parser::set_description(std::string)

Allows user to set the description of the program, used in the help message.

    void parser::parse_args(int argc, char** argv)

This doeos the actual parsing. NOTE: you must call this function before using any of the get<T>() functions to get actual output. argc and argv are the same that are used for the main function when accessing command line arguments.

    
Example Code:

    #include <string>
    #include <iostream>
    #include <argparser/argparser.h>


    int main(int argc, char **argv) 
    {
        argparser::argument<ACTION::STORE> a("FIRST ARGUMENET");
        a.set_flags("-a", "--apple");
        a.set_help_message("generate full output");

        argparser::argument<ACTION::STORE_TRUE> b("SECOND ARGUMENT");
        b.set_flags("-b", "--book");

        argparser::argument<ACTION::STORE_FALSE> c("THIRD ARGUMENT");
        c.set_flags("-c", "--cutlet");

        argparser::argument<ACTION::COUNT> d("FOURTH ARGUMENT");
        d.set_flags("-d", "--dracula");
        d.set_requirement(true);

        argparser::argument<ACTION::APPEND> e("FIFTH ARGUMENT");
        e.set_flags("-e", "--epsilon");
        e.set_nargs(3);

        parser p;
        p.add_arguments(a,b,c,d,e);
        p.set_description("this program does many many important things");
        p.parse_args(argc, argv);

        if (!a.is_empty)
            std::cout << "first argument value: " << a.get_store() << std::endl
        else
            std::cout << "first argument is empty" << std::endl;    

        if (b.get<bool>())
            std::cout << "second argument was specified" << std::endl; 
        else
            std::cout << "second argument was not specified" << std::endl;
        
        if (d.get<double>() > 0)
            std::cout << "fourth argument was specified at least once" << std::endl;
        else
        {
            std::cout << "fourth argument was not specified << std::endl;
            std::cout << but this is not possible since it is required << std::endl;
        }

        // d.get<char>() and d.get<std::string>() are also supported for COUNT

        if (!e.is_empty())
        {
            std::cout << "fifth argument has values: " << std::endl;
            for (std::string s : e.get<std::vector<std::string>>())
            {
                std::cout << s << std::endl;
            }
        }

        //e.get<std::string>() <-- this will throw a compiler warning since e is APPEND but no vector is specified



    }






