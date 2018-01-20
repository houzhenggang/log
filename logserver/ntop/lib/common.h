#ifndef COMMON_H
#define COMMON_H

#define SOCKET_BUFFER_LEN 4*1024*1024

typedef struct {
    char ifname[64];
    int ifID;
    u_int64_t hashkey;
}If_Msg,*pIf_Msg;
#define IF_MSG_SIZE  sizeof(If_Msg)
void Xs_Set_UDPSocket_Buffer(int sock);
void WriteLog(char *pFile,const char *str);
void parseMacAddress(u_int8_t dest[6], char *src);
void get_dateString(char *date,int size);
void get_dateString2(char *date,int size);
void XsGet_Format_Date(char *pDate, char *DataBuf);
void get_timeString(char *time,int size);
int ParseUIntNums(char *InBuf,int buflen,u_int dataArry[],u_int ArrySize,u_int MaxValue);
long GetTick(char *str_time);
char *  my_strlwr(char * str,char* pDest) ;
//void urldecode(char *p);
void urldecode(char url[]);
void urlencode(char url[]);

#endif // COMMON_H
