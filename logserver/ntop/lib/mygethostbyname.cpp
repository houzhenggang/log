#include "mygethostbyname.h"
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>

int change_domaine(char *fname, char *tname)
{
    int j =0;
    int i =strlen(fname)-1;
    int k = i+1;
    tname[i+2] = 0;
    for (; i>=0;i--,k--)
    {
        if (fname[i] == '.')
        {
            tname[k] = j;
            j=0;
        }
        else
        {
            tname[k] = fname[i];
            j++;
        }
    }
    tname[k] = j;
    return strlen(tname)+1;
}


#ifndef MAX_DNS_REQUESTS
#define MAX_DNS_REQUESTS			16
#endif

#define NDPI_MAX_IPV4_HOSTBIT_ADDR ntohl(inet_addr("255.255.255.255"))

static u_int getNameLength(u_int i, const u_int8_t *payload, u_int payloadLen) {
    if(payload[i] == 0x00)
        return(1);
    else if(payload[i] == 0xC0)
        return(2);
    else {
        u_int8_t len = payload[i];
        u_int8_t off = len + 1;

        if(off == 0) /* Bad packet */
            return(0);
        else
            return(off + getNameLength(i+off, payload, payloadLen));
    }
}

/* *********************************************** */

static char* ndpi_intoa_v4(unsigned int addr, char* buf, u_short bufLen) {
    char *cp, *retStr;
    uint byte;
    int n;

    cp = &buf[bufLen];
    *--cp = '\0';

    n = 4;
    do {
        byte = addr & 0xff;
        *--cp = byte % 10 + '0';
        byte /= 10;
        if(byte > 0) {
            *--cp = byte % 10 + '0';
            byte /= 10;
            if(byte > 0)
                *--cp = byte + '0';
        }
        *--cp = '.';
        addr >>= 8;
    } while (--n > 0);

    /* Convert the string to lowercase */
    retStr = (char*)(cp+1);

    return(retStr);
}

/* *********************************************** */

static u_int16_t get16(int *i, const u_int8_t *payload) {
    u_int16_t v = *(u_int16_t*)&payload[*i];

    (*i) += 2;

    return(ntohs(v));
}

u_int32_t xs_direct_get_dns_reply_ip(const u_char* payload,const u_int16_t loadlen,u_int32_t records[],u_int MAX_RECORDS)
{
    if(loadlen > DNS_PACKET_HEADER_SIZE) {
        int i = 0;
        struct dns_packet_header header, *dns = (struct dns_packet_header*)&payload[i];
        u_int8_t is_query, ret_code, is_dns = 0;
        u_int32_t query_offset, num_a_records = 0;

        header.flags = ntohs(dns->flags);
        header.transaction_id = ntohs(dns->transaction_id);
        header.num_queries = ntohs(dns->num_queries);
        header.answer_rrs = ntohs(dns->answer_rrs);
        header.authority_rrs = ntohs(dns->authority_rrs);
        header.additional_rrs = ntohs(dns->additional_rrs);
        is_query = (header.flags & 0x8000) ? 0 : 1;
        ret_code = is_query ? 0 : (header.flags & 0x0F);
        i += DNS_PACKET_HEADER_SIZE;
        query_offset = i;

        if(!is_query) {
            /* DNS Reply */

            if((header.num_queries <= MAX_DNS_REQUESTS) /* Don't assume that num_queries must be zero */
                    && (((header.answer_rrs > 0) && (header.answer_rrs <= MAX_DNS_REQUESTS))
                        || ((header.authority_rrs > 0) && (header.authority_rrs <= MAX_DNS_REQUESTS))
                        || ((header.additional_rrs > 0) && (header.additional_rrs <= MAX_DNS_REQUESTS)))
                    ) {
                /* This is a good reply */
                is_dns = 1;

                i++;

                if(payload[i] != '\0') {
                    while((i < loadlen) && (payload[i] != '\0')) {
                        i++;
                    }

                    i++;
                }

                i += 4;

                if(header.answer_rrs > 0) {
                    u_int16_t rsp_type;
                    u_int16_t num;
                    u_int16_t data_len;
                    u_int32_t lip;
                    for(num = 0; num < header.answer_rrs; num++) {

                        if((i+6) >= loadlen) {
                            break;
                        }

                        if((data_len = getNameLength(i, payload, loadlen)) == 0) {
                            break;
                        } else {
                            i += data_len;
                        }

                        rsp_type = get16(&i, payload);

                        // Skip past the CLASS (2 octets) and TTL (4 octets) fields.
                        i += 6;
                        data_len = get16(&i,payload);

                        if((data_len <= 1) || (data_len > (loadlen-i))) {
                            break;
                        }

                        if(rsp_type == 1 /* A */) {
                            if(data_len == 4) {
                                lip=ntohl(*((u_int32_t*)&payload[i]));
                                if(lip<NDPI_MAX_IPV4_HOSTBIT_ADDR){
                                    if(num_a_records < (MAX_RECORDS-1))
                                        records[num_a_records++] = *((u_int32_t*)&payload[i]);
                                    else
                                        break; /* One record is enough */
                                }
                            }
                        }

                        if(data_len == 0) {
                            break;
                        }
                        i += data_len;
                    } /* for */
                }
            }
        }
        return num_a_records;
    }
    return 0;
}

