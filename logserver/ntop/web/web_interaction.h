#ifndef WEB_INTERACTION_H
#define WEB_INTERACTION_H
#include"xsmysql/mysqldb.h"
#include"xsmysql/mydbop.h"

//field
#define XSWEB_FIELD_MAC                   2000
#define XSWEB_FIELD_FLOW                  2001
#define XSWEB_FIELD_HOST                  2002
#define XSWEB_FIELD_INTERFACE          2003

//action
#define XSWEB_ACT_QUERY                 10
#define XSWEB_ACT_UPDATE                11
#define XSWEB_ACT_DELETE                12
#define XSWEB_ACT_ADD                     13

//show entry(Whole or  Specified entry)
#define XSWEB_SHOW_SPECIAL                20    //指定条件
#define XSWEB_SHOW_NORMAL                 21   //遍历

typedef enum{
    sortcolumn = 0,
    countryfilter,
    hostfilter,
}web_Param;

/*~~~~~~~~~~json_parse~~~~~~~~~~~~~*/
typedef struct _xsjsonweb{
    int filed;//field
    int act;//action
    int actEntry;//show entry(Whole or  Specified entry
}XSJSONWEB,*PXSJSONWEB;

void Xsweb_Work(/*PXSWORK pXsWork*/);
/*~~~~~~~~~~~~~~~~~~~mac_msg~~~~~~~~~~~~~~*/
typedef struct _xsjsonmac{
    u_int16_t ifaceID;
    u_int16_t vlanID;
    u_int16_t max_hit;
    u_int16_t to_skip;
    u_int8_t mac[6];
}XSJSONMAC,*PXSJSONMAC;

int Xsweb_Field_Mac(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot);
/*~~~~~~~~~~~~~~~~~~~~flow_msg~~~~~~~~~~~~~*/
typedef struct _xsjsonflow{
    u_int16_t ifaceID;
    u_int32_t ip_addr;
    u_int16_t vlanID;
    u_int16_t max_hit;
    u_int16_t to_skip;
    u_int8_t mac[6];
    web_Param webParam;
}XSJSONFLOW,*PXSJSONFLOW;

int Xsweb_Field_Flow(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot);

/*~~~~~~~~~~~~~~~~~~~~host_msg~~~~~~~~~~~~~*/
typedef struct _xsjsonhost{
    u_int16_t ifaceID;
    u_int16_t vlanID;
    u_int16_t max_hit;
    u_int16_t to_skip;
    struct in_addr  ip_addr;
    u_int8_t mac[6];
}XSJSONHOST,*PXSJSONHOST;

int Xsweb_Field_Host(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot);

/*~~~~~~~~~~~~~~~interface_msg~~~~~~~~~~~~~~~~~~~~~~~~~*/
int Xsweb_Field_Interface(PXSJSONWEB json_web, json_object *JSONroot, json_object *pRetRoot);

/*~~~~~~~~~~~~~~~~run_interaction~~~~~~~~~~~~~~~~~~~~~~~~*/
void  set_logging_level(u_int8_t param);

#endif // WEB_INTERACTION_H
