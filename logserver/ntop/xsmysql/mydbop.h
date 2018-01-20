#ifndef _MYDBOP_H_
#define _MYDBOP_H_
#include <xsmysql/mysqldb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/xs_compatibility.h"
#include"lib/common.h"
#include"lib/xs_utils.h"
#include"lib/xssharefunc.h"

typedef bool (*bool_fn)(void *obj,void *buf,int len);

typedef struct _mydbop{
    PMYSQLBASE m_pDB;
    MYSQLRECORDSET m_Query;
    char m_szUser[32];
    char m_szPassword[32];
    char m_szDBName[32];
    char m_szIP[40];
    char szStarTime[40];
    int m_nPort;
    int m_nAlreadyInit;
}MYDBOP,*PMYDBOP;
#define MYDBOP_SIZE     sizeof(MYDBOP)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
bool xsmysql_connect(PMYDBOP pDB);

void MyDBOp_Init(PMYDBOP pMyDB);
void MyDBOp_Destroy(PMYDBOP pMyDB);
bool MyDBOp_OpenDB(PMYDBOP pMyDB,char *pUser,char *pPassword,char *pDBName,char *pSvrIP,int nPort);
bool MyDBOp_ReOpenDB(PMYDBOP pMyDB);
bool MyDBOp_CloseDB(PMYDBOP pMyDB);

bool MyDBOp_ExecSQL(PMYDBOP pMyDB,char *pSQL);
bool MyDBOp_ExecSQL_1(PMYDBOP pMyDB,char *pSQL);

void MyDBOp_Ping(PMYDBOP pDB);
//verify
int ReadVerifyInfo(PMYDBOP pMyDB, PUSERINFOEX2 pUserInfo, int ver);
bool UpdateUserInfo(PMYDBOP pMyDB,PUSERINFOEX2 pUserInfo);
void MyDBOp_Repair(PMYDBOP pMyDB);
bool UpdateProtocolex2(PMYDBOP pMyDB,PPROTO pProto,char *pTag);

#endif
