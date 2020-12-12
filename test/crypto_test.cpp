#include "util/crypto.h"

#include "gtest/gtest.h"
namespace uestc {

TEST(CryptoTest, HmacMD5Test) {
  // empty value
  ASSERT_EQ(HmacMD5("", ""), "74e6f7298a9c2d168935f58c001bad88");
  // normal world
  ASSERT_EQ(HmacMD5("hello", "world"), "1a75d01d561d51776bf36c996b826d6b");
}

TEST(CryptoTest, SHa1Test) {
  // empty_value
  ASSERT_EQ(Sha1(""), "da39a3ee5e6b4b0d3255bfef95601890afd80709");
  // normal value
  ASSERT_EQ(Sha1("2020"), "85568b20c3315286c4dfebb330b25146f92bed66");
}

TEST(XEncodeTest, XEncodeTest) {
  // empty value
  ASSERT_TRUE(XEncode("", "").size() == 0);
  // normal value
  auto res = XEncode(
      "2020",
      "6359decb953189ff51352a282c61db729c320b9706852335ffc7cec56d016452");
  ASSERT_TRUE(res.size() == 8);
  std::vector<unsigned char> b{0x61, 0x85, 0x7c, 0x2b, 0xc0, 0x86, 0x40, 0x89};
  ASSERT_TRUE(std::equal(b.begin(), b.end(), res.begin()));
}

}  // namespace uestc
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}