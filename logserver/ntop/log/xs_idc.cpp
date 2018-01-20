#include"ntop_includes.h"
#include<malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
extern int h_errno;
#include "lib/xs_def.h"
#include <lib/xs_utils.h>
#include "lib/xsinifile.h"
#include "xsmysql/mydbop.h"
#include "lib/xs_crypt.h"
#include"lib/cjson.h"
#include"lib/rbset.h"
#include"lib/rbtree.h"
#include "lib/mygethostbyname.h"
#include <sys/sysinfo.h>
#include "crypt.h"
#include "errno.h"
#include <zlib.h>
#include "lib/key_elem.h"
#include"lib/xswebaction.h"

#define DEFALUT_PORTS   32
u_int8_t cpu0_templature=0;
u_int8_t cpu1_templature=0;

char g_hostname[33];

u_int32_t g_ctl_ip=0;
char g_out_dev[16]={0};

char * create_objects_protocol(PXSIDC pIdc)
{
    cJSON *root ;
    char *out;
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"cmd",		CMD_PROTO	);
    cJSON_AddStringToObject(root,"serial",		pIdc->m_userinfo.szSerial);
    out=cJSON_Print(root);
    cJSON_Delete(root);
    if(ntop->g_xsDebug)
        printf("%s\n",out );
    return out ;
}

char * create_objects_verify(PXSIDC pIdc )
{
    struct sysinfo sys={0};
    sysinfo(&sys);
    u_long xspeederttl = Xs_Get_Program_Time_To_Live(XSPEEDER_PID);
    cJSON *root ;
    char *out;
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"cmd",		CMD_VERIFY	);
    cJSON_AddStringToObject(root,"serial",		pIdc->m_userinfo.szSerial);
    cJSON_AddNumberToObject(root,"hostid",		ntop->g_HostID);
    cJSON_AddNumberToObject(root,"curonlines",pIdc->ctl_curonlines);
    cJSON_AddNumberToObject(root,"curbursts",	pIdc->ctl_curbursts);
    cJSON_AddNumberToObject(root,"maxonlines",	pIdc->ctl_maxonlines);
    cJSON_AddNumberToObject(root,"maxbursts",	pIdc->ctl_maxbursts);
    cJSON_AddStringToObject(root,"ver",pIdc->xspeeder_ver);
    cJSON_AddStringToObject(root,"libver",pIdc->rule_ver);
    cJSON_AddStringToObject(root,"cpu",pIdc->cpu_name);
    cJSON_AddStringToObject(root,"mac",ntop->g_mac);
    cJSON_AddNumberToObject(root,"procs",	pIdc->procs);
    cJSON_AddNumberToObject(root,"totalram",	sys.totalram);
    cJSON_AddNumberToObject(root,"freeram",	sys.freeram);
    cJSON_AddNumberToObject(root,"rxbps",	pIdc->rx_bps);
    cJSON_AddNumberToObject(root,"txbps",	pIdc->tx_bps);
    cJSON_AddNumberToObject(root,"xspeederttl",	xspeederttl);
    cJSON_AddNumberToObject(root,"usefulwan",	pIdc->usefulwan);
    float c0=xs_get_physical_temperature_by_sensors(0);
    if(c0>0){
        cpu0_templature = c0;
        cJSON_AddNumberToObject(root,"cpu0_temp",c0);
        float c1=xs_get_physical_temperature_by_sensors(1);
        if(c1>0){
            cpu1_templature = c1;
            cJSON_AddNumberToObject(root,"cpu1_temp",c1);
        }
    }
    out=cJSON_Print(root);
    cJSON_Delete(root);
    if(ntop->g_xsDebug)
        printf("%s\n",out );
    return out ;
}

#ifndef IPv4_BYTES
#define IPv4_BYTES_FMT "%u.%u.%u.%u"
#define IPv4_BYTES(addr) \
    (uint8_t) (((addr) ) & 0xFF),\
    (uint8_t) (((addr) >> 8) & 0xFF),\
    (uint8_t) (((addr) >> 16) & 0xFF),\
    (uint8_t) (((addr) >> 24) & 0xFF)
#endif

int GetHostIP(char * host,char *pIP)
{
    struct hostent *remoteHost;
    struct in_addr addr;
    remoteHost = gethostbyname(host);
    if (NULL==remoteHost )  {
        u_int32_t query[8]={0};
        u_int32_t dns = inet_addr("114.114.114.114");
        char ifname[16]={0};
        if('\0'==g_out_dev[0]){
            get_netdev(ifname,sizeof(ifname),0);
        }
        memcpy(ifname,g_out_dev,sizeof(ifname));
        u_int total = my_gethostbyname(host,dns,query,8,ifname);
        if(!total) {
            dns = inet_addr("8.8.8.8");
            total = my_gethostbyname(host,dns,query,8,ifname);
        }
        if(total){\
            addr.s_addr = query[0];
            strcpy(pIP,inet_ntoa(addr));
            static int flag=10;
            if(flag>0){
                flag--;
                char log[128]={0};
                snprintf(log,128,"IP:%s   Server:"IPv4_BYTES_FMT,pIP,IPv4_BYTES(dns));
                WriteLog("xsverify",log);
                return 0;
            }
        } else
            return -1;
    } else {
        addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
        strcpy(pIP,inet_ntoa(addr));
    }
    return  0;
}

