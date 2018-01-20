//#include "httpcmulti1.h"
//#include <unistd.h>
//#include <stdlib.h>
//#include <string.h>
//#include <fcntl.h>

////#include "dw_publib.h"
////#include "dw_message.h"
////#include "mydbop.h"
////#include "autolock.h"
////#include "dwp2psohu.h"
////#include "xshash.h"
////#include "dwp2pqq.h"
////#include "dw_capture.h"
////#include "dw_inifile.h"

////#include "cJSON.h"
//#include "json.h"

////curl_multi_exec
////curl_multi_socket()
////curl_multi_perform()
////curl_multi_fdset()
////curl_progress_callback
//extern char g_hostName[50];
//extern CMyDBOp g_dbop;
//extern FLOWCOUNT g_flowcount;
//extern dwp2psohu g_sohu;
//extern dwp2pqq g_qqtsp2p;
//extern dw_capture g_capture;
//extern CDataArray m_arWebSite;


////#include "qykerlen.h"
//bool GetFileSha1_sys(PFILENODE pnewfn,const char* pPath)
//{
//    u_char szmd5[20] = {0};
//    //
//    char szFCMD[MAX_PATH] = {0};
//    sprintf(szFCMD,"sha1sum %s",pPath);
//    FILE* fpmd5 = popen(szFCMD,"r");
//    if(fpmd5)
//    {
//        char sztmp[500] = {0};
//        int nred = fread(sztmp,1,300,fpmd5);
//        pclose(fpmd5);
//        if(nred >40 && str2sha1(sztmp,pnewfn->szFHash))
//        {
//            //printf("%s",sztmp);
//            //PrintfPPSMessage(pnewfn->szFHash,20);
//            return true;
//        }
//    }
//}


//bool isSohuP2PDownload(PWEBSITEEX pWebSite)
//{
//    if(pWebSite)
//    {
//        if(strncmp(pWebSite->szHost1,DW_SOHUP2P_HOST/*"sohu-p2p"*/,8)==0)
//        {
//            if(g_nDebug) printf("sohu download\n");
//            return true;
//        }

//    }
//    return false;
//}

//char* pResponseT1[100]=
//{
//    "HTTP/1.1 302 Found\r\n",
//    "Location: http://172.20.1.101/IXC327db8e9bcbaecc49c1125a0e82542f2/qqfile/qq/tm/2013Preview2/10913/TM2013Preview2.exe\r\n",
//    "Content-Type: text/html\r\n",
//    "Content-Length: 0\r\n",
//    "Connection: close\r\n",
//    "\r\n",
//    "HTTP/1.1 302 Found\r\n",
//    "Location: http://172.20.1.100:8084/A/3512495485/12582481862840909536/txkj_url/dldir1.qq.com/qqfile/qq/tm/2013Preview2/10913/TM2013Preview2.exe\r\n",
//    "Content-Type: text/html\r\n",
//    "Content-Length: 0\r\n",
//    "Connection: close\r\n",
//    "\r\n",
//    "HTTP/1.1 200 OK\r\n",
//    "Server: TCDN_NWS\r\n",
//    "Connection: keep-alive\r\n",
//    "Date: Mon, 24 Oct 2016 07:09:04 GMT\r\n",
//    "Cache-Control: max-age=600\r\n",
//    "Expires: Mon, 24 Oct 2016 07:19:04 GMT\r\n",
//    "Last-Modified: Thu, 16 Oct 2014 08:27:01 GMT\r\n",
//    "Content-Type: application/octet-stream\r\n",
//    "Content-Length: 49286712\r\n",
//    "X-Cache-Lookup: Hit From Disktank\r\n",
//    "\r\n"
//};

////pthread_mutex_t* gpMte = NULL;
////set<HttpCMulti*> gpHttpMulti;
////bool isDownList(void* curl)
////{
////    set<HttpCMulti*>::iterator itb = gpHttpMulti.begin();
////    while(itb != gpHttpMulti.end())
////    {
////        if((*itb)->isDownList(curl))
////            return true;
////        itb++;
////    }
////    return false;
////}


//long chunk_end_callback(void *ptr)
//{
//    printf("chunk_end_callback%x\n",ptr);
//}

//int assetsManagerProgressFunc(void *clientp,
//                              double dltotal,
//                              double dlnow,
//                              double ultotal,
//                              double ulnow)
//{
//    PDOWNFILEINDO pDown = (PDOWNFILEINDO)clientp;
//    if(pDown)
//    {
//        pDown->dltotal = dltotal;
//        pDown->dlnow = dlnow;
//        pDown->progress = dlnow*100/dltotal;
//        //printf("id:%u,%f\n",pDown->nid,pDown->progress);
//    }
//    return 0;
//}

//HttpCDown g_httpc;

//ulong llDownTraffic = 0;
//ulong ultimeTraffic = 0;

//void checkspeed()
//{
//    if((time(0)-ultimeTraffic)>60)
//    {
//        llDownTraffic = 0;
//        ultimeTraffic = time(0);

//        //        SYSTEMTIME st;
//        //        GetLocalTime2(&st);
//        //        int nWeekOfDay = 0;
//        //        if(st.tm_wday==0)
//        //            nWeekOfDay = 6;
//        //        else
//        //            nWeekOfDay = st.tm_wday-1;
//        //        int nspeedtmp = g_httpc.m_limitspeed[nWeekOfDay][st.tm_hour];
//        //        if(nspeedtmp < (llDownTraffic/1024))
//        //        {
//        //        }
//    }
//}

//static int gnlogLocation = 20;
//static size_t cb_hcmulti_Data(void* buffer, size_t size, size_t nmemb, void* lpVoid)
//{
//    llDownTraffic += (size*nmemb);
//    checkspeed();

//    //printf("check:%d,%d,%x\n",nmemb,size,lpVoid);
//    PDOWNFILEINDO pDown = (PDOWNFILEINDO)lpVoid;
//    if(pDown && pDown->nType==1)
//    {
//        //mtxautolock mtxalock(gpMte);
//        //if(!isDownList(pDown->p))
//        //    return 0;
//        //bool bloglocal = false;
//        char* pBfTmp = (char*)buffer;
//        if((strncmp(pBfTmp,"HTTP/1.1 ",9)==0)||(strncmp(pBfTmp,"HTTP/1.0 ",9)==0))
//        {
//            //pDown->Content_Length = 0;//20170713
//            //pDown->ulWriteLength = 0;//20170713
//            //pDown->nDataLen = 0;
//            //dfinfo.pData = NULL;
//            //dfinfo.strPath.swap(strSPath);
//            //dfinfo.fio = -1;
//            //dfinfo.pdf = pdf;
//            //dfinfo.p = curl;
//            pDown->nhsize = 0;
//            pDown->res_code = 0;
//            //dfinfo.nid = __sync_fetch_and_add(&g_id,1);
//            pDown->ulRecvTime = pDown->ulSTime = time(0);
//            //dfinfo.Content_Length = dfinfo.ulWriteLength = 0;
//            pDown->bChunked = false;
//            pDown->nType = 1;
//            //dfinfo.user = this;
//            pDown->speed = 0;
//        }
//        if(g_nDebug)
//        {
//            //string strtmp = pBfTmp;
//            //printf(strtmp.c_str());
//        }
//        pDown->ulRecvTime = time(0);
//        if(pDown->res_code==0)
//        {
//            pDown->res_code = atoi(&pBfTmp[9]);
//            //curl_easy_getinfo(pDown->p, CURLINFO_RESPONSE_CODE, &pDown->res_code);
//            switch (pDown->res_code) {
//            case 0:
//                break;
//            case 200:
//            {
//                if(!pDown->strPath.empty())
//                {
//                    if(pDown->fio == -1)
//                    {
//                        pDown->fio = open(pDown->strPath.c_str(),O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
//                    }
//                    if(pDown->fio < 0)
//                    {
//                        pDown->nType = 5;
//                        ((HttpCMulti*)pDown->user)->End(pDown);
//                        LogFilePrintf("dwhttpmulti","openfile:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                        return 0;
//                    }
//                    else// if(pDown->ulWriteLength>0)//206
//                    {
//                        pDown->ulWriteLength = 0;
//                        __off64_t uloffset = lseek64(pDown->fio,pDown->ulWriteLength,SEEK_SET);
//                        if(g_nDebug)
//                            printf("start 200:%ld,%ld,%ld\n",uloffset,pDown->ulWriteLength,pDown->Content_Length);
//                        pDown->Content_Length = 0;
//                    }
//                }
//                break;
//            }
//                //case 404:
//            case 301:
//            case 302:
//            {
//                //if(pDown->res_code==301 || pDown->res_code==302)
//                //{
//                //    char szUrl[1024] = {0};
//                //    char* pUrl = szUrl;
//                //    curl_easy_getinfo(pDown->p, CURLINFO_REDIRECT_URL, &pUrl);
//                //    pDown->nType = 4;
//                //    ((HttpCMulti*)pDown->user)->End(pDown);
//                //}
//                pDown->nLocation++;
//                if(gnlogLocation>0)
//                {
//                    gnlogLocation--;
//                }
//                break;
//            }
//            case 206:
//            {
//                if(pDown->pdf->cReTry > 0)
//                {
//                    if(!pDown->strPath.empty())
//                    {
//                        if(pDown->fio == -1)
//                        {
//                            pDown->fio = open(pDown->strPath.c_str(),O_WRONLY/*|O_CREAT*/, S_IRUSR|S_IWUSR);
//                        }
//                        if(pDown->fio < 0)
//                        {
//                            pDown->nType = 5;
//                            ((HttpCMulti*)pDown->user)->End(pDown);
//                            LogFilePrintf("dwhttpmulti","openfile:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                            return 0;
//                        }
//                        else// if(pDown->ulWriteLength>0)//206
//                        {
//                            __off64_t uloffset = lseek64(pDown->fio,pDown->ulWriteLength,SEEK_SET);
//                            if(g_nDebug)
//                                printf("start 206:%ld,%ld,%ld\n",uloffset,pDown->ulWriteLength,pDown->Content_Length);
//                        }
//                        break;
//                    }
//                }
//            }
//            default:
//            {//异常下载需要日志记录
//                pDown->nType = 6;
//                ((HttpCMulti*)pDown->user)->End(pDown);
//                if(g_nDebug) LogFilePrintf("dwhttpmulti","responsecode:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                return 0;
//            }
//            }
//        }

//        if(pDown->pData==NULL)
//        {
//            //            pDown->nDataLen = 0;
//            //            pDown->pData = (char*)dwmalloc(DEFAULT_NEW_SIZE,1);
//            //            if(pDown->pData==NULL)
//            //            {
//            //                printf("dwmalloc failed\n");
//            //                return 0;
//            //            }
//        }
//        size_t nbuffsize = size* nmemb;
//        g_flowcount.llDownTraffic += nbuffsize;
//        //printf("recv:%d\n",nbuffsize);
//        //return nbuffsize;
//        //head
//        if(pDown->strHead.empty())
//        {
//            if((pDown->nDataLen+nbuffsize) >= DEFAULT_NEW_SIZE)
//            {
//                LogFilePrintf("dwhttpmulti","down head long:%s",pDown->pdf->szURI);
//                return 0;
//            }
//            memcpy(pDown->pData+pDown->nDataLen,buffer,nbuffsize);
//            pDown->nDataLen += nbuffsize;
//            if((pDown->res_code==302 || pDown->res_code==301)&& nbuffsize > 9)
//            {
//                char* pTmp = (char*)buffer;
//                if(pDown->strLocation.empty() && strncasecmp(pTmp,"Location",8)==0)
//                {
//                    char* pUrl = pTmp + 10;
//                    int nUrlLen = nbuffsize-10 -2;
//                    //next add re download
//                    if(pDown->strLocation.empty())
//                    {
//                        pDown->strLocation.append(pUrl,nUrlLen);
//                    }
//                    else
//                        pDown->strLocation.clear();
//                    //printf("location:%s",pDown->strLocation.c_str());
//                    //pDown->nType = 4;
//                    //((HttpCMulti*)pDown->user)->End(pDown);
//                    //return 0;
//                    //LogFilePrintf("dwhttpmulti","Locat:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                    return nbuffsize;
//                }
//            }
//            if(pDown->res_code==200 || (pDown->res_code==206 && pDown->chefile.cReTry>0))
//            {
//                if(pDown->bChunked==false && pDown->Content_Length==0 && nbuffsize > 17)
//                {//Transfer-Encoding: chunked
//                    char* pTmp = (char*)buffer;
//                    if(strncasecmp(pTmp,"Transfer-Encoding",17)==0)
//                    {
//                        pTmp += 19;
//                        if(strncasecmp(pTmp,"chunked",7)==0)
//                        {
//                            if(pDown->pdf)
//                                LogFilePrintf("dwhttpmulti","chunked:%s",pDown->pdf->szURI);
//                            pDown->bChunked=true;
//                        }
//                    }
//                }
//                if(pDown->bChunked==false && pDown->Content_Length==0 && nbuffsize > 16)
//                {
//                    char* pTmp = (char*)buffer;
//                    if(strncasecmp(pTmp,"Content-Length: ",16)==0)//
//                    {
//                        pTmp += 16;
//                        pDown->Content_Length = atol(pTmp);
//                        PFILENODE ptmp = NULL;
//                        g_capture.FileAtLocal(pDown->pdf->nCRC64,ptmp,pDown->pdf->pWebSite);
//                        if(pDown->strPath.size()>10 && ptmp && ptmp->nStatus==1)
//                        {
//                            string strPathTmp;
//                            strPathTmp.append(pDown->strPath.c_str(),pDown->strPath.length()-3);
//                            struct stat stfilesize = {0};
//                            stat(strPathTmp.c_str(),&stfilesize);
//                            if(stfilesize.st_size == pDown->Content_Length)
//                            {
//                                ptmp->nStatus = 0;
//                                pDown->nType = -1;
//                                ((HttpCMulti*)pDown->user)->End(pDown);
//                                return 0;
//                            }
//                        }

