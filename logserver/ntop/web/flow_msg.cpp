#include"ntop_includes.h"
//#include"web/web_interaction.h"





void Flow::Ret_getJSONObject_Flow(json_object *pArray,u_int64_t numFlows){
    json_object *pItem = json_object_new_object();
    char buf[64];
    Host *src = get_cli_host(), *dst = get_srv_host();
    bool src_match = true, dst_match = true;
    bool mask_cli_host = true, mask_dst_host = true, mask_flow;

    if((src == NULL) || (dst == NULL)) return;

    //    if(ptree) {
    //        src_match = src->match(ptree), dst_match = dst->match(ptree);
    //        if((!src_match) && (!dst_match)) return;
    //    }
    //总的条目
    json_object_object_add(pItem, "numFlows", json_object_new_int64(numFlows));

    if(src) {
        mask_cli_host = Utils::maskHost(src->isLocalHost());
        json_object_object_add(pItem, "cli.ip",json_object_new_string(src->get_ip()->printMask(buf, sizeof(buf),src->isLocalHost())));
        json_object_object_add(pItem, "cli.key", json_object_new_int64(mask_cli_host ? 0 : src->key()));
    }
    json_object_object_add(pItem, "cli.port", json_object_new_int(get_cli_port()));

    if(dst){
        mask_dst_host = Utils::maskHost(dst->isLocalHost());
        json_object_object_add(pItem, "srv.ip",json_object_new_string(dst->get_ip()->printMask(buf, sizeof(buf),dst->isLocalHost())));
        json_object_object_add(pItem,  "srv.key", json_object_new_int64(mask_dst_host ? 0 : dst->key()));
    }
    json_object_object_add(pItem, "srv.port", json_object_new_int(get_srv_port()));

    mask_flow = isMaskedFlow(); // mask_cli_host || mask_dst_host;

    json_object_object_add(pItem, "bytes", json_object_new_int(cli2srv_bytes+srv2cli_bytes));
    json_object_object_add(pItem, "goodput_bytes", json_object_new_int(cli2srv_goodput_bytes+srv2cli_goodput_bytes));

    if(src && !mask_cli_host) {
        json_object_object_add(pItem, "cli.host", json_object_new_string(src->get_visual_name(buf, sizeof(buf))));
        json_object_object_add(pItem, "cli.source_id", json_object_new_int(src->getSourceId()));
        json_object_object_add(pItem, "cli.mac",json_object_new_string(Utils::formatMac(src->get_mac(), buf, sizeof(buf))) );

        json_object_object_add(pItem, "cli.systemhost", json_object_new_boolean(src->isSystemHost()));
        json_object_object_add(pItem, "cli.allowed_host", json_object_new_boolean(src_match));
        json_object_object_add(pItem, "cli.network_id", json_object_new_int(src->get_local_network_id()));
        json_object_object_add(pItem,"cli.pool_id", json_object_new_int(src->get_host_pool()));
    }

    if(dst && !mask_dst_host) {
        json_object_object_add(pItem, "srv.host", json_object_new_string(dst->get_visual_name(buf, sizeof(buf))));
        json_object_object_add(pItem, "srv.source_id", json_object_new_int(src->getSourceId()));
        json_object_object_add(pItem, "srv.mac",json_object_new_string(Utils::formatMac(dst->get_mac(), buf, sizeof(buf))) );

        json_object_object_add(pItem ,"srv.systemhost",json_object_new_boolean(dst->isSystemHost()));
        json_object_object_add(pItem, "srv.allowed_host", json_object_new_boolean(dst_match));
        json_object_object_add(pItem, "srv.network_id", json_object_new_int(dst->get_local_network_id()));
        json_object_object_add(pItem, "srv.pool_id",json_object_new_int(dst->get_host_pool()) );
    }

    if(vrfId)  json_object_object_add(pItem, "vrfId", json_object_new_int(vrfId));
    json_object_object_add(pItem, "vlan", json_object_new_int(get_vlan_id()));
    json_object_object_add(pItem, "proto.l4", json_object_new_string(get_protocol_name()));

    /*~~~~~~byzpl~~~~~~~~*/
    switch (virtualnum.type){
    case virtual_weixin:
    {
        ndpiDetectedProtocol.master_protocol=NDPI_PROTOCOL_WECHAT;
        ndpiDetectedProtocol.app_protocol=NDPI_PROTOCOL_WECHAT;
        json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));
    }
        break;
    case virtual_qq:
    {
        ndpiDetectedProtocol.master_protocol=NDPI_PROTOCOL_OICQ;
        ndpiDetectedProtocol.app_protocol=NDPI_PROTOCOL_OICQ;
        json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));//...
    }
        break;
    case virtual_weibo:
    {
        ndpiDetectedProtocol.master_protocol=NDPI_PROTOCOL_WEIBO;
        ndpiDetectedProtocol.app_protocol=NDPI_PROTOCOL_WEIBO;
        json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));//...
    }
        break;
    case virtual_wangwang:
    {
        json_object_object_add(pItem, "proto.ndpi",json_object_new_string("AliWangWang"));//...
    }
        break;
    case virtual_mail:
    {
        if(virtualnum.proType==mail_imap){
            ndpiDetectedProtocol.master_protocol=NDPI_PROTOCOL_MAIL_IMAP;
            ndpiDetectedProtocol.app_protocol=NDPI_PROTOCOL_MAIL_IMAP;
            json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));//...
        }else if(virtualnum.proType==mail_pop){
            ndpiDetectedProtocol.master_protocol=NDPI_PROTOCOL_MAIL_POP;
            ndpiDetectedProtocol.app_protocol=NDPI_PROTOCOL_MAIL_POP;
            json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));//...
        }else if(virtualnum.proType==mail_smtp){
            ndpiDetectedProtocol.master_protocol=NDPI_PROTOCOL_MAIL_SMTP;
            ndpiDetectedProtocol.app_protocol=NDPI_PROTOCOL_MAIL_SMTP;
            json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));//...
        }else if(virtualnum.proType==mail_sina){
            ndpiDetectedProtocol.master_protocol=NDPI_PROTOCOL_SINA;
            ndpiDetectedProtocol.app_protocol=NDPI_PROTOCOL_SINA;
            json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));//...
        }else
            break;
    }
        break;
    default:
    {
        if(((cli2srv_packets+srv2cli_packets) > NDPI_MIN_NUM_PACKETS)
                || (ndpiDetectedProtocol.app_protocol != NDPI_PROTOCOL_UNKNOWN)
                || iface->is_ndpi_enabled()
                || iface->isSampledTraffic()
                || iface->is_sprobe_interface()
                || (!strcmp(iface->get_type(), CONST_INTERFACE_TYPE_ZMQ))
                || (!strcmp(iface->get_type(), CONST_INTERFACE_TYPE_ZC_FLOW))) {
            json_object_object_add(pItem, "proto.ndpi",json_object_new_string(get_detected_protocol_name(buf, sizeof(buf))));//.........application
        } else
            json_object_object_add(pItem, "proto.ndpi",json_object_new_string((char*)CONST_TOO_EARLY)) ;}
        break;
    }

    json_object_object_add(pItem, "proto.ndpi_id", json_object_new_int(ndpiDetectedProtocol.app_protocol));//...搜索栏application
    json_object_object_add(pItem, "proto.ndpi_breed", json_object_new_string(get_protocol_breed_name()));

    json_object_object_add(pItem, "bytes.last",
                           json_object_new_int64(get_current_bytes_cli2srv() + get_current_bytes_srv2cli()) );
    json_object_object_add(pItem, "goodput_bytes",
                           json_object_new_int64(cli2srv_goodput_bytes+srv2cli_goodput_bytes));
    json_object_object_add(pItem, "goodput_bytes.last",
                           json_object_new_int64(get_current_goodput_bytes_cli2srv() + get_current_goodput_bytes_srv2cli()));
    json_object_object_add(pItem, "packets", json_object_new_int64(cli2srv_packets+srv2cli_packets));
    json_object_object_add(pItem, "packets.last",
                           json_object_new_int64(get_current_packets_cli2srv() + get_current_packets_srv2cli()));
    json_object_object_add(pItem, "seen.first", json_object_new_int64(get_first_seen()));
    json_object_object_add(pItem, "seen.last", json_object_new_int64(get_last_seen()));
    json_object_object_add(pItem, "duration", json_object_new_int64(get_duration()));

    json_object_object_add(pItem, "cli2srv.bytes", json_object_new_int64(cli2srv_bytes));
    json_object_object_add(pItem, "srv2cli.bytes", json_object_new_int64(srv2cli_bytes));
    json_object_object_add(pItem, "cli2srv.goodput_bytes", json_object_new_int64(cli2srv_goodput_bytes));
    json_object_object_add(pItem, "srv2cli.goodput_bytes", json_object_new_int64(srv2cli_goodput_bytes));
    json_object_object_add(pItem, "cli2srv.packets", json_object_new_int(cli2srv_packets));
    json_object_object_add(pItem, "srv2cli.packets", json_object_new_int(srv2cli_packets));

    if(isICMP()) {
        json_object_object_add(pItem, "type", json_object_new_int(protos.icmp.icmp_type));
        json_object_object_add(pItem, "code", json_object_new_int(protos.icmp.icmp_code));
    }

    json_object_object_add(pItem, "flow_goodput.low", json_object_new_boolean(isLowGoodput()));
    json_object_object_add(pItem, "dump.disk", json_object_new_boolean(getDumpFlowTraffic()));

    if(protocol == IPPROTO_TCP) {
        json_object_object_add(pItem, "tcp.seq_problems",
                               json_object_new_boolean((tcp_stats_s2d.pktRetr
                                                        | tcp_stats_s2d.pktOOO
                                                        | tcp_stats_s2d.pktLost
                                                        | tcp_stats_d2s.pktRetr
                                                        | tcp_stats_d2s.pktOOO
                                                        | tcp_stats_d2s.pktLost) ? true : false) );

        json_object_object_add(pItem, "tcp.nw_latency.client", json_object_new_double(toMs(&clientNwLatency)));
        json_object_object_add(pItem, "tcp.nw_latency.server", json_object_new_double(toMs(&serverNwLatency)));
        json_object_object_add(pItem, "tcp.appl_latency", json_object_new_double(applLatencyMsec));
        json_object_object_add(pItem, "tcp.max_thpt.cli2srv", json_object_new_double(getCli2SrvMaxThpt()));
        json_object_object_add(pItem, "tcp.max_thpt.srv2cli", json_object_new_double(getSrv2CliMaxThpt()));

        json_object_object_add(pItem, "cli2srv.retransmissions", json_object_new_int(tcp_stats_s2d.pktRetr));
        json_object_object_add(pItem, "cli2srv.out_of_order", json_object_new_int(tcp_stats_s2d.pktOOO));
        json_object_object_add(pItem, "cli2srv.lost", json_object_new_int(tcp_stats_s2d.pktLost));
        json_object_object_add(pItem, "srv2cli.retransmissions", json_object_new_int(tcp_stats_d2s.pktRetr));
        json_object_object_add(pItem, "srv2cli.out_of_order", json_object_new_int(tcp_stats_d2s.pktOOO));
        json_object_object_add(pItem, "srv2cli.lost", json_object_new_int(tcp_stats_d2s.pktLost));

        json_object_object_add(pItem, "cli2srv.tcp_flags", json_object_new_int(src2dst_tcp_flags));
        json_object_object_add(pItem, "srv2cli.tcp_flags", json_object_new_int(dst2src_tcp_flags));

        json_object_object_add(pItem, "tcp_established", json_object_new_boolean(isEstablished()));
    }

    char buffer[1024]={0};
    if(wechat.id){
        memcpy(buffer,wechat.id,MIN(strlen(wechat.id),sizeof(buffer)-1));
        json_object_object_add(pItem, "info",json_object_new_string(buffer));
    }
    else if(qq.id){
        memcpy(buffer,qq.id,MIN(strlen(qq.id),sizeof(buffer)-1));
        json_object_object_add(pItem, "info",json_object_new_string(buffer));
    }
    else if(weibo.id){
        memcpy(buffer,weibo.id,MIN(strlen(weibo.id),sizeof(buffer)-1));
        json_object_object_add(pItem, "info",json_object_new_string(buffer));
    }
    else if(wangwang.id){
        memcpy(buffer,wangwang.id,MIN(strlen(wangwang.id),sizeof(buffer)-1));
        json_object_object_add(pItem, "info",json_object_new_string(buffer));
    }
    else if(e_mail.mailId){
        memcpy(buffer,e_mail.mailId,MIN(strlen(e_mail.mailId),sizeof(buffer)-1));
        json_object_object_add(pItem, "info",json_object_new_string(buffer));
    }
    size_t buflen=0;
    buflen=strlen(buffer);
    if(buflen==0&&!mask_flow){
        if(host_server_name) json_object_object_add(pItem, "host_server_name", json_object_new_string(host_server_name));
        if(bt_hash)          json_object_object_add(pItem, "bittorrent_hash", json_object_new_string(bt_hash));
        getFlowInfo(buffer,sizeof(buffer)-1);
        json_object_object_add(pItem, "info",json_object_new_string(buffer));
    }

    if(isHTTP() && protos.http.last_method && protos.http.last_url) {
        json_object_object_add(pItem, "protos.http.last_method",json_object_new_string(protos.http.last_method));
        json_object_object_add(pItem, "protos.http.last_return_code", json_object_new_int(protos.http.last_return_code));
    }

    if(!mask_flow) {
        if(isHTTP() && protos.http.last_method && protos.http.last_url)
            json_object_object_add(pItem, "protos.http.last_url", json_object_new_string(protos.http.last_url));//......
        /*~~~~~~byzpl~~~~*/
        if(isWECHAT()&&wechat.id)
            json_object_object_add(pItem, "protos.wechat.id", json_object_new_string(wechat.id));
        if(isQQ()&&qq.id)
            json_object_object_add(pItem, "protos.qq.id", json_object_new_string(qq.id));
        if(isWEIBO()&&weibo.id)
            json_object_object_add(pItem, "protos.weibo.id", json_object_new_string(weibo.id));
        if(e_mail.mailId)
            json_object_object_add(pItem, "protos.email.id", json_object_new_string(e_mail.mailId));
        if(wangwang.id)
              json_object_object_add(pItem, "protos.wangwang.id", json_object_new_string(wangwang.id));
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        if(host_server_name && (!mask_flow))
            json_object_object_add(pItem, "protos.http.server_name", json_object_new_string(host_server_name));

        if(isDNS() && protos.dns.last_query)
            json_object_object_add(pItem, "protos.dns.last_query", json_object_new_string(protos.dns.last_query));

        if(isSSH()) {
            if(protos.ssh.client_signature) json_object_object_add(pItem, "protos.ssh.client_signature", json_object_new_string(protos.ssh.client_signature));
            if(protos.ssh.server_signature) json_object_object_add(pItem, "protos.ssh.server_signature", json_object_new_string(protos.ssh.server_signature));
        }

        if(isSSL()) {
            if(protos.ssl.certificate)
                json_object_object_add(pItem, "protos.ssl.certificate", json_object_new_string(protos.ssl.certificate));

            if(protos.ssl.server_certificate)
                json_object_object_add(pItem, "protos.ssl.server_certificate", json_object_new_string(protos.ssl.server_certificate));
        }
    }

    json_object_object_add(pItem, "moreinfo.json", json_object_new_string(get_json_info()));

    //    if(client_proc) processLua(vm, client_proc, true);
    //    if(server_proc) processLua(vm, server_proc, false);

    // overall throughput stats
    json_object_object_add(pItem, "top_throughput_bps",   json_object_new_double(top_bytes_thpt));
    json_object_object_add(pItem, "throughput_bps",       json_object_new_double(bytes_thpt));
    json_object_object_add(pItem,   "throughput_trend_bps", json_object_new_double(bytes_thpt_trend));
    json_object_object_add(pItem, "top_throughput_pps",   json_object_new_double(top_pkts_thpt));
    json_object_object_add(pItem, "throughput_pps",       json_object_new_double(pkts_thpt));
    json_object_object_add(pItem,   "throughput_trend_pps", json_object_new_double(pkts_thpt_trend));

    // throughput stats cli2srv and srv2cli breakdown
    json_object_object_add(pItem, "throughput_cli2srv_bps", json_object_new_double(bytes_thpt_cli2srv));
    json_object_object_add(pItem, "throughput_srv2cli_bps", json_object_new_double(bytes_thpt_srv2cli));
    json_object_object_add(pItem, "throughput_cli2srv_pps", json_object_new_double(pkts_thpt_cli2srv));
    json_object_object_add(pItem, "throughput_srv2cli_pps", json_object_new_double(pkts_thpt_srv2cli));

    json_object_object_add(pItem, "cli2srv.packets", json_object_new_int(cli2srv_packets));
    json_object_object_add(pItem, "srv2cli.packets", json_object_new_int(srv2cli_packets));
    json_object_object_add(pItem, "cli2srv.last", json_object_new_int64(get_current_bytes_cli2srv()));
    json_object_object_add(pItem, "srv2cli.last", json_object_new_int64(get_current_bytes_srv2cli()));

    /* ********************* */
    //        dumpPacketStats(vm, true);
    //        dumpPacketStats(vm, false);

    if((!mask_flow) /*&& (details_level >= details_higher)*/) {
        json_object_object_add(pItem, "cli2srv.goodput_bytes.last", json_object_new_int64(get_current_goodput_bytes_cli2srv()));
        json_object_object_add(pItem, "srv2cli.goodput_bytes.last", json_object_new_int64(get_current_goodput_bytes_srv2cli()));

        json_object_object_add(pItem, "cli.latitude", json_object_new_double(get_cli_host()->get_latitude()));
        json_object_object_add(pItem, "cli.longitude", json_object_new_double(get_cli_host()->get_longitude()));
        json_object_object_add(pItem, "srv.latitude", json_object_new_double(get_srv_host()->get_latitude()));
        json_object_object_add(pItem, "srv.longitude", json_object_new_double(get_srv_host()->get_longitude()));

        if(0/*details_level >= details_max*/) {
            json_object_object_add(pItem, "cli.private", json_object_new_boolean(get_cli_host()->get_ip()->isPrivateAddress())); // cli. */
            json_object_object_add(pItem,  "cli.country", json_object_new_string(get_cli_host()->get_country() ? get_cli_host()->get_country() : (char*)""));
            json_object_object_add(pItem,  "cli.city", json_object_new_string(get_cli_host()->get_city() ? get_cli_host()->get_city() : (char*)""));
            json_object_object_add(pItem, "srv.private", json_object_new_boolean(get_srv_host()->get_ip()->isPrivateAddress()));
            json_object_object_add(pItem,  "srv.country", json_object_new_string(get_srv_host()->get_country() ? get_srv_host()->get_country() : (char*)""));
            json_object_object_add(pItem,  "srv.city", json_object_new_string(get_srv_host()->get_city() ? get_srv_host()->get_city() : (char*)""));
        }
    }

    json_object_object_add(pItem, "flow.idle", json_object_new_boolean(isIdleFlow()));
    json_object_object_add(pItem, "flow.status", json_object_new_int(getFlowStatus()));
    // this is used to dynamicall update entries in the GUI
    json_object_object_add(pItem, "ntopng.key", json_object_new_int64(key())); // Key

    json_object_array_add(pArray,pItem);

}

