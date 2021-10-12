extern "C" {

#include "mwReader.h"

}
#include<stdio.h>
#include<string.h>
#include<string>
#include <unistd.h>
using namespace std;
void ZeroMemory(void* data, int len){
	memset(data, 0, len);
}

int main(){
	char list[64] = { 0 };
	HANDLE devNo; //设备句柄
	char errMsg[2048]; //错误信息
	int st=mwDevOpen("USB","",&devNo); //打开读写器
	if(st<0)
	{
		ZeroMemory(errMsg,2048);
		getErrDescription(st,1,errMsg);//获取中文错误信息
		printf("%s\n",errMsg); 
		system("pause");
		return 0;
	}
	else
	{
		printf("设备连接成功!\n");
	}

	st = mwIDCardOpen(); //打开二代证模块
	if(st<0)
	{
		ZeroMemory(errMsg,2048);
		getErrDescription(st,1,errMsg);//获取中文错误信息
		printf("%s\n",errMsg); 
		system("pause");
		return 0;
	}
	else
	{
		printf("打开二代证成功!\n");
	}
	while(1) {
		do {
			mwIDCardClearData();
			st = mwIDCardRequest();  //寻卡
			if(st<0)
			{
				printf("%s\n","读取二代证寻卡失败"); 
				ZeroMemory(errMsg,2048);
				getErrDescription(st,1,errMsg);//获取中文错误信息
				printf("%s\n",errMsg); 
				break;
			}
			st = mwIDCardSelect();   //选卡
			st = mwIDCardRead();     //读二代证
			if(st<0)
			{
				printf("%s\n","读取二代证失败"); 
				ZeroMemory(errMsg,2048);
				getErrDescription(st,1,errMsg);//获取中文错误信息
				printf("%s\n",errMsg); 
				break;
			}
			else
			{
				printf("读取二代证成功!\n");
			}
			//st = mwIDCardQueryPhotoFile("./MWID.bmp");  //读照片
			if(st<0)
			{
				printf("%s\n","读取照片失败"); 
				ZeroMemory(errMsg,2048);
				getErrDescription(st,1,errMsg);//获取中文错误信息
				printf("%s\n",errMsg); 
				break;
			}
			else
			{
				printf("读取照片成功!\n");
				string IDCardName = mwIDCardQueryName();
				string IDCardSex = mwIDCardQuerySex();
				string IDCardNation = mwIDCardQueryNation();
				string IDCardBirth = mwIDCardQueryBirth();
				string IDCardAddress = mwIDCardQueryAddress();
				string IDCardNumber = mwIDCardQueryNumber();
				string IDCardDepartment = mwIDCardQueryDepartment();
				string IDCardExpireDay = mwIDCardQueryExpireDay();
				printf("读二代证成功，返回值如下：\n");
				printf("姓名: %s\n",IDCardName.c_str());
				printf("性别: %s\n",IDCardSex.c_str());
				printf("民族: %s\n",IDCardNation.c_str());
				printf("生日: %s\n",IDCardBirth.c_str());
				printf("住址: %s\n",IDCardAddress.c_str());
				printf("身份证号: %s\n",IDCardNumber.c_str());
				printf("签发机关: %s\n",IDCardDepartment.c_str());
				printf("有效期: %s\n",IDCardExpireDay.c_str());
			}
		} while(0);
		usleep(1000 * 200);
	}
	st = mwIDCardClose();  //关闭二代证模块
	mwDevClose(devNo); //关闭设备
	return 0;
}