//                    }
//                }

//                if(nbuffsize==2)
//                {
//                    int nheadsize = 0;
//                    nheadsize = pDown->nDataLen;
//                    if(pDown->p)
//                        curl_easy_getinfo(pDown->p, CURLINFO_HEADER_SIZE, &nheadsize);
//                    if(nheadsize > 0)
//                    {
//                        if(pDown->nhsize != nheadsize)
//                        {
//                            nheadsize += 2;
//                            //pDown->strHead.append(pDown->pData , pDown->pData + pDown->nhsize);
//                            pDown->strHead = string(pDown->pData , nheadsize);
//                            pDown->nhsize = nheadsize;
//                            pDown->nDataLen = 0;
//                            if(!pDown->bChunked && pDown->pdf->nRuleID!=RULE_ID_M3U8 && pDown->pdf->pWebSite->nMinSize && pDown->Content_Length < pDown->pdf->pWebSite->nMinSize*1024)
//                            {
//                                if(pDown->pdf->pWebSite && pDown->pdf->pWebSite->nHostCRC==DW_QIYI_CRC)//RULE_ID_QIYI_ADD1//if(pDown->pdf->nRuleID==RULE_ID_QIYI_ADD1 || pDown->pdf->nRuleID==RULE_ID_QIYI_ADD2)//20170802 test code
//                                {
//                                    return nbuffsize;
//                                }
//                                if(g_nDebug)
//                                    printf("%d < %d\n",pDown->Content_Length,pDown->pdf->pWebSite->nMinSize*1024);
//                                pDown->nType = 9;
//                                ((HttpCMulti*)pDown->user)->End(pDown);
//                                //LogFilePrintf("dwhttpmulti","checksize1:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                                return 0;
//                            }
//                            else if(!pDown->bChunked && pDown->pdf->pWebSite->nMaxSize && pDown->Content_Length > pDown->pdf->pWebSite->nMaxSize*1024)
//                            {
//                                if(g_nDebug)
//                                    printf("%d > %d\n",pDown->Content_Length,pDown->pdf->pWebSite->nMaxSize*1024);
//                                pDown->nType = 9;
//                                ((HttpCMulti*)pDown->user)->End(pDown);
//                                //LogFilePrintf("dwhttpmulti","checksize2:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                                return 0;
//                            }//if(pDown->Content_Length < pDown->pdf->pWebSite->nMaxSize)
//                        }
//                        if(pDown->nLocation && gnlogLocation)
//                            LogFilePrintf("dwhttpmulti","strLocation:%d,%s",gnlogLocation,pDown->strHead.c_str());
//                    }
//                    nheadsize = 0;
//                    curl_easy_getinfo(pDown->p, CURLINFO_REQUEST_SIZE, &pDown->nHeadSize);
//                    if(pDown->nHeadSize > 0)
//                    {
//                        if(g_nDebug)printf("request %d,%d\n",pDown->res_code,pDown->nHeadSize);
//                        if(pDown->Content_Length)
//                            ((HttpCMulti*)pDown->user)->RestoreSQL(*pDown);
//                    }
//                    if(pDown->strPath.empty())
//                    {
//                        pDown->nType = 3;
//                        ((HttpCMulti*)pDown->user)->End(pDown);
//                        LogFilePrintf("dwhttpmulti","head empty:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                        return 0;
//                    }
//                }
//            }
//            return nbuffsize;
//        }
//        if(pDown->bChunked)
//        {
//            //printf("check:%d,%d\n",nmemb,nbuffsize);
//            if(nbuffsize < 0  || nbuffsize > 90000)
//            {
//                pDown->nType = 10;
//                ((HttpCMulti*)pDown->user)->End(pDown);
//                LogFilePrintf("dwhttpmulti","nbuffsize1:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                return 0;
//            }
//            if(nbuffsize==0)
//            {
//                pDown->nType = 0;
//                ((HttpCMulti*)pDown->user)->End(pDown);
//                LogFilePrintf("dwhttpmulti","nbuffsize2:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                return 0;
//            }
//            pDown->Content_Length += nbuffsize;
//            pDown->nCKlen = nbuffsize;
//            if((pDown->nDataLen+nbuffsize) >= DEFAULT_NEW_SIZE)
//            {
//                if(pDown->fio >= 0)
//                {
//                    ssize_t ws1 = write(pDown->fio,pDown->pData,pDown->nDataLen);
//                    ssize_t ws2 = write(pDown->fio,buffer,nbuffsize);
//                    pDown->ulWriteLength += ws1;
//                    pDown->ulWriteLength += ws2;
//                    pDown->nDataLen = 0;
//                    return nbuffsize;
//                }
//                //LogFilePrintf("dwhttpmulti","write recv buff long:%d,%s",pDown->res_code,pDown->pdf->szURI);
//                //printf("write recv buff long:%d,%d\n",pDown->nDataLen,nbuffsize);
//                pDown->nType = 2;
//                ((HttpCMulti*)pDown->user)->End(pDown);
//                LogFilePrintf("dwhttpmulti","data>max new size:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//                return 0;
//            }

//            memcpy(pDown->pData+pDown->nDataLen,buffer,nbuffsize);
//            pDown->nDataLen += nbuffsize;
//            return nbuffsize;
//        }
//        //response
//        //int ntimeout = pDown->ulRecvTime-pDown->ulSTime+10;
//        //curl_easy_setopt(pDown->p, CURLOPT_TIMEOUT, ntimeout);
//        //curl_easy_getinfo(pDown->p, CURLINFO_SPEED_DOWNLOAD, &pDown->speed);

//        if((pDown->nDataLen+nbuffsize) >= DEFAULT_NEW_SIZE)
//        {
//            if(pDown->fio >= 0)
//            {
//                ssize_t ws1 = write(pDown->fio,pDown->pData,pDown->nDataLen);
//                ssize_t ws2 = write(pDown->fio,buffer,nbuffsize);
//                pDown->ulWriteLength += ws1;
//                pDown->ulWriteLength += ws2;
//                pDown->nDataLen = 0;
//                return nbuffsize;
//            }
//            //LogFilePrintf("dwhttpmulti","write recv buff long:%d,%s",pDown->res_code,pDown->pdf->szURI);
//            //printf("write recv buff long:%d,%d\n",pDown->nDataLen,nbuffsize);
//            pDown->nType = 2;
//            ((HttpCMulti*)pDown->user)->End(pDown);
//            LogFilePrintf("dwhttpmulti","fopen 0:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//            return 0;
//        }
//        memcpy(pDown->pData+pDown->nDataLen,buffer,nbuffsize);
//        pDown->nDataLen += nbuffsize;
//        if((pDown->ulWriteLength + pDown->nDataLen) >= pDown->Content_Length)
//        {
//            if(pDown->fio >= 0)
//            {
//                ssize_t ws1 = write(pDown->fio,pDown->pData,pDown->nDataLen);
//                pDown->nDataLen = 0;
//                pDown->ulWriteLength += ws1;
//                pDown->nType = 0;
//                pDown->dlnow = pDown->ulWriteLength;
//                ((HttpCMulti*)pDown->user)->End(pDown);
//                return nbuffsize;
//            }
//            //LogFilePrintf("dwhttpmulti","data>max new size:%hu,%d,%d",pDown->nType,pDown->res_code,pDown->fio);
//            return 0;
//        }
//        return nbuffsize;
//    }
//    return 0;
//    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
//    if( NULL == str || NULL == buffer )
//    {
//        return -1;
//    }
//    printf("recv:%d\n",size * nmemb);
//    char* pData = (char*)buffer;
//    str->append(pData, size * nmemb);
//    return nmemb;
//}


//void* ThreadDownFile(void* param)
//{
//    LogFilePID("DownFile:%ld",gettid());
//    HttpCMulti* phc = (HttpCMulti*)param;
//    if(!phc)
//    {
//        return NULL;
//    }
//    static int ndfthreadidx = 0;
//    phc->m_threadid = __sync_fetch_and_add(&ndfthreadidx,1);
//    phc->Wait();
//    //    int addtask = 1;
//    //    CURLM* multi_handle = curl_multi_init();
//    //    while(true)
//    //    {
//    //        phc->URL2CURL(multi_handle,addtask);
//    //        phc->RemoveEnd();

//    //    }
//    return NULL;
//}

////==>HttpCDown
//void* Threadaddurl(void* param)
//{
//    LogFilePID("addurl:%ld",gettid());
//    HttpCDown* phcd = (HttpCDown*)param;
//    if(!phcd)
//    {
//        return NULL;
//    }
//    int nOneHour = -1;

//    while(1)
//    {
//        SYSTEMTIME st;
//        GetLocalTime2(&st);
//        if(phcd->bReSetSpeed || st.tm_hour != nOneHour)
//        {
//            phcd->bReSetSpeed = false;
//            nOneHour = st.tm_hour;
//            phcd->SetSpeed(st);
//        }

//        phcd->DelFailedDown();
//        phcd->EndofDownLoad();
//        if(phcd->m_dflstint64.size()>MAX_DF_LIST)//50
//        {
//            sleep(1);
//            continue;
//        }
//        //phcd->Data2list();
//        PCACHEFILE pdf = phcd->Get();
//        if(pdf)
//        {
//            if(!phcd->addInCurlLst(pdf))
//            {
//                if(pdf->pTS)
//                    delete pdf->pTS;
//                pdf->pTS = NULL;
//                delete pdf;
//            }
//            phcd->AutoClearList();
//        }
//        //else
//        {
//            sleep(1);
//        }
//    }

//}

//void* ThreadaddurlEx(void* param)
//{
//    LogFilePID("addurlex:%ld",gettid());
//    HttpCDown* phcd = (HttpCDown*)param;
//    if(!phcd)
//    {
//        return NULL;
//    }
//    int nOneHour = -1;
//    int nfxHttpdata = 0;
//    while(1)
//    {
//        sleep(1);
//        SYSTEMTIME st;
//        GetLocalTime2(&st);
//        if(phcd->bReSetSpeed || st.tm_hour != nOneHour)
//        {
//            phcd->bReSetSpeed = false;
//            nOneHour = st.tm_hour;
//            phcd->SetSpeed(st);
//        }

//        phcd->UpdateWaiteDown();
//        phcd->UpdateDB();
//        phcd->DelFailedDown();
//        phcd->EndofDownLoad();
//        if(phcd->m_dflstint64.size()>MAX_DF_LIST)//50
//        {
//            sleep(1);
//            continue;
//        }

//        nfxHttpdata++;
//        if(nfxHttpdata < 5)
//        {
//            for(int a=0;a<m_arWebSite.GetArrayCount();a++)
//            {
//                PWEBSITEEX pWebSite = (PWEBSITEEX)m_arWebSite.GetNode(a);
//                if(pWebSite==NULL)
//                    continue;
//                if(pWebSite->arhttpdata.size() < 1)
//                {
//                    continue;
//                }
//                httpDQData& arhttpdata = pWebSite->arhttpdata;
//                //if(arhttpdata.size()==0)
//                //    nfxHttpdata += 5;

//                //add last url download
//                //if(pWebSite->arDLFile.size() < pWebSite->nTasks || pWebSite->nCurTasks < pWebSite->nTasks)
//                for(int i = pWebSite->nCurTasks ; i<pWebSite->nTasks ; i++)
//                {
//                    if(arhttpdata.size()==0)
//                    {
//                        nfxHttpdata += 5;
//                        break;
//                    }
//                    PHTTPDOWNQUEUE pdqutmp = NULL;
//                    httpDQData::iterator itaddlast;
//                    httpDQData::iterator itaddbegin = arhttpdata.begin();
//                    if(itaddbegin != arhttpdata.end())//20170721
//                    {
//                        //if(itaddbegin->second->pfd)
//                        {
//                            pdqutmp =(PHTTPDOWNQUEUE) *itaddbegin;
//                            itaddlast = itaddbegin;
//                        }

//                        itaddbegin++;
//                    }
//                    if(pdqutmp)
//                    {
//                        //PCACHEFILE pfdAdd = pdqutmp->pfd;
//                        PCACHEFILE pfdAdd = phcd->m_deturl.GetDetURL(pdqutmp->nCRC64);
//                        arhttpdata.erase(itaddlast);
//                        pthread_mutex_lock(&pWebSite->mtx);
//                        httpDetQueue::iterator itdqfind = pWebSite->arhttpDQ.find(*pdqutmp);
//                        if(itdqfind != pWebSite->arhttpDQ.end())
//                            pWebSite->arhttpDQ.erase(itdqfind);
//                        pthread_mutex_unlock(&pWebSite->mtx);
//                        phcd->m_deturl.DeleteDetURL(pdqutmp->nCRC64);

//                        if(pfdAdd)
//                        {
//                            if(!phcd->addInCurlLst(pfdAdd))
//                            {
//                                if(pfdAdd->pTS)
//                                    delete pfdAdd->pTS;
//                                pfdAdd->pTS = NULL;
//                                delete pfdAdd;
//                            }
//                        }
//                    }
//                }

//            }
//            continue;
//        }
//        nfxHttpdata = 0;

//        for(int b=0;b<m_arWebSite.GetArrayCount();b++)
//        {
//            PWEBSITEEX pWebSite = (PWEBSITEEX)m_arWebSite.GetNode(b);
//            if(pWebSite==NULL)
//                continue;
//            //            if(g_nDebug)
//            //            {
//            //                if(pWebSite->arhttpDQ.size() < 100)
//            //                    continue;
//            //            }
//            //            else
//            pthread_mutex_lock(&pWebSite->mtx);
//            int narDQSize = pWebSite->arhttpDQ.size();
//            pthread_mutex_unlock(&pWebSite->mtx);
//            if(narDQSize < 1)
//            {
//                continue;
//            }

//            httpDetQueue arhttpDQTmp;

