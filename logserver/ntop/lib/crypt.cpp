#include "crypt.h"
#include "stdlib.h"
#include <string.h>


static char szLcpPWD[] = "F25674EE6673417108604A17BCF0F742LCPCOLLECTONFO400AF830D9C07EA19CB3ETONGXUNF1A95AE94D960A";
void Lcp_DeCrypt(char* szBuffer,int nBYTELen)
{
    if(nBYTELen==0)
        return;
    u_int  nPWLen=strlen(szLcpPWD);
    u_int  nPos1=0;
    u_int  nPos2=0;
    nPos2=0;
    for (nPos1=0;  nPos1<nBYTELen;  nPos1++)
    {
        if (nPos2==nPWLen)
            nPos2=0;
        szBuffer[nPos1]=(~szBuffer[nPos1])^szLcpPWD[nPos2++];
    }
}

//加密

void Lcp_EnCrypt(char* buffer,int len)
{
    if(len==0)
        return;
    int PWLen=strlen(szLcpPWD);						//  密码的长度
    int BYTELen=len;									//  缓冲的大小
    int i=0;
    int  j=0;
    for (i=0;  i<BYTELen;  i++)
    {
        if (j==PWLen)  j=0;								//   生成密匙种子
         buffer[i]=(~buffer[i])^szLcpPWD[j++];	//   逐个加密字符
    }
}
