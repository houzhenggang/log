#include "ntop_includes.h"



//int XsDBOp_Query_Host_Label(char *host,char *host_label){
//    char sql[256]={0};
//    int i;

//    snprintf(sql,sizeof(sql)-1,"select * from tbhostlabel where host_ip='%s'",host);

//    MYSQLRECORDSET GetQueryPartition={0};
//    CSqlRecorDset_Init(&GetQueryPartition);
//    CSqlRecorDset_SetConn(&GetQueryPartition, ntop->g_xsMySql.m_pDB);
//    CSqlRecorDset_CloseRec(&GetQueryPartition);
//    CSqlRecorDset_ExecSQL(&GetQueryPartition, sql);
//    if(CSqlRecorDset_GetRecordCount(&GetQueryPartition)==0) {
//        CSqlRecorDset_CloseRec(&GetQueryPartition);
//        CSqlRecorDset_Destroy(&GetQueryPartition);
//        return 0 ;
//    }
//    for(i=0; i< CSqlRecorDset_GetRecordCount(&GetQueryPartition);i++)
//    {
//        CSqlRecorDset_GetFieldValue_String(&GetQueryPartition,"host_label",host_label,32);
//        CSqlRecorDset_MoveNext(&GetQueryPartition);
//    }
//    CSqlRecorDset_CloseRec(&GetQueryPartition);
//    CSqlRecorDset_Destroy(&GetQueryPartition);
//    return 1;
//}

//int XsDBOp_Query_Dhcp_Cache(char * mac,char* name){
//    char sql[256]={0};
//    int i;

//    snprintf(sql,sizeof(sql)-1,"select * from tbdhcpcache where mac='%s'",mac);

//    MYSQLRECORDSET GetQueryPartition={0};
//    CSqlRecorDset_Init(&GetQueryPartition);
//    CSqlRecorDset_SetConn(&GetQueryPartition, ntop->g_xsMySql.m_pDB);
//    CSqlRecorDset_CloseRec(&GetQueryPartition);
//    CSqlRecorDset_ExecSQL(&GetQueryPartition, sql);
//    if(CSqlRecorDset_GetRecordCount(&GetQueryPartition)==0) {
//        CSqlRecorDset_CloseRec(&GetQueryPartition);
//        CSqlRecorDset_Destroy(&GetQueryPartition);
//        return 0 ;
//    }
//    for(i=0; i< CSqlRecorDset_GetRecordCount(&GetQueryPartition);i++)
//    {
//        CSqlRecorDset_GetFieldValue_String(&GetQueryPartition,"name",name,64);
//        CSqlRecorDset_MoveNext(&GetQueryPartition);
//    }
//    CSqlRecorDset_CloseRec(&GetQueryPartition);
//    CSqlRecorDset_Destroy(&GetQueryPartition);
//    return 1;
//}

//void XsDBOp_DhcpCache_InsertMysql(char *mac,char *name){
//    char sql[MAX_SQL_LEN-1]={0};
//    char pdate[10];
//    char time[10];
//    u_int date;
//    memset(pdate,0,sizeof(pdate));
//    memset(time,0,sizeof(time));
//    get_dateString(pdate,sizeof(pdate)-1);
//    date=atoi(pdate);
//    get_timeString(time,sizeof(time)-1);

//    XsMyDBOp_Add_Partition("tbdhcpcache",&ntop->mapPartitionDhcp,date);

//    snprintf(sql,MAX_SQL_LEN,"insert into tbdhcpcache (mac,name,date,time) values ('%s','%s',%d,'%s')",mac,name,date,time);
//    if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql,sql))
//        WriteLog("writemysql",sql);

//    XsDeal_Partition_Days("tbdhcpcache",&ntop->mapPartitionDhcp);

//}


//void XsDBOp_HostLabel_InsertMysql(char *host ,char * label_name){
//    char sql[MAX_SQL_LEN-1]={0};
//    snprintf(sql,MAX_SQL_LEN,"insert into tbhostlabel (host_ip,host_label) values ('%s','%s')",host,label_name);
//    if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sql))
//        WriteLog("writemysql",sql);
//}


//void XsDBOp_NtopngTrace_InsertMysql(char *out_buf){
//    char sql[MAX_SQL_LEN-1]={0};
//    snprintf(sql,MAX_SQL_LEN,"insert into tbntoptrace (value) values ('%s')",out_buf);
//    if(!MyDBOp_ExecSQL_1(&ntop->g_xsMySql ,sql))
//        WriteLog("writemysql",sql);
//}
