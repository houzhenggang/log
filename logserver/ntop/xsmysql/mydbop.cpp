#include <xsmysql/mydbop.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "pthread.h"
#include"ntop_includes.h"

/* ******************************** */
bool xsmysql_connect(PMYDBOP pDB)
{
        if(MyDBOp_OpenDB(pDB,"root","tXkj-8002-vErygood","ntop","127.0.0.1",8306)){
            printf("open data base ok (ntop)\n");
        }else{
            printf("connect data base failed ! exit!!!\n");
            return false;
        }
 //   MyDBOp_ExecSQL_1(pDB,"set names utf8");
    return true;
}
/* ******************************** */

void MyDBOp_Init(PMYDBOP pMyDB)
{
    memset(pMyDB,0,MYDBOP_SIZE);
    struct tm st;
    GetLocalTime(&st);
    sprintf(pMyDB->szStarTime,"%04d-%02d-%02d %02d:%02d:%02d",st.tm_year,st.tm_mon,st.tm_mday,st.tm_hour,st.tm_min,st.tm_sec);
    pMyDB->m_pDB = NULL;
    CSqlRecorDset_Init(&pMyDB->m_Query);
    pMyDB->m_nAlreadyInit=1;
}

void MyDBOp_Destroy(PMYDBOP pMyDB)
{
    MyDBOp_CloseDB(pMyDB);
}

bool MyDBOp_ReOpenDB(PMYDBOP pMyDB)
{
    MyDBOp_CloseDB(pMyDB);
    bool bRet = MyDBOp_OpenDB(pMyDB,pMyDB->m_szUser,pMyDB->m_szPassword,pMyDB->m_szDBName,pMyDB->m_szIP,pMyDB->m_nPort);
    return bRet;
}

bool MyDBOp_OpenDB(PMYDBOP pMyDB,char *pUser,char *pPassword,char *pDBName,char *pSvrIP,int nPort)
{
    pMyDB->m_pDB = (PMYSQLBASE)malloc(sizeof(MYSQLBASE));//new CSqlDataBase;
    memset(pMyDB->m_pDB,0,sizeof(MYSQLBASE));
    bool bRet = MysqlBase_OpenDB(pMyDB->m_pDB,pUser,pPassword,pDBName,pSvrIP,nPort);//m_pDB->OpenDB(pUser,pPassword,pDBName,pSvrIP,nPort);
    if(bRet)
    {
        CSqlRecorDset_SetConn(&pMyDB->m_Query,pMyDB->m_pDB);//m_Query.InitConn(m_pDB);
        CSqlRecorDset_ExecSQL(&pMyDB->m_Query,"set names gbk");//m_Query.ExecSQL("set names gbk");
    }
    strcpy(pMyDB->m_szUser,pUser);
    strcpy(pMyDB->m_szPassword,pPassword);
    strcpy(pMyDB->m_szDBName,pDBName);
    strcpy(pMyDB->m_szIP,pSvrIP);
    pMyDB->m_nPort = nPort;
    return bRet;
}

bool MyDBOp_CloseDB(PMYDBOP pMyDB)
{
    if(pMyDB->m_pDB) {
        MysqlBase_CloseDB(pMyDB->m_pDB);//m_pDB->CloseDB();
        free(pMyDB->m_pDB);
        pMyDB->m_pDB = NULL;
    }
    return true;
}


bool MyDBOp_ExecSQL(PMYDBOP pMyDB,char *pSQL)
{
    MYSQLRECORDSET Query={0};
    CSqlRecorDset_Init(&Query);
    CSqlRecorDset_SetConn(&Query,pMyDB->m_pDB);//Query.InitConn(m_pDB);

    if(!CSqlRecorDset_ExecSQL(&Query,pSQL))
    {
        CSqlRecorDset_Destroy(&Query);
        return false;
    }
    CSqlRecorDset_Destroy(&Query);
    return true;
}

