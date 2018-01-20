#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "ntop_includes.h"
#include"lib/xshash.h"
#include"lib/xssharefunc.h"



#define CurlSaveDir "/opt/xslog/CurlFileDn"
#define CurlSaveDirQqPic "/opt/xslog/CurlFileDn/%s/qq/picture"
#define CurlSaveDirQqFile"/opt/xslog/CurlFileDn/%s/qq/file"
#define CurlSaveDirQqVedio "/opt/xslog/CurlFileDn/%s/qq/vedio"
#define CurlSaveDirWeChatPic "/opt/xslog/CurlFileDn/%s/wechat/picture"
#define CurlSaveDirWeChatVedio "/opt/xslog/CurlFileDn/%s/wechat/vedio"

//curl_multi_exec
//curl_multi_socket()
//curl_multi_perform()
//curl_multi_fdset()
int assetsManagerProgressFunc(void *clientp,
                              double dltotal,
                              double dlnow,
                              double ultotal,
                              double ulnow)
{
    PDOWNFILEINDO pDown = (PDOWNFILEINDO)clientp;
    if(pDown)
    {
        pDown->dltotal = dltotal;
        pDown->dlnow = dlnow;
        pDown->progress = dlnow*100/dltotal;
        //printf("id:%u,%f\n",pDown->nid,pDown->progress);
    }
    return 0;
}
/*buffer(指向cURL返回的数据的指针),size(cURL返回数据的单个的大小)，nmemb(cURL返回数据的总个数)，
stream(一个文件指针，将返回的数据指向某个文件中，这个参数由CURLOPT_WRITEDATA定义*/
static size_t cb_hcmulti_Data(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    PDOWNFILEINDO pDown = (PDOWNFILEINDO)lpVoid;
    if(pDown)
    {
        pDown->ulRecvTime =ntop->g_xstime;
        if(pDown->res_code==0)
        {
            curl_easy_getinfo(pDown->p, CURLINFO_RESPONSE_CODE, &pDown->res_code);
            switch (pDown->res_code){
            case 0:
                break;
            case 200:
            {
                if(!pDown->strPath.empty())
                {
                    if(pDown->fio == -1)
                        pDown->fio = open(pDown->strPath.c_str(),O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IWRITE);
                    if(pDown->fio < 0){
                        int err=errno;
                        perror("Open File Error: ");
                        return 0;
                    }
                }
                break;
            }
            case 301:
            case 302:
            {
                pDown->nType = 4;
                ((HttpCMulti*)pDown->user)->End(pDown);
                break;
            }
            default:
                return 0;
            }
        }


        if(pDown->pData==NULL)
        {
            pDown->nDataLen = 0;
            pDown->pData =(char*) malloc(DEFAULT_NEW_SIZE);
        }
        size_t nbuffsize = size* nmemb;
        //head
        if(pDown->strHead.empty())
        {
            memcpy(pDown->pData+pDown->nDataLen,buffer,nbuffsize);
            pDown->nDataLen += nbuffsize;
            if(pDown->bChunked==false && nbuffsize > 17)
            {//Transfer-Encoding: chunked
                char* pTmp = (char*)buffer;
                if(strncasecmp(pTmp,"Transfer-Encoding",17)==0)
                {
                    pTmp += 19;
                    if(strncasecmp(pTmp,"chunked",7)==0)
                        pDown->bChunked=true;
                }
            }
            if(pDown->Content_Length==0 && nbuffsize > 16)
            {
                char* pTmp = (char*)buffer;
                if(strncasecmp(pTmp,"Content-Length: ",16)==0)//
                {
                    pTmp += 16;
                    pDown->Content_Length = atol(pTmp);
                }
            }
            if(nbuffsize==2)
            {
                int nheadsize = 0;
                curl_easy_getinfo(pDown->p, CURLINFO_HEADER_SIZE, &nheadsize);
                if(nheadsize > 0)
                {
                    if(pDown->nhsize != nheadsize)
                    {
                        nheadsize += 2;
                        //pDown->strHead.append(pDown->pData , pDown->pData + pDown->nhsize);
                        pDown->strHead = string(pDown->pData , nheadsize);
                        pDown->nhsize = nheadsize;
                        pDown->nDataLen = 0;
                    }
                }
                nheadsize = 0;
                curl_easy_getinfo(pDown->p, CURLINFO_REQUEST_SIZE, &nheadsize);
                if(nheadsize > 0)
                {
                    if (ntop->g_xsDebug)
                        printf("request %d\n",nheadsize);
                }
                if(pDown->strPath.empty())
                {
                    pDown->nType = 3;
                    ((HttpCMulti*)pDown->user)->End(pDown);
                }
            }
            return nmemb;
        }
        //response
        //int ntimeout = pDown->ulRecvTime-pDown->ulSTime+10;
        //curl_easy_setopt(pDown->p, CURLOPT_TIMEOUT, ntimeout);
        //curl_easy_getinfo(pDown->p, CURLINFO_SPEED_DOWNLOAD, &pDown->speed);

        if((pDown->nDataLen+nbuffsize) >= DEFAULT_NEW_SIZE)
        {
            if(pDown->fio >= 0)
            {
                ssize_t ws1 = write(pDown->fio,pDown->pData,pDown->nDataLen);
                pDown->nDataLen = 0;
                ssize_t ws2 = write(pDown->fio,buffer,nbuffsize);

                pDown->ulWriteLength += ws1;
                pDown->ulWriteLength += ws2;
                return nmemb;
            }
            printf("write recv buff long:%d,%d\n",pDown->nDataLen,nbuffsize);
            pDown->nType = 2;
            ((HttpCMulti*)pDown->user)->End(pDown);
            return 0;
        }
        memcpy(pDown->pData+pDown->nDataLen,buffer,nbuffsize);
        pDown->nDataLen += nbuffsize;
        if((pDown->ulWriteLength + pDown->nDataLen) >= pDown->Content_Length)
        {
            if(pDown->fio >= 0)
            {
                ssize_t ws1 = write(pDown->fio,pDown->pData,pDown->nDataLen);
                pDown->nDataLen = 0;
                pDown->ulWriteLength += ws1;
                pDown->nType = 0;
                ((HttpCMulti*)pDown->user)->End(pDown);
                //下载成功
            }
            return 0;
        }
        return nmemb;
    }
    return nmemb;
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }
    printf("recv:%d\n",size * nmemb);
    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

