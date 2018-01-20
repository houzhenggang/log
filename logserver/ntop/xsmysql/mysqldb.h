#ifndef XS_MYSQL_DB_H
#define XS_MYSQL_DB_H


#ifdef __linux
#include <mysql/mysql.h>
#include "lib/xsbool.h"
#else
#include "/usr/local/mysql/include/mysql.h"
#endif
typedef struct _mysqlbase
{
    int conn_status;
    bool has_init;
    MYSQL *m_con;
    pthread_mutex_t m_cs;
}MYSQLBASE,*PMYSQLBASE;
#define MYSQLBASE_SIZE      sizeof(MYSQLBASE)

typedef struct _mysqlrecordset
{
    MYSQL_RES *m_res;//??????
    MYSQL_FIELD *m_field;//?????
    int m_lies;//????
    int m_rows;//????
    MYSQL_ROW m_row;//?????
    bool m_isEndOf;

    MYSQL* m_con;
    PMYSQLBASE m_pDB;
}MYSQLRECORDSET,*PMYSQLRECORDSET;
#define MYSQLRECORDSET_SIZE     sizeof(MYSQLRECORDSET)

bool MysqlBase_OpenDB(PMYSQLBASE pDB,char *srvUser,char *DBPass,char *DBName,char *srvIP,u_int16_t port);
void MysqlBase_CloseDB(PMYSQLBASE pDB);

void CSqlRecorDset_Init(PMYSQLRECORDSET pRecor);
void CSqlRecorDset_Destroy(PMYSQLRECORDSET pRecor);
bool CSqlRecorDset_SetConn(PMYSQLRECORDSET pRecor,PMYSQLBASE pDB);
bool CSqlRecorDset_ExecSQL(PMYSQLRECORDSET pRecor,char* sql);
bool CSqlRecorDset_CloseRec(PMYSQLRECORDSET pRecor);
//bool CSqlRecorDset_GetFieldValue_I8(PMYSQLRECORDSET pRecor,char *strField,int8_t *nValue);
bool CSqlRecorDset_GetFieldValue_I16(PMYSQLRECORDSET pRecor,char *strField,int16_t *nValue);
bool CSqlRecorDset_GetFieldValue_U16(PMYSQLRECORDSET pRecor,char *strField,u_int16_t *nValue);
bool CSqlRecorDset_GetFieldValue_I32(PMYSQLRECORDSET pRecor,char *strField,int32_t *nValue);
bool CSqlRecorDset_GetFieldValue_I64(PMYSQLRECORDSET pRecor,char *strField,int64_t *nValue);
bool CSqlRecorDset_GetFieldValue_U64(PMYSQLRECORDSET pRecor,char *strField,u_int64_t *nValue);
bool CSqlRecorDset_GetFieldValue_U32(PMYSQLRECORDSET pRecor,char *strField,u_int32_t *nValue);
bool CSqlRecorDset_GetFieldValue_Double(PMYSQLRECORDSET pRecor,char *strField,double *nValue);
bool CSqlRecorDset_GetFieldValue_String(PMYSQLRECORDSET pRecor,char *strField,char * strValue,uint size);
bool CSqlRecorDset_MoveNext(PMYSQLRECORDSET pRecor);
bool CSqlRecorDset_MoveFirst(PMYSQLRECORDSET pRecor);
bool CSqlRecorDset_IsEndEOF(PMYSQLRECORDSET pRecor);
int  CSqlRecorDset_GetRecordCount(PMYSQLRECORDSET pRecor);


int CSql_Transaction_Start(PMYSQLRECORDSET pRecor);
int CSql_Transaction_Query(PMYSQLRECORDSET pRecor,char *sql);
int CSql_Transaction_Committ(PMYSQLRECORDSET pRecor);

#endif
