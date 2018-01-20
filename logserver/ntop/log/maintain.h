#ifndef MAINTAIN_H
#define MAINTAIN_H
#include"ntop_includes.h"

struct _diskinfo;
typedef struct _diskinfo
{
    char szName[16];
    char szMountPath[256];
    char szUUID[56];
    char szType[12];
    char szLabel[32];
    char szDeviceModel[64];
    char szTotalSize[16];
    char szUseSize[16];
    bool bSystem;
    void* pData;
}DiskInfo,*PDiskInfo;

void GetDiskInfo();
void *MainTain_Thread(void *p);
void MyDBOp_Clear_Interface_Info(/*PMYDBOP pMyDB*/);
#endif // MAINTAIN_H
