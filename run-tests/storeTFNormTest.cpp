#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"

class storeTFNormTest : public ::testing::Test 
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
make sure that the setup for the true STORE_FALSE
and STORE_TRUE arguments a and b works properly
by checking all their member vars
*/
TEST_F(storeTFNormTest, proper_setup_works)
{
    ASSERT_EQ(2, p.known_arguments.size()) << "STORE_TRUE and STORE_FALSE arguments could not be added to parser" << std::endl;

    ASSERT_EQ(2, a.accepted_flags.size());
    EXPECT_EQ("-t", a.accepted_flags[0]);
    EXPECT_EQ("--true", a.accepted_flags[1]);
    EXPECT_EQ(STORE_TRUE, a.action);
    EXPECT_EQ("check truthness", a.help_message);
    EXPECT_FALSE(a.is_required);
    EXPECT_EQ(0, a.n_args);


    ASSERT_EQ(2, b.accepted_flags.size());
    EXPECT_EQ("-f", b.accepted_flags[0]);
    EXPECT_EQ("--false", b.accepted_flags[1]);
    EXPECT_EQ(STORE_FALSE, b.action);
    EXPECT_EQ("check falseness", b.help_message);
    EXPECT_FALSE(b.is_required);
    EXPECT_EQ(0, b.n_args);
}




/*
make sure that when the flags are not specified,
they default to opposite of what they store
*/
TEST_F(storeTFNormTest, no_input)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"zeroeth", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.get_store_tf());
    EXPECT_TRUE(b.get_store_tf());
}



/*
make sure that making the argument required has no 
effect on how argument is parsed. It doesn't make sense
to have a required STORE_TRUE or STORE_FALSE argument
*/
TEST_F(storeTFNormTest, requirement_no_effect)
{
    a.set_requirement(true);
    b.set_requirement(false);
    char* argv_[5] = { (char*)"prog_name", (char*)"zeroeth", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.get_store_tf());
    EXPECT_TRUE(b.get_store_tf());
}



/*
make sure that the input is stored properly in each
case
*/
TEST_F(storeTFNormTest, input1)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"-t" };
    int argc_ = 2;

    p.parse_args(argc_, argv_);

    EXPECT_TRUE(a.get_store_tf());
    EXPECT_TRUE(b.get_store_tf());
}

TEST_F(storeTFNormTest, input2)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"--true" };
    int argc_ = 2;

    p.parse_args(argc_, argv_);

    EXPECT_TRUE(a.get_store_tf());
    EXPECT_TRUE(b.get_store_tf());
}

TEST_F(storeTFNormTest, input3)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"-f" };
    int argc_ = 2;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.get_store_tf());
    EXPECT_FALSE(b.get_store_tf());
}

TEST_F(storeTFNormTest, input4)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"--false" };
    int argc_ = 2;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.get_store_tf());
    EXPECT_FALSE(b.get_store_tf());
}

TEST_F(storeTFNormTest, input5)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"-t", (char*)"--false" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_TRUE(a.get_store_tf());
    EXPECT_FALSE(b.get_store_tf());
}