bool MyDBOp_ExecSQL_1(PMYDBOP pMyDB,char *pSQL)
{
    if(!CSqlRecorDset_ExecSQL(&pMyDB->m_Query,pSQL))
    {
        return false;
    }
    return true;
}

void MyDBOp_Ping(PMYDBOP pMyDB)
{
    PMYSQLBASE m_pDB=pMyDB->m_pDB;
    if(!m_pDB || !m_pDB->m_con){
        MyDBOp_ReOpenDB(pMyDB);
        return;
    }
    pthread_mutex_lock(&m_pDB->m_cs);//EnterCriticalSection(&m_pDB->m_cs);
    int ret = mysql_ping(m_pDB->m_con);
    m_pDB->conn_status = ret;
    pthread_mutex_unlock(&m_pDB->m_cs);//EnterCriticalSection(&m_pDB->m_cs);
    if(0 != m_pDB->conn_status){
        MyDBOp_ReOpenDB(pMyDB);
        return;
    }
}


int ReadVerifyInfo(PMYDBOP pMyDB, PUSERINFOEX2 pUserInfo, int ver)
{
    static int get_burst=2;
    char sql[256]={0};
    sprintf(sql,"select * from tbsysinfo where root='%s'","root");
    MYSQLRECORDSET Query={0};
    CSqlRecorDset_Init(&Query);
    CSqlRecorDset_SetConn(&Query,pMyDB->m_pDB);
    CSqlRecorDset_CloseRec(&Query);
    CSqlRecorDset_ExecSQL(&Query,sql);
    if(CSqlRecorDset_GetRecordCount(&Query)!=1)
    {
        CSqlRecorDset_CloseRec(&Query);
        CSqlRecorDset_Destroy(&Query);
        printf("请检查数据库 tbsysInfo 表里 root 的相关信息是否为空.\n");
        char info[256]={0};
        sprintf(info,"请检查数据库 tbsysInfo 表里 root 的相关信息是否为空");
        WriteLog("xsverify",info);
        return 0;
    }

    CSqlRecorDset_GetFieldValue_String(&Query,"cdkey",pUserInfo->szSerial, sizeof(pUserInfo->szSerial));
    if(get_burst>0) {
        u_int ctlbursts=0;
        u_int ctlonlines=0;
        u_int flag=0;
        CSqlRecorDset_GetFieldValue_U32(&Query,"ctlbursts",&ctlbursts);
        CSqlRecorDset_GetFieldValue_U32(&Query,"ctlonlines",&ctlonlines);
        CSqlRecorDset_GetFieldValue_U32(&Query,"ctlflag",&flag);
        pUserInfo->ctlBursts = ctlbursts;
        pUserInfo->ctlOnlines = ctlonlines;
        pUserInfo->ctlFlag = flag;
        get_burst--;
    }
    if(ver)
        CSqlRecorDset_GetFieldValue_String(&Query,"libver",pUserInfo->rulever,sizeof(pUserInfo->rulever));
    CSqlRecorDset_CloseRec(&Query);
    CSqlRecorDset_Destroy(&Query);
    return 1;
}
bool UpdateUserInfo(PMYDBOP pMyDB,PUSERINFOEX2 pUserInfo)
{
    if(strlen(pUserInfo->szSerial)==0)
        return false;
    char sql[512]={0};
    char tmp[256]={0};
    u2g(pUserInfo->szUserName,strlen(pUserInfo->szUserName),tmp,256);
    char szDesc[256]={0};
    u2g(pUserInfo->szDesc,strlen(pUserInfo->szDesc),szDesc,256);
    snprintf(sql,512,"call prxsidcsysinfoex('%s','%s','%s','%s',%lu,'%s','%s',%lu,%lu,%lu,%lu,%u ,'%s')","root",\
             pUserInfo->szSerial,pUserInfo->szUserCode,tmp,\
             pUserInfo->nVerifystatus,szDesc,pUserInfo->rulever,\
             pUserInfo->tRegTime,pUserInfo->tExpireTime,\
             pUserInfo->ctlBursts, pUserInfo->ctlOnlines, pUserInfo->ctlFlag ,
             pUserInfo->model);

    if(ntop->g_xsDebug) {
        printf("%s\n",sql);
    }
    MyDBOp_ExecSQL_1(pMyDB,sql);
    return true;
}


