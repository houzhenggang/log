#include <string.h>
#include<xsmysql/mysqldb.h>
#include <xsmysql/mydbop.h>

char *mystrstr(char *Str,const char *SubStr,int Depth)
{
    if(Str==NULL)
        return NULL;
    if(Depth==0)
        return strstr(Str,SubStr);
    else
    {
        int i,j;
        for(i=0;i<=Depth;i++)
        {
            if(Str[i]==SubStr[0])
            {
                j = 1;
                for(;j<Depth;j++)
                {
                    if(SubStr[j]==0)
                        return Str+i;
                    if(Str[i+j]==0||Str[i+j]!=SubStr[j])
                    {
                        break;
                    }
                }
                if(j==Depth)
                    return Str+i;
            }
        }
    }
    return NULL;
}

int GetParamValue(char *pData,const char *pParam,char *pValue,int nSize,char *pEndFlag,int nDataSize)//分布
{
    char *pTemp1=NULL;
    char *pTemp2 = NULL;
    memset(pValue,0,nSize);
    short int nLen = strlen(pEndFlag);
    short int nParamLen = strlen(pParam);
    if(nDataSize)
    {
        pTemp1 = mystrstr(pData,pParam,nDataSize);
    }
    else
    {
        pTemp1 = strstr(pData,pParam);
    }
    if(pTemp1)
    {
        if(strcmp("txtemplet:",pParam)==0)
        {
            strcpy(pValue,pTemp1+10);
            int nLen = strlen(pValue);
            int i = nLen-1;
            for(;i>0;i--)
            {
                if(pValue[i]==';')
                {
                    pValue[i]=0;
                    break;
                }
            }
            return i;
        }
        int nArPos = 0;
        char szArNode[16];
        while(1)
        {
            if(nArPos==nLen)
                break;
            char *pArKey = strstr(pEndFlag+nArPos+1,",");
            if(pArKey-pEndFlag >=16) {
                char log[256]={0};
                snprintf(log,256,"[1]cross the border :%d : 16",(int)(pArKey-pEndFlag));
                printf("%s\n",log);
            }
            strncpy(szArNode,pEndFlag+nArPos,pArKey-pEndFlag-nArPos);
            szArNode[pArKey-pEndFlag-nArPos]='\0';
            nArPos = pArKey-pEndFlag+1;
            char *pTemp;
            if(nDataSize)
            {
                pTemp = mystrstr(pTemp1+nParamLen,szArNode,nDataSize - nParamLen);
            }
            else
            {
                pTemp = strstr(pTemp1+nParamLen,szArNode);
            }
            if(pTemp)
            {
                if(pTemp2>pTemp||pTemp2==NULL)
                    pTemp2= pTemp;
            }
        }
        if(!pTemp2)
        {
            if(nDataSize)
                pTemp2 = pData+nDataSize;
            else
                return 0;
        }
        if(pTemp2-pTemp1-nParamLen>=nSize||pTemp2-pTemp1-nParamLen<0)
        {
            return 0;
        }
        else
        {
            strncpy(pValue,pTemp1+nParamLen,pTemp2-pTemp1-nParamLen);
            return pTemp2-pData;
        }
    }
    return 0;
}

