#include"ntop_includes.h"



void NetworkInterface::Ret_getJSONObject_Interface(json_object *pArray){

    json_object *pItem = json_object_new_object();

    TcpFlowStats _tcpFlowStats;
    EthStats _ethStats;
    LocalTrafficStats _localStats;
    nDPIStats _ndpiStats;
    PacketStats _pktStats;
    TcpPacketStats _tcpPacketStats;


    json_object_object_add(pItem, "name", json_object_new_string(get_name()));
    json_object_object_add(pItem, "description",   json_object_new_string(get_description()));
   json_object_object_add(pItem, "scalingFactor", json_object_new_int(scalingFactor));
    json_object_object_add(pItem,  "id", json_object_new_int(id));
    json_object_object_add(pItem, "isView", json_object_new_boolean( isView())); /* View interface */
    json_object_object_add(pItem , "seen.last", json_object_new_int(getTimeLastPktRcvd()));
    json_object_object_add(pItem, "sprobe", json_object_new_boolean(get_sprobe_interface()));
    json_object_object_add(pItem, "inline", json_object_new_boolean(get_inline_interface()));
    json_object_object_add(pItem, "vlan",     json_object_new_boolean(hasSeenVlanTaggedPackets()));
    json_object_object_add(pItem, "has_macs", json_object_new_boolean(hasSeenMacAddresses()));

    if(remoteIfname)                    json_object_object_add(pItem, "remote.name",    json_object_new_string(remoteIfname));
    if(remoteIfIPaddr)                   json_object_object_add(pItem, "remote.if_addr", json_object_new_string(remoteIfIPaddr));
    if(remoteProbeIPaddr)             json_object_object_add(pItem, "probe.ip", json_object_new_string(remoteProbeIPaddr));
    if(remoteProbePublicIPaddr)     json_object_object_add(pItem, "probe.public_ip", json_object_new_string(remoteProbePublicIPaddr));

    json_object_object_add(pItem, "packets",     json_object_new_int(getNumPackets()));
    json_object_object_add(pItem, "bytes",       json_object_new_int(getNumBytes()));
    json_object_object_add(pItem, "flows",       json_object_new_int(getNumFlows()));
    json_object_object_add(pItem, "hosts",       json_object_new_int(getNumHosts()));
    json_object_object_add(pItem, "local_hosts", json_object_new_int(getNumLocalHosts()));
    json_object_object_add(pItem, "http_hosts",  json_object_new_int(getNumHTTPHosts()));
    json_object_object_add(pItem, "drops",       json_object_new_int(getNumPacketDrops()));
    json_object_object_add(pItem, "devices",     json_object_new_int(numL2Devices));

    //
    /* even if the counter is global, we put it here on every interface
     as we may decide to make an elasticsearch thread per interface.
  */
    if(ntop->getPrefs()->do_dump_flows_on_es()) {
        //        ntop->getElasticSearch()->lua(vm, false /* Overall */);
        json_object_object_add(pItem,   "flow_export_count",
                               json_object_new_int64(ntop->getElasticSearch()->getElkExportedFlows()));
        json_object_object_add(pItem, "flow_export_drops",
                               json_object_new_int(ntop->getElasticSearch()->numDroppedFlows()));
        json_object_object_add(pItem, "flow_export_rate",
                               json_object_new_double(ntop->getElasticSearch()->getElkExportRate()>= 0 ? ntop->getElasticSearch()->getElkExportRate() : 0));

    } else if(ntop->getPrefs()->do_dump_flows_on_mysql()) {
        //        if(db)
        //{db->lua(vm, false /* Overall */);
        //        json_object_object_add(pItem, "flow_export_count",
        //                                 mysqlExportedFlows);
        //        json_object_object_add(pItem, "flow_export_drops",
        //                                   mysqlDroppedFlows);
        //        json_object_object_add(pItem, "flow_export_rate",
        //                                   mysqlExportRate >= 0 ? mysqlExportRate : 0);}
    }else if(ntop->getPrefs()->do_dump_flows_on_ls()){
        //        ntop->getLogstash()->lua(vm, false /* Overall */);
        json_object_object_add(pItem,   "flow_export_count",
                               json_object_new_int64(ntop->getLogstash()->getElkExportedFlows()));
        json_object_object_add(pItem, "flow_export_drops",
                               json_object_new_int(ntop->getLogstash()->numDroppedFlows()));
        json_object_object_add(pItem, "flow_export_rate",
                               json_object_new_double(ntop->getLogstash()->getElkExportRate()>= 0 ? ntop->getLogstash()->getElkExportRate() : 0));
    }



    json_object_object_add(pItem, "packets",     json_object_new_int(getNumPackets() - getCheckPointNumPackets()));
    json_object_object_add(pItem, "bytes",       json_object_new_int(getNumBytes() - getCheckPointNumBytes()));
    json_object_object_add(pItem, "drops",       json_object_new_int(getNumPacketDrops() - getCheckPointNumPacketDrops()));

    if(ntop->getPrefs()->do_dump_flows_on_es()) {
        //        ntop->getElasticSearch()->lua(vm, true /* Since last checkpoint */);
        json_object_object_add(pItem,   "flow_export_count",
                               json_object_new_int64(ntop->getElasticSearch()->getElkExportedFlows()-ntop->getElasticSearch()->getCheckpointExportedFlows()));
        json_object_object_add(pItem, "flow_export_drops",
                               json_object_new_int(ntop->getElasticSearch()->numDroppedFlows()-ntop->getElasticSearch()->getCheckpointDroppedFlows()));
        json_object_object_add(pItem, "flow_export_rate",
                               json_object_new_double(ntop->getElasticSearch()->getElkExportRate()>= 0 ? ntop->getElasticSearch()->getElkExportRate() : 0));
    } else if(ntop->getPrefs()->do_dump_flows_on_mysql()) {
        //        if(db) {
        //db->lua(vm, true /* Since last checkpoint */);

        //        json_object_object_add(pItem, "flow_export_count",
        //                                 mysqlExportedFlows - (since_last_checkpoint ? checkpointExportedFlows : 0));
        //        json_object_object_add(pItem, "flow_export_drops",
        //                                    mysqlDroppedFlows - (since_last_checkpoint ? checkpointDroppedFlows : 0));
        //        json_object_object_add(pItem, "flow_export_rate",
        //                                     mysqlExportRate >= 0 ? mysqlExportRate : 0);
        //  }

    }else if(ntop->getPrefs()->do_dump_flows_on_ls()){
        //        ntop->getLogstash()->lua(vm, true /* Since last checkpoint */);
        json_object_object_add(pItem,   "flow_export_count",
                               json_object_new_int64(ntop->getLogstash()->getElkExportedFlows()-ntop->getLogstash()->getCheckpointExportedFlows()));
        json_object_object_add(pItem, "flow_export_drops",
                               json_object_new_int(ntop->getLogstash()->numDroppedFlows()-ntop->getLogstash()->getCheckpointDroppedFlows()));
        json_object_object_add(pItem, "flow_export_rate",
                               json_object_new_double(ntop->getLogstash()->getElkExportRate()>= 0 ? ntop->getLogstash()->getElkExportRate() : 0));
    }


    json_object_object_add(pItem, "remote_pps", json_object_new_int(last_remote_pps));
    json_object_object_add(pItem, "remote_bps", json_object_new_int(last_remote_bps));
    //    icmp_v4.lua(true, vm);
    //    icmp_v6.lua(false, vm);
    json_object_object_add(pItem, "arp.requests", json_object_new_int(arp_requests));
    json_object_object_add(pItem, "arp.replies", json_object_new_int(arp_replies));
    json_object_object_add(pItem, "type", json_object_new_string((char*)get_type()));
    json_object_object_add(pItem, "speed", json_object_new_int(ifSpeed));
    json_object_object_add(pItem, "mtu", json_object_new_int(ifMTU));
    json_object_object_add(pItem, "alertLevel", json_object_new_int(alertLevel));
    json_object_object_add(pItem, "ip_addresses", json_object_new_string((char*)getLocalIPAddresses()));


    if(has_too_many_flows) json_object_object_add(pItem, "too_many_flows",json_object_new_boolean (1));
    if(has_too_many_hosts) json_object_object_add(pItem, "too_many_hosts", json_object_new_boolean(1));


    sumStats(&_tcpFlowStats, &_ethStats, &_localStats,
             &_ndpiStats, &_pktStats, &_tcpPacketStats);

    for(u_int8_t s = 0; s<numSubInterfaces; s++)
        subInterfaces[s]->sumStats(&_tcpFlowStats, &_ethStats,
                                   &_localStats, &_ndpiStats, &_pktStats, &_tcpPacketStats);

    //    _tcpFlowStats.lua(vm, "tcpFlowStats");
    json_object_object_add(pItem, "numSynFlows", json_object_new_int(_tcpFlowStats.getNumSynFlows()));
    json_object_object_add(pItem, "numEstablishedFlows",json_object_new_int(_tcpFlowStats.getNumEstablishedFlows()) );
    json_object_object_add(pItem, "numResetFlows", json_object_new_int(_tcpFlowStats.getNumResetFlows()));
    json_object_object_add(pItem, "numFinFlows", json_object_new_int(_tcpFlowStats.getNumFinFlows()));
    //    _ethStats.lua(vm);
    //    _ethStats.eth_IPv4.lua(vm,  "IPv4_");
    json_object_object_add(pItem, "IPv4_bytes",json_object_new_int64(_ethStats.getIPv4Stats()->getBytes()));
    json_object_object_add(pItem,  "IPv4_packets", json_object_new_int64(_ethStats.getIPv4Stats()->getPkts()));
    //    _ethStats.eth_IPv6.lua(vm,  "IPv6_");
    json_object_object_add(pItem, "IPv4_bytes",json_object_new_int64(_ethStats.getIPv6Stats()->getBytes()));
    json_object_object_add(pItem,  "IPv4_packets", json_object_new_int64(_ethStats.getIPv6Stats()->getPkts()));
    //    _ethStats.eth_ARP.lua(vm,   "ARP_");
    json_object_object_add(pItem, "ARP_bytes",json_object_new_int64(_ethStats.getARPStats()->getBytes()));
    json_object_object_add(pItem,  "ARP_packets", json_object_new_int64(_ethStats.getARPStats()->getPkts()));
    //    _ethStats.eth_MPLS.lua(vm,  "MPLS_");
    json_object_object_add(pItem, "MPLS_bytes",json_object_new_int64(_ethStats.getMPLSStats()->getBytes()));
    json_object_object_add(pItem,  "MPLS_packets", json_object_new_int64(_ethStats.getMPLSStats()->getPkts()));
    //    _ethStats.eth_other.lua(vm, "other_");
    json_object_object_add(pItem, "other_bytes",json_object_new_int64(_ethStats.getEthOtherStats()->getBytes()));
    json_object_object_add(pItem,  "other_packets", json_object_new_int64(_ethStats.getEthOtherStats()->getPkts()));

    json_object_object_add(pItem, "bytes",   json_object_new_int(getNumBytes()));
    json_object_object_add(pItem, "packets", json_object_new_int(getNumPackets()));

    //    _localStats.lua(vm);
    json_object_object_add(pItem, "localstats_packets_local2local", json_object_new_int(_localStats.getPackets().local2local));
    json_object_object_add(pItem, "localstats_packets_local2remote", json_object_new_int(_localStats.getPackets().local2remote));
    json_object_object_add(pItem, "localstats_packets_remote2local",json_object_new_int( _localStats.getPackets().remote2local));
    json_object_object_add(pItem, "localstats_packets_remote2remote", json_object_new_int(_localStats.getPackets().remote2remote));

    json_object_object_add(pItem, "localstats_bytes_local2local", json_object_new_int(_localStats.getBytes().local2local));
    json_object_object_add(pItem, "localstats_bytes_local2remote", json_object_new_int(_localStats.getBytes().local2remote));
    json_object_object_add(pItem, "localstats_bytes_remote2local", json_object_new_int(_localStats.getBytes().remote2local));
    json_object_object_add(pItem, "localstats_bytes_remote2remote", json_object_new_int(_localStats.getBytes().remote2remote));

    //       _ndpiStats.lua(this, vm);//........................................
    //     _pktStats.lua(vm, "pktSizeDistribution");

    //       _tcpPacketStats.lua(vm, "tcpPacketStats");


    if(!isView()) {
        if(pkt_dumper)

        {
            //                  pkt_dumper->lua(vm);
            json_object_object_add(pItem, "pkt_dumper_num_dumped_pkts", json_object_new_int(pkt_dumper->get_num_dumped_packets()));
            json_object_object_add(pItem, "pkt_dumper_num_dumped_files", json_object_new_int(pkt_dumper->get_num_dumped_files()));
        }

#ifdef NTOPNG_PRO
        if(flow_profiles) flow_profiles->lua(vm);
#endif
    }

    json_object_array_add(pArray,pItem);


}

