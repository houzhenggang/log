#ifndef __XSTYDEF_H__
#define  __XSTYDEF_H__

#include <stdlib.h>
#include "xstydef2.h"
#include "xsbool.h"
#include "rbtree.h"
#include "lib/uintsegtable.h"
#include <lib/xsdefine.h>
#include <semaphore.h>

#include "xsarray.h"

#ifdef XSPEEDER_OBJ
#ifdef HAVE_DPDK
#include <rte_config.h>
#include <rte_ethdev.h>
#include "rte_ip_frag.h"
#include "xs_meter.h"
#endif
#endif

enum DIRECTION{
    LAN_TO_WAN=0,
    WAN_TO_LAN,
    LAN_TO_LAN,
    WAN_TO_WAN
};

enum MIRROR_DIRECT
{
    MIRROR_IN=0,
    MIRROR_OUT,
    MIRROR_DOUBLE
};

enum PORTMODE {
    MODE_ROUTE=0,
    MODE_BRIDGE,
    MODE_MIRROR
};

enum Xs_ACT
{
    ACT_DROP=0,
    ACT_ROUTE,
    ACT_NAT,
    ACT_HIJACK,
    ACT_VPNPROXY,
    ACT_LAN2LAN_NAT
};

enum IFPOSITION{
    POSITION_LAN=0,
    POSITION_WAN
};

enum LINETYPE {
    LINE_STATIC=0,//static ip
    LINE_PPPOEC,
    LINE_PPPOES
};

enum LAYER {
    LAYRE2=2,
    LAYRE3
};


