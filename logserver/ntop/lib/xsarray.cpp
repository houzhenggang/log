#include <string.h>
#include <stdlib.h>
#include "xsarray.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void XsArray_Init(PXsArray pArray,int order,NODEFREEPROC f_free,NODECOMPAREPROC f_cmp)
{
    memset(pArray,0,XSARRAY_SIZE);
    pArray->m_ArrayPointer = (long *)malloc(INI_COUNT*sizeof(long));
    int n;
    for ( n=0 ;n<INI_COUNT;n++)
    {
        pArray->m_ArrayPointer[n]=(long)0;
    }
    pArray->m_nCurCount=0;
    pArray->m_ArraySize=INI_COUNT;
    pArray->m_pCompareProc = f_cmp;
    pArray->m_pFreeProc = f_free;
    if (order!=ORDER_ASC&&order!=ORDER_DES&&order!=NO_ORDER)
        order= ORDER_ASC;
    pArray->m_nOrder = order;
}

void XsArray_Destroy(PXsArray pArray)
{
    if (pArray->m_ArrayPointer)
    {
        int i ;
        for (i=0 ;i<pArray->m_nCurCount;i++)
        {
            void *pNode=XsArray_GetNode(pArray,i);
            if (pNode&&pArray->m_pFreeProc)
            {
                pArray->m_pFreeProc(pNode);
            }
        }
        free(pArray->m_ArrayPointer);
    }
}

bool XsArray_AddNodeToTail(PXsArray pArray,void *pNode)
{
    if (!pNode)
        return 0;

    if (!pArray->m_ArrayPointer)
        return 0;

    if (pArray->m_nCurCount>pArray->m_ArraySize)
        return 0;

    if (pArray->m_nCurCount==pArray->m_ArraySize)
    {
        pArray->m_ArraySize = pArray->m_ArraySize+INI_COUNT;
        pArray->m_ArrayPointer = (long *)realloc((void *)pArray->m_ArrayPointer,pArray->m_ArraySize*sizeof(long));
        if (!pArray->m_ArrayPointer)
            return 0;
        int n;
        for (n=pArray->m_nCurCount;n<pArray->m_ArraySize;n++)
        {
            pArray->m_ArrayPointer[n]=(long)0;
        }
    }

    pArray->m_ArrayPointer[pArray->m_nCurCount]=(long)pNode;
    pArray->m_nCurCount++;
    pArray->m_nOrder=NO_ORDER;//自动变为无序数组

    return 1;
}

bool XsArray_AddNodeByOrder(PXsArray pArray,void *pNode)
{
    if (pArray->m_nOrder==NO_ORDER)
    {
        return XsArray_AddNodeToTail(pArray,pNode);
    }

    if (!pNode)
        return 0;

    if (!pArray->m_pCompareProc)
        return 0;

    if (!pArray->m_ArrayPointer)
        return 0;

    if (pArray->m_nCurCount>pArray->m_ArraySize)
        return 0;

    if (pArray->m_nCurCount==pArray->m_ArraySize)
    {
        pArray->m_ArraySize = pArray->m_ArraySize+INI_COUNT;
        pArray->m_ArrayPointer = (long *)realloc((void *)pArray->m_ArrayPointer,pArray->m_ArraySize*sizeof(long));
        if (!pArray->m_ArrayPointer)
            return 0;
        int n;
        for (n=pArray->m_nCurCount;n<pArray->m_ArraySize;n++)
        {
            pArray->m_ArrayPointer[n]=(long)0;
        }
    }

    int nIndex=XsArray_GetInsterPos(pArray,pNode);

    if (nIndex!=pArray->m_nCurCount)
    {
        int i;
        for (i=pArray->m_nCurCount;i>nIndex;i--)
        {
            pArray->m_ArrayPointer[i]=pArray->m_ArrayPointer[i-1];
        }
        pArray->m_ArrayPointer[nIndex]=(long)pNode;
    }
    else
    {
        pArray->m_ArrayPointer[pArray->m_nCurCount]=(long)pNode;
    }

    pArray->m_nCurCount++;

    return 1;
}


