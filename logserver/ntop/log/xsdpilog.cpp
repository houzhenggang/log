#include"ntop_includes.h"
#include "lib/xs_prediction.h"
#include"lib/xssharefunc.h"
#include"lib/xshash.h"

#include<errno.h>
Mutex reslock;
#define GET_TIME(x)	time ((x))

#define MAX_PORT_ID 65535

#define InsertIP4_Log_Sql_FMT_For_Dns "insert into tbdnslog "\
    "(node,usr,srcip,srcport,dstip,dstport,mac,act,data1,l7,type,proto,date,time,snat_ip,snat_port)"  \
    "values " \
    "('%s','%s',%u,%d,%u,%d,'%s','%s','%s',%u,'%s','%s',%d,'%s',%u,%d);"

#define InsertIP4_Log_Sql_FMT_For_ID "insert into tbidlog "\
    "(node,usr,srcip,mac,act,type,date,time)"  \
    "values " \
    "('%s','%s',%u,'%s','%s','%s',%d,'%s');"


unsigned long long recvpacket = 0,parsepacket=0,dequeuepacket = 0,enqueuepacket = 0,mallocfail = 0;

void   MysqlGetPartition_InsertMap_Log(PMYDBOP pDB,char * tablename,struct rb_root * pMapPartition)
{
    char sqlPartition[256]={0};
    snprintf(sqlPartition,sizeof(sqlPartition)-1,"select  partition_name part, partition_expression expr, \
             partition_description descn, table_rows from information_schema.partitions\
             where table_schema=schema() and table_name='%s';",tablename);
            MYSQLRECORDSET GetQueryPartition={0};
    CSqlRecorDset_Init(&GetQueryPartition);
    CSqlRecorDset_SetConn(&GetQueryPartition,pDB->m_pDB);
    CSqlRecorDset_CloseRec(&GetQueryPartition);
    CSqlRecorDset_ExecSQL(&GetQueryPartition, sqlPartition);
    if(CSqlRecorDset_GetRecordCount(&GetQueryPartition)==0){
        CSqlRecorDset_CloseRec(&GetQueryPartition);
        CSqlRecorDset_Destroy(&GetQueryPartition);
        return ;
    }

    int i,nPartition=0;
    struct rb_node * node_iter=NULL;
    char szPartition[10] = {0};

    for(i=0; i< CSqlRecorDset_GetRecordCount(&GetQueryPartition);i++)
    {
        CSqlRecorDset_GetFieldValue_String(&GetQueryPartition,"descn",szPartition,10);
        nPartition = atoi(szPartition);
        if(ntop->g_xsDebug)  printf("Partition:%d Table:%s\n",nPartition,tablename);
        node_iter = int_void_rbsearch(pMapPartition,nPartition);
        if(node_iter&&ntop->g_xsDebug)
            printf("find to success Table:%s\n",tablename);
        else{
            node_iter = int_void_rbinsert(pMapPartition,nPartition,NULL);
            if(node_iter&&ntop->g_xsDebug)
                printf("Partition insert Faild %d Table:%s\n",nPartition,tablename);
        }
        CSqlRecorDset_MoveNext(&GetQueryPartition);
    }
    CSqlRecorDset_CloseRec(&GetQueryPartition);
    CSqlRecorDset_Destroy(&GetQueryPartition);
    return ;
}

void XsWriteLog_FailedAnalysis(char *pPopBuf){
    WriteLog("FailedAnalysis",pPopBuf);
    ntop->nFailedAnalysis++;
}

void XsFree_pPopBuf(char *pPopBuf){
    if(pPopBuf){
        free(pPopBuf);
        pPopBuf=NULL;
    }
}

//void   MyDBOp_Add_Partition(struct rb_root * pMapPartition,char * szSqlAdd,u_int bufDate,char * tablename){
//    struct rb_node *node;
//    node= int_void_rbsearch(pMapPartition,bufDate);
//    if(NULL==node){
//        sprintf(szSqlAdd,"alter table %s add  partition (partition p%d values in (%u));",tablename,bufDate,bufDate);
//        if(ntop->g_xsDebug)  printf("%s\n",szSqlAdd);
//        if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,szSqlAdd))
//            WriteLog("writemysql",szSqlAdd);
//        node = int_void_rbinsert(pMapPartition,bufDate,NULL);
//        if(!node){
//            if(ntop->g_xsDebug)  printf("insert success:p%d Table:%s\n",bufDate,tablename);
//        }else{
//            if(ntop->g_xsDebug)  printf("Insert Faild:%d Table:%s\n",bufDate,tablename);
//        }
//    }
//}

//void XsDeal_MainTain_Partition(struct rb_root *pMapPartition,char * tablename){
//    int nPartitionCount=0;
//    char szSqlDrop[128]={0};
//    struct rb_node *node;
//    int n;

//    nPartitionCount=pMapPartition->ncount;//查看有多少分区
//    if(ntop->g_xsDebug)  printf("%d Table:%s\n",nPartitionCount,tablename);

//    if(pMapPartition->ncount>90){
//        node=rb_first(pMapPartition);
//        if(node){
//            n = node->key;
//            if(ntop->g_xsDebug)  printf("erase the first one :%d Table:%s\n",n,tablename);
//            rb_erase(node, pMapPartition);

//            sprintf(szSqlDrop,"alter table %s drop partition p%d;",tablename,n);
//            if(ntop->g_xsDebug)  printf("%s\n",szSqlDrop);
//            if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,szSqlDrop))
//                WriteLog("writemysql",szSqlDrop);
//        }
//    }
//}

//void XsDBOp_Insert_DwLog(PXsWork pxswork,char * szSqlAdd,u_int bufDate,char *szSqlQuery){
//    MyDBOp_Add_Partition(&pxswork->mapPartitionFileDw,szSqlAdd,bufDate,"tbfiledwlog");

//    bool sql_return=MyDBOp_ExecSQL_1(&ntop->g_xsMySql,szSqlQuery);
//    if(!sql_return)
//        WriteLog("writemysql",szSqlQuery);
//    XsDeal_MainTain_Partition(&pxswork->mapPartitionFileDw,"tbfiledwlog");

//}

//void XsDBOp_Insert_UrlLog(PXsWork pxswork,char * szSqlAdd,u_int bufDate,char *szSqlQuery){

//    MyDBOp_Add_Partition(&pxswork->mapPartitionUrl,szSqlAdd,bufDate,"tburllog");

//    bool sql_return=MyDBOp_ExecSQL_1(&ntop->g_xsMySql,szSqlQuery);
//    if(!sql_return)
//        WriteLog("writemysql",szSqlQuery);
//    XsDeal_MainTain_Partition(&pxswork->mapPartitionUrl,"tburllog");
//}

//void XsDBOp_Insert_IDLog(PXsWork pxswork,char * szSqlAdd,u_int bufDate,char *szSqlQuery){
//    MyDBOp_Add_Partition(&pxswork->mapPartitionID,szSqlAdd,bufDate,"tbidlog");

//    bool sql_return=MyDBOp_ExecSQL_1(&ntop->g_xsMySql,szSqlQuery);
//    if(!sql_return)
//        WriteLog("writemysql",szSqlQuery);
//    XsDeal_MainTain_Partition(&pxswork->mapPartitionID,"tbidlog");
//}

//void XsDBOp_Insert_DnsLog(PXsWork pxswork,char * szSqlAdd,u_int bufDate,char *szSqlQuery){
//    MyDBOp_Add_Partition(&pxswork->mapPartitionDns,szSqlAdd,bufDate,"tbdnslog");

//    bool sql_return=MyDBOp_ExecSQL_1(&ntop->g_xsMySql,szSqlQuery);
//    if(!sql_return)
//        WriteLog("writemysql",szSqlQuery);
//    XsDeal_MainTain_Partition(&pxswork->mapPartitionDns,"tbdnslog");
//}

//void XsDBOp_Insert_OthLog(PXsWork pxswork,char * szSqlAdd,u_int bufDate,char *szSqlQuery){

//    MyDBOp_Add_Partition(&pxswork->mapPartition,szSqlAdd,bufDate,"tbnetlog");

