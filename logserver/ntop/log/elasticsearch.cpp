#include"ntop_includes.h"
#include<time.h>
#include"lib/xshash.h"
#include"sys/mman.h"
//{"index": {"_type": "flows", "_index": "ntopng-2017.11.28"}}

#define  Protocol_Index_Head  "{\"index\": {\"_type\": \"proto\", \"_index\": \"protocol\"}}"
#define  Interface_Index_Head  "{\"index\": {\"_type\": \"ifmsg\", \"_index\": \"interface\"}}"

#define Protocol_Index_Url "http://localhost:9200/protocolex/protocol"
#define Index_Head "sxzlog-%s"

#define ES_MAX_SIZE 1024*1024*2

extern int g_threadnum;
int g_dnsthreadnum = 2;

TypeName typeName[]={
    {Type_URL,"urllog",6500},
    {Type_DNS,"dnslog",2000},
    {Type_ID,"idlog",100},
    {Type_FileDw,"filedwlog",2000},
    {Type_Game,"gamelog",1500},
    {Type_Redirect,"redirectlog",100},
    {Type_Oth,"othlog",3500},
    {Type_ProTo,"proto",600},
    {Type_PicFile,"picfile",2000},
    {Type_PhoneMsg,"phonemsg",1500}
};

ES::ES(){
    //    reportDrops = false;
    elkDroppedFlowsQueueTooLong = 0;
    elkExportedFlows = 0, elkLastExportedFlows = 0;

    int i=0;
    for(i=0;i<Type_Last;i++){
        work_es[i].num_elems = 0;
        work_es[i].head = NULL;
        work_es[i].tail = NULL;
        work_es[i].level = typeName[i].level;
        memcpy(work_es[i].g_es_type,typeName[i].name,strlen(typeName[i].name));
        mmapInfo[i].pMmap = NULL;
        mmapInfo[i].nLen = 0;
        mmapInfo[i].fd = 0;
        mmapInfo[i].nNum = 1;
    }
}

ES::~ES(){
}

/* Send   ntopng index template to Elastic Search */
void ES::pushEStemplate() {
    char *postbuf = NULL, *es_host = NULL;
    char template_path[MAX_PATH], es_template_url[MAX_PATH], es_url[MAX_PATH];
    ifstream template_file;
    u_int8_t max_attempts = 3;
    u_int16_t length = 0;

    // store the original es update url
    strncpy(es_url, ntop->getPrefs()->get_es_url(), MAX_PATH);
    // prepare the template file path...
    snprintf(template_path, sizeof(template_path), "%s/misc/%s",
             ntop->get_docs_dir(), NTOP_ES_TEMPLATE);
    ntop->fixPath(template_path);

    // and the ES url (keep only host and port by retaining only characters left of the first slash)
    if(!strncmp(es_url, "http://", 7)){  // url starts either with http or https
        Utils::tokenizer(es_url + 7, '/', &es_host);
        snprintf(es_template_url, MAX_PATH, "http://%s/_template/ntopng_template", es_host);
    } else if(!strncmp(es_url, "https://", 8)){
        Utils::tokenizer(es_url + 8, '/', &es_host);
        snprintf(es_template_url, MAX_PATH, "https://%s/_template/ntopng_template", es_host);
    } else {
        Utils::tokenizer(es_url, '/', &es_host);
        snprintf(es_template_url, MAX_PATH, "%s/_template/ntopng_template", es_host);
    }

    template_file.open(template_path);   // open input file
    template_file.seekg(0, ios::end);    // go to the end
    length = template_file.tellg();      // report location (this is the length)
    template_file.seekg(0, ios::beg);    // go back to the beginning
    postbuf = new char[length+1];        // allocate memory for a buffer of appropriate dimension
    template_file.read(postbuf, length); // read the whole file into the buffer
    postbuf[length] = '\0';
    if(template_file.is_open())
        template_file.close();            // close file handle

    while(max_attempts > 0) {
        if(!Utils::postHTTPJsonData(ntop->getPrefs()->get_es_user(),
                                    ntop->getPrefs()->get_es_pwd(),
                                    es_template_url, postbuf)){
            /* Post failure */
            // sleep(1);
        } else {
            ntop->getTrace()->traceEvent(TRACE_INFO, "ntopng template successfully sent to ES");
            break;
        }

        max_attempts--;
    } /* while */

    if(postbuf) delete[] postbuf;

    if(max_attempts == 0)
        ntop->getTrace()->traceEvent(TRACE_ERROR, "Unable to send ntopng template (%s) to ES", template_path);
}