//            pthread_mutex_lock(&pWebSite->mtx);
//            arhttpDQTmp.swap(pWebSite->arhttpDQ);
//            //pWebSite->arhttpDQTmp.swap(pWebSite->arhttpDQ);
//            pthread_mutex_unlock(&pWebSite->mtx);

//            httpDQData& arhttpdata = pWebSite->arhttpdata;
//            arhttpdata.clear();
//            //save default data i queue
//            httpDetQueue::iterator ithttpdq = arhttpDQTmp.begin();
//            while(ithttpdq != arhttpDQTmp.end())
//            {
//                PHTTPDOWNQUEUE pdqu =(PHTTPDOWNQUEUE) &(*ithttpdq);
//                arhttpdata.insert(pdqu);

//                if(arhttpdata.size() > DEFAULT_QUEUE_DATA)// delete mini node
//                {
//                    httpDQData::iterator ithdbe = arhttpdata.begin();
//                    PHTTPDOWNQUEUE pdqutmp = *ithdbe;
//                    if(pdqutmp->ctype==1)
//                    {
//                        pdqutmp->ctype = 0;
//                        //add download
//                        if(pdqutmp->pfd)
//                        {

//                            //phcd->m_deturl.addDetURL(pdqutmp->pfd);
//                            //delete pdqutmp->pfd;
//                        }
//                        pdqutmp->pfd = NULL;
//                    }
//                    arhttpdata.erase(ithdbe);

//                }

//                ithttpdq++;
//            }

//            //            httpDQData::iterator itaddtype = arhttpdata.begin();
//            //            while(itaddtype != arhttpdata.end())
//            //            {
//            //                PHTTPDOWNQUEUE pdqutmp = *itaddtype;
//            //                if(pdqutmp->ctype==0)
//            //                    pdqutmp->ctype = 1;
//            //                itaddtype++;
//            //            }

//            pthread_mutex_lock(&pWebSite->mtx);
//            arhttpDQTmp.swap(pWebSite->arhttpDQ);
//            pthread_mutex_unlock(&pWebSite->mtx);

//            /*httpDetQueue::iterator */ithttpdq = arhttpDQTmp.begin();
//            while(ithttpdq != arhttpDQTmp.end())
//            {
//                const httpdownqueue& pdqu = *ithttpdq;
//                //if(pdqu->pfd)
//                {
//                    //PCACHEFILE pDFile = pdqu->pfd;
//                    g_capture.AddhttpDownQueue(pdqu,0);//

//                }
//                ithttpdq++;
//            }
//        }

//    }
//}


//int HttpCMulti::Run(int type /*= 0*/)
//{
//    m_csetspeed = read_profile_int("http","setspeed",0,"./dw_config.ini");
//    LogFilePrintf("dwspeed","read setspeed:%d",m_csetspeed);
//    pthread_create(&m_thread,0,ThreadDownFile,this);

//    if(type)
//    {
//        while(1)
//            sleep(1);
//    }
//    return 0;
//}

//int HttpCMulti::End(PDOWNFILEINDO pDown)
//{
//    if(pDown)
//    {
//        mtxautolock mtxalock(m_pmtx);
//        if(pDown->fio>=0)
//            close(pDown->fio);
//        pDown->fio = -1;

//        long sockextr = -1;
//        CURLcode rescurl = curl_easy_getinfo(pDown->p, CURLINFO_LASTSOCKET, &sockextr);
//        if(rescurl==CURLE_OK)
//        {
//            //pDown->nlastsocket = sockextr;
//            struct sockaddr_in sa;
//            u_int len = sizeof(sa);
//            if(!getpeername(sockextr, (struct sockaddr *)&sa, &len))
//            {
//                pDown->nlastIP = sa.sin_addr.s_addr;
//                pDown->nLastPort = sa.sin_port;
//                //printf( "对方IP：%s ", inet_ntoa(sa.sin_addr));
//                //printf( "对方PORT：%d ", ntohs(sa.sin_port));
//            }
//        }

//        if(g_nDebug)printf("End:%u,%hd,%s\n",pDown->nid,pDown->nType,pDown->strPath.c_str());
//        m_EndLst.insert(pDown);
//        if(pDown->nType==0)
//            RestoreSQL(*pDown);
//    }
//    return 0;
//}

//bool HttpCMulti::isDownList(void* curl)
//{
//    DownFileList::iterator itdl = m_downlst.find(curl);
//    if(itdl != m_downlst.end())
//        return true;
//    return false;
//}

//HttpCMulti::HttpCMulti(HttpCDown& phcd):m_hcd(phcd)
//{
//    //gpHttpMulti.insert(this);
//    pthread_mutex_init(&m_cs,NULL);
//    pthread_mutex_init(&m_mtx,NULL);
//    m_pmtx = &m_mtx;
//    //m_id = 1;
//    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
//    if(code != CURLE_OK)
//    {
//        printf("init global failed\n");
//    }
//    m_multi_handle = curl_multi_init();
//    if(NULL == m_multi_handle)
//    {
//        printf("init download file failed\n");
//    }
//    //CURLMoption
//    //CURLOPT_MAX_RECV_SPEED_LARGE
//    //curl_multi_setopt(m_curlm,CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE,CURLOPT_MAX_RECV_SPEED_LARGE);
//}

//HttpCMulti::~HttpCMulti()
//{

//    curl_multi_cleanup(m_multi_handle);
//    curl_global_cleanup();

//    pthread_mutex_destroy(&m_cs);
//    pthread_mutex_destroy(&m_mtx);
//}

////int HttpCMulti::AddUrlHead(const char* pURL,int nUrlLen)
////{
////    AddUrl(pURL,nUrlLen,0,0);
////    return 0;
////}

//int HttpCMulti::GetSavePath(PCACHEFILE p,char* path,bool buc /*= true*/)
//{
//    int nresult = 0;
//    char szCRC64[21] = {0};
//    dw_uint642a(p->nCRC64,szCRC64);
//    if(p->szSaveDir[0] != '/')
//    {
//        path[0] = '/';
//        path += 1;
//    }
//    if(buc)
//    {
//        nresult = sprintf(path,"%s/%.2s/%s.uc",p->szSaveDir,szCRC64,szCRC64);
//    }
//    else
//        nresult = sprintf(path,"%s/%.2s/%s",p->szSaveDir,szCRC64,szCRC64);

//    if(nresult && buc)
//    {
//        char szDir[MAX_PATH] = {0};
//        char* pdir = szDir;
//        if(p->szSaveDir[0] != '/')
//        {
//            pdir[0] = '/';
//            pdir += 1;
//        }
//        sprintf(pdir,"%s/%.2s",p->szSaveDir,szCRC64);
//        if(0!= access(szDir,F_OK))
//        {
//            if((ENOTDIR==errno)||(ENOENT==errno))
//                if(my_mkdir(szDir,755) == -1)
//                {
//                    return 0;
//                }
//        }
//    }
//    //int ns = sprintf(pPath,"/%.1s/%s/%.2s/%s",pDisk,m_szHostHash,pHash,pHash);
//    return nresult;
//}


//bool dw_http_geturl20170630(void* pData)
//{
//    PCACHEFILE pNode = (PCACHEFILE)pData;
//    char szURI[HTTP_URL_SIZE]={0};
//    char *pTemp = NULL;
//    char szFileName[HTTP_URL_SIZE]={0};
//    if(strlen(pNode->szURI)>=HTTP_URL_SIZE) {
//        WriteLog("DWERROR",pNode->szURI);
//        return false;
//    }
//    strcpy(szURI,pNode->szURI);

//    if(pNode->nCacheType==0)
//    {
//        int i = strlen(pNode->szFileName)-1;
//        for(;i>0;i--)
//        {
//            if(pNode->szFileName[i]=='/')
//            {
//                break;
//            }
//        }
//        if(i>0)//截取完整保存的文件名的真实文件名
//            strcpy(szFileName,pNode->szFileName+i+1);
//        else
//            strcpy(szFileName,pNode->szFileName);
//    }
//    else
//    {
//        strcpy(szFileName,pNode->szFileName);
//    }

//    pTemp = strstr(szFileName,"--");//处理加了前缀的文件名
//    if(pTemp)
//    {
//        pTemp=pTemp+2;
//        pTemp = strstr(szURI,pTemp);
//    }
//    else
//    {
//        pTemp = strstr(szURI,szFileName);
//        if (!pTemp)
//        {
//            std::string temp = szFileName;
//            size_t nos = 0;
//            if ((nos=temp.find_last_of("."))!=string::npos)
//            {
//                if(nos>=HTTP_URL_SIZE){
//                    WriteLog("DWERROR",pNode->szURI);
//                    return false;
//                }
//                char filename[HTTP_URL_SIZE]={0};
//                strncpy(filename,temp.c_str(),nos);
//                pTemp = strstr(szURI,filename);
//            }
//        }
//    }

//    if(!pTemp)
//    {
//        return false;
//    }
//    else
//    {
//        pTemp = strstr(szURI,"?");
//        if(pTemp)
//        {
//            memset(pNode->szURI,0,sizeof(pNode->szURI));
//            if((pTemp-szURI)>=HTTP_URL_SIZE)return false;
//            memcpy(pNode->szURI,szURI,pTemp-szURI);
//        }
//    }
//    if(pTemp)
//    {
//        char* pJump = pNode->szURI;
//        int nJmpLen = pTemp-szURI;

//        //根据网站时间偏移自动处理参数以后可能吧时间偏移放进规则。
//        PWEBSITEEX pWebSite = pNode->pWebSite;
//        char * & pData = pTemp;//pPacket->http.pParam;
//        short int nLen = strlen(pTemp);

//        //char szJump[1600];
//        //char* pJump = szJump;
//        //memset(szJump,0,1600);
//        //int nJmpLen = 0;

//        bool bcpparam = true;
//        if(/*g_nDebug ||*/pWebSite->pT ||pWebSite->pO)
//        {
//            int nParNum = 0;
//            int nendadd = 0;
//            //char* pParam = pPacket->ptmp + pPacket->nTmpPos;
//            //pPacket->nTmpPos += 3001;
//            char pParam[3000] ;
//            memset(pParam,0,3000);
//            paramv1* pPar = (paramv1*)pParam;

//            pPar[0].pKeyPos = pData+1;
//            //short int &nLen = pPacket->http.nParamLen;

//            for(short int i = 0;i<nLen && i < 1200;i++)
//            {
//                if(pPar[nParNum].pKeyPos==NULL)
//                {
//                    pPar[nParNum].ntype = nParNum;
//                    pPar[nParNum].pKeyPos= pData + i;
//                }
//                else if(pPar[nParNum].pVlPos==NULL && pData[i]=='=')
//                {
//                    pPar[nParNum].pVlPos = pData + i + 1;
//                    pPar[nParNum].nKeyLen = pPar[nParNum].pVlPos-pPar[nParNum].pKeyPos;
//                }
//                else if(pPar[nParNum].nVlLen==0 && pData[i]=='&')
//                {
//                    bcpparam = false;
//                    if(nJmpLen >= HTTP_URL_SIZE)
//                    {
//                        nendadd =nParNum = 0;
//                        break;
//                    }

//                    pPar[nParNum].nVlLen = pData+i - pPar[nParNum].pVlPos;
//                    if(pPar[nParNum].pKeyPos==NULL ||pPar[nParNum].pVlPos==NULL
//                            ||pPar[nParNum].nKeyLen==0 /*|| pPar[nParNum].nVlLen==0*/
//                            || (pPar[nParNum].nVlLen+pPar[nParNum].nKeyLen+nJmpLen)>1500)
//                    {
//                        nendadd =nParNum = 0;
//                        break;
//                    }
//                    addwebsiteparam(pPar[nParNum],pWebSite,pJump,nJmpLen,1);

//                    nendadd = nParNum;
//                    if(nParNum > 100)
//                    {
//                        nendadd =nParNum = 0;
//                        break;
//                    }
//                    nParNum++;
//                    memset(&pPar[nParNum],0,sizeof(paramv1));

//                }
//            }
//            if(nendadd<nParNum && pPar[nParNum].pVlPos && (pPar[nParNum].nVlLen+pPar[nParNum].nKeyLen+nJmpLen)<1500)
//            {
//                pPar[nParNum].nVlLen = pData+nLen - pPar[nParNum].pVlPos;
//                addwebsiteparam(pPar[nParNum],pWebSite,pJump,nJmpLen,1);
//            }
//        }
//        if(bcpparam)
//        {
//            //memcpy(szJump+nJmpLen,"?",1);//strcat(szJump,"?");
//            //nJmpLen += 1;
//            memcpy(pJump+nJmpLen,pData,nLen);
//            nJmpLen += nLen;
//        }
//    }
//    return true;
//}



//int HttpCMulti::AddNode(PCACHEFILE p)
//{
//    if(m_setspeed<=0 || p==NULL)
//        return 0;
//    //if(pPacket->http.nParamLen)
//    //{
//    //    char szParam[HTTP_URL_SIZE];
//    //    strncpy(szParam,pPacket->http.pParam,pPacket->http.nParamLen);
//    //    bool br1 = RemoveParam(szParam,pWebSite->szDragTime);
//    //    bool br2 = RemoveParam(szParam,pWebSite->szDragOffset);
//    //    if(br1 || br2)
//    //    {
//    //    }
//    //}
//    PWEBSITEEX pWebSite = (PWEBSITEEX)(p->pWebSite);
//    if(pWebSite==NULL)
//        return 0;
//    //if(strncmp(pWebSite->szHost1,DW_SOHUP2P_HOST/*"sohu-p2p"*/,8)==0)
//    if(isSohuP2PDownload(pWebSite))
//    {
//        //printf("sohu download\n");
//    }else
//        if(strstr(pWebSite->szHost1,"-ts")==NULL)
//        {
//            if(!dw_http_geturl20170630(p))
//                return 0;
//        }
//    //if(dw_http_geturl(p))//old down url
//    {
//        char szSPath[MAX_PATH] = {0};
//        int nplen = GetSavePath(p,szSPath);
//        if(nplen)
//        {
//            return AddUrl(p,szSPath);
//            //AddUrl(p->szURI,strlen(p->szURI),szSPath,strlen(szSPath));
//            //printf("addurl:%s->%s\n",p->szURI,szSPath);
//            return 1;
//        }
//    }
//    return 0;
//}