//    bool sql_return=MyDBOp_ExecSQL_1(&ntop->g_xsMySql,szSqlQuery);
//    if(!sql_return)
//        WriteLog("writemysql",szSqlQuery);
//    XsDeal_MainTain_Partition(&pxswork->mapPartition,"tbnetlog");
//}

void XsParse_Special_Url_ForQQWeChat(int  l7,char *Url,size_t UrlLen,pAccountInfo pInfo){
    u_int64_t nID=0;

    if(1099==l7){//QQ file
        pInfo->chatType=qq_file;
        memcpy(pInfo->nametype,"qq_file",strlen("qq_file"));
    }else if(1407==l7){//QQ vedio  http://183.3.253.79/qqdownload?ver=5537&rkey=...&filetype=1001&auto_type=50&videotype=1&subvideotype=0
        char  *p = NULL;
        char *q = NULL;
        p = xs_strnstr2(Url+20,"ver=5537",UrlLen-20,8);
        if(p != NULL){
            q =  xs_strnstr2(p+15,"&filetype=1001&auto_type=50&videotype=1&subvideotype=0",UrlLen-20-15,strlen("&filetype=1001&auto_type=50&videotype=1&subvideotype=0"));
            if(q != NULL){
                pInfo->chatType=qq_video;
                memcpy(pInfo->nametype,"qq_video",strlen("qq_video"));
            }
        }
    }else if(1408==l7){//QQ picture  http://shp.qpic.cn/collector/475029929/34033d70-4a12-40be-95c3-0fe42f5cc8a4/140
        if(UrlLen>18&&(memcmp(Url+7,"shp.qpic.cn",11)==0)){
            pInfo->chatType=qq_picture;
            memcpy(pInfo->nametype,"qq_picture",strlen("qq_picture"));

            char  *p = NULL;
            p = xs_strnstr(Url+18,"collector/",UrlLen-18,10);
            if(p != NULL){
                nID=strtoul(p+10,NULL,10);
                snprintf(pInfo->id,31,"%lu",nID);
            }
        }
    }else if(1978==l7){//WeChat picture
        if(0 == memcmp(Url+7,"vweixinf.tc.qq.com",strlen("vweixinf.tc.qq.com"))){//http://vweixinf.tc.qq.com/102/20202/snsvideodownload?filekey=30270201010420301e0201660402535a0410844c3dd876867e9a28bc1ff2efb6088702032527f10400&bizid=1023&hy=SZ&fileparam=302c020101042530230204338bc63502045860cf9c02024eea02031e903a02032dc6c002040edc380a0201000400
            pInfo->chatType=wechat_video;
            memcpy(pInfo->nametype,"wechat_video",strlen("wechat_video"));
        }else{
            pInfo->chatType=wechat_picture;
            memcpy(pInfo->nametype,"wechat_picture",strlen("wechat_picture"));
        }
    }
}

int  XsGet_Type(char *filed,char *pPopBuf,char *type,char *types,size_t len){
    memset(filed,0,len);
    GetParamValue(pPopBuf,types,filed,len,";,",0);
    if(strlen(filed)==0){
        GetParamValue(pPopBuf,type,filed,len,";,",0);
        if(strlen(filed)==0) {
            if(ntop->g_xsDebug)
                XsWriteLog_FailedAnalysis(pPopBuf);
            return -1;
        }
    }
    if(ntop->g_xsDebug)  printf("%s\n",filed);
    return 0;
}

int  XsGet_Oth_Filed(char *filed,char *pPopBuf,char *param,size_t len){
    memset(filed,0,len);
    GetParamValue(pPopBuf,param,filed,len,";,",0);
    if(strlen(filed)==0){
        if(ntop->g_xsDebug)
            XsWriteLog_FailedAnalysis(pPopBuf);
        return -1;
    }

    //if(ntop->g_xsDebug)  printf("%s\n",filed);
    return 0;
}

int XsGet_Ip_And_Port(char * pPopBuf,char * buf,char * ip,int *port){
    char *q=buf;
    memset(ip,0,20);
    char *pPos = strstr(q,":");
    if(pPos) {
        strncpy( ip,q,pPos - q);
        q = pPos + 1;
    }else{
        return -1;
    }
    *port = atoi(q);
    return 0;
}

int XsGet_Usr_Or_snat(char *filed,char *pPopBuf,char *param,size_t len){
    memset(filed,0,len);
    GetParamValue(pPopBuf,param,filed,len,";,",0);
    if(strlen(filed)==0){
        return -1;
    }
    if(ntop->g_xsDebug) printf("%s\n",filed);
    return 0;
}

void XsAccountInfo_Init(pAccountInfo paccountInfo){
    paccountInfo->chatType=unknow;
    memset(paccountInfo->id,0,sizeof(paccountInfo->id));
    memset(paccountInfo->nametype,0,sizeof(paccountInfo->nametype));
}

void XsPhoneMsg_Init(PhoneMsg *phoneMsg){
    phoneMsg->imei = 0;
    phoneMsg->imsi = 0;
    memset(phoneMsg->device_type,0,sizeof(phoneMsg->device_type));
    memset(phoneMsg->wifi_ip_address,0,sizeof(phoneMsg->wifi_ip_address));
    memset(phoneMsg->wifi_ssid,0,sizeof(phoneMsg->wifi_ssid));
    memset(phoneMsg->device_brand,0,sizeof(phoneMsg->device_brand));
    memset(phoneMsg->bssid,0,sizeof(phoneMsg->bssid));
    memset(phoneMsg->mac,0,sizeof(phoneMsg->mac));
}

int XsGet_File_DwUrl(char * url,int urllen){
    char * res = NULL;
    res=xs_strnstr(url+(urllen-5),".exe",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-5),".apk",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-5),".ipa",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-5),"npk",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-5),".iod",5,4);
    if(res) return 1;
    return 0;
}

int XsGet_Picture_DwUrl(char * url,int urllen){
    char * res = NULL;
    res=xs_strnstr(url+(urllen-5),".jpg",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-5),".png",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-6),".jpeg",6,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-5),".bmp",5,4);
    if(res) return 1;
    return 0;
}

int XsGet_Picture_File_DwUrl(char * url,int urllen){
    char * res = NULL;
    res=xs_strnstr(url+(urllen-5),".jpg",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-5),".png",5,4);
    if(res) return 1;
    res=xs_strnstr(url+(urllen-6),".jpeg",6,4);
    if(res) return 1;
    res=xs_strnstr(url,".exe",urllen,4);
    if(res) return 2;
    res=xs_strnstr(url,".apk",urllen,4);
    if(res) return 2;
    res=xs_strnstr(url,".ipa",urllen,4);
    if(res) return 2;

    return -1;

}

int XsGet_MobilePhone_TianQiTong_Msg_From_Url(char * url,int urllen,PhoneMsg * phoneMsg){
    int  ret = 0,len = 0;
    char * p = NULL,*q = NULL,*r = NULL,*w = NULL;
    char mac[32] = {0};
    char a[3] = {0},b[3] = {0},c[3] = {0},d[3] = {0},e[3] = {0},f[3] = {0};
    p = xs_strnstr(url,"mac=",urllen-16,4);
    q = xs_strnstr(url,"device=",urllen-16,7);
    r = xs_strnstr(url,"ip=",urllen-16,3);

    if(p != NULL){
        w = xs_strnstr(p,"&",urllen-(p+4-url),1);
        if(w){
            len = (sizeof(mac)>(w-p-4))?(w-p-4):sizeof(mac);
            memcpy(mac,p+4,len);
        }else
            sscanf(p+4,"%32s",mac);
        //int a,b,c;
        //sscanf("2015.04.05", "%d.%d.%d", &a,&b,&c); //取需要的字符串
        //printf("a=%d,b=%d,c=%d",a,b,c);  //  a=2015,b=4,c=5
        if(6 == sscanf(mac,"%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s",a,b,c,d,e,f)){
            snprintf(phoneMsg->mac,sizeof(phoneMsg->mac) - 1,"%s:%s:%s:%s:%s:%s",a,b,c,d,e,f);
            ret = 1;
        }
    }
    if(q != NULL){
        w = xs_strnstr(q,"&",urllen-(q+7-url),1);
        if(w != NULL){
            len = (sizeof(phoneMsg->device_type)>(w-q-7))?(w-q-7):sizeof(phoneMsg->device_type);
            memcpy(phoneMsg->device_type,q+7,len);
        }else
            sscanf(q+7,"%32s",phoneMsg->device_type);
        urldecode(phoneMsg->device_type); //解码后
        ret = 1;

    }
    if(r != NULL){
        w = xs_strnstr(r,"&",urllen-(r+3-url),1);
        if(w){
            len = (sizeof(phoneMsg->wifi_ip_address)>(w-r-3))?(w-r-3):sizeof(phoneMsg->wifi_ip_address);
            memcpy(phoneMsg->wifi_ip_address,r+3,len);
        }else
            sscanf(r+3,"%32s",phoneMsg->wifi_ip_address);
        ret = 1;

    }
    return ret;
}

