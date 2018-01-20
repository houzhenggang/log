/*
 * quic.c
 *
 * Andrea Buscarinu - <andrea.buscarinu@gmail.com>
 * Michele Campus - <campus@ntop.org>
 *
 * Copyright (C) 2012-15 - ntop.org
 *
 * This module is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ndpi_api.h"



struct oicq_head
{
    u_int8_t hdr;
    u_int16_t ver;
    u_int16_t cmd;
    u_int16_t seq;
    u_int32_t qq_num;
}__attribute__((packed));

#ifdef NDPI_PROTOCOL_OICQ

#define OICQ_HTONS443   47873
#define OICQ_HTONS8000  16415

int xs_oicq_check_stats_udp(struct ndpi_flow_struct *flow){
    struct ndpi_packet_struct *packet = &flow->packet;
    if((OICQ_HTONS8000 == packet->udp->dest) && packet->payload_packet_len >10){
        if(packet->payload[0] == 0x2 && packet->payload[packet->payload_packet_len - 1] == 0x3){
            struct oicq_head *head = (struct oicq_head*)packet->payload;
            if(0x6200==head->cmd){
                flow->virtualnum.status = vrt_method_offline;
            } else if( 0x0828 == htons(head->cmd)){
                flow->virtualnum.status = vrt_method_online;
            }else
                return 0;
            flow->virtualnum.type = virtual_qq;
            flow->virtualnum.id = ntohl(head->qq_num);
            return 1;
        }
    }
    return 0;
}

void xs_oicq_offline_ssl_tcp(struct ndpi_flow_struct *flow)
{
        struct ndpi_packet_struct *packet = &flow->packet;
        if((OICQ_HTONS443 == packet->tcp->dest) && packet->payload_packet_len >12){
            if(packet->payload[0] == 0x2 && packet->payload[packet->payload_packet_len - 1] == 0x3){
                u_int16_t command = *((u_int16_t*)&packet->payload[5]);
                if(command == 0x6200){
                    flow->virtualnum.status = vrt_method_offline;
                    flow->virtualnum.type = virtual_qq;
                    flow->virtualnum.id = ntohl(*((u_int32_t*)&packet->payload[9]));
                }
            }
        }
}

int xs_oicq_online_stats_udp(struct ndpi_flow_struct *flow){
        struct ndpi_packet_struct *packet = &flow->packet;
        if((OICQ_HTONS8000 == packet->udp->dest) && packet->payload_packet_len >10){
            if(packet->payload[0] == 0x2 && packet->payload[packet->payload_packet_len - 1] == 0x3){
                struct oicq_head *head = (struct oicq_head*)packet->payload;
                if(0x1300==head->cmd || 0x3608 == head->cmd || 0x5900==head->cmd){
                    flow->virtualnum.status = vrt_method_online;
                } else
                    return 0;
                flow->virtualnum.type = virtual_qq;
                flow->virtualnum.id = ntohl(head->qq_num);
                return 1;
            }
        }
        return 0;
}

int xs_oicq_online_ssl(struct ndpi_flow_struct *flow){
        struct ndpi_packet_struct *packet = &flow->packet;
        if((OICQ_HTONS443 == packet->tcp->dest) && packet->payload_packet_len >12){
            if(packet->payload[2] == 0x2 && packet->payload[packet->payload_packet_len - 1] == 0x3){
                u_int16_t command = *((u_int16_t*)&packet->payload[5]);
                if(command == 0x2508){
                    flow->virtualnum.status = vrt_method_online;
                    flow->virtualnum.type = virtual_qq;
                    flow->virtualnum.id= ntohl(*((u_int32_t*)&packet->payload[9]));
                    return 1;
                }
            }
        }
        return 0;
}


void xs_search_oicq(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;

    if( flow->detected_protocol_stack[0] != NDPI_PROTOCOL_OICQ/*NDPI_PROTOCOL_OICQ == packet->detected_protocol*/){
        if(IPPROTO_UDP==packet->l4_protocol && NULL != packet->udp)
            xs_oicq_check_stats_udp(flow);
        else if(IPPROTO_TCP==packet->l4_protocol && NULL != packet->tcp)
            xs_oicq_offline_ssl_tcp(flow);
        return;
    }

    if(packet->payload_packet_len >= 4){
        if(packet->payload_packet_len == 72 && (OICQ_HTONS8000 != packet->udp->dest) && (OICQ_HTONS8000 != packet->udp->source)){
            if(packet->payload[0] == 0x02 && packet->payload[packet->payload_packet_len - 1] == 0x03){
                NDPI_LOG(NDPI_PROTOCOL_OICQ/*NDPI_PROTOCOL_QQ_CHAT*/, ndpi_struct, NDPI_LOG_DEBUG, "detected qqchat.\n");
                ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_OICQ/*NDPI_PROTOCOL_QQ_CHAT*/,NDPI_PROTOCOL_UNKNOWN);
                return;
            }
        }
        //  flow->qq_stage == 3
        if(packet->payload[0] == 0x28 && packet->payload[1] == 0 && packet->payload[2] == 0 && packet->payload[3] == 0){
            flow->qq_stage++;
            if(flow->qq_stage >= 3){
                NDPI_LOG(NDPI_PROTOCOL_OICQ/*NDPI_PROTOCOL_QQ_CHAT*/, ndpi_struct, NDPI_LOG_DEBUG, "detected qqchat.\n");
                ndpi_set_detected_protocol(ndpi_struct, flow,NDPI_PROTOCOL_OICQ /*NDPI_PROTOCOL_QQ_CHAT*/,NDPI_PROTOCOL_UNKNOWN);
            }
            return;
        }
    }
    //   int n=0;
    if(IPPROTO_UDP==packet->l4_protocol && packet->udp != NULL && packet->payload_packet_len > 11) {
        NDPI_LOG(NDPI_PROTOCOL_OICQ, ndpi_struct, NDPI_LOG_DEBUG, "calculating oicq over udp.\n");
        if((OICQ_HTONS8000 ==packet->udp->source) || (OICQ_HTONS8000 == packet->udp->dest)) {
            if(packet->payload[0] == 0x2 && packet->payload[packet->payload_packet_len - 1] == 0x3){
                xs_oicq_online_stats_udp(flow);
                ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_OICQ,NDPI_PROTOCOL_UNKNOWN);
                return;
            }
        }
    }else if(IPPROTO_TCP==packet->l4_protocol && packet->tcp != NULL && packet->payload_packet_len>12){
        NDPI_LOG(NDPI_PROTOCOL_OICQ, ndpi_struct, NDPI_LOG_DEBUG, "calculating oicq over udp.\n");
        if((OICQ_HTONS443 ==packet->tcp->source) || (OICQ_HTONS443 == packet->tcp->dest)) {
            if(packet->payload[2] == 0x2 && packet->payload[packet->payload_packet_len - 1] == 0x3){
                xs_oicq_online_ssl(flow);
                ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_OICQ,NDPI_PROTOCOL_UNKNOWN);
                return;
            }
        }
    }
          //  if(1==n){