int Xsweb_Flow_Query_Special(PXSJSONFLOW json_flow,json_object *pRetRoot)
{
    int ret = -1;

    NetworkInterface *iface = ntop->getInterfaceById(json_flow->ifaceID);
    if(!iface) return -1;
    //利用FlowHash的findByKey函数(继承GenericHash)直接获取FLOW  (1)
    json_object *pArray = json_object_new_array();
    json_object_object_add(pRetRoot,"Flow_Info",pArray);
    Flow * flow= (Flow*)iface->get_flows_hash()->findByKey(json_flow->ip_addr);
    if(!flow)  return ret;
    flow->Ret_getJSONObject_Flow(pArray,1);

    //自己实现，walker遍历获取对应FLOW  (2)
    //     iface->getPartFlows(pRetRoot,json_flow);

    //  利用NetworkInterface原有函数getFlow获取一个FLOW   (3)
    //未实现,不建议使用，未找到会new flow

    ret = 0;
    return ret;
}

int deserialize_flow(json_object *childObj,PXSJSONFLOW json_flow){
    int ret =-1;
    char *pMac=NULL;
    json_object *ifaceIDObj,*vlanIdObj,*macObj,*ipObj,*maxHitObj,*toSkipObj;

    if(!json_object_object_get_ex(childObj,"ifaceID",&ifaceIDObj))//接口ID(网卡)
        return ret;
    json_flow->ifaceID =json_object_get_int(ifaceIDObj);

    if(!json_object_object_get_ex(childObj,"vlanID",&vlanIdObj))
        return ret;
    json_flow->vlanID=json_object_get_int(vlanIdObj);

    if(!json_object_object_get_ex(childObj,"maxHit",&maxHitObj))
        return ret;
    json_flow->max_hit=json_object_get_int(maxHitObj);

    if(!json_object_object_get_ex(childObj,"toSkip",&toSkipObj))
        return ret;
    json_flow->to_skip=json_object_get_int(toSkipObj);

    if(!json_object_object_get_ex(childObj,"mac",&macObj))
        return ret;
    pMac=(char *)json_object_get_string(macObj);
    parseMacAddress(json_flow->mac,pMac);

    if(!json_object_object_get_ex(childObj,"ip",&ipObj))
        return ret;
    json_flow->ip_addr=json_object_get_int64(ipObj);


    ret=0;
    return ret;

}


