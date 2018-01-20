#include "xssharefunc.h"
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <zlib.h>
#include "xshash.h"

#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)



/* ******************************** */
/* set advisory lock on file */
int lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;  /* write lock */
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;  //lock the whole file

    return(fcntl(fd, F_SETLK, &fl));
}
int already_running(const char *filename)
{
    int fd;
    char buf[16]={0};

    fd = open(filename, O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0) {
        printf("can't open %s: %m\n", filename);
        exit(1);
    }

    /* 先获取文件锁 */
    if (lockfile(fd) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            printf("file: %s already locked\n", filename);
            close(fd);
            return 1;
        }
        printf("can't lock %s: %m\n", filename);
        exit(1);
    }
    /* 写入运行实例的pid */
    ftruncate(fd, 0);
    snprintf(buf,sizeof(buf)-1, "%ld\n", (long)getpid());
    write(fd, buf, strlen(buf) + 1);
    return 0;
}

int write_xs_pid_file(char *pid_file)
{
    int fd;
    char buf[16]={0};
    unlink (pid_file);
    fd = open(pid_file, O_RDWR | O_CREAT, 0640);
    if (fd < 0) {
        printf("can't open %s: %m\n", pid_file);
        return -1;
    }

    snprintf(buf,sizeof(buf)-1, "%ld\n", (long)getpid());
    int len = strlen(buf)+1;
    /* 写入运行实例的pid */
    ftruncate(fd, len);
    write(fd, buf, len);
    close(fd);
    return 0;
}


int Get_CPU_ModeName(char *pname,int buflen)
{
    char buffer[256]={0};
    FILE* fp = popen("cat /proc/cpuinfo |grep 'model name' ","r");
    if(NULL == fp)
        return -1;
    fgets(buffer, 255, fp);
    fclose(fp);
    char *p = strstr(buffer,"model name	: ");
    if(p){
        p += strlen("model name	: ");
        if(p){
            int len = strlen(p);
            if(len>1){
                if('\n'==p[len-1])
                    p[len-1]='\0';
            }
            len = len > buflen ? buflen : len;
            memcpy(pname,p,len);
            return 0;
        }
    }
    return -1;
}

int MyMakeDir(const char *sPathName,int mode)
{
    char DirName[256]={0};
    strcpy(DirName,sPathName);
    int i,len = strlen(DirName);
    if(DirName[len-1]!='/')
        strcat(DirName,   "/");
    len = strlen(DirName);
    for(i=1;i<len;i++)
    {
        if(DirName[i]=='/')
        {
            DirName[i]   =   0;
            if( access(DirName, F_OK)!=0 )
            {
                if(mkdir(DirName, mode /*0755*/)==-1)
                {
                    perror("mkdir   error");
                    return   -1;
                }
            }
            DirName[i] = '/';
        }
    }
    return   0;
}

void GetLocalTime(struct tm *st)
{
    time_t t = time(NULL);
    struct tm *pst = localtime(&t);
    memcpy(st,pst,sizeof(struct tm));
    st->tm_year += 1900;
    st->tm_mon+= 1;
}


void GetLocalTime2(struct tm *st,time_t t)
{
    struct tm *pst = localtime(&t);
    memcpy(st,pst,sizeof(struct tm));
    st->tm_year += 1900;
    st->tm_mon+= 1;
}

inline void microseconds_sleep(unsigned long uSec){
    usleep(uSec);
//    struct timeval tv;
//    tv.tv_sec=uSec/1000000;
//    tv.tv_usec=uSec%1000000;
//    int err;
//    do{
//        err=select(0,NULL,NULL,NULL,&tv);
//    }while(err<0 && errno==EINTR);
}

//void WriteLog(char *pFile,const char *str)
//{
//    char szFileName[256]={0};
//    //printf("log:%s\n",str);
//    snprintf(szFileName,255,"/var/log/ntopng/%s.log",pFile);
//    FILE *fp=fopen(szFileName,"a+");