int ES::XsDealHttpJsonData(PWorkEs workes,char *postbuf,size_t postbuflen,u_int min_buf_size,int index){
    int nSendNum = 0;
    //     char * type = NULL;

    int nRet = 0;
    if(workes->num_elems) {
        u_int len, num_flows;
        char index_name[64] = {0}, header[128] = {0};
        struct tm* tm_info;
        struct timeval tv;
        time_t t;
        char index_head[32] = {0};

        gettimeofday(&tv, NULL);
        t = tv.tv_sec;
        tm_info = gmtime(&t);

        // strftime(index_name, sizeof(index_name), ntop->getPrefs()->get_es_index(), tm_info);

        if(index == Type_ProTo){
            memset(index_name,0,sizeof(index_name));
            memcpy(index_name,"protocol",strlen("protocol"));
        }else{//es_index = strdup((char*)"sxzlog-%Y.%m.%d")

            strftime(index_head, sizeof(index_head)-1, /*sxzlog-2018.01.05*/ntop->getPrefs()->get_es_index(), tm_info);
            //    type = my_strlwr(typeName[index].name);
            snprintf(index_name,sizeof(index_name)-1,"%s-%s",index_head, typeName[index].name);
        }

        snprintf(header, sizeof(header),
                 "{\"index\": {\"_type\": \"%s\", \"_index\": \"%s\"}}",
                 typeName[index].name,/*"sxzlog-2018.01.12-testlog"*/index_name);
        len = strlen(postbuf);
        num_flows = 0;


        nSendNum = (workes->num_elems >= typeName[index].level) ? typeName[index].level : workes->num_elems ;
        for(u_int i=0; (i<nSendNum) /*&& ((postbuflen-len) > min_buf_size)*/; i++) {
            struct string_list *current = NULL;
            listMutex.lock(__FILE__, __LINE__);
            if(workes->num_elems)
            {
                current = workes->cursor;
                workes->cursor = workes->cursor->prev;
                workes->tail = workes->cursor;
                workes->num_elems--;
                if(workes->num_elems == 0)
                    workes->head = NULL;
                listMutex.unlock(__FILE__, __LINE__);
            }
            else
            {
                listMutex.unlock(__FILE__, __LINE__);
                break;
            }


            len += snprintf(&postbuf[len], postbuflen-len, "%s\n%s\n", header,current->str), num_flows++;
            free(current->str);
            free(current);
            //workes->tail = prev,

            if(len > ES_MAX_SIZE*3/4)
            {
                //postbuf[len] = '\0';
                //msync(postbuf,postbuflen,MS_ASYNC);
                memset(postbuf + len + 1,0,ES_MAX_SIZE - len - 1);
                munmap(postbuf,postbuflen);
                nRet = 1;
                break;
            }
            dequeuepacket++;
        } /* for */

        //postbuf[len] = '\0';



//        if(!Utils::postHTTPJsonData(ntop->getPrefs()->get_es_user(),
//                                    ntop->getPrefs()->get_es_pwd(),
//                                    ntop->getPrefs()->get_es_url(),
//                                    postbuf)) {
//            /* Post failure */
//            //   sleep(1);
//        } else {
//            //ntop->getTrace()->traceEvent(TRACE_INFO, "Sent %u flow(s) to ES",  num_flows);
//            elkExportedFlows += num_flows;
//        }
    }
    return nRet;
}


void GetTimeBuf(char* pBuf,int nLen)
{
    time_t t;
    struct tm* tm_info;
    t = time(0);
    tm_info = localtime(&t);
    strftime(pBuf,nLen,"%Y.%m.%d.%H.%M.%S",tm_info);
}

void ES::indexESdata(logtype flag, char *postbuf, unsigned long len) {
    return ;
    const u_int  min_buf_size = 64*6500;//512
    char szTime[64] = {0};

    char szFileName[256] = {0};
    char szPath[256] = {0};
    GetTimeBuf(szTime,64);
    sprintf(szPath,"./%s",typeName[flag].name);
    mkdir(szPath,777);
    sprintf(szFileName,"./tmp/%s-%s-%d.json.tmp",typeName[flag].name,szTime,rand()%10000);
    int fd = open(szFileName,O_CREAT|O_RDWR);
    lseek(fd,len - 1,SEEK_SET);
    write(fd,"",1);
    char* p_map=(char*)mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p_map == MAP_FAILED)
    {
        return ;
    }
    memset(p_map,0,len);
    while(!ntop->getGlobals()->isShutdown()){
        if(work_es[flag].num_elems)
        {
            //memset(postbuf,0,len);
            int nRet = XsDealHttpJsonData(&work_es[flag],p_map,len/*sizeof(postbuf)*/,min_buf_size,flag);
            if(nRet)
            {
                close(fd);
                char szNewName[256] = {0};
                strcat(szNewName,szPath);
                char* pPos = strrchr(szFileName,'/');
                strncat(szNewName,pPos,strlen(pPos) - 4);
                rename(szFileName,szNewName);

                GetTimeBuf(szTime,64);
                sprintf(szFileName,"./tmp/%s-%s-%d.json.tmp",typeName[flag].name,szTime,rand()%10000);
                fd = open(szFileName,O_CREAT|O_RDWR);
                lseek(fd,len - 1,SEEK_SET);
                write(fd,"",1);
                p_map=(char*)mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
                if(p_map == MAP_FAILED)
                {
                    printf("%d\n",errno);
                    return ;
                }
                memset(p_map,0,len);
            }
        }
        else
            usleep(5000);
    }
}


