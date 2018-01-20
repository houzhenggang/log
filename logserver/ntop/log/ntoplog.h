#ifndef WRITELOG_H
#define WRITELOG_H
#include <linux/if_ether.h>
#include "xsmysql/mydbop.h"
#include"xsmysql/mysqldb.h"
#include"lib/rbtree.h"
#include"lib/rbtree_callback.h"

#define MAX_SQL_LEN 2048

#define InsertIP4_Log_Sql_FMT_For_Url "insert into tburllog "\
    "(node,usr,srcip,srcport,dstip,dstport,mac,act,data1,flag,l7,type,proto,date,time,snat_ip,snat_port,os)"  \
    "values " \
    "('%s','%s',%u,%d,%u,%d,'%s','%s','%s',%d,%u,'%s','%s',%d,'%s',%u,%d,'%s');"

#define InsertIP4_Log_Sql_FMT_For_Other "insert into tbnetlog "\
    "(node,usr,srcip,srcport,dstip,dstport,mac,act,data1,flag,l7,type,proto,date,time,snat_ip,snat_port,os)"  \
    "values " \
    "('%s','%s',%u,%d,%u,%d,'%s','%s','%s',%d,%u,'%s','%s',%d,'%s',%u,%d,'%s');"

#define InsertIP4_Log_Sql_FMT_For_FileDw "insert into tbfiledwlog "\
    "(srcip,srcport,dstip,dstport,mac,url,account,filename,type,os,flag,date,time)"  \
    "values " \
    "(%u,%u,%u,%u,'%s','%s','%s',%lu,'%s','%s',%d,%d,'%s');"

#define InsertIP4_Log_Sql_FMT_For_PicAndFileDw "insert into tbpicfiledwurl "\
    "(srcip,srcport,dstip,dstport,mac,url,filename,type,os,date,time)"  \
    "values " \
    "(%u,%u,%u,%u,'%s','%s',%lu,'%s','%s',%d,'%s');"


enum log_type{
    Log_type_unknow=0,
    Log_type_Url ,
    Log_type_qq,
    Log_type_wechat,
    Log_type_redirect,
    Log_type_game,
    Log_type_weibo,//byzpl
    //...
    Log_type_end
};

void MYDBOp_GetPartition_InsertMap();
void   MysqlGetPartition_InsertMap(PMYDBOP pDB, char *tablename, rb_root *pMapPartition);
bool  dumpMysqlFlow(Flow *f);
bool MyDBOp_InsertDB(Flow * flow);
void XsInsert_FileDw_Log_MyDB(Flow *f, char *bufUrl, char *os, char *act, char *mac, u_int date, char *time);
void XsInsert_Wechat_VedioDw_Log_MyDB(Flow *f, char *bufUrl, char *os, char *type, char *mac, u_int64_t hashkey, u_int date, char *time, char *account);
void XsInsert_Url_Log_MyDB(Flow *f, char *bufUrl, char *act, char *type, char *mac, u_int date, char *time, char *os);
void *ThreadInsertVedioUrlToMysql(void* param);
void   XsMyDBOp_Add_Partition(char *tablename,struct rb_root *pMapPartition,u_int date);
void XsDeal_Partition_Days(char * tablename,struct rb_root* pMapPartition);
#endif // WRITELOG_H
