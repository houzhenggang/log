#include"ntop_includes.h"
#include"ndpi_includes.h"
#include"xsmysql/mydbop.h"
#include"common.h"


void Xs_Set_UDPSocket_Buffer(int sock)
{
    long long sockbufflen = SOCKET_BUFFER_LEN;
    long long sockopt = 0;
    setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void *)&sockbufflen,sizeof(sockbufflen));
    setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void *)&sockbufflen,sizeof(sockbufflen));
    setsockopt(sock,SOL_SOCKET,SO_NO_CHECK,(void *)&sockopt,sizeof(sockopt));
    return ;
}

void parseMacAddress(u_int8_t dest[6],char *src){
    unsigned int a,b,c,d,e,f;
    if(!strlen(src))
        return;
    if((sscanf(src,"%x:%x:%x:%x:%x:%x",&a,&b,&c,&d,&e,&f)==6)&&
            a<256 &&b<256&&c<256&&d<256&&e<256&&f<256){
        dest[0]=(unsigned char )a;
        dest[1]=(unsigned char )b;
        dest[2]=(unsigned char )c;
        dest[3]=(unsigned char )d;
        dest[4]=(unsigned char )e;
        dest[5]=(unsigned char )f;
    }
    return ;
}

//void urldecode(char *p)
//{
//int  i=0;
//while(*(p+i))
//{
//   if ((*p=*(p+i)) == '%')
//   {
//    *p=*(p+i+1) >= 'A' ? ((*(p+i+1) & 0XDF) - 'A') + 10 : (*(p+i+1) - '0');
//    *p=(*p) * 16;
//    *p+=*(p+i+2) >= 'A' ? ((*(p+i+2) & 0XDF) - 'A') + 10 : (*(p+i+2) - '0');
//    i+=2;
//   }
//   else if (*(p+i)=='+')
//   {
//    *p=' ';
//   }
//   p++;
//}
//*p='\0';
//}



char *  my_strlwr(char * str,char* pDest)     //定义一个my_strlwr函数
{
    assert(str);                  //str的非空性
    char *ret = str;              //定义一个ret保存最初的str
    while(*ret != '\0')           //判断字符串是否结束
    {
           if((*ret >= 'A')&&(*ret <= 'Z'))//判断当前啊字符是否是                                                大写字母
        {
            *ret = *ret +32;          //将其转化为小写字母
            ret++;
        }
        else
            ret++;
    }
    strcpy(pDest,str);
    return str;             //返回该字符串数组的首地址
}


void WriteLog(char *pFile,const char *str)
{
    char szFileName[256]={0};
    //printf("log:%s\n",str);
    snprintf(szFileName,255,"/var/log/sxzlog/%s.log",pFile);
    FILE *fp=fopen(szFileName,"a+");
    if(NULL==fp) return ;

    struct tm st;
    GetLocalTime(&st);
    fprintf(fp,"%04d-%02d-%02d %02d:%02d:%02d %s\n",st.tm_year,st.tm_mon,st.tm_mday,st.tm_hour,st.tm_min,st.tm_sec,str);
    fclose(fp);
}

void get_dateString(char *date,int size)
{
    struct tm st;
    GetLocalTime(&st);
    snprintf(date,size,"%04d%02d%02d",st.tm_year,st.tm_mon,st.tm_mday);
}

void get_dateString2(char *date,int size)
{
    struct tm st;
    GetLocalTime(&st);
    snprintf(date,size,"%04d-%02d-%02d",st.tm_year,st.tm_mon,st.tm_mday);
}


void XsGet_Format_Date(char *pDate,char *DataBuf){
    char year[5]={0},month[3]={0},day[3]={0};
    if (sscanf(pDate,"%4s%2s%2s",year,month,day) == 3) {
        snprintf(DataBuf,15,"%s-%s-%s",year,month,day);
    }
}
/* ************************************************************** */

void get_timeString(char *time,int size){
    struct tm st;
    GetLocalTime(&st);
    snprintf(time,size,"%02d:%02d:%02d ",st.tm_hour,st.tm_min,st.tm_sec);
}

