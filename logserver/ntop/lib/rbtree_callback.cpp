#include "rbtree_callback.h"
#include<time.h>
#include <string.h>


struct rb_node* int_void_rbinsert(struct rb_root *root,const key_type first,void *second)
{
    struct rb_node **new_node=&(root->rb_node), *parent=NULL;
    /* Figure out where to put new node  */
    while(*new_node) {
        parent = *new_node;
        if(first < (*new_node)->key) {
            new_node = &((*new_node)->rb_left);
        } else if(first > (*new_node)->key) {
            new_node = &((*new_node)->rb_right);
        } else
            return (*new_node);
    }

    /* Add new node and rebalance tree  */
    struct rb_node *node_t=(struct rb_node *)malloc(RB_NODE_SIZE);
    memset(node_t,0,RB_NODE_SIZE);
    node_t->key=first;  node_t->data=second;
    rb_link_node(node_t, parent, new_node);
    rb_insert_color(node_t, root);
    return NULL;
}

struct rb_node* int_void_rbinsert3(struct rb_root *root,const key_type first,void *second,time_t tag)
{
    struct rb_node **new_node=&(root->rb_node), *parent=NULL;
    /* Figure out where to put new node  */
    while(*new_node) {
        parent = *new_node;
        if(first < (*new_node)->key) {
            new_node = &((*new_node)->rb_left);
        } else if(first > (*new_node)->key) {
            new_node = &((*new_node)->rb_right);
        } else
            return (*new_node);
    }

    /* Add new node and rebalance tree  */
    struct rb_node *node_t=(struct rb_node *)malloc(RB_NODE_SIZE);
    memset(node_t,0,RB_NODE_SIZE);
    node_t->key=first;  node_t->data=second;    node_t->tag=tag;
    rb_link_node(node_t, parent, new_node);
    rb_insert_color(node_t, root);
    return NULL;
}

struct rb_node* int_void_rbinsert_second(struct rb_root *root,const key_type first,u_int32_t second,time_t tag)
{
    struct rb_node **new_node=&(root->rb_node), *parent=NULL;
    /* Figure out where to put new node  */
    while(*new_node) {
        parent = *new_node;
        if(first < (*new_node)->key) {
            new_node = &((*new_node)->rb_left);
        } else if(first > (*new_node)->key) {
            new_node = &((*new_node)->rb_right);
        } else
            return (*new_node);
    }

    /* Add new node and rebalance tree  */
    struct rb_node *node_t=(struct rb_node *)malloc(RB_NODE_SIZE);
    memset(node_t,0,RB_NODE_SIZE);
    node_t->key=first;  node_t->second=second;    node_t->tag=tag;
    rb_link_node(node_t, parent, new_node);
    rb_insert_color(node_t, root);
    return NULL;
}

struct rb_node* int_void_rbsearch(struct rb_root *root,key_type key)
{
    struct rb_node *node=root->rb_node;
    while(node) {

        if(key < node->key)  {
            node = node->rb_left;
        } else if(key > node->key) {
            node = node->rb_right;
        } else
            return node;
    }
    return NULL;
}

void int_void_rbclear(struct rb_root *root)
{
    struct rb_node *node;
    for(node=rb_first(root);node; )
    {
        node=int_void_rberase(root,node);
    }
}

void int_void_rbclear_Ex(struct rb_root *root, void (*freedate)(void *))
{
    if(freedate) {
        struct rb_node *node;
        for(node=rb_first(root);node; )
        {
            freedate(node->data);
            node=int_void_rberase(root,node);
        }
    } else {
        int_void_rbclear(root);
    }
}

void int_void_rbdestroy(struct rb_root *root)
{
    if(!root)
        return ;
    int_void_rbclear(root);
}

void *int_void_rberase_key(struct rb_root *root,key_type key)
{
    struct rb_node *node=root->rb_node;
    while(node) {
        if(key < node->key) {
            node = node->rb_left;
        } else if(key > node->key) {
            node = node->rb_right;
        } else {
            void *pdata=node->data;
            rb_erase(node,root);
            free(node);
            return pdata;
        }
    }
#ifdef XS_CODE_DEBUG
    printf("[2]Key Not Found\n");
#endif
    return NULL;
}

void int_void_rbclear2(struct rb_root *root,void (*func)(void *))
{
    struct rb_node *node;
    void *p;
    for(node=rb_first(root);node; )
    {
        p=node->data;
        node=int_void_rberase(root,node);
        func(p);
    }
}

struct rb_node *int_void_rberase(struct rb_root *root,struct rb_node *node)
{
    //    struct int_void_rbnode *data=container_of(node,struct int_void_rbnode,node);
    if(!node) {
        //WriteLog("rbtree","[1]erase maybe has bug!!");
        return node;
    }
    struct rb_node *tmp_node=rb_next(node);
    rb_erase(node,root);
    free(node);
    node=tmp_node;
    return node;
}


struct rb_node *int_void_rberase_EX(struct rb_root *root,struct rb_node *node_it,void *param,void (*func)(void*,void*))
{
    // struct int_void_rbnode *data=container_of(node_it,struct int_void_rbnode,node);
    //    if(!node_it) {
    //        WriteLog("rbtree","[2]eraseEX maybe has bug!!");
    //        return node_it;
    //    }
    struct rb_node *tmp_node=rb_next(node_it);
    func(node_it,param);//free(data);
    rb_erase(node_it,root);
    node_it=tmp_node;
    return node_it;
}


void int_void_rberase_key_EX(struct rb_root *root,key_type key,void *param,void (*func)(void*,void*))
{
    //    struct rb_node *last=NULL;
    struct rb_node *node=root->rb_node;
    while(node) {
        //        last=node;
        if(key < node->key) {
            node = node->rb_left;
        } else if(key > node->key) {
            node = node->rb_right;
        } else {
            rb_erase(node,root);
            func(node,param);//free(data);
            return ;
        }
    }
      //    char log[128]={0};
     // snprintf(log,128,"[3]Key Not Found %ld\n",key);
     // WriteLog("rbtree",log);
#ifdef XS_CODE_DEBUG
    printf("[3]Key Not Found %ld\n",key);
#endif
}

void *int_void_rberase_key_EX2(struct rb_root *root,key_type key,void *param,void (*func)(void*,void*))
{
    struct rb_node *node=root->rb_node;
    while(node) {
        if(key < node->key) {
            node = node->rb_left;
        } else if(key > node->key) {
            node = node->rb_right;
        } else {
            void *pdata = node->data;
            rb_erase(node,root);
            func(node,param);//free(data);
            return pdata;
        }
    }
    return NULL;
}


struct rb_node *Rbtree_FindLastNode_FollowKey(struct rb_root *root,const u_int32_t key)
{
    struct rb_node *last=NULL;
    struct rb_node *node=root->rb_node;
    while(node) {
        last=node;
        if(key < node->key)  {
            node = node->rb_left;
        } else if(key > node->key) {
            node = node->rb_right;
        } else
            break;//return node;
    }
    return last;
}
