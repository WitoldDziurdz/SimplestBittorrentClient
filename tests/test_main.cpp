#include "gtest/gtest.h"
#include "BencodeParser.h"
#include "header_libs/json.hpp"

// Example test case
TEST(MainTest, GetString) {
parser::BencodeParser bp;
std::string result = bp.getString("4:spam").first;
EXPECT_EQ(result, "spam");
}
