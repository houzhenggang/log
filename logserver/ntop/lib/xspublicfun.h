#ifndef __PUBLICFUNCTION_H__
#define __PUBLICFUNCTION_H__
#include <stdlib.h>
#include <string.h>
#include "xsbool.h"
#include <time.h>



#include <lib/xstydef.h>

#ifdef XSPEEDER_OBJ
#include <rte_mbuf.h>
#endif

#include <ctype.h>
/* Used to compare MAC addresses. */
#define MAC_ADDR_CMP 0xFFFFFFFFFFFFULL

//#define SAFE_DELETE(p)  { if(p) {free(p); (p) =NULL;}}
#define stricmp(a,b) strcasecmp(a,b)
#define strincmp(a,b,n) strncasecmp(a,b,n)

void PrintHex(char *buf,u_int16_t nLen);

int GetSystemBits();
void GetTag(char *pTag);
u_long GetTickCount();
u_long GetTickCount_Usec();


char* IpProto2Name(u_short proto_id);
u_int8_t GetNetMaskPrefix_by_netbit(u_int32_t mask);
u_int8_t GetNetMaskPrefix_by_str(char *pmask) ;
u_int32_t GetMaskByPrefix(u_int8_t pre);

bool StrMac2Uchar(char *pSrt,u_char *pMac);
char *EtherAddr2String(const u_char *ep, char *buf) ;
int EtherAddrCmp(u_char ea[6], u_char eb[6]);
int EtherAddrIsNull(u_char ea[6]);

int Bind2Core(u_int core_id);
int BindThread2Core(pthread_t thread_id, u_int core_id);


//bool GetIntFromString(const char *in,int *out,u_int limit);


/* ########################### XsNat ############################### */
#ifdef XSPEEDER_OBJ
int nodeport_cmp_Portmap_byWan(const void *a, const void *b);
#endif

int tmpportnode_cmp(const void *a, const void *b);



bool IsValidIP(const char *ipaddr);
bool IsValidNetmask(const char *netmask);

void addinto_trash(void *ptr,void *param);

int Xs_get_host_seed();

int Fill_Ipv4Hdr_Frag(struct xs_iphdr *dst,u_int16_t len,u_int16_t fofs,u_int16_t dofs,u_int16_t mf);

void Check_TTL_Cfg(PXSTTLCFG pTTL);

#endif
