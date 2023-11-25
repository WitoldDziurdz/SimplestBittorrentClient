#include "gtest/gtest.h"
#include "BencodeParser.h"

// Example test case
TEST(MainTest, GetString) {
parser::BencodeParser bp;
std::string result = bp.getString("4:spam");
EXPECT_EQ(result, "spam");
}