//test
void ES::XsPostEsData(PESfileInfo info){
    char szPath[256];
    char szTime[64];
    char szFileName[256];
    char szBuf[1024] ;
    char szCmd[1024] ;
    int nCount;
//    int nEnd;
//    if(nType >= 2)
//        nEnd = Type_Last;
//    else
//    {
//        nEnd = nType+1;
//    }
    char szTemp[256] = {0};
    sprintf(szTemp,"start send Es %s",typeName[info->type].name);
    int nSaveSize = 0;
    while(1)
    {
        memset(szPath,0,256);
        memset(szTime,0,64);
        memset(szFileName,0,256);
        memset(szBuf,0,1024);
        memset(szCmd,0,1024);
        nCount = 0;
        //for(int i = nType; i < nEnd; i++)
        {
//            if(Type_FileDw == i)
//                continue;
            nCount = 0;
            //nType = i;
            if(info->type <= Type_DNS)
                sprintf(szPath,OTHLOGPATH"/sxzlog/%s/%d",typeName[info->type].name,info->nNum);
            else
                sprintf(szPath,LOGPATH"/sxzlog/%s",typeName[info->type].name);
            DIR *dir;
            struct dirent *ptr;
            if ((dir=opendir(szPath)) == NULL)
            {
                //printf("Open dir error %s...",szPath);
                sleep(1);
                continue;
            }

            while ((ptr=readdir(dir)) != NULL && nCount++ < 50)
            {
                if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
                    continue;
                else
                {
                    strncpy(szTime,ptr->d_name + strlen(typeName[info->type].name) + 1,10);

                    sprintf(szCmd,"curl -XPOST 'localhost:9200/sxzlog-%s-%s/_bulk' --data-binary  @%s/%s",
                            szTime,typeName[info->type].name,szPath,ptr->d_name);

                    FILE* fp = popen(szCmd,"r");
                    while(!feof(fp))
                    {
                        fgets(szBuf,1024,fp);
                        if(feof(fp))
                            break;
                    }
                    sprintf(szFileName,"%s/%s",szPath,ptr->d_name);
                    //system(szCmd);
                    nCount ++;
                    remove(szFileName);
                    fclose(fp);
                }
            }
            closedir(dir);
        }
        sleep(1);
    }
}



int ES::CreateMmapInfo(MmapInfo* info,int index)
{
    char szTime[64] = {0};
    char szPath[256] = {0};
    GetTimeBuf(szTime,64);
    sprintf(szPath,LOGPATH"/sxzlog/%s",typeName[index].name);


//    mkdir(szPath,777);
//    if(index <= Type_DNS)
//    {
//        sprintf(szPath,LOGPATH"/sxzlog/%s/%d",typeName[index].name,info->nNum);
//    }

    sprintf(info->szFileName,LOGPATH"/sxzlog/tmp/%s-%s-%d.json.tmp",typeName[index].name,szTime,rand()%10000);
    info->fd = open(info->szFileName,O_CREAT|O_RDWR);
    if(info->fd == -1)
    {
        printf("create file %s fail,%d",info->szFileName,errno);
        return 0;
    }
    lseek(info->fd,ES_MAX_SIZE - 1,SEEK_SET);
    write(info->fd,"",1);

    info->pMmap=(char*)mmap(NULL,ES_MAX_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,info->fd,0);
    if(info->pMmap == MAP_FAILED)
    {
        return 0;
    }
    info->nLen = 0;
    memset(info->pMmap,0,ES_MAX_SIZE);
    info->t1 = time(0);
    return 1;
}

int ES::CloseMmapInfo(MmapInfo *info,int index)
{
    memset(info->pMmap + info->nLen + 1,0
           ,ES_MAX_SIZE - info->nLen - 1);
    munmap(info->pMmap,ES_MAX_SIZE);
    close(info->fd);
    char szPath[256] = {0};
    if(index <= Type_DNS)
    {
        sprintf(szPath,LOGPATH"/sxzlog/%s/%d",typeName[index].name,info->nNum);
        int n = 0;
        if(index == Type_DNS)
            n = info->nNum %g_dnsthreadnum;
        if(index == Type_URL)
            n = info->nNum %g_threadnum;
        if(n)
            info->nNum ++;
        else
            info->nNum = 1;
    }
    else
        sprintf(szPath,LOGPATH"/sxzlog/%s",typeName[index].name);
    char szNewName[256] = {0};
    strcat(szNewName,szPath);
    char* pPos = strrchr(info->szFileName,'/');
    strncat(szNewName,pPos,strlen(pPos) - 4);
    rename(info->szFileName,szNewName);
    info->pMmap = NULL;
    info->fd = 0;
    memset(info->szFileName,0,256);

    return 1;
}