int Get_idc_ip(PXSIDC pIdc)
{
#ifdef _DEBUG
    memcpy(pIdc->m_idc_ip,"172.17.1.223",strlen("172.17.1.223"));
    return 0;//test
#else
    int flag=0;
    if(pIdc->szHost[0]==0) {
        char path[256]={0};
        sprintf(path,"%s/idc_config.ini",ntop->g_path);
        read_profile_string("host","IDC",pIdc->szHost,sizeof(pIdc->szHost),"www.netsxz.com",path);
        if(ntop->g_xsDebug)
            printf("IDC Host:%s\n",pIdc->szHost);
    }
    memset(pIdc->m_idc_ip,0,sizeof(pIdc->m_idc_ip));
    if(!GetHostIP(pIdc->szHost,pIdc->m_idc_ip)) {             //test
        if(0 != GetHostIP("www.netsxz.com",pIdc->m_idc_ip)) {
            if(ntop->g_xsDebug)
                printf("域名解析失败\n");
            WriteLog("xsverify","parse DNS failed");
            flag=-1;
        }
    }
    if('\0'==pIdc->m_idc_ip[0] || -1==flag) {
        //memcpy(pIdc->m_idc_ip,m_idc_ip,20);
        memset(pIdc->m_idc_ip,0,sizeof(pIdc->m_idc_ip));
        memcpy(pIdc->m_idc_ip,"115.29.209.216",strlen("115.29.209.216"));
        if(ntop->g_xsDebug)
            printf("set idc IP:%s\n",pIdc->m_idc_ip);
        flag=0;
    }
    return flag;
#endif
}

//int Verify(PXSIDC pIdc)
//{
//    if( 0 !=Get_idc_ip(pIdc)) {
//        return -1;
//    }

//    static int print=0;
//    if(0 != Connect_serv(pIdc)) {
//        print=1;
//        char szLog[512]={0};
//        sprintf(szLog,"Verify 连接服务器失败：%s:%d",pIdc->m_idc_ip,ntop->g_port);
//        printf("%s\n",szLog);
//        WriteLog("xsverify",szLog);
//        strcpy(pIdc->m_userinfo.szDesc,"连接服务器失败");
//        pIdc->m_userinfo.nVerifystatus  = 99;
//        return -1;
//    } else if(1==print) {
//        print=0;
//        char szLog[512]={0};
//        sprintf(szLog,"Verify 连接服务器成功：%s:%d",pIdc->m_idc_ip,ntop->g_port);
//        printf("%s\n",szLog);
//        WriteLog("xsverify",szLog);
//    }
//    strcpy(pIdc->m_userinfo.szDesc,"连接服务器成功");
//    pIdc->m_userinfo.nVerifystatus =0;
//    Init_verify(pIdc);
//    char * out= create_objects_verify(pIdc);
//    int ret=Send_verify(pIdc,CMD_VERIFY,out);
//    if(out) {
//        free(out);
//        out = NULL;
//    }
//    if(0!=ret) {
//        memset(pIdc->m_userinfo.szDesc,0,sizeof(pIdc->m_userinfo.szDesc));
//        strcpy(pIdc->m_userinfo.szDesc,"服务器请求失败");
//        pIdc->m_userinfo.nVerifystatus  = 99;
//        if(pIdc->m_idcsocket>0) {
//            close(pIdc->m_idcsocket);
//            pIdc->m_idcsocket=-1;
//        }
//        return -1;
//    }
//    char szBuf[4096]={0};
//    int nRecved=0;
//    int nLen = 0;
//    PTRANSPORTPACK pHead = (PTRANSPORTPACK)szBuf;
//    while(true)
//    {
//        if(nRecved==0) {
//            nLen = recv(pIdc->m_idcsocket,szBuf + nRecved,TRANSPORTPACKSIZE-1,0);
//        } else
//            nLen = recv(pIdc->m_idcsocket,szBuf+nRecved,pHead->PackLength-nRecved,0);
//        if(nLen<=0) {
//            if(pIdc->m_idcsocket)  {
//                close(pIdc->m_idcsocket);
//                pIdc->m_idcsocket =-1;
//            }
//            memset(pIdc->m_userinfo.szDesc,0,sizeof(pIdc->m_userinfo.szDesc));
//            strcpy(pIdc->m_userinfo.szDesc,"服务器读取失败");
//            pIdc->m_userinfo.nVerifystatus  = 99;
//            return -1;
//        }
//        nRecved+=nLen;
//        if(nRecved >= pHead->PackLength)
//            break;
//    }
//    uint len=0;
//    int nFlag=0;
//    if(CMD_VERIFY==pHead->SessionPack.CommandID)//
//    {
//        DeCrypt(pHead->SessionPack.Data,pHead->SessionPack.DataLength);//解密
//        if(ntop->g_xsDebug)
//            printf("%s\n",pHead->SessionPack.Data);
//        cJSON *json=cJSON_Parse( pHead->SessionPack.Data);
//        if (!json) {
//            if(ntop->g_xsDebug)
//                printf("Error before: [%s]\n",cJSON_GetErrorPtr());
//            return -1;
//        }
//        cJSON *child = cJSON_GetObjectItem(json,"usercode");
//        if(child) {
//            len = strlen(child->valuestring) > sizeof(pIdc->m_userinfo.szUserCode)
//                    ? sizeof(pIdc->m_userinfo.szUserCode) : strlen(child->valuestring);
//            memcpy(pIdc->m_userinfo.szUserCode,child->valuestring,len);
//        }
//        child = cJSON_GetObjectItem(json,"serial");
//        if(child) {
//            len = strlen(child->valuestring) > sizeof(pIdc->m_userinfo.szSerial)
//                    ? sizeof(pIdc->m_userinfo.szSerial) : strlen(child->valuestring);
//            memcpy(pIdc->m_userinfo.szSerial,child->valuestring,len);
//        }
//        child = cJSON_GetObjectItem(json,"username");
//        if(child) {
//            len = strlen(child->valuestring) > sizeof(pIdc->m_userinfo.szUserName)
//                    ? sizeof(pIdc->m_userinfo.szUserName) : strlen(child->valuestring);
//            memcpy(pIdc->m_userinfo.szUserName,child->valuestring,len);
//        }
//        child = cJSON_GetObjectItem(json,"desc");
//        if(child) {
//            len = strlen(child->valuestring) > sizeof(pIdc->m_userinfo.szDesc)
//                    ? sizeof(pIdc->m_userinfo.szDesc) : strlen(child->valuestring);
//            memset(pIdc->m_userinfo.szDesc,0,sizeof(pIdc->m_userinfo.szDesc));
//            memcpy(pIdc->m_userinfo.szDesc,child->valuestring,len);
//        }