int XsGet_MobilePhone_TouTiao_Msg_From_Url(char * url,int urllen,PhoneMsg * phoneMsg){
    int ret = 0,len = 0;
    char * p = NULL,*q = NULL,*r = NULL,*ptr = NULL,*w = NULL;
    p = xs_strnstr(url,"wifi_ssid=",urllen-19,10);
    q = xs_strnstr(url,"wifi_ip_address=",urllen-19,16);
    r = xs_strnstr(url,"device_type=",urllen-19,12);
    ptr = xs_strnstr(url,"device_brand=",urllen-19,13);
    if(p != NULL){
        w = xs_strnstr(p,"&",urllen-(p+10-url),1);
        if(w != NULL){
            len = (sizeof(phoneMsg->wifi_ssid)>(w-p-13-3))?(w-p-13-3):sizeof(phoneMsg->wifi_ssid);
            memcpy(phoneMsg->wifi_ssid,p+13,len);
        } else
            sscanf(p+13,"%32s",phoneMsg->wifi_ssid);
        urldecode(phoneMsg->wifi_ssid); //解码后
        ret = 1;
    }
    if(q != NULL){
        w = xs_strnstr(q,"&",urllen-(q+16-url),1);
        if(w != NULL){
            len = (sizeof(phoneMsg->wifi_ip_address)>(w-q-16))?(w-q-16):sizeof(phoneMsg->wifi_ip_address);
            memcpy(phoneMsg->wifi_ip_address,q+16,len);
        }else
            sscanf(q+16,"%20s",phoneMsg->wifi_ip_address);
        ret = 1;
    }
    if(r != NULL){
        w = xs_strnstr(r,"&",urllen-(r+12-url),1);
        if(w){
            len = (sizeof(phoneMsg->device_type)>(w-r-12))?(w-r-12):sizeof(phoneMsg->device_type);
            memcpy(phoneMsg->device_type,r+12,len);
        }else
            sscanf(r+12,"%32s",phoneMsg->device_type);
        urldecode(phoneMsg->device_type); //解码后
        ret = 1;
    }
    if(ptr != NULL){
        w = xs_strnstr(ptr,"&",urllen-(ptr+13-url),1);
        if(w){
            len = (sizeof(phoneMsg->device_brand)>(w-ptr-13))?(w-ptr-13):sizeof(phoneMsg->device_brand);
            memcpy(phoneMsg->device_brand,ptr+13,len);
        }else
            sscanf(ptr+13,"%32s",phoneMsg->device_brand);
        urldecode(phoneMsg->device_brand); //解码后
        ret = 1;
    }
    return ret;
}

int  XsGet_MobilePhone_TianTianKuaiBao_Msg_From_Url(char * url,int urllen,PhoneMsg * phoneMsg){
    int len = 0,ret = 0,nLen = 0;
    char * p = NULL,*q = NULL,*r = NULL,*ptr = NULL,*x = NULL,*y = NULL,*w = NULL;
    char a[3] = {0},b[3] = {0},c[3] = {0},d[3] = {0},e[3] = {0},f[3] = {0},mac[32] = {0},bssid[32];
    if(0 == memcmp(url+7,"s.inews.gtimg.com",strlen("s.inews.gtimg.com"))||
            0 == memcmp(url+7,"r.cnews.qq.com",strlen("r.cnews.qq.com"))){
        p = xs_strnstr(url,"hw=",urllen-22,3);
        q = xs_strnstr(url,"mac=",urllen-22,4);
        r = xs_strnstr(url,"origin_imei=",urllen-22,12);
        ptr =xs_strnstr(url,"bssid=",urllen-22,6);
        x = xs_strnstr(url,"imsi_history=",urllen-22,13);
        y = xs_strnstr(url,"ssid=",urllen-22,5);

        if(p != NULL){
            w = xs_strnstr(p,"&",urllen-(p+3-url),1);
            if(w){
                nLen = (sizeof(phoneMsg->device_type)>(w-p-3))?(w-p-3):sizeof(phoneMsg->device_type);
                memcpy(phoneMsg->device_type,p+3,nLen);
            }else
                sscanf(p+3,"%32s",phoneMsg->device_type);
            urldecode(phoneMsg->device_type); //解码后
            ret = 1;
        }
        if(q != NULL){
            w = xs_strnstr(q,"&",urllen-(q+4-url),1);
            if(w){
                nLen = (sizeof(mac)>(w-q-4))?(w-q-4):sizeof(mac);
                memcpy(mac,q+4,nLen);
            } else
                sscanf(q+4,"%32s",mac);
            memcpy(phoneMsg->mac,mac,strlen(phoneMsg->mac));
            if(6 == sscanf(mac,"%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s",a,b,c,d,e,f)){
                memset(phoneMsg->mac,0,sizeof(phoneMsg->mac));
                snprintf(phoneMsg->mac,sizeof(phoneMsg->mac) - 1,"%s:%s:%s:%s:%s:%s",a,b,c,d,e,f);
                ret = 1;
            }
        }
        if(r != NULL){
            phoneMsg->imei = strtol(r+12,NULL,10);
            ret = 1;
            //            w = xs_strnstr(r,"&",urllen-(r+12-url),1);
            //            if(w)
            //                memcpy(phoneMsg->imei,r+12,w-r-12);
            //            else
            //                sscanf(r,"%32s",phoneMsg->imei);
        }else{
            r = xs_strnstr(url,"qimei=",urllen-22,6);
            if(r!= NULL){
                w = xs_strnstr(r,"&",urllen-(r+6-url),1);
                if(w){
                    len = w-r-6;
                    if(len<16){
                        phoneMsg->imei = strtol(r+6,NULL,10);
                        ret = 1;
                    }
                }else{
                    len = urllen - (r+6-url);
                    if(len < 16){
                        phoneMsg->imei = strtol(r+6,NULL,10);
                        ret = 1;
                    }
                }
            }
        }

        if(ptr != NULL){
            w = xs_strnstr(ptr,"&",urllen-(ptr+6-url),1);
            if(w){
                nLen = (sizeof(bssid)>(w-ptr-6))?(w-ptr-6):sizeof(bssid);
                memcpy(bssid,ptr+6,nLen);
            }else
                sscanf(ptr+6,"%32s",bssid);
            if(6 == sscanf(bssid,"%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s",a,b,c,d,e,f)){
                snprintf(phoneMsg->bssid,sizeof(phoneMsg->bssid) - 1,"%s:%s:%s:%s:%s:%s",a,b,c,d,e,f);
                ret = 1;
            }
        }
        if(x != NULL){
            phoneMsg->imsi = strtol(x+13,NULL,10);
            ret = 1;
            //            w = xs_strnstr(x,"&",urllen-(x+13-url),1);
            //            if(w)
            //                memcpy(phoneMsg->imsi,x+13,w-x-13);
            //            else
            //                sscanf(x,"%32s",phoneMsg->imsi);
        }else{
            x = xs_strnstr(url,"imsi=",urllen-22,5);
            if(x != NULL){
                phoneMsg->imsi = strtol(x+5,NULL,10);
                ret = 1;
                //                w = xs_strnstr(x,"&",urllen-(x+5-url),1);
                //                if(w)
                //                    memcpy(phoneMsg->imsi,x+5,w-x-5);
                //                else
                //                    sscanf(x,"%32s",phoneMsg->imsi);
            }
        }
        if(y != NULL){
            w = xs_strnstr(y,"&",urllen-(y+5-url),1);
            if(w){
                nLen = (sizeof(phoneMsg->wifi_ssid)>(w-y-5))?(w-y-5):sizeof(phoneMsg->wifi_ssid);
                memcpy(phoneMsg->wifi_ssid,y+5,nLen);
            }else
                sscanf(y+5,"%32s",phoneMsg->wifi_ssid);
            urldecode(phoneMsg->wifi_ssid);
            ret = 1;
        }
    }else if(0 == memcmp(url+7,"sdkconfig.video.qq.com",strlen("sdkconfig.video.qq.com"))){
        p = xs_strnstr(url,"device=",urllen-30,7);
        q = xs_strnstr(url,"mac=",urllen-30,4);
        r = xs_strnstr(url,"imei=",urllen-30,5);
        x = xs_strnstr(url,"imsi=",urllen-30,5);
        if(p != NULL){
            w = xs_strnstr(p,"&",urllen-(p+7-url),1);
            if(w){
                nLen = (sizeof(phoneMsg->device_type)>(w-p-7))?(w-p-7):sizeof(phoneMsg->device_type);
                memcpy(phoneMsg->device_type,p+7,nLen);
            }else
                sscanf(p+7,"%32s",phoneMsg->device_type);
            urldecode(phoneMsg->device_type); //解码后
            ret = 1;
        }
        if(q != NULL){
            w = xs_strnstr(q,"&",urllen-(q+4-url),1);
            if(w){
                nLen = (sizeof(mac)>(w-q-4))?(w-q-4):sizeof(mac);
                memcpy(mac,q+4,nLen);
            } else
                sscanf(q+4,"%32s",mac);
            memcpy(phoneMsg->mac,mac,strlen(mac));
            if(6 == sscanf(mac,"%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s",a,b,c,d,e,f)){
                memset(phoneMsg->mac,0,sizeof(phoneMsg->mac));
                snprintf(phoneMsg->mac,sizeof(phoneMsg->mac) - 1,"%s:%s:%s:%s:%s:%s",a,b,c,d,e,f);
                ret = 1;
            }
        }
        if(r!= NULL){
            phoneMsg->imei = strtol(r+5,NULL,10);
            ret = 1;
            //            w = xs_strnstr(r,"&",urllen-(r+5-url),1);
            //            if(w)
            //                memcpy(phoneMsg->imei,r+5,w-r-5);
            //            else
            //                sscanf(r,"%32s",phoneMsg->imei);
        }
        if(x != NULL){
            phoneMsg->imsi = strtol(x+5,NULL,10);
            ret = 1;
            //            w = xs_strnstr(x,"&",urllen-(x+5-url),1);
            //            if(w)
            //                memcpy(phoneMsg->imsi,x+5,w-x-5);
            //            else
            //                sscanf(x,"%32s",phoneMsg->imsi);
        }
    }
    else if(0 == memcmp(url+7,"mcgi.v.qq.com",strlen("mcgi.v.qq.com"))){
        p = xs_strnstr(url,"device_type=",urllen-21,12);
        if(p != NULL){
            w = xs_strnstr(p,"&",urllen-(p+12-url),1);
            if(w){
                nLen = (sizeof(phoneMsg->device_type)>(w-p-12))?(w-p-12):sizeof(phoneMsg->device_type);
                memcpy(phoneMsg->device_type,p+12,nLen);
            }else
                sscanf(p+12,"%32s",phoneMsg->device_type);
            urldecode(phoneMsg->device_type); //解码后
            ret = 1;
        }
    }
    return ret;
}

