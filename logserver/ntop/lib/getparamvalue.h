#ifndef GETPARAMVALUE_H
#define GETPARAMVALUE_H

#include <stdio.h>
#include <xsmysql/mydbop.h>
#include <xsmysql/mysqldb.h>


int GetParamValue(char *pData,const char *pParam,char *pValue,int nSize,char *pEndFlag,int nDataSize);

#endif // GETPARAMVALUE_H