//int HttpCMulti::AddUrl(PCACHEFILE p,string strSPath)
//{
//    //    string strURL(pURL,nUrlLen);
//    //    string strPath(pSPath,nSPathLen);
//    int nresult = 0;
//    pthread_mutex_lock(&m_cs);
//    pair<DownURLList::iterator,bool> paiadd = m_downURL.insert(make_pair(p,strSPath));
//    if(paiadd.second)
//    {
//        __sync_fetch_and_add(&m_In,1);
//        nresult =  1;
//    }
//    pthread_mutex_unlock(&m_cs);
//    return nresult;
//}

////int HttpCMulti::AddUrl(const char* pURL,int nUrlLen,const char* pSPath,int nSPathLen)
////{
////    string strURL(pURL,nUrlLen);
////    string strPath(pSPath,nSPathLen);
////    pthread_mutex_lock(&m_cs);
////    pair<DownURLList::iterator,bool> paiadd = m_downURL.insert(make_pair(strURL,strPath));
////    pthread_mutex_unlock(&m_cs);
////    if(paiadd.second)
////        __sync_fetch_and_add(&m_In,1);
////    return 0;
////}
//#include "dw_rule.h"

//int HttpCMulti::ReadTSURL(string& strUrl,string& strSPath,PCACHEFILE pdf)
//{
//    PCACHETSFILE pTS = pdf->pTS;
//    if(pTS->nfio == NULL)
//    {
//        pTS->nfio = fopen(pTS->szm3u8,"r");
//        //pTS->nfio = open(pTS->szm3u8,O_RDONLY,0);
//    }
//    if(pTS->nfio>0)
//    {
//        u_int64_t nm3u8CRC64 = CRC64((u_char*)pTS->szm3u8FileName,pTS->nm3u8len);
//        char sztmp[5001] = {0};
//        while(fgets(sztmp,5000,pTS->nfio))
//        {
//            if(sztmp[0]=='#')
//                continue;
//            if(sztmp[0])
//            {
//                int ntmplen = strlen(sztmp);
//                for(int i = 1;i<3;i++)//delete最后两位换行
//                {
//                    if(sztmp[ntmplen-i] == '\n' || sztmp[ntmplen-i] == '\r')
//                    {
//                        sztmp[ntmplen-i] = 0;
//                    }
//                    else
//                        break;
//                }

//                string strPathTmp = sztmp;
//                int nposts = strPathTmp.find(".ts");
//                if(nposts<2)
//                    continue;
//                nposts += 3;

//                if(pTS->arNamePre)
//                {
//                    CDataArray *pArPre = pTS->arNamePre;
//                    char szPre[200]={0};
//                    char *pData =sztmp + nposts;
//                    int nLen = strPathTmp.length() - nposts;
//                    for(int j=0;j<pArPre->GetArrayCount();j++)
//                    {
//                        PRULEKEY pNode = (PRULEKEY)pArPre->GetNode(j);
//                        if(pNode==NULL)
//                            continue;
//                        if(GetParamValue(pData,pNode->szKey,szPre+strlen(szPre),200,pNode->szParam,nLen)) //shenjiang 20150113 v4112//if(GetParamValue(pData,pNode->szKey,szPre+strlen(szPre),200,pNode->szParam))
//                        {
//                            pTS->index = atoi(szPre);
//                            strcat(szPre,"-");
//                        }
//                    }
//                    strcat(szPre,"-");
//                    int nPreLen = strlen(szPre);
//                    strPathTmp = szPre;
//                    strPathTmp += sztmp;
//                    nposts = strPathTmp.find(".ts");
//                    if(nposts<2)
//                        continue;
//                    nposts += 3;
//                }

//                pTS->index = atoi(strPathTmp.c_str());
//                if(pTS->index<0 || pTS->index>= 99999/*MAX_QQTS_COUNT*/)
//                    continue;
//                pTS->nCRC64 = CRC64((u_char*)strPathTmp.c_str(),nposts);
//                u_int64_t& ncrc64 = pTS->nCRC64;
//                FILENODE filenodecrc;
//                filenodecrc.nKeyCRC64 = ncrc64;

//                pthread_mutex_lock(&pdf->pWebSite->mtx);
//                setcrcnode::iterator itfcrc = pdf->pWebSite->arFileCRC.find(&filenodecrc);
//                pthread_mutex_unlock(&pdf->pWebSite->mtx);
//                if(itfcrc != pdf->pWebSite->arFileCRC.end())
//                {
//                    PFILENODE pfnode = *itfcrc;
//                    if(pfnode->ts.nM3u8CRC != nm3u8CRC64 || pfnode->ts.nM3u8ID != pTS->index)
//                    {
//                        //pfnode->ts.nM3u8CRC = nm3u8CRC64;
//                        //pfnode->ts.nM3u8ID = pTS->index;
//                        g_qqtsp2p.addqqtsidxfile(nm3u8CRC64,pTS->index,pfnode->ulFileSize,pTS->nCRC64,pTS->szm3u8+1/*pfnode->disk.szDisk*/);
//                    }
//                    continue;
//                }else
//                {
//                    g_qqtsp2p.UpdateM3u8id(nm3u8CRC64,pTS->index);
//                }

//                char szCRC64[21] = {0};
//                dw_uint642a(ncrc64,szCRC64);
//                char szPathTmp[MAX_PATH] = {0};
//                int nresult = sprintf(szPathTmp,"%s/%.2s/%s.uc",pdf->szSaveDir,szCRC64,szCRC64);
//                strSPath = szPathTmp;
//                //tsmkdir
//                memset(szPathTmp,0,MAX_PATH);
//                sprintf(szPathTmp,"%s/%.2s",pdf->szSaveDir,szCRC64);
//                if(0!= access(szPathTmp,F_OK))
//                {
//                    if((ENOTDIR==errno)||(ENOENT==errno))
//                        if(my_mkdir(szPathTmp,755) == -1)
//                        {
//                            //return 0;
//                        }
//                }

//                //int ns = sprintf(pTS->szm3u8,"/%.1s/%s/%.2s/%s",pdf->szDisk,m_qqts->szHostCRC,szCRC,szCRC);
//                strUrl = pTS->sztsPath;
//                strUrl += "/";
//                strUrl += sztmp;
//                if(pTS->ncodelen>0)
//                {
//                    strUrl += "&";
//                    strUrl += pTS->sztscode;
//                }
//                if(g_nDebug)
//                    LogFilePrintf("dwqqtsurl","create ts url:%s,%s",strSPath.c_str(),strUrl.c_str());
//                //update db info
//                if(pdf)
//                {
//                    memset(pdf->szURI,0,HTTP_URL_SIZE);
//                    int ncpurllen = strUrl.size();
//                    if(ncpurllen>=HTTP_URL_SIZE)
//                        ncpurllen = 1000;
//                    memcpy(pdf->szURI,strUrl.c_str(),ncpurllen);
//                    memcpy(pdf->szFileName,strPathTmp.c_str(),nposts);
//                }
//                return 1;
//            }
//            memset(sztmp,0,sizeof(sztmp));
//        }
//        fclose(pTS->nfio);
//        pTS->nfio = NULL;
//        g_qqtsp2p.Updatem3u8End(nm3u8CRC64);
//    }
//    return 0;
//}


//uint g_id = 1;//download file index
//int HttpCMulti::URL2CURL(int addcount)
//{
//    if(m_multi_handle == NULL || m_downURL.size()==0)
//        return 0;
//    int naddsc = 0;
//    for(int i = 0;i<addcount;i++)
//    {
//        if(m_downURL.size()==0)
//            return naddsc;

//        string strUrl;
//        string strSPath;
//        PCACHEFILE pdf = NULL;
//        pthread_mutex_lock(&m_cs);
//        DownURLList::iterator iturl = m_downURL.begin();
//        if(iturl != m_downURL.end())
//        {
//            pdf = iturl->first;
//            strUrl = iturl->first->szURI;
//            //if(g_nDebug)
//            //    LogFilePrintf("dwhttpmulti","URL:%s\n%s",strUrl.c_str(),pdf->szURI);
//            strSPath.swap(iturl->second);
//            m_downURL.erase(iturl);
//            m_downURL.erase(pdf);
//        }
//        pthread_mutex_unlock(&m_cs);
//        if(pdf == NULL)
//            continue;//return 0;

//        if(pdf->pTS && pdf->pTS->nType==1)
//        {
//            strSPath.clear();
//            strUrl.clear();
//            ReadTSURL(strUrl,strSPath,pdf);
//            //LogFilePrintf("dwqqtsurl","")
//        }
//        //
//        if(strUrl.size()==0 /*|| strSPath.size()==0*/)
//        {
//            RemoveFailed(pdf);
//            //无效下载清理
//            //delete pdf;
//            continue;//return 0;
//        }

//        CURL* curl = curl_easy_init();
//        if(NULL == curl)
//        {
//            RemoveFailed(pdf);
//            //无效下载清理
//            continue;//return 0;//CURLE_FAILED_INIT;
//        }
//        //DownFileInfo dfinfo;
//        PDOWNFILEINDO pdfinfo = new DownFileInfo;
//        if(pdfinfo == NULL)
//        {
//            RemoveFailed(pdf);
//            //无效下载清理
//            continue;//return 0;
//        }
//        memcpy(&pdfinfo->chefile,pdf,CACHEFILESIZE);
//        //        pdfinfo->pData = NULL;
//        pdfinfo->setspeed = 0;
//        pdfinfo->nDataLen = 0;
//        pdfinfo->strPath.swap(strSPath);
//        pdfinfo->fio = -1;
//        pdfinfo->pdf = &pdfinfo->chefile;
//        pdfinfo->p = curl;
//        pdfinfo->nhsize = 0;
//        pdfinfo->res_code = 0;
//        pdfinfo->nid = __sync_fetch_and_add(&g_id,1);
//        pdfinfo->ulRecvTime = pdfinfo->ulSTime = time(0);
//        pdfinfo->Content_Length = pdfinfo->ulWriteLength = 0;
//        pdfinfo->bChunked = false;
//        pdfinfo->nType = 1;
//        pdfinfo->user = this;
//        pdfinfo->speed = 0;
//        pdfinfo->nLocation = 0;
//        pdfinfo->listKA = NULL;
//        if(pdf->cReTry>0)
//        {
//            pdfinfo->Content_Length = pdf->lFileSize;
//            pdfinfo->ulWriteLength = pdf->lCacheSize;
//        }

//        pair<DownFileList::iterator,bool> pain =  m_downlst.insert(make_pair(curl,pdfinfo));
//        if(pain.second)
//        {
//            //LogFilePrintf("downloadsohuurl",pdfinfo->chefile.szURI);
//            adddcfc(3);
//            mtxautolock mtxalock(m_pmtx);
//            delete pdf;
//            pdf = &pdfinfo->chefile;
//            m_hcd.earsedflstNode(pdf->nCRC64);
//            curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
//            curl_easy_setopt(curl, CURLOPT_HEADER, 1);
//            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb_hcmulti_Data);
//            curl_easy_setopt(curl, CURLOPT_WRITEDATA, pain.first->second);
//            /**
//            * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
//            * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
//            */
//            //
//            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
//            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
//            //auto 302
//            //curl_easy_setopt(curl, CURLOPT_RETURNTRANSFER, 1);
//            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

//            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
//            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, assetsManagerProgressFunc);
//            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, pain.first->second);
//            if(pdfinfo->nid==1)
//            {
//                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 65535);
//                if(m_csetspeed==0)
//                    curl_easy_setopt(curl,CURLOPT_MAX_RECV_SPEED_LARGE,1024*1024);
//            }
//            else
//            {
//                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 65535);
//                if(m_csetspeed==0)
//                    curl_easy_setopt(curl,CURLOPT_MAX_RECV_SPEED_LARGE,1024*1024);
//            }
//            //            if(strlen(pdf->szReferer))
//            //            {
//            //                curl_easy_setopt(curl, CURLOPT_REFERER, pdf->szReferer);
//            //                //if(g_nDebug)printf("szReferer\n");
//            //            }
//            //            if(strlen(pdf->szUserAgent))
//            //            {
//            //                curl_easy_setopt(curl, CURLOPT_USERAGENT, pdf->szUserAgent);
//            //                //if(g_nDebug)printf("szUserAgent\n");
//            //            }
//            //            if(strlen(pdf->szCookie))
//            //            {
//            //                curl_easy_setopt(curl, CURLOPT_COOKIE, pdf->szCookie);
//            //                //if(g_nDebug)printf("szCookie\n");
//            //            }
//            //keepalibe//if(strlen(pdf->szCookie))
//            if(pdf->nFieldLen>0)
//            {
//                struct curl_slist *list = NULL;
//                //list = curl_slist_append(list, "Connection: keep-alive");
//                //list = curl_slist_append(list, "Connection: close");
//                list = curl_slist_append(list, pdf->szCookie);
//                curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);
//                pain.first->second->listKA = list;
//            }
//            else
//            {
//                struct curl_slist *list = NULL;
//                list = curl_slist_append(list, "Connection: close");
//                curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);
//                pain.first->second->listKA = list;
//            }
//            //CURLFORM_END
//            //curl_formadd
//            curl_easy_setopt(curl,CURLOPT_CHUNK_END_FUNCTION,chunk_end_callback);
//            //curl_easy_setopt(CURL *handle, CURLOPT_CHUNK_END_FUNCTION,chunk_end_callback);

//            if(pdf->szRange[0])
//            {
//                curl_easy_setopt(curl,CURLOPT_RANGE,pdf->szRange);
//                memset(pdf->szRange,0,sizeof(pdf->szRange));
//            }

