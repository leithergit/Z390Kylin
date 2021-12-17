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
       cout << "!!!mwDevGetUsbList!!! 1" << endl; // prints !!!Hello World!!!
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
           printf("查找设备成功!：%s\n",list);	
       }
       cout << "!!!mwDevOpen!!! 2" << endl; // prints !!!Hello World!!!
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
           printf("设备2连接成功!\n");	
       }

       char version[20];
       st = mwDevGetHardwareVer(devNo2, version); //读设备版本号
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
           printf("读版本号失败，错误代码: %d\n",st);
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
       char cardNo[20];
       char idNumber[20];
       char name[20];
       int sexCode = 0;
       st = mwReadSSCardInfo2(devNo2,cardNo,idNumber,name,&sexCode);		
       if (st < 0)
       {
           printf(getErrMsg(st).c_str());
           printf("\n");
           //printf("读社保卡失败，错误代码: %d\n",st);
           mwDevClose (devNo2);
           return 0;
       }
       else
       {
           printf("卡号: %s\n", cardNo);   
	       printf("社会保障号码: %s\n", idNumber);
	       printf("姓名: %s\n", name);
	       printf("性别: %d\n", sexCode);
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
