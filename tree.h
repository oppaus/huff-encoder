/* vim: set filetype=c : */
#define MAXVAL 127

struct node {
  int id;
  int count;
  char value;
  struct node *left;
  struct node *right;
};

#ifdef __cplusplus
extern "C" {
#endif

void set_freq(int c, int v);
void init_tree();
void increment(int c);
void make_binary();
struct node *get_root();

#ifdef __cplusplus
}
#endif
