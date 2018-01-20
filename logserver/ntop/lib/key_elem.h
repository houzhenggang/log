#ifndef KEY_ELEM_H
#define KEY_ELEM_H

#include "rbtree.h"

typedef u_int64_t key_type;
typedef void * data_type;

struct key_node
{
    struct rb_node node;
    key_type key;
    union {
        data_type data;
        u_long second;
    };
};


struct key_tree
{
    struct rb_root root;

    u_long ncount;
    union{
        time_t erase_tick;
        time_t last_tick;
    };
    int mutex;// = 0;
    int lock;// = 0;
    int unlock;// = 1;
};

#define KEY_TREE_NODES(tree) ((tree)->ncount)
#define key_tree_lock(tree)     while (!(__sync_bool_compare_and_swap (&(tree)->mutex,(tree)->lock, 1) )) {sched_yield();}
#define key_tree_unlock(tree)   __sync_bool_compare_and_swap(&(tree)->mutex,(tree)->unlock,0);

extern struct key_node *key_first(struct key_tree *tree);
extern struct key_node *key_next(struct key_node *node_t);
void key_tree_init(struct key_tree *tree);
void key_tree_destroy(struct key_tree *tree);
struct key_node* key_rbinsert(struct key_tree *tree,const key_type first,void *second);
struct key_node* key_rbinsert2(struct key_tree *tree,const key_type first,u_long second);
struct key_node* key_rbsearch(struct key_tree *tree,key_type key);

struct key_node *key_rberase(struct key_tree *tree,struct key_node *node);
struct key_node *key_rberase_EX(struct key_tree *tree,struct key_node *node_it,void *param,void (*freec)(void*,void*));

void *key_rberase_by_key(struct key_tree *tree,key_type key);
void *key_rberase_by_key_EX(struct key_tree *tree,key_type key,void *param,void (*func)(void*,void*));


void key_rbclear_Ex(struct key_tree *tree, void (*freedate)(void *));


struct key_node *Rbtree_FindLastNode_FollowKey(struct key_tree *tree,const u_int32_t key);

#endif
