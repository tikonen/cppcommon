
#include <CppCommonConfig.h>

#include <CSVReader.h>
#include <gtest/gtest.h>

using namespace CSV;

TEST(CSVReader, Empty)
{
    char buffer[] = { 0 };
    // parse file, returns vector of rows
    std::vector<Row> result = Parse(buffer, strlen(buffer));

    ASSERT_EQ(0, result.size());
}

TEST(CSVReader, OneCell)
{
    char buffer[] = { "a" };
    // parse file, returns vector of rows
    std::vector<Row> result = Parse(buffer, strlen(buffer));

    ASSERT_EQ(1, result.size());
    auto& row = result[0];
    ASSERT_EQ(1, row.size());
    ASSERT_EQ("a", row[0]);
}

TEST(CSVReader, OneRow)
{
    char buffer[] = { "a,b,c\n\n" };
    // parse file, returns vector of rows
    std::vector<Row> result = Parse(buffer, strlen(buffer));

    ASSERT_EQ(1, result.size());
    auto& row = result[0];
    ASSERT_EQ(3, row.size());
    ASSERT_EQ("a", row[0]);
    ASSERT_EQ("b", row[1]);
    ASSERT_EQ("c", row[2]);
}

TEST(CSVReader, OneColumn)
{
    char buffer[] = { "a\nb\nc\n\n" };
    // parse file, returns vector of rows
    std::vector<Row> result = Parse(buffer, strlen(buffer));

    ASSERT_EQ(3, result.size());
    auto& row = result[0];
    ASSERT_EQ(1, row.size());
    ASSERT_EQ("a", row[0]);

    row = result[1];
    ASSERT_EQ(1, row.size());
    ASSERT_EQ("b", row[0]);

    row = result[2];
    ASSERT_EQ(1, row.size());
    ASSERT_EQ("c", row[0]);
}

TEST(CSVReader, Simple)
{
    char buffer[] = { "a,b\nc,d\n\n" };
    // parse file, returns vector of rows
    std::vector<Row> result = Parse(buffer, strlen(buffer));

    ASSERT_EQ(2, result.size());

    auto& row = result[0];
    ASSERT_EQ(2, row.size());
    ASSERT_EQ("a", row[0]);
    ASSERT_EQ("b", row[1]);

    row = result[1];
    ASSERT_EQ(2, row.size());
    ASSERT_EQ("c", row[0]);
    ASSERT_EQ("d", row[1]);
}

TEST(CSVReader, EmptyCells)
{
    char buffer[] = { "a,,b\nc,d,\n\n" };
    // parse file, returns vector of rows
    std::vector<Row> result = Parse(buffer, strlen(buffer));

    ASSERT_EQ(2, result.size());

    auto& row = result[0];
    ASSERT_EQ(3, row.size());
    ASSERT_EQ("a", row[0]);
    ASSERT_EQ("", row[1]);
    ASSERT_EQ("b", row[2]);

    row = result[1];
    ASSERT_EQ(3, row.size());
    ASSERT_EQ("c", row[0]);
    ASSERT_EQ("d", row[1]);
    ASSERT_EQ("", row[2]);
}
