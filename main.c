// Huffman Encoder/Decoder
// https://codingchallenges.fyi/challenges/challenge-huffman/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXVAL 127

// TAB: 9, NEWLINE: 10
enum { TAB = '\t', NEWLINE = '\n', SPACE = ' ', DEL = '\x7f' };

struct node {
  int id;
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
  for (size_t i = 32; i < size; ++i) {
    printf("[%c] %d,", arr[i].value, arr[i].count);
    if (i % 10 == 0)
      printf("\n");
  }
  printf("\n");
}

void printTree(struct node *tree) {
  if (tree != NULL) {
    printTree(tree->left);
    if (isprint(tree->value))
      printf("%c %8d\n", tree->value, tree->count);
    printTree(tree->right);
  }
}

void dotTree(struct node *tree) {
  char subg[] = "%c->{%c,%c};";
  char attr[] = "%c[label='%c\\n%i'];";
  if (tree != NULL) {
    dotTree(tree->left);

    if (tree->left && tree->right)
      printf(subg, tree->count, tree->left->count, tree->right->count);

    if (isprint(tree->value))
      printf(attr, tree->value, tree->value, tree->count);

    dotTree(tree->right);
  }
}

void graphTree(struct node *tree) {
  printf("digraph {");
  dotTree(tree);
  printf("}\n");
}

int main(int argc, char **argv) {

  char *file = "test.txt";

  // NOTE: no error handling (at least for now)
  FILE *fp = fopen(file, "r");

  // enough to store all the leaf nodes in a binary table
  struct node freq[MAXVAL];
  size_t size = sizeof(freq) / sizeof(freq[0]);

  // tracks pointers into the frequency table while creating sorted trees
  // will hold the binary tree
  struct node *lut[MAXVAL];

  // initialize frequency table
  for (int i = 0; i < MAXVAL; i++) {
    freq[i].id = i + 1;
    freq[i].value = (char)i;
    freq[i].count = 0;
    freq[i].left = NULL;
    freq[i].right = NULL;
    lut[i] = &freq[i];
  }

  int ch;
  while ((ch = getc(fp)) != EOF)
    if (ch < 127)
      freq[ch].count++;

  // smallest frequencies to largest
  // NOTE: the lut tracks with this, i.e., no need to sort the lut
  qsort(freq, size, sizeof(struct node), frqcmp);

  printArray(freq, size);

  //  TODO: This works, but I'm not quite sure how yet
  //
  // printArray(*lut, size);

  // allocate storage for internal (non-leaf) nodes
  struct node internals[MAXVAL - 1];
  struct node *next = &internals[0];

  // convert to a binary tree
  int idx = 0;
  while (idx < MAXVAL - 1) {
    // get the next two minimum values, lut must be kept in 'priority' order
    // by min(count)
    struct node *n1 = lut[idx];
    struct node *n2 = lut[idx + 1];

    // grab next node from the internals (as in 'internal' to the tree) storage
    struct node *new = next++;

    // Sum the weights and create tree pointing to these nodes
    new->count = n1->count + n2->count;
    new->left = n1;
    new->right = n2;
    new->value = 0;

    // Need to save this new node in a way that it can be compared and assigned
    // later ... in the right order. Try a sort of stack-based operation using
    // the lut
    // We're combining two columns into one with the new node, find the next
    // slot for the new node based on count comparisons, shifting pointers left
    // to make room.
    int slot = ++idx;

    // we're at the end this must be equal to, or larger than the last count
    if (idx == MAXVAL - 1) {
      lut[slot] = new;
      break;
    }

    // Replace n2 with the new (tree) node, if next element is larger
    if (new->count < lut[slot + 1]->count) {
      lut[slot] = new;
      continue;
    }

    // shift nodes left until a spot is found, mind the boundary
    do {
      lut[slot] = lut[slot + 1];
      slot++;
      if (slot == MAXVAL - 1)
        break;
    } while (lut[slot]->count < new->count);

    lut[slot] = new;
  }
  // graphTree(lut[idx]);
}