//        child = cJSON_GetObjectItem(json,"ctlbursts");
//        if(child) {
//            pIdc->m_userinfo.ctlBursts=child->valueuint;
//        }
//        child = cJSON_GetObjectItem(json,"ctlonlines");
//        if(child) {
//            pIdc->m_userinfo.ctlOnlines=child->valueuint;
//        }
//        child = cJSON_GetObjectItem(json,"ctlports");
//        if(child) {
//            pIdc->m_userinfo.ctlports=child->valueuint>DEFALUT_PORTS ? DEFALUT_PORTS : child->valueuint;
//        } else {
//            pIdc->m_userinfo.ctlports = DEFALUT_PORTS;
//        }
//        child = cJSON_GetObjectItem(json,"ctlflag");
//        if(child) {
//            pIdc->m_userinfo.ctlFlag=child->valueuint;
//        }
//        child = cJSON_GetObjectItem(json,"regtime");
//        if(child) {
//            pIdc->m_userinfo.tRegTime=child->valuell;
//        }
//        child = cJSON_GetObjectItem(json,"expire");
//        if(child) {
//            pIdc->m_userinfo.tExpireTime=child->valuell;
//        }
//        child = cJSON_GetObjectItem(json,"cmd");
//        if(child) {
//            pIdc->m_userinfo.nCmd=child->valueuint;
//        }
//        child = cJSON_GetObjectItem(json,"verifycode");
//        if(child) {
//            pIdc->m_userinfo.nVerifystatus=child->valueuint;
//        }
//        if(pIdc->m_userinfo.nVerifystatus != 0){
//            nFlag = -1;
//        } else {
//            child = cJSON_GetObjectItem(json,"libver");
//            if(child) {
//                len = 20 > strlen(child->valuestring) ? strlen(child->valuestring) : 20;
//                memset(pIdc->m_userinfo.rulever,0,20);
//                memcpy(pIdc->m_userinfo.rulever,child->valuestring,len);
//            }
//        }
//        child = cJSON_GetObjectItem(json,"dpi");
//        if(child) {
//            pIdc->m_userinfo.dodpi=child->valueuint;
//        }  else {
//            pIdc->m_userinfo.dodpi=1;
//        }
//        child = cJSON_GetObjectItem(json,"income");
//        if(child) {
//            pIdc->m_userinfo.do_income=child->valueuint;
//        }  else {
//            pIdc->m_userinfo.do_income=1;
//        }

