#include <time.h>
#include <sys/time.h>
#include "ntop_includes.h"
#include"xsmysql/mydbop.h"
#include"lib/xshash.h"
#include"log/ntoplog.h"


/* ************************************************************** */

char *get_L4_protocolType(int proto){
    char *protocol=NULL;
    switch (proto){
    case IPPROTO_UDP:
        protocol = "UDP";
        break;
    case IPPROTO_TCP:
        protocol = "TCP";
        break;
    default:
        protocol = "Oth";
        break;
    }
    return protocol;
}
/*************************************************************/
//void XsInsert_PicFileDw_Url_MyDB(Flow *f,char *bufUrl,char *os,char *type,char *mac,u_int date,char *time){
//    char sql[MAX_SQL_LEN+1];

//    snprintf(sql,MAX_SQL_LEN,InsertIP4_Log_Sql_FMT_For_PicAndFileDw ,
//             htonl(f->get_cli_host()->get_ip()->get_ipv4()),
//             f->get_cli_port(),
//             htonl(f->get_srv_host()->get_ip()->get_ipv4()),
//             f->get_srv_port(),
//             mac,
//             bufUrl,
//             f->virtualnum.hashkey,
//             type,
//             os,//操作系统，终端类型
//             date,
//             time
//             );
//    if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sql))
//        WriteLog("writemysql",sql);
//}
/* ************************************************************** */
//void XsInsert_FileDw_Log_MyDB(Flow *f,char *bufUrl,char *os,char *type,char *mac,u_int date,char *time){
//    char sql[MAX_SQL_LEN+1];

//    snprintf(sql,MAX_SQL_LEN,InsertIP4_Log_Sql_FMT_For_FileDw ,
//             htonl(f->get_cli_host()->get_ip()->get_ipv4()),
//             f->get_cli_port(),
//             htonl(f->get_srv_host()->get_ip()->get_ipv4()),
//             f->get_srv_port(),
//             mac,
//             f->virtualnum.szUrl,
//             bufUrl,
//             f->virtualnum.hashkey,
//             type,
//             os,//操作系统，终端类型
//             1,//flag 1表示镜像  0牵引模式
//             date,
//             time
//             );
//    if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sql))
//        WriteLog("writemysql",sql);
//}
/*****************************************************************/
//void XsInsert_Wechat_VedioDw_Log_MyDB(Flow *f,char *bufUrl,char *os,char *type,char *mac,u_int64_t hashkey,u_int date,char *time,char * account){
//    char sql[MAX_SQL_LEN+1];

//    snprintf(sql,MAX_SQL_LEN,InsertIP4_Log_Sql_FMT_For_FileDw ,
//             htonl(f->get_cli_host()->get_ip()->get_ipv4()),
//             f->get_cli_port(),
//             htonl(f->get_srv_host()->get_ip()->get_ipv4()),
//             f->get_srv_port(),
//             mac,
//             bufUrl,
//             account,//account
//             hashkey,//filename
//             type,
//             os,//操作系统，终端类型
//             1,//flag 1表示镜像  0牵引模式
//             date,
//             time
//             );
//    if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sql))
//        WriteLog("writemysql",sql);
//}


//void XsInsert_Other_Log_MyDB(Flow *f,char *bufUrl,char *act,char *type,char *macString,u_int date,char *time,char * os){
//    size_t urlLen=strlen(bufUrl);
//    int ret=memcmp(macString,"00:00:00:00:00:00",17);
//    char sql[MAX_SQL_LEN+1];
//    if(urlLen&&ret){
//        snprintf(sql,MAX_SQL_LEN,InsertIP4_Log_Sql_FMT_For_Other ,
//                 " ",                //node
//                 " ", //usr
//                 htonl(f->get_cli_host()->get_ip()->get_ipv4()),
//                 f->get_cli_port(),
//                 htonl(f->get_srv_host()->get_ip()->get_ipv4()),
//                 f->get_srv_port(),
//                 macString,
//                 act,
//                 bufUrl,
//                 0,               //flag
//                 0,               //L7
//                 type,
//                 get_L4_protocolType(f->get_protocol()),
//                 date,
//                 time,
//                 NULL,
//                 NULL,
//                 os
//                 );
//        if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sql))
//            WriteLog("writemysql",sql);
//    }
//}

