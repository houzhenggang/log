#include "ndpi_api.h"
#include<stdlib.h>

#define   WECHAT_HTONS443   47873

#ifdef NDPI_PROTOCOL_WECHAT

static void ndpi_int_wechat_add_connection(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_WECHAT,NDPI_PROTOCOL_UNKNOWN);
}

void search_wechat_udp(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;
    //      struct ndpi_id_struct         *src=ndpi_struct->src;
    //      struct ndpi_id_struct         *dst=ndpi_struct->dst;
    NDPI_LOG(NDPI_PROTOCOL_WECHAT, ndpi_struct, NDPI_LOG_DEBUG, "search wechat  udp.\n");

    if(packet->payload_packet_len >= 90 && packet->payload_packet_len <= 103){
        if(packet->payload[0] == 0xd5){
            //   ++flow->wechat_out_stage;//byzpl  default exist
            ndpi_int_wechat_add_connection(ndpi_struct, flow);
            //            if(flow->wechat_out_stage>=3){
            //                NDPI_LOG(NDPI_PROTOCOL_WECHAT, ndpi_struct, NDPI_LOG_DEBUG, "detected wechat.\n");
            //                ndpi_int_wechat_add_connection(ndpi_struct, flow);
            //            }
            return;
        }
    }
    if(packet->payload_packet_len == 96){
        if(packet->payload[0] == 0xa3){
            //   ++flow->wechat_in_stage;//byzpl  default exist
            ndpi_int_wechat_add_connection(ndpi_struct, flow);
            //            if(flow->wechat_in_stage >= 3){
            //                NDPI_LOG(NDPI_PROTOCOL_WECHAT, ndpi_struct, NDPI_LOG_DEBUG, "detected wechat.\n");
            //                ndpi_int_wechat_add_connection(ndpi_struct, flow);
            //            }
            return;
        }
    }
    NDPI_LOG(NDPI_PROTOCOL_WECHAT, ndpi_struct, NDPI_LOG_DEBUG, "exclude wechat.\n");
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_WECHAT);
}

/*~~~~~~~~~~~~~~~~byzpl~~~~~~~~~~~~~~~~*/
void search_wechat_tcp(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;
    NDPI_LOG(NDPI_PROTOCOL_WECHAT, ndpi_struct, NDPI_LOG_DEBUG, "search wechat tcp.\n");
    if(packet->payload_packet_len>150&&WECHAT_HTONS443 == packet->tcp->dest)
    {
        if(memcmp(&packet->payload[125], "weixinnum", 9) == 0)
        {
            NDPI_LOG(NDPI_PROTOCOL_WECHAT, ndpi_struct, NDPI_LOG_DEBUG, "wechat  ID  msg\n");
            flow->virtualnum.id=strtoul((char *)&packet->payload[138],NULL,10);
            flow->virtualnum.type=virtual_weixin;
            flow->virtualnum.status = vrt_method_online;
            ndpi_int_wechat_add_connection(ndpi_struct, flow);
        }
    }
}

void xs_search_wechat(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow){
    struct ndpi_packet_struct *packet = &flow->packet;
    if(IPPROTO_TCP==packet->l4_protocol&&packet->tcp!=NULL){
        search_wechat_tcp(ndpi_struct,flow);
    } else  if(IPPROTO_UDP==packet->l4_protocol&&packet->udp!=NULL){
        search_wechat_udp(ndpi_struct,flow);
    }
}

void init_wechat_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id, NDPI_PROTOCOL_BITMASK *detection_bitmask)
{
    ndpi_set_bitmask_protocol_detection("微信", ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_WECHAT,
                                        xs_search_wechat,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_OR_UDP_WITH_PAYLOAD,
                                        SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
}

#endif

