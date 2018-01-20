/*
 * http.c
 *
 * Copyright (C) 2011-17 - ntop.org
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
#include "lib/xshash_1.h"


#include<stdlib.h>
#ifdef NDPI_PROTOCOL_HTTP


/* global variables used for 1kxun protocol and iqiyi service */
static u_int16_t kxun_counter;
static u_int16_t iqiyi_counter;

static void ndpi_int_http_add_connection(struct ndpi_detection_module_struct *ndpi_struct,
                                         struct ndpi_flow_struct *flow,
                                         u_int32_t protocol) {

    if(flow->detected_protocol_stack[0] == NDPI_PROTOCOL_UNKNOWN) {
        /* This is HTTP and it is not a sub protocol (e.g. skype or dropbox) */

        ndpi_search_tcp_or_udp(ndpi_struct, flow);

        /* If no custom protocol has been detected */
        /* if(flow->detected_protocol_stack[0] == NDPI_PROTOCOL_UNKNOWN) */
        if(protocol == NDPI_PROTOCOL_HTTP) {
            ndpi_int_reset_protocol(flow);
            ndpi_set_detected_protocol(ndpi_struct, flow, flow->guessed_host_protocol_id, protocol);
        } else
            ndpi_set_detected_protocol(ndpi_struct, flow, protocol, NDPI_PROTOCOL_HTTP);

        flow->http_detected = 1;
    }
}

#ifdef NDPI_CONTENT_FLASH
static void flash_check_http_payload(struct ndpi_detection_module_struct
                                     *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;
    const u_int8_t *pos;

    if(packet->empty_line_position_set == 0 || (packet->empty_line_position + 10) > (packet->payload_packet_len))
        return;

    pos = &packet->payload[packet->empty_line_position] + 2;

    if(memcmp(pos, "FLV", 3) == 0 && pos[3] == 0x01 && (pos[4] == 0x01 || pos[4] == 0x04 || pos[4] == 0x05)
            && pos[5] == 0x00 && pos[6] == 0x00 && pos[7] == 0x00 && pos[8] == 0x09) {

        NDPI_LOG(NDPI_CONTENT_FLASH, ndpi_struct, NDPI_LOG_DEBUG, "Flash content in http detected\n");
        ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_CONTENT_FLASH);
    }
}
#endif

#ifdef NDPI_CONTENT_AVI
static void avi_check_http_payload(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;


    NDPI_LOG(NDPI_CONTENT_AVI, ndpi_struct, NDPI_LOG_DEBUG, "called avi_check_http_payload: %u %u %u\n",
             packet->empty_line_position_set, flow->l4.tcp.http_empty_line_seen, packet->empty_line_position);

    if(packet->empty_line_position_set == 0 && flow->l4.tcp.http_empty_line_seen == 0)
        return;

    if(packet->empty_line_position_set != 0 && ((packet->empty_line_position + 20) > (packet->payload_packet_len))
            && flow->l4.tcp.http_empty_line_seen == 0) {
        flow->l4.tcp.http_empty_line_seen = 1;
        return;
    }

    if(flow->l4.tcp.http_empty_line_seen == 1) {
        if(packet->payload_packet_len > 20 && memcmp(packet->payload, "RIFF", 4) == 0
                && memcmp(packet->payload + 8, "AVI LIST", 8) == 0) {
            NDPI_LOG(NDPI_CONTENT_AVI, ndpi_struct, NDPI_LOG_DEBUG, "Avi content in http detected\n");
            ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_CONTENT_AVI);
        }
        flow->l4.tcp.http_empty_line_seen = 0;
        return;
    }

    /**
     for reference see http://msdn.microsoft.com/archive/default.asp?url=/archive/en-us/directx9_c/directx/htm/avirifffilereference.asp
  **/
    if(packet->empty_line_position_set != 0) {

        u_int32_t p = packet->empty_line_position + 2;

        // check for avi header
        NDPI_LOG(NDPI_CONTENT_AVI, ndpi_struct, NDPI_LOG_DEBUG, "p = %u\n", p);

        if((p + 16) <= packet->payload_packet_len && memcmp(&packet->payload[p], "RIFF", 4) == 0
                && memcmp(&packet->payload[p + 8], "AVI LIST", 8) == 0) {
            NDPI_LOG(NDPI_CONTENT_AVI, ndpi_struct, NDPI_LOG_DEBUG, "Avi content in http detected\n");
            ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_CONTENT_AVI);
        }
    }
}
#endif

#ifdef NDPI_PROTOCOL_TEAMVIEWER
static void teamviewer_check_http_payload(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;
    const u_int8_t *pos;

    NDPI_LOG(NDPI_PROTOCOL_TEAMVIEWER, ndpi_struct, NDPI_LOG_DEBUG, "called teamviewer_check_http_payload: %u %u %u\n",
             packet->empty_line_position_set, flow->l4.tcp.http_empty_line_seen, packet->empty_line_position);

    if(packet->empty_line_position_set == 0 || (packet->empty_line_position + 5) > (packet->payload_packet_len))
        return;

    pos = &packet->payload[packet->empty_line_position] + 2;

    if(pos[0] == 0x17 && pos[1] == 0x24) {
        NDPI_LOG(NDPI_PROTOCOL_TEAMVIEWER, ndpi_struct, NDPI_LOG_DEBUG, "TeamViewer content in http detected\n");
        ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_TEAMVIEWER);
    }
}
#endif


#ifdef NDPI_PROTOCOL_RTSP
static void rtsp_parse_packet_acceptline(struct ndpi_detection_module_struct
                                         *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;

    if(packet->accept_line.len >= 28 && memcmp(packet->accept_line.ptr, "application/x-rtsp-tunnelled", 28) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_RTSP, ndpi_struct, NDPI_LOG_DEBUG, "RTSP accept line detected\n");
        ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_RTSP);
    }
}
#endif

static void setHttpUserAgent(struct ndpi_flow_struct *flow, char *ua) {
    if(!strcmp(ua, "Windows NT 5.0")) ua = "Windows 2000";
    else if(!strcmp(ua, "Windows NT 5.1")) ua = "Windows XP";
    else if(!strcmp(ua, "Windows NT 5.2")) ua = "Windows Server 2003";
    else if(!strcmp(ua, "Windows NT 6.0")) ua = "Windows Vista";
    else if(!strcmp(ua, "Windows NT 6.1")) ua = "Windows 7";
    else if(!strcmp(ua, "Windows NT 6.2")) ua = "Windows 8";
    else if(!strcmp(ua, "Windows NT 6.3")) ua = "Windows 8.1";

    // printf("==> %s\n", ua);
    snprintf((char*)flow->detected_os, sizeof(flow->detected_os), "%s", ua);
}