int ES::sendToES(char* msg,int typeindex,char *DataBuf) {
//    if(typeindex == Type_FileDw)
//        return -1;
    struct string_list *e;
    int rc = 0;
    if(mmapInfo[typeindex].fd == 0 ||
       mmapInfo[typeindex].pMmap == NULL)
    {
        if(!CreateMmapInfo(&mmapInfo[typeindex],typeindex))
            return -1;
    }

    u_int len, num_flows;
    char index_name[64] = {0}, header[128] = {0};
    struct tm* tm_info;
    struct timeval tv;
    time_t t = time(0);
    char index_head[32] = {0};

    tm_info = localtime(&t);

    // strftime(index_name, sizeof(index_name), ntop->getPrefs()->get_es_index(), tm_info);

    if(typeindex == Type_ProTo){
        memset(index_name,0,sizeof(index_name));
        memcpy(index_name,"protocol",strlen("protocol"));
    }else{//es_index = strdup((char*)"sxzlog-%Y.%m.%d")

        strftime(index_head, sizeof(index_head)-1, /*sxzlog-2018.01.05*/ntop->getPrefs()->get_es_index(), tm_info);
        //    type = my_strlwr(typeName[index].name);
        snprintf(index_name,sizeof(index_name)-1,"%s-%s",index_head, typeName[typeindex].name);
    }
    snprintf(header, sizeof(header),
             "{\"index\": {\"_type\": \"%s\", \"_index\": \"%s\"}}",
             typeName[typeindex].name,/*"sxzlog-2018.01.12-testlog"*/index_name);
    //len = strlen(postbuf);
    num_flows = 0;

    mmapInfo[typeindex].nLen += snprintf(mmapInfo[typeindex].pMmap + mmapInfo[typeindex].nLen,
        ES_MAX_SIZE-mmapInfo[typeindex].nLen, "%s\n%s\n", header,msg), num_flows++;

    if(mmapInfo[typeindex].nLen >(ES_MAX_SIZE - 128*1024) ||
       time(0) - mmapInfo[typeindex].t1 > 2*60)
    {
        CloseMmapInfo(&mmapInfo[typeindex],typeindex);
    }

//  listMutex.lock(__FILE__, __LINE__);

//    e = (struct string_list*)calloc(1, sizeof(struct string_list));
//    if(e != NULL) {
//        e->str = strdup(msg), e->next =work_es[typeindex].head;
//        if(typeindex != Type_ProTo)
//            memcpy(e->dateTime,DataBuf,strlen(DataBuf));//time
//        if(e->str) {
//            if(work_es[typeindex].head/*head*/)
//                work_es[typeindex].head->prev = e;// head->prev = e;
//            work_es[typeindex].head = e;//head = e;
//            if(work_es[typeindex].tail/*tail*/ == NULL)
//            {
//                work_es[typeindex].tail = e; //tail = e;
//                work_es[typeindex].cursor = e;
//            }
//            work_es[typeindex].num_elems++;// num_elems++;

//            enqueuepacket++;//test

//            rc = 0;
//        } else {
//            /* Out of memory */
//            free(e);
//            e = NULL;
//            rc = -1;
//        }
//    } else {
//        mallocfail++;//test
//    }

//    listMutex.unlock(__FILE__, __LINE__);

    return rc;
}

void* esLoop(void* ptr) {
    int type = *(int*)ptr;
    delete ptr;
    prctl(PR_SET_NAME,"LogESLoop");
    //ntop->getES()->pushEStemplate();  // sends ES ntopng template
    char *postbuf = NULL;
    ntop->elasticsearch->indexESdata((logtype)(type),postbuf,ES_MAX_SIZE);
    //free(postbuf);
    return(NULL);
}

void* PostLogToEs(void* ptr)
{
    //if(ntype == Type_URL)
    ntop->elasticsearch->XsPostEsData((PESfileInfo)ptr);

    return NULL;
}


void ES::startEsDump(){
    int nEnd;
    int i = Type_URL ;
    for(; i < Type_Last;i++)
    {
        int j = 1;
        if(i == Type_URL)
            nEnd = g_threadnum;
        else if(i == Type_DNS)
            nEnd = g_dnsthreadnum;
        else
        {
            j = i;
            nEnd = i;
        }

        for(; j <= nEnd;j++)
        {
            PESfileInfo info = new ESfileInfo;
            info->type = i;
            info->nNum = j;
            pthread_t esThreadLoop1;
            pthread_create(&esThreadLoop1, NULL, PostLogToEs,(void*)info);
        }


    }
    //pthread_t esThreadLoop1;
    //pthread_create(&esThreadLoop1, NULL, PostLogToEs,NULL);
}


