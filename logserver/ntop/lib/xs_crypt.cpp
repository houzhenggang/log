#include "xs_crypt.h"
#include "stdlib.h"
#include <string.h>
#include"cjson.h"

static char szPWD[] = "TONGXUNF1AF25674EE6673417108604A17BCF0F7427D6A12FBD939BC20400AF830D9C07EA19CB3E95AE94D595960A";
void DeCrypt(char* szBuffer,int nBYTELen)
{
    if(nBYTELen==0)
        return;
    u_int  nPWLen=strlen(szPWD);
    u_int  nPos1=0;
    u_int  nPos2=0;
    nPos2=0;
    for (nPos1=0;  nPos1<nBYTELen;  nPos1++)
    {
        if (nPos2==nPWLen)
            nPos2=0;
        szBuffer[nPos1]=(~szBuffer[nPos1])^szPWD[nPos2++];
    }
}

//加密

void EnCrypt(char* buffer,int len)
{
    if(len==0)
        return;
    int PWLen=strlen(szPWD);						//  密码的长度
    int BYTELen=len;									//  缓冲的大小
    int i=0;
    int  j=0;
    for (i=0;  i<BYTELen;  i++)
    {
        if (j==PWLen)  j=0;								//   生成密匙种子
         buffer[i]=(~buffer[i])^szPWD[j++];	//   逐个加密字符
    }
}

void EnCrypt2(char* buffer,int pos ,int size,char *pKey)
{
    if(pos>16)
        return;
    int PWLen=strlen(pKey);						//  密码的长度
    int BYTELen=size;					//  缓冲的大小
    if (BYTELen>16)
        BYTELen=16;
    int i=pos;
    int  j=pos;
    for (i;  i<BYTELen;  i++)
    {
        if (j==PWLen)  j=0;							//   生成密匙种子
        buffer[i]=(~buffer[i])^pKey[j++];	//   逐个加密字符
    }
}