static void parseHttpSubprotocol(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow) {
    if((flow->l4.tcp.http_stage == 0) || (flow->http.url && flow->http_detected)) {
        char *double_col = strchr((char*)flow->host_server_name, ':');

        if(double_col) double_col[0] = '\0';

        /**
       NOTE
       If http_dont_dissect_response = 1 dissection of HTTP response
       mime types won't happen
    */
        ndpi_match_host_subprotocol(ndpi_struct, flow, (char *)flow->host_server_name,
                                    strlen((const char *)flow->host_server_name),
                                    NDPI_PROTOCOL_HTTP);
    }
}

/**
   NOTE
   ndpi_parse_packet_line_info is in ndpi_main.c
*/
static void check_content_type_and_change_protocol(struct ndpi_detection_module_struct *ndpi_struct,
                                                   struct ndpi_flow_struct *flow) {

    struct ndpi_packet_struct *packet = &flow->packet;
    u_int8_t a;


#ifdef NDPI_PROTOCOL_PPSTREAM
    /* PPStream */
    if(flow->l4.tcp.ppstream_stage > 0 && iqiyi_counter == 0) {
        NDPI_LOG(NDPI_PROTOCOL_PPSTREAM, ndpi_struct, NDPI_LOG_DEBUG, "PPStream found.\n");
        /* ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_PPSTREAM); */
        ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_PPSTREAM, NDPI_PROTOCOL_HTTP);
    }
    else if(iqiyi_counter > 0) {
        NDPI_LOG(NDPI_PROTOCOL_IQIYI, ndpi_struct, NDPI_LOG_DEBUG, "iQiyi found.\n");
        /* ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_IQIYI); */
        ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_IQIYI, NDPI_PROTOCOL_HTTP);
    }
#endif

#ifdef NDPI_PROTOCOL_1KXUN
    /* 1KXUN */
    if(kxun_counter > 0) {
        NDPI_LOG(NDPI_PROTOCOL_1KXUN, ndpi_struct, NDPI_LOG_DEBUG, "1kxun found.\n");
        /* ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_1KXUN); */
        ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_1KXUN, NDPI_PROTOCOL_HTTP);
    }
