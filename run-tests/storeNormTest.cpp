#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"

class storeNormTest : public ::testing::Test 
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
just check to make sure that the set up was done
in the SetUp() function actually worked properly.
*/
TEST_F(storeNormTest, proper_setup_works)
{
    ASSERT_EQ(1, p.known_arguments.size()) << "STORE argument could not be added to parser" << std::endl;

    ASSERT_EQ(2, a.accepted_flags.size());
    EXPECT_EQ("-a", a.accepted_flags[0]);
    EXPECT_EQ("--apple", a.accepted_flags[1]);
    EXPECT_EQ(STORE, a.action);
    EXPECT_EQ("decides if apple or not", a.help_message);
    EXPECT_FALSE(a.is_required);

    a.set_requirement(true);
    EXPECT_EQ(true, a.is_required);
}


/*
checks that when no input is passed in or when
= is used without anything after or another flag after
it should be empty and get_store should return
NO_INPUT
*/
TEST_F(storeNormTest, no_input1)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[3] = { (char*)"prog_name", (char*)"first", (char*)"second" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_TRUE(a.is_empty());
    EXPECT_EQ(NO_INPUT, a.get_store());
}



/*
input passed in properly after = this time
*/
TEST_F(storeNormTest, input1)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[2] = { (char*)"prog_name", (char*)"-a=first" };
    int argc_ = 2;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", a.get_store());
}

TEST_F(storeNormTest, input2)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[3] = { (char*)"prog_name", (char*)"-a", (char*)"first" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", a.get_store());
}

TEST_F(storeNormTest, input3)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[3] = { (char*)"prog_name", (char*)"--apple", (char*)"first" };
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", a.get_store());
}
/*
make sure that passing in flag again overwrites previous value
*/
TEST_F(storeNormTest, input4)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[3] = { (char*)"prog_name", (char*)"-a=first", (char*)"-a=second"};
    int argc_ = 3;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("second", a.get_store());
}

TEST_F(storeNormTest, input5)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[5] = { (char*)"prog_name", (char*)"-a", (char*)"first", (char*)"-a", (char*)"second"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("second", a.get_store());
}

TEST_F(storeNormTest, input6)
{
    ASSERT_FALSE(a.is_required);
    ASSERT_EQ(STORE, a.action);

    char* argv_[5] = { (char*)"prog_name", (char*)"--apple", (char*)"first", (char*)"--apple", (char*)"second"};
    int argc_ = 5;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("second", a.get_store());
}



