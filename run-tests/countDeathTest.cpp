#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"


class countDeathTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        a.arg_name = "COUNT_ARG";
        a.set_flags("-D", "--Dracula");
        a.set_help_message("denotes the dracula's count");
        a.set_action(COUNT);
        

        p.add_arguments(&a);
        p.set_prog_name("argparse_test3");
        p.set_description("third argparse test in suite");
    }

    argument a;
    parser p;
};


/*
check that the help message outputs all correct 
information pertinent to action COUNT
*/
TEST_F(countDeathTest, h_message_works1)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"-h" };
    int argc_ = 2;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("-D"), std::string::npos);
    EXPECT_NE(output.find("--Dracula"), std::string::npos);
    EXPECT_NE(output.find("denotes the dracula's count"), std::string::npos);
    EXPECT_NE(output.find("description"), std::string::npos);
    EXPECT_NE(output.find("argparse_test3"), std::string::npos);
    EXPECT_NE(output.find("third argparse test in suite"), std::string::npos);
    EXPECT_EQ(output.find("("), std::string::npos);
    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}

/*
check that the help message is outputted despite
the errors such as invalid flag and an argument
passed in to an argument of type COUNT
*/
TEST_F(countDeathTest, h_message_works2)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"--verbose", (char*)"-D", (char*)"one", (char*)"--help" };
    int argc_ = 6;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}


/*
check that requirement works, should output an
error asking that the flag be specified
*/
TEST_F(countDeathTest, requirement_works)
{
    a.set_requirement(true);
    ASSERT_TRUE(a.is_required);
    char* argv_[3] = { (char*)"prog_name", (char*)"two", (char*)"one" };
    int argc_ = 3;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "required COUNT action argument");
}



/*
check that if you add in values after the COUNT 
argument flag, then it should produce an error 
*/
TEST_F(countDeathTest, too_many_inputs1)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"-D", (char*)"one" };
    int argc_ = 3;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected for -D");
}

TEST_F(countDeathTest, too_many_inputs2)
{
    char* argv_[4] = { (char*)"prog_name", (char *)"eyois", (char*)"--Dracula", (char*)"one" };
    int argc_ = 4;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "no input expected for --Dracula");
}


