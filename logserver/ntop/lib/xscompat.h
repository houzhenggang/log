/*
 * xscompat.h
 *
 * Copyright (C) 2015-2020  liucongpei
 * QQ:121518214
 *
 *
 */



#ifndef __XS_COMPAT_H__
#define __XS_COMPAT_H__
#include <stdlib.h>
#ifdef __linux__
#include <linux/if_ether.h>
#endif
#ifdef WIN32
#define __LITTLE_ENDIAN__ 1
#endif

#if !(defined(__LITTLE_ENDIAN__) || defined(__BIG_ENDIAN__))
/* Kernel modules */
#if defined(__LITTLE_ENDIAN)
#define __LITTLE_ENDIAN__
#endif
#if defined(__BIG_ENDIAN)
#define __BIG_ENDIAN__
#endif
/* Everything else */
#if (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__))
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#else
#define __BIG_ENDIAN__
#endif
#endif

#endif

#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
#include <machine/endian.h>

#if _BYTE_ORDER == _LITTLE_ENDIAN
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#else
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__ 1
#endif
#endif
#endif

#ifndef ETH_ALEN
#define ETH_ALEN    6
#endif
//#pragma pack(push, 1)  /* push current alignment to stack */
//#pragma pack(1)     /* set alignment to 1 byte boundary */

//#pragma pack(pop)   /* restore original alignment from stack */
#pragma pack(1)

struct xs_ethhdr {
    u_char h_dest[ETH_ALEN];       /* destination eth addr */
    u_char h_source[ETH_ALEN];     /* source ether addr    */
    //u_int64_t n_source:6;
    u_int16_t h_proto;      /* packet type ID field */
};
#define XSETHHDR_SIZE       14// sizeof(struct xs_ethhdr)

struct xs_vlanhdr {
    u_int16_t vlan_tci;       /*< Priority (3) + CFI (1) + Identifier Code (12) */
    u_int16_t eth_proto;    /*< Ethernet type of encapsulated frame. */
};
#define XSVLANHDR_SIZE     4// sizeof(struct xs_vlanhdr)

#define IP_CE       0x8000  /* Flag: "Congestion" */
#define IP_DF       0x4000  /* Flag: "Don`t Fragment" */
#define IP_MF       0x2000 /* Flag: "More Fragments"  */
#define IP_OFFMASK   0x1FFF  /* "Fragment Offset" part */

struct xs_iphdr {
#if defined(__LITTLE_ENDIAN__)
    u_int8_t ihl:4, version:4;
#elif defined(__BIG_ENDIAN__)
    u_int8_t version:4, ihl:4;
#else
# error "Byte order must be defined"
#endif
    u_int8_t tos;
    u_int16_t tot_len;
    u_int16_t id;
    u_int16_t frag_off;
    u_int8_t ttl;
    u_int8_t protocol;
    u_int16_t check;
    u_int32_t saddr;
    u_int32_t daddr;
};
#define XSIPv4HDR_SIZE        20// sizeof(struct xs_iphdr)


struct xs_ip6_addr {
    union {
        u_int8_t u6_addr8[16];
        u_int16_t u6_addr16[8];
        u_int32_t u6_addr32[4];
        u_int64_t u6_addr64[2];
    } xs_v6_u;

#define xs_v6_addr		xs_v6_u.u6_addr8
#define xs_v6_addr16		xs_v6_u.u6_addr16
#define xs_v6_addr32		xs_v6_u.u6_addr32
#define xs_v6_addr64		xs_v6_u.u6_addr64
};

struct xs_ipv6hdr {
    /* use userspace and kernelspace compatible compile parameters */
#if defined(__LITTLE_ENDIAN__)
    u_int8_t priority:4, version:4;
#elif defined(__BIG_ENDIAN__)
    u_int8_t version:4, priority:4;
#else
# error "Byte order must be defined"
#endif

    u_int8_t flow_lbl[3];

    u_int16_t payload_len;
    u_int8_t nexthdr;
    u_int8_t hop_limit;

    struct xs_ip6_addr saddr;
    struct xs_ip6_addr daddr;
};
#define XSIPv6HDR_SIZE  sizeof(struct xs_ipv6hdr)

