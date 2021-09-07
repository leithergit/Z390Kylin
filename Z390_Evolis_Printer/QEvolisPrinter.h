﻿#ifndef QEVOLISPRINTER_H
#define QEVOLISPRINTER_H

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <map>
#include <vector>
#include "LithographPrinter.h"
#define LibVer          "Z390_kylin_aarch64_1.0.0.4 "

using namespace std;
typedef struct CardInfo
{
    std::string bankNumber;      //银行卡号
    std::string regionCode;      //区域代码
    std::string ATR;             //复位信息
    std::string algoCode = "03"; //算法标识
    std::string identifyNum;     //卡识别码
    std::string cardVersion;     //卡片版本
    std::string cardReleaseDate; //发卡日期
    std::string cardValidDate;   //卡有效期
    std::string cardNumber;      //卡号
    std::string cardID;          //身份证号
    std::string name;            //姓名
    std::string nameExtend;      //姓名拓展
    std::string sex;             //性别
    std::string nation;          //民族
    std::string birthday;        //出生日期
}CARDINFO;

using CardInfoPtr = shared_ptr<CardInfo>;

typedef struct NewCardInfo
{
    std::string bankNumber;      //银行卡号
    std::string regionCode;      //区域代码
    std::string ATR;             //复位信息
    std::string algoCode = "03"; //算法标识
    std::string identifyNum;     //卡识别码
    std::string cardVersion;     //卡片版本
    std::string cardReleaseDate; //发卡日期
    std::string cardValidDate;   //卡有效期
    std::string cardNumber;      //卡号
    std::string cardID;          //身份证号
    std::string name;            //姓名
    std::string nameExtend;      //姓名拓展
    std::string sex;             //性别
    std::string nation;          //民族
    std::string birthday;        //出生日期
}NEWCARDINFO;
using NewCardInfoPtr = shared_ptr<NewCardInfo>;

typedef struct PicInfo
{
    string  picpath;
    int     nAngle;
    float   fxPos;
    float   fyPos;
    float   fHeight;
    float   fWidth;
    int     nType;
    PicInfo()
    {
        memset(&nAngle,0,sizeof(PicInfo) - sizeof(picpath));
    }
}PICINFO;
using PicInfoPtr = shared_ptr<PicInfo>;

typedef struct TextInfo
{
    string  sText;
    int     nAngle; //旋转角度
    float   fxPos;
    float   fyPos;
    string  pFontName;
    int     nFontSize;
    int     nFontStyle; // 1-常规；2-粗体；4-斜体；8-黑体
    int     nColor;
    int     nType;
    TextInfo()
    {
        nAngle = 0;
        fxPos = 0;
        nFontSize = 0;
        nFontStyle = 0;
        nColor = 0;
        nType = 0;
    }
}TEXTINFO;

enum CardPostion
{
    Pos_NA = -1,
    Pos_Smart =  1,      // 读磁
    Pos_Contact ,        // 接触
    Pos_ContactLess,     // 非接触
    Pos_Print,           // 打印位
    Pos_EjectCard,       // 出卡口
    Pos_RejectCard       // 废卡箱
};
enum TaskType
{
    Task_Print_Open         ,
    Task_Print_Close        ,
    Task_Print_Reset        ,
    Task_Print_Eject        ,
    Task_Print_Retract      ,
    Task_Print_Dispense     ,
    Task_Print_GetBoxStatus ,
    Task_Print_Status       ,
    Task_Print_InitPrint    ,
    Task_Print_StartPrint   ,
    Task_Print_EnableCard   ,
    Task_Print_DisableCard
};

struct Task
{
    TaskType    nTaskType;
    //string      strTaskName;
    int         nResult = 0;
    char*       pRCode;
    void        *pValue = nullptr;
    long        nTimeout;
    long        nArrayParam[2];
    Task(TaskType nType,char *szRCode,long lTimeout = 0,long nParam1 = 0,long nParam2 = 0)
        :nTaskType(nType)
    {
        nResult = 0;
        pRCode = szRCode;
        strcpy(pRCode,"0000");
        nTimeout = lTimeout;
        nArrayParam[0] = nParam1;
        nArrayParam[1] = nParam2;
    }
    Task() = delete;

};

