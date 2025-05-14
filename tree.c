#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

// enough to store all the leaf nodes in a binary table
static struct node freq[MAXVAL];
static size_t nelems = sizeof(freq) / sizeof(freq[0]);

// tracks pointers into the frequency table while creating sorted trees
// will hold the binary tree
static struct node *lut[MAXVAL];
static struct node *_root;

// allocate storage for internal (non-leaf) nodes
static struct node internals[MAXVAL - 1];
static struct node *next = &internals[0];

void set_freq(int c, int v) { freq[c].count = v; }

struct node *get_freq() { return freq; }

void bitstring(char *str, int len, uint32_t code) {
  for (int i = 0, j = len - 1; i < len; i++, j--)
    if ((code >> i) & 1)
      str[j] = '1';

  str[len] = '\0';
  printf("%i: %s\n", code, str);
}

void printArray(struct node arr[], size_t size) {
  // TODO: stop printing char's (that's why I'm skipping), because of things
  // like newlines
  for (size_t i = 32; i < size; ++i) {
    if (arr[i].value == '\n' || arr[i].value == '\r') {
      printf("[%s] %d,", "\\n", arr[i].count);
    } else
      printf("[%c] %d,", arr[i].value, arr[i].count);

    if (i % 10 == 0)
      printf("\n");
  }
  printf("\n");
}

// initializes the frequency array and pointer look up table
void init_freq() {
  for (int i = 0; i < MAXVAL; i++) {
    freq[i].id = i + 1;
    freq[i].value = (char)i;
    freq[i].count = 0;
    freq[i].left = NULL;
    freq[i].right = NULL;
    lut[i] = &freq[i];
  }
}

// increments the cound of the given character in the frequency table
void increment(int c) {
  if (c < 127)
    freq[c].count++;
}

// converts the look up table into a binary tree, skipping characters not seen
// in the file
static void _treeify() {
  int idx = 0;
  int newid = MAXVAL + 1;

  // skip to the first seen character
  while (lut[idx]->count == 0)
    ++idx;

  while (idx < MAXVAL - 1) {
    // get the next two minimum values, lut must be kept in 'priority' order
    // by min(count)
    struct node *n1 = lut[idx];
    struct node *n2 = lut[idx + 1];

    // grab next node from the internals (as in 'internal' to the tree) storage
    struct node *new = next++;

    // Sum the weights and create tree pointing to these nodes
    new->id = newid++;
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
    while (lut[slot + 1]->count < new->count) {
      lut[slot] = lut[slot + 1];
      slot++;
      if (slot == MAXVAL - 1)
        break;
    }

    lut[slot] = new;
  }
}

static int frqcmp(const void *a, const void *b) {
  const struct node *A = a, *B = b;
  if (A->count < B->count)
    return -1;
  else if (A->count == B->count)
    return 0;
  else
    return 1;
}

void create_tree() {
  // smallest frequencies to largest
  // NOTE: the lut tracks with this, i.e., no need to sort the lut
  qsort(freq, nelems, sizeof(struct node), frqcmp);

  // printArray(freq, nelems);

  _treeify();

  _root = lut[MAXVAL - 1];
}

void rebuild_tree() {
  for (int i = 0; i < MAXVAL; i++)
    lut[i] = &freq[i];

  _treeify();
  _root = lut[MAXVAL - 1];
}

// private recursive function to find the depth
static int _get_depth(struct node *root) {
  if (root == NULL)
    return -1;

  int ld = _get_depth(root->left);
  int rd = _get_depth(root->right);

  return (ld > rd ? ld : rd) + 1;
}

// public function to return the depth
int depth() { return _get_depth(_root); }

static void _build_codec(struct node *n, uint32_t path, int depth) {
  if (n == NULL)
    return;

  depth++;
  if (n->left == NULL && n->right == NULL) {
    struct prefix *p = &codec[n->value];
    p->value = n->value;
    p->count = n->count;
    p->code = path;
    p->bits = depth;
  } else {
    _build_codec(n->left, (path << 1), depth);
    _build_codec(n->right, (path << 1) | 1, depth);
  }

  depth--;
  path = path >> 1;
}

struct prefix *get_codec() { return codec; }

// traverse the tree and build the prefix codes
int codify() {
  // initialize the codec table
  for (int i = 0; i < MAXVAL; i++) {
    codec[i].count = 0;
    codec[i].value = 0;
    codec[i].code = 0;
    codec[i].bits = 0;
  }

  uint32_t path = 0;
  _build_codec(_root, path, -1);

  int sizez = 0;
  for (int i = 0; i < MAXVAL; i++)
    if (codec[i].count)
      sizez += codec[i].count * codec[i].bits;

  return sizez;
}

struct node *get_root() { return _root; }
