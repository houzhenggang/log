#ifndef XSSHAREFUNC_H
#define XSSHAREFUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



int already_running(const char *filename);
int write_xs_pid_file(char *pid_file);

int MyMakeDir(const char *sPathName,int mode);
void GetLocalTime(struct tm *st);
void GetLocalTime2(struct tm *st,time_t t);
void RemoveLogFile(char *pFile);
void RenameLogFile(char *pOld,char *pNew);

int setnonblocking(int sockfd);
int Get_CPU_ModeName(char *pname,int buflen);
int CreateUDPSocket(u_int16_t port,u_int16_t sendtimeout,u_int16_t recvtimeout,u_int8_t local);
int CreateUDPSocket2(u_int16_t sendtimeout,u_int16_t recvtimeout);
int CreateUDPSocket3(u_int16_t port, u_int16_t sendtimeout, u_int16_t recvtimeout, int reuser, int sock_buff_len);
void microseconds_sleep(unsigned long uSec);

static inline void get_cpuid(unsigned int i, unsigned int * buf)
{
    unsigned int eax,ebx,ecx,edx;
    __asm__ (
    "cpuid"
    :"=a"(eax),"=b"(ebx),"=c"(ecx),"=d"(edx):"a"(i));
    buf[0]=eax;
    buf[1]=ebx;
    buf[2]=edx;
    buf[3]=ecx;

}

void get_control_netmac(char *mac);
void get_control_netmac2(char *mac);
u_int get_control_dev_ip();
u_int get_host_id();
int get_control_dev(char *pdev, int len);
//int main() {
//   unsigned int cpu[4];
//   get_cpuid(0,cpu);
//}

#endif // XSSHAREFUNC_H