#endif

    if(!ndpi_struct->http_dont_dissect_response) {
        if((flow->http.url == NULL)
                && (packet->http_url_name.len > 0)
                && (packet->host_line.len > 0)) {
            int len = packet->http_url_name.len + packet->host_line.len + 7 + 1; /* "http://" */

            flow->http.url = ndpi_malloc(len);
            if(flow->http.url) {
                strncpy(flow->http.url, "http://", 7);
                strncpy(&flow->http.url[7], (char*)packet->host_line.ptr, packet->host_line.len);
                strncpy(&flow->http.url[7+packet->host_line.len], (char*)packet->http_url_name.ptr,
                        packet->http_url_name.len);
                flow->http.url[len-1] = '\0';
            }

            if(flow->packet.http_method.len < 3)
                flow->http.method = HTTP_METHOD_UNKNOWN;
            else {
                switch(flow->packet.http_method.ptr[0]) {
                case 'O':  flow->http.method = HTTP_METHOD_OPTIONS; break;
                case 'G':  flow->http.method = HTTP_METHOD_GET; break;
                case 'H':  flow->http.method = HTTP_METHOD_HEAD; break;

                case 'P':
                    switch(flow->packet.http_method.ptr[1]) {
                    case 'O': flow->http.method = HTTP_METHOD_POST; break;
                    case 'U': flow->http.method = HTTP_METHOD_PUT; break;
                    }
                    break;

                case 'D':   flow->http.method = HTTP_METHOD_DELETE; break;
                case 'T':   flow->http.method = HTTP_METHOD_TRACE; break;
                case 'C':   flow->http.method = HTTP_METHOD_CONNECT; break;
                default:
                    flow->http.method = HTTP_METHOD_UNKNOWN;
                    break;
                }
            }
        }

        if((flow->http.content_type == NULL) && (packet->content_line.len > 0)) {
            int len = packet->content_line.len + 1;

            flow->http.content_type = ndpi_malloc(len);
            if(flow->http.content_type) {
                strncpy(flow->http.content_type, (char*)packet->content_line.ptr,
                        packet->content_line.len);
                flow->http.content_type[packet->content_line.len] = '\0';
            }
        }
    }

    if(packet->user_agent_line.ptr != NULL && packet->user_agent_line.len != 0) {
        /**
       Format:
       Mozilla/5.0 (iPad; U; CPU OS 3_2 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) ....
    */
        if(packet->user_agent_line.len > 7) {
            char ua[256];
            u_int mlen = ndpi_min(packet->user_agent_line.len, sizeof(ua)-1);

            strncpy(ua, (const char *)packet->user_agent_line.ptr, mlen);
            ua[mlen] = '\0';

            if(strncmp(ua, "Mozilla", 7) == 0) {
                char *parent = strchr(ua, '(');

                if(parent){
                    char *token, *end,*p,*version;

                    parent++;
                    end = strchr(parent, ')');
                    if(end) end[0] = '\0';

                    token = strsep(&parent, ";");
                    if(token) {
                        if((strcmp(token, "X11") == 0)
                                || (strcmp(token, "compatible") == 0)
                                || (strcmp(token, "Linux") == 0)
                                || (strcmp(token, "Macintosh") == 0)
                                ) {
                            token = strsep(&parent, ";");
                            if(token && (token[0] == ' ')) token++; /* Skip space */

                            if(token
                                    && ((strcmp(token, "U") == 0)
                                        || (strncmp(token, "MSIE", 4) == 0))) {
                                token = strsep(&parent, ";");
                                if(token && (token[0] == ' ')) token++; /* Skip space */

                                if(token && (strncmp(token, "Update", 6)  == 0)) {
                                    token = strsep(&parent, ";");

                                    if(token && (token[0] == ' ')) token++; /* Skip space */

                                    if(token && (strncmp(token, "AOL", 3)  == 0)) {
                                        token = strsep(&parent, ";");

                                        if(token && (token[0] == ' ')) token++; /* Skip space */
                                    }
                                }
                            }
                        }

                        if(token)
                            setHttpUserAgent(flow, token);
                    }
                    //
                    if(token && (strncmp(token, "iPhone", 6)  == 0)) {
                        p=strsep(&parent,"OS");
                        if(p&&parent){
                            version=strsep(&parent,"like");
                            if(version&&parent){
                                version++;
                                snprintf((char*)flow->detected_os, 31, "%s+OS%s", token,version);
                            }
                        }
                    }
                    //
                }
            } //else if(memcmp(ua, "Dalvik", 6) == 0) {
            //                    char *parent = strchr(ua, '(');
            //                    if(parent) {
            //                        char *token, *end;

            //                        parent++;
            //                        end = strchr(parent, ')');
            //                        if(end) end[0] = '\0';

            //                        token = strsep(&parent, ";");
            //                        if(token) {
            //                            if((strcmp(token, "X11") == 0)
            //                                    || (strcmp(token, "compatible") == 0)
            //                                    || (strcmp(token, "Linux") == 0)
            //                                    || (strcmp(token, "Macintosh") == 0)
            //                                    ) {
            //                                token = strsep(&parent, ";");
            //                                if(token && (token[0] == ' ')) token++; /* Skip space */

            //                                if(token
            //                                        && ((strcmp(token, "U") == 0)
            //                                            || (strncmp(token, "MSIE", 4) == 0))) {
            //                                    token = strsep(&parent, ";");
            //                                    if(token && (token[0] == ' ')) token++; /* Skip space */

            //                                    if(token && (strncmp(token, "Update", 6)  == 0)) {
            //                                        token = strsep(&parent, ";");

            //                                        if(token && (token[0] == ' ')) token++; /* Skip space */

            //                                        if(token && (strncmp(token, "AOL", 3)  == 0)) {
            //                                            token = strsep(&parent, ";");

            //                                            if(token && (token[0] == ' ')) token++; /* Skip space */
            //                                        }
            //                                    }
            //                                }
            //                            }

            //                            if(token)
            //                                setHttpUserAgent(flow, token);
            //                        }
            //                    }
            //  }
            else if(memcmp(ua, "netflix-ios-app", 15) == 0) {
                ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_NETFLIX);
                return;
            }
        }

        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "User Agent Type Line found %.*s\n",
                 packet->user_agent_line.len, packet->user_agent_line.ptr);
    }

    /* check for host line */
    if(packet->host_line.ptr != NULL) {
        u_int len;

        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HOST Line found %.*s\n",
                 packet->host_line.len, packet->host_line.ptr);

        /* call ndpi_match_host_subprotocol to see if there is a match with known-host http subprotocol */
        if((ndpi_struct->http_dont_dissect_response) || flow->http_detected)
            ndpi_match_host_subprotocol(ndpi_struct, flow,
                                        (char*)packet->host_line.ptr,
                                        packet->host_line.len,
                                        NDPI_PROTOCOL_HTTP);

        /* Copy result for nDPI apps */
        len = ndpi_min(packet->host_line.len, sizeof(flow->host_server_name)-1);
        strncpy((char*)flow->host_server_name, (char*)packet->host_line.ptr, len);
        flow->host_server_name[len] = '\0', flow->server_id = flow->dst;

        len = ndpi_min(packet->forwarded_line.len, sizeof(flow->nat_ip)-1);
        strncpy((char*)flow->nat_ip, (char*)packet->forwarded_line.ptr, len);
        flow->nat_ip[len] = '\0';

        if(ndpi_struct->http_dont_dissect_response)
            parseHttpSubprotocol(ndpi_struct, flow);

        /**
       check result of host subprotocol detection

       if "detected" in flow == 0 then "detected" = "guess"
       else "guess" = "detected"
    **/
        if(flow->detected_protocol_stack[1] == 0) {
            flow->detected_protocol_stack[1] = flow->guessed_protocol_id;
            if(flow->detected_protocol_stack[0] == 0)
                flow->detected_protocol_stack[0] = flow->guessed_host_protocol_id;
        }
        else {
            if(flow->detected_protocol_stack[1] != flow->guessed_protocol_id)
                flow->guessed_protocol_id = flow->detected_protocol_stack[1];
            if(flow->detected_protocol_stack[0] != flow->guessed_host_protocol_id)
                flow->guessed_host_protocol_id = flow->detected_protocol_stack[0];
        }

        if((flow->detected_protocol_stack[0] == NDPI_PROTOCOL_UNKNOWN)
                && ((ndpi_struct->http_dont_dissect_response) || flow->http_detected)
                && (packet->http_origin.len > 0))
            ndpi_match_host_subprotocol(ndpi_struct, flow,
                                        (char *)packet->http_origin.ptr,
                                        packet->http_origin.len,
                                        NDPI_PROTOCOL_HTTP);

        if(flow->detected_protocol_stack[0] != NDPI_PROTOCOL_UNKNOWN) {
            if(packet->detected_protocol_stack[0] != NDPI_PROTOCOL_HTTP) {
                ndpi_int_http_add_connection(ndpi_struct, flow, packet->detected_protocol_stack[0]);
                return; /* We have identified a sub-protocol so we're done */
            }
        }
    }

    if(!ndpi_struct->http_dont_dissect_response && flow->http_detected)
        parseHttpSubprotocol(ndpi_struct, flow);

    if(flow->guessed_protocol_id == NDPI_PROTOCOL_UNKNOWN)
        flow->guessed_protocol_id = NDPI_PROTOCOL_HTTP;

    /* check for accept line */
    if(packet->accept_line.ptr != NULL) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "Accept Line found %.*s\n",
                 packet->accept_line.len, packet->accept_line.ptr);
#ifdef NDPI_PROTOCOL_RTSP
        if(NDPI_COMPARE_PROTOCOL_TO_BITMASK(ndpi_struct->detection_bitmask, NDPI_PROTOCOL_RTSP) != 0) {
            rtsp_parse_packet_acceptline(ndpi_struct, flow);
        }
#endif
    }

    /* search for line startin with "Icy-MetaData" */
#ifdef NDPI_CONTENT_MPEG
    for (a = 0; a < packet->parsed_lines; a++) {
        if(packet->line[a].len > 11 && memcmp(packet->line[a].ptr, "Icy-MetaData", 12) == 0) {
            NDPI_LOG(NDPI_CONTENT_MPEG, ndpi_struct, NDPI_LOG_DEBUG, "MPEG: Icy-MetaData found.\n");
            ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_CONTENT_MPEG);
            return;
        }
    }
#ifdef NDPI_CONTENT_AVI
#endif
#endif

    if(packet->content_line.ptr != NULL && packet->content_line.len != 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "Content Type Line found %.*s\n",
                 packet->content_line.len, packet->content_line.ptr);

        if((ndpi_struct->http_dont_dissect_response) || flow->http_detected)
            ndpi_match_content_subprotocol(ndpi_struct, flow,
                                           (char*)packet->content_line.ptr, packet->content_line.len,
                                           NDPI_PROTOCOL_HTTP);
    }
}

