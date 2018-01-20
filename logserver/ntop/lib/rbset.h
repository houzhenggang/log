#ifndef _RBMAP_H
#define _RBMAP_H

#include "rbtree.h"


struct rb_node* rbset_insert(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *));
struct rb_node *rbset_search(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *));
void rbset_destroy(struct rb_root *root,void (*freec)(void *));

void rbset_clear(struct rb_root *root);
void rbset_clear_Ex(struct rb_root *root,void (*freec)(void *));


struct rb_node *rbset_erase(struct rb_root *root,struct rb_node *node);
void *rbset_erase_key(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *));

struct rb_node *rbset_erase_EX(struct rb_root *root,struct rb_node *node_it,void *param,void (*freec)(void*,void*));
void *rbset_erase_key_EX(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *),void *param,void (*freec)(void*,void*));

#endif // RBMAP_H

