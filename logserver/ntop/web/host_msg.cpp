#include"ntop_includes.h"
//#include"web/web_interaction.h"


void Host::Ret_getJSONObject_Host(json_object *pArray, u_int64_t numHosts){

    json_object *pItem = json_object_new_object();
    char buf[64],ip_buf[64],*local_net;
    char  buf_id[64],  *ipaddr = NULL, *host_id = buf_id;
    bool mask_host = Utils::maskHost(localHost);

    bool host_details = true;

    //    if((ptree && (!match(ptree))) || mask_host)
    //        return;

    json_object_object_add(pItem, "numHosts",  json_object_new_int64(numHosts));

    json_object_object_add(pItem, "ip",  json_object_new_string(ip.printMask(ip_buf, sizeof(ip_buf), localHost)));
    json_object_object_add(pItem, "ipkey", json_object_new_int64(ip.key()));

    json_object_object_add(pItem, "mac", json_object_new_string(Utils::formatMac(mac ? mac->get_mac() : NULL, buf, sizeof(buf))));
    json_object_object_add(pItem, "localHost", json_object_new_boolean(localHost));

    json_object_object_add(pItem, "bytes.sent",
                           json_object_new_int64(sent.getNumBytes() - (1 ? sent.getNumDeserializedBytes() : 0)));
    json_object_object_add(pItem, "bytes.rcvd",
                           json_object_new_int64(rcvd.getNumBytes() - (1 ? rcvd.getNumDeserializedBytes() : 0)));


    json_object_object_add(pItem, "num_alerts", json_object_new_int(triggerAlerts() ? getNumAlerts() : 0));
    json_object_object_add(pItem, "name", json_object_new_string(get_visual_name(buf, sizeof(buf))));
    json_object_object_add(pItem, "local_network_id", json_object_new_int(local_network_id));

    local_net = ntop->getLocalNetworkName(local_network_id);
    if(local_net )
        json_object_object_add(pItem, "local_network_name", json_object_new_string(local_net));

    json_object_object_add(pItem, "asn", json_object_new_int(asn));

    json_object_object_add(pItem, "privatehost", json_object_new_boolean(isPrivateHost()));

    json_object_object_add(pItem, "systemhost", json_object_new_boolean(systemHost));
    json_object_object_add(pItem, "is_blacklisted", json_object_new_boolean(blacklisted_host));
    json_object_object_add(pItem ,"childSafe", json_object_new_boolean(isChildSafe()));
    json_object_object_add(pItem, "source_id", json_object_new_int(source_id));
    json_object_object_add(pItem, "host_pool_id", json_object_new_int(host_pool_id));
    if(asname)  json_object_object_add(pItem, "asname", json_object_new_string(asname ? asname : (char*)""));
    if(strlen(os))    json_object_object_add(pItem, "os", json_object_new_string(os));

    json_object_object_add(pItem, "continent", json_object_new_string(continent ? continent : (char*)""));
    json_object_object_add(pItem, "country", json_object_new_string(country ? country : (char*)""));
    json_object_object_add(pItem, "active_flows.as_client", json_object_new_int(num_active_flows_as_client));
    json_object_object_add(pItem, "active_flows.as_server", json_object_new_int(num_active_flows_as_server));
    json_object_object_add(pItem, "active_http_hosts", json_object_new_int(http ? http->get_num_virtual_hosts() : 0));

    //    if(host_details) {
    //        /*
    //      This has been disabled as in case of an attack, most hosts do not have a name and we will waste
    //      a lot of time doing activities that are not necessary
    //    */
    //        if((symbolic_name == NULL) || (strcmp(symbolic_name, ipaddr) == 0)) {
    //            /* We resolve immediately the IP address by queueing on the top of address queue */

    //            ntop->getRedis()->pushHostToResolve(ipaddr, false, true /* Fake to resolve it ASAP */);
    //        }

    //        if(icmp)
    //            icmp->lua(ip.isIPv4(), vm);

    //        if(ssdpLocation)
    //            lua_push_str_table_entry(vm, "ssdp", ssdpLocation);
    //    }

    /* TCP stats */
    if(host_details) {
        json_object_object_add(pItem, "tcp.packets.sent",  json_object_new_int64(tcp_sent.getNumPkts()));
        json_object_object_add(pItem, "tcp.packets.rcvd",  json_object_new_int64(tcp_rcvd.getNumPkts()));
        json_object_object_add(pItem, "tcp.bytes.sent", json_object_new_int64(tcp_sent.getNumBytes()));
        json_object_object_add(pItem, "tcp.bytes.rcvd", json_object_new_int64(tcp_rcvd.getNumBytes()));

        json_object_object_add(pItem, "tcp.packets.seq_problems",
                               json_object_new_boolean(  (tcpPacketStats.pktRetr
                                                          || tcpPacketStats.pktOOO
                                                          || tcpPacketStats.pktLost) ? true : false));
        json_object_object_add(pItem,"tcpPacketStats.pktRetr",
                               json_object_new_int(tcpPacketStats.pktRetr));
        json_object_object_add(pItem,"tcpPacketStats.pktOOO",
                               json_object_new_int(tcpPacketStats.pktOOO));
        json_object_object_add(pItem,"tcpPacketStats.pktLost",
                               json_object_new_int(tcpPacketStats.pktLost));
    } else {
        /* Limit tcp information to anomalies when host_details aren't required */
        if(tcpPacketStats.pktRetr) json_object_object_add(pItem,
                                                          "tcpPacketStats.pktRetr",
                                                          json_object_new_int(tcpPacketStats.pktRetr));
        if(tcpPacketStats.pktOOO)  json_object_object_add(pItem,
                                                          "tcpPacketStats.pktOOO",
                                                          json_object_new_int(tcpPacketStats.pktOOO));
        if(tcpPacketStats.pktLost) json_object_object_add(pItem,
                                                          "tcpPacketStats.pktLost",
                                                          json_object_new_int(tcpPacketStats.pktLost));
    }

    if(host_details) {
        json_object_object_add(pItem, "total_activity_time", json_object_new_int(total_activity_time));

        if(info) json_object_object_add(pItem, "info", json_object_new_string(getInfo(buf, sizeof(buf))));

        json_object_object_add(pItem, "latitude", json_object_new_double(latitude));
        json_object_object_add(pItem, "longitude", json_object_new_double(longitude));
        json_object_object_add(pItem, "city", json_object_new_string(city ? city : (char*)""));

        json_object_object_add(pItem, "flows.as_client",json_object_new_int(total_num_flows_as_client) );
        json_object_object_add(pItem, "flows.as_server", json_object_new_int(total_num_flows_as_server));

        json_object_object_add(pItem, "udp.packets.sent",  json_object_new_int64(udp_sent.getNumPkts()));
        json_object_object_add(pItem, "udp.bytes.sent", json_object_new_int64(udp_sent.getNumBytes()));
        json_object_object_add(pItem, "udp.packets.rcvd", json_object_new_int64(udp_rcvd.getNumPkts()));
        json_object_object_add(pItem, "udp.bytes.rcvd", json_object_new_int64(udp_rcvd.getNumBytes()));

        json_object_object_add(pItem, "icmp.packets.sent",  json_object_new_int64(icmp_sent.getNumPkts()));
        json_object_object_add(pItem, "icmp.bytes.sent", json_object_new_int64(icmp_sent.getNumBytes()));
        json_object_object_add(pItem, "icmp.packets.rcvd", json_object_new_int64(icmp_rcvd.getNumPkts()));
        json_object_object_add(pItem, "icmp.bytes.rcvd", json_object_new_int64(icmp_rcvd.getNumBytes()));

        json_object_object_add(pItem, "other_ip.packets.sent",  json_object_new_int64(other_ip_sent.getNumPkts()));
        json_object_object_add(pItem, "other_ip.bytes.sent", json_object_new_int64(other_ip_sent.getNumBytes()));
        json_object_object_add(pItem, "other_ip.packets.rcvd",  json_object_new_int64(other_ip_rcvd.getNumPkts()));
        json_object_object_add(pItem, "other_ip.bytes.rcvd", json_object_new_int64(other_ip_rcvd.getNumBytes()));

        json_object_object_add(pItem, "drop_all_host_traffic", json_object_new_boolean(drop_all_host_traffic));

        /* Host ingress/egress drops */
        json_object_object_add(pItem, "bridge.ingress_drops.bytes", json_object_new_int64(ingress_drops.getNumBytes()));
        json_object_object_add(pItem, "bridge.ingress_drops.packets",  json_object_new_int64(ingress_drops.getNumPkts()));
        json_object_object_add(pItem, "bridge.egress_drops.bytes", json_object_new_int64(egress_drops.getNumBytes()));
        json_object_object_add(pItem, "bridge.egress_drops.packets",  json_object_new_int64(egress_drops.getNumPkts()));

        json_object_object_add(pItem, "low_goodput_flows.as_client", json_object_new_int(low_goodput_client_flows));
        json_object_object_add(pItem, "low_goodput_flows.as_server", json_object_new_int(low_goodput_server_flows));

        if((!mask_host) && top_sites && ntop->getPrefs()->are_top_talkers_enabled()) {
            json_object_object_add(pItem, "sites",json_object_new_string(top_sites->json()));
            json_object_object_add(pItem, "sites.old", json_object_new_string(old_sites));
        }
    }

    if(localHost) {
        /* Criteria */
        json_object_object_add(pItem, "upload", json_object_new_int64(getNumBytesSent()));
        json_object_object_add(pItem, "download", json_object_new_int64(getNumBytesRcvd()));
        json_object_object_add(pItem, "unknown", json_object_new_int64(get_ndpi_stats()->getProtoBytes(NDPI_PROTOCOL_UNKNOWN)));
        json_object_object_add(pItem, "incomingflows", json_object_new_int(getNumIncomingFlows()));
        json_object_object_add(pItem, "outgoingflows", json_object_new_int64(getNumOutgoingFlows()));
    }

    json_object_object_add(pItem, "seen.first", json_object_new_int64(first_seen));
    json_object_object_add(pItem, "seen.last", json_object_new_int64(last_seen));
    json_object_object_add(pItem, "duration", json_object_new_int(get_duration()));

//    if(ntop->getPrefs()->is_httpbl_enabled())
//        json_object_object_add(pItem, "httpbl", json_object_new_string(get_httpbl()));

    json_object_object_add(pItem, "dump_host_traffic", json_object_new_boolean(dump_host_traffic));
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    //    if(verbose) {
    //        char *rsp = serialize();//...获取json string

    //        if(categoryStats) categoryStats->lua(vm);
    //        if(ndpiStats) ndpiStats->lua(iface, vm);
    //        lua_push_str_table_entry(vm, "json", rsp);
    //        free(rsp);
    //        sent_stats.lua(vm, "pktStats.sent");
    //        recv_stats.lua(vm, "pktStats.recv");

    //        if(dns)            dns->lua(vm);
    //        if(http)           http->lua(vm);
    //        /*~byzpl~*/
    //        // if(wechat)      wechat->lua(vm);

    //        if(hasAnomalies()) luaAnomalies(vm);
    //    }

    //    if(!returnHost)
    //        host_id = get_hostkey(buf_id, sizeof(buf_id));

    /*~~~~~~~~~~~~   ((GenericTrafficElement*)this)->lua(vm, host_details);~~~~~~~~~*/
    json_object_object_add(pItem, "vlan_id",   json_object_new_int(vlan_id));

    json_object_object_add(pItem, "throughput_bps", json_object_new_double( bytes_thpt));
    json_object_object_add(pItem, "last_throughput_bps", json_object_new_double(last_bytes_thpt));
    json_object_object_add(pItem, "throughput_trend_bps", json_object_new_int(bytes_thpt_trend));
    json_object_object_add(pItem, "throughput_trend_bps_diff", json_object_new_double(bytes_thpt_diff));

    // ntop->getTrace()->traceEvent(TRACE_NORMAL, "[bytes_thpt: %.2f] [bytes_thpt_trend: %d]", bytes_thpt,bytes_thpt_trend);
    json_object_object_add(pItem, "throughput_pps", json_object_new_double(pkts_thpt));
    json_object_object_add(pItem, "last_throughput_pps", json_object_new_double(last_pkts_thpt));
    json_object_object_add(pItem, "throughput_trend_pps", json_object_new_int(pkts_thpt_trend));

    if(host_details) {
        json_object_object_add(pItem, "bytes.sent", json_object_new_int(sent.getNumBytes()));
        json_object_object_add(pItem, "bytes.rcvd", json_object_new_int(rcvd.getNumBytes()));
        json_object_object_add(pItem, "packets.sent", json_object_new_int(sent.getNumPkts()));
        json_object_object_add(pItem, "packets.rcvd", json_object_new_int(rcvd.getNumPkts()));
    }
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    // json_object_object_add(pItem, "ip", ip.getJSONObject());
    //    if(symbolic_name)                  json_object_object_add(pItem, "symbolic_name", json_object_new_string(symbolic_name));
    //   if(trafficCategory[0] != '\0')     json_object_object_add(pItem, "trafficCategory",    json_object_new_string(trafficCategory));
    //    if(city) free(city);
    //    ntop->getGeolocation()->getInfo(&ip, &continent, &country, &city, &latitude, &longitude);
    /* packet stats */
    //    json_object_object_add(pItem, "pktStats.sent", sent_stats.getJSONObject());
    //    json_object_object_add(pItem, "pktStats.recv", recv_stats.getJSONObject());
    /* throughput stats */
    //    json_object_object_add(pItem, "throughput_bps", json_object_new_double(bytes_thpt));
    //    json_object_object_add(pItem, "throughput_trend_bps", json_object_new_string(Utils::trend2str(bytes_thpt_trend)));
    //    json_object_object_add(pItem, "throughput_pps", json_object_new_double(pkts_thpt));
    //    json_object_object_add(pItem, "throughput_trend_pps", json_object_new_string(Utils::trend2str(pkts_thpt_trend)));
    //    json_object_object_add(pItem, "flows.as_client", json_object_new_int(total_num_flows_as_client));
    //    json_object_object_add(pItem, "flows.as_server", json_object_new_int(total_num_flows_as_server))
    //    json_object_object_add(pItem, "sent", sent.getJSONObject());
    //    json_object_object_add(pItem, "rcvd", rcvd.getJSONObject());
    //    json_object_object_add(pItem, "ndpiStats", ndpiStats->getJSONObject(iface));
    //    json_object_object_add(pItem, "total_activity_time", json_object_new_int(total_activity_time));

    //    if(categoryStats)  json_object_object_add(pItem, "categories", categoryStats->getJSONObject());
    //    if(dns)  json_object_object_add(pItem, "dns", dns->getJSONObject());
    //    if(http) json_object_object_add(pItem, "http", http->getJSONObject());

    json_object_array_add(pArray,pItem);
}


