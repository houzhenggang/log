#ifndef ELASTICSEARCH_H
#define ELASTICSEARCH_H

#include"ntop_includes.h"

#define NUM_MAX_CURL_ES  8

enum logtype{
    Type_URL=0,
    Type_DNS,
    Type_ID,
    Type_FileDw,
    Type_Game,
    Type_Redirect,//redirectlog
    Type_Oth,//(QQ/WeChat/WangWang/WeiBo/Sina...)
    Type_ProTo,//(L7 protocol)
    Type_PicFile,//picture file
    Type_PhoneMsg,//
    Type_Last
};

typedef struct{
    enum logtype  index;
    char name[16];
    int level;
}TypeName,*PTypeName;

typedef struct _mmapInfo
{
    char* pMmap;
    int nLen;
    int fd;
    char szFileName[256];
    int nNum;
    time_t t1;
}MmapInfo,*PMmapInfo;


typedef struct _esfileinfo
{
    int nNum;
    int type;
}ESfileInfo,*PESfileInfo;

typedef struct _workes{
    unsigned long num_elems;//
    struct string_list *head, *tail;
    struct string_list *cursor;
    char g_es_type[16];//_type
    int level;//
}WorkEs,*PWorkEs;
#define WORKES_SIZE sizeof(WorkEs)\

class ES {
public:
    WorkEs work_es[Type_Last];
    MmapInfo mmapInfo[Type_Last];
    Mutex listMutex;
//    bool reportDrops;
    u_int32_t elkDroppedFlowsQueueTooLong;
    u_int64_t elkExportedFlows, elkLastExportedFlows;

public:
    ES();
    ~ES();

    void XsPostEsData(PESfileInfo info);
    void indexESdata(logtype flag, char *postbuf, unsigned long len);
    int CreateMmapInfo(MmapInfo* info,int index);
    int CloseMmapInfo(MmapInfo* info,int index);
    void startEsDump();
    int sendToES(char* msg, int typeindex, char *DataBuf);
    void pushEStemplate();
    void updateStats(const struct timeval *tv) ;
    int dumpEsFlowDpi(PParseFiled pParseFiled, pAccountInfo accountInfo, pPhoneMsg phoneMsg);
    int XsDealHttpJsonData(PWorkEs work_es, char *postbuf, size_t postbuflen, u_int min_buf_size, int index);
};

int UpdateProtocolex(PPROTO pProto,char *pTag,FILE* fp);
void DeleteSpecialProcotol(char * pTag);
void XsDeleteIndexForProtocol();
void XsPostInterfaceToES(char *ifname,int ifid);


#endif // ELASTICSEARCH_H