void* DownFileThread(void* param)
{
    prctl(PR_SET_NAME,"DownFile");
    HttpCMulti* phc = (HttpCMulti*)param;
    if(!phc)  return NULL;
    static int ndfthreadidx = 0;
    phc->m_threadid = __sync_fetch_and_add(&ndfthreadidx,1);

    phc->Wait();


    //    int addtask = 1;
    //    CURLM* multi_handle = curl_multi_init();
    //    while(true)
    //    {
    //        phc->URL2CURL(multi_handle,addtask);
    //        phc->RemoveEnd();

    //    }
    return NULL;
}

int HttpCMulti::Run(int type /*= 0*/)
{
    pthread_create(&m_thread,0,DownFileThread,this);

    if(type)
    {
        while(1)
            sleep(1);
    }
    return 0;
}

int HttpCMulti::End(PDOWNFILEINDO pDown)
{
    if(pDown){
        if(ntop->g_xsDebug)
            printf("End:%u,%hu,%s\n",pDown->nid,pDown->nType,pDown->strPath.c_str());
        m_EndLst.insert(pDown);
    }
    return 0;
}

HttpCMulti::HttpCMulti()
{
    pthread_mutex_init(&m_cs,NULL);

    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);//定义res来保存Curl/http的请求的返回码
    if(code != CURLE_OK)  printf("init global failed:curl_global_init\n");

    m_multi_handle = curl_multi_init();//初始化一个multi curl对象
    if(NULL == m_multi_handle)  printf("init download file failed: curl_multi_init()\n");
}

HttpCMulti::~HttpCMulti()
{
    pthread_mutex_destroy(&m_cs);
    curl_multi_cleanup(m_multi_handle);//清除multi curl对象
    curl_global_cleanup();

}

//int HttpCMulti::AddUrlHead(const char* pURL,int nUrlLen)
//{
//    AddUrl(pURL,nUrlLen,0,0);
//    return 0;
//}

