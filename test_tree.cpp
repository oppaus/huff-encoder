#include "tree.h"
#include <gtest/gtest.h>

class TestTree : public testing::Test {
protected:
  void SetUp() override {
    init_freq();
    set_freq('C', 32);
    set_freq('D', 42);
    set_freq('E', 120);
    set_freq('K', 7);
    set_freq('L', 42);
    set_freq('M', 24);
    set_freq('U', 37);
    set_freq('Z', 2);

    create_tree();
  }
};

TEST_F(TestTree, MakeTreeWorks) {
  struct node *t = get_root();
  EXPECT_EQ(t->count, 306);
}

TEST_F(TestTree, TreeDepth) { EXPECT_EQ(depth(), 6); }

TEST_F(TestTree, Codec) {
  codify();
  struct prefix *tbl = get_codec();
  EXPECT_EQ(tbl['C'].count, 32);
  EXPECT_EQ(tbl['Z'].count, 2);

  EXPECT_EQ(tbl['C'].bits, 4);
  EXPECT_EQ(tbl['E'].bits, 1);
  EXPECT_EQ(tbl['L'].bits, 3);
  EXPECT_EQ(tbl['Z'].bits, 6);

  EXPECT_EQ(tbl['C'].code, 14);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
