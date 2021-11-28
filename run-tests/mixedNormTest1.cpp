#include <gtest/gtest.h>
#include "argparser/argparser-test-version.h"

class mixedNormTest1 : public ::testing::Test 
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
just check that for each input the expected output
occurs by checking values of every argument variable
*/
TEST_F(mixedNormTest1, input1)
{
    char* argv_[2] = { (char*)"prog_name", (char*)"-a=first" };
    int argc_ = 2;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", a.get_store());

    EXPECT_TRUE(b.is_empty());
    EXPECT_EQ(0, b.get_append().size());

    EXPECT_EQ(0, c.get_count());

    EXPECT_FALSE(d.get_store_tf());

    EXPECT_TRUE(e.get_store_tf());
}


TEST_F(mixedNormTest1, input2)
{
    char* argv_[4] = { (char*)"prog_name", (char*)"-a", (char*)"first", (char*)"-D"};
    int argc_ = 4;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", a.get_store());

    EXPECT_TRUE(b.is_empty());
    EXPECT_EQ(0, b.get_append().size());

    EXPECT_EQ(1, c.get_count());

    EXPECT_FALSE(d.get_store_tf());

    EXPECT_TRUE(e.get_store_tf());
}


TEST_F(mixedNormTest1, input3)
{
    char* argv_[7] = { (char*)"prog_name", (char*)"-a", (char*)"first", (char*)"-D", (char*)"-v", (char*)"second", (char*)"third"};
    int argc_ = 7;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ("first", a.get_store());

    EXPECT_FALSE(b.is_empty());
    EXPECT_EQ(2, b.get_append().size());
    EXPECT_EQ("second", b.get_append()[0]);
    EXPECT_EQ("third", b.get_append()[1]);

    EXPECT_EQ(1, c.get_count());

    EXPECT_FALSE(d.get_store_tf());

    EXPECT_TRUE(e.get_store_tf());
}

TEST_F(mixedNormTest1, input4)
{
    char* argv_[6] = { (char*)"prog_name", (char*)"--Dracula", (char*)"--false", (char*)"-f", (char*)"--apple", (char*)"f%%dsklfk^xl&"};
    int argc_ = 6;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ(a.get_store(), "f%%dsklfk^xl&");

    EXPECT_TRUE(b.is_empty());

    EXPECT_EQ(1, c.get_count());

    EXPECT_FALSE(d.get_store_tf());

    EXPECT_FALSE(e.get_store_tf());
}


TEST_F(mixedNormTest1, input5)
{
    char* argv_[7] = { (char*)"prog_name", (char*)"--Dracula", (char*)"--truth", (char*)"-f", (char*)"-D", (char*)"--apple", (char*)"first"};
    int argc_ = 7;

    p.parse_args(argc_, argv_);

    EXPECT_FALSE(a.is_empty());
    EXPECT_EQ(a.get_store(), "first");

    EXPECT_TRUE(b.is_empty());

    EXPECT_EQ(2, c.get_count());

    EXPECT_TRUE(d.get_store_tf());

    EXPECT_FALSE(e.get_store_tf());
}


TEST_F(mixedNormTest1, input6)
{
    a.set_requirement(false);
    char* argv_[7] = { (char*)"prog_name", (char*)"--false", (char*)"--verbose", (char*)"second", (char*)"third", (char*)"--truth", (char*)"-t"};
    int argc_ = 7;

    p.parse_args(argc_, argv_);

    ASSERT_FALSE(a.is_required);
    EXPECT_TRUE(a.is_empty());

    EXPECT_FALSE(b.is_empty());
    EXPECT_EQ(2, b.get_append().size());
    EXPECT_EQ("second", b.get_append()[0]);
    EXPECT_EQ("third", b.get_append()[1]);

    EXPECT_EQ(0, c.get_count());

    EXPECT_TRUE(d.get_store_tf());

    EXPECT_FALSE(e.get_store_tf());
}

