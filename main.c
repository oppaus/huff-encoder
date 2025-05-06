// Huffman Encoder/Decoder
// https://codingchallenges.fyi/challenges/challenge-huffman/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

void printTree(struct node *tree) {
  if (tree != NULL) {
    printTree(tree->left);
    if (isprint(tree->value))
      printf("%c %8d\n", tree->value, tree->count);
    printTree(tree->right);
  }
}

void dotTree(struct node *tree) {
  char subg[] = "%i->{%i,%i};";
  char attr[] = "%i[label=\"%i\\n%i\"];";
  if (tree != NULL) {
    dotTree(tree->left);

    if (tree->left && tree->right)
      printf(subg, tree->id, tree->left->id, tree->right->id);

    printf(attr, tree->id, tree->value, tree->count);

    dotTree(tree->right);
  }
}

// Dot (graphviz) formatted output
void graphTree(struct node *tree) {
  printf("digraph {");
  dotTree(tree);
  printf("}\n");
}

int main(int argc, char **argv) {

  init_tree();

  char *file = "test.txt";

  // NOTE: no error handling (at least for now)
  FILE *fp = fopen(file, "r");

  // int ch;
  // while ((ch = getc(fp)) != EOF)
  //   increment(ch);

  set_freq('C', 32);
  set_freq('D', 42);
  set_freq('E', 120);
  set_freq('K', 7);
  set_freq('L', 42);
  set_freq('M', 24);
  set_freq('U', 37);
  set_freq('Z', 2);

  // convert to a binary tree
  make_binary();

  struct prefix *tbl = codify();

  int len = tbl['C'].bits;
  char str[33];
  memset(str, '0', sizeof(str));
  bitstring(str, len, tbl['C'].code);

  // struct node *root = get_root();
  // graphTree(root);
}
