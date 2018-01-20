#ifndef XS_STRUCT_H
#define XS_STRUCT_H
#include <stdio.h>
#include <stdlib.h>
//#include"cjson.h"


#pragma pack(1)


// 对话结构
typedef struct _SessionPack {
    int	    CommandID;
    unsigned int	DataLength;
    char    Reserver[20];
    char    Data[1];
}SESSIONPACK,*PSESSIONPACK;
#define SESSIONPACKSIZE sizeof(SESSIONPACK)

// 传输包结构
typedef struct _TransportPack {
    int	            PackLength;
    unsigned int	CheckSum;
    SESSIONPACK     SessionPack;
}TRANSPORTPACK,*PTRANSPORTPACK;
#define TRANSPORTPACKSIZE sizeof(TRANSPORTPACK)
typedef struct  _Rule{
    int nID;
    int nRuleid;
    int nRuletype;
    int nl4pro;
    char *szRule;
    uint nApp;
    u_int nCreatetime;
    u_int nUpdatetime;
    u_int nExpire;
}RULE,*PRULE;

typedef struct _Proto{
    int nId;
    char  szName[40];
    int nLevel;
    int nProtocol;
    int nRoot;
    int nParent;
    u_int8_t nField;
    u_int8_t nField0,nField1,nField2,nField3;
    u_int16_t nTTL;
    u_int8_t nSwitch;
}PROTO,*PPROTO;

typedef struct _UserInfoEx2
{
    //服务端返回的字段
    u_long nCmd;
    u_long nVerifystatus;
    char szSerial[16];
    char szUserCode[16];
    char szUserName[80];
    u_long  tRegTime ;
    u_long  tExpireTime ;
    short int nType;
    int  nUserNum;
    char rulever[20];
    char szDesc[160];
    u_long   ctlBursts  ;
    u_long  ctlOnlines  ;
    u_int8_t ctlports;
    u_int8_t  ctlFlag  ;
    u_int8_t dodpi,do_income,syn_count_for_404;
    char model[20];
    char cis[64];
    char web_user[64];
    char web_passwd[64];
    int update_webpasswd;
}USERINFOEX2,*PUSERINFOEX2;
#define USERINFOEX2SIZE sizeof(USERINFOEX2)


#pragma pack()//一字节对齐
#endif // XS_STRUCT_H

