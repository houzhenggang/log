#ifndef XS_LOG_H
#define XS_LOG_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define strincmp(a,b,n) strncasecmp(a,b,n)
int API_StringToTimeEX(char* strDateStr,time_t *timeData);
char *GetTag(char *pTag);
int Xs_get_host_seed();

void xs_free_rule(void *p);
float xs_get_physical_temperature_by_sensors(u_int phy_id);
char* xs_strnstr(char* s1, char* s2, int l1,int l2);

int u2g(char *inbuf,int inlen,char *outbuf,int outlen);
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen);


ushort CheckSum(ushort *buff, int size);
ushort computeTCPChecksum(u_char *ipHdr, u_char *tcpHdr);

#endif // XS_LOG_H