//            if(pdfinfo->strPath.empty())
//            {
//                curl_easy_setopt(curl, CURLOPT_NOBODY, true);
//                //if(g_nDebug)printf("nobody\n");
//            }
//            //pthread_mutex_lock(&m_cs);
//            long sockextr = -1;
//            CURLcode rescurl = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sockextr);
//            if(rescurl==CURLE_OK)
//            {
//                //pdfinfo->nlastsocket = sockextr;
//                struct sockaddr_in sa;
//                u_int len = sizeof(sa);
//                if(!getpeername(sockextr, (struct sockaddr *)&sa, &len))
//                {
//                    pdfinfo->nlastIP = sa.sin_addr.s_addr;
//                    pdfinfo->nLastPort = sa.sin_port;
//                }
//            }
//            //...............
//            CURLMcode addcode = curl_multi_add_handle(m_multi_handle,curl);
//            if(addcode != CURLM_OK)
//                LogFilePrintf("dwhttpmulti","add:%d,URL:%s\n%s",addcode,pdf->szURI);
//            //pthread_mutex_unlock(&m_cs);
//            m_hcd.AddDownCount(pdf->nRuleID,pdf->pWebSite->szName);
//            AddDownUrl(pdfinfo);
//            naddsc++;
//            continue;//return pdf->nRuleID;//return CURLE_OK;
//        }
//        else
//        {
//            delete pdfinfo;
//            RemoveFailed(pdf);
//            //无效下载清理
//            continue;//return 0;//return CURLE_FAILED_INIT;
//        }
//    }
//    return naddsc;
//}

//int HttpCMulti::RemoveFailed(PCACHEFILE p)
//{
//    if(p)m_hcd.addFailedLst(p);

//    return -1;
//}


////int MyReadFile(int hFile,char *pData,int nOffset,int nLen)
////{
////    if(hFile==INVALID_HANDLE_VALUE)
////    {
////        return 0;
////    }
////    lseek(hFile,nOffset,SEEK_SET);//::SetFilePointer(hFile,nOffset,NULL,FILE_BEGIN);
////    //DWORD dwRead;
////    //read(hFile,pData,nLen);//::ReadFile(hFile,pData,nLen,&dwRead,NULL);
////    return read(hFile,pData,nLen);
////}

//bool HttpCMulti::SaveKeyFrame(const char *pFileName,unsigned long dwFileSize)
//{
//    int hIdxFile = 0;
//    char szUcFileName[HTTP_PATH_SIZE]={0};
//    char szIdxFileName[HTTP_PATH_SIZE]={0};

//    sprintf(szIdxFileName,"%s.idx",pFileName);
//    sprintf(szUcFileName,"%s.uc",szIdxFileName);


//    char szBuffer[33]={0};
//    char szData[4]={0};
//    unsigned long nDataSize = 0;
//    bool bIdxFile = false;
//    unsigned long nFilePos = 0;
//    KEYFRAME keyframe;
//    int hFile = open(pFileName,O_RDONLY,0);// CreateFileA(pFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
//    //if(!MyReadFile(hFile,szBuffer,0,32))
//    int nreds = pread(hFile,szBuffer,32,0);
//    if(nreds!=32)
//    {
//        return false;
//    }
//    //	DeCrypt(szBuffer,16,pKey);
//    if(memcmp(szBuffer,"FLV",3)==0)   //flv
//    {
//        hIdxFile= open(szUcFileName,O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);//CreateFileA(szUcFileName,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
//        if(hIdxFile==INVALID_HANDLE_VALUE)
//        {
//            char szLog[1024]={0};
//            sprintf(szLog,"idx error: %d %s",strerror(errno),pFileName);
//            WriteLog("Cache003",szLog);
//            close(hFile);
//            return false;
//        }

//        while(true)
//        {
//            if(nFilePos==0)
//            {
//                szData[0]=szBuffer[16];
//                szData[1]=szBuffer[15];
//                szData[2]=szBuffer[14];
//                szData[3]=0;
//                memcpy(&nDataSize,szData,4);
//                nFilePos=nDataSize+9+4+11+4;
//            }
//            if(nFilePos>=dwFileSize)
//            {
//                //CloseHandle(hIdxFile);
//                break;
//            }

//            memset(szBuffer,0,sizeof(szBuffer));
//            //if(!MyReadFile(hFile,szBuffer,nFilePos,16))
//            int nreds = pread(hFile,szBuffer,16,nFilePos);
//            if(nreds!=16)
//            {
//                break;
//            }

//            if(szBuffer[0]!=0x09&&szBuffer[0]!=0x8&&szBuffer[0]!=0x12)
//            {
//                break;
//            }

//            if(szBuffer[0]==0x9&&(szBuffer[11]>>4)==1)
//            {
//                szData[0]=szBuffer[6];
//                szData[1]=szBuffer[5];
//                szData[2]=szBuffer[4];
//                szData[3]=0;
//                memcpy(&keyframe.nTimeScale,szData,4);
//                keyframe.nFilePositions = nFilePos;
//                write(hIdxFile,&keyframe,KEYFRAMESIZE);//::WriteFile(hIdxFile,&keyframe,KEYFRAMESIZE,&dwWritten,NULL);
//                bIdxFile = true;
//            }

//            szData[0]=szBuffer[3];
//            szData[1]=szBuffer[2];
//            szData[2]=szBuffer[1];
//            szData[3]=0;
//            memcpy(&nDataSize,szData,4);
//            nFilePos+=nDataSize+11+4;
//        }
//    }

//    if(hFile>0)
//    {
//        close(hFile);
//        hFile=-1;
//    }
//    if(hIdxFile>0)
//    {
//        close(hIdxFile);
//        hIdxFile=-1;
//        if(bIdxFile)
//        {
//            rename(szUcFileName,szIdxFileName);//MoveFileA(szUcFileName,szIdxFileName);
//            return true;
//        }
//        else
//        {
//            remove(szUcFileName);// DeleteFile(szUcFileName);
//        }
//    }


//    return false;
//}

//void HttpCMulti::CheckDone()
//{
//    int         msgs_left;
//    CURLMsg *   msg;
//    if((msg = curl_multi_info_read(m_multi_handle, &msgs_left)))
//    {
//        if(CURLMSG_DONE == msg->msg)
//        {
//            if(msg->easy_handle)
//            {
//                DownFileList::iterator itdl = m_downlst.find(msg->easy_handle);
//                if(itdl != m_downlst.end())
//                {
//                    PDOWNFILEINDO pDown = itdl->second;
//                    if(msg->data.result==CURLE_OK)
//                    {
//                        bool bchukfilesize = false;
//                        PWEBSITEEX pWebSite = pDown->chefile.pWebSite;
//                        if(pWebSite)
//                        {
//                            ulong ulFileSize = pDown->ulWriteLength + pDown->nDataLen;
//                            if((pWebSite->nMinSize && ulFileSize < pWebSite->nMinSize*1024) || (pWebSite->nMaxSize && ulFileSize > pWebSite->nMaxSize*1024))
//                            {
//                                if(pDown->pdf->nRuleID!=RULE_ID_M3U8)
//                                    bchukfilesize = true;
//                            }
//                            //if((pWebToNode->nMinbyte&&pFileNodeCrc->ulFileSize<pWebToNode->nMinbyte) ||(pWebToNode->nMaxbyte&&pFileNodeCrc->ulFileSize>pWebToNode->nMaxbyte))
//                            //    return 2;
//                        }
//                        if(bchukfilesize)
//                        {
//                            pDown->nType = 9;
//                        }
//                        else
//                        {
//                            pDown->nType = 0;
//                            if(pDown->fio<0){
//                                pDown->fio = open(pDown->strPath.c_str(),O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
//                            }
//                            if(pDown->fio >= 0)
//                            {
//                                ssize_t ws1 = pwrite64(pDown->fio,pDown->pData,pDown->nDataLen,pDown->ulWriteLength);//20170706 add pDown->ulWriteLength
//                                if(ws1==pDown->nDataLen)
//                                {
//                                    pDown->ulWriteLength += pDown->nDataLen;
//                                    pDown->nDataLen = 0;
//                                }
//                                else
//                                {
//                                    pDown->nType = 21;
//                                }
//                            }
//                        }
//                    }
//                    else
//                    {
//                        if(pDown->nType<=1 && pDown->nType != -1)
//                            pDown->nType = 20;
//                        LogFilePrintf("dwhttpmulti","Done:%hu,%d,%u",pDown->nType,pDown->res_code,msg->data.result);
//                    }
//                    End(pDown);
//                    if(g_nDebug)
//                        LogFilePrintf("dwhttpmulti","Done:%hu,%lu,%s,%s\n",pDown->nType,pDown->Content_Length,pDown->strPath.c_str(),pDown->chefile.szURI);
//                }

//            }
//            //            int idx;
//            //            for (idx = 0; idx < num; ++idx)
//            //            {
//            //                if (msg->easy_handle == CurlArray[idx]) break;
//            //            }

//            //            if (idx == num)
//            //            {
//            //                cerr << "curl not found" << endl;
//            //            } else
//            //            {
//            //                cout << "curl [" << idx << "] completed with status: "
//            //                     << msg->data.result << endl;
//            //                cout << "rsp: " << RspArray[idx] << endl;
//            //            }
//        }
//    }

//}

//int HttpCMulti::RemoveEnd()
//{
//    if(m_EndLst.size()==0)
//        return 0;

//    //for(; itel != m_EndLst.end();)
//    while(!m_EndLst.empty())
//    {
//        mtxautolock mtxalock(m_pmtx);
//        EndList::iterator itel = m_EndLst.begin();
//        if(itel == m_EndLst.end())
//            return 0;
//        PDOWNFILEINDO pDown = *itel;
//        m_EndLst.erase(itel);
//        m_EndLst.erase(pDown);

//        __sync_fetch_and_add(&m_Out,1);
//        if(pDown->nType == 0)
//        {
//            struct stat st;
//            stat(pDown->strPath.c_str(), &st);
//            if(st.st_size == pDown->ulWriteLength)
//            {
//                char szPath[MAX_PATH] = {0};
//                GetSavePath(pDown->pdf,szPath,false);
//                SaveKeyFrame(szPath,pDown->Content_Length);
//            }
//            else
//                pDown->nType = 8;
//        }

//        //m_comlst.insert(make_pair(pDown->p,*pDown));
//        //pthread_mutex_lock(&m_cs);
//        //pthread_mutex_unlock(&m_cs);
//        if(pDown->p == NULL)
//        {
//            LogFilePrintf("dwhttpmulti","if(pDown->p == NULL)11");
//            return 0;
//        }
//        DownFileList::iterator itf = m_downlst.find(pDown->p);
//        if(itf != m_downlst.end())
//        {
//            m_downlst.erase(itf);
//            m_downlst.erase(pDown->p);

//            CURLMcode removecode = curl_multi_remove_handle(m_multi_handle,pDown->p);
//            if(CURLM_OK == removecode)
//            {
//                EndDownUrl(pDown);
//                m_hcd.DownCom(pDown);
//            }
//            else
//                LogFilePrintf("dwhttpmulti","remove handle failed:%d,URL:%s\n%s",removecode,pDown->pdf->szURI);
//        }
//    }
//    return 0;
//}

//int HttpCMulti::RemoveTimeOut()
//{
//    DownFileList::iterator itf = m_downlst.begin();
//    for(int i = 0;i < m_downlst.size();i++ ,itf++)
//    {
//        if(itf == m_downlst.end())
//            return 0;
//        else
//        {
//            CURL* pcurl = itf->first;
//            //DownFileInfo& dfinfo = itf->second;
//            PDOWNFILEINDO pdfinfo = itf->second;
//            if(pdfinfo->res_code==200)
//            {
//                if(pdfinfo->Content_Length && pdfinfo->Content_Length <= pdfinfo->ulWriteLength)
//                {
//                    if(g_nDebug) printf("remove to:%lu,content len:%lu\n",pdfinfo->pdf->nCRC64,pdfinfo->Content_Length);
//                    bool bchukfilesize = false;
//                    PWEBSITEEX pWebSite = pdfinfo->chefile.pWebSite;
//                    if(pWebSite)
//                    {
//                        ulong ulFileSize = pdfinfo->ulWriteLength + pdfinfo->nDataLen;
//                        if((pWebSite->nMinSize && ulFileSize < pWebSite->nMinSize*1024) || (pWebSite->nMaxSize && ulFileSize > pWebSite->nMaxSize*1024))
//                        {
//                            if(pdfinfo->pdf->nRuleID!=RULE_ID_M3U8)
//                                bchukfilesize = true;
//                        }
//                    }
//                    if(bchukfilesize)//20170802
//                        pdfinfo->nType = 9;
//                    else
//                        pdfinfo->nType = 0;
//                    End(pdfinfo);
//                }else
//                    if(time(0)-(pdfinfo->ulRecvTime)>30)
//                    {
//                        pdfinfo->nType = 7;
//                        End(pdfinfo);
//                        //m_downlst.erase(itf);
//                    }
//            }
//            else
//                //if(dfinfo.res_code==200)
//            {
//                if(time(0)-(pdfinfo->ulRecvTime)>20)
//                {
//                    pdfinfo->nType = 7;
//                    End(pdfinfo);
//                    //m_downlst.erase(itf);
//                }
//            }
//        }
//    }
//    return 2;
//}
////downlist
//void HttpCMulti::AddDownUrl(PDOWNFILEINDO pdfinfo)
//{
//    SYSTEMTIME st;
//    time_t t0=::GetLocalTime2(&st);
//    char sztime[100] = {0};
//    sprintf(sztime,"%04d%02d%02d%02d%02d%02d",st.tm_year,st.tm_mon,st.tm_mday,st.tm_hour,st.tm_min,st.tm_sec);

//    char szClientIP[50] = {0};
//    in_addr addrtmp;
//    addrtmp.s_addr = pdfinfo->pdf->lSIP;
//    strcpy(szClientIP,inet_ntoa(addrtmp));