//void XsInsert_Url_Log_MyDB(Flow *f,char *bufUrl,char *act,char *type,char *macString,u_int date,char *time,char *os){
//    size_t urlLen=strlen(bufUrl);
//    int ret=memcmp(macString,"00:00:00:00:00:00",17);
//    char sql[MAX_SQL_LEN+1];
//    if(urlLen&&ret){
//        snprintf(sql,MAX_SQL_LEN,InsertIP4_Log_Sql_FMT_For_Url ,
//                 " ",                //node
//                 " ",//usr
//                 htonl(f->get_cli_host()->get_ip()->get_ipv4()),
//                 f->get_cli_port(),
//                 htonl(f->get_srv_host()->get_ip()->get_ipv4()),
//                 f->get_srv_port(),
//                 macString,
//                 act,
//                 bufUrl,
//                 0,               //flag
//                 0,               //L7
//                 type,
//                 get_L4_protocolType(f->get_protocol()),
//                 date,
//                 time,
//                 NULL,//snat_ip
//                 NULL,//snat_port
//                 os
//                 );
//        if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sql))
//            WriteLog("writemysql",sql);
//    }

//}

/* ************************************************************** */

//void XsDeal_Partition_Days(char * tablename,struct rb_root* pMapPartition){
//    int nPartitionCount=0;
//    char szSqlDrop[128]={0};
//    struct rb_node *node;
//    int n;

//    nPartitionCount=pMapPartition->ncount;//查 看有多少分区
//    if(ntop->g_xsDebug)  printf("Total nPartitionCount %d Table:%s\n",pMapPartition->ncount,tablename);

//    if(nPartitionCount>90){
//        node=rb_first(pMapPartition);
//        if(node){
//            n = node->key;
//            if(ntop->g_xsDebug)  printf("(%s)erase the first one :%d\n",tablename,n);
//            rb_erase(node, pMapPartition);

//            sprintf(szSqlDrop,"alter table %s drop partition p%d;",tablename,n);
//            if(ntop->g_xsDebug)  printf("%s Table:%s\n",szSqlDrop,tablename);
//            if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,szSqlDrop))
//                WriteLog("writemysql",szSqlDrop);
//        }
//    }
//}

/* ************************************************************** */
//void MYDBOp_GetPartition_InsertMap(){
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tburllog",&ntop->mapPartitionUrl);//tburllog
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbnetlog",&ntop->mapPartition);//tbnetlog
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbfiledwlog",&ntop->mapPartitionFileDw);//tbfiledwlog
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbpicfiledwurl",&ntop->mapPartitionPicFile);//tbpicfiledwurl
//    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbdhcpcache",&ntop->mapPartitionDhcp);//tbdhcpcache
////    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbdnslog",&ntop->mapPartitionDns);//tbdnslog
////    MysqlGetPartition_InsertMap(&ntop->g_xsMySql,"tbidlog",&ntop->mapPartitionID);//tbidlog
//}
/* ************************************************************** */
void get_MacString(char *dst,int size,u_char *src){
    snprintf(dst,size,"%02x:%02x:%02x:%02x:%02x:%02x",
             src[0],  src[1], src[2], src[3], src[4], src[5]);
}
/* ************************************************************** */
void get_CliHost_Os(Flow* flow,char *os,int size){
    if(flow->get_cli_host()->get_os())
        memcpy(os,flow->get_cli_host()->get_os(),MIN(strlen(flow->get_cli_host()->get_os()),size));
    else
        memset(os,0,size);
}


