#include "xs_list.h"
#include <string.h>
#include <lib/xspublicfun.h>
#include "lib/xs_prediction.h"

void Xs_List_Init(PXS_LIST pXsList)
{
    memset(pXsList,0,XS_LIST_T_SIZE);
    xs_mutex_init(&pXsList->mutex);
}

void Xs_List_Destroy(PXS_LIST pXsList,void (*freec)(void *pdata))
{
    listnode_t * p = pXsList->head;
    while(p != NULL)
    {
        pXsList->head = pXsList->head->next;
        if(p->data) {
            freec(p->data);
            p->data = NULL;
        }
        free(p);
        p = pXsList->head;
    }
    memset(pXsList,0,XS_LIST_T_SIZE);
    return;
}

u_int Xs_List_Node_Size(PXS_LIST pXsList)
{
    return pXsList->nodes;
}

int Xs_List_sp_PushList(PXS_LIST pXsList,void *x)
{
    if(unlikely(pXsList->nodes < pXsList->nums)) {
        if(pXsList->idle->data) {
#ifdef XSPEEDER_OBJ
            WriteLog("xslist","bug  PushList");
#endif
            return -1;
        }
        pXsList->idle->data=x;
        pXsList->idle = pXsList->idle->next;
        pXsList->nodes++;
    } else {
        listnode_t * new_p;
        new_p = (listnode_t *)malloc(sizeof(listnode_t));
        if (unlikely(NULL==new_p ))  {
#ifdef XSPEEDER_OBJ
            WriteLog("xslist","分配空间出错！");
#endif
            return -1;
        }
        new_p->data = x;
        new_p->next = NULL;
        if (NULL==pXsList->head) {
            pXsList->head = new_p;
            pXsList->tail = new_p;
        } else {
            pXsList->tail->next = new_p;
            pXsList->tail = new_p;
        }
        pXsList->nums++;
        pXsList->nodes++;
        pXsList->idle = NULL;
    }
    return 0;
}


int Xs_List_mp_PushList(PXS_LIST pXsList,void *x)
{
    int ret;
    xs_mutex_lock(&pXsList->mutex);
    ret = Xs_List_sp_PushList(pXsList,x);
    pXsList->nRecvPacket ++;
    xs_mutex_unlock(&pXsList->mutex);
    return ret;
}

int Xs_List_zero_ParseNum(PXS_LIST pXsList)
{
    xs_mutex_lock(&pXsList->mutex);
    pXsList->nRecvPacket = 0;
    pXsList->nParsePacket = 0;
    xs_mutex_unlock(&pXsList->mutex);
}

void *Xs_List_sc_POPList(PXS_LIST pXsList)
{
    listnode_t * p;
    void *pdata=NULL;

    p = pXsList->head;
    if(p){
        pXsList->head = pXsList->head->next;
        pdata = p->data;
        free(p);
    }

    return pdata;
}

void *Xs_List_mc_POPList(PXS_LIST pXsList)
{
    listnode_t * p;
    void *pdata=NULL;

    xs_mutex_lock(&pXsList->mutex);
    p = pXsList->head;
    if(p){
        pXsList->head = pXsList->head->next;
        pXsList->nParsePacket++;
        xs_mutex_unlock(&pXsList->mutex);
        pdata = p->data;
        free(p);
    } else {
        xs_mutex_unlock(&pXsList->mutex);
    }

    return pdata;
}