//    char szSQL[3001] = {0};
//    int nsqlLen = snprintf(szSQL,3000,"INSERT INTO tbrestore(nid,szWebSite,szDisk,szfilename,nfilesize,nrestoresize,nspeed,dttime,szuri,nstatus,dtlasttime,ngettype,szclientip,sznode) VALUES(%u,'%s','%s','%s',%d,%d,%d,'%s','%s',%d,'%s',%d,'%s','%s');",
//                           pdfinfo->nid,pdfinfo->pdf->pWebSite->szHost1,pdfinfo->chefile.szSaveDir,pdfinfo->chefile.szFileName,0,0,0,
//                           sztime,pdfinfo->chefile.szURI,0,sztime,pdfinfo->chefile.nGetType,szClientIP,g_hostName);

//    char* pSQL =(char*) dwnew(nsqlLen,1);
//    if(pSQL)
//    {
//        memcpy(pSQL,szSQL,nsqlLen);
//        if(!PostMessage(WM_CACHE_SQL,(void*)pSQL))
//        {
//            ::WriteLog("SRunning","PostThreadMessage fail adddownurl");
//            delete []pSQL;//::FreeMemory(pSQL,g_pMem);
//        }
//    }

//}

//void HttpCMulti::UpdateDownUrl(PDOWNFILEINDO pdfinfo)
//{
//    SYSTEMTIME st;
//    time_t t0=::GetLocalTime2(&st);
//    char sztime[100] = {0};
//    sprintf(sztime,"%04d%02d%02d%02d%02d%02d",st.tm_year,st.tm_mon,st.tm_mday,st.tm_hour,st.tm_min,st.tm_sec);

//    unsigned int nSpeed = pdfinfo->speed/1024;
//    char szSQL[1001] = {0};
//    int nsqlLen = snprintf(szSQL,1000,"update tbRestore set nFileSize=%lu,nRestoreSize=%lu,nSpeed=%u,nStatus=%d,dtLastTime='%s' where nID = %u and szNode='%s';",
//                           pdfinfo->Content_Length,pdfinfo->ulWriteLength,nSpeed,1,sztime,pdfinfo->nid,g_hostName);

//    char* pSQL =(char*) dwnew(nsqlLen,1);
//    if(pSQL)
//    {
//        memcpy(pSQL,szSQL,nsqlLen);
//        if(!PostMessage(WM_CACHE_SQL,(void*)pSQL))
//        {
//            ::WriteLog("SRunning","PostThreadMessage fail updatedownurl");
//            delete []pSQL;//::FreeMemory(pSQL,g_pMem);
//        }
//    }
//}

//void HttpCMulti::EndDownUrl(PDOWNFILEINDO pdfinfo)
//{
//    char szSQL[1000] = {0};
//    int nsqlLen = snprintf(szSQL,900,"DELETE from tbrestore where sznode='%s' and nid=%u;",
//                           g_hostName,pdfinfo->nid);

//    char* pSQL =(char*) dwnew(nsqlLen,1);
//    if(pSQL)
//    {
//        memcpy(pSQL,szSQL,nsqlLen);
//        if(!PostMessage(WM_CACHE_SQL,(void*)pSQL))
//        {
//            ::WriteLog("SRunning","PostThreadMessage fail updatedownurl");
//            delete []pSQL;//::FreeMemory(pSQL,g_pMem);
//        }
//    }
//    //add cache file
//    if(pdfinfo->ulWriteLength >= pdfinfo->Content_Length && pdfinfo->res_code==200)
//    {
//        char *pSQL =(char*) dwnew(4096);
//        if(pSQL)
//        {
//            DownFileInfo& fDown = *pdfinfo;
//            //
//            RESTORE mRestore = {0};
//            mRestore.nSpeed = fDown.speed/1024;
//            mRestore.nRestoreSize = fDown.dlnow;//_nCachesized;
//            in_addr addrtmp;
//            addrtmp.s_addr = fDown.pdf->lSIP;
//            strcpy(mRestore.szClientIP,inet_ntoa(addrtmp));

//            mRestore.nFileSize = fDown.Content_Length;
//            mRestore.nGetType = fDown.pdf->nGetType;
//            mRestore.nCacheType = fDown.pdf->nCacheType;
//            strncpy(mRestore.szDisk,fDown.pdf->szSaveDir,HTTP_PATH_SIZE);
//            strncpy(mRestore.szFileName,fDown.pdf->szFileName,HTTP_PATH_SIZE);
//            strncpy(mRestore.szWebSite,fDown.pdf->pWebSite->szHost1,40);
//            strncpy(mRestore.szURI,fDown.pdf->szURI,HTTP_URL_SIZE-1);
//            //strncpy(mRestore.szHashCode,fDown.pdf->szFileHashCode,40);
//            dw_uint642a(fDown.pdf->nCRC64,mRestore.szHashCode);

//            memset(pSQL,0,4096);
//            g_dbop.WriteCacheFile(pSQL,&mRestore);/*WriteCacheFile*/
//            //printf("%s\n",pSQL);
//            if(!PostMessage(WM_CACHE_SQL,(void*)pSQL))
//            {
//                ::WriteLog("SRunning","PostThreadMessage fail(addcachefile)");
//                delete []pSQL;//::FreeMemory(pSQL,g_pMem);
//            }
//        }
//    }
//}

//void HttpCMulti::RestoreSQL(DownFileInfo& fDown)
//{
//    return UpdateDownUrl(&fDown);

//    if(fDown.Content_Length==0)
//        return;
//    //
//    RESTORE mRestore = {0};
//    mRestore.nSpeed = fDown.speed/1024;
//    mRestore.nRestoreSize = fDown.dlnow;//_nCachesized;
//    in_addr addrtmp;
//    addrtmp.s_addr = fDown.pdf->lSIP;
//    strcpy(mRestore.szClientIP,inet_ntoa(addrtmp));

//    mRestore.nFileSize = fDown.Content_Length;
//    mRestore.nGetType = fDown.pdf->nGetType;
//    mRestore.nCacheType = fDown.pdf->nCacheType;
//    strncpy(mRestore.szDisk,fDown.pdf->szSaveDir,HTTP_PATH_SIZE);
//    strncpy(mRestore.szFileName,fDown.pdf->szFileName,HTTP_PATH_SIZE);
//    strncpy(mRestore.szWebSite,fDown.pdf->pWebSite->szHost1,40);
//    strncpy(mRestore.szURI,fDown.pdf->szURI,HTTP_URL_SIZE-1);
//    //strncpy(mRestore.szHashCode,fDown.pdf->szFileHashCode,40);
//    dw_uint642a(fDown.pdf->nCRC64,mRestore.szHashCode);
//    char *pSQL =(char*) dwnew(4096);
//    if(pSQL)
//    {
//        memset(pSQL,0,4096);
//        g_dbop.WriteRestore(pSQL,&mRestore);/*WriteCacheFile*/
//        //printf("%s\n",pSQL);
//        if(!PostMessage(WM_CACHE_SQL,(void*)pSQL))
//        {
//            ::WriteLog("SRunning","PostThreadMessage fail(05)");
//            delete []pSQL;//::FreeMemory(pSQL,g_pMem);
//        }
//    }

//    if(fDown.ulWriteLength >= fDown.Content_Length)
//    {
//        char *pSQL =(char*) dwnew(4096);
//        if(pSQL)
//        {
//            memset(pSQL,0,4096);
//            g_dbop.WriteCacheFile(pSQL,&mRestore);/*WriteCacheFile*/
//            //printf("%s\n",pSQL);
//            if(!PostMessage(WM_CACHE_SQL,(void*)pSQL))
//            {
//                ::WriteLog("SRunning","PostThreadMessage fail(05)");
//                delete []pSQL;//::FreeMemory(pSQL,g_pMem);
//            }
//        }
//    }
//}

//int HttpCMulti::PrintfSpeed(int setspeed)
//{
//    if(m_downlst.size())
//    {
//        //mtxautolock mtxalock(m_pmtx);
//        DownFileList::iterator itb = m_downlst.begin();
//        for(;itb != m_downlst.end();itb++)
//        {
//            //DownFileInfo& fDown = itb->second;
//            PDOWNFILEINDO pdfinfo = itb->second;
//            //int ntimeout = fDown.ulRecvTime-fDown.ulSTime+100;
//            //curl_easy_setopt(fDown.p, CURLOPT_TIMEOUT, ntimeout);
//            //if(pdfinfo->nid==2)
//            //    setspeed = 1024*1024;
//            if(pdfinfo && pdfinfo->nType < 2)
//            {
//                if(pdfinfo->setspeed != setspeed)
//                {
//                    if(m_csetspeed==0)
//                        curl_easy_setopt(pdfinfo->p,CURLOPT_MAX_RECV_SPEED_LARGE,setspeed);
//                    pdfinfo->setspeed = setspeed;
//                }
//                curl_easy_getinfo(pdfinfo->p, CURLINFO_SPEED_DOWNLOAD, &pdfinfo->speed);
//                //printf("%x:%u:%lf  %f\n",m_thread,fDown.nid,fDown.speed,fDown.progress);
//                RestoreSQL(*pdfinfo);
//            }
//        }
//    }
//    return 0;
//}

////CURLOPT_NOPROGRESS
//int HttpCMulti::Wait()
//{
//    ulong ultimes = time(0);
//    ulong ultimee = time(0);
//    int still_running = 1;
//    ulong repeats = 0;
//    int setspeed = 0;

//    //curl_waitfd tmpwaitfd[10] = {0};
//    int nRID = 0;
//    while(true)
//    {
//        if(setspeed != m_setspeed)
//            LogFilePrintf("dwspeed","Wait setspeed:%d,%d,%d,%d",setspeed , m_setspeed,still_running,m_downlst.size());
//        setspeed = m_setspeed;
//        nRID = URL2CURL(200);
//        if(nRID)
//        {

//        }
//        RemoveEnd();

//        CURLMcode mc;
//        int numfds;

//        still_running = 0;
//        //pthread_mutex_lock(&m_cs);
//        mc = curl_multi_perform(m_multi_handle, &still_running);
//        //pthread_mutex_unlock(&m_cs);
//        if(mc == CURLM_OK ) {
//            /* wait for activity, timeout or "nothing" */
//            mc = curl_multi_wait(m_multi_handle, NULL, 0, 1000, &numfds);
//            m_Run = still_running;
//        }

//        if(mc != CURLM_OK) {
//            fprintf(stderr, "curl_multi failed, code %d.n", mc);
//            break;
//        }
//        CheckDone();

//        ultimee = time(0);
//        if(still_running && (ultimee - ultimes) > 10)
//        {
//            ultimes = ultimee;
//            int nruncount = still_running;
//            if(nruncount < m_downlst.size())
//                nruncount = m_downlst.size();

//            PrintfSpeed(setspeed/nruncount);
//        }
//        // if(still_running < m_downlst.size())
//        {
//            RemoveTimeOut();
//        }
//        /* 'numfds' being zero means either a timeout or no file descriptors to
//         wait for. Try timeout on first occurrence, then assume no file
//         descriptors and no file descriptors to wait for means wait for 100
//         milliseconds. */

//        if(!numfds) {
//            repeats++; /* count number of repeated zero numfds */
//            if(repeats > 10) {
//                //WAITMS(100); /* sleep 100 milliseconds */
//                usleep(50);
//                repeats = 0;
//            }
//        }
//        else
//            repeats = 0;

//    }
//    //while(still_running);
//    //gpMte = NULL;
//    return 0;
//}



//HttpCDown::HttpCDown()
//{

//    //pthread_mutex_init(&mte,0);
//    //gpMte =& mte;
//    //    DownFileInfo dfi;
//    //    dfi.pData = NULL;
//    //    dfi.nDataLen = 0;
//    //    dfi.res_code = 0;
//    //    for(int i = 0;i<100;i++)
//    //        cb_hcmulti_Data(pResponseT1[i],strlen(pResponseT1[i]),1,&dfi);
//    memset(&m_limitspeed,0,sizeof(m_limitspeed));
//    xs_mutex_init(&mutex_Queue);
//    xs_mutex_init(&mutex_del);
//    xs_mutex_init(&mutex_dflist);
//}

//HttpCDown::~HttpCDown()
//{
//    xs_mutex_destroy(&mutex_Queue);
//    xs_mutex_destroy(&mutex_del);
//    xs_mutex_destroy(&mutex_dflist);
//}

//int HttpCDown::Init(int ndownthread /*= 2*/)
//{
//    char szdetField[100] = "/var/log/DetField.dblog";
//    if(!m_deturl.Init(szdetField))
//    {
//        memset(szdetField,0,sizeof(szdetField));
//        strcpy(szdetField,"/tmp/DetField.dblog");
//        m_deturl.Init(szdetField);
//    }
//    g_dbop.ClearDownLoadInfo(g_hostName);

//    for(int i = 0;i<ndownthread;i++)
//    {
//        HttpCMulti* phc = new HttpCMulti(*this);
//        phc->Run();
//        m_arDown.AddNodeToTail(phc);
//    }

//    pthread_create(&m_thread,0,ThreadaddurlEx,this);
//}

//int HttpCDown::Add(PCACHEFILE pDFile)
//{
//    if(!pDFile)
//        return 0;

//    xs_mutex_lock(&mutex_dflist);
//    downloadlst::iterator itf = m_dflstint64.find(pDFile->nCRC64);
//    xs_mutex_unlock(&mutex_dflist);
//    if(itf == m_dflstint64.end() && m_Queuelst.size() < MAX_DOWNLOADFILE)
//        //if(m_Queuelst.size() < MAX_DOWNLOADFILE)
//    {
//        xs_mutex_lock(&mutex_Queue);
//        m_Queuelst.push(pDFile);
//        xs_mutex_unlock(&mutex_Queue);
//    }
//    else
//    {
//        if(pDFile->pTS)
//            delete pDFile->pTS;
//        pDFile->pTS = NULL;
//        delete pDFile;
//    }
//    return 0;
//}

