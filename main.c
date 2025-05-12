// Huffman Encoder/Decoder
// https://codingchallenges.fyi/challenges/challenge-huffman/

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

#define MIN(a, b) (a < b) ? a : b;

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

  char *infile = "test.txt";
  char *outfile = "test.txt.jz";

  FILE *infp = fopen(infile, "r");

  if (infp == NULL) {
    fprintf(stderr, "Error opening %s\n", infile);
    exit(errno);
  }

  int ch;
  while ((ch = getc(infp)) != EOF)
    increment(ch);

  // convert to a binary tree
  make_binary();

  // write out the tree
  FILE *outfp = fopen(outfile, "wb");

  if (outfp == NULL) {
    fprintf(stderr, "Error opening %s\n", outfile);
    exit(errno);
  }

  struct node *freq = get_freq();
  struct node *root = get_root();

  // TODO: change this to return total number of bits
  // make a get_codify accessor method
  struct prefix *codec = codify();

  // write out the frequency table
  size_t bytes_written = fwrite(freq, sizeof(struct node), MAXVAL, outfp);

  if (bytes_written != MAXVAL) {
    fprintf(stderr, "Problem writing frequency table\n");
    exit(1);
  }

  // write out size of compressed data
  int compressed_size = 0;
  for (int i = 0; i < MAXVAL; i++)
    if (codec[i].count)
      compressed_size += codec[i].count;

  fputc(compressed_size, outfp);

  int buf_capacity = 8;
  unsigned char buffer = 0;
  // read (again) from the input text file to compress
  rewind(infp);
  while ((ch = getc(infp)) != EOF) {
    struct prefix *p = &codec[ch];
    int len = p->bits;
    uint32_t code = p->code;

    int bits_remaining = len;
    while (bits_remaining) {

      // pack buffer, write fully-packed bytes, refreshing the buffer as
      // needed, some codes will take multiple bytes
      int bits_to_write = MIN(bits_remaining, buf_capacity);

      // make room for the next bits
      buffer = buffer << bits_to_write;

      // Isolate the bits that will fill buffer
      // remember, code is a 32 bit integer
      int shift = bits_remaining - bits_to_write;
      uint32_t tmp = (code >> shift) & ((1 << bits_to_write) - 1);

      buffer |= tmp;

      bits_remaining -= bits_to_write;
      buf_capacity -= bits_to_write;

      assert(buf_capacity >= 0);

      if (!buf_capacity) {
        // flush the buffer
        fputc(buffer, outfp);
        buf_capacity = 8;
        buffer = 0;
      }
    }
  }

  if (buffer) {
    buffer <<= buf_capacity;
    fputc(buffer, outfp);
  }

  fclose(infp);
  fclose(outfp);
}
