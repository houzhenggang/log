#ifndef XSHASH2
#define XSHASH2
#include <stdlib.h>

unsigned int APHash(char* str, unsigned int len)  ;
u_int FNVHash(char* str, u_int len);
unsigned int BKDRHash(char* str, unsigned int len)  ;
u_int CRC32(u_char*pbyte,u_int len);
u_int64_t CRC64(const u_char*pbyte,u_int64_t len);
int64_t CRCInt64(const u_char*pbyte,u_int64_t len);
#endif // XSHASH2