//    if(NULL==fp)
//        return;
//    struct tm st;
//    GetLocalTime(&st);
//    fprintf(fp,"%04d-%02d-%02d %02d:%02d:%02d %s\n",st.tm_year,st.tm_mon,st.tm_mday,st.tm_hour,st.tm_min,st.tm_sec,str);
//    fclose(fp);
//}


void RemoveLogFile(char *pFile)
{
    char cmd[256]={0};
    //printf("log:%s\n",str);
    snprintf(cmd,sizeof(cmd),"/var/log/ntopng/%s.log",pFile);
    remove(cmd);
}

void RenameLogFile(char *pOld,char *pNew)
{
    char olefile[256]={0};
    //printf("log:%s\n",str);
    snprintf(olefile,sizeof(olefile),"/var/log/ntopng/%s.log",pOld);
    char newfile[256]={0};
    snprintf(newfile,sizeof(newfile),"/var/log/ntopng/%s.log",pNew);
    rename(olefile,newfile);
}



int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}


int CreateUDPSocket(u_int16_t port,u_int16_t sendtimeout,u_int16_t recvtimeout,u_int8_t local)
{
    struct sockaddr_in sin={0};
    sin.sin_family=AF_INET;

    if(local) {
        sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    } else {
        sin.sin_addr.s_addr=htonl(INADDR_ANY);
    }

    sin.sin_port=htons(port);
    socklen_t sin_len=sizeof(sin);

    int nSocket=socket(AF_INET,SOCK_DGRAM,0);
    if(-1==nSocket)
        return -1;
    if(-1 == bind(nSocket,(struct sockaddr *)&sin,sin_len)) {
        close(nSocket);
        return -1;
    }

    if(sendtimeout) {
        struct timeval tv;
        tv.tv_sec = sendtimeout;
        tv.tv_usec=0;
        setsockopt(nSocket,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv));
    }

    if(recvtimeout) {
        struct timeval tv;
        tv.tv_sec = recvtimeout;
        tv.tv_usec=0;
        setsockopt(nSocket,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
    }

    return nSocket;
}

int CreateUDPSocket2(u_int16_t sendtimeout,u_int16_t recvtimeout)
{
    int nSocket=socket(AF_INET,SOCK_DGRAM,0);
    if(-1==nSocket)
        return -1;

    if(sendtimeout) {
        struct timeval tv;
        tv.tv_sec = sendtimeout;
        tv.tv_usec=0;
        setsockopt(nSocket,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv));
    }

    if(recvtimeout) {
        struct timeval tv;
        tv.tv_sec = recvtimeout;
        tv.tv_usec=0;
        setsockopt(nSocket,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
    }
    return nSocket;
}

int CreateUDPSocket3(u_int16_t port, u_int16_t sendtimeout, u_int16_t recvtimeout, int reuser,int sock_buff_len)
{
    struct sockaddr_in sin={0};

    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=htonl(INADDR_ANY);
    sin.sin_port=htons(port);
    socklen_t sin_len=sizeof(sin);

    int nSocket=socket(AF_INET,SOCK_DGRAM,0);
    if(-1==nSocket)
        return -1;

    if(sock_buff_len>0)
        setsockopt(nSocket,SOL_SOCKET,SO_RCVBUF,(void*)&sock_buff_len,sizeof(sock_buff_len));

    if(sendtimeout) {
        struct timeval tv;
        tv.tv_sec = sendtimeout;
        tv.tv_usec=0;
        setsockopt(nSocket,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv));
    }

    if(recvtimeout) {
        struct timeval tv;
        tv.tv_sec = recvtimeout;
        tv.tv_usec=0;
        setsockopt(nSocket,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
    }

    if(1==reuser)
        setsockopt(nSocket,SOL_SOCKET,SO_REUSEADDR,(const void *)&reuser,sizeof(reuser));

    if(-1 == bind(nSocket,(struct sockaddr *)&sin,sin_len)) {
        close(nSocket);
        return -1;
    }


    return nSocket;
}

