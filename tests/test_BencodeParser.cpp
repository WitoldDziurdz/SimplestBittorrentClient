#include <gtest/gtest.h>
#include "BencodeParser.h"
#include "header_libs/json.hpp"

// Tests for getString
TEST(BencodeParser, GetString_Valid) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getString("4:spam").first, nlohmann::json("spam"));
    EXPECT_EQ(parser.getString("12:Middle Earth").first, nlohmann::json("Middle Earth"));
    EXPECT_EQ(parser.getString("9:some data").first, nlohmann::json("some data"));
    EXPECT_EQ(parser.getString("9:some data111").first, nlohmann::json("some data"));
}

TEST(BencodeParser, GetString_Empty) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getString("0:").first, nlohmann::json(""));
}

TEST(BencodeParser, GetString_Invalid) {
    parser::BencodeParser parser;
    EXPECT_THROW(parser.getString("4spam"), std::invalid_argument);
    EXPECT_THROW(parser.getString(":spam"), std::invalid_argument);
    EXPECT_THROW(parser.getString("5:sp"), std::invalid_argument);
}

// Tests for getInteger
TEST(BencodeParser, GetInteger_Valid) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getInteger("i42e").first, nlohmann::json(42));
    EXPECT_EQ(parser.getInteger("i-42e").first, nlohmann::json(-42));
    EXPECT_EQ(parser.getInteger("i123456789e").first, nlohmann::json(123456789));
    EXPECT_EQ(parser.getInteger("i123e456").first, nlohmann::json(123));
}

TEST(BencodeParser, GetInteger_Invalid) {
    parser::BencodeParser parser;
    EXPECT_THROW(parser.getInteger("42"), std::invalid_argument);
    EXPECT_THROW(parser.getInteger("ie"), std::invalid_argument);
    EXPECT_THROW(parser.getInteger("i-42"), std::invalid_argument);
}

TEST(BencodeParser, GetInteger_Zero) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getInteger("i0e").first, nlohmann::json(0));
}

TEST(BencodeParser, GetInteger_LeadingZeros) {
    parser::BencodeParser parser;
    // Leading zeros are not allowed in Bencode integers, except for '0'.
    EXPECT_THROW(parser.getInteger("i0123e"), std::invalid_argument);
}

TEST(BencodeParserTest, GetListValid) {
    parser::BencodeParser parser;
    auto result = parser.getList("l4:spam4:eggse");
    EXPECT_EQ(result.first.size(), 2);
    EXPECT_EQ(result.first[0], "spam");
    EXPECT_EQ(result.first[1], "eggs");
    EXPECT_EQ(result.second, 13);
}

TEST(BencodeParserTest, GetListEmpty) {
    parser::BencodeParser parser;
    auto result = parser.getList("le");
    EXPECT_TRUE(result.first.empty());
    EXPECT_EQ(result.second, 2);
}

TEST(BencodeParserTest, GetDictValid) {
    parser::BencodeParser parser;
    auto result = parser.getDict("d3:cow3:moo4:spam4:eggse");
    EXPECT_EQ(result.first.size(), 2);
    EXPECT_EQ(result.first["cow"], "moo");
    EXPECT_EQ(result.first["spam"], "eggs");
    EXPECT_EQ(result.second, 23);
}

TEST(BencodeParserTest, GetDictEmpty) {
    parser::BencodeParser parser;
    auto result = parser.getDict("de");
    EXPECT_TRUE(result.first.empty());
    EXPECT_EQ(result.second, 2);
}