//void HttpCDown::AutoClearList()
//{
//    while(m_Queuelst.size() > MAX_DOWNLOADFILE /*|| m_dflstint64.size() > MAX_DOWNLOADFILE*/)
//    {
//        PCACHEFILE pDelFile = Get();
//        if(pDelFile)delete pDelFile;
//    }
//}

//int HttpCDown::addInCurlLst_TS(PCACHEFILE p)
//{

//    return 0;
//}

//int HttpCDown::addInCurlLst(PCACHEFILE pdf)
//{
//    xs_mutex_lock(&mutex_dflist);
//    pair<downloadlst::iterator,bool> pain = m_dflstint64.insert(make_pair(pdf->nCRC64,pdf));
//    xs_mutex_unlock(&mutex_dflist);
//    if(pain.second)
//    {
//        if(pdf->pWebSite && (pdf->pWebSite->arDLFile.size() < pdf->pWebSite->nTasks || pdf->pWebSite->nCurTasks < pdf->pWebSite->nTasks))
//        {
//            if(AddUrl(pdf))
//            {
//                //pthread_mutex_t* pmtx = &pdf->pWebSite->mtx;
//                //pthread_mutex_lock(pmtx);
//                pdf->pWebSite->arDLFile.insert(make_pair(pdf->nCRC64,pdf));
//                //pthread_mutex_unlock(pmtx);
//                __sync_fetch_and_add(&pdf->pWebSite->nCurTasks,1);
//                return 1;
//            }

//        }
//        //else
//        {
//            xs_mutex_lock(&mutex_dflist);
//            m_dflstint64.erase(pdf->nCRC64);
//            xs_mutex_unlock(&mutex_dflist);
//        }
//    }else
//    {
//        //xs_mutex_lock(&mutex_dflist);
//        //downloadlst::iterator iffcrc = m_dflstint64.find(pdf->nCRC64);
//        //if(iffcrc != m_dflstint64.end())
//        //{
//        //if(iffcrc->second)
//        //    ((PCACHEFILE)iffcrc->second)->nClickHot++;
//        //}
//        //xs_mutex_unlock(&mutex_dflist);
//        //((PCACHEFILE)pain.first->second)->nClickHot++;
//    }
//    return 0;


//}

//int HttpCDown::SetSpeed(struct tm& st)
//{
//    int nWeekOfDay = 0;
//    if(st.tm_wday==0)
//        nWeekOfDay = 6;
//    else
//        nWeekOfDay = st.tm_wday-1;
//    int nspeedtmp = m_limitspeed[nWeekOfDay][st.tm_hour];

//    if(nspeedtmp>100 && m_arDown.GetArrayCount())
//        nspeedtmp /= m_arDown.GetArrayCount();
//    else if(nspeedtmp <= 100)
//        nspeedtmp = 0;
//    if(nspeedtmp>0)
//        nspeedtmp *= 1024;

//    for(int i = 0;i < m_arDown.GetArrayCount();i++)
//    {
//        HttpCMulti* phc =(HttpCMulti*) m_arDown.GetNode(i);
//        if(phc)
//        {
//            phc->SetSpeed(nspeedtmp);
//            LogFilePrintf("dwspeed","setspeed:%d",nspeedtmp);
//        }
//    }
//}

//int HttpCDown::AddUrl(PCACHEFILE pDFile)
//{
//    CDataArray arDownTmp;
//    arDownTmp.m_pCompareProc = CompareHttpCNode;
//    arDownTmp.SetOrder(ORDER_ASC,true);
//    //test
//    for(int i = 0;i < m_arDown.GetArrayCount();i++)
//    {
//        HttpCMulti* phc =(HttpCMulti*) m_arDown.GetNode(i);
//        arDownTmp.AddNodeByOrder(phc);
//        if(g_nDebug)printf("addurl %d:%d  ",i,phc->size());
//    }
//    if(g_nDebug)printf("\n");

//    if(arDownTmp.GetArrayCount()>0)
//    {
//        HttpCMulti* phc =(HttpCMulti*) arDownTmp.GetNode(0);
//        return phc->AddNode(pDFile);
//    }
//    return 0;
//}

//PCACHEFILE HttpCDown::Get()
//{
//    PCACHEFILE pResult = NULL;
//    xs_mutex_lock(&mutex_Queue);
//    if(m_Queuelst.size())
//    {
//        //m_Queuelst.push(pDownloadFile);
//        pResult = m_Queuelst.front();
//        m_Queuelst.pop();
//    }
//    xs_mutex_unlock(&mutex_Queue);


//    return pResult;
//}

//void HttpCDown::Data2list()
//{
//    //    int nsize = m_Queuelst.size();
//    //    for(int i = 0;i<nsize;i++)
//    //    {
//    //        PCACHEFILE pdf = Get();
//    //        if(!pdf)
//    //            return;
//    //        pair<downloadlst::iterator,bool> pain = m_dflstint64.insert(make_pair(pdf->nCRC64,pdf));
//    //        if(pain.second==false)
//    //        {
//    //            ((PCACHEFILE)pain.first->second)->nClickHot++;
//    //            delete pdf;
//    //        }
//    //    }
//}

//int HttpCDown::InitSpeed(int limitspeed[7][24])
//{
//    for(int i = 0;i<7;i++)
//    {
//        for(int j = 0; j<24; j++)
//        {
//            m_limitspeed[i][j] = limitspeed[i][j];
//        }
//    }
//    //memcpy(&m_limitspeed,&limitspeed,sizeof(m_limitspeed));
//    bReSetSpeed = true;
//    LogFilePrintf("dwspeed","init ReSetSpeed");
//}


//int HttpCDown::addFailedLst(PCACHEFILE pdf)
//{
//    xs_mutex_lock(&mutex_del);
//    m_dellst.push(pdf);
//    //pair<downloadlst::iterator,bool> pain = m_dellst.insert(make_pair(pdf->nCRC64,pdf));
//    xs_mutex_unlock(&mutex_del);
//}

//int HttpCDown::DelFailedDown()
//{
//    //for(int i = 0;i<m_dellst.size();i++)
//    while(!m_dellst.empty())
//    {
//        PCACHEFILE pDel = NULL;
//        xs_mutex_lock(&mutex_del);
//        pDel = m_dellst.front();
//        m_dellst.pop();
//        xs_mutex_unlock(&mutex_del);
//        if(pDel)
//        {
//            xs_mutex_lock(&mutex_dflist);
//            downloadlst::iterator itf = m_dflstint64.find(pDel->nCRC64);
//            if(itf != m_dflstint64.end())
//            {
//                pDel = itf->second;
//                m_dflstint64.erase(itf);
//            }
//            xs_mutex_unlock(&mutex_dflist);
//            if(pDel)
//            {
//                //pthread_mutex_t* pmtx = &pDel->pWebSite->mtx;
//                //pthread_mutex_lock(pmtx);
//                pDel->pWebSite->arDLFile.erase(pDel->nCRC64);
//                //pthread_mutex_unlock(pmtx);
//                __sync_fetch_and_sub(&pDel->pWebSite->nCurTasks,1);

//                if(pDel->pTS)
//                    delete pDel->pTS;
//                pDel->pTS = NULL;
//                delete pDel;
//            }
//        }
//    }
//}

//void HttpCDown::DownCom(PDOWNFILEINDO pDown)
//{
//    xs_mutex_lock(&mutex_del);
//    m_comlst.insert(make_pair(pDown->p,pDown));
//    xs_mutex_unlock(&mutex_del);
//}
//#include "dwp2pyouku.h"
//#include "dw_http_service.h"
//#include "dw_md5.h"
//extern dw_http_service g_http_service;
//int AddNodeinWebSiteV3(PWEBSITEEX pWebSite,PFILENODE pNode,bool breNew,bool bmd5);
//void HttpCDown::EndofDownLoad()
//{
//    if(m_comlst.empty())
//        return;//这里输出来错误，清理以后正常，加快清理
//    while(!m_comlst.empty())
//    {
//        CURL* pcurl = NULL;
//        //DownFileInfo downinfo;
//        PDOWNFILEINDO pdfinfo = NULL;
//        xs_mutex_lock(&mutex_del);
//        DownFileList::iterator itremove = m_comlst.begin();
//        if(itremove != m_comlst.end())
//        {
//            pcurl = itremove->first;
//            //downinfo = itremove->second;
//            pdfinfo = itremove->second;
//            m_comlst.erase(itremove);
//        }
//        xs_mutex_unlock(&mutex_del);
//        if(pcurl)
//        {
//            //        if(pdfinfo->pData)
//            //            free(pdfinfo->pData);
//            //        pdfinfo->pData = NULL;
//            if(pdfinfo->listKA)
//                curl_slist_free_all(pdfinfo->listKA);
//            pdfinfo->listKA = NULL;
//            //curl_free(downinfo.p);
//            //curl_easy_cleanup(downinfo.p);
//            //curl_easy_duphandle(downinfo.p);
//            pdfinfo->p = NULL;
//            curl_easy_cleanup(pcurl);
//            pcurl = NULL;

//            u_char cDownLoad200 = 0;
//            //        if(downinfo.fio >=0)
//            //            close(downinfo.fio);
//            //        downinfo.fio = -1;
//            PCACHEFILE pdf = pdfinfo->pdf;
//            switch (pdfinfo->nType) {
//            case -1:
//            {
//                cDownLoad200 = 1;
//                //AddDownStatus(pdf->nRuleID,200);
//                PFILENODE ptmp = NULL;
//                if(g_capture.FileAtLocal(pdf->nCRC64,ptmp,pdf->pWebSite))
//                {
//                    ptmp->nStatus = 0;
//                    g_http_service.send_cache_file_node(*ptmp);
//                }
//                break;
//            }
//            case 0:
//            {
//                ++g_flowcount.nDownLoadUrlNum;
//                if(pdfinfo->Content_Length != pdfinfo->ulWriteLength)
//                {
//                    AddDownStatus(pdf->nRuleID,0);
//                    remove(pdfinfo->strPath.c_str());
//                    LogFilePrintf("dwhttpmulti","http download failed:%lu,%lu",pdfinfo->Content_Length, pdfinfo->ulWriteLength);
//                    break;
//                }
//                cDownLoad200 = 1;
//                //AddDownStatus(pdf->nRuleID,200);
//                string strPathTmp;
//                strPathTmp.append(pdfinfo->strPath.c_str(),pdfinfo->strPath.length()-3);
//                if(rename(pdfinfo->strPath.c_str(),strPathTmp.c_str())==0)
//                {
//                    PFILENODE pnewfn = &m_newfn;
//                    if(pnewfn)
//                    {
//                        memset(pnewfn,0,FILENODESIZE);
//                        pnewfn->ver = 2;
//                        //filenode.nIndex = -1;
//                        if(pdf->szSaveDir[0] == '/')
//                            pnewfn->disk.szDisk[0] = pdf->szSaveDir[1];
//                        else
//                            pnewfn->disk.szDisk[0] = pdf->szSaveDir[0];
//                        strncpy(pnewfn->szHost1,pdf->pWebSite->szHost1,40);
//                        memcpy(pnewfn->szFileName,pdf->szFileHashCode,40);
//                        pnewfn->ulFileSize = pdfinfo->ulWriteLength;
//                        pnewfn->ulFileSize = pdfinfo->Content_Length;
//                        pnewfn->ft1.dwLowDateTime = time(0);//statbuff.st_ctime;
//                        pnewfn->ft2 = pnewfn->ft1;
//                        pnewfn->pWebSite = pdf->pWebSite;
//                        strncpy(pnewfn->szTerminalName,g_hostName,40);
//                        pnewfn->nKeyLen = strlen(pdf->szFileName);
//                        pnewfn->nKeyCRC64 = pdf->nCRC64;
//                        if(pdf->pTS && pdf->pTS->nType==1)
//                            pnewfn->nKeyCRC64 = pdfinfo->pdf->pTS->nCRC64;
//                        pnewfn->ndlip = pdfinfo->nlastIP;
//                        pnewfn->ndlport = pdfinfo->nLastPort;

//                        bool bmd5 = dwp2pYouKu::GetInstince().GetMd5(pnewfn,pdf->szFileName);
//                        if(g_sohu.sohuP2pCfg.nOn_off && isSohuP2PDownload(pdf->pWebSite))
//                        {
//                            GetFileSha1_sys(pnewfn,strPathTmp.c_str());
//                            //pnewfn->szFHash
//                        }
//                        AddNodeinWebSiteV3(pdf->pWebSite,pnewfn,true,bmd5);
//                        g_http_service.send_cache_file_node(*pnewfn);
//                        if(pdf && pdf->pTS && pdf->pTS->nType==1)
//                        {
//                            u_int64_t nm3u8CRC64 = CRC64((u_char*)pdf->pTS->szm3u8FileName,pdf->pTS->nm3u8len);
//                            g_qqtsp2p.addqqtsidxfile(nm3u8CRC64,pdf->pTS->index,pdfinfo->Content_Length,pdf->pTS->nCRC64,pdf->pTS->szm3u8+1/*pnewfn->disk.szDisk*/);

