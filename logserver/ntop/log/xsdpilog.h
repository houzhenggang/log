#ifndef WORKLOG_H
#define WORKLOG_H
#include <semaphore.h>

#include"lib/xsarray.h"
#include"lib/xs_list.h"
#include"lib/getparamvalue.h"


#define  RCV_PKT_NUM_PATH  "/opt/xslog/rcvpkt.json"
#define FMT_RCVPKT_JSON   \
    "{" \
    "   \"Port\":%d,"  \
    "   \"RcvPkt\":%lu"\
    "} \r\n"

#define FMT_PARSE_JSON  \
    "{"\
    "   \"ParsePkt\":%lu"\
    "}"



#define My_BUFSIZ  2048
/*~~~~~~~~~~~~~~~wechatID~~~~~~~~~~~~~~~~*/
#define MIN_WECHAT_UPDATE_TIME 300
#define MIN_WECHAT_CLEAR_TIME 2*24*3600//clear time

typedef struct wechatID{
    u_int32_t uID;
    time_t time;
}WECHATID,*PWECHATID;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
extern unsigned long long recvpacket ,parsepacket,dequeuepacket ,enqueuepacket ,mallocfail;


typedef  enum{
    unknow=0,
    qq_picture,
    qq_video,
    qq_file,
    wechat_picture,
    wechat_video,
}ChatType;

typedef struct{
    ChatType chatType;
    char nametype[20];
    char id[32];//qq/wechat
}AccountInfo,*pAccountInfo;
#define ACCOUNTINFO_SIZE  sizeof(AccountInfo)

typedef struct{
    char wifi_ip_address[20];
    char wifi_ssid[32];

    char device_brand[32];
    char device_type[64];

    char mac[32];
    char bssid[32];

    int64_t imei;
    int64_t imsi;
}PhoneMsg,*pPhoneMsg;
#define MOBILEMSG_SIZE  sizeof(PhoneMsg)

typedef struct _LOCALINFO
{
    int nLabs;
    char szCity[128];
    char szCountry[256];
    double latitude;
    double longitude;
    char szSubdivisions[128];
}LocalInfo,*PLocalInfo;

typedef struct _parsefiled{
    char bufLabel[10];
    char szbufDate[12];
    char bufTime[10];
    char bufSrc[30];
    char bufSip[20];
    int nSport;
    char bufDst[30];
    char bufDip[20];
    int nDport;
    char bufSmac[18];
    char bufType[32];
    char bufProto[10];
    char bufMethod[10];
    char bufUrl[1500];
    u_int bufDate;
    char bufSnat[30];
    char bufSnat_ip[20];
    int nSnat_port=0;
    char szUsr[32];
    char L7[10];
    int flagpic;
    char softname[256];
    LocalInfo localinfo;
}ParseFiled,*PParseFiled;
#define PRASEFILED_SIZE sizeof(ParseFiled)


int GetLogMsg_InsertMysql_FromXsdpi();
void XsWork_Init();
void XsWork_Destroy();
void  MysqlGetPartition_InsertMap_Log(PMYDBOP pDB, char *tablename, struct rb_root *pMapPartition);
void  *ThreadXslogToMysql(void *param);
void XsCreate_RcvThread_WithPort();
int XsGet_LocalInfo(ParseFiled* pParseFiled);
int  XsGet_Oth_Filed(char *filed,char *pPopBuf,char *param,size_t len);
#endif // WORKLOG_H