using TextInfoPtr = std::shared_ptr<TextInfo>;
using TaskPtr = std::shared_ptr<Task>;

class QEvolisPrinter
{
public:
    QEvolisPrinter();
    ~QEvolisPrinter();
//    void ExtractIso();
//    void ExtractFont();
    thread *pThread = nullptr;
    list<TaskPtr> listTask;
    mutex mtlistTask;
    mutex mtComplete;
    bool  bEnablelog = false;
    condition_variable cvComplete,cvTask;
    volatile bool bRunning = false;
    void run() ;
    int ProcessTask(TaskPtr pTask);
    int GetDeviceState(void  *pPrinter, char* majorStatusValue, char* minorStatusValue);
    int GetPrinterStatus(void  *pPrinter,int * RibbonNum,int *Device, int *Media);

    //int Qt_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout);
    int Cv_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout);

    int SetDarkTextRegion(int nLeft,int nTop,int nRight,int nBottom);
    PICINFO m_picInfo;
    list<TextInfoPtr> m_textInfo;
    CardPostion     nCardPosition = Pos_NA;
    string  strOverlayer;
    string  strPreviewFile = "";
    string  strGRibbonType = "RC_YMCKO";
    string  strIFDarkLevelValue = "";
    string  strBColorBrightness = "";
    string  strBColorContrast = "";
    string  strFColorBrightness = "";
    string  strFColorContrast = "";
    map<string,float> mapFontSize;
    Lithograph::LITHOGRAPHBOXUNIT   CardBoxInfo[2];
    void *m_pPrinter = nullptr;
    int  On_Print_Open(char *pPort, char *pPortParam, char *pszRcCode);
    int  On_Print_Close(char *pszRcCode) ;
    int  On_Print_Reset(long lTimeout, int nResetAction, char *pszRcCode) ;
    int  On_Print_Eject(long lTimeout, char *pszRcCode);
    int  On_Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode);
    int  On_Print_EnableCard(long lTimeout, int nCheckable, char *pszRcCode);
    int  On_Print_DisableCard(long lTimeout, char *pszRcCode);
    int  On_Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode);
    int  On_Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode);
    int  On_Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode);
    int  On_Print_InitPrint(long lTimeout, char *pszRcCode);
    int  On_Print_StartPrint(long lTimeout, char *pszRcCode);

    int MoveCardContact();
    void Exit();

    int  Print_Open(char *pPort, char *pPortParam, char *pszRcCode);

    int  Print_Close(char *pszRcCode);

    int  Print_Reset(long lTimeout, int nResetAction, char *pszRcCode);

    int  Print_ResetSync(long lTimeout, int nResetAction, char *pszRcCode);

    int  Print_Eject(long lTimeout, char *pszRcCode);

    int  Print_EjectSync(long lTimeout, char *pszRcCode);

    int  Print_EnableCard(long lTimeout, int nCheckMode, char* pszRcCode);

    int  Print_DisableCard(long lTimeout,  char* pszRcCode);

    int  Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode);

    int  Print_RetractSync(long lTimeout, int nBoxNo, char *pszRcCode);

    int  Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode);

    int  Print_DispenseSync(long lTimeout, int nBox, int nDispPos, char* pszRcCode);

    int  Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode);

    int  Print_GetBoxStatusSync(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode);

    int  Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode);

    int  Print_StatusSync(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode);

    int  Print_InitPrint(long lTimeout, char *pszRcCode);

    int  Print_InitPrintSync(long lTimeout, char *pszRcCode);

    int  Print_StartPrint(long lTimeout, char *pszRcCode);

    int  Print_StartPrintSync(long lTimeout, char *pszRcCode);
};

#endif // QEVOLISPRINTER_H
