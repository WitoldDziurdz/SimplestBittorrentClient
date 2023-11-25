#include <gtest/gtest.h>
#include "BencodeParser.h"

// Tests for getString
TEST(BencodeParser, GetString_Valid) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getString("4:spam"), "spam");
    EXPECT_EQ(parser.getString("12:Middle Earth"), "Middle Earth");
    EXPECT_EQ(parser.getString("9:some data"), "some data");
}

TEST(BencodeParser, GetString_Empty) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getString("0:"), "");
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
    EXPECT_EQ(parser.getInteger("i42e"), 42);
    EXPECT_EQ(parser.getInteger("i-42e"), -42);
    EXPECT_EQ(parser.getInteger("i123456789e"), 123456789);
}

TEST(BencodeParser, GetInteger_Invalid) {
    parser::BencodeParser parser;
    EXPECT_THROW(parser.getInteger("42"), std::invalid_argument);
    EXPECT_THROW(parser.getInteger("ie"), std::invalid_argument);
    EXPECT_THROW(parser.getInteger("i-42"), std::invalid_argument);
    EXPECT_THROW(parser.getInteger("i123e456"), std::invalid_argument);
}

TEST(BencodeParser, GetInteger_Zero) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getInteger("i0e"), 0);
}

TEST(BencodeParser, GetInteger_LeadingZeros) {
    parser::BencodeParser parser;
    // Leading zeros are not allowed in Bencode integers, except for '0'.
    EXPECT_THROW(parser.getInteger("i0123e"), std::invalid_argument);
}