//                        }
//                        if(pdf->pTS && pdf->pTS->nType==0)
//                            pdf->pTS->nType = 1;
//                    }
//                }
//                else
//                {
//                    LogFilePrintf("dwhttpmulti","rename failed:%s",strPathTmp.c_str());
//                }
//                break;
//            }
//            case 1:
//            case 2:
//            case 3:
//            case 4:
//            case 5:
//            case 6:
//            case 7:
//            {
//            }
//            case 8:
//            default:
//            {
//                //int naddstatus = 0;
//                //if(pdfinfo->nType==9)
//                //    naddstatus = 1;
//                //else
//                //    naddstatus = pdfinfo->res_code;
//                //AddDownStatus(pdf->nRuleID,naddstatus);
//                static int nLogerrorCount =100;
//                char szField[HTTP_COOKIE_SIZE];
//                memcpy(szField,pdf->szCookie,HTTP_COOKIE_SIZE);
//                szField[HTTP_COOKIE_SIZE-1] = 0;
//                for(int i = 0;i<20;i++)
//                {
//                    char* pend = strstr(szField,"\r\n");
//                    if(pend)
//                    {
//                        memset(pend,'_',2);
//                    }
//                    else
//                        break;
//                }
//                //if(nLogerrorCount<1000)
//                LogFilePrintf("dwhttpmulti","down failed:%hu,%d,%lu,%lu,%d,%s,Location:%s,URL:%s,field:%d,%s,%d",pdfinfo->nType,pdfinfo->res_code
//                              ,pdfinfo->Content_Length,pdfinfo->ulWriteLength,pdfinfo->nDataLen,pdfinfo->strPath.c_str(),pdfinfo->strLocation.c_str(),pdf->szURI,pdf->nFieldLen,szField,nLogerrorCount++);
//                //remove(pdfinfo->strPath.c_str());
//                break;
//            }
//            }

//            //del
//            int badd = 0;
//            PCACHEFILE pDel = pdfinfo->pdf;
//            if(pDel && pDel->pTS && pdf->pTS->nType==1)
//            {
//                //g_qqtsp2p.addqqtsidxfile(pDel->nCRC64,pDel->pTS->index,pdfinfo->Content_Length,pDel->pTS->nCRC64);
//                if(g_nDebug)
//                    LogFilePrintf("dwqqtsurl","addts:%d,%lu,%lu,%s",pdfinfo->res_code,pdfinfo->Content_Length,pdfinfo->ulWriteLength,pdfinfo->strPath.c_str());
//                PCACHEFILE pAdd = new CACHEFILE;
//                memcpy(pAdd,pDel,sizeof(CACHEFILE));
//                if(pAdd->nRuleID==RULE_ID_M3U8)
//                    pAdd->nRuleID = RULE_ID_QQTS;

//                badd = AddUrl(pAdd);//add ts down file
//                if(badd)
//                {
//                    xs_mutex_lock(&mutex_dflist);
//                    downloadlst::iterator itf = m_dflstint64.find(pDel->nCRC64);
//                    if(itf != m_dflstint64.end())
//                    {
//                        itf->second = pAdd;
//                    }
//                    xs_mutex_unlock(&mutex_dflist);
//                }
//                else
//                {
//                    delete pAdd;
//                }
//                //URL2CURL()
//            }
//            else if(pDel && cDownLoad200==0 && pDel->cReTry < MAX_HTTP_DOWNLOAD && pdfinfo->ulWriteLength > pDel->lCacheSize && pdfinfo->ulWriteLength < pdfinfo->Content_Length)
//            {
//                ulong ulstartOffset = pdfinfo->ulWriteLength;// + pdfinfo->nDataLen;
//                memset(pDel->szRange,0,sizeof(pDel->szRange));
//                //sprintf(pDel->szRange,"Range: bytes=%ld-%ld",_nCurrRangePos,_nFilesize-1);
//                //sprintf(pDel->szRange,"bytes=%ld-%ld",ulstartOffset,pdfinfo->Content_Length-1);
//                sprintf(pDel->szRange,"%lu-%lu",ulstartOffset,pdfinfo->Content_Length-1);

//                pDel->lFileSize = pdfinfo->Content_Length;
//                pDel->lCacheSize = pdfinfo->ulWriteLength;
//                pDel->cReTry++;
//                PCACHEFILE pAdd = new CACHEFILE;
//                memcpy(pAdd,pDel,sizeof(CACHEFILE));
//                //if(pAdd->nRuleID==RULE_ID_M3U8)
//                //    pAdd->nRuleID = RULE_ID_QQTS;

//                badd = AddUrl(pAdd);//add ts down file
//                if(badd)
//                {
//                    cDownLoad200 = 2;
//                    xs_mutex_lock(&mutex_dflist);
//                    downloadlst::iterator itf =m_dflstint64.find(pDel->nCRC64);
//                    if(itf != m_dflstint64.end())
//                    {
//                        itf->second = pAdd;
//                    }
//                    xs_mutex_unlock(&mutex_dflist);
//                }
//                else
//                {
//                    delete pAdd;
//                }

//                if(g_nDebug)
//                {
//                    printf("download retray:%d\n",pDel->cReTry);
//                }
//            }
//            else
//            {
//                int njsonT = strncmp("{\"t\"",pdfinfo->pData,4);
//                if(pdfinfo->nType==9 && (pdfinfo->nDataLen+pdfinfo->ulWriteLength) > 4 && njsonT == 0 && pDel->cReQiyi<3)
//                {
//                    pdfinfo->nDataLen = 0;
//                    pdfinfo->ulWriteLength = 0;
//                    cJSON* pl = cJSON_Parse(pdfinfo->pData);
//                    if(pl)
//                    {
//                        cJSON* purl  = cJSON_GetObjectItem(pl,"l");
//                        if(purl && HTTP_URL_SIZE > strlen(purl->valuestring))
//                        {
//                            pDel->cReTry = 0;
//                            if(g_nDebug) printf("parse ok:%s,%s\n",purl->string,purl->valuestring);
//                            PCACHEFILE pAdd = new CACHEFILE;
//                            memcpy(pAdd,pDel,sizeof(CACHEFILE));
//                            memset(pAdd->szURI,0,HTTP_URL_SIZE);
//                            pAdd->nFieldLen = 0;
//                            memset(pAdd->szCookie,0,HTTP_COOKIE_SIZE);
//                            //memset(pAdd,0,sizeof(CACHEFILE));
//                            pAdd->cReQiyi = pDel->cReQiyi++;

//                            strcpy(pAdd->szURI,purl->valuestring);
//                            //if(pAdd->nRuleID==RULE_ID_M3U8)
//                            //    pAdd->nRuleID = RULE_ID_QQTS;

//                            badd = AddUrl(pAdd);//add ts down file
//                            if(badd)
//                            {
//                                xs_mutex_lock(&mutex_dflist);
//                                downloadlst::iterator itf = m_dflstint64.find(pDel->nCRC64);
//                                if(itf != m_dflstint64.end())
//                                {
//                                    itf->second = pAdd;
//                                }
//                                xs_mutex_unlock(&mutex_dflist);
//                            }
//                            else
//                            {
//                                delete pAdd;
//                            }
//                        }
//                        cJSON_Delete(pl);
//                    }
//                }
//                remove(pdfinfo->strPath.c_str());
//            }
//            if(cDownLoad200 >= 1)
//            {
//                AddDownStatus(pdf->nRuleID,200);
//            }
//            else
//            {
//                int naddstatus = 0;
//                if(pdfinfo->nType==9)
//                    naddstatus = 1;
//                else
//                    naddstatus = pdfinfo->res_code;
//                AddDownStatus(pdf->nRuleID,naddstatus);
//            }
//            if(pDel && 0==badd)
//            {
//                pdfinfo->pdf = NULL;
//                {
//                    //pthread_mutex_t* pmtx = &pDel->pWebSite->mtx;
//                    //pthread_mutex_lock(pmtx);
//                    pdf->pWebSite->arDLFile.erase(pDel->nCRC64);
//                    //pthread_mutex_unlock(pmtx);
//                    __sync_fetch_and_sub(&pDel->pWebSite->nCurTasks,1);
//                }
//                xs_mutex_lock(&mutex_dflist);
//                m_dflstint64.erase(pDel->nCRC64);
//                xs_mutex_unlock(&mutex_dflist);
//                /*downloadlst::iterator itf = m_dflstint64.erase(pDel->nCRC64);
//            if(itf != m_dflstint64.end())
//            {
//                pDel = itf->second;
//                m_dflstint64.erase(itf);
//                //if(pDel)delete pDel;
//            }
//            else
//            {
//                //LogFilePrintf("dwhttpmulti","del dflstint64 failed");
//            }*/

//            }

//            delete pdfinfo;
//        }
//    }
//}


//WEBSITEEX TmpWebSite;
//void HttpCDown::TestAdd()
//{
//    char * pdownurl1 = "http://172.20.1.100:8084/A/1330238813/01812606383846529236/txkj_url/1";
//    //char * pdownurl2 = "http://172.20.1.100:8084/A/1330238813/04154601981551710098/txkj_url/2";
//    for(int i = 0;i<200;i++)
//    {
//        if(i==199)
//            usleep(0);
//        PCACHEFILE pdown = new CACHEFILE;
//        if(pdown)
//        {
//            memset(pdown,0,sizeof(CACHEFILE));
//            pdown->nCRC64 = i;
//            pdown->pWebSite = &TmpWebSite;
//            strcpy(pdown->szSaveDir,"/A/testtest00");
//            strcpy(pdown->szURI,pdownurl1);
//            Add(pdown);
//        }
//    }

//}

//void HttpCDown::earsedflstNode(u_int64_t & ncrc64)
//{
//    xs_mutex_lock(&mutex_dflist);
//    downloadlst::iterator itf = m_dflstint64.find(ncrc64);
//    if(itf != m_dflstint64.end())
//    {
//        itf->second = NULL;
//    }
//    xs_mutex_unlock(&mutex_dflist);
//}

//void HttpCDown::UpdateWaiteDown()
//{
//    //int nridtest = m_deturl.GetRuleCount(12121);
//    ulong ulcenttime = time(0);
//    if((ulcenttime-ulWaitTime)>120)
//    {
//        map<int,DownLoadCnt>::iterator itdowncnt =  m_downcnt.begin();
//        while(itdowncnt != m_downcnt.end())
//        {
//            int nridcount = m_deturl.GetRuleCount(itdowncnt->first);
//            itdowncnt->second.nWaitDown = nridcount;
//            itdowncnt++;
//        }

//        ulWaitTime = ulcenttime;
//    }

//}

//void HttpCDown::AddDownCount(int & nRID,char* pName)
//{
//    map<int,DownLoadCnt>::iterator itdown = m_downcnt.find(nRID);
//    if(itdown != m_downcnt.end())
//    {
//        itdown->second.nCount++;
//    }
//    else
//    {
//        DownLoadCnt downloadcnttmp = {0};
//        downloadcnttmp.nRID = nRID;
//        downloadcnttmp.nCount = 1;
//        strncpy(downloadcnttmp.szHost,pName,20);
//        m_downcnt.insert(make_pair(nRID,downloadcnttmp));
//    }
//    m_cupdateDLtype = 1;
//}
//void HttpCDown::AddDownStatus(int & nRID,int nStatus)
//{
//    map<int,DownLoadCnt>::iterator itdown = m_downcnt.find(nRID);
//    if(itdown != m_downcnt.end())
//    {
//        m_cupdateDLtype = 1;
//        if(nStatus==1)
//            itdown->second.nFailMaxMin++;
//        else if(nStatus==200)
//            itdown->second.nSucessDown++;
//        else if(nStatus > 200 && nStatus < 300)
//            itdown->second.nFail206++;
//        else if(nStatus >= 300 && nStatus < 400)//if(nStatus == 302 || nStatus == 301)
//            itdown->second.nFail302++;
//        else if(nStatus >= 400 && nStatus < 500)
//            itdown->second.nFail403++;
//        else if(nStatus >= 500 && nStatus < 506)
//            itdown->second.nFail502++;
//        else
//            itdown->second.nFailOther++;
//    }
//}

//void HttpCDown::UpdateDB()
//{
//    if(m_downcnt.size() && m_cupdateDLtype)
//    {
//        if((time(0)-ulUpdatetime) > 100)
//        {
//            g_dbop.UpdateDownLoadLst(m_downcnt);
//            ulUpdatetime = time(0);
//            m_cupdateDLtype = 0;
//        }
//    }
//}


//void HttpCDown::PrintfDown()
//{
//    int nAllCurDown = 0;
//    for(int i = 0;i < m_arDown.GetArrayCount();i++)
//    {
//        HttpCMulti* phc =(HttpCMulti*) m_arDown.GetNode(i);
//        if(phc)
//        {
//            nAllCurDown += phc->downsize();
//            nAllCurDown += phc->downqueuesize();
//            LogFilePrintf("dwqqtsm3u8","httpDown:%d,%d,%d,%d,%d",i,phc->downsize(),phc->downqueuesize(),m_dflstint64.size(),m_comlst.size());
//            //if(g_nDebug)printf("addurl %d:%d  ",i,phc->size());
//        }
//    }

//    int nWaitDown = 0;
//    int nAllCurTasks = 0;
//    for(int a=0;a<m_arWebSite.GetArrayCount();a++)
//    {
//        PWEBSITEEX pWebSite = (PWEBSITEEX)m_arWebSite.GetNode(a);
//        if(pWebSite==NULL)
//            continue;
//        nAllCurTasks += pWebSite->nCurTasks;
//        nWaitDown += pWebSite->arhttpdata.size();
//        nWaitDown += pWebSite->arhttpDQ.size();
//        //LogFilePrintf("dwqqtsm3u8","httpTasks:%d,%s,%d,%hd,%hd",a,pWebSite->szHost1,pWebSite->arDLFile.size(),pWebSite->nCurTasks,pWebSite->nTasks);
//    }

//    //if((nAllCurTasks - nAllCurDown) > 100)
//    //{
//    //    for(int a=0;a<m_arWebSite.GetArrayCount();a++)
//    //    {
//    //        PWEBSITEEX pWebSite = (PWEBSITEEX)m_arWebSite.GetNode(a);
//    //        if(pWebSite==NULL)
//    //            continue;
//    //        pWebSite->nCurTasks = 0;
//    //        pWebSite->arDLFile.clear();
//    //    }
//    //    LogFilePrintf("dwqqtsm3u8","httpallTasks clear:%d",nAllCurTasks);
//    //}
//    //else
//    LogFilePrintf("dwqqtsm3u8","httpallTasks:%d,%d",nAllCurTasks,nWaitDown);
//}
