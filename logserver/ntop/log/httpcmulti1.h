#ifndef HTTPCMULTI1_H
#define HTTPCMULTI1_H


#ifndef HTTPCLIENTMULTI_H
#define HTTPCLIENTMULTI_H

#include "curl/curl.h"
#include <string>
#include <set>
#include <map>
#include <queue>

using namespace std;

#include <pthread.h>
//#include "dw_struct.h"
#include "lib/xs_mutex.h"
#define DEFAULT_NEW_SIZE 1048576
//2097152
//(1024*1024*2)
//unsigned char define max 255
#define MAX_HTTP_DOWNLOAD 10
#define MAX_DF_LIST 900
//#include "sqlitedeturl.h"


typedef struct _CacheFile {
    char szFileName[64];
    char szSaveDir[64];
    char szURI[1500];
    //update 20170704//dpi max len 1800
    //    char szField[1800];//cookie
    int nFieldLen;
    char szHost[64];
    char szReferer[128];
    char szCookie[512];
    char szUserAgent[64];
    //char szHashCode[41];//站点hash
    char szFileHashCode[41];//文件hash
    char szRange[100];
    u_char cReTry;
    u_char cReQiyi;
    //char szCRC64[21];//文件hash
    u_int64_t nCRC64;//文件hash
    unsigned int lSIP;
    int  nGetType;  //获取方式 1：还原 2：下载  3：推送 4:p2p推送
    unsigned long lFileSize;
    unsigned long lCacheSize;
    //    unsigned int lCurSize;  //用来计算速度，每秒计算一次，计算完后清零
    //    void    *pLinkNode;
    //    PWEBSITEEX pWebSite;
    //    time_t time;
    int    nStatus;  //-1:退出 0:待处理  1：正在处理  2:处理完成  9:还原失败
    //bool   bPush;    //false:非推送数据 1：推送数据
    //    int    nClickHot;  //点击
    int    nCacheType;
    int nRuleID;
    //    PCACHETSFILE pTS;//ts file 特殊处理
    //CDataArray   *pArIP;
    //unsigned int nNextSeq;
#ifdef SAVE_HTTP_HRL
    int nRuleID;
    char szOriGet[800];
    /*~_CacheFile()
    {
        usleep(1);
    }
*/
#endif
    //    friend bool operator <(const _CacheFile &a,const _CacheFile &b)
    //    {
    //        if(a.nClickHot>b.nClickHot)return true;
    //        if(a.time<b.time)return true;
    //        else
    //            return memcmp(a.szFileName,b.szFileHashCode,40);
    //    }

}CACHEFILE,*PCACHEFILE;
#define CACHEFILESIZE sizeof(CACHEFILE)





typedef struct stDownFileInfo
{
    CURL* p;
    ulong ulSTime;//start download time
    ulong ulRecvTime;//last recv data time
    uint nid;
    short nType;//-1:filethesame,0:OK,1:Start,2:Error Exit,3:head,4:302,5:open file failed,6:http failed,7:timeout300,8:checkfailed,9:filter,10:check failed
    PCACHEFILE pdf;
    int res_code;//respance code
    int nhsize;//respance size
    int nHeadSize;//respance head size
    ulong Content_Length;
    bool bChunked;
    uint nCKlen;

    string strPath;
    string strHead;
    string strLocation;
    int fio;
    ulong ulWriteLength;
    //char* pData;//default new 5M
    int nDataLen;
    char pData[DEFAULT_NEW_SIZE];
    int setspeed;
    double speed;
    double dltotal;
    double dlnow;
    float progress;
    short nLocation;
    CACHEFILE chefile;
    u_int nlastIP;
    u_short nLastPort;
    curl_slist *listKA;
    void* user;
}DownFileInfo,*PDOWNFILEINDO;

typedef map<PCACHEFILE,string> DownURLList;//url->file
typedef map<CURL*,PDOWNFILEINDO> DownFileList;
typedef set<PDOWNFILEINDO> EndList;

class HttpCDown;
class HttpCMulti
{

public:
    HttpCMulti(HttpCDown& phcd);
    ~HttpCMulti();
    //
    int m_threadid;
    pthread_mutex_t m_mtx;//
    pthread_mutex_t* m_pmtx;//
public:
    bool isDownList(void* curl);
    bool SaveKeyFrame(const char *pFileName,unsigned long dwFileSize);
    int End(PDOWNFILEINDO pDown);
    int RemoveEnd();
    void CheckDone();
    int RemoveFailed(PCACHEFILE p);
    int RemoveTimeOut();

