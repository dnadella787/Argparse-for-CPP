#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"

class appendNormTest : public ::testing::Test 
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
just check to make sure that the set up was done
in the SetUp() function actually worked properly.
*/
TEST_F(appendNormTest, proper_setup_works)
{
    ASSERT_EQ(1, p.known_arguments.size()) << "APPEND argument could not be added to parser" << std::endl;

    ASSERT_EQ(2, a.accepted_flags.size());
    EXPECT_EQ("-v", a.accepted_flags[0]);
    EXPECT_EQ("--verbose", a.accepted_flags[1]);
    EXPECT_EQ(APPEND, a.action);
    EXPECT_EQ("decides output style", a.help_message);
    EXPECT_FALSE(a.is_required);
    EXPECT_EQ(3, a.n_args);

    a.set_requirement(true);
    EXPECT_TRUE(a.is_required);
}

/*
make sure that get_append() returns a vector of length 0
and make sure that a.is_empty() returns true as no
command line arguments should be captured by the argument
*/
TEST_F(appendNormTest, no_input)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"zeroeth", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    EXPECT_EQ(0, a.get_append().size());
    EXPECT_TRUE(a.is_empty());
}




/*
make sure that the only input is first, second and third 
and nothing else, make sure the outputted vector is appropriate 
size and the predicate, is_empty() should retur false here in 
both cases.
*/
TEST_F(appendNormTest, input1)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"-v", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    std::vector<std::string> output = a.get_append();
    ASSERT_EQ(3, output.size());
    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", output[0]);
    EXPECT_EQ("second", output[1]);
    EXPECT_EQ("third", output[2]);
}

TEST_F(appendNormTest, input2)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"--verbose", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    std::vector<std::string> output = a.get_append();
    ASSERT_EQ(3, output.size());
    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", output[0]);
    EXPECT_EQ("second", output[1]);
    EXPECT_EQ("third", output[2]);
}

TEST_F(appendNormTest, input3)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"nonsense", (char*)"-v", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 6;

    p.parse_args(argc_, argv_);

    std::vector<std::string> output = a.get_append();
    ASSERT_EQ(3, output.size());
    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", output[0]);
    EXPECT_EQ("second", output[1]);
    EXPECT_EQ("third", output[2]);
}

TEST_F(appendNormTest, input4)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"nonsense", (char*)"--verbose", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 6;

    p.parse_args(argc_, argv_);

    std::vector<std::string> output = a.get_append();
    ASSERT_EQ(3, output.size());
    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", output[0]);
    EXPECT_EQ("second", output[1]);
    EXPECT_EQ("third", output[2]);
}





