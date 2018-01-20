#ifndef XS_IDC_H
#define XS_IDC_H

#include <pthread.h>
#include "lib/xs_compatibility.h"
#include "lib/xs_def.h"
#include "lib/xscompat.h"
#include "lib/xsipcstruct.h"
#include"lib/rbtree.h"
#include"lib/rbset.h"
#include "xsmysql/mydbop.h"
#include "lib/key_elem.h"

typedef struct _xsidc_
{
    struct rb_root rule_set;
    USERINFOEX2 m_userinfo;
    char szHost[256];
    char m_idc_ip[20];
    int m_idcsocket,m_ipcsock;
    char rule_ver[20];
    char xspeeder_ver[32];
    char cpu_name[64];
    u_int dpi_ver;
    u_int16_t procs;
    u_int8_t do_send_reload_rule_msg_2_xspeeder;
    u_long ctl_curonlines;
    u_long ctl_curbursts;
    u_long ctl_maxonlines;
    u_long ctl_maxbursts;
    u_long ctl_type;
    u_long rx_bps,tx_bps;
    u_int usefulwan;
    u_long ncf,mcf; //now create flow , max create flow
    u_long ncn,mcn; //now create nat ,max create nat
    u_long mfnat,fnat;//failure nat flow
    u_int8_t interface_flag;
}XSIDC,*PXSIDC;
#define XSIDC_SIZE      sizeof(XSIDC)
int Connect_serv(PXSIDC pIdc);
int Send_data(PXSIDC pIdc,int nCommand,char *pData,int nLen);

int GetHostIP(char * host,char *pIP);
int Get_idc_ip(PXSIDC pIdc);
//int Verify(PXSIDC pIdc);
//void Init_verify(PXSIDC pIdc);
int Send_verify(PXSIDC pIdc,int nCommandID,char* out);

int node_CompareProto(const void *a,const void *b);


char * create_objects_verify(PXSIDC pIdc );


char * create_objects_protocol(PXSIDC pIdc );




void *Xs_Idc_Verify_Thread(void *param);




void *FRPC_thread(void *param);
int Xs_Idc_Init(PXSIDC pIdc);
void Xs_Idc_Destroy(PXSIDC pIdc);
int Xs_Idc_Start();


//void Xs_Idc_Update_Web_Passwd(PXSIDC pIdc);
u_long Xs_Get_Program_Time_To_Live(char *pidfile);

int Xs_Get_Protocol_From_Server(PXSIDC pIdc);

char *get_netdev(char *pdev,int size,int flag);

#endif // XS_IDC_H

