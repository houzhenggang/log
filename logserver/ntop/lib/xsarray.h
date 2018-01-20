#ifndef XSARRAY_H_
#define XSARRAY_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "xsbool.h"



typedef int  (* NODECOMPAREPROC) (const void *,const void *);
typedef void  (* NODEFREEPROC) (void *);

#define  INI_COUNT 128 //初始数组大小
#define ORDER_ASC 1  //升序
#define ORDER_DES 2  //降序
#define NO_ORDER  3  //乱序

typedef struct _xs_array
{
    long *m_ArrayPointer;//放置结构指针的数组的首地址
    int m_nCurCount;//当前数组中放置的结构体数量
    int m_ArraySize;//当前数组被分配的空间
    int m_nOrder;//1:升序 2 降序 3乱序

    NODECOMPAREPROC m_pCompareProc;
    NODEFREEPROC m_pFreeProc;
}XsArray,*PXsArray;
#define XSARRAY_SIZE    sizeof(XsArray)


void XsArray_Init(PXsArray pArray,int order,NODEFREEPROC f_free,NODECOMPAREPROC f_cmp);
void XsArray_Destroy(PXsArray pArray);

bool XsArray_AddNodeToTail(PXsArray pArray,void *pNode);
bool XsArray_AddNodeByOrder(PXsArray pArray,void *pNode);
bool XsArray_InsertNode(PXsArray pArray,void *pNode,int nIndex);


void* XsArray_GetNode(PXsArray pArray,int nIndex);
bool XsArray_DelNode(PXsArray pArray,int nIndex);
bool XsArray_RemoveAllNode(PXsArray pArray);
bool XsArray_RemoveAllNode2(PXsArray pArray);
int XsArray_GetArrayCount(PXsArray pArray);
void XsArray_SetOrder(PXsArray pArray,int nOrder/*,bool bSort=true*/);
void XsArray_QuickSort(PXsArray pArray,int nStart, int nEnd,bool bAsc);

void* XsArray_FindData(PXsArray pArray,void *pNode,int* nIndex);
//二分查找插入位置
int XsArray_GetInsterPos(PXsArray pArray,void *pNode);

#endif // XSARRAY_H_

