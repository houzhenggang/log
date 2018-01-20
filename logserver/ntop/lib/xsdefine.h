#ifndef XSDEFINE_H
#define XSDEFINE_H

#ifdef XSPEEDER_OBJ
#include <rte_config.h>
#include <rte_mbuf.h>
#endif


#define HAVE_DPDK

//#define USE_DPDK_KNI
//#define USE_IEEE1588_PTP
#define DO_RFC_1812_CHECKS

//#define XS_CODE_DEBUG

#define XS_PPPOE_OBJ
//ttl cfg
#define DEFAULT_SYN_WAITIME      3
#define DEFAULT_FIN_WAITIME       3
#define DEFAULT_TCPFLOW_TTL            180
#define DEFAULT_UDPFLOW_TTL            30
#define DEFAULT_ICMPFLOW_TTL            5
#define DEFAULT_DNSFLOW_TTL     10


#define XSDPI_FLOW_TIMEOUT  180
#define XSROUTE_ARP_TIMEOUT      600


#ifndef HAVE_PTHREAD_SETAFFINITY_NP
#define HAVE_PTHREAD_SETAFFINITY_NP 1
#endif

/* ########################### XsDpi ############################### */

/*
 * These are the types that are the same on all platforms, and that
 * have been defined by <net/bpf.h> for ages.
 */
#define DLT_NULL	0	/* BSD loopback encapsulation */
#define DLT_EN10MB	1	/* Ethernet (10Mb) */
#define DLT_EN3MB	2	/* Experimental Ethernet (3Mb) */
#define DLT_AX25	3	/* Amateur Radio AX.25 */
#define DLT_PRONET	4	/* Proteon ProNET Token Ring */
#define DLT_CHAOS	5	/* Chaos */
#define DLT_IEEE802	6	/* 802.5 Token Ring */
#define DLT_ARCNET	7	/* ARCNET, with BSD-style header */
#define DLT_SLIP	8	/* Serial Line IP */
#define DLT_PPP		9	/* Point-to-point Protocol */
#define DLT_FDDI	10	/* FDDI */
#define DLT_LCP   113 /* Linux Cooked Capture */

#define MAX_SUPORT_RULE_NUMS     4096//must be power of 2
#define MAX_SUPORT_RULE_MASK        (MAX_SUPORT_RULE_NUMS-1)
#define MAX_SUPORT_PROTOCOL_NUMS    2048
#define MIN_CUSTOM_PROTOCOL_NUM     1000
#ifndef MAX_DETECT_THREAD_NUM
#define MAX_DETECT_THREAD_NUM     10
#endif


#define NUM_ROOTS                 512//must be power of 2
#define NUMS_ROOTS_MASK        511  //NUM_ROOTS-1
#define TUPLE_ROOTS                4//must be power of 2
#define TUPLE_ROOTS_MASK        3//TUPLE_ROOTS - 1

#define MAX_SCALE       100


//return
#define XS_ERROR -1
#define XS_OK 0


//port
#define GTP_U_V1_PORT        2152


#ifndef __KERNEL__
//#define HAOT_SERVER_NAME_SIZE   256
#define HAOT_SERVER_NAME_SIZE 128       //lcp
#else
#define HAOT_SERVER_NAME_SIZE   160
#endif
/* ########################### XsNat ############################### */

//#define XS_STATIC_NAT_PROTOCOL_ICMP			(1<<0)
#define XS_STATIC_NAT_PROTOCOL_TCP			(1<<0)
#define XS_STATIC_NAT_PROTOCOL_UDP			(1<<1)
#define  XS_STATIC_NAT_PROTOCOL_ALL     ( XS_STATIC_NAT_PROTOCOL_TCP | XS_STATIC_NAT_PROTOCOL_UDP)


/* ########################### DPDK or XsRoute ############################### */


#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#define RET_IPFRAG_ON_PACK     99990
#define RET_IPFRAG_PACK_OK     99991
#define RET_IPFRAG_PACK_FAILD   99992
#define RTE_KNI_PACK_RX     99993
#ifndef RTE_MAX_ETHPORTS
#define RTE_MAX_ETHPORTS 32
#endif

#define MAX_LINE_GROUP_NUMS  32
#define MAX_VLAN_ID     4094
#define NUMS_PER_SCHEDGROUP     8192


#define RX_RING_SIZE 128
#define TX_RING_SIZE 512
#define MAX_QUEUE_PER_PORT  32
#define NUM_MBUFS 8192
#define MBUF_CACHE_SIZE 250
#define MAX_PKT_BURST 32

#define	BUF_SIZE	RTE_MBUF_DEFAULT_DATAROOM
#define MBUF_SIZE	\
    (BUF_SIZE + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)


/* Configure how many packets ahead to prefetch, when reading packets */
#define PREFETCH_OFFSET	3
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  */


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

#define IPV4HDR_DF_SHIFT        14
#define IPV4HDR_MF_SHIFT        13
#define IPV4HDR_FO_SHIFT        3

#define IPV4HDR_DF_MASK     (1<<IPV4HDR_DF_SHIFT)
#define IPV4HDR_MF_MASK    (1<<IPV4HDR_MF_SHIFT)
#define IPV4HDR_FO_MASK     (1 << IPV4HDR_FO_SHIFT)


/* PPPoE codes */
#define CODE_PADI           0x09
#define CODE_PADO           0x07
#define CODE_PADR           0x19
#define CODE_PADS           0x65
#define CODE_PADT           0xA7

#endif // XSDEFINE

