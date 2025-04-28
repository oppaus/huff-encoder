// Huffman Encoder/Decoder
// https://codingchallenges.fyi/challenges/challenge-huffman/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: do we need all 256 values?
#define MAXVAL 256

struct node {
  int count;
  char value;
  struct node *left;
  struct node *right;
};

int frqcmp(const void *a, const void *b) {
  const struct node *A = a, *B = b;
  if (A->count < B->count)
    return -1;
  else if (A->count == B->count)
    return 0;
  else
    return 1;
}

void printArray(struct node arr[], size_t size) {
  for (size_t i = 0; i < size; ++i)
    printf("%d ", arr[i].count);
  printf("\n");
}

void printTree(struct node *tree) {
  if (tree != NULL && isprint(tree->value)) {
    printTree(tree->left);
    printf("%c %8d\n", tree->value, tree->count);
    printTree(tree->right);
  }
}

int main(int argc, char **argv) {

  if (argc < 2) {
    fprintf(stderr, "Usage: compress <filename>\n");
    abort();
  }

  char *file = argv[1];

  // NOTE: no error handling (at least for now)
  FILE *fp = fopen(file, "r");

  // enough to store all the leaf nodes in a binary table
  struct node freq[MAXVAL];
  size_t size = sizeof(freq) / sizeof(freq[0]);

  // tracks pointers into the frequency table while creating sorted trees
  // holds the binary tree
  struct node *lut[MAXVAL];

  // initialize frequency table
  for (int i = 0; i < MAXVAL; i++) {
    freq[i].value = (char)i;
    freq[i].count = 0;
    freq[i].left = NULL;
    freq[i].right = NULL;
    lut[i] = &freq[i];
  }

  int ch;
  while ((ch = getc(fp)) != EOF) {
    freq[ch].count++;
  }

  // smallest frequencies to largest
  qsort(freq, size, sizeof(struct node), frqcmp);
  printArray(freq, size);

  for (int i = 0; i < size; i++)
    printf("%d ", lut[i]->count);

  printf("\n");

  // allocate storage for internal (non-leaf) nodes
  struct node internals[MAXVAL - 1];
  struct node *next = &internals[0];

  // start lut at the second node because we combine the first two elements
  struct node *p = lut[1];

  // convert to a binary tree
  int idx = 0;
  while (idx < MAXVAL) {
    // pop (well shift) off the next two minimum values
    struct node *n1 = lut[idx];
    struct node *n2 = lut[idx + 1];

    // grab next node from the internals (as in 'internal' to the tree) storage
    struct node *new = next++;

    // Sum the weights and create tree pointing to these nodes
    new->count = n1->count + n2->count;
    new->left = n1;
    new->right = n2;
    new->value = 0;

    if (new->count > 0)
      printf("Adding node with count: %d\n", new->count);

    // Need to save this new node in a way that it can be compared and assigned
    // later ... in the right order. Try a sort of stack-based operation using
    // the lut
    // We're combining two columns into one with the new node, find the next
    // slot for the new node based on count comparisons, shifting pointers left
    // to make room.
    int slot = ++idx;
    while (slot < MAXVAL && lut[slot++]->count < new->count)
      lut[slot - 1] = lut[slot];

    lut[slot] = new;
  }

  printTree(lut[idx]);
}