/* ************************************************************** */
/*查看当前数据表中已有分区并插入红黑树*/
void   MysqlGetPartition_InsertMap(PMYDBOP pDB,char * tablename,struct rb_root *pMapPartition)
{
    //    char sqlPartition[256]="select  partition_name part, partition_expression expr, \
    //            partition_description descn, table_rows from information_schema.partitions\
    //            where table_schema=schema() and table_name='tbntoplog';";
    char sqlPartition[256]={0};
    snprintf(sqlPartition,sizeof(sqlPartition)-1,"select  partition_name part, partition_expression expr, \
             partition_description descn, table_rows from information_schema.partitions\
             where table_schema=schema() and table_name='%s';",tablename);

            MYSQLRECORDSET GetQueryPartition={0};
    CSqlRecorDset_Init(&GetQueryPartition);
    CSqlRecorDset_SetConn(&GetQueryPartition,pDB->m_pDB);
    CSqlRecorDset_CloseRec(&GetQueryPartition);
    // pDB->m_Query = GetQueryPartition;
    CSqlRecorDset_ExecSQL(&GetQueryPartition, sqlPartition);
    if(CSqlRecorDset_GetRecordCount(&GetQueryPartition)==0) {
        CSqlRecorDset_CloseRec(&GetQueryPartition);
        CSqlRecorDset_Destroy(&GetQueryPartition);
        return ;
    }
    int i,nPartition;
    char szPartition[10] = {0};
    struct rb_node * node_iter ;
    for(i=0; i< CSqlRecorDset_GetRecordCount(&GetQueryPartition);i++)
    {
        CSqlRecorDset_GetFieldValue_String(&GetQueryPartition,"descn",szPartition,10);
        nPartition = atoi(szPartition);
        if(ntop->g_xsDebug) fprintf(stdout,"Partition:%d,Table:%s\t",nPartition,tablename);
        node_iter = int_void_rbsearch(pMapPartition,nPartition);
        if(node_iter&&ntop->g_xsDebug)
            fprintf(stdout,"find partition success in map Table:%s\n",tablename);
        else{
            node_iter = int_void_rbinsert(pMapPartition,nPartition,NULL);
            if(!node_iter&&ntop->g_xsDebug)
                printf("Partition insert map success Table:%s\n",tablename);
            else{
                if(ntop->g_xsDebug) printf("Partition insert map faild %d Table:%s\n",nPartition,tablename);
            }
        }
        CSqlRecorDset_MoveNext(&GetQueryPartition);
    }
    CSqlRecorDset_CloseRec(&GetQueryPartition);
    CSqlRecorDset_Destroy(&GetQueryPartition);
    return;
}
/* ************************************************************** */
//void   XsMyDBOp_Add_Partition(char *tablename,struct rb_root *pMapPartition,u_int date){
//    struct rb_node *node;
//    node= int_void_rbsearch(pMapPartition,date);
//    if(NULL==node)
//    {
//        char sqlAdd[256];
//        memset(sqlAdd,0,sizeof(sqlAdd));
//        sprintf(sqlAdd,"alter table %s add  partition (partition p%d values in (%u));",tablename,date,date);
//        if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sqlAdd))
//            WriteLog("writemysql",sqlAdd);
//        node = int_void_rbinsert(pMapPartition,date,NULL);
//        if(!node&&ntop->g_xsDebug){
//            printf("Partition insert map success:p%d Table:%s\n",date,tablename);
//        }
//        else{
//            if(ntop->g_xsDebug) printf("Partition insert map faild:%d Table:%s\n",date,tablename);
//        }
//    }
//}
/******************************************************************/

int get_picture_file_dwurl(Flow *flow,char * url){

    //http://image.baidu.com/
    //    if(flow->get_ndpi_flow()->packet.host_line.len=15
    //            &&(0==memcmp(flow->get_ndpi_flow()->packet.host_line.ptr,"image.baidu.com",15)))
    //        return 1;

    //    char *res=NULL;
    //    char urltest[1500]={"http://pic.58pic.com/58pic/13/74/51/99d58PIC6vm_1024.jpg"};
    //    res=xs_strnstr(urltest,".jpg",strlen(urltest),4);
    //    if(NULL==res)
    //        return -1;

    //http://sw.bos.baidu.com/sw-search-sp/av_aladdin/0845364a31b2b/rj_ns1696.exe
    //    memcpy(url,"http://sw.bos.baidu.com/sw-search-sp/av_aladdin/0845364a31b2b/rj_ns1696.exe",
    //           strlen("http://sw.bos.baidu.com/sw-search-sp/av_aladdin/0845364a31b2b/rj_ns1696.exe"));
    char * res=NULL;
    res=xs_strnstr(url,".jpg",strlen(url),4);
    if(res) return 1;
    res=xs_strnstr(url,".png",strlen(url),4);
    if(res) return 1;
    res=xs_strnstr(url,".jpeg",strlen(url),4);
    if(res) return 1;
    res=xs_strnstr(url,".exe",strlen(url),4);
    if(res) return 2;
    res=xs_strnstr(url,".apk",strlen(url),4);
    if(res) return 2;
    res=xs_strnstr(url,".ipa",strlen(url),4);
    if(res) return 2;

    return -1;

}
/* ************************************************************** */
//static inline void MYDBOP_Insert_Ipv4_Log(Flow *f){
//    int res=-1;
//    char sql[MAX_SQL_LEN+1];
//    char macString[20];
//    char bufUrl[1500];
//    char pdate[10];
//    char time[10];
//    char type[15];
//    char act[15];
//    char os[32];

//    memset(sql,0,sizeof(sql));
//    memset(macString,0,sizeof(macString));
//    memset(bufUrl,0,sizeof(bufUrl));
//    memset(pdate,0,sizeof(pdate));
//    memset(time,0,sizeof(time));
//    memset(type,0,sizeof(type));
//    memset(act,0,sizeof(act));
//    memset(os,0,sizeof(os));

//    get_dateString(pdate,sizeof(pdate)-1);
//    u_int date=atoi(pdate);
//    get_timeString(time,sizeof(time)-1);
//    get_MacString(macString,sizeof(macString)-1,(u_char *)f->get_cli_host()->get_mac());
//    get_CliHost_Os(f,os,sizeof(os)-1);
//    //test
//    //    if(strlen(os)>0){
//    //        static u_int count_os=0;
//    //        ++count_os;
//    //        fprintf(stdout,"InsertOS:[%s] Count:[%d]\n",os,count_os);
//    //    }

