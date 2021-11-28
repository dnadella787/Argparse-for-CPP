#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"

class mixedDeathTest1 : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        a.arg_name = "STORE_ARG";
        a.set_flags("-a", "--apple");
        a.set_help_message("decides if apple or not");
        a.set_requirement(true);
        a.set_action(STORE);
        
        b.arg_name = "APPEND_ARG";
        b.set_flags("-v", "--verbose");
        b.set_help_message("decides output style");
        b.set_action(APPEND);
        b.set_nargs(2);

        c.arg_name = "STORE_ARG";
        c.set_flags("-D", "--Dracula");
        c.set_help_message("count number of Draculas");
        c.set_action(COUNT);
        
        d.arg_name = "STORET_ARG";
        d.set_flags("-t", "--truth");
        d.set_help_message("check truthness");
        d.set_action(STORE_TRUE);

        e.arg_name = "STOREF_ARG";
        e.set_flags("-f", "--false");
        e.set_help_message("check falseness");
        e.set_action(STORE_FALSE);

        p.add_arguments(&a, &b, &c, &d, &e);
        p.set_prog_name("argparse_test5");
        p.set_description("fifth argparse test in suite");
    }

    argument a, b, c, d, e;
    parser p;
};


/*
check that the help message outputs all of the 
usage information relavent to each message
*/
TEST_F(mixedDeathTest1, h_message_works1)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"--apple=third", (char*)"--Dracula", (char*)"-h" };
    int argc_ = 4;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("-a, --apple"), std::string::npos);
    EXPECT_NE(output.find("-v, --verbose"), std::string::npos);
    EXPECT_NE(output.find("-D, --Dracula"), std::string::npos);
    EXPECT_NE(output.find("-t, --truth"), std::string::npos);
    EXPECT_NE(output.find("-f, --false"), std::string::npos);

    EXPECT_NE(output.find("decides if apple or not"), std::string::npos);
    EXPECT_NE(output.find("decides output style"), std::string::npos);
    EXPECT_NE(output.find("count number of Draculas"), std::string::npos);
    EXPECT_NE(output.find("check truthness"), std::string::npos);
    EXPECT_NE(output.find("check falseness"), std::string::npos);

    EXPECT_NE(output.find("description"), std::string::npos);
    EXPECT_NE(output.find("argparse_test5"), std::string::npos);
    EXPECT_NE(output.find("fifth argparse test in suite"), std::string::npos);

    EXPECT_NE(output.find("[input]"), std::string::npos);
    EXPECT_NE(output.find("[inputk]"), std::string::npos);

    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}

/*
error comes first but help message should 
be outputted first and should still
EXIT_SUCCESS rather than EXIT_FAILURE
*/
TEST_F(mixedDeathTest1, h_message_works2)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"--apple=third", (char*)"--Dracula=", (char*)"-h" };
    int argc_ = 4;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}



/*
check that requirement works
*/
TEST_F(mixedDeathTest1, required)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--verbose", (char*)"eskee", (char*)"Jayce", (char*)"-D" };
    int argc_ = 5;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "required STORE argument");
}


/*
check that too many inputs for the STORE and
APPEND argument properly fails with EXIT_FAILURE
and outputs the relavent flag with too many inputs
error.
*/
TEST_F(mixedDeathTest1, too_many_inputs1)
{
    char* argv_[8] = { (char*)"prog_name", (char*)"--truth", (char*)"--verbose", (char*)"eskee", (char*)"Jayce", (char*)"eyes", (char*)"-a=ees", (char*)"bruins" };
    int argc_ = 8;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs for --verbose");
}

TEST_F(mixedDeathTest1, too_many_inputs2)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"-a=second", (char*)"eskee", (char*)"--false", (char*)"--Dracula" };
    int argc_ = 5;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs for -a");
}

TEST_F(mixedDeathTest1, too_many_inputs3)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--Dracula", (char*)"errors", (char*)"--false", (char*)"--Dracula" };
    int argc_ = 5;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected for --Dracula");
}

TEST_F(mixedDeathTest1, too_many_inputs4)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--Dracula", (char*)"-t", (char*)"--false", (char*)"another_error" };
    int argc_ = 5;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected for --false");
}


/*
invalid flag amongst all other correct flags 
should output a relevant error message and 
exit EXIT_FAILURE
*/
TEST_F(mixedDeathTest1, invalid_flag)
{
    char* argv_[8] = { (char*)"prog_name", (char*)"--truth", (char*)"--verbose", (char*)"eskee", (char*)"Jayce", (char*)"--invalid", (char*)"-a=ees", (char*)"--Dracula" };
    int argc_ = 8;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "--invalid is not a recognized argument");
}