bool XsArray_InsertNode(PXsArray pArray,void *pNode,int nIndex)
{
    if (!pNode)
        return 0;

    if (!pArray->m_ArrayPointer)
        return 0;

    if (pArray->m_nCurCount>pArray->m_ArraySize)
        return 0;

    if (pArray->m_nCurCount==pArray->m_ArraySize)
    {
        pArray->m_ArraySize = pArray->m_ArraySize+INI_COUNT;
        pArray->m_ArrayPointer = (long *)realloc((void *)pArray->m_ArrayPointer,pArray->m_ArraySize*sizeof(long));
        if (!pArray->m_ArrayPointer)
            return 0;
        int n;
        for ( n=pArray->m_nCurCount;n<pArray->m_ArraySize;n++)
        {
           pArray->m_ArrayPointer[n]=(long)0;
        }
    }


    if (nIndex!=pArray->m_nCurCount)
    {
        int i;
        for (i=pArray->m_nCurCount;i>nIndex;i--)
        {
            pArray->m_ArrayPointer[i]=pArray->m_ArrayPointer[i-1];
        }
        pArray->m_ArrayPointer[nIndex]=(long)pNode;
    }
    else
    {
        pArray->m_ArrayPointer[pArray->m_nCurCount]=(long)pNode;
    }

    pArray->m_nCurCount++;

    return 1;
}

bool XsArray_DelNode(PXsArray pArray,int nIndex)
{
    if (nIndex<0||nIndex>=pArray->m_nCurCount)
        return 0;

    if (!pArray->m_ArrayPointer)
        return 0;

    void *pNode=XsArray_GetNode(pArray,nIndex);
    if (pNode&&pArray->m_pFreeProc)
    {
        pArray->m_pFreeProc(pNode);
    }
    int i;
    for (i=nIndex;i<pArray->m_nCurCount-1;i++)
    {
        pArray->m_ArrayPointer[i]=pArray->m_ArrayPointer[i+1];
    }
    pArray->m_ArrayPointer[pArray->m_nCurCount-1]=(long)0;
    pArray->m_nCurCount--;
    return 1;
}

bool XsArray_RemoveAllNode(PXsArray pArray)
{
    if (pArray->m_nCurCount==0)
        return 1;

    if (!pArray->m_ArrayPointer)
        return 0;

    int i;
    for (i=0;i<pArray->m_nCurCount;i++)
    {
        void *pNode=XsArray_GetNode(pArray,i);
        if (pNode&&pArray->m_pFreeProc)
        {
            pArray->m_pFreeProc(pNode);
        }
        pArray->m_ArrayPointer[i]=(long)0;
    }

    free(pArray->m_ArrayPointer);

    pArray->m_nCurCount=0;
    pArray->m_ArrayPointer = (long *)malloc(INI_COUNT*sizeof(long));
    memset(pArray->m_ArrayPointer,0,INI_COUNT*sizeof(long));
    pArray->m_ArraySize=INI_COUNT;
    pArray->m_nOrder = ORDER_ASC;

    return 1;
}

void* XsArray_GetNode(PXsArray pArray,int nIndex)
{
    if (nIndex<0||nIndex>=pArray->m_nCurCount)
        return NULL;

    if (!pArray->m_ArrayPointer)
        return NULL;

    return (void *)pArray->m_ArrayPointer[nIndex];
}


int XsArray_GetArrayCount(PXsArray pArray)
{
    return pArray->m_nCurCount;
}


void XsArray_SetOrder(PXsArray pArray,int nOrder/*,bool bSort*/)
{
    bool bSort=true;
    if (nOrder!=ORDER_ASC&&nOrder!=ORDER_DES&&nOrder!=NO_ORDER)
        return ;

    if (nOrder==NO_ORDER)
    {
        pArray->m_nOrder = nOrder;
        return ;
    }

    if (pArray->m_nCurCount==0||!pArray->m_ArrayPointer)
    {
        pArray->m_nOrder = nOrder;
        return ;
    }


    if (pArray->m_nOrder!=nOrder&& pArray->m_pCompareProc&&bSort)
    {
        if (nOrder==ORDER_ASC)
        {
            XsArray_QuickSort(pArray,0,pArray->m_nCurCount-1,1);
        }
        else if (nOrder==ORDER_DES)
        {
            XsArray_QuickSort(pArray,0,pArray->m_nCurCount-1,0);
        }
    }

    pArray->m_nOrder = nOrder;
}

void* XsArray_FindData(PXsArray pArray,void *pNode,int* nIndex)
{
    (*nIndex) = -1;
    if (!pNode||!pArray->m_ArrayPointer)
    {
        return NULL;
    }

    if (pArray->m_nOrder==NO_ORDER)
    {
        int n;
        for (n=0;n<pArray->m_nCurCount;n++)
        {
            if (pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[n],pNode)==0)
            {
                 (*nIndex)  = n;
                return (void *)pArray->m_ArrayPointer[n];
            }
        }
    }
    else
    {
        int nHead=0;
        int nTail=pArray->m_nCurCount-1;
        int nMid=0;
        while (nHead<=nTail)
        {
            nMid=(int)((nTail+nHead)/2);

            int nRet=pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nMid],pNode);
            if (pArray->m_nOrder==ORDER_ASC)
            {
                if (nRet==0)
                {
                     (*nIndex) =nMid;
                    return (void *)pArray->m_ArrayPointer[nMid];
                }
                else if (nRet<0)
                {
                    nHead=++nMid;
                    continue;
                }
                else if (nRet>0)
                {
                    nTail=--nMid;
                    continue;
                }
            }
            else if (pArray->m_nOrder==ORDER_DES)
            {
                if (nRet==0)
                {
                     (*nIndex) =nMid;
                    return (void *)pArray->m_ArrayPointer[nMid];
                }
                else if (nRet>0)
                {
                    nHead=++nMid;
                    continue;
                }
                else if (nRet<0)
                {
                    nTail=--nMid;
                    continue;
                }
            }
        }
    }

     (*nIndex) =-1;
    return NULL;

}


