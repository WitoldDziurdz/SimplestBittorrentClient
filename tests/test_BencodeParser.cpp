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
    EXPECT_EQ(parser.getString("9:some data111").second, 11);
}

TEST(BencodeParser, GetString_Empty) {
    parser::BencodeParser parser;
    EXPECT_EQ(parser.getString("0:").first, nlohmann::json(""));
    EXPECT_EQ(parser.getString("0:").second, 2);
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
    EXPECT_EQ(parser.getInteger("i-42e").second, 5);
    EXPECT_EQ(parser.getInteger("i123456789e").first, nlohmann::json(123456789));
    EXPECT_EQ(parser.getInteger("i123456789e").second, 11);
    EXPECT_EQ(parser.getInteger("i123e456").first, nlohmann::json(123));
    EXPECT_EQ(parser.getInteger("i123e456").second, 5);
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
    EXPECT_EQ(result.second, 14);
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
    EXPECT_EQ(result.second, 24);
}

TEST(BencodeParserTest, GetDictEmpty) {
    parser::BencodeParser parser;
    auto result = parser.getDict("de");
    EXPECT_TRUE(result.first.empty());
    EXPECT_EQ(result.second, 2);
}

TEST(BencodeParserTest, ParseValidNestedStructures) {
    parser::BencodeParser parser;
    auto result = parser.parse("d4:listl4:spam4:eggse3:numi42ee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]["list"][0], "spam");
    EXPECT_EQ(result[0]["list"][1], "eggs");
    EXPECT_EQ(result[0]["num"], 42);
}

TEST(BencodeParserTest, ParseInvalidList) {
    parser::BencodeParser parser;
    EXPECT_THROW(parser.parse("ls:e"), std::invalid_argument);
}

TEST(BencodeParserTest, ParseInvalidDict) {
    parser::BencodeParser parser;
    EXPECT_THROW(parser.parse("d3:cow3:moo4:spame"), std::invalid_argument);
}

// Test a complex dictionary with nested lists and dictionaries
TEST(BencodeParserTest, ParseComplexDictWithNestedListAndDict) {
    parser::BencodeParser parser;
    auto result = parser.parse("d4:info3:1234:datal4:spaml1:a1:bee3:numi42ee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]["info"], "123");
    EXPECT_EQ(result[0]["data"][0], "spam");
    EXPECT_EQ(result[0]["num"], 42);
    EXPECT_EQ(result[0]["data"][1][0], "a");
    EXPECT_EQ(result[0]["data"][1][1], "b");
}

// Test nested lists within a list
TEST(BencodeParserTest, ParseNestedLists) {
    parser::BencodeParser parser;
    auto result = parser.parse("ll4:spam4:eggsel3:foo3:baree");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0][0], "spam");
    EXPECT_EQ(result[0][0][1], "eggs");
    EXPECT_EQ(result[0][1][0], "foo");
    EXPECT_EQ(result[0][1][1], "bar");
}

// Test nested dictionaries within a list
TEST(BencodeParserTest, ParseListOfDicts) {
    parser::BencodeParser parser;
    auto result = parser.parse("ld3:cow3:moo4:spam4:eggsed3:cat4:meowee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0]["cow"], "moo");
    EXPECT_EQ(result[0][0]["spam"], "eggs");
    EXPECT_EQ(result[0][1]["cat"], "meow");
}

// Test a complex dictionary with multiple types and levels of nesting
TEST(BencodeParserTest, ParseVeryComplexDict) {
    parser::BencodeParser parser;
    auto result = parser.parse("d4:dictd3:foo3:bare4:listl4:spam4:eggsd4:itemi42eed3:key5:valueel3:abc3:defeee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]["dict"]["foo"], "bar");
    EXPECT_EQ(result[0]["list"][0], "spam");
    EXPECT_EQ(result[0]["list"][1], "eggs");
    EXPECT_EQ(result[0]["list"][2]["item"], 42);
    EXPECT_EQ(result[0]["list"][3]["key"], "value");
    EXPECT_EQ(result[0]["list"][4][0], "abc");
    EXPECT_EQ(result[0]["list"][4][1], "def");
}

// Test a complex list with mixed types at different levels
TEST(BencodeParserTest, ParseComplexListWithMixedTypes) {
    parser::BencodeParser parser;
    auto result = parser.parse("l4:spamd3:cow3:moo4:listl2:ab2:cdeei42ee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0], "spam");
    EXPECT_EQ(result[0][1]["cow"], "moo");
    EXPECT_EQ(result[0][2], 42);
    EXPECT_EQ(result[0][1]["list"][0], "ab");
    EXPECT_EQ(result[0][1]["list"][1], "cd");
}

// Test a complex structure with all Bencode types
TEST(BencodeParserTest, ParseAllBencodeTypesComplexStructure) {
    parser::BencodeParser parser;
    auto result = parser.parse("d4:spaml4:spam4:eggsd3:cow3:moo3:piei314eee4:dictd5:fruit5:apple4:yeari2023eee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]["spam"][0], "spam");
    EXPECT_EQ(result[0]["spam"][1], "eggs");
    EXPECT_EQ(result[0]["spam"][2]["cow"], "moo");
    EXPECT_EQ(result[0]["spam"][2]["pie"], 314);
    EXPECT_EQ(result[0]["dict"]["fruit"], "apple");
    EXPECT_EQ(result[0]["dict"]["year"], 2023);
}

// Test a mixed-type list with different element types
TEST(BencodeParserTest, ParseMixedTypeList) {
    parser::BencodeParser parser;
    auto result = parser.parse("li42e4:spaml1:aee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0], 42);
    EXPECT_EQ(result[0][1], "spam");
    EXPECT_EQ(result[0][2][0], "a");
}

// Test a dictionary with nested lists and other dictionaries
TEST(BencodeParserTest, ParseDictWithNestedListAndDict) {
    parser::BencodeParser parser;
    auto result = parser.parse("d4:listl4:spam4:eggseed4:dictd3:cow3:mooee");
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0]["list"][0], "spam");
    EXPECT_EQ(result[0]["list"][1], "eggs");
    EXPECT_EQ(result[1]["dict"]["cow"], "moo");
}

// Test a list with nested dictionaries and lists
TEST(BencodeParserTest, ParseListWithNestedDictsAndLists) {
    parser::BencodeParser parser;
    auto result = parser.parse("ld3:cow3:mooel4:spam4:eggse3:pied5:applei5eee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0][0]["cow"], "moo");
    EXPECT_EQ(result[0][1][0], "spam");
    EXPECT_EQ(result[0][1][1], "eggs");
    EXPECT_EQ(result[0][3]["apple"], 5);
}

// Test complex nesting within dictionaries and lists
TEST(BencodeParserTest, ParseComplexNesting) {
    parser::BencodeParser parser;
    auto result = parser.parse("d4:dictd4:listld3:cow3:mooel4:spam4:eggsed4:name4:Johnel3:doe3:doei99eeeee");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]["dict"]["list"][0]["cow"], "moo");
    EXPECT_EQ(result[0]["dict"]["list"][1][0], "spam");
    EXPECT_EQ(result[0]["dict"]["list"][1][1], "eggs");
    EXPECT_EQ(result[0]["dict"]["list"][2]["name"], "John");
    EXPECT_EQ(result[0]["dict"]["list"][3][0], "doe");
    EXPECT_EQ(result[0]["dict"]["list"][3][1], "doe");
    EXPECT_EQ(result[0]["dict"]["list"][3][2], 99);
}