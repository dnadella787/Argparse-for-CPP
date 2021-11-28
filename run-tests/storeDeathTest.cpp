#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"


/*
TEST CASE/SUITE: Death Tests for STORE action

This file is filled with action STORE tests where 
the test is meant to cause an exit of some kind 
whether it be for the help message (EXIT_SUCCESS) 
or an actual error (EXIT_FAILURE).

each test uses the objects defined in the class
storeDeathTest and have the starting behavior 
according to the SetUp() function.
*/
class storeDeathTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        a.arg_name = "STORE_ARG";
        a.set_flags("-a", "--apple");
        a.set_help_message("decides if apple or not");
        a.set_action(STORE);
        

        p.add_arguments(&a);
        p.set_prog_name("argparse_test1");
        p.set_description("first argparse test in suite");
    }

    argument a;
    parser p;
};




/*
simple test, just checks to see if help message 
is actually outputted

all h[elp]_message_works[x] tests establish that help
message takes priority over any error that would
cause an exit

LOOK FOR BETTER SOLUTION THAN CaptureStdout for
all help_message_worksx tests
*/
TEST_F(storeDeathTest, h_message_works1)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"-h" };
    int argc_ = 2;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("-a"), std::string::npos);
    EXPECT_NE(output.find("--apple"), std::string::npos);
    EXPECT_NE(output.find("decides if apple or not"), std::string::npos);
    EXPECT_NE(output.find("description"), std::string::npos);
    EXPECT_NE(output.find("argparse_test1"), std::string::npos);
    EXPECT_NE(output.find("first argparse test in suite"), std::string::npos);
    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}

TEST_F(storeDeathTest, help_message_works1)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"--help" };
    int argc_ = 2;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("-a"), std::string::npos);
    EXPECT_NE(output.find("--apple"), std::string::npos);
    EXPECT_NE(output.find("decides if apple or not"), std::string::npos);
    EXPECT_NE(output.find("description"), std::string::npos);
    EXPECT_NE(output.find("argparse_test1"), std::string::npos);
    EXPECT_NE(output.find("first argparse test in suite"), std::string::npos);
    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}




/*
makes sure that help message is outputted even 
though multiple valid and invalid inputs are 
specified.
*/
TEST_F(storeDeathTest, h_message_works2)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"-a", (char*)"-grskd", (char*)"-dfks", (char*)"-h" };
    int argc_ = 5;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}

TEST_F(storeDeathTest, help_message_works2)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--apple", (char*)"-grskd", (char*)"-dfks", (char*)"--help" };
    int argc_ = 5;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}




/*
makes sure that program ends if a required input
is not specified, in this case the -a, --apple.
Should EXIT_FAILURE and output to stderr, check both.
*/
TEST_F(storeDeathTest, requirement_works)
{
    a.set_requirement(true);
    ASSERT_TRUE(a.is_required);

    char* argv_[3] = { (char*)"prog_name" , (char*)"nonsense", (char*)"nonsense" };
    int argc_ = 3;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "required STORE argument");
}




/*
make sure that the program ends if too many inputs 
are given for -a or --apple. Should EXIT_FAILURE and 
output to stderr, check both.
*/
TEST_F(storeDeathTest, too_many_inputs1)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[4] = { (char*)"prog_name", (char*)"-a", (char*)"first", (char*)"second" };
    int argc_ = 4;
    
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}

TEST_F(storeDeathTest, too_many_inputs2)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[4] = { (char*)"prog_name", (char*)"--apple", (char*)"first", (char*)"second" };
    int argc_ = 4;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}




/*
same but check for = input flag
*/
TEST_F(storeDeathTest, too_many_inputs3)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[3] = { (char*)"prog_name", (char*)"-a=first", (char*)"second" };
    int argc_ = 3;
    
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}

TEST_F(storeDeathTest, too_many_inputs4)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[3] = { (char*)"prog_name", (char*)"--apple=first", (char*)"second" };
    int argc_ = 3;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}




/*
same but shifted one back
*/
TEST_F(storeDeathTest, too_many_inputs5)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[5] = { (char*)"prog_name", (char*)"first", (char*)"-a", (char*)"second", (char*)"third" };
    int argc_ = 5;
    
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}

TEST_F(storeDeathTest, too_many_inputs6)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[5] = { (char*)"prog_name", (char*)"first", (char*)"--apple", (char*)"second", (char*)"third" };
    int argc_ = 5;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many input");
}

TEST_F(storeDeathTest, too_many_inputs7)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[4] = { (char*)"prog_name", (char*)"first", (char*)"-a=second", (char*)"third" };
    int argc_ = 4;
    
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}

