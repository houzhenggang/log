#ifndef XS_LIST_H__
#define XS_LIST_H__
#include <stdlib.h>
#include "lib/xs_mutex.h"
typedef struct listnodet
{
    void * data;
    struct listnodet * next;
} listnode_t;            // 节点的结构

typedef struct xs_list
{
    xs_mutex mutex;
    listnode_t * head;
    listnode_t * tail;
    listnode_t *idle;
    u_int nums,nodes;
    unsigned long long nParsePacket; //five second parse packet
    unsigned long long nRecvPacket; //five second recv packet
} XS_LIST,*PXS_LIST;          // 队列的结构
#define XS_LIST_T_SIZE      sizeof(XS_LIST)


void Xs_List_Init(PXS_LIST pXsList);
void Xs_List_Destroy(PXS_LIST pXsList,void (*freec)(void *pdata));

u_int Xs_List_Node_Size(PXS_LIST pXsList);
int Xs_List_sp_PushList(PXS_LIST pXsList,void *x);
int Xs_List_mp_PushList(PXS_LIST pXsList,void *x);
int Xs_List_zero_ParseNum(PXS_LIST pXsList);


void *Xs_List_sc_POPList(PXS_LIST pXsList);
void *Xs_List_mc_POPList(PXS_LIST pXsList);
#endif // XS_LIST_H__