//    res= f->getFlowInfoToMysql(bufUrl,sizeof(bufUrl)-1,type,act,macString,date);

//    //    if(strlen(f->virtualnum.vedioUrl))
//    //        XsInsert_Wechat_VedioDw_Log_MyDB(f,f->virtualnum.vedioUrl,os,type,macString,date,time);

//    if(2==res){//file dw log
//        XsMyDBOp_Add_Partition("tbfiledwlog",&ntop->mapPartitionFileDw,date);

//        XsInsert_FileDw_Log_MyDB(f,bufUrl,os,type,macString,date,time);

//        XsDeal_Partition_Days("tbfiledwlog",&ntop->mapPartitionFileDw);
//    }else if (1==res){//other log

//        XsMyDBOp_Add_Partition("tbnetlog",&ntop->mapPartition,date);

//        XsInsert_Other_Log_MyDB(f,bufUrl,act,type,macString,date,time,os);

//        XsDeal_Partition_Days("tbnetlog",&ntop->mapPartition);

//    }else if(0==res){//url log
//        /*若数据库没有该分区，添加该分区，并插入红黑树*/
//        XsMyDBOp_Add_Partition("tburllog",&ntop->mapPartitionUrl,date);
//        /*将数据插入数据库(过滤URL为空)*/
//        XsInsert_Url_Log_MyDB(f,bufUrl,act,type,macString,date,time,os);
//        /*查看map有多少分区,数据库保存90天数据*/
//        XsDeal_Partition_Days("tburllog",&ntop->mapPartitionUrl);


//        //匹配链接是否为图片浏览和文件下载链接 保存入库
//        XsMyDBOp_Add_Partition("tbpicfiledwurl",&ntop->mapPartitionPicFile,date);
//        res=get_picture_file_dwurl(f,bufUrl);
//        if(1==res)
//            XsInsert_PicFileDw_Url_MyDB(f,bufUrl,os,"picture",macString,date,time);
//        else if(2==res)
//            XsInsert_PicFileDw_Url_MyDB(f,bufUrl,os,"file",macString,date,time);
//        XsDeal_Partition_Days("tbpicfiledwurl",&ntop->mapPartitionPicFile);
//    }

//}


/* ************************************************************** */
static inline void MYDBOP_Insert_Ipv6_Log(Flow *f){

}
/* ************************************************************** */
//bool dumpMysqlFlow(Flow *f) {
//    //    f-> cli_host->getInterface();
//    //    if((f->get_cli_host() == NULL) && (f->get_srv_host() == NULL))
//    //        return(false);
//    if(f->get_cli_host()->get_ip()->isIPv4())
//        MYDBOP_Insert_Ipv4_Log(f);
//    else
//        MYDBOP_Insert_Ipv6_Log(f);
//    return(true);
//}
/* ************************************************************** */


//bool MyDBOp_InsertDB(Flow * f){
//    if((f->get_cli_host() == NULL) && (f->get_srv_host() == NULL))
//        return(false);
//    if(f->get_cli_host()->get_ip()->isIPv4())
//        MYDBOP_Insert_Ipv4_Log(f);
//    else
//        MYDBOP_Insert_Ipv6_Log(f);
//    return(true);
//}

//void *ThreadInsertVedioUrlToMysql(void* param){
//    prctl(PR_SET_NAME,"intoVedioUrl");
//    Flow *popFlow=NULL;
//    char macString[20];
//    u_int64_t hashkey;//file name
//    char pdate[10];
//    char time[10];
//    u_int date;
//    char account[64]={0};

//    while(!ntop->getGlobals()->isShutdown()){
//        popFlow=(Flow *) Xs_List_mc_POPList(&ntop->listUrl);
//        if(!popFlow) continue;

//        memset(macString,0,sizeof(macString));
//        get_MacString(macString,sizeof(macString)-1,(u_char *)popFlow->get_cli_host()->get_mac());
//        memset(pdate,0,sizeof(pdate));
//        memset(time,0,sizeof(time));
//        get_dateString(pdate,sizeof(pdate)-1);
//        date=atoi(pdate);
//        get_timeString(time,sizeof(time)-1);

//        hashkey=CRC64((const u_char *)popFlow->virtualnum.vedioUrl,strlen(popFlow->virtualnum.vedioUrl));
//        snprintf(account,sizeof(account),"%lu",popFlow->virtualnum.id);

//        XsMyDBOp_Add_Partition("tbfiledwlog",&ntop->mapPartitionFileDw,date);

//        XsInsert_Wechat_VedioDw_Log_MyDB(popFlow,popFlow->virtualnum.vedioUrl," ",
//                                         "wechat_vedio",macString,hashkey,date,time,account);

//        XsDeal_Partition_Days("tbfiledwlog",&ntop->mapPartitionFileDw);
//    }

//}
