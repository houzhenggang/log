#ifndef XSWEBACTION_H
#define XSWEBACTION_H

//Field


#define XSWEB_FIELD_PORTMAP                        1001
#define XSWEB_FIELD_DPIRULE                       1002
#define XSWEB_FIELD_IPGROUP                            1003
#define XSWEB_FIELD_POLICYROUTE                     1004
#define XSWEB_FIELD_LINEGROUP                      1005
#define XSWEB_FIELD_IFLINE                            1006
#define XSWEB_FIELD_BWPIPE                           1007
#define XSWEB_FIELD_BWPOLICY                    1008
#define XSWEB_FIELD_MIRRORPOLICY            1009
#define XSWEB_FIELD_INTERFACE                   1010
#define XSWEB_FIELD_DHCPV4                            1011
#define XSWEB_FIELD_ARP                                  1012
#define XSWEB_FIELD_IPGROUPDEFINE            1013
#define XSWEB_FIELD_ONLINENUMS                1014
#define XSWEB_FIELD_PPPOES                           1015
#define XSWEB_FIELD_PPPOES_USER                  1016
#define XSWEB_FIELD_DNSCTRL                              1017
#define XSWEB_FIELD_DOMAIN                              1018
#define XSWEB_FIELD_PPPOES_USER_GROUP       1019
#define XSWEB_FIELD_DDOS                                    1020
#define XSWEB_FIELD_TTL                                          1021
#define XSWEB_FIELD_NODIALING                             1022
#define XSWEB_FIELD_LOGSERVER                           1023
#define XSWEB_FIELD_OPENVPNS                            1024
#define XSWEB_FIELD_OPENVPNS_USER                   1025
#define XSWEB_FIELD_XSURL                                       1026
#define XSWEB_FIELID_PARAMTERS                              1027
#define XSWEB_FIELD_PPPOES_RADIUS_POOLS                 1028
#define XSWEB_FIELD_PPPOES_ONLINE_USER              1029
#define XSWEB_FIELD_TUN_OUT                                     1030
#define XSWEB_FIELD_TIMEOBJ                                     1031
#define XSWEB_FIELD_MAC_ACL                                     1032
#define XSWEB_FIELD_PPPOES_BINDPOOL                     1033    //服务绑定地址池功能取消
#define XSWEB_FIELD_IP_BURST                                    1034
//#define XSWEB_FIELD_PPPOE_EXPRESS_NOTICE            1035
#define XSWEB_FIELD_PPPOES_SIDE_AUTH                     1036
#define XSWEB_FIELD_LANMSGPUSH                      1037
#define XSWEB_FIELD_BONDING                         1038
#define XSWEB_FIELD_DMZ                                 1039
#define XSWEB_FIELD_CONN_SNAPSHOOT                  1040
#define XSWEB_FIELD_PPPOES_WEBAUTH           1041
#define XSWEB_FIELD_PPTPCLIENT_CONN            1042         //用于PPTP线路更新删除
#define XSWEB_FIELD_USRDEFPROGROUP              1050
#define XSWEB_FIELD_PYAPI_PING                                    1060
#define XSWEB_FIELD_LANIPPROTECT                    1070
#define XSWEB_FIELD_CUSTOM_DPI                      1080


#define XSWEB_FIELD_START           9009



//Action
#define XSWEB_ACT_RELOAD                11
#define XSWEB_ACT_ENABLE                12
#define XSWEB_ACT_UPDATE                13
#define XSWEB_ACT_DELETE                14
#define XSWEB_ACT_ADD                     15
#define XSWEB_ACT_UPDATE_PW          16
#define XSWEB_ACT_INFO                     17
#define XSWEB_ACT_INFO2                   18
#define XSWEB_ACT_RELOAD_TTL            19
#define XSWEB_ACT_LOGIN                 20
#define XSWEB_ACT_LOGOUT            21


#define XSWEB_ACT_RESTART           99


#endif // XSWEBACTION_H

