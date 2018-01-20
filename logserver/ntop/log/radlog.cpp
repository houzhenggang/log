//#include"ntop_includes.h"
//#include"lib/xs_prediction.h"


//void XsFree_pPopBuf_RadLog(char *pPopBuf){
//    free(pPopBuf);
//    pPopBuf=NULL;
//}

//void XsRadLog_init(){
//    ntop->radLog.port=6002;
//    Xs_List_Init(&ntop->radLog.list);
//    int res=sem_init(&ntop->radLog.bin_sem,0,0);
//    if(res!=0)  perror("Semaphore initialization failed");
//}


//void *Thread_RadLog_work(void *param)
//{
//    prctl(PR_SET_NAME,"rcvpkt_radlog");
//    PXsRadLog pRadLog = (PXsRadLog)param;
//    int len =0,err=0;
//    int reboot=0;  /*Create Udp Socket Failed 30times .We Reboot Program*/
//    char *buffer=NULL;
//    pRadLog->list.nums = 0;

//    struct sockaddr_in sin;
//    socklen_t s_len = sizeof(sin);

//    int socket=CreateUDPSocket(ntop->radLog.port,0,0,0);
//    if(-1 != socket)
//        Xs_Set_UDPSocket_Buffer(socket);

//    while(!ntop->getGlobals()->isShutdown())
//    {
//        buffer=(char *)malloc(My_BUFSIZ);
//        if(!buffer)  continue;
//        memset(buffer,0,My_BUFSIZ);

//        len=recvfrom(socket, buffer,My_BUFSIZ,0,(struct sockaddr*)&sin,&s_len);
//        if(len <= 0)
//        {
//            if(buffer != NULL)
//                XsFree_pPopBuf_RadLog(buffer);

//            printf("Recvfrom RadLog Failed:%d\n",errno);

//            err = errno;
//            if(err != EAGAIN && err != EINTR)
//            {
//                close(socket);
//                usleep(500);
//                socket = CreateUDPSocket(ntop->radLog.port,0,0,0);
//                if(reboot > 50)
//                {
//                    WriteLog("ntop","Main Thread: Re-Create UdpSocket Failed. Exit!!!!");
//                    kill(getpid(),SIGINT);
//                    sleep(1);
//                }
//                if(-1!=socket)
//                    Xs_Set_UDPSocket_Buffer(socket);
//                ++reboot;
//            }
//            continue;
//        }

//        if(unlikely(0 != Xs_List_mp_PushList(&ntop->radLog.list, buffer))){
//            XsFree_pPopBuf_RadLog(buffer);
//            continue;
//        }
//        sem_post(&ntop->radLog.bin_sem);
//        buffer = NULL;
//    }
//    close(socket);
//    return NULL;
//}

//void *Thread_InsertEs_ForRadlog(void *param){
//    PXsRadLog pRadLog = (PXsRadLog)param;
//    prctl(PR_SET_NAME,"GetRadLog");
//    XsRadFiled radFiled;
//    char * pPopBuf=NULL;

//    while(!ntop->getGlobals()->isShutdown()){
//        sem_wait(&pRadLog->bin_sem);

//        pPo  pBuf=(char *) Xs_List_mc_POPList(&pxswork->list);
//        if(!pPopBuf) continue;