int deserialize_host( json_object *childObj,PXSJSONHOST json_host){
    int ret=-1;
    char *pMac=NULL;
    json_object *vlanIdObj,*macObj,*ipObj,*ifaceIDObj,*maxHitObj,*toSkipObj;

    if(!json_object_object_get_ex(childObj,"ifaceID",&ifaceIDObj))//接口ID(网卡)
        return ret;
    json_host->ifaceID =json_object_get_int(ifaceIDObj);

    if(!json_object_object_get_ex(childObj,"vlanID",&vlanIdObj))
        return ret;
    json_host->vlanID=json_object_get_int(vlanIdObj);

    if(!json_object_object_get_ex(childObj,"maxHit",&maxHitObj))
        return ret;
    json_host->max_hit=json_object_get_int(maxHitObj);

    if(!json_object_object_get_ex(childObj,"toSkip",&toSkipObj))
        return ret;
    json_host->to_skip=json_object_get_int(toSkipObj);

    if(!json_object_object_get_ex(childObj,"mac",&macObj))
        return ret;
    pMac=(char *)json_object_get_string(macObj);
    parseMacAddress(json_host->mac,pMac);

    if(!json_object_object_get_ex(childObj,"ip",&ipObj))
        return ret;
    json_host->ip_addr.s_addr=json_object_get_int64(ipObj);

    ret=0;
    return ret;
}