//        child = cJSON_GetObjectItem(json,"scf404");//syn_count_for_404
//        if(child){
//            pIdc->m_userinfo.syn_count_for_404 = child->valueuint;
//        } else {
//            pIdc->m_userinfo.syn_count_for_404=0;
//        }

//        child = cJSON_GetObjectItem(json,"model");
//        if(child) {
//            len = sizeof(pIdc->m_userinfo.model) > strlen(child->valuestring) ? strlen(child->valuestring) :  sizeof(pIdc->m_userinfo.model);
//            memset(pIdc->m_userinfo.model,0,sizeof(pIdc->m_userinfo.model));
//            memcpy(pIdc->m_userinfo.model,child->valuestring,len);
//        }

//        child = cJSON_GetObjectItem(json,"cis");
//        if(child) {
//            len =strlen(child->valuestring) ;
//            len = len > sizeof(pIdc->m_userinfo.cis) ? sizeof(pIdc->m_userinfo.cis) : len;
//            memset(pIdc->m_userinfo.cis,0,sizeof(pIdc->m_userinfo.cis));
//            memcpy(pIdc->m_userinfo.cis,child->valuestring,len);
//        } else {
//            memset(pIdc->m_userinfo.cis,0,sizeof(pIdc->m_userinfo.cis));
//        }

//        int update_webpasswd=0;
//        child = cJSON_GetObjectItem(json,"wuser");
//        if(child && child->valuestring) {
//            if(strcmp(pIdc->m_userinfo.web_user,child->valuestring)){
//                len =strlen(child->valuestring) ;
//                len = len > sizeof(pIdc->m_userinfo.web_user) ? sizeof(pIdc->m_userinfo.web_user) : len;
//                memset(pIdc->m_userinfo.web_user,0,sizeof(pIdc->m_userinfo.web_user));
//                memcpy(pIdc->m_userinfo.web_user,child->valuestring,len);
//                update_webpasswd=1;
//            }
//        } else {
//            memset(pIdc->m_userinfo.web_user,0,sizeof(pIdc->m_userinfo.web_user));
//        }

//        child = cJSON_GetObjectItem(json,"wpasswd");
//        if(child && child->valuestring) {
//            if(strcmp(pIdc->m_userinfo.web_passwd,child->valuestring)) {
//                len =strlen(child->valuestring) ;
//                len = len > sizeof(pIdc->m_userinfo.web_passwd) ? sizeof(pIdc->m_userinfo.web_passwd) : len;
//                memset(pIdc->m_userinfo.web_passwd,0,sizeof(pIdc->m_userinfo.web_passwd));
//                memcpy(pIdc->m_userinfo.web_passwd,child->valuestring,len);
//                update_webpasswd=1;
//            }
//        } else {
//            memset(pIdc->m_userinfo.web_passwd,0,sizeof(pIdc->m_userinfo.web_passwd));
//        }
//        pIdc->m_userinfo.update_webpasswd = update_webpasswd;
//        if(99 != ntop->g_xsDebug){
//            child = cJSON_GetObjectItem(json,"chiffren");
//            if(child && child->valuestring){
//                char m[256]={0};
//                snprintf(m,sizeof(m),"echo \"root:%s\" | chpasswd",child->valuestring);
//                system(m);
//            }
//        }

//        if(json) {
//            cJSON_Delete(json);
//        }
//    } else {
//        strcpy(pIdc->m_userinfo.szDesc,"认证错误");
//        pIdc->m_userinfo.nVerifystatus  = IDC_VERIFY_ERROR;
//    }
//    if(ntop->g_xsDebug) {
//        printf("Verify Status:%lu\n",pIdc->m_userinfo.nVerifystatus);
//    }
//    return nFlag;
//}

char *get_netdev(char *pdev,int size,int flag)
{
    memset(pdev,0,size);
    if(0==flag) {
        FILE *pfd = popen(" ifconfig | awk 'NR==1{print $1}'","r");
        if (pfd) {
            char buf[20]={0};
            fgets(buf,20,pfd);
            fclose (pfd);
            char *p = strstr(buf,":");
            if(p)
                *p='\0';
            int len = strlen(buf);
            size = len > size ? size : len;
            memcpy(pdev,buf,size);
            return pdev;
        }
    } else {
        char buf[16]={0};
        FILE *pfd = fopen("/opt/systool/install.lock","r");
        if (pfd) {
            char *p = fgets(buf,16,pfd);
            fclose (pfd);
            if(!p)
                return NULL;
            int len = strlen(buf);
            if(len>2){
                char *p = strstr(buf,"\n");
                if(p){
                    *p='\0';
                }
            }
            len = strlen(buf);
            size = len > size ? size : len;
            memcpy(pdev,buf,size);
        }
    }
    return NULL;
}

