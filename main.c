// Huffman Encoder/Decoder
// https://codingchallenges.fyi/challenges/challenge-huffman/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
  int weight;
  char value;
  struct node *left;
  struct node *right;
};

int frqcmp(const void *a, const void *b) {
  const struct node *A = a, *B = b;
  if (A->weight < B->weight)
    return -1;
  else if (A->weight == B->weight)
    return 0;
  else
    return 1;
}

void printArray(const struct node arr[], size_t size) {
  for (size_t i = 0; i < size; ++i)
    printf("%d ", arr[i].weight);
  printf("\n");
}

int main(int argc, char **argv) {

  if (argc < 2) {
    fprintf(stderr, "Usage: compress <filename>\n");
    abort();
  }

  char *file = argv[1];
  FILE *fp = fopen(file, "r");

  struct node freq[256];
  size_t size = sizeof(freq) / sizeof(freq[0]);

  // initialize frequency table
  for (int i = 0; i < 256; i++) {
    freq[i].weight = 0;
    freq[i].value = (char)i;
  }

  int ch;
  while ((ch = getc(fp)) != EOF) {
    freq[ch].weight++;
  }

  qsort(freq, size, sizeof(struct node), frqcmp);

  // convert to a binary tree
  struct node *root;
  root = NULL;
}
