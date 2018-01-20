#ifndef XSLOCKFILE_H_
#define XSLOCKFILE_H_


//version
#define XSLOG_VERSION              "18-0102-1"
#define XSIDC_VERSION                "17-1102-1"
#define XSPEEDER_VERSION          "17-1107-1"
#define XSDHCP_VERSION              "17-0801-1"
#define XSDAEMON_VERSION         "17-0626-1"
#define XSDPI_VERSION                    "17-1013-1"
#define XSPPPoEClient_VERSION        "17-1011-1"
#define XSPPPoEServer_VERSION        "17-1107-1"
#define XSPPTPClient_VERSION         "17-0809-1"
#define XSOPVPNC_VERSION             "16-0713-1"
#define XSDDNS_VERSION               "17-0802-1"
#define XSOPVPNS_VERSION            "16-0713-1"
#define XSUSERIDENTIFY_VERSION   "17-0921-1"

#define XSDPI_VER_NUMS      20171026    //if update xsdpi (init\format...)rule, XSDPI_VER_NUMS+= 1


//lockfile
#define XSIDC_LOCKFILE             "/var/run/xsidc.lock"
#define XSPEEDER_LOCKFILE       "/var/run/xspeeder.lock"
#define XSDDNS_LOCKFILE         "/var/run/xsddns.lock"
#define XSDPI_LOCKFILE              "/var/run/xsdpi.lock"
#define XSDHCP_LOCKFILE         "/var/run/xsdhcp.lock"
#define XSPPPOEC_LOCKFILE      "/var/run/xspppoec.lock"
#define XSPPPOES_LOCKFILE      "/var/run/xspppoes.lock"
#define XSDAEMON_LOCKFILE     "/var/run/xsdaemon.lock"
#define XSPPTPClient_LOCKFILE   "/var/run/xspptp.lock"
#define XSOPVPNC_LOCKFILE       "/var/run/xsopvpnc.lock"
#define XSOPVPNS_LOCKFILE       "/var/run/xsopvpns.lock"
#define XSUSERIDENTIFY_LOCKFILE "/var/run/xsuseridentify.lock"


//pid file
#define XSSXZLOG_PID            "/var/run/xssxzlog.pid"
#define XSPEEDER_PID            "/var/run/xspeeder.pid"
#define XSDHCP_PID               "/var/run/xsdhcp.pid"
#define XSDAEMON_PID            "/var/run/xsdaemon.pid"
#define XSIDC_IDC_PID               "/var/run/xsidc.pid"
#define XSPPPOEC_PID            "/var/run/xspppoec.pid"
#define XSPPPOES_PID            "/var/run/xspppoes.pid"
#define XSDPI_PID                   "/var/run/xsdpi.pid"
#define XSPPTPClient_PID                "/var/run/xspptp.pid"
#define XSOPVPNC_PID       "/var/run/xsopvpnc.pid"
#define XSDDNS_PID              "/var/run/xsddns.pid"
#define XSOPVPNS_PID       "/var/run/xsopvpns.pid"
#define XSUSERIDENTIFY_PID "/var/run/xsuseridentify.pid"


#define XSDHCP_UDP_WORK_PORT                             6667
#define XSPEEDER_UDP_WORK_PORT                          6668
#define XSIDC_UDP_WORK_PORT                                6669
#define XSPPPOEC_UDP_WORK_PORT                          6670
#define XSPPPOES_UDP_WORK_PORT                          6671
#define XSPPTPC_UDP_WORK_PORT                             6672
#define XSOPVPNC_UDP_WORK_PORT                          6673
#define XSOPVPNS_UDP_WORK_PORT                          6674
#define XSWEBAUTH_UDP_WORK_PORT                        6675
#define BACKSTAGE_EPOLL_PORT                                  8001

#define XSTAG_PPTPVPN_PROCESS                   1723

#define XSPEEDER_PROCESS                          2001
#define XSDHCPv4_PROCESS                          2101
#define XSIDCC_PROCESS                              2201

#define XSPPPOEC_DISC_PROCESS                  2301
#define XSPPPOEC_SES_PROCESS                   2302
#define XSPPPOEC_MSG                                2303
#define XSPPPOEC_CONF                               2304