int Xsweb_Interface_Query_Normal(json_object *childObj, u_int16_t ifaceID,json_object *pRetRoot){

    int ret=-1;
    NetworkInterface *iface = ntop->getInterfaceById(ifaceID);
    if(!iface) return -1;

    iface->getAllInterfaceStats(pRetRoot);

    ret=0;
    return ret;
}


int Xsweb_Action_Interface_Query_Normal(json_object *JSONroot,json_object *pRetRoot){
    if(!JSONroot)
        return -1;
    int ret=-1;
    json_object *Array;
    json_object_object_get_ex(JSONroot,"ntop",&Array);//获取接下来查询条件(Array)
    if(!Array) return -1;

    int nums= json_object_array_length(Array);//nums批处理数量

    json_object *childObj,*child;
    u_int16_t ifaceID;//接口ID(网卡)
    int i;

    for(i=0;i<nums;i++)
    {
        childObj=json_object_array_get_idx(Array,i);
        if(!childObj) {
            ret = -1;
            break;
        }
        json_object_object_get_ex(childObj,"ifaceID",&child);//接口ID(网卡)
        if(!child) {
            ret = -1;
            break;
        }

        ifaceID =json_object_get_int(child);
        ret = Xsweb_Interface_Query_Normal(childObj,ifaceID,pRetRoot);//
    }

    return ret;
}



int  Xsweb_Action_Interface_Query(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot){
    int ret = -1;

    switch (json_web->actEntry) {
    case XSWEB_SHOW_SPECIAL:
        //            ret = Xsweb_Action_Flow_Query_Special(JSONroot,pRetRoot);
        break;
    case XSWEB_SHOW_NORMAL:
        ret =Xsweb_Action_Interface_Query_Normal(JSONroot,pRetRoot);
        break;
    default:
        break;
    }
    return ret;

}


int Xsweb_Field_Interface(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot){
    int ret = -1;
    switch (json_web->act) {
    //    case XSWEB_ACT_ADD:
    //    case XSWEB_ACT_DELETE:
    //    case XSWEB_ACT_UPDATE:
    case XSWEB_ACT_QUERY:
        ret = Xsweb_Action_Interface_Query(json_web,JSONroot,pRetRoot);
        break;
    default:
        break;
    }
}
