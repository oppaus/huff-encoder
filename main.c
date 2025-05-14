// Huffman Encoder/Decoder
// https://codingchallenges.fyi/challenges/challenge-huffman/

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

#define MIN(a, b) (a < b) ? a : b;

int build_encoding(FILE *fp) {
  init_freq();

  int ch;
  while ((ch = getc(fp)) != EOF)
    increment(ch);

  // convert frequency table to a binary tree
  create_tree();
  return codify();
}

int main(int argc, char **argv) {

  char *infile = "test.txt";
  char *outfile = "test.txt.jz";

  FILE *infp = fopen(infile, "r");

  if (infp == NULL) {
    fprintf(stderr, "Error opening %s\n", infile);
    exit(errno);
  }

  int sizez = build_encoding(infp);

  FILE *outfp = fopen(outfile, "wb");
  if (outfp == NULL) {
    fprintf(stderr, "Error opening %s\n", outfile);
    exit(errno);
  }

  // write out the frequency table
  struct node *freq = get_freq();
  size_t bytes_written = fwrite(freq, sizeof(struct node), MAXVAL, outfp);

  if (bytes_written != MAXVAL) {
    fprintf(stderr, "Problem writing frequency table\n");
    exit(1);
  }

  // write out size of compressed data
  fprintf(outfp, "%d", sizez);

  int buf_capacity = 8;
  unsigned char buffer = 0;
  struct prefix *codec = get_codec();

  // read (again) from the input text file to compress
  rewind(infp);
  int ch;
  while ((ch = getc(infp)) != EOF) {
    struct prefix *p = &codec[ch];
    int len = p->bits;
    uint32_t code = p->code;

    int bits_remaining = len;
    while (bits_remaining) {
      int bits_to_write = MIN(bits_remaining, buf_capacity);
      buffer = buffer << bits_to_write;

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