enum ADDR_TYPE
{
    ADDR_ANY=0,
    ADDR_RANGE,
    ADDR_GROUP
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~ XsDNS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _domain_node
{
    char domain[HAOT_SERVER_NAME_SIZE];
    u_int16_t domain_len;
    u_int32_t groupid;
    u_int32_t id;
    u_int8_t matchtype;//0:all  1:include
}DOMAINNODE,*PDOMAINNODE;
#define DOMAINNODE_SIZE     sizeof(DOMAINNODE)

/* ########################### XsNat ############################### */
#ifdef XSPEEDER_OBJ

typedef struct _tuple{
    u_int32_t src;
    union{
        u_int16_t sport;
        u_int32_t sid;
    };

    u_int32_t dst;
    union{
        u_int16_t dport;
        u_int32_t did;
    };
}TUPLE,*PTUPLE;
#define TUPLE_SIZE sizeof(TUPLE)

typedef struct _nattuple
{
    u_int8_t tcp_syn:1,tcp_fin:1;
    time_t lastseen;
    //u_int32_t lineid;
    u_int16_t nproto;
    struct _tuple original;
    struct _tuple alter;
}NATTUPLE,*PNATTUPLE;
#define NATTUPLE_SIZE      sizeof(NATTUPLE)


typedef struct natttupetrack
{
    NATTUPLE Tuple;
}NATTUPE_TRACK,*PNATTUPE_TRACK;
#define NATTUPE_TRACK_SIZE      sizeof(NATTUPE_TRACK)

typedef struct _nodeport
{
    u_int8_t enable;//for portmap;
    u_int32_t lineid;
    //    u_int32_t publip;
    u_int32_t prilip;
    u_int16_t pubport;//netbit
    u_int16_t priport;
    u_int8_t protobitmap;
    struct rb_root HashTuple[TUPLE_ROOTS];
    struct _nodeport *left, *right;
}NODEPORT,*PNODEPORT;
#define NODEPORT_SIZE sizeof(NODEPORT)

typedef struct _ip_nodeport
{
    u_int32_t lineid;
    //    u_int32_t wanip;
    time_t last_seen;
    u_int32_t _nPortNum;
    PNODEPORT *pPool;
}WANIPNODE,*PWANIPNODE;
#define WANIPNODE_SIZE          sizeof(WANIPNODE)

/* ########################### XsRoute ############################### */
typedef struct _ipgroup_t
{
    time_t erase_time;
    u_int groupid;
    char groupname[64];
    struct UintSegTable *pTable;
}IPGROUP_T,*PIPGROUP_T;
#define IPGROUP_T_SIZE     sizeof(struct _ipgroup_t)

struct xs_pppoeConnect_struct
{
    u_char local_mac[ETH_ALEN];  //本机wan_mac
    u_char remote_mac[ETH_ALEN];  //远程mac
    char user[64];
    char passwd[64];
    char serverName[64];// pppoe 服务器名称
    char acName[64];//   pppoe指定接入服务器
    u_int16_t session_id;
    u_int16_t  state;         //pppoe当前状态
    char  auth_status;    //认证当前状态
    u_int32_t reconnect;  //重拨等待时间
    u_int16_t mru;              //远程最大接收单元
    u_int32_t mtu;
    time_t lastdial,lastpack;
};

typedef struct _interfaceline
{
    u_int32_t lineid;
    u_int8_t enable;
    char name[32];
    u_int8_t driveid : 6;// portid <MAX_DATA_STREAMS(64)
    u_int8_t groupid : 6;//groupid<=MAX_LINE_GROUP_NUMS (15)
    u_int32_t networkcard;//DPDKPORT.DbIdx;
    u_int32_t isp_up_speed;//mbps
    u_int32_t isp_down_speed;//mbps

    enum LINETYPE kind;//line type 0:static IP,1:pppoe
    u_int8_t scale;//比重 default 1
    u_int sc_low,sc_up;//计算出来的比重范围
    u_int sc_group_low,sc_group_up;
    u_int32_t payload;//负载

    u_int32_t lip; //netbit
    u_int32_t mask;  //netbit
    u_int32_t gatway;//netbit
    u_int32_t dns;//netbit
    u_int32_t dns2;//netbit;
    u_char gatmac[ETH_ALEN];
    char vmac_str[20];
    u_char vmac[ETH_ALEN];//
    u_int32_t broadcast;
    u_int16_t vlan;//hostbit
    u_int16_t innervlan:12,open_qinq:4;
    u_int16_t tpid;//netbit
    VLAN_BITMASK inner_vlan_bitmask;

    u_int16_t mtu, l3_mtu;

    u_int32_t heartip;
    int8_t ping; //允许  ping
    time_t last_heart_reply,last_heart_request;
    u_int16_t	last_ping_seq;

    struct xs_pppoeConnect_struct pppoeConn;
    struct xs_pptpvpn_conn pptpconn;


    /* ~~~~~~~~~~~~~~~~~~~~~~~~~ */
    struct rb_root *map_arpInfo;
    struct rb_root *map_pppoes_client_schedinfo;

    //////////////////////////////////////////
    u_int64_t rxbytes_rt, txbytes_rt;     //real time
    u_int64_t prev_rxbytes_rt,prev_txbytes_rt;
    u_long rxspeed_rt,txspeed_rt;

    u_long balance_bytes,group_balance_bytes;
    u_int16_t balance_cur_scale,group_balance_cur_scale;
    u_int16_t peer_dead;

    time_t erasetime;
    u_int8_t disconnection,real_status;
    u_int8_t insertinfo_to_db;
    u_int16_t uid;
    void *pPortCfg;

    /////////////////////////////////////
    u_int64_t tmp_burst;
    u_int64_t roughly_burst;
    u_int64_t precisely_burst;
    u_int64_t burstlimit;
}IFLINE,*PIFLINE;
#define IFLINE_SIZE     sizeof(IFLINE)

typedef struct _sample_vlan_ifline_group
{
    PIFLINE tot_ifline[32];
    u_int16_t tot_line_nums;
    u_int16_t vlan_id;
}SVIG,*PSVIG;
#define SVIG_SIZE   sizeof(SVIG)

typedef struct _line_member
{
    time_t erase_time;

    PIFLINE tot_ifline[MAX_VLAN_ID+1];
    u_int16_t tot_line_nums;
    struct rb_root map_vmac_to_ifline;
    PSVIG pSvig[MAX_VLAN_ID + 1];
}LINE_MEMBER,*PLINE_MEMBER;
#define LINE_MEMBER_SIZE    sizeof(LINE_MEMBER)

typedef struct _line_group_member
{
    u_int8_t groupId;
    char groupName[19];
    u_int16_t payloadType;
    u_int16_t payloadTTL;
    u_int16_t cur_line;
    u_int16_t lineNums;
    u_int scale_base;
    PIFLINE line[MAX_VLAN_ID+1];

}LINE_GROUP_MEMBER,*PLINE_GROUP_MEMBER;

typedef struct _line_group
{
    LINE_GROUP_MEMBER linegroup[MAX_LINE_GROUP_NUMS+1];
    time_t erase_time;
}LINEGROUP,*PLINEGROUP;
#define LINEGROUP_SIZE  sizeof(LINEGROUP)

#ifdef XSPEEDER_POWER
enum freq_scale_hint_t
{
    FREQ_LOWER    =      -1,
    FREQ_CURRENT  =       0,
    FREQ_HIGHER   =       1,
    FREQ_HIGHEST  =       2
};
#endif

typedef struct thread_conf{
    struct rte_ip_frag_tbl *frag_tbl;
    struct rte_ip_frag_death_row death_row;
#ifdef XSPEEDER_POWER
    enum freq_scale_hint_t freq_up_hint;
    u_int32_t zero_rx_packet_count;
#define MIN_ZERO_POLL_COUNT 10
    u_int32_t idle_hint;
#endif
    u_int16_t rx_queueid,tx_queueid;
    u_int8_t portid;
}THREAD_CONF,*PTHREAD_CONF;



typedef struct _dpdk_port
{
    u_int32_t DbIdx;
    u_int8_t enable;
    u_int8_t driveid;
    u_int32_t peerid;
    u_int8_t n_rx_queue ;
    u_int8_t n_tx_queue;
    u_int16_t mtu;
    char portname[IFNAMSIZ];
    u_char szMac[ETH_ALEN];
    char mac_str[20];
    char pciaddr[10];//bus
    char pciaddr2[16];//bus
    char chip[32];// 芯片型号
    enum PORTMODE model;
    enum IFPOSITION position;
    struct rte_eth_link link;
    struct rte_eth_conf port_conf;
    struct rte_eth_dev_info dev_info;
    struct rte_eth_stats eth_stats;
    struct rte_mempool *mbuf_pool;
    struct rte_ring *tag_ring;
    struct rte_ring *branch_tx_ring;
    struct rte_ring *tx_ring;
    struct rte_ring *tx_sched_ring;
    struct rte_ring *groupsched_ring;
    struct rte_ring *loop_ring;
#ifdef USE_DPDK_KNI
    struct rte_kni *kni;
    struct rte_ring *kni_ring;
#endif

    u_int tick,tick2;
    THREAD_CONF thread_conf[MAX_QUEUE_PER_PORT];

    PLINE_MEMBER pLineMember;
    struct rb_root *map_mirrorpolicytable; //single thread with add,delete or update
    struct rb_root *map_all_mac;//single thread with add,delete or update
    u_int8_t sched_init_ok;
    struct _dpdk_port *pPeerPort;
    u_int64_t prev_rxbytes,prev_txbytes;
    u_long real_rxspeed,real_txspeed;
    u_long tx_drop;
}DPDKPORT,*PDPDKPORT;
#define DPDKPORT_SIZE       sizeof(DPDKPORT)

typedef struct dpdk_portarray
{
    u_int16_t totalnums;
    PDPDKPORT PortArray[RTE_MAX_ETHPORTS+1];
}DPDK_PORT_ARRAY,*PDPDK_PORT_ARRAY;
#define DPDK_PORT_ARRAY_SIZE        sizeof(DPDK_PORT_ARRAY)

typedef struct _arpinfo
{
    u_int32_t dstip;
    u_char dstmac[ETH_ALEN];
    time_t lastseen;
    u_int16_t innervlan:12,isbind:2,insert_db:1,erase:1;
}ARPINFO,*PARPINFO;
#define ARPINFO_SIZE        sizeof(ARPINFO)


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ track ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _l2track
{
    u_char mac[ETH_ALEN];
    PIFLINE pIFLine;
}L2TRACK,*PL2TRACK;
#define L2TRACK_SIZE        sizeof(L2TRACK)

typedef struct _sessiontrack
{
    time_t detectseen;
    u_int16_t session;
    void *pgroupschedinfo;
}SESSIONTIDRACK,*PSESSIONTIDRACK;

typedef struct _usefordpiflowtrack
{
    enum IFPOSITION position;
    enum Xs_ACT act[2];
    enum LAYER layer[2];
    PIFLINE pL3[2];
    L2TRACK L2[2];
    u_int16_t inner_vlan[2];
    SESSIONTIDRACK pppoessession;
    u_char policyroute_detected[2];
    void *pBRP[2];
    u_int32_t inlineid[2];
    u_int8_t policyroute_flag[2];
    u_int8_t track_flag;
    u_int8_t dodnsctl:1,loop_burst:1,tcp_step:6;
    u_int8_t nodialing_flag[2],nodialing[2];
    int (*trackcallback)(void *pObj,void *pflow);
}USEFORDPIFLOWTRACK,*PUSEFORDPIFLOWTRACK;
#define USEFORDPIFLOWTRACK_SIZE      sizeof(USEFORDPIFLOWTRACK)


typedef struct _mirrorpolicytrack
{
    //time_t lastseen;
    void *policy;
    u_int8_t detect;
    u_int8_t degree;
}MIRRORPOLICY_TRACK,*PMIRRORPOLICY_TRACK;
#define MIRRORPOLICY_TRACK_SIZE     sizeof(MIRRORPOLICY_TRACK)

typedef struct _mirrorpolicytrack_group
{
    u_int8_t track_flag;
    MIRRORPOLICY_TRACK track[2][2];
}MIRRORPOLICYTRACK_GROUP,*PMIRRORPOLICYTRACK_GROUP;
#define MIRRORPOLICYTRACK_GROUP_SIZE            sizeof(MIRRORPOLICYTRACK_GROUP)

#endif


/* ########################### XsRule ############################### */

typedef struct prototmpnode{
    u_int protoid;
    u_int16_t rootid,parentid;
    char name[40];
}TMPPROTONODE,*PTMPPROTONODE;

typedef struct {
    size_t      len;
    u_char     *data;
} dw_str_t;


typedef struct {
    dw_str_t   key;
    dw_str_t   value;
} dw_keyval_t;

typedef struct _IPMask//分布0515
{
    u_int32_t lip;//netbit
    u_int8_t     icdr;// 0<nMask<=32
}IPMASK,*PIPMASK;
#define IPMASK_SIZE sizeof(IPMASK)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ track ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct _detectflow
{
    enum IFPOSITION position;
    enum DIRECTION n_direct;
    u_int8_t n_IPversion;//ipv4  ipv6
    u_int16_t n_l4proto;//udp/tcp icmp
    u_int16_t n_l7proto;//qq  http  ...
    u_int16_t l7_parentid;
    u_int16_t l7_rootid;

    u_int32_t n_packlen;
    u_int8_t ip_hdrlen;
    u_int16_t pppoe_disc_len;// * + ip+tcp/udp/icmp+payload
    u_int16_t pppoe_sess_len; //  * + ip+tcp/udp/icmp+payload
    u_int16_t l3_len;//ip+tcp/udp/icmp+payload
    u_int16_t l4_len;//tcp/udp/icmp+payload
    union{
        u_int16_t vlantag[2];
        struct {u_int16_t outer_vlanid;
                u_int16_t inner_vlanid;};//hostbit
    };
    u_int16_t sessionid;//hostbit
    u_int8_t vlan_len;
    union{
        u_int32_t ip[2]; //netbit
        struct {u_int32_t lower_ip, upper_ip;};
    };

    union{
        u_int16_t port[2];
        struct {u_int16_t lower_port, upper_port;};
    };
    u_char *pFrame;
    struct xs_ethhdr *pEthHdr;
    struct xs_vlanhdr *pVlanhdr;
    struct xs_pppoe_disc_hdr *pPPPoe_Disc;    u_int16_t disc_offset;
    struct xs_pppoe_ses_hdr *pPPPoe_Ses;    u_int16_t session_offset;
    struct xs_arphdr *pArphdr;              u_int16_t arp_offset;
    struct xs_iphdr *pIphv4;
    struct xs_ipv6hdr *pIphv6;
    u_int16_t ip_offset;
    union{
        struct xs_tcphdr *tcph;
        struct xs_udphdr *udph;
        struct xs_icmphdr *icmp;
    };
    u_int16_t l4_offset;

    struct gre_tunnel_content {
        struct xs_grehdr  *pgre;
        union{
            struct xs_iphdr *pIphv4;
            struct xs_ipv6hdr *pIphv6;
        };
        u_int16_t gre_offset;
        u_int8_t gre_hdr_len;
        u_int16_t ppp_proto;
        u_int16_t ip_offset;
        u_int8_t ip_ver;
        u_int8_t n_l4proto;
        u_int16_t payload_len;
        u_int16_t l4_offset;
        union{
            struct xs_tcphdr *tcph;
            struct xs_udphdr *udph;
            struct xs_icmphdr *icmp;
        };
    }gre_tunnel_content;

#ifdef XSPEEDER_OBJ
    PDPDKPORT pOutPortCfg;
    PDPDKPORT pInPortCfg;
    PIFLINE pOutIFLine;
    PIFLINE pInIFLine;
    PNATTUPE_TRACK pTupleTrack;
    PUSEFORDPIFLOWTRACK pRouteTrack;
    PMIRRORPOLICYTRACK_GROUP pMirrrorPolicyTrack;
    PBWPIPE_TRACK pBwTrack;
    time_t flowlastseen;
#endif
    u_int8_t trackdir:1,old_trackdir:1,ip_frag_Reassemble:1,gre_proxy:1,dns_ctl:1, ctl_rest:2,parse_l4:1;
    u_int8_t looppack : 1,direct_loop:2,rc:5;
#ifndef MAX_DNS_REQUESTS
#define MAX_DNS_REQUESTS 16
#endif
    u_int32_t dns_record[MAX_DNS_REQUESTS],num_dns_records;
    PDOMAINNODE pDomainNode;
    void *pdpiflow;
    void *pgroupschedinfo;
    u_char tmp[64];
    u_char *p64KbTmpBuf;
    u_char *p64KbTmpBuf2;
    u_char *p64KbTmpBuf3;
    u_int this_ip_connets;
}DETECTFLOW,*PDETECTFLOW;
#define DETECTFLOW_SIZE      sizeof(DETECTFLOW)


typedef struct _myparam
{
    void *p1,*p2,*p3;
    long l1;
}MYPARAM,*PMYPARAM;
#define MYPARAM_SIZE     sizeof(MYPARAM)



typedef struct nodialing{
    u_int32_t id;
    u_int32_t ip;
   // u_char mac[ETH_ALEN];
    u_int8_t nomac:2;
}NODIALING,*PNODIALING;
#define NODIALING_SIZE      sizeof(NODIALING)


#endif // XSDEFINE

