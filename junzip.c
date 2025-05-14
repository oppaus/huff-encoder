#include <assert.h>
#include <stdio.h>

#include "tree.h"

int main(int argc, char **argv) {
  char *infile = "test.txt.jz";
  FILE *fp = fopen(infile, "rb");

  if (fp == NULL) {
    fprintf(stderr, "Error opening file: %s", infile);
  }

  // load the frequency table and build binary tree
  struct node *freq = get_freq();
  fread(freq, sizeof(struct node), MAXVAL, fp);
  rebuild_tree();

  // load the size of compressed data (in bits)
  int sizez = 0;
  fscanf(fp, "%d", &sizez);

  int ch, offs = 8, bits_read = 0;
  uint32_t buffer = 0;
  struct node *n = get_root();
  while ((ch = fgetc(fp)) != EOF) {
    buffer = buffer << 8 | ch;
    // decode
    while (n && offs) {
      int bit = buffer & (1 << --offs) ? 1 : 0;
      n = bit ? n->right : n->left;
      bits_read++;
      assert(offs > -1);
      if (n && n->value && bits_read <= sizez) {
        fputc(n->value, stdout);
        n = get_root();
      }
    }
    offs += 8;
  }

  fclose(fp);
}
