#ifndef UINTSEGTABLE_H
#define UINTSEGTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <lib/rbtree_callback.h>rbtree_callback.h>
#include <lib/xsarray.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "xsbool.h"
#include "lib/trasharray.h"

struct uint_seg
{
    u_int32_t ulStartIp;//host bit
    u_int32_t ulEndIp;//host bit ......rbtree key,don`t change is after insert into rbtree
};
#define UINTSEG_SIZE      sizeof(struct uint_seg)


struct UintSegTable
{
    struct rb_root m_Table;
    //XsArray Trash;
    trash_array_t trash;
    pthread_mutex_t cs;
};

void UintSegTable_Init(struct UintSegTable *Obj);
void UintSegTable_Clear(struct UintSegTable *Obj);
void UintSegTable_Destroy(struct UintSegTable *Obj);

int UintSegTable_Insert(struct UintSegTable *Obj,u_int32_t ulStartIP,u_int32_t ulEndIP);
int UintSegTable_Delete(struct UintSegTable *Obj,const u_int32_t ulIP);
int UintSegTable_Delete_Member(struct UintSegTable *Obj,u_int32_t ulStartIP,u_int32_t ulEndIP);


void* UintSegTable_Find(struct UintSegTable *Obj,const u_int32_t key);
void UintSegTable_ClearTrash(struct UintSegTable *Obj);


#endif // UINTSEGTABLE_H

