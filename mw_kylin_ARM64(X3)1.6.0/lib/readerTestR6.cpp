//============================================================================
// Name        : readerTest.cpp
// Author      : sgt
// Version     :
// Copyright   : m&w
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <cstring>
using namespace std;

#include "mwReader.h"
HANDLE devNo2; //设备句柄

//获取错误信息

string getErrMsg(int errCode)
{

    char errMsg[2048];
    getErrDescription(errCode, 0, errMsg);
    string str = string((char*) errMsg);
    return str;
}

int openDev2()
{
       cout << "!!!Hello World!!! 2" << endl; // prints !!!Hello World!!!
       int st = mwDevOpen("USB", NULL, &devNo2); //打开读写器
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
           return 0;
       }
       else
       {
           printf("设备2连接成功!\n");	
       }

       char version[20];

       st = mwDevGetHardwareVer(devNo2, version); //读设备版本号
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
           //printf("读版本号失败，错误代码: %d\n",st);
           mwDevClose (devNo2);
           return 0;
       }
       else
       {
           printf("设备2版本号: %s\n", version);
           mwDevBeep(devNo2, 2, 2, 2);
       }

       char SerialNumber[20];	
       st = mwDevGetSerialNumber(devNo2, SerialNumber); //读序列号
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
           //printf("读序列号失败，错误代码: %d\n",st);
           mwDevClose (devNo2);
           return 0;
       }
       else
       {
           printf("设备2序列号: %s\n", SerialNumber);
           mwDevBeep(devNo2, 2, 2, 2);
       }	
       printf("准备关闭2设备\n");
       mwDevClose (devNo2);
       printf("设备已经关闭\n");
       return 0;
}

int main()
{
    int t = 2;
    while (t)
    {
        printf("=======================%d=========================", t);
        openDev2();
        t--;
    }

    return 0;
}
