#include"ntop_includes.h"
#include"lib/xssharefunc.h"

#define XWEBINTERACTION_EPOLL_PORT   18002

int Xsweb_ReturnMsg_Udp(int sock,char *msg,struct sockaddr_in sin)
{
    if(!msg)
        return -1;
    return sendto(sock,msg,strlen(msg),0,(struct sockaddr*)&sin,sizeof(sin));
}

int Parse_JsonObject_Get_Value(PXSJSONWEB json_web,json_object *JSONroot){
    int ret=-1;
    json_object * obj_XswebFiled,*obj_XswebAct,*obj_XswebActEntry;
    if(!json_object_object_get_ex(JSONroot,"XSWEB_FIELD",&obj_XswebFiled))
        return -1;
    json_web->filed= json_object_get_int(obj_XswebFiled);

    if(!json_object_object_get_ex(JSONroot,"XSWEB_ACT",&obj_XswebAct))
        return -1;
    json_web->act = json_object_get_int(obj_XswebAct);

    if(!json_object_object_get_ex(JSONroot,"XSWEB_ACTENTRY",&obj_XswebActEntry))
        return -1;
    json_web->actEntry = json_object_get_int(obj_XswebActEntry);

    ret=0;
    return ret;
}

int Xsweb_DealWithClientRequest_Udp(int sock/*PXSWORK pXsWork*/,char* pbuf,struct sockaddr_in sin)
{
    if(ntop->g_xsDebug)
        printf("%s\n",pbuf);
    int ret=-1, status=-1;
    XSJSONWEB json_web;
    json_object * pRetRoot = json_object_new_object();//return backstage
    if(!pRetRoot) return(NULL);
    //    由str里的JSON字符串生成JSON对象
    json_object *JSONroot= json_tokener_parse(pbuf);
    if(NULL==JSONroot) {
        status=-99;
        goto XSWEB_OUT;
    }
    /*~Gets the value of all fields from JSONroot~*/
    ret=Parse_JsonObject_Get_Value(&json_web,JSONroot);
    if( ret<0 )  return ret;
    char log[128];
    memset(log,0,sizeof(log));
    snprintf(log,127,"Start Do With :%d",json_web.filed);
    WriteLog("xsweb",log);

    switch(json_web.filed)
    {
    case XSWEB_FIELD_MAC:
        status = Xsweb_Field_Mac(&json_web,JSONroot,pRetRoot);
        if(-1==status) {
            WriteLog("xsweb",pbuf);
        }
        break;
    case XSWEB_FIELD_FLOW:
        status= Xsweb_Field_Flow(&json_web,JSONroot,pRetRoot);
        if(-1==status) {
            WriteLog("xsweb",pbuf);
        }
        break;
    case XSWEB_FIELD_HOST:
        status = Xsweb_Field_Host(&json_web,JSONroot,pRetRoot);
        if(-1==status) {
            WriteLog("xsweb",pbuf);
        }
        break;
    case XSWEB_FIELD_INTERFACE:
        status = Xsweb_Field_Interface(&json_web,JSONroot,pRetRoot);
        if(-1==status) {
            WriteLog("xsweb",pbuf);
        }
        break;
    default :
        break;
    }
XSWEB_OUT:
    if(JSONroot){
        json_object_put(JSONroot);//释放
        JSONroot=NULL;
    }

    char *pMsg = strdup(json_object_to_json_string(pRetRoot));//将jsonJSON对象转为JSON字符串
    if(0!=memcmp(pMsg,"{\n}",strlen("{\n}")))
        ret = Xsweb_ReturnMsg_Udp(sock/*pXsWork*/,pMsg,sin);//返回给后台
    if(ntop->g_xsDebug){
        if(pMsg)
            printf("msg:%s\n",pMsg);
        else if( ret<=0 || status != 0)
            printf("ret:%d   status:%d\n",ret,status);
    }
    snprintf(log,127,"End Do With :%d  ret:%d   status:%d",json_web.filed,ret,status);
    WriteLog("xsweb",log);

    json_object_put(pRetRoot);//释放
    if(pMsg)
        free(pMsg);

    ret=0;
    return ret;
}

void Xsweb_UdpServer(){
    int sock=CreateUDPSocket(XWEBINTERACTION_EPOLL_PORT,1,1,0);
    if(-1==sock){
        WriteLog("xsweb","Web Thread: Create UdpSocket Failed.");
        return ;   //Create UDP Socket Failed.
    }
    int ret=0,res=-1;
    int rebuild=0;
    char *rcvBuf;//,,,,,,,,,,,,,,,,,,,,,,,location

    struct sockaddr_in sin={0};
    socklen_t slen = sizeof(sin);

    WriteLog("xsweb","Web Thread Start");

    while(!ntop->getGlobals()->isShutdown()){

        rcvBuf=(char *)malloc(My_BUFSIZ);
        if(!rcvBuf)  continue;
        memset(rcvBuf,0,My_BUFSIZ);

        ret=recvfrom(sock,rcvBuf,My_BUFSIZ,0,(struct sockaddr *)&sin,&slen);
        if(ret <= 0)
        {
            int err = errno;
            if(err != EAGAIN && err != EINTR)
            {
                close(sock);
                usleep(500);
                sock = CreateUDPSocket(XWEBINTERACTION_EPOLL_PORT,1,1,0);//,,,,,,,,,,,,,,,,,,,port
                if(rebuild > 30)
                {
                    WriteLog("xsweb","Web Thread: Re-Create UdpSocket Failed.");
                    break;
                }
                ++rebuild;
            }
            continue;
        }
        rcvBuf[ret]='\0';
        res=Xsweb_DealWithClientRequest_Udp(sock/*pXsWork*/,rcvBuf,sin);
        if(res<0)  continue;
    }
    free(rcvBuf);
    if(-1 != sock)
        close(sock);
}

void *Xsweb_EpollWork_Thread(void *param){
    prctl(PR_SET_NAME,"Xsweb");
    while(!ntop->getGlobals()->isShutdown()){
        Xsweb_UdpServer();
    }
    char log[128];
    memset(log,0,sizeof(log));
    snprintf(log,128-1,"Exit???[shutdown :%d ] [signal : %d ]",ntop->getGlobals()->isShutdown(),ntop->g_signal);
    WriteLog("xsweb",log);
    return NULL;
}

void Xsweb_Work(/*PXSWORK pXsWork*/){
    pthread_t  web_Thread;
    pthread_create(&web_Thread,NULL,Xsweb_EpollWork_Thread,NULL/*pXsWork*/);
}