static void check_http_payload(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "called check_http_payload.\n");

#ifdef NDPI_CONTENT_FLASH
    if(NDPI_COMPARE_PROTOCOL_TO_BITMASK(ndpi_struct->detection_bitmask, NDPI_CONTENT_FLASH) != 0)
        flash_check_http_payload(ndpi_struct, flow);
#endif
#ifdef NDPI_CONTENT_AVI
    if(NDPI_COMPARE_PROTOCOL_TO_BITMASK(ndpi_struct->detection_bitmask, NDPI_CONTENT_AVI) != 0)
        avi_check_http_payload(ndpi_struct, flow);
#endif
#ifdef NDPI_PROTOCOL_TEAMVIEWER
    teamviewer_check_http_payload(ndpi_struct, flow);
#endif

}

/**
 * Functions to check whether the packet begins with a valid http request
 * @param ndpi_struct
 * @returnvalue 0 if no valid request has been found
 * @returnvalue >0 indicates start of filename but not necessarily in packet limit
 */
static u_int16_t http_request_url_offset(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;

    NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "====>>>> HTTP: %c%c%c%c [len: %u]\n",
             packet->payload[0], packet->payload[1], packet->payload[2], packet->payload[3],
            packet->payload_packet_len);

    /**
     FIRST PAYLOAD PACKET FROM CLIENT
  **/

    /* check if the packet starts with POST or GET */
    if(packet->payload_packet_len >= 4 && memcmp(packet->payload, "GET ", 4) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: GET FOUND\n");
        return 4;
    } else if(packet->payload_packet_len >= 5 && memcmp(packet->payload, "POST ", 5) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: POST FOUND\n");
        return 5;
    } else if(packet->payload_packet_len >= 8 && memcmp(packet->payload, "OPTIONS ", 8) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: OPTIONS FOUND\n");
        return 8;
    } else if(packet->payload_packet_len >= 5 && memcmp(packet->payload, "HEAD ", 5) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: HEAD FOUND\n");
        return 5;
    } else if(packet->payload_packet_len >= 4 && memcmp(packet->payload, "PUT ", 4) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: PUT FOUND\n");
        return 4;
    } else if(packet->payload_packet_len >= 7 && memcmp(packet->payload, "DELETE ", 7) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: DELETE FOUND\n");
        return 7;
    } else if(packet->payload_packet_len >= 8 && memcmp(packet->payload, "CONNECT ", 8) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: CONNECT FOUND\n");
        return 8;
    } else if(packet->payload_packet_len >= 9 && memcmp(packet->payload, "PROPFIND ", 9) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: PROFIND FOUND\n");
        return 9;
    } else if(packet->payload_packet_len >= 7 && memcmp(packet->payload, "REPORT ", 7) == 0) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: REPORT FOUND\n");
        return 7;
    }

    return 0;
}

static void http_bitmask_exclude(struct ndpi_flow_struct *flow)
{
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_HTTP);
#ifdef NDPI_CONTENT_MPEG
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_CONTENT_MPEG);
#endif
#ifdef NDPI_CONTENT_QUICKTIME
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_CONTENT_QUICKTIME);
#endif
#ifdef NDPI_CONTENT_WINDOWSMEDIA
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_CONTENT_WINDOWSMEDIA);
#endif
#ifdef NDPI_CONTENT_REALMEDIA
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_CONTENT_REALMEDIA);
#endif
#ifdef NDPI_CONTENT_AVI
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_CONTENT_AVI);
#endif
#ifdef NDPI_CONTENT_OGG
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_CONTENT_OGG);
#endif
#ifdef NDPI_PROTOCOL_MOVE
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_MOVE);
#endif
#ifdef NDPI_PROTOCOL_XBOX
    NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_XBOX);
#endif
}