void MyDBOp_Repair(PMYDBOP pMyDB)
{
    char szSQL[512]={0};
    char szValue[256]={0};
    XsArray arTable;
    XsArray_Init(&arTable,ORDER_ASC,free,NULL);
    MYSQLRECORDSET Query={0};
    CSqlRecorDset_Init(&Query);
    CSqlRecorDset_SetConn(&Query,pMyDB->m_pDB);//Query.InitConn(m_pDB);
    CSqlRecorDset_CloseRec(&Query);//Query.CloseRec();
    CSqlRecorDset_ExecSQL(&Query,"show table status from xspeeder");//Query.ExecSQL("show table status from xspeeder");
    int i;
    for(i=0;i<CSqlRecorDset_GetRecordCount(&Query);i++)
    {
        char *pTable =(char*)malloc(64);
        memset(pTable,0,64);
        CSqlRecorDset_GetFieldValue_String(&Query,"name",pTable,64);
        CSqlRecorDset_MoveNext(&Query);
        XsArray_AddNodeToTail(&arTable,pTable);
    }
    CSqlRecorDset_CloseRec(&Query);

    for(i=0;i<XsArray_GetArrayCount(&arTable);i++)
    {
        memset(szSQL,0,sizeof(szSQL));
        sprintf(szSQL,"CHECK TABLE %s FAST",(char*)XsArray_GetNode(&arTable,i));
        CSqlRecorDset_CloseRec(&Query);
        CSqlRecorDset_ExecSQL(&Query,szSQL);
        CSqlRecorDset_GetFieldValue_String(&Query,"msg_type",szValue,255);
        if(strcasecmp(szValue,"status"))
        {
            memset(szSQL,0,sizeof(szSQL));
            sprintf(szSQL,"REPAIR TABLE %s QUICK",(char*)XsArray_GetNode(&arTable,i));
            CSqlRecorDset_CloseRec(&Query);
            CSqlRecorDset_ExecSQL(&Query,szSQL);
        }
    }
    //    XsArray_RemoveAllNode(&arTable);
    XsArray_Destroy(&arTable);
    CSqlRecorDset_Destroy(&Query);
}



//更新tbprotocolex
bool UpdateProtocolex2(PMYDBOP pMyDB,PPROTO pProto,char *pTag)
{
    char sql[512]={0};
    char tmp[64]={0};
    u2g(pProto->szName,strlen(pProto->szName),tmp,64);
    snprintf(sql,512,"call prprotocolex(%d,'%s',%d,%d,%d,%d,'%s',%u,%d,%d,%d,%d,%d,%d)", \
             pProto->nId,tmp,pProto->nLevel,pProto->nProtocol,pProto->nRoot, pProto->nParent,pTag,pProto->nTTL,pProto->nSwitch,pProto->nField,
             pProto->nField0,pProto->nField1,pProto->nField2,pProto->nField3);
    if(ntop->g_xsDebug) {
        printf("%s\n",sql);
    }
    if(false==MyDBOp_ExecSQL_1(pMyDB,sql)){//for old version
        memset(sql,0,sizeof(sql));
        snprintf(sql,512,"call prprotocolex(%d,'%s',%d,%d,%d,%d,'%s',%u)", \
                 pProto->nId,tmp,pProto->nLevel,pProto->nProtocol,pProto->nRoot, pProto->nParent,pTag,pProto->nTTL);
                MyDBOp_ExecSQL_1(pMyDB,sql);
    }
    return true;
}

