
#include <gtest/gtest.h>

#include <StrUtil.h>

using namespace strutil;

TEST(StringUtil, Replace)
{
    std::string str = "this is a test";
    replace_all(str, " ", "");
    ASSERT_EQ("thisisatest", str);
}

TEST(StringUtil, Str)
{
    ASSERT_EQ("a", str("a"));
    ASSERT_EQ("1", str(1));
    ASSERT_EQ("1.5", str(1.5));
    ASSERT_EQ("a1b", str("a", 1, "b"));
}

TEST(StringUtil, Format)
{
    ASSERT_EQ("a", format("a"));
    ASSERT_EQ("a 1", format("{0} {1}", "a", 1));
    ASSERT_EQ("a 1", format("{value} {number}", _p("value", "a"), _p("number", 1)));
}