TEST_F(storeDeathTest, too_many_inputs8)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[4] = { (char*)"prog_name", (char*)"first", (char*)"--apple=second", (char*)"third" };
    int argc_ = 4;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many input");
}




/*
same but shifted one back and also has unrecognized arguments
but the unrecognized argument should be first error not
too many inputs
*/
TEST_F(storeDeathTest, false_arg_first1)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[5] = { (char*)"prog_name", (char*)"-first", (char*)"-a", (char*)"second", (char*)"third" };
    int argc_ = 5;
    
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "-first is not a recognized");
}

TEST_F(storeDeathTest, false_arg_first2)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[5] = { (char*)"prog_name", (char*)"-first", (char*)"--apple", (char*)"second", (char*)"third" };
    int argc_ = 5;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "-first is not a recognized");
}

TEST_F(storeDeathTest, false_arg_first3)
{
    ASSERT_EQ(STORE, a.action);
    char* argv_[5] = { (char*)"prog_name", (char*)"-first=", (char*)"--apple", (char*)"second", (char*)"third" };
    int argc_ = 5;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "-first= is not a recognized");
}




/*
makes sure help flag with = is not recognized
*/
TEST_F(storeDeathTest, h_equals1)
{
    char* argv_help2[2] = { (char*)"prog_name", (char*)"-h=" };
    int argc_help2 = 2;

    EXPECT_EXIT(p.parse_args(argc_help2, argv_help2), testing::ExitedWithCode(EXIT_FAILURE), "not a recognized flag");
}

TEST_F(storeDeathTest, help_equal1)
{
    char* argv_help2[2] = { (char*)"prog_name", (char*)"--help=" };
    int argc_help2 = 2;

    EXPECT_EXIT(p.parse_args(argc_help2, argv_help2), testing::ExitedWithCode(EXIT_FAILURE), "not a recognized flag");
}

TEST_F(storeDeathTest, h_equals2)
{
    char* argv_help2[2] = { (char*)"prog_name", (char*)"-h=fd" };
    int argc_help2 = 2;

    EXPECT_EXIT(p.parse_args(argc_help2, argv_help2), testing::ExitedWithCode(EXIT_FAILURE), "not a recognized flag");
}

TEST_F(storeDeathTest, help_equals2)
{
    char* argv_help2[2] = { (char*)"prog_name", (char*)"--help=fd" };
    int argc_help2 = 2;

    EXPECT_EXIT(p.parse_args(argc_help2, argv_help2), testing::ExitedWithCode(EXIT_FAILURE), "not a recognized flag");
}



/*
Make sure that you cannot call the get_[action]() 
method other than get_store().
*/
TEST_F(storeDeathTest, illegal_get_append1)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"-a", (char*)"input" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_EXIT(a.get_append(), testing::ExitedWithCode(EXIT_FAILURE), "does not have action APPEND");
}

TEST_F(storeDeathTest, illegal_get_append2)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"--apple", (char*)"input" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_EXIT(a.get_append(), testing::ExitedWithCode(EXIT_FAILURE), "does not have action APPEND");
}

TEST_F(storeDeathTest, illegal_get_count1)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"-a", (char*)"input" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_EXIT(a.get_count(), testing::ExitedWithCode(EXIT_FAILURE), "does not have COUNT action");
}

TEST_F(storeDeathTest, illegal_get_count2)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"--apple", (char*)"input" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_EXIT(a.get_count(), testing::ExitedWithCode(EXIT_FAILURE), "does not have COUNT action");
}

TEST_F(storeDeathTest, illegal_get_store_tf1)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"--apple", (char*)"input" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_EXIT(a.get_store_tf(), testing::ExitedWithCode(EXIT_FAILURE), "does not have action STORE_FALSE");
}

TEST_F(storeDeathTest, illegal_get_store_tf2)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"-a", (char*)"input" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_EXIT(a.get_store_tf(), testing::ExitedWithCode(EXIT_FAILURE), "does not have action STORE_FALSE");
}



/*
check that when = is used without output after
program stops executing.
*/
TEST_F(storeDeathTest, input_equal_failure)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[3] = { (char*)"prog_name", (char*)"-a=", (char*)"first" };
    int argc_ = 3;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input after =");
}

TEST_F(storeDeathTest, no_input1)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[2] = { (char*)"prog_name", (char*)"-a=" };
    int argc_ = 2;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input after =");
}

TEST_F(storeDeathTest, no_input2)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[2] = { (char*)"prog_name", (char*)"--apple=" };
    int argc_ = 2;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input after =");
}




