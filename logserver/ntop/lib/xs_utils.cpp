#include "xs_utils.h"
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include "time.h"
#include"cjson.h"
#include"xs_compatibility.h"

#include<arpa/inet.h>

#include <iconv.h>


float xs_get_physical_temperature_by_sensors(u_int phy_id)
{
    static int not_suport1=0,not_suport2=0;
    if(0==phy_id){
        if(not_suport1)
            return 0;
    } else if(not_suport2)
        return 0;

    float ret=0.0;
    char buf[128]={0};
    snprintf(buf,sizeof(buf),"sensors |grep 'Physical id %u'|awk '{print $4}'",phy_id);
    FILE *pf=popen(buf,"r");
    if(NULL ==pf){
        return ret;
    }
    char tmp[8]={0};
    fgets(tmp,sizeof(tmp),pf);
    pclose(pf);
    if('+'==tmp[0]){
        char *p=&tmp[1];
        tmp[5]='\0';
        ret = atof(p);
    }
    if(ret<1 && 0==phy_id){
        memset(buf,0,sizeof(buf));
        snprintf(buf,sizeof(buf),"sensors |grep 'Core %u'|awk '{print $3}'",1);
        pf=popen(buf,"r");
        if(NULL ==pf){
            return ret;
        }
        memset(tmp,0,sizeof(tmp));
        fgets(tmp,sizeof(tmp),pf);
        pclose(pf);
        if('+'==tmp[0]){
            char *p=&tmp[1];
            tmp[5]='\0';
            ret = atof(p);
        } else
            not_suport1=1;
    } else if(ret<1 && phy_id)
        not_suport2=1;
    return ret;
}

char* xs_strnstr(char* s1, char* s2, int l1,int l2)
{
    if (!l2)
        return (char *)s1;
    while (l1 >= l2) {
        l1--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}


int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);
    if (cd==0)
        return -1;
    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen) == -1){
        iconv_close(cd);
        return -1;
    }
    iconv_close(cd);
    return 0;
}

int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
    return code_convert("utf-8","gbk",inbuf,inlen,outbuf,outlen);
}
int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("gbk","utf-8",inbuf,inlen,outbuf,outlen);
}

/*
   string to time_t
   时间格式 2009-3-24 0:00:08 或 2009-3-24
   */
int API_StringToTimeEX(char* strDateStr,time_t *timeData)
{
    char *pBeginPos = strDateStr;
    char *pPos = strstr(pBeginPos,"-");
    if(pPos == NULL)
    {
        printf("strDateStr[%s] err \n", strDateStr);
        return -1;
    }
    int iYear = atoi(pBeginPos);
    int iMonth = atoi(pPos + 1);
    pPos = strstr(pPos + 1,"-");
    if(pPos == NULL)
    {
        printf("strDateStr[%s] err \n", strDateStr);
        return -1;
    }
    int iDay = atoi(pPos + 1);
    int iHour=0;
    int iMin=0;
    int iSec=0;
    pPos = strstr(pPos + 1," ");
    //为了兼容有些没精确到时分秒的
    if(pPos != NULL)
    {
        iHour=atoi(pPos + 1);
        pPos = strstr(pPos + 1,":");
        if(pPos != NULL)
        {
            iMin=atoi(pPos + 1);
            pPos = strstr(pPos + 1,":");
            if(pPos != NULL)
            {
                iSec=atoi(pPos + 1);
            }
        }
    }
    struct tm sourcedate;
    bzero((void*)&sourcedate,sizeof(sourcedate));
    sourcedate.tm_sec = iSec;
    sourcedate.tm_min = iMin;
    sourcedate.tm_hour = iHour;
    sourcedate.tm_mday = iDay;
    sourcedate.tm_mon = iMonth - 1;
    sourcedate.tm_year = iYear - 1900;
    *timeData = mktime(&sourcedate);
    return 0;
}

char *GetTag(char *pTag)
{
    time_t t = time(NULL);
    struct tm *st = localtime(&t);
    st->tm_year +=1900;
    st->tm_mon +=1;
    sprintf(pTag,"%04d%02d%02d%02d%02d%02d",st->tm_year,st->tm_mon,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);
return pTag;
}

//int Xs_get_host_seed()
//{
//    static char hostname[128]={0};
//    gethostname(hostname,128);
//    int h;
//    char *p = hostname;

//    h = 520;
//    for (p = hostname; *p != 0; ++p)
//        h = h * 37 + *p;
//    return h;
//}


void xs_free_rule(void *p)
{
    PRULE pRule = (PRULE)p;
    if(pRule->szRule){
        free(pRule->szRule);
        pRule->szRule = NULL;
    }
    free(pRule);
}

ushort CheckSum(ushort *buff, int size)
{
    {
        unsigned long cksum = 0;
        while(size>1)
        {
            cksum += *buff++;
            size -= 2/*sizeof(ushort)*/;
        }
        // 是奇数
        if(size==1)
        {
            cksum +=*(u_char*)buff;
            //cksum += *(ushort*)buff;
        }
        // 将32位的chsum高16位和低16位相加，然后取反
        cksum = (cksum >> 16) + (cksum & 0xffff);
        cksum += (cksum >> 16);
        return (ushort)(~cksum);
    }
}

ushort computeTCPChecksum(u_char *ipHdr, u_char *tcpHdr){
    u_int32_t sum = 0;
    u_int16_t count = ipHdr[2] * 256 + ipHdr[3];
    u_int16_t tmp;

    unsigned char *addr = tcpHdr;
    unsigned char pseudoHeader[12];

    /* Count number of bytes in TCP header and data */
    count -= (ipHdr[0] & 0x0F) * 4;

    memcpy(pseudoHeader, ipHdr+12, 8);
    pseudoHeader[8] = 0;
    pseudoHeader[9] = ipHdr[9];
    pseudoHeader[10] = (count >> 8) & 0xFF;
    pseudoHeader[11] = (count & 0xFF);

    /* Checksum the pseudo-header */
    sum += * ((u_int16_t *) pseudoHeader);
    sum += * ((u_int16_t *) (pseudoHeader+2));
    sum += * ((u_int16_t *) (pseudoHeader+4));
    sum += * ((u_int16_t *) (pseudoHeader+6));
    sum += * ((u_int16_t *) (pseudoHeader+8));
    sum += * ((u_int16_t *) (pseudoHeader+10));

    /* Checksum the TCP header and data */
    while (count > 1) {
        memcpy(&tmp, addr, 2);
        sum += (u_int32_t) tmp;
        addr += 2;
        count -= 2;
    }
    if (count > 0) {
        sum += (unsigned char) *addr;
    }

    while(sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return (u_int16_t) ((~sum) & 0xFFFF);
}