//int HttpCMulti::AddUrl(const char* pURL,int nUrlLen,const char* pSPath,int nSPathLen)
//{
//    string strURL(pURL,nUrlLen);
//    string strPath(pSPath,nSPathLen);
//    pthread_mutex_lock(&m_cs);
//    pair<DownURLList::iterator,bool> paiadd = m_downURL.insert(make_pair(strURL,strPath));
//    pthread_mutex_unlock(&m_cs);
//    if(paiadd.second)
//        __sync_fetch_and_add(&m_In,1);//先fetch，然后自加，返回的是自加以前的值
//    return 0;
//}

int HttpCMulti::AddUrl(PHTTPCACHEFILE pCacheFile)
{
    int nresult = 0;
    pthread_mutex_lock(&m_cs);
    pair<DownURLList::iterator,bool> paiadd = m_downURL.insert(make_pair(pCacheFile->szURL,*pCacheFile));
    if(paiadd.second) {
        __sync_fetch_and_add(&m_In,1);
        nresult =  1;
    }
    pthread_mutex_unlock(&m_cs);
    return nresult;
}

uint g_id = 1;//download file index
int HttpCMulti::URL2CURL(int addcount)
{
    if(m_multi_handle == NULL)
        return -1;
    for(int i = 0;i<addcount;i++)
    {
        if(m_downURL.size()==0)
            return 0;
        string strUrl;
        HTTPCACHEFILE cacheFile;//..
        pthread_mutex_lock(&m_cs);
        DownURLList::iterator iturl = m_downURL.begin();
        if(iturl != m_downURL.end()) {
            strUrl = iturl->first;//..
            cacheFile = iturl->second;
            m_downURL.erase(iturl++);
        }
        pthread_mutex_unlock(&m_cs);

        if(strUrl.size()==0 || cacheFile.szPath.size()==0)
            return 0;

        CURL* curl = curl_easy_init();//curl初始化
        if(NULL == curl)
            return CURLE_FAILED_INIT;

        DownFileInfo dfinfo;
        dfinfo.pData = NULL;
        dfinfo.strPath.swap(cacheFile.szPath);
        dfinfo.fio = -1;
        dfinfo.p = curl;
        dfinfo.nhsize = 0;
        dfinfo.res_code = 0;
        dfinfo.nid = __sync_fetch_and_add(&g_id,1);
        dfinfo.ulRecvTime = dfinfo.ulSTime = ntop->g_xstime;
        dfinfo.Content_Length = dfinfo.ulWriteLength = 0;
        dfinfo.bChunked = false;
        dfinfo.nType = 1;
        dfinfo.user = this;
        dfinfo.speed = 0;

        dfinfo.listKA = NULL;

        pair<DownFileList::iterator,bool> pain =  m_downlst.insert(make_pair(curl,dfinfo));
        if(pain.second){//将属性,传输信息的URL和你要传输的信息添加到我们建立的cURL指针或是变量之中，同时使用这个函数将我们的报文头变量或是指针也添加到cURL指针或是变量中
            curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());//给cURL结构体添加发送信息的url
            curl_easy_setopt(curl, CURLOPT_HEADER, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb_hcmulti_Data);//这个属性是给cURL结构体设置当cURL成功发送数据，并且取得返回数据，由那个函数来处理得到的返回报文,
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pain.first->second);
            /**
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
    */
            //curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);

            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, assetsManagerProgressFunc);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &pain.first->second);
            if(dfinfo.nid==1){
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600);
                curl_easy_setopt(curl,CURLOPT_MAX_RECV_SPEED_LARGE,1024*1024);
            }else{
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
                curl_easy_setopt(curl,CURLOPT_MAX_RECV_SPEED_LARGE,1024*1024);
            }

            if(dfinfo.strPath.empty())
                curl_easy_setopt(curl, CURLOPT_NOBODY, true);

            if(strlen(cacheFile.szCookie))
                curl_easy_setopt(curl,CURLOPT_COOKIE,cacheFile.szCookie);

            //~~~~~~~~~~~~
            //            struct curl_slist *list = NULL;
            //            //list = curl_slist_append(list, "Connection: keep-alive");
            //            //list = curl_slist_append(list, "Connection: close");
            //            if(strlen(cacheFile.szCookie))
            //                list = curl_slist_append(list, cacheFile.szCookie);
            //            curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);
            //            pain.first->second.listKA = list;
            //

            //pthread_mutex_lock(&m_cs);
            curl_multi_add_handle(m_multi_handle,curl);//调用curl_multi _add_handle把easy curl对象添加到multi curl对象中
            //pthread_mutex_unlock(&m_cs);
            return CURLE_OK;
        }
    }
    return -1;
}