u_int my_gethostbyname(char *domain, u_int32_t dnsserver, u_int32_t records[], u_int MAX_RECORDS, char *pdev)
{
    int domainlen=strlen(domain);
    if(domainlen<=0 || domainlen>256)
        return 0;
    char buf[1024]={0};
    static u_int16_t transaction_id=1;
    transaction_id++;

    int udp_sock = socket(AF_INET,SOCK_DGRAM,0);
    if(-1==udp_sock)
        return 0;

    if(pdev && '\0' != pdev[0]) {
        struct ifreq interface;
        memset(&interface,0,sizeof(interface));
        strcpy(interface.ifr_name,pdev);
        if(-1==setsockopt(udp_sock,SOL_SOCKET,SO_BINDTODEVICE,(char*)&interface,sizeof(interface))){
            close(udp_sock);
            udp_sock = socket(AF_INET,SOCK_DGRAM,0);
            if(-1==udp_sock)
                return 0;
        }
    }

    struct timeval tv={2,0};
    setsockopt(udp_sock,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv));
    setsockopt(udp_sock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
    struct dns_packet_header *pDnsHdr;
    struct dns_query *pDnsQuery;
    memset(buf,0,DNS_PACKET_HEADER_SIZE);

    pDnsHdr = (struct dns_packet_header*)buf;
    pDnsHdr->transaction_id=transaction_id;
    pDnsHdr->flags = htons(0x0100);
    pDnsHdr->num_queries = htons(0x0001);
    pDnsHdr->answer_rrs=0;
    pDnsHdr->authority_rrs=0;
    pDnsHdr->additional_rrs=0;

    char *pBuf = (char*)(pDnsHdr + 1);
    int namelen = change_domaine(domain,pBuf);


    pDnsQuery = (struct dns_query*)(pBuf +namelen);
    pDnsQuery->classes = htons(0x0001);
    pDnsQuery->type = htons(0x0001);

    struct sockaddr_in addr={0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(53);
    addr.sin_addr.s_addr = dnsserver;
    socklen_t slen = sizeof(addr);
    int ret = sendto(udp_sock,buf,DNS_PACKET_HEADER_SIZE + namelen + 4,0,(struct sockaddr*)&addr,slen);
    if(ret>0) {
        ret =recvfrom(udp_sock,buf,1024,0,(struct sockaddr*)&addr,&slen);
        if(ret>0) {
            close(udp_sock);
            return xs_direct_get_dns_reply_ip((u_char*)buf,ret,records,MAX_RECORDS);
        }
    }
    close(udp_sock);
    return 0;
}