json_object* flow2json(PParseFiled pParseFiled,char * DataBuf) {
    json_object *my_object;
    char jsonbuf[64];
    char dateTime[32] = {0};
    u_int32_t sip=0,dip=0,snatip=0;

    if((my_object = json_object_new_object()) == NULL) return(NULL);

    json_object_object_add(my_object, Utils::jsonLabel(0, "node", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufLabel));

    json_object_object_add(my_object, Utils::jsonLabel(0, "usr", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->szUsr));


    sip=atol(pParseFiled->bufSip);
    //    sip=strtoul(pParseFiled->bufSip,NULL,10);
    json_object_object_add(my_object, Utils::jsonLabel(0, "srcip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(sip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "srcport", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->nSport));

    dip=atol(pParseFiled->bufDip);
    //    dip=strtoul(pParseFiled->bufDip,NULL,10);
    json_object_object_add(my_object, Utils::jsonLabel(0, "dstip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(dip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "dstport", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->nDport));

    json_object_object_add(my_object, Utils::jsonLabel(0, "mac", jsonbuf, sizeof(jsonbuf)),//0 test
                           json_object_new_string(pParseFiled->bufSmac));

    json_object_object_add(my_object, Utils::jsonLabel(0, "act", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufMethod));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data1", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufUrl));

    json_object_object_add(my_object, Utils::jsonLabel(0, "flag", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(1));

    int l7= atoi(pParseFiled->L7);
    json_object_object_add(my_object, Utils::jsonLabel(0, "l7",jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(l7));

    json_object_object_add(my_object, Utils::jsonLabel(0, "type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "log_type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "proto", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufProto));

    //    json_object_object_add(my_object, Utils::jsonLabel(0, "date", jsonbuf, sizeof(jsonbuf)),
    //                           json_object_new_int(pParseFiled->bufDate));

    //    json_object_object_add(my_object, Utils::jsonLabel(0, "time", jsonbuf, sizeof(jsonbuf)),
    //                           json_object_new_string(pParseFiled->bufTime));

    //    char DataBuf[16] = {0};
    //    XsGet_Format_Date(pParseFiled->szbufDate,DataBuf);
    snprintf(dateTime,sizeof(dateTime),"%s %s",DataBuf,pParseFiled->bufTime);
    json_object_object_add(my_object, Utils::jsonLabel(0, "time", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(GetTick(dateTime)));

    snatip=atol(pParseFiled->bufSnat_ip);
    //    snatip=strtoul(pParseFiled->bufSnat_ip,NULL,10);
    json_object_object_add(my_object, Utils::jsonLabel(0, "snat_ip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(snatip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "snat_port", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->nSnat_port));

    json_object_object_add(my_object, Utils::jsonLabel(0, "country", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szCountry));

    json_object_object_add(my_object, Utils::jsonLabel(0, "province", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szSubdivisions));

    json_object_object_add(my_object, Utils::jsonLabel(0, "city", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szCity));

    char szLocal[128] = {0};
    sprintf(szLocal,"%g-%g",pParseFiled->localinfo.latitude,pParseFiled->localinfo.longitude);

//    json_object_object_add(my_object, Utils::jsonLabel(0, "local", jsonbuf, sizeof(jsonbuf)),
//                           json_object_new_string(szLocal));

    json_object_object_add(my_object, Utils::jsonLabel(0, "latitude", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_double(pParseFiled->localinfo.latitude));

    json_object_object_add(my_object, Utils::jsonLabel(0, "longitude", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_double(pParseFiled->localinfo.longitude));

    json_object_object_add(my_object, Utils::jsonLabel(0, "os", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data5", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data6", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data7", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data8", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data9", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data10", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    return(my_object);
}

json_object* flow2json_forIDLog(PParseFiled pParseFiled,char *DataBuf){
    json_object *my_object;
    char jsonbuf[64];
    char dateTime[32] = {0};
    u_int32_t sip=0;

    if((my_object = json_object_new_object()) == NULL) return(NULL);

    json_object_object_add(my_object, Utils::jsonLabel(0, "node", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufLabel));

    json_object_object_add(my_object, Utils::jsonLabel(0, "usr", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->szUsr));

    sip=atol(pParseFiled->bufSip);
    json_object_object_add(my_object, Utils::jsonLabel(0, "srcip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(sip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "mac", jsonbuf, sizeof(jsonbuf)),//0 test
                           json_object_new_string(pParseFiled->bufSmac));

    json_object_object_add(my_object, Utils::jsonLabel(0, "act", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufMethod));

    json_object_object_add(my_object, Utils::jsonLabel(0, "type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "log_type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data5", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data6", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data7", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data8", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    //    char DataBuf[16] = {0};
    //    XsGet_Format_Date(pParseFiled->szbufDate,DataBuf);
    snprintf(dateTime,sizeof(dateTime),"%s %s",DataBuf,pParseFiled->bufTime);
    json_object_object_add(my_object, Utils::jsonLabel(0, "time", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(GetTick(dateTime)));

    return(my_object);
}

json_object* flow2json_forFileDwLog(PParseFiled pParseFiled,pAccountInfo accountInfo,char *DataBuf){
    json_object *my_object;
    char jsonbuf[64];
    char dateTime[32] = {0};
    u_int64_t hashkey = 0;
    u_int32_t sip=0,dip=0;

    if((my_object = json_object_new_object()) == NULL) return(NULL);

    sip=atol(pParseFiled->bufSip);
    json_object_object_add(my_object, Utils::jsonLabel(0, "srcip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(sip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "srcport", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->nSport));

    dip=atol(pParseFiled->bufDip);
    json_object_object_add(my_object, Utils::jsonLabel(0, "dstip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(dip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "dstport", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->nDport));

    json_object_object_add(my_object, Utils::jsonLabel(0, "mac", jsonbuf, sizeof(jsonbuf)),//0 test
                           json_object_new_string(pParseFiled->bufSmac));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data1", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufUrl));

    json_object_object_add(my_object, Utils::jsonLabel(0, "account", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(accountInfo->id));

    hashkey=CRCInt64((const u_char *)pParseFiled->bufUrl,strlen(pParseFiled->bufUrl));
    json_object_object_add(my_object, Utils::jsonLabel(0, "filename", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(hashkey));

    json_object_object_add(my_object, Utils::jsonLabel(0, "type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(accountInfo->nametype));

    json_object_object_add(my_object, Utils::jsonLabel(0, "log_type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "os", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "flag", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(1));

    json_object_object_add(my_object, Utils::jsonLabel(0, "country", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szCountry));

    json_object_object_add(my_object, Utils::jsonLabel(0, "province", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szSubdivisions));

    json_object_object_add(my_object, Utils::jsonLabel(0, "city", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szCity));

    json_object_object_add(my_object, Utils::jsonLabel(0, "latitude", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_double(pParseFiled->localinfo.latitude));

    json_object_object_add(my_object, Utils::jsonLabel(0, "longitude", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_double(pParseFiled->localinfo.longitude));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data5", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data6", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data7", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data8", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    //    char DataBuf[16] = {0};
    //    XsGet_Format_Date(pParseFiled->szbufDate,DataBuf);
    snprintf(dateTime,sizeof(dateTime),"%s %s",DataBuf,pParseFiled->bufTime);
    json_object_object_add(my_object, Utils::jsonLabel(0, "time", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(GetTick(dateTime)));

    return(my_object);
}
json_object* flow2json_forPhoneMsg(PParseFiled pParseFiled,pPhoneMsg phoneMsg,char * DataBuf){
    json_object *my_object;
    char jsonbuf[64];
    char dateTime[32] = {0};

    if((my_object = json_object_new_object()) == NULL) return(NULL);

    json_object_object_add(my_object, Utils::jsonLabel(0, "node", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufLabel));

    json_object_object_add(my_object, Utils::jsonLabel(0, "usr", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->szUsr));

    json_object_object_add(my_object, Utils::jsonLabel(0, "type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "log_type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "mac", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufSmac));

    json_object_object_add(my_object, Utils::jsonLabel(0, "ipaddress", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(strlen(pParseFiled->bufSip) ? pParseFiled->bufSip : "0"));

    json_object_object_add(my_object, Utils::jsonLabel(0, "devicetype", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(phoneMsg->device_type));

    json_object_object_add(my_object, Utils::jsonLabel(0, "ssid", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(phoneMsg->wifi_ssid));

    json_object_object_add(my_object, Utils::jsonLabel(0, "devicebrand", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(phoneMsg->device_brand));

    json_object_object_add(my_object, Utils::jsonLabel(0, "bssid", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(phoneMsg->bssid));

    json_object_object_add(my_object, Utils::jsonLabel(0, "imei", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(phoneMsg->imei));

    json_object_object_add(my_object, Utils::jsonLabel(0, "imsi", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(phoneMsg->imsi));

    //    char DataBuf[16] = {0};
    //    XsGet_Format_Date(pParseFiled->szbufDate,DataBuf);
    snprintf(dateTime,sizeof(dateTime),"%s %s",DataBuf,pParseFiled->bufTime);
    json_object_object_add(my_object, Utils::jsonLabel(0, "time", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(GetTick(dateTime)));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data5", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data6", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data7", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data8", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    return(my_object);
}
json_object* flow2json_forPicFileLog(PParseFiled pParseFiled,char *DataBuf) {
    json_object *my_object;
    char jsonbuf[64];
    char dateTime[32] = {0};
    u_int32_t sip=0,dip=0;

    if((my_object = json_object_new_object()) == NULL) return(NULL);

    json_object_object_add(my_object, Utils::jsonLabel(0, "node", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufLabel));

    json_object_object_add(my_object, Utils::jsonLabel(0, "usr", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->szUsr));

    sip=atol(pParseFiled->bufSip);
    json_object_object_add(my_object, Utils::jsonLabel(0, "srcip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(sip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "srcport", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->nSport));

    dip=atol(pParseFiled->bufDip);
    json_object_object_add(my_object, Utils::jsonLabel(0, "dstip", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(dip));

    json_object_object_add(my_object, Utils::jsonLabel(0, "dstport", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->nDport));

    json_object_object_add(my_object, Utils::jsonLabel(0, "mac", jsonbuf, sizeof(jsonbuf)),//0 test
                           json_object_new_string(pParseFiled->bufSmac));

    json_object_object_add(my_object, Utils::jsonLabel(0, "act", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufMethod));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data1", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufUrl));

    json_object_object_add(my_object, Utils::jsonLabel(0, "type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "log_type", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufType));

    json_object_object_add(my_object, Utils::jsonLabel(0, "proto", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->bufProto));

    json_object_object_add(my_object, Utils::jsonLabel(0, "flagpic", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pParseFiled->flagpic));

    json_object_object_add(my_object, Utils::jsonLabel(0, "softname", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->softname));

    //    char DataBuf[16] = {0};
    //    XsGet_Format_Date(pParseFiled->szbufDate,DataBuf);
    snprintf(dateTime,sizeof(dateTime),"%s %s",DataBuf,pParseFiled->bufTime);
    json_object_object_add(my_object, Utils::jsonLabel(0, "time", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int64(GetTick(dateTime)));

    json_object_object_add(my_object, Utils::jsonLabel(0, "country", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szCountry));

    json_object_object_add(my_object, Utils::jsonLabel(0, "province", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szSubdivisions));

    json_object_object_add(my_object, Utils::jsonLabel(0, "city", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pParseFiled->localinfo.szCity));

    json_object_object_add(my_object, Utils::jsonLabel(0, "latitude", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_double(pParseFiled->localinfo.latitude));

    json_object_object_add(my_object, Utils::jsonLabel(0, "longitude", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_double(pParseFiled->localinfo.longitude));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data5", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data6", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data7", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data8", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    return(my_object);
}

char*  serialize(PParseFiled pParseFiled,pAccountInfo accountInfo,pPhoneMsg phoneMsg,char * DataBuf) {
    json_object *my_object;
    char *rsp;

    ntop->getPrefs()->set_json_symbolic_labels_format(true);
    if(0 == memcmp(pParseFiled->bufType,"idlog",strlen("idlog"))){
        my_object = flow2json_forIDLog(pParseFiled,DataBuf);
    }else if(0 == memcmp(pParseFiled->bufType,"filedwlog",strlen("filedwlog"))){
        my_object = flow2json_forFileDwLog(pParseFiled,accountInfo,DataBuf);
    }else if(0 == memcmp(pParseFiled->bufType,"picfile",strlen("picfile"))){
        my_object = flow2json_forPicFileLog(pParseFiled,DataBuf);
    }else if(0 == memcmp(pParseFiled->bufType,"phonemsg",strlen("phonemsg"))){
        my_object = flow2json_forPhoneMsg(pParseFiled,phoneMsg,DataBuf);
    }else{
        my_object = flow2json(pParseFiled,DataBuf);
    }

    if(my_object != NULL) {
        /* JSON string */
        rsp = strdup(json_object_to_json_string(my_object));
        /* Free memory */
        json_object_put(my_object);
    } else
        rsp = NULL;

    return(rsp);
}

enum logtype XsGet_TypeIndex(char *type){
    int i=0;
    for(i=0;i<Type_Last;i++){
        if(0 == strncasecmp(type,typeName[i].name,strlen(type)))
            return typeName[i].index;
    }
    return Type_Oth;
}

int ES::dumpEsFlowDpi(PParseFiled pParseFiled,pAccountInfo accountInfo,pPhoneMsg phoneMsg){
    char DataBuf[16] = {0};
    XsGet_Format_Date(pParseFiled->szbufDate,DataBuf);
    char *json = serialize(pParseFiled,accountInfo,phoneMsg,DataBuf);
    int rc;
    int typeIndex;
    if(json) {

        // ntop->getTrace()->traceEvent(TRACE_INFO, "[ES] %s", json);
        typeIndex=XsGet_TypeIndex(pParseFiled->bufType);
        if(typeIndex == Type_Oth)
            printf("%s\n",json);
        listMutex.lock(__FILE__,__LINE__);
        rc = ntop->getES()->sendToES(json,typeIndex,DataBuf);
        listMutex.unlock(__FILE__,__LINE__);
        rc = 0;
        free(json);
    } else
        rc = -1;

    return(rc);
}

json_object* proto2json(PPROTO pProto,char *pTag){
    json_object *my_object;
    char jsonbuf[64];

    if((my_object = json_object_new_object()) == NULL) return(NULL);

    // char tmp[64]={0};
    //g2u(pProto->szName,strlen(pProto->szName),tmp,64);

    json_object_object_add(my_object, Utils::jsonLabel(0, "id", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nId));

    json_object_object_add(my_object, Utils::jsonLabel(0, "name", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pProto->szName));

    /*json_object_object_add(my_object, Utils::jsonLabel(0, "level", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nLevel));

    json_object_object_add(my_object, Utils::jsonLabel(0, "protocol", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nProtocol));

    json_object_object_add(my_object, Utils::jsonLabel(0, "root", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nRoot));

    json_object_object_add(my_object, Utils::jsonLabel(0, "parentid", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nParent));

    json_object_object_add(my_object, Utils::jsonLabel(0, "sztag", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(pTag));

    json_object_object_add(my_object, Utils::jsonLabel(0, "ttl", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nTTL));

    json_object_object_add(my_object, Utils::jsonLabel(0, "switch", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nSwitch));

    json_object_object_add(my_object, Utils::jsonLabel(0, "field", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nField));

    json_object_object_add(my_object, Utils::jsonLabel(0, "nField0", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nField0));

    json_object_object_add(my_object, Utils::jsonLabel(0, "nField1", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nField1));

    json_object_object_add(my_object, Utils::jsonLabel(0, "nField2", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nField2));

    json_object_object_add(my_object, Utils::jsonLabel(0, "nField3", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(pProto->nField3));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data5", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data6", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data7", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data8", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));*/

    return(my_object);

}

void XsPost_Proto_To_Es(PPROTO pProto,char *pTag){
    json_object *my_object;
    char *rsp = NULL;

    ntop->getPrefs()->set_json_symbolic_labels_format(true);

    my_object = proto2json(pProto,pTag);
    if(my_object != NULL) {

        /* JSON string */
        rsp = strdup(json_object_to_json_string(my_object));

        /* Free memory */
        json_object_put(my_object);
    } else
        rsp = NULL;


    char postbuf[1024] = {0};
    snprintf(postbuf, 1024 - 1, "%s\n%s\n", Protocol_Index_Head,rsp);
    if(!Utils::postHTTPJsonData(ntop->getPrefs()->get_es_user(),
                                ntop->getPrefs()->get_es_pwd(),
                                ntop->getPrefs()->get_es_url()/*Protocol_Index_Url*/, postbuf)){
        /* Post failure */
        WriteLog("PostEs",postbuf); // sleep(1);
    }else {
        ntop->getTrace()->traceEvent(TRACE_INFO, "ntopng template successfully sent to ES");
    }

    free(rsp);
}

char*  serializePro(PPROTO pProto,char *pTag){
    json_object *my_object;
    char *rsp = NULL;

    ntop->getPrefs()->set_json_symbolic_labels_format(true);

    my_object = proto2json(pProto,pTag);

    if(my_object != NULL) {

        /* JSON string */
        rsp = strdup(json_object_to_json_string(my_object));

        /* Free memory */
        json_object_put(my_object);
    } else
        rsp = NULL;

    return(rsp);
}

//更新tbprotocolex
int  UpdateProtocolex(PPROTO pProto,char *pTag,FILE* fp)
{
    // XsPost_Proto_To_Es(pProto,pTag);
    char *json = serializePro(pProto,pTag);
    int rc;
    if(json) {

        ntop->getTrace()->traceEvent(TRACE_INFO, "[ES] %s", json);

        //rc = ntop->getES()->sendToES(json,Type_ProTo,NULL);
        free(json);
    } else
        rc = -1;

    return(rc);
}

void XsDeleteIndexForProtocol(){
    char  deletepro[128] = {0};
    memcpy(deletepro,"curl -XDELETE  'http://localhost:9200/protocol?pretty'",sizeof(deletepro));
    system(deletepro);

    int status;
    status = system(deletepro);
    if(status < 0) {
        printf("cmd: %s\t error: %s", deletepro, strerror(errno)); // 这里务必要把errno信息输出或记入Log
    }
    //    if(WIFEXITED(status)){
    //     //   printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //取得deletepro执行结果
    //    }else if(WIFSIGNALED(status)){
    //        printf("abnormal termination,signal number =%d\n", WTERMSIG(status)); //如果deletepro被信号中断，取得信号值
    //    }else if(WIFSTOPPED(status)){
    //        printf("process stopped, signal number =%d\n", WSTOPSIG(status)); //如果deletepro被信号暂停执行，取得信号值
    //    }
}
//
//void DeleteSpecialProcotol(char * pTag){

//    char postbuf[256] = {0};

//    char query[168] = {0};

//    snprintf(query,strlen(query) - 1,"");

//    snprintf(postbuf,strlen(postbuf) - 1, "%s\n%s\n",Protocol_Index_Head,query);

//    if(!Utils::postHTTPJsonData(ntop->getPrefs()->get_es_user(),
//                                ntop->getPrefs()->get_es_pwd(),
//                                ntop->getPrefs()->get_es_url(), postbuf)){
//        /* Post failure */
//        // sleep(1);
//    }else {
//        ntop->getTrace()->traceEvent(TRACE_INFO, "ntopng template successfully sent to ES");
//    }
//}

json_object *interface2json(char *ifname,int ifid){
    json_object *my_object;
    char jsonbuf[64];

    if((my_object = json_object_new_object()) == NULL) return(NULL);

    json_object_object_add(my_object, Utils::jsonLabel(0, "ifName", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(ifname));

    json_object_object_add(my_object, Utils::jsonLabel(0, "ifID", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_int(ifid));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data7", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data8", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data9", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    json_object_object_add(my_object, Utils::jsonLabel(0, "data10", jsonbuf, sizeof(jsonbuf)),
                           json_object_new_string(" "));

    return (my_object);

}

//void XsPostInterfaceToES(char *ifname,int ifid){
//    json_object *my_object;
//    char *rsp = NULL;
//    ntop->getPrefs()->set_json_symbolic_labels_format(true);
//    my_object = interface2json(ifname,ifid);
//    if(my_object != NULL) {
//        /* JSON string */
//        rsp = strdup(json_object_to_json_string(my_object));
//        /* Free memory */
//        json_object_put(my_object);
//    } else
//        rsp = NULL;

//    char postbuf[256] = {0};
//    snprintf(postbuf, sizeof(postbuf) - 1, "%s\n%s\n", Interface_Index_Head,rsp);
//    if(!Utils::postHTTPJsonData(ntop->getPrefs()->get_es_user(),
//                                ntop->getPrefs()->get_es_pwd(),
//                                ntop->getPrefs()->get_es_url()/*Protocol_Index_Url*/, postbuf)){
//        /* Post failure */
//        WriteLog("PostEs",postbuf); // sleep(1);
//    }else {
//        ntop->getTrace()->traceEvent(TRACE_INFO, "ntopng interface successfully sent to ES");
//    }
//    free(rsp);
//}