/*************************************************************************************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~byzpl~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
char* xs_strnstr2(char* s1, char* s2, int l1,int l2)
{
    if (!l2)
        return (char *)s1;
    while (l1 >= l2) {
        l1--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~byzpl~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//get http_param  http_path
void parse_url_path_and_param(struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;
    if(packet->http_url_name.ptr)  {
        const u_char *ch;
        packet->http_path.ptr=packet->http_url_name.ptr;
        packet->http_path.len=packet->http_url_name.len;
        int i;
        for(i=0;i<packet->http_url_name.len;i++) {
            ch=packet->http_url_name.ptr+i;
            if((*ch == '?' || *ch == '#'))
            {
                packet->http_path.len=i;
                packet->http_param.ptr =ch;
                packet->http_param.len=packet->http_url_name.len-i;
                break;
            }
        }
    }
}

void get_file_hash_by_url(struct ndpi_flow_struct *flow,struct ndpi_packet_struct *packet){
    char pHttpHost[20] = { 0 };
    char pHttpUrlName[1500] = { 0 };
    char szUrl[1500] = { 0 };

    memcpy(pHttpHost,packet->host_line.ptr,packet->host_line.len);
    memcpy(pHttpUrlName,packet->http_url_name.ptr,packet->http_url_name.len);
    snprintf(szUrl,sizeof(szUrl)-1,"http://%s%s",pHttpHost,pHttpUrlName);

    memcpy( flow->virtualnum.szUrl,szUrl,MIN(strlen(szUrl),1500-1));
    flow->virtualnum.hashkey=CRC64((const u_char *)szUrl,strlen(szUrl));
}

//get qq picture url
void  get_qq_picture_url(struct ndpi_flow_struct*flow,struct ndpi_packet_struct *packet){
    flow->virtualnum.TypeMsg=msg_qq_picture;
    flow->virtualnum.type = virtual_qq;
    flow->virtualnum.status = vrt_method_online;

    char  *p,*ptr;
    ptr=(char*)packet->http_path.ptr;
    p=xs_strnstr2(ptr+9,"/",packet->http_path.len-9,1);
    if(p!=NULL){
        flow->virtualnum.id=strtoul(p+1,NULL,10);
    }

    get_file_hash_by_url(flow,packet);
}

// get qq file url
void  get_qq_file_url(struct ndpi_flow_struct*flow,struct ndpi_packet_struct *packet){
    flow->virtualnum.TypeMsg=msg_qq_file;
    flow->virtualnum.type = virtual_qq;
    flow->virtualnum.status = vrt_method_online;
    get_file_hash_by_url(flow,packet);
}

//get qq video url
void  get_qq_video_url(struct ndpi_flow_struct*flow,struct ndpi_packet_struct *packet){
    flow->virtualnum.TypeMsg=msg_qq_video;
    flow->virtualnum.type = virtual_qq;
    flow->virtualnum.status = vrt_method_online;
    get_file_hash_by_url(flow,packet);
}


//get Wechat video url(朋友圈加载视频)
void  get_wechat_video_url(struct ndpi_flow_struct*flow,struct ndpi_packet_struct *packet){
    flow->virtualnum.TypeMsg=msg_wechat_video;
    flow->virtualnum.type=virtual_weixin;
    flow->virtualnum.status = vrt_method_online;
    get_file_hash_by_url(flow,packet);
}

//get Wechat picture url(朋友圈加载图片(封面，头像))
void get_wechat_pic(struct ndpi_flow_struct*flow,struct ndpi_packet_struct *packet){
    flow->virtualnum.TypeMsg=msg_wechat_picture;
    flow->virtualnum.type=virtual_weixin;
    flow->virtualnum.status = vrt_method_online;

    get_wechatID_from_http_referer(flow,packet);

    get_file_hash_by_url(flow,packet);
}

static void XsFree_Sub_UserName(void *param)
{
    //    PConnectionInfo pConnInfo = (PConnectionInfo)pConn;
    char user[32];
    memset(user,0,sizeof(user));
    sscanf(param,"%[0-9a-zA-Z:,.?;<> !#%&*-+_=/]",user);
    //    BCOPY(user,pConnInfo->username,MaxUserNameLen);
}

//wangwang
int  get_wangwangID_from_http_param(struct ndpi_flow_struct*flow,struct ndpi_packet_struct *packet){
    if(packet->http_param.len<18)
        return -1 ;
    char *p=NULL,*q=NULL,*ptr=NULL;

    ptr=(char *)packet->http_param.ptr;

    if((q=xs_strnstr2(ptr+1,"login_id=cntaobao",packet->http_param.len-1,17))){//buyer  version
        u_int len = q - (ptr+1);
        p=xs_strnstr2(q+17,"&",packet->http_param.len-len-17,1);
        if(p!=NULL){
            len = p-q-17;
            if(len>sizeof(flow->virtualnum.szNum))
                len = sizeof(flow->virtualnum.szNum);
            memcpy(flow->virtualnum.szNum,q+17,len);//drop cntaobao  +8
        }
        else{
                     sscanf(q+17,"%s",flow->virtualnum.szNum);
          //  snprintf(flow->virtualnum.szNum,sizeof(flow->virtualnum.szNum),"%s",q+17);
        }

        flow->virtualnum.type = virtual_wangwang;
        flow->virtualnum.status = vrt_method_online;//login_id
        flow->virtualnum.wangVer=buyer_version;
        return 0;
    }else if((q=xs_strnstr2(ptr+1,"longId=cntaobao",packet->http_param.len-1,15))){//seller  version
        u_int len = q - (ptr + 1);
        p=xs_strnstr2(q+15,"&",packet->http_param.len-len-15,1);
        if(p!=NULL){
            len = p-q-15;
            if(len>sizeof(flow->virtualnum.szNum))
                len = sizeof(flow->virtualnum.szNum);
            memcpy(flow->virtualnum.szNum,q+15,len);
        } else{
                        sscanf(q+15,"%s",flow->virtualnum.szNum);
            //snprintf(flow->virtualnum.szNum,sizeof(flow->virtualnum.szNum),"%s",q+15);
        }

        flow->virtualnum.type = virtual_wangwang;
        flow->virtualnum.status = vrt_method_online;
        flow->virtualnum.wangVer=seller_version;
        return 0;
    }else if((q=xs_strnstr2(ptr+1,"loginId=cntaobao",packet->http_param.len-1,16))){//seller version
        u_int len = q - (ptr+1);
        p=xs_strnstr2(q+16,"&",packet->http_param.len-len-16,1);
        if(p != NULL){
            len = p-q-16;
            if(len>sizeof(flow->virtualnum.szNum))
                len = sizeof(flow->virtualnum.szNum);
            memcpy(flow->virtualnum.szNum,q+16,len);
        }
        else{
                       sscanf(q+16,"%s",flow->virtualnum.szNum);
          //  snprintf(flow->virtualnum.szNum,sizeof(flow->virtualnum.szNum),"%s",q+16);
        }

        flow->virtualnum.type = virtual_wangwang;
        flow->virtualnum.status = vrt_method_online;
        flow->virtualnum.wangVer=seller_version;
        return 0;
    }

    return -1;
}

// sina weibo
void  get_weiboID_from_http_x_log_uid(struct ndpi_flow_struct*flow,struct ndpi_packet_struct *packet){
    char *ptr;
    ptr = (char*)packet->http_log_uid.ptr;
    flow->virtualnum.type = virtual_weibo;
    flow->virtualnum.status = vrt_method_online;
    flow->virtualnum.id=strtoul(ptr,NULL,10);
}

//wechat
void
get_wechatID_from_http_referer(struct ndpi_flow_struct *flow,  struct ndpi_packet_struct *packet){
    char *q,*ptr;
    ptr = (char*)packet->referer_line.ptr;
    q=xs_strnstr2(ptr+20 ,"&uin=",packet->referer_line.len-20,5);
    if(q!=NULL){
        if(packet->referer_line.len - (q-ptr) > 5){
            q+=5;
            flow->virtualnum.type = virtual_weixin;
            flow->virtualnum.status = vrt_method_online;
            flow->virtualnum.id=strtoul(q,NULL,10);
        }
    }
}
//sina_mail ID
void get_sinamail_acount_from_http_cookie(struct ndpi_flow_struct *flow,  struct ndpi_packet_struct *packet){
    char  *p=NULL,*q=NULL,*ptr=NULL;
    ptr=(char *)packet->http_cookie.ptr;
    p=xs_strnstr2(ptr,"freeName=",packet->http_cookie.len,9);
    if(p!=NULL){
        u_int len = p-ptr;
        q=xs_strnstr2(p,"@sina.com",packet->http_cookie.len-len-9,9);
        if(q!=NULL){
            p+=9;
            len = q-p+9;
            if(len>sizeof(flow->virtualnum.szNum))
                len = sizeof(flow->virtualnum.szNum);
            memcpy(flow->virtualnum.szNum,p,len);
            flow->virtualnum.type=virtual_mail;
            flow->virtualnum.proType=mail_sina;

            if(packet->http_param.len>6&&(0==memcmp(packet->http_param.ptr+1,"logout",6)))
                flow->virtualnum.status=vrt_method_offline;
            else
                flow->virtualnum.status = vrt_method_online;
        }
    }
}

//sina_mail ID
void get_sinamail_acount_from_http_param(struct ndpi_flow_struct *flow,  struct ndpi_packet_struct *packet){
    if(packet->http_param.len<16)
        return;
    char *p=NULL,*q=NULL,*ptr=NULL ;
    ptr=(char *)packet->http_param.ptr;
    p=xs_strnstr2(ptr+1,"email=",packet->http_param.len-1,6);
    if(p != NULL){
        u_int len = p - (ptr+1);
        q=xs_strnstr2(p+6,"@sina.com",packet->http_param.len - len - 6,9);
        if(q != NULL){
            p+=6;
            len = q-p+9;
            if(len>sizeof(flow->virtualnum.szNum))
                len=sizeof(flow->virtualnum.szNum);
            memcpy(flow->virtualnum.szNum,p,len);
            flow->virtualnum.status = vrt_method_online;
            flow->virtualnum.type=virtual_mail;
            flow->virtualnum.proType=mail_sina;
        }
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void ndpi_check_http_tcp(struct ndpi_detection_module_struct *ndpi_struct,
                                struct ndpi_flow_struct *flow) {
    struct ndpi_packet_struct *packet = &flow->packet;
    u_int16_t filename_start;

    packet->packet_lines_parsed_complete = 0;

    /* Check if we so far detected the protocol in the request or not. */
    if(flow->l4.tcp.http_stage == 0) {
        flow->http_detected = 0;

        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP stage %d: \n",
                 flow->l4.tcp.http_stage);

        filename_start = http_request_url_offset(ndpi_struct, flow);

        if(filename_start == 0) {
            NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                     "Filename HTTP not found, we look for possible truncate flow...\n");

            if(packet->payload_packet_len >= 7 && memcmp(packet->payload, "HTTP/1.", 7) == 0) {
                NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                         "HTTP response found (truncated flow ?)\n");
                ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_HTTP);
                check_content_type_and_change_protocol(ndpi_struct, flow);
                return;
            }

            if((packet->payload_packet_len == 3) && memcmp(packet->payload, "HI\n", 3) == 0) {
                /* This looks like Ookla: we don't give up with HTTP yet */
                flow->l4.tcp.http_stage = 1;
                return;
            }

            if((packet->payload_packet_len == 23) && (memcmp(packet->payload, "<policy-file-request/>", 23) == 0)) {
                /*
      <policy-file-request/>
      <cross-domain-policy>
      <allow-access-from domain="*.ookla.com" to-ports="8080"/>
      <allow-access-from domain="*.speedtest.net" to-ports="8080"/>
      </cross-domain-policy>
    */
                ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_OOKLA, NDPI_PROTOCOL_UNKNOWN);
                return;
            }

            NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "Exclude HTTP\n");
            http_bitmask_exclude(flow);
            return;
        }

        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                 "Filename HTTP found: %d, we look for line info..\n", filename_start);

        ndpi_parse_packet_line_info(ndpi_struct, flow);
        //(wechat)
        if(packet->referer_line.len>25&&(memcmp(packet->referer_line.ptr+7,"weixin.qq.com",13)==0))
            get_wechatID_from_http_referer(flow,packet);
        //(sina weibo)
        if(packet->http_log_uid.len>0&&packet->host_line.len>14
                &&(0==memcmp(packet->host_line.ptr,"f.us.sinaimg.cn",15)) ){
            get_weiboID_from_http_x_log_uid(flow,packet);
        }

        if(packet->parsed_lines <= 1) {
            NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                     "Found just one line, we will look further for the next packet...\n");

            packet->http_method.ptr = packet->line[0].ptr;
            packet->http_method.len = filename_start - 1;

            /* Encode the direction of the packet in the stage, so we will know when we need to look for the response packet. */
            flow->l4.tcp.http_stage = packet->packet_direction + 1; // packet_direction 0: stage 1, packet_direction 1: stage 2
            return;
        }

        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                 "Found more than one line, we look further for the next packet...\n");

        if(packet->line[0].len >= (9 + filename_start)
                && memcmp(&packet->line[0].ptr[packet->line[0].len - 9], " HTTP/1.", 8) == 0) {

            packet->http_url_name.ptr = &packet->payload[filename_start];
            packet->http_url_name.len = packet->line[0].len - (filename_start + 9);

            //get  http_path http_param
            parse_url_path_and_param(flow);

            //get wangwang
            if(0 != get_wangwangID_from_http_param(flow,packet)){
                //get sina_mail ID
                get_sinamail_acount_from_http_param(flow,packet);//之前分析位置
            }
            //get sina_mail ID
            if(packet->host_line.len==16&&(0==memcmp(packet->host_line.ptr,"mail.sina.com.cn",16))//新浪微博11.22分析ID位置
                    &&packet->http_cookie.len>20)
                get_sinamail_acount_from_http_cookie(flow,packet);

            //get qq picture url..
            if(packet->host_line.len>=11&&(memcmp(packet->host_line.ptr,"shp.qpic.cn",11)==0))
                get_qq_picture_url(flow,packet);
            //get qq file url..
            if(packet->http_path.len>=12&&(memcmp(packet->http_path.ptr,"/ftn_handler",12)==0))
                get_qq_file_url(flow,packet);
            //get qq video url..
            if(packet->http_path.len>=11&&(memcmp(packet->http_path.ptr,"/qqdownload",11)==0))
                get_qq_video_url(flow,packet);
            //get wechat video url..
            if(packet->referer_line.len>=20&&(memcmp(packet->referer_line.ptr+7,"weixin.qq.com",13)==0)&&
                    packet->host_line.len==22&&(memcmp(packet->host_line.ptr,"szzjwxsns.video.qq.com",22)==0)    )
                get_wechat_video_url(flow,packet);
            //get wechat logo pic(头像)
            if(packet->referer_line.len>=20&&(memcmp(packet->referer_line.ptr+7,"weixin.qq.com",13)==0)&&
                    packet->host_line.len==11&&(memcmp(packet->host_line.ptr,"wx.qlogo.cn",11)==0)    )
                get_wechat_pic(flow,packet);
            //get  wechat  video first pictrue
            if(packet->referer_line.len>=20&&(memcmp(packet->referer_line.ptr+7,"weixin.qq.com",13)==0)&&
                    packet->host_line.len==22&&(memcmp(packet->host_line.ptr,"vweixinthumb.tc.qq.com",22)==0)    )
                get_wechat_pic(flow,packet);
            //get wechat pictrue(封面)
            if(packet->referer_line.len>=20&&(memcmp(packet->referer_line.ptr+7,"weixin.qq.com",13)==0)&&
                    packet->host_line.len==13&&(memcmp(packet->host_line.ptr,"mmsns.qpic.cn",13)==0))
                get_wechat_pic(flow,packet);

            packet->http_method.ptr = packet->line[0].ptr;
            packet->http_method.len = filename_start - 1;

            /* Check for Ookla */
            if((packet->referer_line.len > 0)
                    && ndpi_strnstr((const char *)packet->referer_line.ptr, "www.speedtest.net", packet->referer_line.len)) {
                ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_OOKLA, NDPI_PROTOCOL_HTTP);
                return;
            }

            /* Check for additional field introduced by Steam */
            int x = 1;
            if((memcmp(packet->line[x].ptr, "x-steam-sid", 11)) == 0){
                ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_STEAM);
                check_content_type_and_change_protocol(ndpi_struct, flow);
                return;
            }

            /* Check for additional field introduced by Facebook */
            x = 1;
            while(packet->line[x].len != 0) {
                if(packet->line[x].len >= 12 && (memcmp(packet->line[x].ptr, "X-FB-SIM-HNI", 12)) == 0) {
                    ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_FACEBOOK);
                    check_content_type_and_change_protocol(ndpi_struct, flow);
                    return;
                }
                x++;
            }

            /* check PPStream protocol or iQiyi service
     (iqiyi is delivered by ppstream) */
            // substring in url
            if(ndpi_strnstr((const char*) &packet->payload[filename_start], "iqiyi.com", (packet->payload_packet_len - filename_start)) != NULL) {
                if(kxun_counter == 0) {
                    flow->l4.tcp.ppstream_stage++;
                    iqiyi_counter++;
                    check_content_type_and_change_protocol(ndpi_struct, flow); /* ***** CHECK ****** */
                    return;
                }
            }
            // additional field in http payload
            x = 1;
            while((packet->line[x].len >= 4) && (packet->line[x+1].len >= 5) && (packet->line[x+2].len >= 10)) {
                if(packet->line[x].ptr && ((memcmp(packet->line[x].ptr, "qyid", 4)) == 0)
                        && packet->line[x+1].ptr && ((memcmp(packet->line[x+1].ptr, "qypid", 5)) == 0)
                        && packet->line[x+2].ptr && ((memcmp(packet->line[x+2].ptr, "qyplatform", 10)) == 0)
                        ) {
                    flow->l4.tcp.ppstream_stage++;
                    iqiyi_counter++;
                    check_content_type_and_change_protocol(ndpi_struct, flow);
                    return;
                }
                x++;
            }

            /* Check for 1kxun packet */
            int a;
            for (a = 0; a < packet->parsed_lines; a++) {
                if(packet->line[a].len >= 14 && (memcmp(packet->line[a].ptr, "Client-Source:", 14)) == 0) {
                    if((memcmp(packet->line[a].ptr+15, "1kxun", 5)) == 0) {
                        kxun_counter++;
                        check_content_type_and_change_protocol(ndpi_struct, flow);
                        return;
                    }
                }
            }

            if((packet->http_url_name.len > 7)
                    && (!strncmp((const char*) packet->http_url_name.ptr, "http://", 7))) {
                NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP_PROXY Found.\n");
                ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_HTTP_PROXY);
                check_content_type_and_change_protocol(ndpi_struct, flow);
            }

            if(filename_start == 8 && (memcmp(packet->payload, "CONNECT ", 8) == 0)) {
                /* nathan@getoffmalawn.com */
                NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP_CONNECT Found.\n");
                ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_HTTP_CONNECT);
                check_content_type_and_change_protocol(ndpi_struct, flow);
            }

            NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                     "HTTP START Found, we will look for sub-protocols (content and host)...\n");

            if(packet->host_line.ptr != NULL) {
                /**
       nDPI is pretty scrupulous about HTTP so it waits until the
       HTTP response is received just to check that it conforms
       with the HTTP specs. However this might be a waste of time as
       in 99.99% of the cases is like that.
    */

                if(ndpi_struct->http_dont_dissect_response) {
                    if(flow->detected_protocol_stack[0] == NDPI_PROTOCOL_UNKNOWN) /* No subprotocol found */
                        ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_HTTP);
                } else {
                    flow->http_detected = 1;
                    NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                             "HTTP START Found, we will look further for the response...\n");
                    flow->l4.tcp.http_stage = packet->packet_direction + 1; // packet_direction 0: stage 1, packet_direction 1: stage 2
                }

                check_content_type_and_change_protocol(ndpi_struct, flow);
                return;
            }
        }

        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP: REQUEST NOT HTTP CONFORM\n");
        http_bitmask_exclude(flow);
    } else if((flow->l4.tcp.http_stage == 1) || (flow->l4.tcp.http_stage == 2)) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP stage %u: \n",
                 flow->l4.tcp.http_stage);


        if(flow->l4.tcp.http_stage == 1) {
            if((packet->payload_packet_len > 6) && memcmp(packet->payload, "HELLO ", 6) == 0) {
                /* This looks like Ookla */
                ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_OOKLA, NDPI_PROTOCOL_UNKNOWN);
                return;
            } else
                NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_OOKLA);
        }

        /**
       At first check, if this is for sure a response packet (in another direction. If not, if http is detected do nothing now and return,
       otherwise check the second packet for the http request
    */
        if((flow->l4.tcp.http_stage - packet->packet_direction) == 1) {

            if(flow->http_detected)
                return;

            NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                     " SECOND PAYLOAD TRAFFIC FROM CLIENT, FIRST PACKET MIGHT HAVE BEEN HTTP...UNKNOWN TRAFFIC, HERE FOR HTTP again.. \n");

            ndpi_parse_packet_line_info(ndpi_struct, flow);

            if(packet->parsed_lines <= 1) {
                /* wait some packets in case request is split over more than 2 packets */
                if(flow->packet_counter < 5) {
                    NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                             "line still not finished, search next packet\n");
                    return;
                } else {
                    /* stop parsing here */
                    NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                             "HTTP: PACKET DOES NOT HAVE A LINE STRUCTURE\n");
                    http_bitmask_exclude(flow);
                    return;
                }
            }
            // http://www.slideshare.net/DSPIP/rtsp-analysis-wireshark
            if(packet->line[0].len >= 9
                    && memcmp(&packet->line[0].ptr[packet->line[0].len - 9], " HTTP/1.", 8) == 0) {

                NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "Found HTTP.\n");
                ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_HTTP);
                check_content_type_and_change_protocol(ndpi_struct, flow);

                NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG,
                         "HTTP START Found in 2. packet, we will look further for the response....\n");
                flow->http_detected = 1;
            }

            return;
        }

        /**
       This is a packet in another direction. Check if we find the proper response.
       We have received a response for a previously identified partial HTTP request
    */

        if((packet->parsed_lines == 1) && (packet->packet_direction == 1 /* server -> client */)) {
            /* In apache if you do "GET /\n\n" the response comes without any header */
            NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "Found HTTP. (apache)\n");
            ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_HTTP);
            check_content_type_and_change_protocol(ndpi_struct, flow);
            return;
        }

        /* If we already detected the http request, we can add the connection and then check for the sub-protocol */
        if(flow->http_detected)
            ndpi_int_http_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_HTTP);

        /* Parse packet line and we look for the subprotocols */
        ndpi_parse_packet_line_info(ndpi_struct, flow);
        check_content_type_and_change_protocol(ndpi_struct, flow);

        if(packet->empty_line_position_set != 0 || flow->l4.tcp.http_empty_line_seen == 1) {
            NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "empty line. check_http_payload.\n");
            check_http_payload(ndpi_struct, flow);
        }

        flow->l4.tcp.http_stage = 0;
        return;
    }

}

