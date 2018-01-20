#ifndef MY_GETHOATNAME_H__
#define MY_GETHOATNAME_H__
#include <stdlib.h>

//typedef enum
//{
//    false = 0;
//    true = !false
//}bool;


struct dns_packet_header {
    u_int16_t transaction_id, flags, num_queries, answer_rrs, authority_rrs, additional_rrs;
} __attribute__((packed));
#define DNS_PACKET_HEADER_SIZE  sizeof(struct dns_packet_header)

struct dns_query    //dns 查询结构
{
    u_int16_t type;   //查询类型，大约有20个不同的类型
    u_int16_t classes;   //查询类,通常是A类既查询IP地址。
};

u_int my_gethostbyname(char *domain,u_int32_t dnsserver,u_int32_t records[],u_int MAX_RECORDS,char *pdev);




#endif //MY_GETHOATNAME_H__
