// Huffman Encoder/Decoder
// https://codingchallenges.fyi/challenges/challenge-huffman/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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

  int ch;
  while ((ch = getc(fp)) != EOF)
    increment(ch);

  // convert to a binary tree
  make_binary();

  struct node *root = get_root();
  graphTree(root);
}
