#ifndef XSTYDEF2_H
#define XSTYDEF2_H

#include <stdlib.h>
#ifdef XSPEEDER_OBJ
#include <rte_config.h>

#endif
#define MAX_BWP_NUMS_PER_PORT       64
#define MAX_EXCLUSIVE_BWP_NUMS_PER_PORT     28
#define MAX_SHARED_BWP_NUMS_PER_PORT    (MAX_BWP_NUMS_PER_PORT-MAX_EXCLUSIVE_BWP_NUMS_PER_PORT)

enum PRILEVEL
{
    PRI_LEVEL_RANDOM=0,
    PRI_LEVEL_HIGHEST,
    PRI_LEVEL_HIGHER,
    PRI_LEVEL_HIGH,
    PRI_LEVEL_GENERAL,
    PRI_LEVEL_LOW,
    PRI_LEVEL_LOWER,
    PRI_LEVEL_LOWEST
};

enum BWPIPE_TYPE
{
    BWPIPE_TYPE_SHARED=0,
    BWPIPE_TYPE_EXCLUSIVE
};

typedef struct _bwpipe_params
{
    u_int id;
    char pipename[50];
    u_int interfaceIDX;
    enum BWPIPE_TYPE bwp_type;
    // enum PRILEVEL pri_level;
    u_int64_t gbw;//byte
    u_int64_t maxbw;//byte
    u_int64_t maxconn;

}BWPOLICYPIPE,*PBWPOLICYPIPE;
#define BWPOLICYPIPE_SIZE       sizeof(BWPOLICYPIPE)

typedef struct bwp_info
{
    u_int16_t bufidx;
    u_int16_t subport;
    u_int16_t pipe;
    u_int16_t tc;
    u_int8_t curque;
    BWPOLICYPIPE param;

}BWPIPE_INFO,*PBWPIPE_INFO;
#define BWPIPE_INFO_SIZE       sizeof(BWPIPE_INFO)


typedef struct _group_sched_rate
{
    u_int groupid;
    u_int64_t rxbps;
    union {
        u_int64_t txbps;
        time_t erasetime;
    };
}GROUPSCHED_RATE,*PGROUPSCHED_RATE;
#define GROUPSCHED_RATE_SIZE        sizeof(GROUPSCHED_RATE)

typedef struct groupschedpipe_info
{
    u_int8_t schedstatus;
    u_int8_t schedid;
    u_int16_t subport;
    u_int16_t pipe;
    u_int16_t tc[2];//0:rx   1:tx
    u_int8_t curque[2];
    GROUPSCHED_RATE param;
    u_int32_t lineid;
    union{
        u_int16_t sessionid;
        u_int32_t lip;
    };

}GROUPSCHEDPIPE_INFO,*PGROUPSCHEDPIPE_INFO;
#define GROUPSCHEDPIPE_INFO_SIZE       sizeof(GROUPSCHEDPIPE_INFO)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ track ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct bwpipe_track
{
#define BWPIPE_TRACK_FIN    1
#define BWPIPE_TRACK_FIN_NO_SCHED       2
    u_int8_t u_fin[2];//1:has sched, 2:no sched
    u_int8_t detect_nums[2];
    u_int8_t subport[2];
    u_int16_t pipe[2];
    u_int8_t tc[2];
    u_int8_t queue[2];
    u_int8_t track_flag;
}BWPIPE_TRACK,*PBWPIPE_TRACK;
#define BWP_TRACK_SIZE      sizeof(BWP_TRACK)

typedef struct __ddos_cfg
{
    u_int64_t syns,perip_syns;
    u_int64_t udps,perip_udps;
    u_int64_t icmps,perip_icmps;
}DDOSCFG,*PDDOSCFG;
#define DDOSCFG_T_SIZE      sizeof(DDOSCFG)

typedef struct _ttl_cfg
{
    u_int32_t max_ttl;
    u_int32_t min_ttl;
    u_int32_t route_track_ttl;
    u_int32_t dns_collect_ttl;
    u_int32_t otherflow_ttl;
    union{
        u_int32_t ttl[6];
        struct {
            u_int32_t syn_waitime;
            u_int32_t fin_waitime;
            u_int32_t tcpflow_ttl;
            u_int32_t udpflow_ttl;
            u_int32_t icmpflow_ttl;
            u_int32_t dnsflow_ttl;

        };
    };
}XSTTL_CFG,*PXSTTLCFG;


/*   url log   */
typedef struct _url_log
{
    time_t start;
    u_int32_t sip,dip;
    u_int16_t url_len;
    char *pUrl;
}URL_LOG_NODE,*PURL_LOG_NODE;
#define URL_LOG_NODE_SIZE       sizeof(URL_LOG_NODE)

#endif // XSTYDEF2_H

