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
       //cout << "!!!mwDevGetUsbList!!! 1" << endl; // prints !!!Hello World!!!
       int st = 0;
       char list[128];
       st = mwDevGetUsbList(list);
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
 	       printf("查找设备失败，错误代码: %d\n",st);
           return 0;
       }
       else
       {
           //printf("查找设备成功!：%s\n",list);	
       }
       //cout << "!!!mwDevOpen!!! 2" << endl; // prints !!!Hello World!!!
       st = mwDevOpen("USB", NULL, &devNo2); //打开读写器
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
 	       printf("设备连接失败，错误代码: %d\n",st);
           return 0;
       }
       else
       {
           //printf("设备2连接成功!\n");	
       }
        
       unsigned short cardType = 0;
       st = mwRequest(devNo2, 1, &cardType);
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
           printf("寻卡失败，错误代码: %d\n",st);
           mwDevClose (devNo2);
           return 0;
       }
       
       unsigned char cardUid[20];	
       st = mwAnticoll(devNo2, cardUid);
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
           printf("读序列号失败，错误代码: %d\n",st);
           mwDevClose (devNo2);
           return 0;
       }
       else
       {
           char cardUiddst[20];	
           st = BinToHex(cardUid,  cardUiddst, st);
           printf("NFC序列号: %s\n", cardUiddst);
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