#ifdef WIN32

typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned int   uint;
typedef unsigned long  u_long;
typedef u_char  u_int8_t;
typedef u_short u_int16_t;
typedef uint   u_int32_t;

#define _WS2TCPIP_H_ /* Avoid compilation problems */
#define HAVE_SIN6_LEN


/* IPv6 address */
/* Already defined in WS2tcpip.h */
struct xs_win_in6_addr
{
    union
    {
        u_int8_t u6_addr8[16];
        u_int16_t u6_addr16[8];
        u_int32_t u6_addr32[4];
    } in6_u;
#ifdef s6_addr
#undef s6_addr
#endif

#ifdef s6_addr16
#undef s6_addr16
#endif

#ifdef s6_addr32
#undef s6_addr32
#endif

#define s6_addr                 in6_u.u6_addr8
    // #define s6_addr16               in6_u.u6_addr16
    // #define s6_addr32               in6_u.u6_addr32

};

#define in6_addr win_in6_addr

/* Generic extension header.  */
struct xs_ip6_ext
{
    u_int8_t  ip6e_nxt;		/* next header.  */
    u_int8_t  ip6e_len;		/* length in units of 8 octets.  */
};

#define s6_addr16		__u6_addr.__u6_addr16
#define s6_addr32		__u6_addr.__u6_addr32
#else
#ifndef __KERNEL__
#include <arpa/inet.h>
#endif
#endif


struct xs_tcphdr {
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
#if defined(__LITTLE_ENDIAN__)
    union {
        u_int16_t len_flags;
        struct {
            u_int8_t lenres;
            u_int8_t flags;
        };
        struct {
            u_int16_t res1:4, doff:4, fin:1, syn:1, rst:1, psh:1, ack:1, urg:1, ece:1, cwr:1;
        };
    };
#elif defined(__BIG_ENDIAN__)
    union {
        u_int16_t len_flags;
        struct {
            u_int8_t flag;
            u_int8_t lenres;
        };
        struct {
            u_int16_t doff:4, res1:4, cwr:1, ece:1, urg:1, ack:1, psh:1, rst:1, syn:1, fin:1;
        };
    };
#else
# error "Byte order must be defined"
#endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
};
#define XSTCPHDR_SIZE       20//sizeof(struct xs_tcphdr)

struct xs_udphdr {
    u_int16_t source;
    u_int16_t dest;
    u_int16_t len;
    u_int16_t check;
};
#define XSUDPHDR_SIZE       8// sizeof(struct xs_udphdr)

struct _Psd{
    u_int32_t saddr;
    u_int32_t daddr;
    char mbz;
    char ptcl;
    u_int16_t tcpudpl;
};
#define XSPSD_SIZE       12// sizeof(struct _Psd)

struct xs_grehdr {
#define PPTP_GRE_PROTO  0x880B
#define PPTP_GRE_PROTO_NETBIT   0xB88
#define PPTP_GRE_VER    0x1

#define PPTP_GRE_FLAG_C	0x80
#define PPTP_GRE_FLAG_R	0x40
#define PPTP_GRE_FLAG_K	0x20
#define PPTP_GRE_FLAG_S	0x10
#define PPTP_GRE_FLAG_A	0x80

#define PPTP_GRE_IS_C(f) ((f)&PPTP_GRE_FLAG_C)
#define PPTP_GRE_IS_R(f) ((f)&PPTP_GRE_FLAG_R)
#define PPTP_GRE_IS_K(f) ((f)&PPTP_GRE_FLAG_K)
#define PPTP_GRE_IS_S(f) ((f)&PPTP_GRE_FLAG_S)
#define PPTP_GRE_IS_A(f) ((f)&PPTP_GRE_FLAG_A)

    u_int8_t flags;		/* bitfield */
    u_int8_t ver;			/* should be PPTP_GRE_VER (enhanced GRE) */
    u_int16_t protocol;		/* should be PPTP_GRE_PROTO (ppp-encaps) */
    u_int16_t payload_len;	/* size of ppp payload, not inc. gre header */
    u_int16_t call_id;		/* peer's call_id for this session */
    u_int32_t seq;		/* sequence number.  Present if S==1 */
    u_int32_t ack;		/* seq number of highest packet recieved by */
    /*  sender in this session */
};
#define XSGREHDR_SIZE sizeof(struct xs_grehdr)

