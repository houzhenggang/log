#ifndef XSIPCSTRUCT_H
#define XSIPCSTRUCT_H

#include <stdlib.h>
#include "xscompat.h"
/* Allow for 1500-byte PPPoE data which makes the
   Ethernet packet size bigger by 8 bytes */
#define ETH_JUMBO_LEN (ETH_DATA_LEN+8)

/* A PPPoE Packet, including Ethernet headers */
typedef struct PPPoEPacketStruct {
    struct xs_ethhdr ethHdr;	/* Ethernet header */
#if defined(__LITTLE_ENDIAN__)
    unsigned int type:4;	/* PPPoE Type (must be 1) */
    unsigned int ver:4;		/* PPPoE Version (must be 1) */
#elif defined(__BIG_ENDIAN__)
    unsigned int ver:4;		/* PPPoE Version (must be 1) */
    unsigned int type:4;	/* PPPoE Type (must be 1) */
#else
# error "Byte order must be defined"
#endif
    unsigned int code:8;	/* PPPoE code */
    unsigned int session:16;	/* PPPoE session */
    unsigned int length:16;	/* Payload length */
    unsigned char payload[ETH_JUMBO_LEN]; /* A bit of room to spare */
} PPPoEPacket;


struct xstag{
    u_int32_t process;//对端进程代号
    u_int16_t code;//操作类型
    u_int8_t driveid;  //dpdk接收网卡
    union{
        u_int32_t lineid; // 线路ID
        struct {
            u_int16_t line1;
            u_int16_t line2;
        };
    };
    u_int16_t session;
    union{
        u_int16_t flag[2];
        struct {u_int16_t outer_vlanid : 12 ; u_int16_t dir:1,r1:3;
                u_int16_t inner_vlanid: 12 ; u_int16_t r2:4;};//hostbit
    };
    struct xs_ethhdr eth;//
    u_int16_t datalen;
    unsigned char pdata[1]; //占位符   IP+ tcp/udp/icmp + data
};
#define XSTAG_T_SIZE        (sizeof(struct xstag) -1 )

struct xstag_head
{
    u_int16_t dport;
    u_int16_t datalen;
    union {
        char data[1];
        struct xstag tag;
    };
};
#define XSTAG_HEZD_T_SIZE       sizeof(struct xstag_head)


enum  looptag_dirx{
    LOOP_Normal=0,
    LOOP_NoRoute_LanNat,
    LOOP_NoRoute_NoNat,
    LOOP_MYSELF_LOOP
};
struct xslooptag
{
    void *pPort;
    void *pIFLine;
    int (*loopcallback)(void *pObj,void *pflow);
    u_int16_t datalen;
    u_int16_t session;
    u_int8_t tunIp_not_change;
    enum  looptag_dirx dirtx;
    u_char pdata[1];
} __attribute__((__packed__));
#define XSLOOPTAG_T_SIZE        (sizeof(struct xslooptag)-1)

typedef struct conninfohead
{
    u_int32_t lineid;
    u_int32_t driveid;
    u_int32_t netcard;
    u_int32_t kind;
    u_int16_t mtu;
    u_int32_t reconntime;
    char user[64];
    char passwd[64];
    char acName[64];
    char serverName[64];
    u_int16_t serverID;                     //用于PPPoE代理，对应的Lan服务ID
    u_int16_t sessionID;
}ConnInfoHead,*PConnInfoHead;
#define CONNINFOHEAD_SIZE       sizeof(ConnInfoHead)

typedef struct _xsipcp_info{
    u_int32_t myip;
    u_int32_t hisip;
    u_int32_t dns1;
    u_int32_t dns2;
    u_int32_t win1;
    u_int32_t win2;
    u_int16_t session;//net bit
    u_char gatmac[ETH_ALEN];
    u_int16_t mru;
}XSIPCPINFOMSG,*PXSIPCPINFOMSG;
#define XSIPCPINFOMSG_SIZE       sizeof(XSIPCPINFOMSG)

typedef struct _pppoeclientinfo
{
    u_int32_t lip;
    u_int32_t lineid;
    u_int32_t serverid;
    u_int32_t groupid;
    u_int64_t rxbps,txbps;
    u_char peermac[ETH_ALEN];
    u_int16_t proxy_lineid;
    char account[32];
    time_t express_time;
    u_int32_t last_tick;
    u_int16_t session;
    u_int16_t innervlan:12;
    u_int8_t nosurfing:1;//0:允许上网 1:不允许上网
    u_int8_t rc:3;
    u_int8_t driveid;
    u_int8_t webauth;
}XSPPPOECLIENTINFO,*PXSPPPOECLIENTINFO;
#define XSPPPOECLIENTINFO_SIZE      sizeof(XSPPPOECLIENTINFO)


