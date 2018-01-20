#include "rbset.h"
#include <stdlib.h>
#include <string.h>

struct rb_node* rbset_insert(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *))
{
    struct rb_node **new_node=&(root->rb_node), *parent=NULL;
    /* Figure out where to put new node  */
    int r;
    while(*new_node) {
        parent = *new_node;
        r = cmp(key_node,(*new_node)->data);
        if(r<0) {
            new_node = &((*new_node)->rb_left);
        } else if(r>0) {
            new_node = &((*new_node)->rb_right);
        } else
            return (*new_node);
    }

    /* Add new node and rebalance tree  */
    struct rb_node *node_t=(struct rb_node *)malloc(RB_NODE_SIZE);
    memset(node_t,0,RB_NODE_SIZE);
    node_t->data=key_node;
    rb_link_node(node_t, parent, new_node);
    rb_insert_color(node_t, root);
    return NULL;
}


struct rb_node *rbset_search(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *))
{
    struct rb_node *node=root->rb_node;
    int r;
    while(node) {
         r = cmp(key_node,node->data);
        if(r<0)  {
            node = node->rb_left;
        } else if(r>0) {
            node = node->rb_right;
        } else
            return node;
    }
    return NULL;
}


struct rb_node *rbset_erase(struct rb_root *root,struct rb_node *node)
{
    if(!node) {
        return node;
    }
    struct rb_node *tmp_node=rb_next(node);
    rb_erase(node,root);
    free(node);
    node=tmp_node;
    return node;
}


void *rbset_erase_key(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *))
{
    struct rb_node *node=root->rb_node;
    int r;
    while(node) {
         r = cmp(key_node,node->data);
        if(r<0) {
            node = node->rb_left;
        } else if(r>0) {
            node = node->rb_right;
        } else {
            void *pdata = node->data;
            rb_erase(node,root);
            free(node);
            return  pdata;
        }
    }
    return NULL;
}

void rbset_clear(struct rb_root *root)
{
    struct rb_node *node;
    for(node=rb_first(root);node; ) {
        node=rbset_erase(root,node);
    }
}

void rbset_clear_Ex(struct rb_root *root,void (*freec)(void *))
{
    if(freec) {
        void *pdata;
        struct rb_node *node;
        for(node=rb_first(root);node; ) {
            pdata = node->data;
            node=rbset_erase(root,node);
            freec(pdata);
        }
    } else {
        rbset_clear(root);
    }
}


void rbset_destroy(struct rb_root *root,void (*freec)(void *))
{
    if(!root)
        return ;
    rbset_clear_Ex(root,freec);
}


struct rb_node *rbset_erase_EX(struct rb_root *root,struct rb_node *node_it,void *param,void (*freec)(void*,void*))
{
    if(!node_it) {
        return node_it;
    }
    struct rb_node *tmp_node=rb_next(node_it);
    rb_erase(node_it,root);
    freec(node_it,param);
    node_it=tmp_node;
    return node_it;
}


void *rbset_erase_key_EX(struct rb_root *root,void *key_node, int (*cmp)(const void *, const void *),void *param,void (*freec)(void*,void*))
{
    struct rb_node *node=root->rb_node;
    int r;
    while(node) {
        r = cmp(key_node,node->data);
        if(r<0) {
            node = node->rb_left;
        } else if(r>0) {
            node = node->rb_right;
        } else {
            void *pdata = node->data;
            rb_erase(node,root);
            freec(node,param);
            return pdata;
        }
    }
    return NULL;
}