//        memset(&parseFiled,0,PRASEFILED_SIZE);
//        if(-1 == XsGet_Type(parseFiled.bufType,pPopBuf,"type:","types:",sizeof(parseFiled.bufType)))//get type/types
//            continue;
//        if(-1 == XsGet_Oth_Filed(parseFiled.bufLabel,pPopBuf,"label:",sizeof(parseFiled.bufLabel)))//get label
//            continue;
//        if(-1 == XsGet_Usr_Or_snat(parseFiled.szUsr,pPopBuf,"usr:",sizeof(parseFiled.szUsr)))//get usr
//            memcpy(parseFiled.szUsr," ",sizeof(parseFiled.szUsr));//为了适应没有snat字段的路由版本 赋值为空格
//        if(-1 == XsGet_Oth_Filed(parseFiled.szbufDate,pPopBuf,"date:",sizeof(parseFiled.szbufDate)))//get date
//            continue;
//        parseFiled.bufDate = atoi(parseFiled.szbufDate);
//        if(-1 == XsGet_Oth_Filed(parseFiled.bufTime,pPopBuf,"time:",sizeof(parseFiled.bufTime)))//get time
//            continue;
//        if(-1 == XsGet_Oth_Filed(parseFiled.bufSmac,pPopBuf,"smac:",sizeof(parseFiled.bufSmac)))//get smac
//            continue;
//        if(-1 == XsGet_Oth_Filed(parseFiled.bufMethod,pPopBuf,"method:",sizeof(parseFiled.bufMethod)))//get method
//            continue;
//        if((strcmp(parseFiled.bufType,"IDlog")==0)){
//            char *type = my_strlwr(parseFiled.bufType);
//            memcpy(parseFiled.bufType,type,sizeof(parseFiled.bufType));
//            if(-1 == XsGet_Oth_Filed(parseFiled.bufSip,pPopBuf,"ip:",sizeof(parseFiled.bufSip)))//get src
//                continue;
//        }
//        if((strcmp(parseFiled.bufType,"idlog")!=0)){
//            if(-1 == XsGet_Usr_Or_snat(parseFiled.L7,pPopBuf,"l7:",sizeof(parseFiled.L7)))//get L7
//            {
//                memcpy(parseFiled.L7," ",sizeof(parseFiled.L7));
//                l7 = 0;
//            }
//            l7 = atoi(parseFiled.L7);

//            if(-1 == XsGet_Oth_Filed(parseFiled.bufSrc,pPopBuf,"src:",sizeof(parseFiled.bufSrc)))//get src
//                continue;
//            if(-1 == XsGet_Ip_And_Port(pPopBuf,parseFiled.bufSrc,parseFiled.bufSip,&parseFiled.nSport))
//                continue;
//            //
//            if(-1 == XsGet_Oth_Filed(parseFiled.bufDst,pPopBuf,"dst:",sizeof(parseFiled.bufDst)))//get dst
//                continue;
//            if(-1 == XsGet_Ip_And_Port(pPopBuf,parseFiled.bufDst,parseFiled.bufDip,&parseFiled.nDport))
//                continue;

//            if(-1 == XsGet_Usr_Or_snat(parseFiled.bufSnat,pPopBuf,"snat:",sizeof(parseFiled.bufSnat))){
//                memcpy(parseFiled.bufSnat_ip,"0",20);//为了适应没有snat字段的路由版本 赋值为0
//                parseFiled.nSnat_port=0;
//            }else{
//                if(-1 == XsGet_Ip_And_Port(pPopBuf,parseFiled.bufSnat,parseFiled.bufSnat_ip,&parseFiled.nSnat_port))
//                    continue;
//            }

//            if(-1 == XsGet_Oth_Filed(parseFiled.bufProto,pPopBuf,"proto:",sizeof(parseFiled.bufProto)))//get proto
//                continue;
//            if(-1 == XsGet_Oth_Filed(parseFiled.bufUrl,pPopBuf,"data:",sizeof(parseFiled.bufUrl)))//get data
//                continue;
//        }

//        if((strcmp(parseFiled.bufType,"qqlog")==0)&&strlen(parseFiled.bufUrl)<=4){
//            XsFree_pPopBuf(pPopBuf);
//            continue;
//        }
//        if(ntop->g_xsDebug)  printf("FailedAnalysis: %d\n",ntop->nFailedAnalysis);

//        if((strcmp(parseFiled.bufType,"mail")==0)){//过滤不符号邮箱格式的邮箱帐号
//            char *p = xs_strnstr(parseFiled.bufUrl,"@",strlen(parseFiled.bufUrl),1);
//            if(p == NULL){
//                XsFree_pPopBuf(pPopBuf);
//                continue;
//            }
//        }


//        ntop->getES()->dumpEsFlowDpi(&parseFiled,&accountInfo,&phoneMsg);
//        XsFree_pPopBuf(pPopBuf);
//    }
//    return NULL;
//}