    int GetSavePath(PCACHEFILE p,char* path,bool buc = true);
    int AddNode(PCACHEFILE p);
    int AddUrl(PCACHEFILE p,string strSPath);
    //    int AddUrl(const char* pURL,int nUrlLen,const char* pSPath,int nSPathLen);
    //    int AddUrlHead(const char* pURL,int nUrlLen);
    int Run(int type = 0);
    int Wait();
    int URL2CURL(int addcount);
    int ReadTSURL(string& strUrl,string& strSPath,PCACHEFILE pdf);
    int PrintfSpeed(int setspeed);
    void RestoreSQL(DownFileInfo& fDown);
    //downlist
    void AddDownUrl(PDOWNFILEINDO p);
    void UpdateDownUrl(PDOWNFILEINDO p);
    void EndDownUrl(PDOWNFILEINDO p);

    int size(){return  (m_downlst.size()+m_downURL.size());}
    void SetSpeed(int speed){m_setspeed = speed;}
    int downsize(){return m_downlst.size();}
    int downqueuesize(){return m_downURL.size();}
protected:
    HttpCDown &m_hcd;//管理资源入口
    pthread_t m_thread = 0;
    int m_bDebug;

    DownURLList m_downURL;//URL排队中
    //CURLM* m_curlm;
    CURLM* m_multi_handle;
    DownFileList m_downlst;//下载队列
    EndList m_EndLst;//zhongzhuan ,非，加锁定，可能需要
    //DownFileList m_comlst;//移除的下载后的队列

    pthread_mutex_t m_cs;//m_downURL

    //
    uint m_In = 0;
    uint m_Out = 0;
    uint m_Run = 0;
    int m_setspeed = 1024*1024*5;
    u_char m_csetspeed = 0;

};

inline int CompareHttpCNode(const void *pNode1,const void *pNode2)
{
    if(pNode2==NULL || pNode1==NULL)
        return 1;
    return ((HttpCMulti*)pNode1)->size()-((HttpCMulti*)pNode2)->size();
}

typedef map<u_int64_t,PCACHEFILE> DownFileOfint64;
class HttpCDown
{

public:
    HttpCDown();
    ~HttpCDown();

    int Init(int ndownthread = 2);
    int InitSpeed(int limitspeed[7][24]);//
    int SetSpeed(struct tm& st);
    int DelFailedDown();

    int addFailedLst(PCACHEFILE p);
    int addInCurlLst(PCACHEFILE p);
    int addInCurlLst_TS(PCACHEFILE p);
    int AddUrl(PCACHEFILE p);
    int Add(PCACHEFILE p);
    //int AddTS(PCACHEFILE p);

    void AutoClearList();//auto clear list
    void TestAdd();
    void Data2list();
    PCACHEFILE Get();

    void DownCom(PDOWNFILEINDO pDown);
    void EndofDownLoad();
    void earsedflstNode(u_int64_t & ncrc64);

public:
    void AddDownCount(int & nRID,char* pName);
    void AddDownStatus(int & nRID,int nStatus);
    void UpdateDB();
    void UpdateWaiteDown();
    void PrintfDown();
public:
    int m_limitspeed[7][24];//={0};
    bool bReSetSpeed = false;
    pthread_t m_thread = 0;
    //    CDataArray m_arDown;

    xs_mutex mutex_Queue;
    queue<PCACHEFILE> m_Queuelst;

    xs_mutex mutex_dflist;
    //    downloadlst m_dflstint64;//all down list//下载中

    queue<PCACHEFILE> m_dellst;//delete failed
    xs_mutex mutex_del;
    DownFileList m_comlst;//移除的下载后的队列

    //    FILENODE m_newfn;
public:
    //    httpDetQueue m_httphitqueue;
    //map<u_int64_t,

    sqlitedeturl m_deturl;
    //    map<int,DownLoadCnt> m_downcnt;
    ulong ulWaitTime = 0;
    u_char m_cupdateDLtype = 0;
    ulong ulUpdatetime = 0;
};

#endif // HTTPCLIENTMULTI_H



#endif // HTTPCMULTI1_H
