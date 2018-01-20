#ifndef TRASHARRAY_H
#define TRASHARRAY_H
#include <stdlib.h>
/**************************/
/*           */
/**************************/
typedef struct trashnodet
{
    void * data;
    struct trashnodet * next;
} trashnode_t;            // 节点的结构

typedef struct trashqueuet
{
    trashnode_t * head;
    trashnode_t * tail;
    trashnode_t *idle;
    u_int nums,nodes;
} trash_array_t;          // 队列的结构

// 所以就有删除或是插入的时候，会判断有没有 队列为空的时候。
void InitTrashArray(trash_array_t * queue_eg);
void DestroyTrashArray(trash_array_t *hq);
/*2.向链队的队尾插入一个元素x*/
int EnTrashArray1(trash_array_t *hq, void * x);
void EnTrashArray2(void *ptr,void *hq);
void ClearTrashArray1(trash_array_t *hq,void (*freec)(void *pdata));
void ClearTrashArray2(trash_array_t *hq,void (*freec)(void *pdata));

#define SIMPLE_Queue    trash_array_t
#define SIMPLE_InitQueue    InitTrashArray
#define SIMPLE_EnQueue  EnTrashArray1
#define SIMPLE_DestroyQueue   ClearTrashArray1


#endif // TRASHARRAY_H

