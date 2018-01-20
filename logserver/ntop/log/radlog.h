//#ifndef RLOG
//#define RLOG
//#include"ntop_includes.h"


//typedef struct _xsradlog{
//    int port;//radlog接收端口
//    sem_t  bin_sem;
//    XS_LIST list;
//}XsRadLog,*PXsRadLog;
//#define XSWORK_SIZE       sizeof(XsRadLog)


//// string 最长不超过253字节
//typedef struct _xsradfiled{
//    char username[64]; //上网账号
//    float input_total  ; //会话的上行流量（KB）
//    float output_total ; //会话的下行流量（KB）
//    int  input_packets; //会话的上行包数量
//    int output_packets ;  //会话的下行包数量
//    char  session_id[64]; //会话id
//    int  session_time ;//会话时长(秒)
//    int  start_time ;//timestemp 会话开始时间
//    int stop_time;  //timestemp 会话结束时间
//    int     terminate_cause ;  //会话中止原因
//    char  mac_addr[32] ; //mac地址
//    int  vlan_id1 ; //内层VLAN
//    int  vlan_id2 ;//外层VLAN
//    char  framed_netmask[32] ;  //地址掩码
//    char  framed_ipaddr[32] ; //IP地址
//    char  nas_class[32] ;  //nas class
//    char  nas_paddr[32] ;  //Nas 外网地址
//    char  nas_addr[32] ; //nas ip地址
//    char  nas_port[16] ;  //接入端口
//    char  nas_port_id[64] ;  //接入端口物理信息
//    int     nas_port_type ;  //接入端口类型
//    int service_type ;  //接入服务类型
//    int   session_timeout ; //会话超时时间
//}XsRadFiled,*pXsRadFiled;
//#define XSRADFILED_SIZE sizeof(XsRadFiled)

//void XsRadLog_init();
//void *Thread_RadLog_work(void *param);
//void *Thread_InsertEs_ForRadlog(void *param);


//#endif // RLOG