int get_control_dev(char *pdev,int len)
{
    char pbuf[16]={0};
    FILE *pfd = fopen("/opt/systool/install.lock","r");
    if (pfd) {
        char *p = fgets(pbuf,16,pfd);
        fclose (pfd);
        if(p){
            int t = strlen(p);
            t = t>len ? len : t;
            memcpy(pdev,p,t);
            return 0;
        }
    }
    return -1;
}

void get_control_netmac(char *mac)
{
    char pbuf[16]={0};
    FILE *pfd = fopen("/opt/systool/install.lock","r");
    if (pfd) {
        char *p = fgets(pbuf,16,pfd);
        fclose (pfd);
        if(!p)
            return ;
        struct ifreq ifreq;
        int sock= socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0)  {
            return ;
        }
        int len = strlen(pbuf);
        if(len>2){
            char *p = strstr(pbuf,"\n");
            if(p){
                *p='\0';
            }
        }
        strcpy(ifreq.ifr_name,pbuf);
        if(ioctl(sock,SIOCGIFHWADDR,&ifreq) < 0){
            close(sock);
            return ;
        }
        close(sock);
        u_char sa_data[14]={0};
        memcpy(sa_data,ifreq.ifr_hwaddr.sa_data,14);
        snprintf(mac, 20, "%02x:%02x:%02x:%02x:%02x:%02x",
                 sa_data[0],
                sa_data[1],
                sa_data[2],
                sa_data[3],
                sa_data[4],
                sa_data[5]);
    }
}

void get_control_netmac2(char *mac)
{
    char pbuf[16]={0};
    FILE *pfd = fopen("/opt/systool/install.lock","r");
    if (pfd) {
        char *p = fgets(pbuf,16,pfd);
        fclose (pfd);
        if(!p)
            return ;
        struct ifreq ifreq;
        int sock= socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0)  {
            return ;
        }
        int len = strlen(pbuf);
        if(len>2){
            char *p = strstr(pbuf,"\n");
            if(p){
                *p='\0';
            }
        }
        strcpy(ifreq.ifr_name,pbuf);
        if(ioctl(sock,SIOCGIFHWADDR,&ifreq) < 0){
            close(sock);
            return ;
        }
        close(sock);
        u_char sa_data[14]={0};
        memcpy(sa_data,ifreq.ifr_hwaddr.sa_data,14);
        int i;
        for(i = 0; i < 6; ++i) {
            sprintf(mac + 2*i, "%02x", sa_data[i]);
        }
    }
}

u_int get_control_dev_ip()
{
    char pbuf[16]={0};
    FILE *pfd = fopen("/opt/systool/install.lock","r");
    if (pfd) {
        char *p = fgets(pbuf,16,pfd);
        fclose (pfd);
        if(!p)
            return 0;
        struct ifreq ifreq;
        int sock= socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0)  {
            return 0;
        }
        int len = strlen(pbuf);
        if(len>2){
            char *p = strstr(pbuf,"\n");
            if(p){
                *p='\0';
            }
        }
        strcpy(ifreq.ifr_name,pbuf);
        if(ioctl(sock,SIOCGIFADDR,&ifreq) < 0){
            close(sock);
            return 0;
        }
        close(sock);
        u_int ip=0;
        memcpy(&ip,ifreq.ifr_addr.sa_data+2,sizeof(u_int));
        return ip;
    }
    return 0;
}

u_int get_host_id()
{
    unsigned int cpu[4]={0};
    get_cpuid(0,cpu);
    u_char str_mac[20]={0};
    get_control_netmac((char*)str_mac);
    char tmp[64]={0};
    snprintf(tmp,64,"%u%u%u%u%s",cpu[0],cpu[1],cpu[2],cpu[3],str_mac);
    int len = strlen (tmp);
    u_int hash =  BKDRHash(tmp,len);//(u_int)crc32(0,(const u_char*)tmp,len);
    return hash;
}