void ndpi_search_http_tcp(struct ndpi_detection_module_struct *ndpi_struct,
                          struct ndpi_flow_struct *flow){
    struct ndpi_packet_struct *packet = &flow->packet;

    /* Break after 20 packets. */
    if(flow->packet_counter > 20) {
        NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "Exclude HTTP.\n");
        http_bitmask_exclude(flow);
        return;
    }

    if(packet->detected_protocol_stack[0] != NDPI_PROTOCOL_UNKNOWN) {
        return;
    }

    NDPI_LOG(NDPI_PROTOCOL_HTTP, ndpi_struct, NDPI_LOG_DEBUG, "HTTP detection...\n");
    ndpi_check_http_tcp(ndpi_struct, flow);

}

/* ********************************* */

ndpi_http_method ndpi_get_http_method(struct ndpi_detection_module_struct *ndpi_mod,
                                      struct ndpi_flow_struct *flow) {
    if(!flow)
        return(HTTP_METHOD_UNKNOWN);
    else
        return(flow->http.method);
}

/* ********************************* */

char* ndpi_get_http_url(struct ndpi_detection_module_struct *ndpi_mod,
                        struct ndpi_flow_struct *flow) {
    if((!flow) || (!flow->http.url))
        return("");
    else
        return(flow->http.url);
}

