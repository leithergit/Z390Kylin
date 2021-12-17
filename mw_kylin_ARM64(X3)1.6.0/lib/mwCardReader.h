/**************************************************************************************************
 * 			CopyRight(C), 1992-2011,M&W CARD TECH.Co,Ltd.
 * 			明华澳汉非接触读卡器应用编程接口(API)说明
 *
 *FileName:	
 *
 *Author: sgt	Version: v1.0.0.1	Date: 2011-06-23
 *
 *Description:   
 *
 *
 *
 *Others:       
 *	1. API命名描述			companyName_category_operation()
 *			companyName:	MW (明华澳汉公司M&W)
 *			category：
 *							DEV	//设备操作
 *							S50	//mifare 1 card 专用操作接口
 *							ISO7816	//符合ISO7816协议的卡片操作接口
 *							cpu	//CPU卡操作(包括非接触CPU卡、接触CPU卡、PSAM卡)
 *							mifare	//s50/s70卡片操作接口
 *							EXT	//扩展的操作接口
 *			operation:		详细请参考各个接口的描述
 *
 *Function List: 
 *   1. ....
 *
 *History:      
 *   1. Date:
 *      Author:
 *      Modification:
 *   2. ...
 * 
 *
 *
 *  推荐的函数调用顺序(以M1卡为例): 
 *
 *	mw_dev_openReader			打开设备     
 *  mw_mifare_openCard			打开卡片
 *  mw_mifare_anticoll			防冲突
 *	mw_mifare_authentication	验证密码   
 *   ......
 *  mw_dev_closeReader			关闭设备
 *************************************************************************************************/

#ifndef __MWCARDREADER_H__
#define __MWCARDREADER_H__


#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#ifdef __x86_64__
#define  HANDLE long long
#elif __i386__
#define  HANDLE int
#endif
#define INVALID_HANDLE_VALUE (HANDLE)(-1)
#define STDCALL
#else
#include <Windows.h>
#define STDCALL __stdcall
#endif

#else

#ifdef __x86_64__
#define  HANDLE long long
//#elif __i386__
//#define  HANDLE int32_t
#else
#define  HANDLE int
#endif

#define INVALID_HANDLE_VALUE (HANDLE)(-1)
#define STDCALL


#endif

