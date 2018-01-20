#include "trasharray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef XSPEEDER_OBJ
#include "xspublicfun.h"
#endif
/*1. 初始化链队列*/
// 其  初始化的 操作就是初始化队列的队头和队尾的两个标志位，


// 所以就有删除或是插入的时候，会判断有没有 队列为空的时候。
void InitTrashArray(trash_array_t * queue_eg)
{
    //    queue_eg->head = NULL; //队头标志位
    //    queue_eg->tail = NULL; //队尾标志位
    //    queue_eg->nums = 0;
    //    queue_eg->nodes = 0;
    memset(queue_eg,0,sizeof(trash_array_t));
}


void DestroyTrashArray(trash_array_t *hq)
{
    trashnode_t * p = hq->head;
    while(p != NULL)
    {
        hq->head = hq->head->next;
        free(p);
        p = hq->head;
    }
    hq->tail = NULL;
    memset(hq,0,sizeof(trash_array_t));
    return;
}

void EnTrashArray2(void *ptr,void *hq){
    trash_array_t *hqueue=(trash_array_t*)hq;
    EnTrashArray1(hqueue,ptr);
}
/*2.向链队的队尾插入一个元素x*/
int EnTrashArray1(trash_array_t *hq, void * x)
{
    if(hq->nodes < hq->nums) {
        if(hq->idle->data) {
#ifdef XSPEEDER_OBJ
            WriteLog("xsroute","bug  enqueue");
#endif
            return -1;
        }
        hq->idle->data=x;
        hq->idle = hq->idle->next;
        hq->nodes++;
    } else {
        trashnode_t * new_p;
        new_p = (trashnode_t *)malloc(sizeof(trash_array_t));
        if (new_p == NULL )  {
#ifdef XSPEEDER_OBJ
            WriteLog("xsroute","分配空间出错！");
#endif
            return -1;
        }
        new_p->data = x;
        new_p->next = NULL;
        if (hq->head == NULL) {
            hq->head = new_p;
            hq->tail = new_p;
        } else {
            //hq->tail->data = x;
            hq->tail->next = new_p;
            hq->tail = new_p;
        }
        hq->nums++;
        hq->nodes++;
        hq->idle = NULL;
    }
    return 0;
}

void ClearTrashArray2(trash_array_t *hq,void (*freec)(void *pdata))
{
    if(0==hq->nodes)
        return;
    trashnode_t * p ;
    trashnode_t *next=hq->head;
    while(next != NULL)
    {
        p = next;
        next = next->next;
        if(p->data) {
            freec(p->data);
            p->data = NULL;
        }
    }

    hq->idle = hq->head;
    hq->nodes=0;
    return;
}

void ClearTrashArray1(trash_array_t *hq,void (*freec)(void *pdata))
{
    trashnode_t * p = hq->head;
    while(p != NULL)
    {
        hq->head = hq->head->next;
        if(p->data) {
            freec(p->data);
            p->data = NULL;
        }
        free(p);
        p = hq->head;
    }
    memset(hq,0,sizeof(trash_array_t));
    return;
}


