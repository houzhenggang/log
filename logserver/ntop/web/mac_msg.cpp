#include"ntop_includes.h"

void Mac::Ret_getJSONObject_Mac(json_object *pArray,u_int64_t numMacs){

    char buf[32];
    u_int16_t host_pool = 0;

    json_object *pItem = json_object_new_object();

    json_object_object_add(pItem, "numMacs", json_object_new_int64(numMacs));

    json_object_object_add(pItem, "mac", json_object_new_string(Utils::formatMac(get_mac(), buf, sizeof(buf))));
    json_object_object_add(pItem, "vlan_id",   json_object_new_int(vlan_id));

    json_object_object_add(pItem, "bytes.sent",  json_object_new_int64(sent.getNumBytes()));
    json_object_object_add(pItem, "bytes.rcvd",  json_object_new_int64(rcvd.getNumBytes()));

    if(1/*show_details*/) {//细节展示
        if(manuf)
            json_object_object_add(pItem, "manufacturer",  json_object_new_string(manuf?manuf:""));

        json_object_object_add(pItem,"arp_requests.sent",  json_object_new_int(arp_stats.sent_requests));
        json_object_object_add(pItem, "arp_requests.rcvd",  json_object_new_int(arp_stats.rcvd_requests));
        json_object_object_add(pItem, "arp_replies.sent",  json_object_new_int(arp_stats.sent_replies));
        json_object_object_add(pItem,"arp_replies.rcvd",  json_object_new_int64(arp_stats.rcvd_replies));

        json_object_object_add(pItem, "source_mac",  json_object_new_boolean(source_mac));
        json_object_object_add(pItem,"special_mac",  json_object_new_boolean(special_mac));
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        //    ((GenericTrafficElement*)this)->lua(vm, show_details);

        json_object_object_add(pItem, "throughput_bps", json_object_new_double(bytes_thpt));
        json_object_object_add(pItem, "last_throughput_bps", json_object_new_double(last_bytes_thpt));
        json_object_object_add(pItem, "throughput_trend_bps", json_object_new_int(bytes_thpt_trend));//...
        json_object_object_add(pItem, "throughput_trend_bps_diff", json_object_new_double(bytes_thpt_diff));

        // ntop->getTrace()->traceEvent(TRACE_NORMAL, "[bytes_thpt: %.2f] [bytes_thpt_trend: %d]", bytes_thpt,bytes_thpt_trend);
        json_object_object_add(pItem, "throughput_pps", json_object_new_double(pkts_thpt));
        json_object_object_add(pItem, "last_throughput_pps", json_object_new_double(last_pkts_thpt));
        json_object_object_add(pItem, "throughput_trend_pps", json_object_new_int(pkts_thpt_trend));//...

        if(1/*host_details*/) {
            json_object_object_add(pItem, "bytes.sent", json_object_new_int64(sent.getNumBytes()));
            json_object_object_add(pItem, "bytes.rcvd", json_object_new_int64(rcvd.getNumBytes()));
            json_object_object_add(pItem, "packets.sent", json_object_new_int64(sent.getNumPkts()));
            json_object_object_add(pItem, "packets.rcvd", json_object_new_int64(rcvd.getNumPkts()));
        }
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    }

    json_object_object_add(pItem, "seen.first", json_object_new_int64(first_seen));
    json_object_object_add(pItem, "seen.last",  json_object_new_int64(last_seen));
    json_object_object_add(pItem, "duration",  json_object_new_int(get_duration()));

    json_object_object_add(pItem, "num_hosts",  json_object_new_int(getNumHosts()));

    getInterface()->getHostPools()->findMacPool(this, &host_pool);
    json_object_object_add(pItem, "pool",  json_object_new_int(host_pool));

    json_object_array_add(pArray,pItem);
}


//childObj (ntop包含的内容)
int  Xsweb_Mac_Query_Special(PXSJSONMAC json_mac,json_object *pRetRoot)
{

    NetworkInterface *iface = ntop->getInterfaceById(json_mac->ifaceID);
    if(!iface) return -1;
    //自己实现，利用find_mac_by_name,walker遍历获取对应MAC (1)
    iface->getPartMacs(json_mac,pRetRoot);

    //利用MacHash的get函数直接获取MAC  (2)
    //    json_object *pArray = json_object_new_array();
    //    json_object_object_add(pRetRoot,"Mac-Info",pArray);
    //    Mac *mac = iface->get_macs_hash()->get(json_mac->vlanID,json_mac->mac);
    //    if(!mac) return -1;
    //    mac->Ret_getJSONObject_Mac(pArray,1);//1 test

    // 利用NetworkInterface原有函数获取一个MAC (3) (不建议使用，未找到会new flow)
    //    json_object *pArray = json_object_new_array();
    //    json_object_object_add(pRetRoot,"Mac_Info",pArray);
    //    Mac *mac = iface->getMac(json_mac->mac, json_mac->vlanID,1);//1 test
    //    if(!mac) return -1;
    //    mac->Ret_getJSONObject_Mac(pArray,1);//1 test

    return 0;
}

