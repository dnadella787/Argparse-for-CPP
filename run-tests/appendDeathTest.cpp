#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"

class appendDeathTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        a.arg_name = "APPEND_ARG";
        a.set_flags("-v", "--verbose");
        a.set_help_message("decides output style");
        a.set_nargs(3);
        a.set_action(APPEND);
        
        p.add_arguments(&a);
        p.set_prog_name("argparse_test2");
        p.set_description("second argparse test in suite");
    }

    argument a;
    parser p;
};



/*
check that help message is triggered and appropriate 
help message is actualy outputted, must include 
functionality unique to action APPEND
*/
TEST_F(appendDeathTest, h_message_works1)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"-h" };
    int argc_ = 2;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("-v"), std::string::npos);
    EXPECT_NE(output.find("--verbose"), std::string::npos);
    EXPECT_NE(output.find("decides output style"), std::string::npos);
    EXPECT_NE(output.find("description"), std::string::npos);
    EXPECT_NE(output.find("argparse_test2"), std::string::npos);
    EXPECT_NE(output.find("second argparse test in suite"), std::string::npos);
    EXPECT_NE(output.find("[inputk]"), std::string::npos);
    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}

TEST_F(appendDeathTest, h_message_works2)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"--verbose", (char*)"two", (char*)"one", (char*)"--help" };
    int argc_ = 6;

    testing::internal::CaptureStdout();
    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_SUCCESS), "");
    std::cout.flush();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("generates this help/usage message"), std::string::npos);
}




/*
check that the set_requirement works and force an 
exit(EXIT_FAILURE)
*/
TEST_F(appendDeathTest, requirement_works)
{
    a.set_requirement(true);
    ASSERT_TRUE(a.is_required);
    char* argv_[3] = { (char*)"prog_name", (char*)"two", (char*)"one" };
    int argc_ = 3;

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "required APPEND argument");
}




/*
check what happens if there are too little inputs.
It should EXIT_FAILURE and output that there were not
enough inputs. Check for both of the flags -v and --verbose
and if you put input before the flag as well.
*/
TEST_F(appendDeathTest, too_little_input1)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"-v", (char*)"one" };
    int argc_ = 3;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "not enough inputs");
}

TEST_F(appendDeathTest, too_little_input2)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"-v", (char*)"one", (char*)"two" };
    int argc_ = 4;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "not enough inputs");
}

TEST_F(appendDeathTest, too_little_input3)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"--verbose", (char*)"one" };
    int argc_ = 3;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "not enough inputs");
}

TEST_F(appendDeathTest, too_little_input4)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"--verbose", (char*)"one", (char*)"two" };
    int argc_ = 4;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "not enough inputs");
}

TEST_F(appendDeathTest, too_little_input5)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"eyoyo", (char*)"-v", (char*)"one" };
    int argc_ = 3;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "not enough inputs");
}

TEST_F(appendDeathTest, too_little_input6)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"eyois", (char*)"--verbose", (char*)"one", (char*)"two" };
    int argc_ = 4;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "not enough inputs");
}




/*
If there is too much input then that means the 
prgram should EXIT_FAILURE and output to stderr
that there were too many inputs. Check for both flags
-v and --verbose
*/
TEST_F(appendDeathTest, too_many_inputs1)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"-v", (char*)"one", (char*)"two", (char*)"three", (char*)"four" };
    int argc_ = 6;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}

TEST_F(appendDeathTest, too_many_inputs2)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"--verbose", (char*)"one", (char*)"two", (char*)"three", (char*)"four" };
    int argc_ = 6;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "too many inputs");
}


/*
as it stands = cannot be used for anything but for the
action STORE. Check for both flags, -v and --verbose. Also
make sure that no matter if the right amount of inputs is 
used that it always outputs error. Lastly check error message 
for appropriate msg and it should EXIT_FAILURE. 
*/
TEST_F(appendDeathTest, illegal_equals1)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"--verbose=first", (char*)"one", (char*)"two" };
    int argc_ = 4;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "can't use '='");
}

TEST_F(appendDeathTest, illegal_equals2)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"-v=first", (char*)"one", (char*)"two", (char*)"three" };
    int argc_ = 5;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "can't use '='");
}

TEST_F(appendDeathTest, illegal_equals3)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"-v=", (char*)"one", (char*)"two", (char*)"three" };
    int argc_ = 5;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "can't use '='");
}

TEST_F(appendDeathTest, illegal_equals4)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--verbose=", (char*)"one", (char*)"two", (char*)"three" };
    int argc_ = 5;   

    EXPECT_EXIT(p.parse_args(argc_, argv_), testing::ExitedWithCode(EXIT_FAILURE), "can't use '='");
}


