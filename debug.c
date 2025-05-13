#include <ctype.h>
#include <stdio.h>

// TODO: move to header? Already included in `main.c`
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
