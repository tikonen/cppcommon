
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

TEST(StringUtil, Utf8)
{
    ASSERT_EQ("a", to_utf8(L"a"));
    ASSERT_EQ("\xc3\xa4", to_utf8(L"ä")); // unicode ä 0x00E4 -> utf8 ä 0xC3A4)
    ASSERT_EQ(L"a", to_utf16("a"));
    ASSERT_EQ(L"ä", to_utf16("\xc3\xa4")); // utf8 (ucs2) ä -> unicode ä
}