int deserialize_mac(json_object *childObj,PXSJSONMAC json_mac){
    int ret =-1;
    char *pMac=NULL;
    json_object *ifaceIDObj,*vlanIdObj,*macObj,*maxHitObj,*toSkipObj;

    json_object_object_get_ex(childObj,"ifaceID",&ifaceIDObj);//接口ID(网卡)
    if(!ifaceIDObj) {
        ret = -1;
    }
    json_mac->ifaceID =json_object_get_int(ifaceIDObj);

    if(!json_object_object_get_ex(childObj,"vlanID",&vlanIdObj))
        return ret;
    json_mac->vlanID=json_object_get_int(vlanIdObj);

    if(!json_object_object_get_ex(childObj,"maxHit",&maxHitObj))
        return ret;
    json_mac->max_hit=json_object_get_int(maxHitObj);

    if(!json_object_object_get_ex(childObj,"toSkip",&toSkipObj))
        return ret;
    json_mac->to_skip=json_object_get_int(toSkipObj);

    if(!json_object_object_get_ex(childObj,"mac",&macObj))
        return ret;
    pMac=(char *)json_object_get_string(macObj);
    parseMacAddress(json_mac->mac,pMac);

    ret=0;
    return ret;

}

int Xsweb_Action_Mac_Query_Special(json_object *JSONroot, json_object *pRetRoot)
{
    if(!JSONroot)
        return -1;

    int i=0,ret=-1, nums=-1;
    json_object *childObj;
    XSJSONMAC json_mac;

    json_object *Array ;
    json_object_object_get_ex(JSONroot,"ntop",&Array);//获取接下来查询条件(Array)
    if(!Array) return -1;

    nums= json_object_array_length(Array);//nums批处理数量
    for(i=0;i<nums;i++)
    {
        childObj=json_object_array_get_idx(Array,i);
        if(!childObj) {
            ret = -1;
            break;
        }

        ret=deserialize_mac(childObj,&json_mac);//json_mac?memset 0?
        //    if(ret<0) continue;
        ret = Xsweb_Mac_Query_Special(&json_mac,pRetRoot);
    }
    return ret;
}


int Xsweb_Mac_Query_Normal(PXSJSONMAC json_mac,json_object *pRetRoot){
    int ret=-1;
    NetworkInterface *iface = ntop->getInterfaceById(json_mac->ifaceID);
    if(!iface) return -1;

    iface->getAllMacs(json_mac,pRetRoot);

    ret=0;
    return ret;
}


int Xsweb_Action_Mac_Query_Normal(json_object *JSONroot, json_object *pRetRoot){
    if(!JSONroot)
        return -1;
    int ret=-1;
    json_object *Array;
    json_object_object_get_ex(JSONroot,"ntop",&Array);//获取接下来查询条件(Array)
    if(!Array) return -1;

    int nums= json_object_array_length(Array);//nums批处理数量
    XSJSONMAC json_mac;
    json_object *childObj;
    int i;

    for(i=0;i<nums;i++)
    {
        childObj=json_object_array_get_idx(Array,i);
        if(!childObj) {
            ret = -1;
            break;
        }

        ret=deserialize_mac(childObj,&json_mac);//json_mac?memset 0?
        ret = Xsweb_Mac_Query_Normal(&json_mac,pRetRoot);
    }

    return ret;
}

int Xsweb_Action_Mac_Query(PXSJSONWEB json_web,json_object *JSONroot, json_object *pRetRoot){
    int ret = -1;

    switch (json_web->actEntry) {
    case XSWEB_SHOW_SPECIAL://指定条件
        ret = Xsweb_Action_Mac_Query_Special(JSONroot,pRetRoot);
        break;
    case XSWEB_SHOW_NORMAL://遍历
        ret =Xsweb_Action_Mac_Query_Normal(JSONroot,pRetRoot);
        break;
    default:
        break;
    }

    return ret;
}


int Xsweb_Field_Mac(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot)
{
    int ret = -1;
    switch (json_web->act) {
    //    case XSWEB_ACT_ADD:
    //    case XSWEB_ACT_DELETE:
    //    case XSWEB_ACT_UPDATE:
    case XSWEB_ACT_QUERY:
        ret = Xsweb_Action_Mac_Query(json_web,JSONroot,pRetRoot);
        break;
    default:
        break;
    }
    return ret;
}