int XsGet_MobilePhone_YouKu_Msg_From_Url(char * url,int urllen,PhoneMsg * phoneMsg){
    int ret = 0,len = 0;
    char * p = NULL,*q = NULL,*r = NULL,*w = NULL ,*x = NULL;
    char mac[32] = {0};
    char a[3] = {0},b[3] = {0},c[3] = {0},d[3] = {0},e[3] = {0},f[3] = {0};

    p = xs_strnstr(url+19,"imei=",urllen-19,5);//imei
    q = xs_strnstr(url+19,"mac=",urllen-19,4);//mac  fmt:b0%3A...
    r = xs_strnstr(url+19,"ip=",urllen-19,3);//ip
    x = xs_strnstr(url+19,"device=",urllen-19,7);//device


    if(p != NULL){
        phoneMsg->imei = strtol(p+5,NULL,10);
        ret = 1;
    }else{
        p = xs_strnstr(url+19,"im=",urllen-19,3);
        if(p != NULL){
            phoneMsg->imei = strtol(p+3,NULL,10);
            ret = 1;
        }
    }
    if(q != NULL){
        w = xs_strnstr(q,"&",urllen-(q+4-url),1);
        if(w){
            len = (sizeof(mac)>(w-q-4))?(w-q-4):sizeof(mac);
            memcpy(mac,q+4,len);
        }else
            sscanf(q+4,"%32s",mac);
        memcpy(phoneMsg->mac,mac,strlen(mac));
        if(6 == sscanf(mac,"%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s",a,b,c,d,e,f)){
            memset(phoneMsg->mac,0,sizeof(phoneMsg->mac));
            snprintf(phoneMsg->mac,sizeof(phoneMsg->mac) - 1,"%s:%s:%s:%s:%s:%s",a,b,c,d,e,f);
            ret = 1;
        }
    }
    if(r != NULL){
        w = xs_strnstr(r,"&",urllen-(r+3-url),1);
        if(w){
            len = (sizeof(phoneMsg->wifi_ip_address)>(w-r-3))?(w-r-3):sizeof(phoneMsg->wifi_ip_address);
            memcpy(phoneMsg->wifi_ip_address,r+3,len);
        }else
            sscanf(r+3,"%32s",phoneMsg->wifi_ip_address);
        ret = 1;
    }else{
        r = xs_strnstr(url+19,"client_ip=",urllen-19,10);//ip
        if(r != NULL){
            w = xs_strnstr(r,"&",urllen-(r+10-url),1);
            if(w){
                len = (sizeof(phoneMsg->wifi_ip_address)>(w-r-10))?(w-r-10):sizeof(phoneMsg->wifi_ip_address);
                memcpy(phoneMsg->wifi_ip_address,r+10,len);
            }else
                sscanf(r+10,"%32s",phoneMsg->wifi_ip_address);
            ret = 1;
        }
    }
    if(x != NULL){
        w = xs_strnstr(x,"&",urllen-(x+7-url),1);
        if(w){
            len = (sizeof(phoneMsg->device_type)>(w-x-7))?(w-x-7):sizeof(phoneMsg->device_type);
            memcpy(phoneMsg->device_type,x+7,len);
        }else
            sscanf(x+7,"%32s",phoneMsg->device_type);
        urldecode(phoneMsg->device_type); //解码后
        ret = 1;
    }else if(NULL != xs_strnstr(url+19,"device_brand=",urllen-19,13)){
        x = xs_strnstr(url+19,"device_brand=",urllen-19,13);//device
        w = xs_strnstr(x,"&",urllen-(x+13-url),1);
        if(w){
            len = (sizeof(phoneMsg->device_type)>(w-x-13))?(w-x-13):sizeof(phoneMsg->device_type);
            memcpy(phoneMsg->device_type,x+13,len);
        }else
            sscanf(x+13,"%32s",phoneMsg->device_type);
        urldecode(phoneMsg->device_type); //解码后
        ret = 1;
    }else{
        x = xs_strnstr(url+19,"bd=",urllen-19,3);//device
        if(x != NULL){
            w = xs_strnstr(x,"&",urllen-(x+3-url),1);
            if(w){
                len = (sizeof(phoneMsg->device_type)>(w-x-3))?(w-x-3):sizeof(phoneMsg->device_type);
                memcpy(phoneMsg->device_type,x+3,len);
            }else
                sscanf(x+3,"%32s",phoneMsg->device_type);
            urldecode(phoneMsg->device_type); //解码后
            ret = 1;
        }
    }
    return ret;
}
int   XsGet_MobilePhone_AiQiYi_Msg_From_Url(char * url,int urllen,PhoneMsg * phoneMsg){
    int ret = 0,len = 0;
    char * p = NULL,*q = NULL,*r = NULL,*w = NULL ,*x = NULL;
    char mac[32] = {0};
    char a[3] = {0},b[3] = {0},c[3] = {0},d[3] = {0},e[3] = {0},f[3] = {0};

    if(0 == memcmp(url+7,"msg.71.am",strlen("msg.71.am"))){
        p = xs_strnstr(url+17,"wsc_sm=",urllen-17,7);//mac fmt:98-E7-B6-34-H8-97
        q = xs_strnstr(url+17,"wsc_ldt=",urllen-17,8);//device
        r = xs_strnstr(url+17,"wsc_imei=",urllen-17,9);//imei
        x = xs_strnstr(url+17,"wsc_iip=",urllen-17,8);//ip
        if(p != NULL){
            w = xs_strnstr(p,"&",urllen-(p+7-url),1);
            if(w){
                len = (sizeof(phoneMsg->mac)>(w-p-7))?(w-p-7):sizeof(phoneMsg->mac);
                memcpy(phoneMsg->mac,p+7,len);
            }else
                sscanf(p+7,"%32s",phoneMsg->mac);
            ret = 1;
        }else if(NULL != xs_strnstr(url+17,"macid=",urllen-17,6)){
            p = xs_strnstr(url+17,"macid=",urllen-17,6);
            w = xs_strnstr(p,"&",urllen-(p+6-url),1);
            if(w){
                len = (sizeof(mac)>(w-p-6))?(w-p-6):sizeof(mac);
                memcpy(mac,p+6,len);
            }else
                sscanf(p+6,"%32s",mac);

            if(6 == sscanf(mac,"%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s%*3s%2s",a,b,c,d,e,f)){
                snprintf(phoneMsg->mac,sizeof(phoneMsg->mac) - 1,"%s:%s:%s:%s:%s:%s",a,b,c,d,e,f);
                ret = 1;
            }
        }else{
            p = xs_strnstr(url+17,"n_mac=",urllen-17,6);//mac fmt:98-E7-B6-34-H8-97
            if(p != NULL){
                w = xs_strnstr(p,"&",urllen-(p+6-url),1);
                if(w){
                    len = (sizeof(phoneMsg->mac)>(w-p-6))?(w-p-6):sizeof(phoneMsg->mac);
                    memcpy(phoneMsg->mac,p+6,len);
                }else
                    sscanf(p+6,"%32s",phoneMsg->mac);
                ret = 1;
            }
        }
        if(q != NULL){
            w = xs_strnstr(q,"&",urllen-(q+8-url),1);
            if(w){
                len = (sizeof(phoneMsg->device_type)>(w-q-8))?(w-q-8):sizeof(phoneMsg->device_type);
                memcpy(phoneMsg->device_type,q+8,len);
            }else
                sscanf(q+8,"%32s",phoneMsg->device_type);
            urldecode(phoneMsg->device_type); //解码后
            ret = 1;
        }
        if(r != NULL){
            phoneMsg->imei = strtol(r+9,NULL,10);
            ret = 1;
        }else {
            r = xs_strnstr(url+17,"imei=",urllen-17,5);
            if(r != NULL){
                phoneMsg->imei = strtol(r+5,NULL,10);
                ret = 1;
            }
        }
        if(x != NULL){
            w = xs_strnstr(x,"&",urllen-(x+8-url),1);
            if(w){
                len = (sizeof(phoneMsg->wifi_ip_address)>(w-x-8))?(w-x-8):sizeof(phoneMsg->wifi_ip_address);
                memcpy(phoneMsg->wifi_ip_address,x+8,len);
            }else
                sscanf(x+8,"%32s",phoneMsg->wifi_ip_address);
            ret = 1;
        }
    }else if(0 == memcmp(url+7,"iface2.iqiyi.com",strlen("iface2.iqiyi.com"))){
        p = xs_strnstr(url+24,"mac=",urllen-24,4);//mac
        q = xs_strnstr(url+24,"dev_ua=",urllen-24,7);//device
        //  r = xs_strnstr(url+24,"qyid=",urllen-24,5);//imei
        //        x= xs_strnstr(url+24,"dev_hw=",urllen-24,7);//device url 解码

        if(p != NULL){
            w = xs_strnstr(p,"&",urllen-(p+4-url),1);
            if(w){
                len = (sizeof(phoneMsg->mac)>(w-p-4))?(w-p-4):sizeof(phoneMsg->mac);
                memcpy(phoneMsg->mac,p+4,len);
            }else
                sscanf(p+4,"%32s",phoneMsg->mac);
            ret = 1;
        }
        if(q != NULL){
            w = xs_strnstr(q,"&",urllen-(q+7-url),1);
            if(w){
                len = (sizeof(phoneMsg->device_type)>(w-q-7))?(w-q-7):sizeof(phoneMsg->device_type);
                memcpy(phoneMsg->device_type,q+7,len);
            }else
                sscanf(q+7,"%32s",phoneMsg->device_type);
            urldecode(phoneMsg->device_type); //解码后
            ret = 1;
        }
        //        if(r != NULL){
        //            phoneMsg->imei = strtol(r+5,NULL,10);
        //            ret = 1;
        //        }else if(NULL != xs_strnstr(url+24,"aqyid=",urllen-24,6)){
        //            r = xs_strnstr(url+24,"aqyid=",urllen-24,6);
        //            phoneMsg->imei = strtol(r+6,NULL,10);
        //            ret = 1;
        //        }else {
        //            r = xs_strnstr(url+24,"cupid_uid=",urllen-24,10);
        //            if(NULL != r){
        //                phoneMsg->imei = strtol(r+10,NULL,10);
        //                ret = 1;
        //            }
        //        }
        //        if(x != NULL){
        //            w = xs_strnstr(x,"&",urllen-(x+7-url),1);
        //            if(w){
        //                len = (sizeof(phoneMsg->device_type)>(w-x-7))?(w-x-7):sizeof(phoneMsg->device_type);
        //                memcpy(phoneMsg->device_type,x+7,len);
        //            } else
        //                sscanf(x+7,"%32s",phoneMsg->device_type);
        //            urldecode(phoneMsg->device_type); //解码后
        //            ret = 1;
        //        }
    }

    return ret;
}

