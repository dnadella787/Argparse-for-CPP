#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h" 

class storeTFDeathTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        a.arg_name = "STORET_ARG";
        a.set_flags("-t", "--true");
        a.set_help_message("check truthness");
        a.set_action(STORE_TRUE);

        b.arg_name = "STOREF_ARG";
        b.set_flags("-f", "--false");
        b.set_help_message("check falseness");
        b.set_action(STORE_FALSE);
        
        p.add_arguments(&a, &b);
        p.set_prog_name("argparse_test4");
        p.set_description("fourth argparse test in suite");
    }

    argument a;
    argument b;
    parser p;
};



/*
check that help message is triggered and appropriate 
help message is actualy outputted, must include 
functionality unique to action APPEND
*/
TEST_F(storeTFDeathTest, h_message_works1)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"-h" };
    int argc_ = 2;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();


    EXPECT_NE(output.find("-t"), std::string::npos);
    EXPECT_NE(output.find("--true"), std::string::npos);

    EXPECT_NE(output.find("-f"), std::string::npos);
    EXPECT_NE(output.find("--false"), std::string::npos);
    
    EXPECT_NE(output.find("check falseness"), std::string::npos);
    EXPECT_NE(output.find("check truthness"), std::string::npos);

    EXPECT_NE(output.find("description"), std::string::npos);
    EXPECT_NE(output.find("argparse_test4"), std::string::npos);
    EXPECT_NE(output.find("fourth argparse test in suite"), std::string::npos);
    
    EXPECT_EQ(output.find("[inputk]"), std::string::npos);
    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}

TEST_F(storeTFDeathTest, h_message_works2)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"-f", (char*)"two", (char*)"one", (char*)"--help" };
    int argc_ = 6;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}




/*
If there is too much input then that means the 
prgram should EXIT_FAILURE and output to stderr
that there were too many inputs. Check for both flags
and both arguments
*/
TEST_F(storeTFDeathTest, too_many_inputs1)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"-f", (char*)"one", (char*)"two" }; 
    int argc_ = 4;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected");
}

TEST_F(storeTFDeathTest, too_many_inputs2)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--false", (char*)"one", (char*)"two", (char*)"three" };
    int argc_ = 5;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected");
}

TEST_F(storeTFDeathTest, too_many_inputs3)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"-t", (char*)"one", (char*)"two" }; 
    int argc_ = 4;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected");
}

TEST_F(storeTFDeathTest, too_many_inputs4)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--true", (char*)"one", (char*)"two", (char*)"three" };
    int argc_ = 5;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected");
}