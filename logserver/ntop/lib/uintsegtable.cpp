#include "uintsegtable.h"
#include "xspublicfun.h"

void UintSegTable_Init(struct UintSegTable *Obj)
{
    memset(Obj,0,sizeof(struct UintSegTable));
    InitTrashArray(&Obj->trash);//XsArray_Init(&Obj->Trash,ORDER_ASC,free,NULL);
    pthread_mutex_init(&Obj->cs,NULL);
    rb_root_init(&Obj->m_Table);
}

void UintSegTable_Destroy(struct UintSegTable *Obj)
{
    UintSegTable_Clear(Obj);
    int_void_rbdestroy(&Obj->m_Table);
    ClearTrashArray1(&Obj->trash,free);//XsArray_Destroy(&Obj->Trash);
    pthread_mutex_destroy(&Obj->cs);
}

void UintSegTable_Clear(struct UintSegTable *Obj)
{
    struct uint_seg *pNode;
    struct rb_node *iter;//=rb_first(&Obj->m_Table);
    for(iter=rb_first(&Obj->m_Table);  iter; iter=rb_next(iter)) {
        pNode=(struct uint_seg*)(iter->data);
        if(!pNode)continue;
        iter->data=NULL;

        pthread_mutex_lock(&Obj->cs);
        EnTrashArray1(&Obj->trash,pNode);//XsArray_AddNodeToTail(&Obj->Trash,pNode);
        pthread_mutex_unlock(&Obj->cs);

        pNode=NULL;
    }
}



int UintSegTable_Insert(struct UintSegTable *Obj,u_int32_t ulStartIP,u_int32_t ulEndIP)
{
    if(ulStartIP > ulEndIP)
        return -1;
    struct uint_seg *pNode;
    struct rb_node *last=Rbtree_FindLastNode_FollowKey(&Obj->m_Table,ulEndIP);
    if(last) {
        pNode=(struct uint_seg *)last->data;
        if(pNode) {
            if(ulEndIP <=pNode->ulEndIp) {
                if(ulStartIP >=pNode->ulStartIp) {
                    return 0;
                } else if((ulEndIP +1) >= pNode->ulStartIp) {
                    pNode->ulStartIp = ulStartIP;
                    if(ulStartIP>0) ulStartIP -= 1;
                    last=Rbtree_FindLastNode_FollowKey(&Obj->m_Table,ulStartIP);
                    if(last) {
                        struct uint_seg * pTmp=(struct uint_seg *)last->data;
                        if(pTmp && pTmp->ulEndIp == ulStartIP ) {
                            pNode->ulStartIp = pTmp->ulStartIp;

                            pthread_mutex_lock(&Obj->cs);
                            EnTrashArray1(&Obj->trash,pTmp);//XsArray_AddNodeToTail(&Obj->Trash,pTmp);
                            int_void_rberase_EX(&Obj->m_Table,last,&Obj->trash,EnTrashArray2);
                            pthread_mutex_unlock(&Obj->cs);
                        }
                    }
                    return 0;
                }
            } else if(ulEndIP > pNode->ulEndIp){
                if(ulStartIP <= (pNode->ulEndIp+1)) {
                    if(ulStartIP>pNode->ulStartIp)
                        ulStartIP = pNode->ulStartIp;

                    pthread_mutex_lock(&Obj->cs);
                    EnTrashArray1(&Obj->trash,pNode);//XsArray_AddNodeToTail(&Obj->Trash,pNode);
                    int_void_rberase_EX(&Obj->m_Table,last,&Obj->trash,EnTrashArray2);
                    pthread_mutex_unlock(&Obj->cs);
                }
            }
        }
        pNode=NULL;
    }
    pNode=(struct uint_seg *)malloc(UINTSEG_SIZE);
    memset(pNode,0,UINTSEG_SIZE);
    pNode->ulStartIp = ulStartIP;    pNode->ulEndIp = ulEndIP;

    pthread_mutex_lock(&Obj->cs);
    struct rb_node *iter=int_void_rbinsert(&Obj->m_Table,pNode->ulEndIp,pNode);
    pthread_mutex_unlock(&Obj->cs);
    if(!iter) {
        return 0;
    } else {
        free(pNode);
        return -1;
    }
}

int UintSegTable_Delete(struct UintSegTable *Obj,const u_int32_t ulIP)
{
    struct rb_node *iter= int_void_rbsearch(&Obj->m_Table,ulIP);
    if(!iter)
        return -1;
    if(iter->data) {
        pthread_mutex_lock(&Obj->cs);
        EnTrashArray1(&Obj->trash,iter->data);//XsArray_AddNodeToTail(&Obj->Trash,iter->data);
        pthread_mutex_unlock(&Obj->cs);
        iter->data=NULL;
    }
    pthread_mutex_lock(&Obj->cs);
    int_void_rberase_EX(&Obj->m_Table,iter,&Obj->trash,EnTrashArray2);
    pthread_mutex_unlock(&Obj->cs);
    return 0;
}

int UintSegTable_Delete_Member(struct UintSegTable *Obj,u_int32_t ulStartIP,u_int32_t ulEndIP)
{
    struct rb_node *iter=Rbtree_FindLastNode_FollowKey(&Obj->m_Table,ulEndIP);
    if(!iter)
        return -1;

    struct uint_seg *pNode=(struct uint_seg*)iter->data;

    pthread_mutex_lock(&Obj->cs);
    int_void_rberase_EX(&Obj->m_Table,iter,&Obj->trash,EnTrashArray2);
    pthread_mutex_unlock(&Obj->cs);

    if(pNode) {
        if(ulStartIP!=pNode->ulStartIp || ulEndIP!=pNode->ulEndIp) {
            if( ulStartIP && (ulStartIP >pNode->ulStartIp)) {
                u_int32_t tmp_start=pNode->ulStartIp;
                u_int32_t tmp_end = ulStartIP-1;
                UintSegTable_Insert(Obj,tmp_start,tmp_end);
            }
            if(ulEndIP && (ulEndIP < pNode->ulEndIp)) {
                u_int32_t tmp_start=ulEndIP+1;
                u_int32_t tmp_end=pNode->ulEndIp;
                UintSegTable_Insert(Obj,tmp_start,tmp_end);
            }
        }

        pthread_mutex_lock(&Obj->cs);
        EnTrashArray1(&Obj->trash,pNode);//XsArray_AddNodeToTail(&Obj->Trash,pNode);
        pthread_mutex_unlock(&Obj->cs);
    }

    return 0;
}

void* UintSegTable_Find(struct UintSegTable *Obj,const u_int32_t key)
{
    struct rb_node *last=Rbtree_FindLastNode_FollowKey(&Obj->m_Table,key);

    if(last) {
        struct uint_seg *pNode=(struct uint_seg *)(last->data);
        if( key >=pNode->ulStartIp && key <= pNode->ulEndIp)
            return pNode;
    }
    return NULL;
}


void UintSegTable_ClearTrash(struct UintSegTable *Obj)
{
    pthread_mutex_lock(&Obj->cs);
    ClearTrashArray1(&Obj->trash,free);//XsArray_RemoveAllNode(&Obj->Trash);
    pthread_mutex_unlock(&Obj->cs);
}