int XsGet_MobilePhone_MoJi_Msg_From_Url(char * url,int urllen,PhoneMsg * phoneMsg){
    int ret = 0,len = 0;
    char * p = NULL,*q = NULL,*r = NULL,*w = NULL;
    p = xs_strnstr(url,"Device=",urllen-17,7);
    q = xs_strnstr(url,"MAC=",urllen-17,4);
    r = xs_strnstr(url,"IMEI=",urllen-17,5);

    if(p != NULL){
        w = xs_strnstr(p,"&",urllen-(p+8-url),1);
        if(w){
            len = (sizeof(phoneMsg->device_type)>(w-p-7))?(w-p-7):sizeof(phoneMsg->device_type);
            memcpy(phoneMsg->device_type,p+7,len);
        }else
            sscanf(p+7,"%32s",phoneMsg->device_type);
        urldecode(phoneMsg->device_type); //解码后
        ret = 1;
    }
    if(q != NULL){
        w = xs_strnstr(q,"&",urllen-(q+4-url),1);
        if(w != NULL){
            len = (sizeof(phoneMsg->mac)>(w-q-4))?(w-q-4):sizeof(phoneMsg->mac);
            memcpy(phoneMsg->mac,q+4,len);
        }else
            sscanf(q+4,"%32s",phoneMsg->mac);
        ret = 1;
    }
    if(r != NULL){
        phoneMsg->imei = strtol(r + 5,NULL,10);
        ret = 1;
        //        w = xs_strnstr(r,"&",urllen-(r+5-url),1);
        //        if(w)
        //            memcpy(phoneMsg->imei,r+5,w-r-5);
        //        else
        //            sscanf(r,"%32s",phoneMsg->imei);
    }
    return ret;
}