int HttpCMulti::RemoveEnd()
{
    if(m_EndLst.size()==0)
        return 0;
    EndList::iterator itel = m_EndLst.begin();
    for(; itel != m_EndLst.end();)
    {
        PDOWNFILEINDO pDown = *itel;
        m_EndLst.erase(itel++);

        __sync_fetch_and_add(&m_Out,1);
        m_comlst.insert(make_pair(pDown->p,*pDown));

        //pthread_mutex_lock(&m_cs);
        curl_multi_remove_handle(m_multi_handle,pDown->p);//移除相关easy curl对象
        //pthread_mutex_unlock(&m_cs);

        DownFileList::iterator itf = m_downlst.find(pDown->p);
        if(itf != m_downlst.end())
        {
            if(pDown->nType==0)
            {
                //下载成功，入库
            }
            m_downlst.erase(itf);
        }
    }
    return 0;
}

int HttpCMulti::PrintfSpeed(int setspeed)
{
    if(m_downlst.size())
    {
        DownFileList::iterator itb = m_downlst.begin();
        for(;itb != m_downlst.end();itb++)
        {
            DownFileInfo& fDown = itb->second;

            int ntimeout = fDown.ulRecvTime-fDown.ulSTime+30;
            curl_easy_setopt(fDown.p, CURLOPT_TIMEOUT, ntimeout);
            if(fDown.nid==2)
                setspeed = 1024*1024;
            curl_easy_setopt(fDown.p,CURLOPT_MAX_RECV_SPEED_LARGE,setspeed);
            curl_easy_getinfo(fDown.p, CURLINFO_SPEED_DOWNLOAD, &fDown.speed);
            printf("%u:%lf\n",fDown.nid,fDown.speed);
        }
    }
    return 0;
}

