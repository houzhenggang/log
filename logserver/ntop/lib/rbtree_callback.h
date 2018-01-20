#ifndef RBTREE_CALLBACK_H
#define RBTREE_CALLBACK_H

#include "rbtree.h"
#include <stdlib.h>
struct rb_node* int_void_rbinsert(struct rb_root *root,const key_type first,void *second);
struct rb_node* int_void_rbinsert3(struct rb_root *root,const key_type first,void *second,time_t tag);
struct rb_node* int_void_rbinsert_second(struct rb_root *root,const key_type first,u_int32_t second,time_t tag);

struct rb_node *int_void_rbsearch(struct rb_root *root,key_type key);
struct rb_node *int_void_rberase(struct rb_root *root,struct rb_node *node);
void *int_void_rberase_key(struct rb_root *root,key_type key);

void int_void_rbclear2(struct rb_root *root,void (*func)(void *));

void int_void_rbclear(struct rb_root *root);
void int_void_rbclear_Ex(struct rb_root *root,void (*freedate)(void *));
void int_void_rbdestroy(struct rb_root *root);

struct rb_node *int_void_rberase_EX(struct rb_root *root,struct rb_node *node,void *param,void (*)(void*,void*));
void int_void_rberase_key_EX(struct rb_root *root,key_type key,void *param,void (*)(void*,void*));
void *int_void_rberase_key_EX2(struct rb_root *root,key_type key,void *param,void (*func)(void*,void*));

struct rb_node *Rbtree_FindLastNode_FollowKey(struct rb_root *root,const u_int32_t key);

#endif // RBTREE_CALLBACK_H