//                NDPI_LOG(NDPI_PROTOCOL_OICQ, ndpi_struct, NDPI_LOG_DEBUG, "exclude oicq.\n");
//                NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_OICQ);
          //  }
}

void init_oicq_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id, NDPI_PROTOCOL_BITMASK *detection_bitmask)
{
    //ndpi_port_range ports_a[MAX_DEFAULT_PORTS], ports_b[MAX_DEFAULT_PORTS];

    //    set_proto_defaults(ndpi_struct, NDPI_PROTOCOL_ACCEPTABLE, NDPI_PROTOCOL_OICQ,
    //                       NDPI_PROTOCOL_OICQ, "QQ",
    //                       build_default_ports(ports_a, 0, 0, 0, 0, 0) /* TCP */,
    //                       build_default_ports(ports_b, 8000, 0, 0, 0, 0) /* UDP */);

    //    set_proto_defaults(ndpi_struct, NDPI_PROTOCOL_FUN, NDPI_PROTOCOL_QQ_CHAT,
    //                       NDPI_PROTOCOL_QQ_CHAT, "qq视频聊天",
    //                       build_default_ports(ports_a, 0, 0, 0, 0, 0) /* TCP */,
    //                       build_default_ports(ports_b, 0, 0, 0, 0, 0) /* UDP */);

    ndpi_set_bitmask_protocol_detection("QQID", ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_OICQ,
                                        xs_search_oicq,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_OR_UDP_WITH_PAYLOAD,
                                        SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);

    *id += 1;
}

#endif