//CURLOPT_NOPROGRESS
int HttpCMulti::Wait()
{
    ulong ultimes = ntop->g_xstime;
    ulong ultimee = ntop->g_xstime;
    int still_running = 1;
    int repeats = 0;
    int setspeed = 1024*1024;
    //curl_waitfd tmpwaitfd[10] = {0};
    while(true)
    {
        URL2CURL(1);

        RemoveEnd();

        CURLMcode mc;
        int numfds;

        still_running = 0;
        //pthread_mutex_lock(&m_cs);
        mc = curl_multi_perform(m_multi_handle, &still_running);//进行并发的访问
        //pthread_mutex_unlock(&m_cs);
        if(mc == CURLM_OK ){
            /* wait for activity, timeout or "nothing" */
            mc = curl_multi_wait(m_multi_handle, NULL, 0, 1000, &numfds);
            m_Run = still_running;
        }

        if(mc != CURLM_OK) {
            fprintf(stderr, "curl_multi failed, code %d.n", mc);
            break;
        }

        ultimee = ntop->g_xstime;
        if(still_running && (ultimee - ultimes) > 5)
        {
            ultimes = ultimee;
            PrintfSpeed(setspeed/still_running);
        }
        /* 'numfds' being zero means either a timeout or no file descriptors to
         wait for. Try timeout on first occurrence, then assume no file
         descriptors and no file descriptors to wait for means wait for 100
         milliseconds. */

        if(!numfds) {
            repeats++; /* count number of repeated zero numfds */
            if(repeats > 1) {
                //WAITMS(100); /* sleep 100 milliseconds */
                sleep(1);
            }
        }
        else
            repeats = 0;

    }
    //while(still_running);
    return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void XsHttpFile_Init_Down (Flow *pFlow ,PHTTPCACHEFILE  pCacheFile)
{
    //初始化
    pCacheFile->hashkey = 0;
    bzero(pCacheFile->szHost,sizeof(pCacheFile->szHost));
    bzero(pCacheFile->szCookie,sizeof(pCacheFile->szCookie));
    bzero(pCacheFile->szReferer,sizeof(pCacheFile->szReferer));
    bzero(pCacheFile->szUserAgent,sizeof(pCacheFile->szUserAgent));

    struct ndpi_flow_struct *pNdpiFlow=pFlow->get_ndpi_flow();

    char pHttpHost[20] = { 0 };
    char pHttpUrlName[1500] = { 0 };
    char szUrl[1500]= { 0 };
    memcpy(pHttpHost,pNdpiFlow->packet.host_line.ptr,pNdpiFlow->packet.host_line.len);
    memcpy(pHttpUrlName,pNdpiFlow->packet.http_url_name.ptr,pNdpiFlow->packet.http_url_name.len);
    snprintf(szUrl,sizeof(szUrl)-1,"http://%s%s",pHttpHost,pHttpUrlName);

    string szURL(szUrl,strlen(szUrl));
    pCacheFile->szURL.swap(szURL);

    pCacheFile->hashkey=CRC64((const u_char *)szUrl,strlen(szUrl));
    // pCacheFile->hashkey=pFlow->virtualnum.hashkey;//test

    char pDate[10]={0};
    get_dateString(pDate,sizeof(pDate)-1);
    char szFilePath[64] ={ 0 };
    char szCurlFilePath[128] = { 0 };

    // filepath /root/CurlFileDn/date/(qq/wecaht)/(pic/file/vedio)
    switch (pFlow->virtualnum.TypeMsg) {
    case msg_qq_file:{
        snprintf(szFilePath,sizeof(szFilePath)-1,CurlSaveDirQqFile,pDate);
        snprintf(szCurlFilePath,sizeof(szCurlFilePath)-1,"%s/%lu",szFilePath, pCacheFile->hashkey);
        break;
    }
    case msg_qq_picture:{
        snprintf(szFilePath,sizeof(szFilePath)-1,CurlSaveDirQqPic,pDate);
        snprintf(szCurlFilePath,sizeof(szCurlFilePath)-1,"%s/%lu",szFilePath, pCacheFile->hashkey);
        break;
    }
    case msg_qq_video:{
        snprintf(szFilePath,sizeof(szFilePath)-1,CurlSaveDirQqVedio,pDate);
        snprintf(szCurlFilePath,sizeof(szCurlFilePath)-1,"%s/%lu.mp4",szFilePath, pCacheFile->hashkey);
        break;
    }
    case msg_wechat_video:{
        snprintf(szFilePath,sizeof(szFilePath)-1,CurlSaveDirWeChatVedio,pDate);
        snprintf(szCurlFilePath,sizeof(szCurlFilePath)-1,"%s/%lu.mp4",szFilePath, pCacheFile->hashkey);
        break;
    }
    case msg_wechat_picture:{
        snprintf(szFilePath,sizeof(szFilePath)-1,CurlSaveDirWeChatPic,pDate);
        snprintf(szCurlFilePath,sizeof(szCurlFilePath)-1,"%s/%lu",szFilePath, pCacheFile->hashkey);
        break;
    }
    default :{
        snprintf(szFilePath,sizeof(szFilePath)-1,"%s/%s",CurlSaveDir,pDate);
        snprintf(szCurlFilePath,sizeof(szCurlFilePath)-1,"%s/%lu",szFilePath, pCacheFile->hashkey);
        break;
    }
    }

    if( -1 ==access(szFilePath,F_OK))
        MyMakeDir(szFilePath,755);
    string szPath(szCurlFilePath,strlen(szCurlFilePath));
    pCacheFile->szPath.swap(szPath);


    //       snprintf(szCurlFilePath,sizeof(szCurlFilePath)-1,"%s/%lu",szFilePath, pCacheFile->hashkey);
    //    if(pFlow->get_ndpi_flow()->packet.http_cookie.len)
    //       memcpy(pCacheFile->szCookie,pNdpiFlow->packet.http_cookie.ptr,pNdpiFlow->packet.http_cookie.len);
}

void XsHttp_Add_Down_URL(Flow *pFlow)
{
    HTTPCACHEFILE  CacheFile;
    if (ntop->httpCMulti){
        XsHttpFile_Init_Down(pFlow,&CacheFile);
        ntop->httpCMulti->AddUrl(&CacheFile);
        //  ntop->httpCMulti->Wait();
    }
}







