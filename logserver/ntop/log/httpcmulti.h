#ifndef HTTPCLIENTMULTI_H
#define HTTPCLIENTMULTI_H

#include "curl/curl.h"
#include <string>
#include <set>
#include <map>
#include "include/Flow.h"
using namespace std;

#include <pthread.h>

#define DEFAULT_NEW_SIZE (1024*1024*5)

typedef struct _HttpCacheFile {
    u_int64_t hashkey;//File Hash
    string szURL;
    string szPath;
    char szHost[64];
    char szReferer[128];
    char szCookie[512];
    char szUserAgent[64];
}HTTPCACHEFILE,*PHTTPCACHEFILE;
#define CACHEFILESIZE sizeof(HTTPCACHEFILE)

typedef struct stDownFileInfo
{
    ulong ulSTime;//start download time
    ulong ulRecvTime;//last recv data time
    uint nid;//serial numbe
    u_short nType;//0:OK,1:Start,2:Error Exit,3:head,4:302
    CURL* p;
    int res_code;//responce code
    int nhsize;//responce size
    int nHeadSize;//respance size
    ulong Content_Length;
    bool bChunked;
    string strPath;//文件保存路径
    string strHead;
    int fio;
    ulong ulWriteLength;
    char* pData;//default new 5M
    int nDataLen;
    double speed;
    double dltotal;
    double dlnow;
    float progress;
    curl_slist *listKA;
    void* user;
}DownFileInfo,*PDOWNFILEINDO;

typedef map<CURL*,DownFileInfo> DownFileList;
typedef set<PDOWNFILEINDO> EndList;
typedef map<string,HTTPCACHEFILE> DownURLList;//

class HttpCMulti
{
public:
    HttpCMulti();
    ~HttpCMulti();
    //
    int m_threadid;

    int End(PDOWNFILEINDO pDown);
    int RemoveEnd();
    //    int AddUrl(const char* pURL,int nUrlLen,const char* pSPath,int nSPathLen);
    //    int AddUrlHead(const char* pURL,int nUrlLen);
    int AddUrl(PHTTPCACHEFILE p);

    int Run(int type = 0);
    int Wait();
    int URL2CURL(int addcount);
    int PrintfSpeed(int setspeed);
protected:
    pthread_t m_thread;
    int m_bDebug;

    DownURLList m_downURL;
    //CURLM* m_curlm;
    CURLM* m_multi_handle;
    DownFileList m_downlst;//下载队列
    EndList m_EndLst;//zhongzhuan ,非，加锁定，可能需要
    DownFileList m_comlst;//移除的下载

    pthread_mutex_t m_cs;
    //
    uint m_In;
    uint m_Out;
    uint m_Run;
    //

};

void XsHttp_Add_Down_URL(Flow *pFlow);

#endif // HTTPCLIENTMULTI_H