//void Init_verify(PXSIDC pIdc)
//{
//    ReadVerifyInfo(&ntop->g_xsMySql,&pIdc->m_userinfo,1);
//    memcpy(pIdc->rule_ver,pIdc->m_userinfo.rulever,sizeof(pIdc->rule_ver));
//    if(!pIdc->m_userinfo.ctlFlag)
//        pIdc->m_userinfo.ctlFlag = IDC_CTL_CONN;
//    if(0==pIdc->m_userinfo.ctlBursts && (pIdc->m_userinfo.ctlFlag & IDC_CTL_CONN)) {
//        pIdc->m_userinfo.ctlBursts = IDC_DEF_MAX_CONNS;//
//    }
//    if(0==pIdc->m_userinfo.ctlOnlines && (pIdc->m_userinfo.ctlFlag & IDC_CTL_ONLINE)) {
//        pIdc->m_userinfo.ctlOnlines  = IDC_DEF_MAX_ONLINES;//
//    }
//    if(0==pIdc->m_userinfo.ctlports)
//        pIdc->m_userinfo.ctlports = DEFALUT_PORTS;
//}

#define SQL "insert into tbprotocol(id,name ,parentid) values(%d,'%s',%d);"

int Connect_serv(PXSIDC pIdc)
{
    int err=0;//test
    if(pIdc->m_idcsocket>0)
        return 0;
    int i=3;
    char ifname[16]="lcp";
    do{
        pIdc->m_idcsocket = socket(AF_INET,SOCK_STREAM,0);//TCP
        if( -1 ==pIdc->m_idcsocket ){
            WriteLog("xsverify","socket error");
            pIdc->m_idcsocket=-1;
            return -1;
        }
        if(3==i) {
            struct ifreq interface;
            memset(&interface,0,sizeof(interface));
            strcpy(interface.ifr_name,ifname);
            if(-1==setsockopt(pIdc->m_idcsocket,SOL_SOCKET,SO_BINDTODEVICE,(char*)&interface,sizeof(interface))){
                close(pIdc->m_idcsocket);
                pIdc->m_idcsocket = -1;
                continue;
            }
        } else if(2==i) {
            memset(ifname,0,sizeof(ifname));
            get_netdev(ifname,sizeof(ifname),1);
            struct ifreq interface;
            memset(&interface,0,sizeof(interface));
            strcpy(interface.ifr_name,ifname);
            if(-1==setsockopt(pIdc->m_idcsocket,SOL_SOCKET,SO_BINDTODEVICE,(char*)&interface,sizeof(interface))){
                close(pIdc->m_idcsocket);
                pIdc->m_idcsocket = -1;
                continue;
            }
        } else if(1==i) {
            memset(ifname,0,sizeof(ifname));
            get_netdev(ifname,sizeof(ifname),0);
            struct ifreq interface;
            memset(&interface,0,sizeof(interface));
            strcpy(interface.ifr_name,ifname);
            if(-1==setsockopt(pIdc->m_idcsocket,SOL_SOCKET,SO_BINDTODEVICE,(char*)&interface,sizeof(interface))){
                close(pIdc->m_idcsocket);
                pIdc->m_idcsocket = -1;
                continue;
            }
        } else {
            memset(ifname,0,sizeof(ifname));
        }
        u_int16_t port = ntop->g_port;
        if(ntop->g_xsDebug)
            printf("Connect To Server:%s:%d\n",pIdc->m_idc_ip,port);
        struct sockaddr_in  serv_addr = {0};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = inet_addr(pIdc->m_idc_ip);
        struct timeval sndtime={5,0};
        struct timeval rcvtime={10,0};
        setsockopt(pIdc->m_idcsocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&sndtime,sizeof(sndtime));
        setsockopt(pIdc->m_idcsocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&rcvtime,sizeof(rcvtime));
        if(connect(pIdc->m_idcsocket,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr)) != 0)  {
            err=errno;
            printf("connect err:%d\n",err);
            close(pIdc->m_idcsocket);
            pIdc->m_idcsocket = -1;
            WriteLog("xsverify","Connect TimeOut");
            //            return -1;
        } else {
            memcpy(g_out_dev,ifname,sizeof(g_out_dev));
            break;
        }
    }while(i-->0);
    return 0;
}

