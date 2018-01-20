/*
 * mail_smtp.c
 *
 * Copyright (C) 2009-2011 by ipoque GmbH
 * Copyright (C) 2011-15 - ntop.org
 *
 * This file is part of nDPI, an open source deep packet inspection
 * library based on the OpenDPI and PACE technology by ipoque GmbH
 *
 * nDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "ndpi_protocols.h"
#include <stdlib.h>

#define SMTP_HTONS25  6400 //byzpl

#ifdef NDPI_PROTOCOL_MAIL_SMTP

#define SMTP_BIT_220		0x01
#define SMTP_BIT_250		0x02
#define SMTP_BIT_235		0x04
#define SMTP_BIT_334		0x08
#define SMTP_BIT_354		0x10
#define SMTP_BIT_HELO_EHLO	0x20
#define SMTP_BIT_MAIL		0x40
#define SMTP_BIT_RCPT		0x80
#define SMTP_BIT_AUTH		0x100
#define SMTP_BIT_STARTTLS	0x200
#define SMTP_BIT_DATA		0x400
#define SMTP_BIT_NOOP		0x800
#define SMTP_BIT_RSET		0x1000
#define SMTP_BIT_TlRM		0x2000

static void ndpi_int_mail_smtp_add_connection(struct ndpi_detection_module_struct
                                              *ndpi_struct, struct ndpi_flow_struct *flow)
{
    ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_MAIL_SMTP, NDPI_PROTOCOL_UNKNOWN);
}

void ndpi_search_mail_smtp_tcp(struct ndpi_detection_module_struct
                               *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;
    /*~~~~~~~~~~~~~~~~~~~~~~byzpl~~~~~~~~~~~~~~~~~~~~~~~*/
    if((SMTP_HTONS25==packet->tcp->dest||SMTP_HTONS25==packet->tcp->source)&&packet->payload_packet_len>7){
        if((packet->payload[0] == 'M' || packet->payload[0] == 'm')//4.客户端分别先后向服务器发送“MAIL FROM”和“RCPT TO”命令，
                && (packet->payload[1] == 'A' || packet->payload[1] == 'a')// 后面分别加上发件人的邮箱地址和收件人的邮箱地址，服务器分别回应“250 Mail OK”表示成功接受
                && (packet->payload[2] == 'I' || packet->payload[2] == 'i')
                && (packet->payload[3] == 'L' || packet->payload[3] == 'l')
                && packet->payload[4] == ' '){
            char *p,*q;
            char * payload=(char *)packet->payload;
            p=strstr(payload,"<");
            q=strstr(payload,">");

            memcpy(flow->virtualnum.szNum,p+1,q-p-1);
            flow->virtualnum.type=virtual_mail;
            flow->virtualnum.status=vrt_method_online;
            flow->virtualnum.proType=mail_smtp;
        }

        if((packet->payload[0] == 'R' || packet->payload[0] == 'r')
                && (packet->payload[1] == 'C' || packet->payload[1] == 'c')
                && (packet->payload[2] == 'P' || packet->payload[2] == 'p')
                && (packet->payload[3] == 'T' || packet->payload[3] == 't')
                && packet->payload[4] == ' '){
            char *p,*q;
            char * payload=(char *)packet->payload;
            p=strstr(payload,"<");
            q=strstr(payload,">");

            memcpy(flow->virtualnum.mail_rcv,p+1,q-p-1);
            flow->virtualnum.type=virtual_mail;
            flow->virtualnum.status=vrt_method_online;
            flow->virtualnum.proType=mail_smtp;
        }
    }
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    //  struct ndpi_id_struct         *src=ndpi_struct->src;
    //  struct ndpi_id_struct         *dst=ndpi_struct->dst;

    NDPI_LOG(NDPI_PROTOCOL_MAIL_SMTP, ndpi_struct, NDPI_LOG_DEBUG, "search mail_smtp.\n");
    if (packet->payload_packet_len > 2 && ntohs(get_u_int16_t(packet->payload, packet->payload_packet_len - 2)) == 0x0d0a) {//3338=0x0d0a ??
        u_int8_t a;
        u_int8_t bit_count = 0;

        NDPI_PARSE_PACKET_LINE_INFO(ndpi_struct, flow,packet);
        for (a = 0; a < packet->parsed_lines; a++){

            // expected server responses  服务器响应
            if (packet->line[a].len >= 3) {
                if (memcmp(packet->line[a].ptr, "220", 3) == 0) {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_220;
                } else if (memcmp(packet->line[a].ptr, "250", 3) == 0) {//1.250表示服务器可用
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_250;
                } else if (memcmp(packet->line[a].ptr, "235", 3) == 0) {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_235;
                } else if (memcmp(packet->line[a].ptr, "334", 3) == 0) {//2.334表示接受
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_334;
                } else if (memcmp(packet->line[a].ptr, "354", 3) == 0) {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_354;
                }
            }
            // expected client requests  客户端请求
            if (packet->line[a].len >= 5) {
                if ((((packet->line[a].ptr[0] == 'H' || packet->line[a].ptr[0] == 'h')
                      && (packet->line[a].ptr[1] == 'E' || packet->line[a].ptr[1] == 'e'))//1.客户端向服务器发送命令“HELO”，并加上本机的主机名
                     || ((packet->line[a].ptr[0] == 'E' || packet->line[a].ptr[0] == 'e')
                         && (packet->line[a].ptr[1] == 'H' || packet->line[a].ptr[1] == 'h')))
                        && (packet->line[a].ptr[2] == 'L' || packet->line[a].ptr[2] == 'l')
                        && (packet->line[a].ptr[3] == 'O' || packet->line[a].ptr[3] == 'o')
                        && packet->line[a].ptr[4] == ' ') {                                                  //  3.客户端分别向服务器发送编码后的用户名和密码， 服务器分别回复“334”“235”表示接受
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_HELO_EHLO;
                } else if ((packet->line[a].ptr[0] == 'M' || packet->line[a].ptr[0] == 'm')//4.客户端分别先后向服务器发送“MAIL FROM”和“RCPT TO”命令，
                           && (packet->line[a].ptr[1] == 'A' || packet->line[a].ptr[1] == 'a')// 后面分别加上发件人的邮箱地址和收件人的邮箱地址，服务器分别回应“250 Mail OK”表示成功接受
                           && (packet->line[a].ptr[2] == 'I' || packet->line[a].ptr[2] == 'i')
                           && (packet->line[a].ptr[3] == 'L' || packet->line[a].ptr[3] == 'l')
                           && packet->line[a].ptr[4] == ' ') {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_MAIL;
                } else if ((packet->line[a].ptr[0] == 'R' || packet->line[a].ptr[0] == 'r')
                           && (packet->line[a].ptr[1] == 'C' || packet->line[a].ptr[1] == 'c')
                           && (packet->line[a].ptr[2] == 'P' || packet->line[a].ptr[2] == 'p')
                           && (packet->line[a].ptr[3] == 'T' || packet->line[a].ptr[3] == 't')
                           && packet->line[a].ptr[4] == ' ') {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_RCPT;
                } else if ((packet->line[a].ptr[0] == 'A' || packet->line[a].ptr[0] == 'a')//2.客户端向服务器发送用户登录命令“AUTH LOGIN”，
                           && (packet->line[a].ptr[1] == 'U' || packet->line[a].ptr[1] == 'u')
                           && (packet->line[a].ptr[2] == 'T' || packet->line[a].ptr[2] == 't')
                           && (packet->line[a].ptr[3] == 'H' || packet->line[a].ptr[3] == 'h')
                           && packet->line[a].ptr[4] == ' ') {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_AUTH;
                }
            }

            if (packet->line[a].len >= 8) {
                if ((packet->line[a].ptr[0] == 'S' || packet->line[a].ptr[0] == 's')
                        && (packet->line[a].ptr[1] == 'T' || packet->line[a].ptr[1] == 't')
                        && (packet->line[a].ptr[2] == 'A' || packet->line[a].ptr[2] == 'a')
                        && (packet->line[a].ptr[3] == 'R' || packet->line[a].ptr[3] == 'r')
                        && (packet->line[a].ptr[4] == 'T' || packet->line[a].ptr[4] == 't')
                        && (packet->line[a].ptr[5] == 'T' || packet->line[a].ptr[5] == 't')
                        && (packet->line[a].ptr[6] == 'L' || packet->line[a].ptr[6] == 'l')
                        && (packet->line[a].ptr[7] == 'S' || packet->line[a].ptr[7] == 's')) {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_STARTTLS;
                }
            }

            if (packet->line[a].len >= 4) {
                if ((packet->line[a].ptr[0] == 'D' || packet->line[a].ptr[0] == 'd')//5.接下来客户端向服务器发送命令“DATA”，表示将要向服务器发送邮件正文，
                        && (packet->line[a].ptr[1] == 'A' || packet->line[a].ptr[1] == 'a')//服务器回应“354 End data with <CR><LF>.<CR><LF>”表示同意接收
                        && (packet->line[a].ptr[2] == 'T' || packet->line[a].ptr[2] == 't')
                        && (packet->line[a].ptr[3] == 'A' || packet->line[a].ptr[3] == 'a')) {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_DATA;
                } else if ((packet->line[a].ptr[0] == 'N' || packet->line[a].ptr[0] == 'n')
                           && (packet->line[a].ptr[1] == 'O' || packet->line[a].ptr[1] == 'o')
                           && (packet->line[a].ptr[2] == 'O' || packet->line[a].ptr[2] == 'o')
                           && (packet->line[a].ptr[3] == 'P' || packet->line[a].ptr[3] == 'p')) {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_NOOP;
                } else if ((packet->line[a].ptr[0] == 'R' || packet->line[a].ptr[0] == 'r')
                           && (packet->line[a].ptr[1] == 'S' || packet->line[a].ptr[1] == 's')
                           && (packet->line[a].ptr[2] == 'E' || packet->line[a].ptr[2] == 'e')
                           && (packet->line[a].ptr[3] == 'T' || packet->line[a].ptr[3] == 't')) {
                    flow->l4.tcp.smtp_command_bitmask |= SMTP_BIT_RSET;
                }
            }

        }

        // now count the bits set in the bitmask
        if (flow->l4.tcp.smtp_command_bitmask != 0) {
            for (a = 0; a < 16; a++) {
                bit_count += (flow->l4.tcp.smtp_command_bitmask >> a) & 0x01;
            }
        }
        NDPI_LOG(NDPI_PROTOCOL_MAIL_SMTP, ndpi_struct, NDPI_LOG_DEBUG, "seen smtp commands and responses: %u.\n",
                 bit_count);

        if (bit_count >= 3) {
            NDPI_LOG(NDPI_PROTOCOL_MAIL_SMTP, ndpi_struct, NDPI_LOG_DEBUG, "mail smtp identified\n");
            ndpi_int_mail_smtp_add_connection(ndpi_struct, flow);
            return;
        }
        if (bit_count >= 1 && flow->packet_counter < 12) {
            return;
        }
    }//当第一个或第二个包被分成两个包时，这些包被忽略。
    /* when the first or second packets are split into two packets, those packets are ignored. */
    if (flow->packet_counter <= 4 &&
            packet->payload_packet_len >= 4 &&
            (ntohs(get_u_int16_t(packet->payload, packet->payload_packet_len - 2)) == 0x0d0a
             || memcmp(packet->payload, "220", 3) == 0 || memcmp(packet->payload, "EHLO", 4) == 0)) {
        NDPI_LOG(NDPI_PROTOCOL_MAIL_SMTP, ndpi_struct, NDPI_LOG_DEBUG, "maybe SMTP, need next packet.\n");
        return;
    }

    NDPI_LOG(NDPI_PROTOCOL_MAIL_SMTP, ndpi_struct, NDPI_LOG_DEBUG, "exclude smtp\n");
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_MAIL_SMTP);

}

void init_mail_smtp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id, NDPI_PROTOCOL_BITMASK *detection_bitmask)
{
    ndpi_set_bitmask_protocol_detection("MAIL_SMTP", ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_MAIL_SMTP,
                                        ndpi_search_mail_smtp_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD_WITHOUT_RETRANSMISSION,
                                        SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);

    *id += 1;
}

#endif