int Xsweb_Action_Flow_Query_Special(json_object *JSONroot, json_object *pRetRoot)
{
    if(!JSONroot)
        return -1;

    int i=0,ret=-1,nums=-1;
    XSJSONFLOW json_flow;
    json_object *Array, *childObj;
    json_object_object_get_ex(JSONroot,"ntop",&Array);//获取接下来查询条件(Array)
    if(!Array)  return -1;


    nums= json_object_array_length(Array);//nums批处理数量
    for(i=0;i<nums;i++)
    {
        childObj=json_object_array_get_idx(Array,i);
        if(!childObj) {
            ret = -1;
            break;
        }

        ret=deserialize_flow(childObj,&json_flow);

        ret = Xsweb_Flow_Query_Special(&json_flow,pRetRoot);//
    }

    return ret;
}


int Xsweb_Flow_Query_Normal( PXSJSONFLOW json_flow,json_object *pRetRoot){

    int ret=-1;
    NetworkInterface *iface = ntop->getInterfaceById(json_flow->ifaceID);
    if(!iface) return -1;

    iface->getAllFlows(json_flow,pRetRoot);

    ret=0;
    return ret;
}

int Xsweb_Action_Flow_Query_Normal(json_object *JSONroot,json_object *pRetRoot){
    if(!JSONroot)
        return -1;
    int ret=-1;
    XSJSONFLOW json_flow;
    json_object *Array;
    json_object_object_get_ex(JSONroot,"ntop",&Array);//获取接下来查询条件(Array)
    if(!Array) return -1;

    int nums= json_object_array_length(Array);//nums批处理数量

    json_object *childObj/*,*child*/;
    //    u_int16_t ifaceID;//接口ID(网卡)
    int i;

    for(i=0;i<nums;i++)
    {
        childObj=json_object_array_get_idx(Array,i);
        if(!childObj) {
            ret = -1;
            break;
        }
        //        json_object_object_get_ex(childObj,"ifaceID",&child);//接口ID(网卡)
        //        if(!child) {
        //            ret = -1;
        //            break;
        //        }
        //        ifaceID =json_object_get_int(child);

        ret=deserialize_flow(childObj,&json_flow);
        ret = Xsweb_Flow_Query_Normal(&json_flow,pRetRoot);//
    }

    return ret;
}

int Xsweb_Action_Flow_Query(PXSJSONWEB json_web,json_object *JSONroot, json_object *pRetRoot){
    int ret = -1;

    switch (json_web->actEntry) {
    case XSWEB_SHOW_SPECIAL:
        ret = Xsweb_Action_Flow_Query_Special(JSONroot,pRetRoot);
        break;
    case XSWEB_SHOW_NORMAL:
        ret =Xsweb_Action_Flow_Query_Normal(JSONroot,pRetRoot);
        break;
    default:
        break;
    }
    return ret;
}


int Xsweb_Field_Flow(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot){
    int ret = -1;
    switch (json_web->act) {
    //    case XSWEB_ACT_ADD:
    //    case XSWEB_ACT_DELETE:
    //    case XSWEB_ACT_UPDATE:
    case XSWEB_ACT_QUERY:
        ret = Xsweb_Action_Flow_Query(json_web,JSONroot,pRetRoot);
        break;
    default:
        break;
    }
}