int XsFilter_ErrMail(PParseFiled pParseFiled){
    if((strcmp(pParseFiled->bufType,"mail")==0)){
        char *p = xs_strnstr(pParseFiled->bufUrl,"@",strlen(pParseFiled->bufUrl),1);
        if(p == NULL)
            return -1;
    }
    return 0;
}

void XsUrl_Decode_ForWangWang(PParseFiled pParseFiled){
    char wangwangbuf[64] = {0}, wangwangbuf1[128] = {0};

    if(strcmp(pParseFiled->bufType,"wangwanglog")==0){//URL解码
        memset(wangwangbuf,0,sizeof(wangwangbuf));
        memset(wangwangbuf1,0,sizeof(wangwangbuf1));

        snprintf(wangwangbuf,sizeof(wangwangbuf)-1,"%s",pParseFiled->bufUrl);
        snprintf(wangwangbuf1,sizeof(wangwangbuf1)-1,"%s",pParseFiled->bufUrl);

        urldecode(wangwangbuf); //解码后
        memset(pParseFiled->bufUrl,0,sizeof(pParseFiled->bufUrl));
        snprintf(pParseFiled->bufUrl,sizeof(pParseFiled->bufUrl)-1,"%s(%s)",wangwangbuf,wangwangbuf1);
    }
}

void XsParse_Special_Url(PParseFiled pParseFiled,pAccountInfo pInfo,PhoneMsg * phoneMsg,int l7){
    int ret  = 0;
    int len = 0;

    if(strcmp(pParseFiled->bufType,"urllog")==0&&(1099==l7||1978==l7||1408==l7||1407==l7)){//QQ图片/视频，WeChat图片/视频
        XsAccountInfo_Init(pInfo);//init
        XsParse_Special_Url_ForQQWeChat(l7,pParseFiled->bufUrl,strlen(pParseFiled->bufUrl),pInfo);
        if(pInfo->chatType != unknow){
            memcpy(pParseFiled->bufType,"filedwlog",strlen("filedwlog"));
        }
    }

    memset(pParseFiled->softname,0,sizeof(pParseFiled->softname));//sizeof()
    if(1979 == l7){//网页图片浏览
        ret = XsGet_Picture_DwUrl(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl));
        if(ret){
            memcpy(pParseFiled->bufType,"picfile",strlen("picfile"));
            pParseFiled->flagpic = 1;//picture
        }
    }

    if(1005 == l7 || 1102 == l7 || 1103 == l7 || 1995 == l7
            || 1994 == l7||1060 == l7 || 1066== l7 || 1067 == l7){//网页等文件下载（.apk/.ipa/.exe/.npk/.iod）
        ret = XsGet_File_DwUrl(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl));
        if(ret){
            const  char * p = strrchr((const char *)pParseFiled->bufUrl,'/');
            if(p != NULL){
                len = strlen(pParseFiled->bufUrl)-(p+1-pParseFiled->bufUrl);
                if(len <= sizeof(pParseFiled->softname)){
                    memcpy(pParseFiled->softname,p+1,len);
                    memcpy(pParseFiled->bufType,"picfile",strlen("picfile"));
                    pParseFiled->flagpic = 0;//file
                }else{
                    memcpy(pParseFiled->softname,"unknow",strlen("unknow"));
                    memcpy(pParseFiled->bufType,"picfile",strlen("picfile"));
                    pParseFiled->flagpic = 0;//file
                }
            }
        }
    }

    XsPhoneMsg_Init(phoneMsg);
    if(1975 == l7 || 1072 == l7){//天气通
        if(0 == memcmp(pParseFiled->bufUrl+7,"forecast.sina.cn",strlen("forecast.sin  a.cn"))||
                0 == memcmp(pParseFiled->bufUrl+7,"tqt.weibo.cn",strlen("tqt.weibo.cn"))){
            ret = XsGet_MobilePhone_TianQiTong_Msg_From_Url(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl),phoneMsg);
            if(ret){
                memcpy(pParseFiled->bufType,"phonemsg",strlen("phonemsg"));
            }
        }
    }

    if(1975 == l7){//今日头条
        char * p = strstr((char *)pParseFiled->bufUrl+7,".snssdk.com");
        if(p){
            ret = XsGet_MobilePhone_TouTiao_Msg_From_Url(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl),phoneMsg);
            if(ret){
                memcpy(pParseFiled->bufType,"phonemsg",strlen("phonemsg"));
            }
        }
    }

    if(1178 == l7){//墨迹天气//register.moji001.com
        if(0 == memcmp(pParseFiled->bufUrl+7,"register.moji001.com",strlen("register.moji001.com"))){
            ret = XsGet_MobilePhone_MoJi_Msg_From_Url(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl),phoneMsg);
            if(ret){
                memcpy(pParseFiled->bufType,"phonemsg",strlen("phonemsg"));
            }
        }
    }

    if(1975 == l7){//天天快报
        ret = XsGet_MobilePhone_TianTianKuaiBao_Msg_From_Url(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl),phoneMsg);
        if(ret){
            memcpy(pParseFiled->bufType,"phonemsg",strlen("phonemsg"));
        }
    }

    if(2005 == l7){//爱奇艺
        ret = XsGet_MobilePhone_AiQiYi_Msg_From_Url(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl),phoneMsg);
        if(ret){
            memcpy(pParseFiled->bufType,"phonemsg",strlen("phonemsg"));
        }
    }

    if(1001 == l7){//优酷
        char * p = strstr((char *)pParseFiled->bufUrl+7,".youku.com");
        if(p != NULL){
            ret = XsGet_MobilePhone_YouKu_Msg_From_Url(pParseFiled->bufUrl,strlen(pParseFiled->bufUrl),phoneMsg);
            if(ret){
                memcpy(pParseFiled->bufType,"phonemsg",strlen("phonemsg"));
            }
        }
    }


}

