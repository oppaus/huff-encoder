/* vim: set filetype=c : */
#include <stdint.h>

#define MAXVAL 127

struct node {
  int id;
  int count;
  char value;
  struct node *left;
  struct node *right;
};

struct prefix {
  char value;
  int count;
  uint32_t code;
  int bits;
} codec[MAXVAL];

#ifdef __cplusplus
extern "C" {
#endif

// use for testing to setup mock frequencies
void set_freq(int c, int v);
struct node *get_freq();
void bitstring(char *str, int len, uint32_t bits);
void init_tree();
void increment(int c);
void make_binary();
int depth();
int codify();
struct prefix *get_codec();
struct node *get_root();

#ifdef __cplusplus
}
#endif