#ifdef __cplusplus
extern "C"{
#endif


//接口说明
/**************************************************************************************************
 * Function:	mw_dev_openReader		
 * Description:	打开读卡器通讯端口
 * Input: 
		name:
			(A)串行口读卡器可取"COM1", "COM2", "COM3", "COM4"等,字母大小写无关
			(B)USB口读卡器可取"USB1", "USB2", "USB3", "USB4"等
			(C)设备序列号
		param1:
			(A)串行口读卡器	波特率,9600/19200/57600/115200
			(B)USB口读卡器	打开设备的模式 1：共享模式 2：独占模式
			(C)未使用，可设置为0.
		param2:
			(A)串行口读卡器	校验位,可为'E' 偶校验, 'N' 无校验,无特殊要求,一般使用偶校验即可
			(B)USB口读卡器	未使用，可设置为0
			(C)未使用，可设置为0.
 * Output:		
		devNo:	通讯设备标识符
 * Return:		=0	正确;	<>0	错误		
 * Others:
**************************************************************************************************/
int STDCALL mw_dev_openReader(char* name, HANDLE *devNo, unsigned int param1, unsigned int param2);

/**************************************************************************************************
 * Function:	mw_dev_closeReader		
 * Description:	关闭读卡器通讯端口		
 * Input:  
		devNo:	通讯设备标识符
 * Output:		无		
 * Return:		=0	正确;	<>0	错误		
 * Others:
**************************************************************************************************/
int STDCALL mw_dev_closeReader(HANDLE devNo);


/**************************************************************************************************
 * Function:	mw_dev_reset		
 * Description:	射频天线复位		
 * Input:  
		devNo: 通讯设备标识符
 * Output:		无			
 * Return:		=0	正确;	<>0	错误		
 * Others:	
**************************************************************************************************/
int STDCALL mw_dev_reset(HANDLE devNo);

/**************************************************************************************************
 * Function:	mw_dev_led		
 * Description:	指示灯控制		
 * Input:       
		devNo:	通讯设备标识符
		ctrlInfo:
			指示灯控制信息，长度1个字节，bit7对应指示灯1控制；bit6对应指示灯2控制，以此类推; 0关、1开
			例如：
				0x80: 绿灯亮，红灯灭;
 * Output:		无				
 * Return:		=0	正确;	<>0	错误	
 * Others: 
**************************************************************************************************/
int STDCALL mw_dev_led(HANDLE devNo, unsigned char ctrlInfo);

/**************************************************************************************************
 * Function:	mw_dev_beep		
 * Description:	蜂鸣器控制		
 * Input:        
		devNo:	通讯设备标识符
		beepTimes:	蜂鸣次数
		interval:	蜂鸣间隔 每100ms一个单位
		time:		蜂鸣时间 每100ms一个单位
		例如:	要蜂鸣3次，每次间隔100ms,蜂鸣时间200ms
				mw_dev_beep(devNo,3,1,2);
 * Output:		无  			
 * Return:		=0	正确;	<>0	错误
		
 * Others:    
**************************************************************************************************/
int STDCALL mw_dev_beep(HANDLE devNo, unsigned char beepTimes, unsigned char interval, unsigned char time);

/**************************************************************************************************
 * Function:	mw_dev_readVer		
 * Description:	读取读写器版本号		
 * Input:         
		devNo: 通讯设备标识符
 * Output:       
		readerVersion:	存放收到的数据，16进制字符串，以'\0'为结束标示
 * Return:		=0	正确;	<>0	错误		
 * Others:     
**************************************************************************************************/
int STDCALL mw_dev_readVer(HANDLE devNo,  char *readerVersion);

/**************************************************************************************************
 * Function:	mw_dev_readSn		
 * Description:	读取读写器唯一的序列号		
 * Input:         
		devNo: 通讯设备标识符
 * Output:       
		readerSn:	存放收到的数据，16进制字符串，以'\0'为结束标示
 * Return:		=0	正确;	<>0	错误		
 * Others:     
**************************************************************************************************/
int STDCALL mw_dev_readSn(HANDLE devNo,  char *readerSn);

/**************************************************************************************************
 * Function:	mw_dev_writeSn		
 * Description:	写读写器唯一的序列号		
 * Input:         
		devNo:	通讯设备标识符
		readerVersion:	存放收到的数据，16进制字符串，以'\0'为结束标示
 * Output:		无  
 * Return:		=0	正确;	<>0	错误		
 * Others:     
**************************************************************************************************/
int STDCALL mw_dev_writeSn(HANDLE devNo,  char *readerSn);
/**************************************************************************************************
 * Function:	mw_cpu_reset	
 * Description:	接触式CPU卡复位	
 * Input:         
		devNo:	通讯设备标识符
		ivCardSeat: 卡座编号
					0	:		接触式大卡座
					1	:		非接触式卡座
					2--5:		扩展的第2--5个小CPU/PSAM卡座
 * Output:       
		infoATR:	存放收到的应答信息
		atrLen:		存放收到的应答信息的长度
 * Return:		=0	正确;	<>0	错误	
 * Others:     
**************************************************************************************************/
int STDCALL mw_cpu_reset(HANDLE devNo, unsigned int ivCardSeat, unsigned char *infoATR, unsigned char *atrLen);

/**************************************************************************************************
 * Function:  mw_cpu_apdu		
 * Description:	对接触式CPU卡发送指令		
 * Input:         
		devNo:		通讯设备标识符
		ivCardSeat: 卡座编号
					0	:		接触式大卡座
					1	:		非接触式卡座
					2--5:		扩展的第2--5个小CPU/PSAM卡座
		apduData:	对接触式CPU卡发送的指令信息(相关指令请参考CPU卡COS手册)
		apduLen:	对接触式CPU卡发送的指令信息长度
 * Output:       
		respData:	返回的信息
		respLen:	返回的信息长度
 * Return:	=0	正确;	<>0	错误				
 * Others:     
**************************************************************************************************/
int STDCALL mw_cpu_apdu(HANDLE devNo, unsigned int ivCardSeat, unsigned char *apduData, unsigned int apduLen, unsigned char *respData, int *respLen);

/**************************************************************************************************
 * Function:  mw_cpu_powerDown		
 * Description:	对接触式CPU卡下电		
 * Input:         
		devNo: 通讯设备标识符
		ivCardSeat: 卡座编号
		0	:		接触式大卡座
		1	:		非接触式卡座
		2--5:		扩展的第2--5个小CPU/PSAM卡座
		* Output:		无     	
		* Return:		=0	正确;	<>0	错误		
 * Others:     
**************************************************************************************************/
int STDCALL mw_cpu_powerDown(HANDLE devNo, unsigned int ivCardSeat);

/**************************************************************************************************
 * Function:	mw_mifare_openCard     
 * Description:	打开mifare卡片 		
 * Input:        
		devNo:	通讯设备标识符
		openMode:
			打开模式：
				0x00	STD模式,只能寻到空闲状态下的卡片，被激活的卡片不会响应
				0x01	ALL模式,所有卡片都能寻到，已经被激活的卡片将回到空闲状态后并重新被激活			
 * Output:      
		cardVid:	返回的卡片序列号，16进制字符串，以'\0'为结束标示		
 * Return:		=0	正确;	<>0	错误	
 * Others:    
**************************************************************************************************/
int STDCALL mw_mifare_openCard(HANDLE devNo, unsigned char openMode,  char *cardUid);

/**************************************************************************************************
 * Function:	mw_mifare_closeCard	
 * Description:	关闭mifare卡片	
 * Input:         
		devNo: 通讯设备标识符
 * Output:		无	
 * Return:		=0	正确;	<>0	错误	
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_closeCard(HANDLE devNo);

/**************************************************************************************************
 * Function:	mw_mifare_requestCard	
 * Description:	请求卡片		
 * Input:         
		devNo: 通讯设备标识符
		openMode:	打开模式
				0x00	STD模式,只能寻到空闲状态下的卡片，被激活的卡片不会响应
				0x01	ALL模式,所有卡片都能寻到，已经被激活的卡片将回到空闲状态后并重新被激活	
 * Output:       
		cardType:	返回的卡片类型
				Mifare 标准 1k   : 0x0004
				Mifare 标准 4k   : 0x0002
				Mifare Light     : 0x0010
				Mifare UtraLight : 0x0044
				CPU :            :0x0008
 * Return:		=0	正确;	<>0	错误	
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_requestCard(HANDLE devNo, unsigned char openMode, unsigned short *cardType);

/**************************************************************************************************
 * Function:	mw_mifare_anticoll
 * Description:	防冲突
 * Input:	
		devNo: 通讯设备标识符
 * Output:
		cardUid:	返回卡片序列号，16进制字符串，以'\0'为结束标示
 * Return:		=0	正确;	<>0	错误
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_anticoll(HANDLE devNo,  char *cardUid);

/**************************************************************************************************
 * Function:	mw_mifare_select		
 * Description:	选卡	
 * Input:         
		devNo:	通讯设备标识符
		cardVid:即将被选择的卡片序列号，以'\0'为结束的16进制字符串
 * Output:       
		cardSak:	返回该卡片的SAK值
 * Return:	=0	正确;	<>0	错误	
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_select(HANDLE devNo,  char *cardUid, unsigned int *cardSak);

/**************************************************************************************************
 * Function:	mw_mifare_authentication	
 * Description:	验证密码		
 * Input:         
		devNo:	通讯设备标识符
		mode:	密码类型：	0-表示输入A密码,1-表示输入是B密码
		blockNo:	要验证的扇区号
		key:	密码,以'\0'为结束的字符串,长度为12个字节，例如:"ffffffffffff"
 * Output:		无	
 * Return:		=0	正确;	<>0	错误	
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_authentication(HANDLE devNo, unsigned char mode, unsigned int blockNo, unsigned char *key);

/**************************************************************************************************
 * Function:	mw_mifare_read			
 * Description:	读数据			
 * Input:         
		devNo: 通讯设备标识符
		blockNo:	要读取的块号，对于S50卡，取值为0～63;对于S70卡，取值为0～255;
 * Output:       
		blockData:	读取的数据
 * Return:		=0	正确;	<>0	错误		
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_read(HANDLE devNo, unsigned int blockNo, unsigned char *blockData);

/**************************************************************************************************
 * Function:	mw_mifare_write		
 * Description:	写数据		
 * Input:         
		devNo: 通讯设备标识符
		blockNo:	要写的块号
		blockData:	写入的数据
 * Output:		无		
 * Return:		=0	正确;	<>0	错误			
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_write(HANDLE devNo, unsigned int blockNo, unsigned char *blockData);

/**************************************************************************************************
 * Function:	mw_mifare_initVal		
 * Description:	初始化值	 
 * Input:         
		devNo: 通讯设备标识符
		blockNo:	初始化的块号
		initValue:	写入的数据，必须是已按照值存储格式格式化后的数据
 * Output:		无	
 * Return:		=0	正确;	<>0	错误	
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_initVal(HANDLE devNo, unsigned int blockNo, unsigned int initValue);

/**************************************************************************************************
 * Function:	mw_mifare_readVal	
 * Description:	读值	
 * Input:         
		devNo: 通讯设备标识符
		blockNo:	读值的块号
 * Output:		
		dstValue:	读取的数值
 * Return:		=0	正确;	<>0	错误
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_readVal(HANDLE devNo, unsigned int blockNo, unsigned int *dstValue);

/**************************************************************************************************
 * Function:	mw_mifare_increment	
 * Description:	增值		
 * Input:         
		devNo: 通讯设备标识符
		boockNo:	增值的块号
		value:	增加的数值，必须是已按照值存储格式格式化后的数据
 * Output:		无		
 * Return:		=0	正确;	<>0	错误		
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_increment(HANDLE devNo, unsigned int blockNo, unsigned int value);

/**************************************************************************************************
 * Function:	mw_mifare_decrement
 * Description:	减值	
 * Input:         
		devNo: 通讯设备标识符
		blockNo:	减值的块号
		value:	减少的值，必须是已按照值存储格式格式化后的数据
 * Output:		无	
 * Return:		=0	正确;	<>0	错误	
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_decrement(HANDLE devNo, unsigned int blockNo, unsigned int value);

/**************************************************************************************************
 * Function:	mw_mifare_restore
 * Description:	将某块的数据传入卡的内部寄存器中
 * Input:         
		devNo: 通讯设备标识符
		blockNo:	寄存数据的块号
 * Output:		无
 * Return:		=0	正确;	<>0	错误
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_restore(HANDLE devNo, unsigned int blockNo);

/**************************************************************************************************
 * Function:	mw_mifare_transfer	
 * Description:	将内部寄存器的数据传送到某一块中，进行此项操作必须验证该扇区的密码		
 * Input:         
		devNo: 通讯设备标识符
		blockNo:	传送数据的块号
 * Output:     	无
 * Return:      =0	正确;	<>0	错误
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_transfer(HANDLE devNo, unsigned int blockNo);

/**************************************************************************************************
 * Function:	mw_mifare_halt	
 * Description:	将选定的卡片置于HALT模式，需要Request All将其唤醒		
 * Input:		devNo: 通讯设备标识符
 * Output:     	无
 * Return:      =0	正确;	<>0	错误
 * Others:     
**************************************************************************************************/
int STDCALL mw_mifare_halt(HANDLE devNo);


/**************************************************************************************************
 * Function:	mw_ext_ascToHex	
 * Description:	将ASCII编码的2个字节合成为一个16进制数，例如：输入数据为字符串"32",则转化0x32
 * Input:          
	srcAsc: 要转换的ASCII编码数据.合法的数据为:'0'--'9' 'a'--'f' 'A'--'F'
	srcLen: ASCII数据长度,必须为2的整数倍
 * Output:         
	dstHex: 保存转换后的HEX格式数据，该存储空间至少是 srcLen/2 个字节的长度
 * Return:		=0	正确;	<>0	错误      
 * Others:
**************************************************************************************************/
int STDCALL mw_ext_ascToHex(const  char *srcAsc, unsigned int srcLen, unsigned char * dstHex);

/**************************************************************************************************
 * Function:	mw_ext_hexToAsc	
 * Description:	将一个16进制数据转化为2个字节的ASCII。例如：输入数据为0X32，则转化后的数据为0x33,0x32,即字符串"32"
 * Input:
	srcHex: 要被转换的HEX格式数据
	srcLen: HEX数据长度
 * Output:
	dstAsc: 保存转换后的ASCII格式数据，该存储空间至少是 srcLen*2 个字节的长度。
 * Return: 		=0	正确;	<>0	错误      
 * Others:        
**************************************************************************************************/
int STDCALL mw_ext_hexToAsc(const unsigned char *srcHex, unsigned int srcLen,  char *dstAsc);

int STDCALL mw_custom_rfRats(HANDLE devNo, unsigned int ivCardSeat, unsigned char *infoATR);

int STDCALL mw_dev_getCardStatus(HANDLE devNo, unsigned int ivCardSeat, unsigned char *CardStatus);

int32_t STDCALL mw_dev_selectAntenna(HANDLE  devNo, unsigned char antennaNumber);
#ifdef __cplusplus
};
#endif


#endif