/* ********************************* */

char* ndpi_get_http_content_type(struct ndpi_detection_module_struct *ndpi_mod,
                                 struct ndpi_flow_struct *flow) {
    if((!flow) || (!flow->http.content_type))
        return("");
    else
        return(flow->http.content_type);
}


void init_http_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id,
                         NDPI_PROTOCOL_BITMASK *detection_bitmask)
{
    ndpi_set_bitmask_protocol_detection("HTTP",ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_HTTP,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;

#if 0
    ndpi_set_bitmask_protocol_detection("HTTP_Proxy", ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_HTTP_PROXY,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;

#ifdef NDPI_CONTENT_MPEG
    ndpi_set_bitmask_protocol_detection("MPEG", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_MPEG,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);

    *id += 1;
#endif
#ifdef NDPI_CONTENT_FLASH
    ndpi_set_bitmask_protocol_detection("Flash", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_FLASH,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_CONTENT_QUICKTIME
    ndpi_set_bitmask_protocol_detection("QuickTime", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_QUICKTIME,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_CONTENT_REALMEDIA
    ndpi_set_bitmask_protocol_detection("RealMedia", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_REALMEDIA,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_CONTENT_WINDOWSMEDIA
    ndpi_set_bitmask_protocol_detection("WindowsMedia", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_WINDOWSMEDIA,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_CONTENT_MMS
    ndpi_set_bitmask_protocol_detection("MMS", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_MMS,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_PROTOCOL_XBOX
    ndpi_set_bitmask_protocol_detection("Xbox", ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_XBOX,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_PROTOCOL_QQ
    ndpi_set_bitmask_protocol_detection("QQ", ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_QQ,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_CONTENT_AVI
    ndpi_set_bitmask_protocol_detection("AVI", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_AVI,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_CONTENT_OGG
    ndpi_set_bitmask_protocol_detection("OggVorbis", ndpi_struct, detection_bitmask, *id,
                                        NDPI_CONTENT_OGG,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif
#ifdef NDPI_PROTOCOL_MOVE
    ndpi_set_bitmask_protocol_detection("Move", ndpi_struct, detection_bitmask, *id,
                                        NDPI_PROTOCOL_MOVE,
                                        ndpi_search_http_tcp,
                                        NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD,
                                        NO_SAVE_DETECTION_BITMASK_AS_UNKNOWN,
                                        ADD_TO_DETECTION_BITMASK);
    *id += 1;
#endif

    /* Update excluded protocol bitmask */
    NDPI_BITMASK_SET(ndpi_struct->callback_buffer[a].excluded_protocol_bitmask,
                     ndpi_struct->callback_buffer[a].detection_bitmask);

    /*Delete protocol from excluded protocol bitmask*/
    NDPI_DEL_PROTOCOL_FROM_BITMASK(ndpi_struct->callback_buffer[a].excluded_protocol_bitmask, NDPI_PROTOCOL_UNKNOWN);

    NDPI_DEL_PROTOCOL_FROM_BITMASK(ndpi_struct->callback_buffer[a].excluded_protocol_bitmask, NDPI_PROTOCOL_QQ);

#ifdef NDPI_CONTENT_FLASH
    NDPI_DEL_PROTOCOL_FROM_BITMASK(ndpi_struct->callback_buffer[a].excluded_protocol_bitmask, NDPI_CONTENT_FLASH);
#endif

    NDPI_DEL_PROTOCOL_FROM_BITMASK(ndpi_struct->callback_buffer[a].excluded_protocol_bitmask,  NDPI_CONTENT_MMS);

    NDPI_DEL_PROTOCOL_FROM_BITMASK(ndpi_struct->callback_buffer[a].excluded_protocol_bitmask, NDPI_PROTOCOL_XBOX);

    NDPI_BITMASK_SET(ndpi_struct->generic_http_packet_bitmask, ndpi_struct->callback_buffer[a].detection_bitmask);

    NDPI_DEL_PROTOCOL_FROM_BITMASK(ndpi_struct->generic_http_packet_bitmask, NDPI_PROTOCOL_UNKNOWN);

    /* Update callback_buffer index */
    a++;

#endif

}

#endif
