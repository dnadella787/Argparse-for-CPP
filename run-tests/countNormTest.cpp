#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h" 

class countNormTest : public ::testing::Test 
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
make sure that the proper setup for a count argumenet 
actually works as intended
*/
TEST_F(countNormTest, proper_setup_works)
{
    ASSERT_EQ(1, p.known_arguments.size()) << "COUNT argument could not be added to parser" << std::endl;

    ASSERT_EQ(2, a.accepted_flags.size());
    EXPECT_EQ("-D", a.accepted_flags[0]);
    EXPECT_EQ("--Dracula", a.accepted_flags[1]);
    EXPECT_EQ(COUNT, a.action);
    EXPECT_EQ("denotes the dracula's count", a.help_message);
    EXPECT_FALSE(a.is_required);
    EXPECT_EQ(0, a.n_args);

    a.set_requirement(true);
    EXPECT_TRUE(a.is_required);
}


/*
as long as argument is not required, then if the flag
is never specified, get_count() should return a 0
*/
TEST_F(countNormTest, no_input)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"zeroeth", (char*)"first", (char*)"second", (char*)"third"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    EXPECT_EQ(0, a.get_count());
}



/*
if either of the flag is specified in any capacity then 
get_count() should return the proper values
*/
TEST_F(countNormTest, input1)
{
    char* argv_[5] = { (char*)"prog_name", (char*)"-D", (char*)"--Dracula", (char*)"-D", (char*)"-D"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    ASSERT_EQ(4, a.get_count());
}

TEST_F(countNormTest, input2)
{
    char* argv_[3] = { (char*)"prog_name", (char*)"-D", (char*)"--Dracula" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    ASSERT_EQ(2, a.get_count());
}

TEST_F(countNormTest, input3)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"nguyen ngo", (char*)"eyes", (char*)"--Dracula" };
    int argc_ = 4;

    p.parse_args(argc_, argv_);

    ASSERT_EQ(1, a.get_count());
}



