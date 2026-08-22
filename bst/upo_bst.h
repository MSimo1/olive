#include <stdlib.h>

typedef int (*key_compare_func)(void*, void*);

struct upo_bst_node_s {
    void* key;
    void* value;
    struct upo_bst_node_s* left;
    struct upo_bst_node_s* right;
};
typedef struct upo_bst_node_s upo_bst_node_t;

typedef struct upo_bst_s {
    upo_bst_node_t* root;
    key_compare_func key_cmp;
} upo_bst_t;