typedef struct _pppoes_config
{
    u_int32_t lineid;
    u_int32_t enable;
    u_int8_t pap;
    u_int8_t chap;
    u_int16_t minmtu,maxmtu,mtu,mru;
    u_int8_t authentication;
    int echointerval,echofail;
    int maxuser;
    char addrzone[80];
}XSPPPOESCONFIG,*PXSPPPOESCONFIG;
#define XSPPPOESCONFIG_SIZE     sizeof(XSPPPOESCONFIG)

typedef struct _pppes_secret
{
    u_int32_t lineid;
    char user[64];
    char passwd[64];
    time_t expire;
    u_int32_t bindip;
    char bindmac[20];
}XSPPPOESECRET,*PXSPPPOESECRET;
#define XSPPPOESECRET_SIZE      sizeof(XSPPPOESECRET)

typedef struct _radius_ip_flow
{
    u_int32_t lip;
    u_int32_t serverid;
    u_int64_t bytes[2];
    u_int packets[2];
    u_int64_t clientkey;
}RADIUS_IP_FLOW,*PRADIU_IP_FLOW;
#define RADIUS_IP_FLOW_SIZE     sizeof(RADIUS_IP_FLOW)

typedef struct pptpinfohead
{
    u_int32_t lineid;                         //线路ID
    u_int32_t peerIp;                       //VPN服务器IP
    u_int32_t localIp;                      //本地线路IP
    u_char username[64];               //用户名
    u_char passwd[64];                  //密码
    u_int16_t mppe;                       //MPPE加密
}PPTPLinkInfo,*PPPTPLinkInfo;
#define PPTPLinkInfoSize       sizeof(PPTPLinkInfo)

struct xs_pptp_conn
{
    u_int32_t myip;
    u_int32_t perip;
    u_int seq;
    u_int16_t hismru;
    u_int16_t compress;
    u_int16_t mppe;
    u_int16_t callid;

    char user[64];
    char passwd[64];

    u_int8_t enable,echo;
    char option_send[6];//CILEN_MPPE
    char option_recv[6];//CILEN_MPPE
    char send_key[16];//MPPE_MAX_KEY_LEN
    char recv_key[16];//MPPE_MAX_KEY_LEN
};

struct xs_pptpCinfo
{
    u_int32_t myip;
    u_int32_t perip;
    u_int32_t seq;
    u_int16_t hismru;
    u_int16_t compress;
    u_int16_t mppe;
    u_int16_t callid;

    void *pMPPE_Xstate;
    void *pMPPE_Rstate;
    u_int8_t enable;
};

typedef struct _ipc_idc_
{
    char ver[33];
    int status;
    long a;
    long b;
    long result;
    u_int hostid;
    u_long ctl_burstconns;
    u_long ctl_maxonlines;
    u_int8_t ctl_flag;
    time_t lastseen;
    u_int8_t dodpi;
    char szSerial[64];
    u_int32_t serverip;
    u_int16_t serverport;
    u_int8_t do_income;
    u_int8_t cpu0_templature,cpu1_templature,ctl_max_ports;
    u_int8_t syn_count_for_404;
}IPC_IDC,*PIPC_IDC;
#define IPC_IDC_SIZE            sizeof(IPC_IDC)


typedef struct _tun_trans
{
    void* pLine;
    u_int32_t last_tick;
    u_int32_t src,dst,middlesip;
    u_int16_t sport,dport;
    u_int16_t nproto;
    enum  looptag_dirx direct_loop;
}TUNTRANS,*PTUNTRANS;
#define TUNTRANS_T_SIZE     sizeof(TUNTRANS)

typedef struct _bridge_host_route_track
{
    u_char mac[ETH_ALEN];
    u_int16_t outer_vlanid[2];
    u_int16_t inner_vlanid[2];
    u_int outdevid[2];
    void *outdev[2];
    char user[64];
    u_int8_t user_len;
    u_int groupid;
    u_int8_t format_vlan:4,b_erase:4;
    time_t last_tick;
}BHR_TRACK,*PBHR_TRACK;

typedef struct {
    u_int16_t serverID;                                 //对应服务ID
    u_char clientMac[ETH_ALEN];                 //拨号端Mac地址
    u_int16_t enable;                                   //认证成功/失败      0:失败/下线  1:成功
    u_int32_t gateWayIP;                              //ISP服务器IP
    u_int32_t clientIP;                                  //客户机上网IP
    u_int32_t dns1,dns2;                             //
    u_int32_t windns1,windns2;
    u_int16_t sessionID;                              //拨号侧会话ID
}PPPoEProxy,*PPPPoEProxy;
#define PPPoEProxy_Size sizeof(PPPoEProxy)

#endif // XSIPCSTRUCT_H