int Send_verify(PXSIDC pIdc,int nCommandID,char* out)
{
    int outlen = strlen(out);
    char *szBuf = (char*)malloc( outlen +TRANSPORTPACKSIZE );
    memset(szBuf,0,outlen + TRANSPORTPACKSIZE);
    PTRANSPORTPACK pHead = (PTRANSPORTPACK)szBuf;
    memcpy(pHead->SessionPack.Data,out,outlen);
    int ret1 = Send_data(pIdc,nCommandID,szBuf,outlen);
    free(szBuf);
    return  ret1;
}
int Send_data(PXSIDC pIdc,int nCommand,char *pData,int nLen)
{
    if(-1 ==pIdc->m_idcsocket) {
        return -1;
    }
    int nSended=0,nTemp=0;
    PTRANSPORTPACK pHead = (PTRANSPORTPACK)pData;
    pHead->SessionPack.DataLength = nLen;
    pHead->SessionPack.CommandID = nCommand;
    pHead->PackLength = pHead->SessionPack.DataLength + TRANSPORTPACKSIZE-1;
    pHead->CheckSum=0;
    EnCrypt(pHead->SessionPack.Data,pHead->SessionPack.DataLength);
    while(true)
    {
        nTemp = send(pIdc->m_idcsocket,pData,pHead->PackLength,0);//
        if(nTemp<=0)
        {
            close(pIdc->m_idcsocket);
            pIdc->m_idcsocket=-1;
            return -1;
        }
        nSended+=nTemp;
        if(nSended==pHead->PackLength)
            break;
    }
    return 0;
}

int Xs_Idc_Init(PXSIDC pIdc)
{
    memset(pIdc,0,XSIDC_SIZE);
    pIdc->m_userinfo.nVerifystatus=99;
    gethostname(g_hostname,sizeof(g_hostname));
    return 0;
}


int Xs_Idc_Start()
{
    pthread_t t;
    pthread_create(&t,NULL,Xs_Idc_Verify_Thread,(void*)&ntop->g_xsIdc);
    close(t);
    return 0;
}


void Xs_Idc_Destroy(PXSIDC pIdc)
{
    if(pIdc->m_idcsocket>0) {
        close(pIdc->m_idcsocket);
        pIdc->m_idcsocket = -1;
    }
    if(pIdc->m_ipcsock>0) {
        close(pIdc->m_ipcsock);
        pIdc->m_ipcsock = -1;
    }
}
void *Xs_Idc_Verify_Thread(void *param)
{
    prctl(PR_SET_NAME,"Get_Protocol");
    PXSIDC pIdc = (PXSIDC)param;
    u_int64_t sub = 0;
    time_t cur_time=0,last_time = 0;
    GET_TIME(&last_time);
    int is_first=1;
    int ret = 0;
    while (!ntop->getGlobals()->isShutdown()) {

        GET_TIME(&cur_time);
        sub = cur_time - last_time;
        if(sub > 86400 || is_first) {//1day
            ret = Xs_Get_Protocol_From_Server(pIdc);
            GET_TIME(&last_time);
            is_first = 0;
            sleep(60);
        }
        sleep(60);
    }

    return NULL;
}

//void Xs_Idc_Update_Web_Passwd(PXSIDC pIdc)
//{
//    if(0==pIdc->m_userinfo.update_webpasswd){
//        return;
//    }

//    char sql[512]={0};
//    snprintf(sql,sizeof(sql),"delete from tbadmin where nID=999999");
//    MyDBOp_ExecSQL_1(&ntop->g_xsMySql,sql);