//二分查找插入位置
int XsArray_GetInsterPos(PXsArray pArray,void *pNode)
{
    if (pArray->m_nOrder==NO_ORDER)
        return pArray->m_nCurCount;

    if (pArray->m_nCurCount==0)
        return pArray->m_nCurCount;

    int nHead=0;
    int nTail=pArray->m_nCurCount-1;
    int nMid=0;
    int nLast=-1;

    while (nHead<=nTail)
    {
        nMid=(int)((nTail+nHead)/2);
        if (nLast==nMid)
        {
            int nRet1=pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nHead],pNode);
            int nRet2=pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nTail],pNode);
            if (pArray->m_nOrder==ORDER_ASC)
            {
                if (nRet2<=0)
                {
                    nTail++;
                    return nTail;
                }
                else if (nRet1<=0)
                {
                    nHead++;
                    return nHead;
                }
                else if (nRet1>0)
                    return nHead;
            }
            else if (pArray->m_nOrder==ORDER_DES)
            {
                if (nRet1<=0)
                    return nHead;
                else if (nRet2<=0)
                    return nTail;
                else if (nRet2>0)
                {
                    nTail++;
                    return nTail;
                }
            }
        }

        nLast=nMid;
        int nRet=pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nMid],pNode);
        if (pArray->m_nOrder==ORDER_ASC)
        {
            if (nRet==0)
                return nMid;
            else if (nRet<0)
            {
                nHead=nMid;
                continue;
            }
            else if (nRet>0)
            {
                nTail=nMid;
                continue;
            }
        }
        else if(pArray->m_nOrder==ORDER_DES)
        {
            if (nRet==0)
                return nMid;
            else if (nRet>0)
            {
                nHead=nMid;
                continue;
            }
            else if (nRet<0)
            {
                nTail=nMid;
                continue;
            }
        }
    }

    return pArray->m_nCurCount;
}


void XsArray_QuickSort(PXsArray pArray,int nStart, int nEnd,bool bAsc)
{
    if (nStart<0||nEnd<0||nStart>=nEnd)
        return;

    if (!pArray->m_pCompareProc)
        return ;

    int nHead=nStart;
    int nTail=nEnd;
    long nSave=pArray->m_ArrayPointer[nHead];
    if (bAsc)
    {
        do
        {
            while (nHead<nTail&& pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nTail],(void *)nSave)>=0)
            {
                nTail--;
            }
            if (nHead<nTail)
            {
                pArray->m_ArrayPointer[nHead]=pArray->m_ArrayPointer[nTail];
                nHead++;
            }
            while (nHead<nTail&& pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nHead],(void *)nSave)<=0)
            {
                nHead++;
            }
            if (nHead<nTail)
            {
                pArray->m_ArrayPointer[nTail]= pArray->m_ArrayPointer[nHead];
                nTail--;
            }
        }while (nHead<nTail);
        pArray->m_ArrayPointer[nHead]=nSave;
        XsArray_QuickSort(pArray,nStart,nHead-1,bAsc);
        XsArray_QuickSort(pArray,nHead+1,nEnd,bAsc);
    }
    else
    {
        do
        {
            while (nHead<nTail&& pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nTail],(void *)nSave)<=0)
            {
                nTail--;
            }
            if (nHead<nTail)
            {
                pArray->m_ArrayPointer[nHead]=pArray->m_ArrayPointer[nTail];
                nHead++;
            }
            while (nHead<nTail&& pArray->m_pCompareProc((void *)pArray->m_ArrayPointer[nHead],(void *)nSave)>=0)
            {
                nHead++;
            }
            if (nHead<nTail)
            {
                pArray->m_ArrayPointer[nTail]=pArray->m_ArrayPointer[nHead];
                nTail--;
            }
        }while (nHead<nTail);
        pArray->m_ArrayPointer[nHead]=nSave;
        XsArray_QuickSort(pArray,nStart,nHead-1,bAsc);
        XsArray_QuickSort(pArray,nHead+1,nEnd,bAsc);
    }
}