int Xsweb_Host_Query_Special(PXSJSONHOST json_host,json_object *pRetRoot)
{
    int ret = -1;
    NetworkInterface *iface = ntop->getInterfaceById(json_host->ifaceID);
    if(!iface) return -1;

    //自己实现获取host的方法(1)
    //    iface->getPartHosts(pRetRoot,json_host);
    //利用NetworkInterface原有函数getHost获取一个host(2)  (未找到会new host?)
    json_object *pArray = json_object_new_array();
    json_object_object_add(pRetRoot,"Host_Info",pArray);
    char * host_ip=inet_ntoa(json_host->ip_addr);  //test
    Host *host = iface->getHost(host_ip, json_host->vlanID);
    if(!host) return -1;
    host->Ret_getJSONObject_Host(pArray,1);//1 test



    //利用HostHash的get函数直接获取Host (3)   未完成
    //    json_object *pArray = json_object_new_array();
    //    json_object_object_add(pRetRoot,"Host_Info",pArray);
    //    Host *host = iface->get_hosts_hash()->get(json_host->vlanID,key);
    //     if(!host) return -1;
    //     host->Ret_getJSONObject_Host(pArray,1);//1 test

//iface->getHostInfo()  findHostsByIP

    ret = 0;
    return ret;

}


int Xsweb_Action_Host_Query_Special(json_object *JSONroot, json_object *pRetRoot)
{
    if(!JSONroot)
        return -1;

    json_object *Array ;
    json_object_object_get_ex(JSONroot,"ntop",&Array);//获取接下来查询条件(Array)
    if(!Array)  return -1;

    int nums= json_object_array_length(Array);//nums批处理数量
    int i=0,ret=-1;
    json_object *childObj;

    XSJSONHOST json_host;

    for(i=0;i<nums;i++)
    {
        childObj=json_object_array_get_idx(Array,i);
        if(!childObj) {
            ret = -1;
            break;
        }

        deserialize_host(childObj,&json_host);
        ret = Xsweb_Host_Query_Special(&json_host,pRetRoot);//
    }

    return ret;
}