struct xs_ppphdr
{
    u_char  addr;
    u_char  ctrl;
    u_int16_t proto;
};
#define XSPPPHDR_SIZE     4//sizeof(struct xs_ppphdr)

struct xs_ppphdr2
{
    u_char proto;
};
#define XSPPPHDR2_SIZE     1//sizeof(struct xs_ppphdr2)
//struct xs_ppphdr
//{
//    u_char  code;
//    u_char  identifier;
//    u_int16_t len;
//};
//#define XSPPPHDR_SIZE     sizeof(struct xs_ppphdr)

typedef struct xs_pppoe_tag// struct xs_pppoe_tag
{
    u_int16_t type;/* tag type */
    u_int16_t len;	/* Length of payload */
}XSPPPoETag;
#define XS_TAG_HDR_SIZE     4
//#define XSPPPOETAG_SIZE       sizeof(struct xs_pppoe_tag)

struct xs_pppoe_disc_hdr
{
    union{
        u_char h_verlen;
#if defined(__LITTLE_ENDIAN__)
        struct {
            u_int8_t ver:4;
            u_int8_t type:4;
        };
#elif defined(__BIG_ENDIAN__)
        struct {
            u_int8_t type:4;
            u_int8_t ver:4;
        };
#else
# error "Byte order must be defined"
#endif
    };
    u_int16_t code:8;	/* PPPoE code */
    u_int16_t sessionid:16;	/* PPPoE session */
    u_int16_t length:16;	/* Payload length */
};
#define XSPPPOEDISCHDR_SIZE   sizeof(struct xs_pppoe_disc_hdr)

struct xs_pppoe_ses_hdr
{
    union{
        u_char h_verlen;
#if defined(__LITTLE_ENDIAN__)
        struct {
            u_int8_t ver:4;
            u_int8_t type:4;
        };
#elif defined(__BIG_ENDIAN__)
        struct {
            u_int8_t type:4;
            u_int8_t ver:4;
        };
#else
# error "Byte order must be defined"
#endif
    };
    u_int16_t code:8;	/* PPPoE code */
    u_int16_t sessionid:16;	/* PPPoE session */
    u_int16_t length:16;	/* Payload length */
    u_int16_t proto:16;
};
#define XSPPPOESESSIONHDR_SIZE      8//sizeof(struct xs_pppoe_ses_hdr)


typedef struct xs_pppoe_option//struct OPTION
{
    u_char type;
    u_char len;
}XSOPTION_HDR;
#define XSPPPOEOPTION_HDR_SIZE       2;//sizeof(struct xs_pppoe_option)

struct xs_arphdr
{
    //硬件类型 - 以太网类型值0x1
    u_int16_t hw_type;
    //上层协议类型 - IP协议(0x0800)
    u_int16_t proto_type;
    //MAC地址长度
    u_char mac_addr_len;
    //IP地址长度
    u_char ip_addr_len;
    //操作码 - 0x1表示ARP请求包,0x2表示应答包
    u_int16_t operation_code;
    //发送方mac
    u_char mac_sender[ETH_ALEN];
    //发送方ip
    u_int32_t ip_sender;
    //接收方mac
    u_char mac_receiver[ETH_ALEN];
    //接收方ip
    u_int32_t ip_receiver;
    //填充数据
    u_char padding[18];
};
#define XSARPHDR_SIZE       46//sizeof(struct xs_arphdr)

//定义ICMP首部

struct xs_icmphdr {
    union {
        struct {
            u_int8_t		type;
            u_int8_t		code;
        };
        u_int16_t type_and_code;
    };
    u_int16_t	checksum;
    union {
        struct {
            u_int16_t	id;
            u_int16_t	sequence;
        } echo;
        union {
            u_int32_t    i_id;
            u_int16_t	gateway;
        };
        struct {
            u_int16_t	__unused;
            u_int16_t	mtu;
        } frag;
    } un;
} ;
#define XSICMPHDR_SIZE       8//sizeof(struct xs_icmphdr)


#pragma pack()
#endif
