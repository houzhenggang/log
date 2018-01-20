#ifndef SAVEMSG_H
#define SAVEMSG_H
void XsDBOp_DhcpCache_InsertMysql(char *mac, char *name);
void XsDBOp_HostLabel_InsertMysql(char *host ,char * label_name);
void XsDBOp_NtopngTrace_InsertMysql(char *out_buf);

int XsDBOp_Query_Dhcp_Cache(char *mac, char *name);
int XsDBOp_Query_Host_Label(char *host,char *host_label);
#endif // SAVEMSG_H