int Xsweb_Host_Query_Normal(PXSJSONHOST json_host,json_object *pRetRoot){

    int ret=-1;
    NetworkInterface *iface = ntop->getInterfaceById(json_host->ifaceID);
    if(!iface) return -1;

    iface->getAllHosts(json_host,pRetRoot);

    ret=0;
    return ret;
}


int Xsweb_Action_Host_Query_Normal(json_object *JSONroot,json_object *pRetRoot){

    if(!JSONroot)
        return -1;

    int ret=-1;
    json_object *Array;
    json_object_object_get_ex(JSONroot,"ntop",&Array);//获取接下来查询条件(Array)
    if(!Array) return -1;

    int nums= json_object_array_length(Array);//nums批处理数量

    XSJSONHOST json_host;
    json_object *childObj;

    int i;
    for(i=0;i<nums;i++)
    {
        childObj=json_object_array_get_idx(Array,i);
        if(!childObj) {
            ret = -1;
            break;
        }

        deserialize_host(childObj,&json_host);
        ret = Xsweb_Host_Query_Normal(&json_host,pRetRoot);//
    }

    return ret;
}


int Xsweb_Action_Host_Query(PXSJSONWEB json_web,json_object *JSONroot, json_object *pRetRoot){
    int ret = -1;

    switch (json_web->actEntry) {
    case XSWEB_SHOW_SPECIAL:
        ret = Xsweb_Action_Host_Query_Special(JSONroot,pRetRoot);
        break;
    case XSWEB_SHOW_NORMAL:
        ret =Xsweb_Action_Host_Query_Normal(JSONroot,pRetRoot);
        break;
    default:
        break;
    }

    return ret;
}

int Xsweb_Field_Host(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot){
    int ret = -1;
    switch (json_web->act) {
    //    case XSWEB_ACT_ADD:
    //    case XSWEB_ACT_DELETE:
    //    case XSWEB_ACT_UPDATE:
    case XSWEB_ACT_QUERY:
        ret = Xsweb_Action_Host_Query(json_web,JSONroot,pRetRoot);
        break;
    default:
        break;
    }
}
