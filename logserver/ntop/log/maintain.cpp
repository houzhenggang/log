#include"ntop_includes.h"


void *MainTain_Thread(void *p)
{
    prctl(PR_SET_NAME,"MainTain");
    u_int64_t sub=0;
    time_t cur_time=0,last_time=0,last_refresh = 0,last_delete_time=0;
    struct rb_node *node;
    GET_TIME(&last_time);
    GET_TIME(&last_delete_time);
    GET_TIME(&last_refresh);

    char rcvpkt[128] = {0};
    char parsepkt[64] = {0};
    int ret = 0;

    while(!ntop->getGlobals()->isShutdown()){
        GET_TIME(&cur_time);

        sub = cur_time - last_time;
        if(sub>5){
            printf("%lu  %lu  %lu  %lu  %lu\n",recvpacket,parsepacket,enqueuepacket,dequeuepacket,mallocfail);
            //  MyDBOp_Ping(&ntop->g_xsMySql);
            FILE *fd = fopen(RCV_PKT_NUM_PATH, "wb");
            node = rb_first(&ntop->mapXsWork);
            json_object* root = json_object_new_array();
            for(node;node&&node->data&&fd;node=rb_next(node)){
                PXsWork pxswork = (PXsWork)node->data;
                json_object* object = json_object_new_object();
                json_object_object_add(object, "port",
                                       json_object_new_int(pxswork->port));
                json_object_object_add(object, "count",
                                       json_object_new_int(pxswork->count));
                json_object_object_add(object, "parsecount",
                                       json_object_new_int(pxswork->nParseCount));
                json_object_object_add(object, "RecvPacket",
                                       json_object_new_int(pxswork->list.nRecvPacket));
                json_object_object_add(object, "nParsePacket",
                                       json_object_new_int(pxswork->list.nParsePacket));
                json_object_array_add(root,object);
                Xs_List_zero_ParseNum(&pxswork->list);
            }
            const char* p = json_object_get_string(root);
            fputs(p,fd);
            printf("%s\n",p);
            json_object_put(root);
            fclose(fd);
            GET_TIME(&last_time);
        }

        sub = cur_time - last_refresh;
        if(sub>5){

            if(!Utils::postHTTPJsonData(ntop->getPrefs()->get_es_user(),
                                        ntop->getPrefs()->get_es_pwd(),
                                        /*ntop->getPrefs()->get_es_url()*/"http://localhost:9200/_refresh", "")){
                /* Post failure */
                WriteLog("PostEs","refresh"); // sleep(1);
            }else {
                ntop->getTrace()->traceEvent(TRACE_INFO, "ntopng template successfully sent to ES");
            }

            GET_TIME(&last_refresh);
        }

        sub = cur_time - last_delete_time;
        if(sub > (Xs_Seconds_Per_Day*2))
        {
            RemoveLogFile("ntop");
            RemoveLogFile("xsweb");
            RemoveLogFile("FailedAnalysis");

            GET_TIME(&last_delete_time);
        }

         sleep(2);//test
    }

    return NULL;
}

/*清空表数据（tbntopinterface）*/
void MyDBOp_Clear_Interface_Info(/*PMYDBOP pMyDB*/){
    //1.mysql
    //    char sql[]="truncate table tbntopinterface;";
    //    if(!MyDBOp_ExecSQL_1(pMyDB ,sql))
    //        WriteLog("writemysql",sql);
    //

    //3.Es
    //    char  deletepro[128] = {0};
    //    memcpy(deletepro,"curl -XDELETE  'http://localhost:9200/sxzlog-2017.12.09?pretty'",sizeof(deletepro));
    //    system(deletepro);
    //    int status;
    //    status = system(deletepro);
    //    if(status < 0) {
    //        printf("cmd: %s\t error: %s", deletepro, strerror(errno)); // 这里务必要把errno信息输出或记入Log
    //    }
    //    if(WIFEXITED(status)){
    //        printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //取得deletepro执行结果
    //    }else if(WIFSIGNALED(status)){
    //        printf("abnormal termination,signal number =%d\n", WTERMSIG(status)); //如果deletepro被信号中断，取得信号值
    //    }else if(WIFSTOPPED(status)){
    //        printf("process stopped, signal number =%d\n", WSTOPSIG(status)); //如果deletepro被信号暂停执行，取得信号值
    //    }
}
