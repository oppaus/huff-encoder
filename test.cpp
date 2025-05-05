#include "tree.h"
#include <gtest/gtest.h>

class CompressorTest : public testing::Test {
protected:
  void SetUp() override {
    init_tree();
    set_freq('C', 32);
    set_freq('D', 42);
    set_freq('E', 120);
    set_freq('K', 7);
    set_freq('L', 42);
    set_freq('M', 24);
    set_freq('U', 37);
    set_freq('Z', 2);
    set_freq('*', 0);
    set_freq('c', 0);
  }
};

TEST_F(CompressorTest, MakeTreeWorks) {
  make_binary();
  struct node *t = get_root();
  EXPECT_EQ(t->count, 306);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