#define XSPPPOES_DISC_PROCESS                  2310
#define XSPPPOES_SES_PROCESS                   2311
#define XSPPPOES_RADIUS_PROCESS              2312
#define XSPPPOES_MSG                                2313
#define XSPPPOES_CONF                               2314
#define XSPPPOES_USER_GROUP                     2315
#define XSPPPOES_DM_COA_PROCESS              2316       /*Deal Radius-Server's Request*/
#define XSPPPOES_WEBAUTH                            2317
#define XSPPPOES_PPPOE_PROXY_PROCESS    2318        //PPPoE代理通信Process

#define XSPPTPC_TCP_PPTP_PROCESS             2410
#define XSPPTPC_GRE_PROCESS                     2411
#define XSPPTPC_CONF                                 2412

#define XSOPVPNC_FROM_WAN_PROCESS     2501
#define XSOPVPNC_TO_WAN_PROCESS         2502
#define XSOPVPNC_FROM_LAN_PROCESS     2503
#define XSOPVPNC_TO_LAN_PROCESS          2504

#define XSOPVPNS_FROM_WAN_PROCESS     2511
#define XSOPVPNS_TO_WAN_PROCESS         2512
#define XSOPVPNS_FROM_LAN_PROCESS    2513
#define XSOPVPNS_TO_LAN_PROCESS        2514

#define XSDDNS_REGIST_LINE                      2601

#define XS_BRIDGE_HOST_PROESS               2621
#define XS_HEARTBEAT_PROCESS            2701


#define CODE_REQUEST                            0
#define CODE_REPLY                                  1
#define CODE_VERIFY                                 2
#define CODE_ADD                                        3
#define CODE_DELETE                                 4
#define CODE_UPDATE                                     5
#define CODE_GET_IFLINE_INFO                            6
#define CODE_REPLY_IFLINE_INFO                      7
#define CODE_RECONNECT                                  8
#define CODE_CONN_SESSIONID                        9
#define CODE_CONN_FINSH                             10
#define CODE_CONN_DELETE                            11
#define CODE_ENABLE                                     12
#define CODE_KIND                                       13
#define CODE_IP_FLOW                                   14
#define CODE_AUTH                                       15
#define CODE_PROXY_FINISH                         16
#define CODE_KIND2                                         17


#define XSIPC_TAG_BUF_LEN       1500
#define XSHeartbeat_gap 5

/* ########################### xsverify ############################### */
//IDC VeriFy status
#define IDC_VERIFY_OK               0
#define IDC_VERIFY_UNREGIST     1
#define IDC_VERIFY_EXPIRE        2
#define IDC_VERIFY_EXCEPTION    3
#define IDC_VERIFY_ERROR        11
#define IDC_VERIFY_OFFLINE      99
#define IDC_VERIFY_OTHER        100


#define IDC_DEF_MAX_CONNS      10000
#define IDC_DEF_MAX_ONLINES    50
#define IDC_CTL_CONN      (1<<0)
#define IDC_CTL_ONLINE    (1<<1)
#define IDC_CTL_PORTS       (1<<2)

#define INCOME_CMD_CNT              1204


/* ~~~~~~~~~~~~~~~~~ WEB Auth~~~~~~~~~~~~~~~~~  */
#define WEBAUTH_ENABLE      (1<<0)
#define WEBAUTH_IP                (1<<1)
#define WEBAUTH_MAC           (1<<2)
#define WEBAUTH_NoSense     (1<<3)


#ifndef ETH_BYTES
#define ETH_BYTES_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define ETH_BYTES(addr) \
    addr[0],\
    addr[1],\
    addr[2],\
    addr[3],\
    addr[4],\
    addr[5]
#endif

#ifndef IPv4_BYTES
#define IPv4_BYTES_FMT "%u.%u.%u.%u"
#define IPv4_BYTES(addr) \
    (uint8_t) (((addr) ) & 0xFF),\
    (uint8_t) (((addr) >> 8) & 0xFF),\
    (uint8_t) (((addr) >> 16) & 0xFF),\
    (uint8_t) (((addr) >> 24) & 0xFF)
#endif

#ifndef IPv6_BYTES
#define IPv6_BYTES_FMT "%02x%02x:%02x%02x:%02x%02x:%02x%02x:"\
    "%02x%02x:%02x%02x:%02x%02x:%02x%02x"
#define IPv6_BYTES(addr) \
    addr[0],  addr[1], addr[2],  addr[3], \
    addr[4],  addr[5], addr[6],  addr[7], \
    addr[8],  addr[9], addr[10], addr[11],\
    addr[12], addr[13],addr[14], addr[15]
#endif


#define UPDATE_PPPOES_STATUES_2_XSPEEDER_INTERVAL   5

#endif