long GetTick(char *str_time)
{
    struct tm stm;
    int iY, iM, iD, iH, iMin, iS;

    memset(&stm,0,sizeof(stm));

    iY = atoi(str_time);
    iM = atoi(str_time+5);
    iD = atoi(str_time+8);
    iH = atoi(str_time+11);
    iMin = atoi(str_time+14);
    iS = atoi(str_time+17);

    stm.tm_year=iY-1900;
    stm.tm_mon=iM-1;
    stm.tm_mday=iD;
    stm.tm_hour=iH;
    stm.tm_min=iMin;
    stm.tm_sec=iS;

    /*printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);*/

    return mktime(&stm);
}


int ParseUIntNums(char *InBuf,int buflen,u_int dataArry[],u_int ArrySize,u_int MaxValue)
{
    int nID=0;

    if(InBuf[0]==0)
        return 0;
    u_int n=0;
    char *p1=NULL,*p2=NULL;
    p2=InBuf;
    p1=InBuf;//strstr(p2,",");
    int nlen=0;
    while(p1)
    {
        char szCon[32]={0};
        char szStart[16]={0};
        char szEnd[16]={0};
        p1=strstr(p2,",");
        if(p1) {
            nlen=p1-p2;
            if(nlen>18)
                return -1;
            strncpy(szCon,p2,nlen);
        } else {
            p1 = strstr(p2,"-");
            if(!p1) {
                nlen=strlen(p2);
                if(nlen>32) {
                    printf("config error!\n");
                    return -1;
                } else if(0==nlen) {
                    return n;
                }
                strcpy(szCon,p2);
            } else {
                nlen = strlen(p2);
                if(nlen>32) {
                    printf("config error!\n");
                    return -1;
                }
                strncpy(szCon,p2,nlen);
            }
        }
        p2=p1+1;
        char *p=strstr(szCon,"-");
        if(p) {
            strncpy(szStart,szCon,p-szCon);
            strcpy(szEnd,p+1);
            u_int start=atoi(szStart);
            u_int end=atoi(szEnd);
            if(start>end||end<=0) {
                printf("config error!\n");
                continue;
            }
            if(MaxValue && (start>MaxValue|| end>MaxValue)) {
                continue;
            }
            for(start;start<=end;start++) {
                if(n>=ArrySize)
                    return n;
                dataArry[n++]=start;
            }
        }else{
            nID=atoi(szCon);
            if(nID<0)
                continue;
            if(MaxValue && nID>MaxValue)
                continue;
            if(n>=ArrySize)
                return n;
            dataArry[n++]=nID;
        }
    }

    return n;
}
//
#define BURSIZE 2048

int hex2dec(char c)
{
    if ('0' <= c && c <= '9')
    {
        return c - '0';
    }
    else if ('a' <= c && c <= 'f')
    {
        return c - 'a' + 10;
    }
    else if ('A' <= c && c <= 'F')
    {
        return c - 'A' + 10;
    }
    else
    {
        return -1;
    }
}

char dec2hex(short int c)
{
    if (0 <= c && c <= 9)
    {
        return c + '0';
    }
    else if (10 <= c && c <= 15)
    {
        return c + 'A' - 10;
    }
    else
    {
        return -1;
    }
}


//编码一个url
void urlencode(char url[])
{
    int i = 0;
    int len = strlen(url);
    int res_len = 0;
    char res[BURSIZE];
    for (i = 0; i < len; ++i)
    {
        char c = url[i];
        if (    ('0' <= c && c <= '9') ||
                ('a' <= c && c <= 'z') ||
                ('A' <= c && c <= 'Z') ||
                c == '/' || c == '.')
        {
            res[res_len++] = c;
        }
        else
        {
            int j = (short int)c;
            if (j < 0)
                j += 256;
            int i1, i0;
            i1 = j / 16;
            i0 = j - i1 * 16;
            res[res_len++] = '%';
            res[res_len++] = dec2hex(i1);
            res[res_len++] = dec2hex(i0);
        }
    }
    res[res_len] = '\0';
    strcpy(url, res);
}

// 解码url
void urldecode(char url[])
{
    int i = 0;
    int len = strlen(url);
    int res_len = 0;
    char res[BURSIZE];
    for (i = 0; i < len; ++i)
    {
        char c = url[i];
        if (c != '%')
        {
            res[res_len++] = c;
        }
        else
        {
            char c1 = url[++i];
            char c0 = url[++i];
            int num = 0;
            num = hex2dec(c1) * 16 + hex2dec(c0);
            res[res_len++] = num;
        }
    }
    res[res_len] = '\0';
    strcpy(url, res);
}