void *ThreadInsertTreeToMysql(void *param)
{
    PXsWork pxswork = (PXsWork)param;
    prctl(PR_SET_NAME,"GetLog");

    u_int l7 = 0;
    int ret = -1,len = 0;
    char *pPopBuf = NULL;
    ParseFiled parseFiled;
    AccountInfo accountInfo;
    PhoneMsg phoneMsg;
    int bStart = 0;
    //    char szSqlAdd[256]={0};
    //    char szSqlQuery[MAX_SQL_LEN] = {0};
    //    u_int64_t hashkey = 0;//filename
    while(!ntop->getGlobals()->isShutdown()){
//        reslock.lock(__FILE__, __LINE__);
//        sem_wait(&pxswork->bin_sem);
//        reslock.unlock(__FILE__, __LINE__);
        pPopBuf=(char *) Xs_List_mc_POPList(&pxswork->list);
        if(!pPopBuf)
            continue;
        memset(&parseFiled,0,PRASEFILED_SIZE);
        if(-1 == XsGet_Type(parseFiled.bufType,pPopBuf,"type:","types:",sizeof(parseFiled.bufType)))//get type/types
            goto FreeBuf;
        if(strstr(parseFiled.bufType,"qqlog"))
            printf("%s\n",pPopBuf);
        if(-1 == XsGet_Oth_Filed(parseFiled.bufLabel,pPopBuf,"label:",sizeof(parseFiled.bufLabel)))//get label
            goto FreeBuf;
        if(-1 == XsGet_Usr_Or_snat(parseFiled.szUsr,pPopBuf,"usr:",sizeof(parseFiled.szUsr)))//get usr
            memcpy(parseFiled.szUsr," ",sizeof(parseFiled.szUsr));//为了适应没有usr字段的路由版本 赋值为空格

       if(-1 == XsGet_Oth_Filed(parseFiled.szbufDate,pPopBuf,"date:",sizeof(parseFiled.szbufDate)))//get date
            goto FreeBuf;
        parseFiled.bufDate = atoi(parseFiled.szbufDate);

        if(-1 == XsGet_Oth_Filed(parseFiled.bufTime,pPopBuf,"time:",sizeof(parseFiled.bufTime)))//get time
            goto FreeBuf;

         if(-1 == XsGet_Oth_Filed(parseFiled.bufSmac,pPopBuf,"smac:",sizeof(parseFiled.bufSmac)))//get smac
            goto FreeBuf;

        if(-1 == XsGet_Oth_Filed(parseFiled.bufMethod,pPopBuf,"method:",sizeof(parseFiled.bufMethod)))//get method
            goto FreeBuf;

        ret = memcmp(parseFiled.bufType,"IDlog",strlen("IDlog"));

        if(0 == ret){
            char szType[32] = {0};

            char* type = my_strlwr(parseFiled.bufType,szType);
            if(type != NULL){
                memset(parseFiled.bufType,0,sizeof(parseFiled.bufType));
                len = (sizeof(parseFiled.bufType)>strlen(type))?strlen(type):(sizeof(parseFiled.bufType)-1);
                memcpy(parseFiled.bufType,type,len);
            }
            strcpy(parseFiled.bufType,szType);
            if(-1 == XsGet_Oth_Filed(parseFiled.bufSip,pPopBuf,"ip:",sizeof(parseFiled.bufSip)))//get src
                goto FreeBuf;
        }else{
            if(-1 == XsGet_Usr_Or_snat(parseFiled.L7,pPopBuf,"l7:",sizeof(parseFiled.L7)))//get L7
            {
                memcpy(parseFiled.L7," ",sizeof(parseFiled.L7));
                l7 = 0;
            }

            l7 = atoi(parseFiled.L7);

            if(-1 == XsGet_Oth_Filed(parseFiled.bufSrc,pPopBuf,"src:",sizeof(parseFiled.bufSrc)))//get src
                goto FreeBuf;
            if(-1 == XsGet_Ip_And_Port(pPopBuf,parseFiled.bufSrc,parseFiled.bufSip,&parseFiled.nSport))
                goto FreeBuf;

            if(-1 == XsGet_Oth_Filed(parseFiled.bufDst,pPopBuf,"dst:",sizeof(parseFiled.bufDst)))//get dst
                goto FreeBuf;
            if(-1 == XsGet_Ip_And_Port(pPopBuf,parseFiled.bufDst,parseFiled.bufDip,&parseFiled.nDport))
                goto FreeBuf;
            if(-1 == XsGet_LocalInfo(&parseFiled))
                goto FreeBuf;

            if(-1 == XsGet_Usr_Or_snat(parseFiled.bufSnat,pPopBuf,"snat:",sizeof(parseFiled.bufSnat))){
                memcpy(parseFiled.bufSnat_ip,"0",20);//为了适应没有snat字段的路由版本 赋值为0
                parseFiled.nSnat_port=0;
            }else{
                if(-1 == XsGet_Ip_And_Port(pPopBuf,parseFiled.bufSnat,parseFiled.bufSnat_ip,&parseFiled.nSnat_port))
                    goto FreeBuf;
            }

            if(-1 == XsGet_Oth_Filed(parseFiled.bufProto,pPopBuf,"proto:",sizeof(parseFiled.bufProto)))//get proto
                goto FreeBuf;

            if(-1 == XsGet_Oth_Filed(parseFiled.bufUrl,pPopBuf,"data:",sizeof(parseFiled.bufUrl)))//get data
                goto FreeBuf;
        }

        if((strcmp(parseFiled.bufType,"qqlog")==0)&&strlen(parseFiled.bufUrl)<=4)//过滤QQ号码少于4位的帐号
            goto FreeBuf;

        if(-1 == XsFilter_ErrMail(&parseFiled))//过滤不符号邮箱格式的邮箱帐号
            goto FreeBuf;

       if(ntop->g_xsDebug)  printf("FailedAnalysis: %d\n",ntop->nFailedAnalysis);

        XsUrl_Decode_ForWangWang(&parseFiled);//wangwang URL解码
         /*////*/
        //匹配特定的的应用协议(l7)作进一步分析
         XsParse_Special_Url(&parseFiled,&accountInfo,&phoneMsg,l7);


        if(memcmp(parseFiled.bufType,"phonemsg",strlen("phonemsg")) == 0 && strlen(phoneMsg.device_type) == 0)
            goto FreeBuf;

        if(strstr(parseFiled.bufType,"qqlog"))
            printf("%s\n",pPopBuf);
        ntop->getES()->dumpEsFlowDpi(&parseFiled,&accountInfo,&phoneMsg);
        pxswork->nParseCount++;
        parsepacket++;
FreeBuf:
        XsFree_pPopBuf(pPopBuf);
        pPopBuf = NULL;

        //        memset(szSqlQuery,0,MAX_SQL_LEN);
        //        memset(szSqlAdd,0,sizeof(szSqlAdd));
        //            if((strcmp(parseFiled.bufType,"urllog")==0)){//URL日志
        //                sprintf(szSqlQuery,"call prurllog('%s',%ld,%d,%ld,%d,%ld,%d,'%s','%s','%s',%d,%u,'%s','%s',%d,'%s','%s','%s')",  parseFiled.bufLabel,atol(parseFiled.bufSip),parseFiled.nSport,
        //                        atol(parseFiled.bufDip),parseFiled.nDport,atol(parseFiled.bufSnat_ip),parseFiled.nSnat_port,parseFiled.bufSmac,parseFiled.bufMethod,parseFiled.bufUrl,1,l7,parseFiled.bufType,parseFiled.bufProto,parseFiled.bufDate,parseFiled.bufTime,parseFiled.szUsr,"");
        //                if(ntop->g_xsDebug) printf("%s\n",szSqlQuery);
        //                XsDBOp_Insert_UrlLog( pxswork,szSqlAdd, parseFiled.bufDate,szSqlQuery);
        //            } else if((strcmp(parseFiled.bufType,"dnslog")==0)) {//DNS日志
        //                sprintf(szSqlQuery,InsertIP4_Log_Sql_FMT_For_Dns,// "(node,usr,srcip,srcport,dstip,dstport,mac,act,data1,l7,type,proto,date,time,snat_ip,snat_port)"
        //                        parseFiled.bufLabel,parseFiled.szUsr,atol(parseFiled.bufSip),parseFiled.nSport,atol(parseFiled.bufDip),parseFiled.nDport,parseFiled.bufSmac,parseFiled.bufMethod,parseFiled.bufUrl,l7,parseFiled.bufType,parseFiled.bufProto,parseFiled.bufDate,parseFiled.bufTime,atol(parseFiled.bufSnat_ip),parseFiled.nSnat_port);
        //                if(ntop->g_xsDebug) printf("%s\n",szSqlQuery);
        //                XsDBOp_Insert_DnsLog(pxswork,szSqlAdd, parseFiled.bufDate,szSqlQuery);
        //            }else if((strcmp(parseFiled.bufType,"IDlog")==0)){//用户认证日志
        //                sprintf(szSqlQuery,InsertIP4_Log_Sql_FMT_For_ID,//node,usr,srcip,mac,act,type,date,time
        //                        parseFiled.bufLabel,parseFiled.szUsr,atol(parseFiled.bufSip),parseFiled.bufSmac,parseFiled.bufMethod,parseFiled.bufType,parseFiled.bufDate,parseFiled.bufTime);
        //                if(ntop->g_xsDebug) printf("%s\n",szSqlQuery);
        //                XsDBOp_Insert_IDLog(pxswork,szSqlAdd, parseFiled.bufDate,szSqlQuery);
        //            }else{//其他日志
        //                sprintf(szSqlQuery,"call prnetlog('%s',%ld,%d,%ld,%d,%ld,%d,'%s','%s','%s',%d,%u,'%s','%s',%d,'%s','%s','%s')",  parseFiled.bufLabel,atol(parseFiled.bufSip),parseFiled.nSport,
        //                        atol(parseFiled.bufDip),parseFiled.nDport,atol(parseFiled.bufSnat_ip),parseFiled.nSnat_port,parseFiled.bufSmac,parseFiled.bufMethod,parseFiled.bufUrl,1,l7,parseFiled.bufType,parseFiled.bufProto,parseFiled.bufDate,parseFiled.bufTime,parseFiled.szUsr,"");
        //                if(ntop->g_xsDebug) printf("%s\n",szSqlQuery);
        //                XsDBOp_Insert_OthLog(pxswork,szSqlAdd, parseFiled.bufDate,szSqlQuery);
        //            }

    }
    return NULL;
}

