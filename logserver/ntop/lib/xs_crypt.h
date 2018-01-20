#ifndef XS_CRYPT_H
#define XS_CRYPT_H

void DeCrypt(char* szBuffer,int nBYTELen);
void EnCrypt(char* buffer,int len);
void EnCrypt2(char* buffer,int pos ,int size,char *pKey);
#endif // XS_CRYPT_H