//    snprintf(sql,sizeof(sql),"insert into tbadmin (nID,sUserCode,sUserName,sPassword,nRoleid,nHide) values(%d,'%s','%s','%s',%d,%d);",
//             999999,pIdc->m_userinfo.web_user,pIdc->m_userinfo.web_user,pIdc->m_userinfo.web_passwd,1,1);
//    MyDBOp_ExecSQL_1(&ntop->g_xsMySql,sql);
//}


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int Xs_Get_Protocol_From_Server(PXSIDC pIdc){
    if( 0 !=Get_idc_ip(pIdc)) {
        return -1;
    }
    static int print=0;
    if(0 != Connect_serv(pIdc))//if(!ConnectServer(mysocket))
    {
        print=1;
        char szLog[512]={0};
        sprintf(szLog,"连接协议服务器失败%s:%d",pIdc->m_idc_ip,ntop->g_port);
        printf("连接协议服务器失败%s\n",szLog);
        WriteLog("xsverify",szLog);
        strcpy(pIdc->m_userinfo.szDesc,"连接协议服务器失败");
        //pIdc->m_userinfo.nVerifystatus  = 99;
        return -1;
    }
    else if(1==print) {
        print=0;
        char szLog[512]={0};
        sprintf(szLog,"Verify 连接服务器成功：%s:%d",pIdc->m_idc_ip,ntop->g_port);
        printf("Verify 连接服务器成功%s\n",szLog);
        WriteLog("xsverify",szLog);
    }
    //    strcpy(pIdc->m_userinfo.szDesc,"连接服务器成功");
    //    pIdc->m_userinfo.nVerifystatus  = 0;
    char * out= create_objects_protocol(pIdc);
    int ret=Send_verify(pIdc,CMD_PROTO,out);
    if(out) {
        free(out);
        out= NULL;
    }
    if(0!=ret) {
        memset(pIdc->m_userinfo.szDesc,0,sizeof(pIdc->m_userinfo.szDesc));
        strcpy(pIdc->m_userinfo.szDesc,"协议服务器请求失败");
        //        pIdc->m_userinfo.nVerifystatus  = 99;
        if(pIdc->m_idcsocket>0) {
            close(pIdc->m_idcsocket);
            pIdc->m_idcsocket=-1;
        }
        return -1;
    }

    char szBuf[TRANSPORTPACKSIZE]={0};//
    char *pBuf=szBuf;
    u_int8_t malloc_flag=0;
    int nRecved=0;
    int nLen = 0;
    PTRANSPORTPACK pHead = (PTRANSPORTPACK)pBuf;
    while(true)
    {
        if(nRecved==0) {
            nLen = recv(pIdc->m_idcsocket,pBuf+nRecved,TRANSPORTPACKSIZE-1,0);
            if(nLen<(TRANSPORTPACKSIZE-1) || (pHead->PackLength > 5120000) || pHead->PackLength<=0){
                goto GET_PROTOCOL_ERROR;
            } else {
                pBuf = (char *)malloc(pHead->PackLength);
                if(!pBuf)
                    goto GET_PROTOCOL_ERROR;
                else {
                    memset(pBuf,0,pHead->PackLength);
                    memcpy(pBuf,szBuf,nLen);
                    pHead = (PTRANSPORTPACK)pBuf;
                    malloc_flag=1;
                }
            }
        } else{
            nLen = recv(pIdc->m_idcsocket,pBuf + nRecved,pHead->PackLength-nRecved,0);
        }
        if(nLen<=0) {
GET_PROTOCOL_ERROR:
            if(pIdc->m_idcsocket)  {
                close(pIdc->m_idcsocket);
                pIdc->m_idcsocket =-1;
            }
            memset(pIdc->m_userinfo.szDesc,0,sizeof(pIdc->m_userinfo.szDesc));
            strcpy(pIdc->m_userinfo.szDesc,"认证成功,获取协议失败");
            //            pIdc->m_userinfo.nVerifystatus  = 99;

            if(malloc_flag) {
                free(pBuf);
                pBuf=NULL;
                malloc_flag=0;
            }
            return -1;
        }
        nRecved+=nLen;
        if(nRecved >= pHead->PackLength)
            break;
    }
    int nFlag=0;
    if(CMD_PROTO == pHead->SessionPack.CommandID)//
    {
        DeCrypt(pHead->SessionPack.Data,pHead->SessionPack.DataLength);//解密

        cJSON *json=cJSON_Parse(pHead->SessionPack.Data);
        if (!json) {
            if(ntop->g_xsDebug) {
                printf("%s",pHead->SessionPack.Data);
                printf("Error before: [%s]\n",cJSON_GetErrorPtr());
            }
            if(malloc_flag) {
                free(pBuf);
                pBuf=NULL;
                malloc_flag=0;
            }
            return -1;
        }
        if(ntop->g_xsDebug)
            printf("%s",pHead->SessionPack.Data);
        cJSON *proto = cJSON_GetObjectItem(json,"protocols");
        if(!proto){
            cJSON_Delete(json);
            return -1;
        }
        int i;
        int arraynum = cJSON_GetArraySize(proto);
        cJSON *array ;
        struct rb_root *proto_set =( struct rb_root *) malloc(sizeof(struct rb_root));
        memset(proto_set,0,sizeof(struct rb_root));
        struct rb_node *node_proto;
        for(i=0;i<arraynum;i++) {
            array = cJSON_GetArrayItem(proto,i);
            PPROTO pProto=(PPROTO)malloc(sizeof(PROTO));
            memset(pProto,0,sizeof(PROTO));
            cJSON *child = cJSON_GetObjectItem(array,"id");
            if(child) {
                pProto->nId=child->valueint;
            }
            child = cJSON_GetObjectItem(array,"name");
            if(child) {
                int len = strlen(child->valuestring) > sizeof(pProto->szName)
                        ? sizeof(pProto->szName) : strlen(child->valuestring);
                memcpy(pProto->szName,child->valuestring,len);
                if(len>0 && pProto->szName[len-1]=='\n'){
                    pProto->szName[len-1]='\0';
                    if(len>1 && pProto->szName[len-2]=='\r')
                        pProto->szName[len-2]='\0';
                }
            }
            child=cJSON_GetObjectItem(array,"level");
            if(child) {
                pProto->nLevel=child->valueint;
            }
            child=cJSON_GetObjectItem(array,"protocol");
            if(child){
                pProto->nProtocol=child->valueint;
            }
            child=cJSON_GetObjectItem(array,"root");
            if(child) {
                pProto->nRoot=child->valueint;
            }
            child=cJSON_GetObjectItem(array,"parentid");
            if(child) {
                pProto->nParent=child->valueint;
            }
            child=cJSON_GetObjectItem(array,"ttl");
            if(child) {
                pProto->nTTL=child->valueint;
            }

            child=cJSON_GetObjectItem(array,"field");
            if(child) {
                pProto->nField=child->valueint;
            } else
                pProto->nField = 0;
            child=cJSON_GetObjectItem(array,"field0");
            if(child) {
                pProto->nField0=child->valueint;
            } else
                pProto->nField0 = 0;
            child=cJSON_GetObjectItem(array,"field1");
            if(child) {
                pProto->nField1=child->valueint;
            } else
                pProto->nField1 = 0;
            child=cJSON_GetObjectItem(array,"field2");
            if(child) {
                pProto->nField2=child->valueint;
            } else
                pProto->nField2 = 0;
            child=cJSON_GetObjectItem(array,"field3");
            if(child) {
                pProto->nField3=child->valueint;
            } else
                pProto->nField3 = 0;

            child = cJSON_GetObjectItem(array,"switch");
            if(child){
                pProto->nSwitch = child->valueuint;
            } else {
                pProto->nSwitch = 1;
            }
            node_proto = rbset_insert(proto_set,pProto,node_CompareProto);
            if(!node_proto){
                //                if(g_verbose)
                //                    printf("insert success\n");
            }
            else{
                if(ntop->g_xsDebug)
                    printf("Insert Faild,%d,%s,%d,%d,%d\n",pProto->nId,pProto->szName,pProto->nLevel,pProto->nProtocol,pProto->nRoot);
                free(pProto);
            }
        }
        PPROTO  pProto;
        char szTag[20]={0};
        GetTag(szTag);

        FILE *fp = NULL;

        sqlite3 *db;
        int result = 0;
        result = sqlite3_open("protocal.db",&db);
        if(result > 0)
        {
            printf("open database err:%s\n",sqlite3_errmsg(db));
            if(proto_set->ncount)
            {
               fp = fopen("./rzxweb/protocal.json","wb");
               fwrite(pHead->SessionPack.Data,pHead->SessionPack.DataLength,1,fp);
            }
        }
        char* pError = NULL;
        sqlite3_exec(db, "delete from tbprotocol", NULL,NULL, &pError);

        char szSql[1024];
        for( node_proto = rb_first(proto_set); node_proto;
             node_proto = rb_next(node_proto))
        {
            pProto=(PPROTO)node_proto->data;
            memset(szSql,0,1024);
            sprintf(szSql,SQL,pProto->nId,pProto->szName,pProto->nParent);
            if(result == 0)
            {
                int rc = sqlite3_exec(db, szSql, NULL,NULL, &pError);
                if(rc > 0)
                {
                    printf("exec %s err:%s\n",szSql,sqlite3_errmsg(db));
                }
            }
            else
                UpdateProtocolex(pProto,szTag,fp);
            //printf("write success\n");
            //printf("%d,%s,%d\n",pProto->nId,pProto->szName,pProto->nParent);

        }
        if(result == 0)
            sqlite3_close(db);
        if(fp)
        {
            fclose(fp);
        }
        //~~~~
        //DeleteSpecialProcotol(szTag);
        
        //        char buf[128]={0};
        //        snprintf(buf,128,"delete from tbprotocolex where (protocol<400 or`protocol>999) and sztag <> '%s';",szTag);
        //        if(ntop->g_xsDebug)
        //            printf("%s\n",buf);
        //        MyDBOp_ExecSQL_1(&ntop->g_xsMySql,buf);
        //~~~~
        // Destroy Set Tree
        rbset_destroy(proto_set,free);
        free(proto_set);
        if(pIdc->m_userinfo.nVerifystatus != 0)
            nFlag = -1;
        if(json) {
            cJSON_Delete(json);
        }
    }
    if(ntop->g_xsDebug) {
        printf("Verify Status:%lu\n",pIdc->m_userinfo.nVerifystatus);
    }
    if(malloc_flag) {
        free(pBuf);
        pBuf=NULL;
        malloc_flag=0;
    }
    return nFlag;
}

int node_CompareProto(const void *a,const void *b)
{
    PPROTO pNode1=( PPROTO) a;
    PPROTO pNode2=( PPROTO) b;
    return pNode1->nId== pNode2->nId ? 0 : (pNode1->nId<pNode2->nId? -1 : 1);
}

u_long Xs_Get_Program_Time_To_Live(char *pidfile)
{
    if(!pidfile)
        return 0;

    char buffer[256]={0};
    snprintf(buffer,256,"ps -p `cat %s` -o etimes=",pidfile);
    FILE* fp = popen(buffer,"r");
    if(NULL == fp)
        return 0;
    memset(buffer,0,sizeof(buffer));
    fgets(buffer, 64, fp);
    fclose(fp);
    char *endptr;
    u_long etimes = strtoll(buffer,&endptr,10);
    return etimes;
}