void *XsLog_work(void *param)
{
    PXsWork pxswork = (PXsWork)param;
    prctl(PR_SET_NAME,"rcvpacket");

    int len =0,err=0;
    int reboot=0;  /*Create Udp Socket Failed 30times .We Reboot Program*/
    char *buffer=NULL;
    pxswork->list.nums = 0;

    struct sockaddr_in sin;
    socklen_t s_len = sizeof(sin);

    int socket=CreateUDPSocket(pxswork->port,0,0,0);
    if(-1 != socket)
        Xs_Set_UDPSocket_Buffer(socket);
    int bStart = 0;
    while(!ntop->getGlobals()->isShutdown())
    {
        buffer=(char *)malloc(My_BUFSIZ+1);
        if(!buffer)  continue;
        memset(buffer,0,My_BUFSIZ+1);

        len=recvfrom(socket, buffer,My_BUFSIZ,0,(struct sockaddr*)&sin,&s_len);
        if(len <= 0)
        {
            XsFree_pPopBuf(buffer);

            printf("Recvfrom Failed:%d\n",errno);

            err = errno;
            if(err != EAGAIN && err != EINTR)
            {
                close(socket);
                usleep(500);
                socket = CreateUDPSocket(pxswork->port,0,0,0);
                if(reboot > 50)
                {
                    WriteLog("xsdpilog","Main Thread: Re-Create UdpSocket Failed. Exit!!!!");
                    kill(getpid(),SIGINT);
                    sleep(1);
                }
                if(-1!=socket)
                    Xs_Set_UDPSocket_Buffer(socket);
                ++reboot;
            }
            continue;
        }

        recvpacket++;

        pxswork->count++;//

        if(unlikely(0 != Xs_List_mp_PushList(&pxswork->list, buffer))){
            XsFree_pPopBuf(buffer);
            buffer = NULL;
            continue;
        }
        sem_post(&pxswork->bin_sem);
        buffer = NULL;
    }
    close(socket);
    return NULL;
}


//void *ThreadXslogToMysql(void *param){
//    prctl(PR_SET_NAME,"XslogToMysql");
//    int   err=0;
//    pthread_t t;
//    err=pthread_create(&t,NULL,ThreadInsertTreeToMysql,NULL);
//    if (err != 0) printf("Create ThreadInsertTreeToMysql Failed. %s\n", strerror(err));

//    XsLog_work();
//    close(t);

//    return NULL;
//}



//int GetLogMsg_InsertMysql_FromXsdpi(){
//    pthread_t t;
//    int err=pthread_create(&t,NULL,ThreadXslogToMysql,NULL);
//    if (err != 0)
//        printf("Create ThreadXslogToMysql Failed. %s\n", strerror(err));
//    close(t);
//}


//void XsMYDBOp_Partition_InsertMap(void *param){
//    PXsWork pxswork = (PXsWork)param;
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tburllog",&pxswork->mapPartitionUrl);//tburllog
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbnetlog",&pxswork->mapPartition);//tbnetlog
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbfiledwlog",&pxswork->mapPartitionFileDw);//tbfiledwlog
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbdnslog",&pxswork->mapPartitionDns);//tbdnslog
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbidlog",&pxswork->mapPartitionID);//tbidlog
//}

void XsCreate_RcvThread_WithPort(){
    int err=-1;
    pthread_t  t,t1;

    struct rb_node *node = rb_first(&ntop->mapXsWork);
    for(node;node&&node->data;node=rb_next(node)){
        //XsMYDBOp_Partition_InsertMap(node->data);
        err=pthread_create(&t,NULL,ThreadInsertTreeToMysql,node->data);
        if (err != 0) printf("Create ThreadInsertTreeToMysql Failed. %s\n", strerror(err));
        err=pthread_create(&t1,NULL,ThreadInsertTreeToMysql,node->data);
        if (err != 0) printf("Create ThreadInsertTreeToMysql Failed. %s\n", strerror(err));
        pthread_create(&t,NULL,XsLog_work,node->data);
        if (err != 0) printf("Create XsLog_work Failed. %s\n", strerror(err));
    }
    return;
}

//void MYDBOp_Read_PortInfo(char *portlist){
//    char sql[256]={0};
//    int i;
//    snprintf(sql,sizeof(sql)-1,"select * from tbportinfo");

//    MYSQLRECORDSET GetQueryPort={0};
//    CSqlRecorDset_Init(&GetQueryPort);
//    CSqlRecorDset_SetConn(&GetQueryPort, ntop->g_xsMySql.m_pDB);
//    CSqlRecorDset_CloseRec(&GetQueryPort);
//    CSqlRecorDset_ExecSQL(&GetQueryPort, sql);
//    if(CSqlRecorDset_GetRecordCount(&GetQueryPort)==0){
//        CSqlRecorDset_CloseRec(&GetQueryPort);
//        CSqlRecorDset_Destroy(&GetQueryPort);
//        return;
//    }
//    for(i=0; i< CSqlRecorDset_GetRecordCount(&GetQueryPort);i++)
//    {
//        CSqlRecorDset_GetFieldValue_String(&GetQueryPort,"portlist",portlist,32);
//        CSqlRecorDset_MoveNext(&GetQueryPort);
//    }
//    CSqlRecorDset_CloseRec(&GetQueryPort);
//    CSqlRecorDset_Destroy(&GetQueryPort);
//    return ;
//}
void Xs_Param_Init(PXsWork pxswork,u_int port){
    pxswork->port=port;
    pxswork->count = 0;
    rb_root_init(&pxswork->mapPartitionFileDw);
    rb_root_init(&pxswork->mapPartition);
    rb_root_init(&pxswork->mapPartitionUrl);
    rb_root_init(&pxswork->mapPartitionDns);
    rb_root_init(&pxswork->mapPartitionID);
    Xs_List_Init(&pxswork->list);
    int res=sem_init(&pxswork->bin_sem,0,0);
    if(res!=0)  perror("Semaphore initialization failed");
}

int  XsGetPortInfo_FormFile(char *port){
    FILE *file = fopen("/opt/xslog/port.txt","r");
    if(file){
        char *p = fgets(port,64,file);
        if(ntop->g_xsDebug)
            printf("sxzlog recv port :%s\n",port);
        fclose (file);
        if(p){
            return 1;
        }else{
            return -1;
        }
    }
}

void XsWork_Init(){
    char port[64];
    int i;
    int ret=-1;
    struct rb_node *node;
    u_int DataArry[MAX_PORT_ID]={0};
    PXsWork pxswork = NULL;

    Xs_Idc_Init(&ntop->g_xsIdc);

    //MYDBOp_GetPartition_InsertMap();//ntop镜像

    //  MYDBOp_Read_PortInfo(port);//获取日志接收端口
    ret = XsGetPortInfo_FormFile(port);
    if(1 == ret){
        ret=ParseUIntNums(port,strlen(port),DataArry,MAX_PORT_ID+1,MAX_PORT_ID);
    }
    if(ret<=0){
        printf("The log server has no receive port\t default rcv port = 8002\n\n");
        ret=1;
        DataArry[0]=8002;
    }
    //申请内存,初始化
    for(i=0;i<ret;i++){
        pxswork =(PXsWork)malloc(XSWORK_SIZE);//notice free
        Xs_Param_Init(pxswork,DataArry[i]);
        node = int_void_rbinsert(&ntop->mapXsWork,pxswork->port,pxswork);
        if(node){
            free(pxswork);
            WriteLog("xsdpilog","insert mapXsWork faild");
            if(ntop->g_xsDebug)  printf("insert mapXsWork faild:%u\n",DataArry[i]);
        }
    }
}

void XsWork_Destroy(){
    int_void_rbclear2(&ntop->mapXsWork,free);
}
