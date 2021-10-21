#ifndef QEVOLISPRINTER_H
#define QEVOLISPRINTER_H

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <map>
#include <vector>
#include <chrono>

#include "dynamiclib.h"
//#include "evolislib.h"
#include "readerbase.h"
#include "LithographPrinter.h"
#include "../sdk/evolis/include/evolis_def.h"
#include <string>
using namespace std;
using namespace chrono ;
#define LibVer     "Z390_1.0.1.8 "
struct myFontInfo
{
    string strPath;
    string strStyle;
    myFontInfo(string  path,string style)
        :strPath(path)
        ,strStyle(style)
    {
    }
};

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
enum FontStyle
{
    Style_Normal     = 1,
    Style_Bold       = 2,
    Style_Italic     = 4,
    Style_Black      = 8,
};

typedef struct TextInfo
{
    string  sText;
    int     nAngle; //旋转角度
    float   fxPos;
    float   fyPos;
    string  pFontName;
    int     nFontSize;
    FontStyle  nFontStyle; // 1-常规；2-粗体；4-斜体；8-黑体
    int     nColor;
    int     nType;
    TextInfo()
    {
        nAngle = 0;
        fxPos = 0;
        nFontSize = 0;
        nFontStyle = Style_Normal;
        nColor = 0;
        nType = 0;
    }
}TEXTINFO;


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


using  evolis_log_set_path       = void (*)(const char* path);
using  evolis_log_set_level      = void (*)(evolis_log_t level);
using  evolis_open2              = evolis_t* (*)(const char* device, evolis_type_t type);
using  evolis_close              = void (*)(evolis_t* printer);
using  evolis_get_state          = int (*)(evolis_t* printer, evolis_major_state_t* major, evolis_minor_state_t* minor);
using  evolis_get_major_string   = const char* (*)(evolis_major_state_t major);
using  evolis_get_minor_string   = const char* (*)(evolis_minor_state_t minor);
using  evolis_get_infos          = int (*)(evolis_t* printer, evolis_infos_t* infos);
using  evolis_get_ribbon         = int (*)(evolis_t* printer, evolis_ribbon_t* ribbon);
using  evolis_get_cleaning       = int (*)(evolis_t* printer, evolis_cleaning_t* infos);
using  evolis_command            = ssize_t (*)(evolis_t* printer, const char* cmd, size_t cmdSize, char* reply, size_t replyMaxSize);
using  evolis_set_card_pos       = int (*)(evolis_t* printer, evolis_pos_t pos);
using  evolis_print_init         = int (*)(evolis_t* printer);
using  evolis_reset              = int (*)(evolis_t* printer,int timeout,char* timeouted);
using  evolis_print_set_option   = int (*)(evolis_t* printer, const char* key, const char* value);
using  evolis_print_get_option   = const char* (*)(evolis_t* printer, const char* key);
using  evolis_print_set_imagep   = int (*)(evolis_t* printer, evolis_face_t face, const char* path);
using  evolis_status             = int (*)(evolis_t* printer, evolis_status_t* status);
using  evolis_print_exec         = int (*)(evolis_t* printer);
using  evolis_get_devices        = int (*)(evolis_device_t** devices, int marks, int models);
using  evolis_get_mark_name      = const char* (*)(evolis_mark_t mark);
using  evolis_get_model_name     = const char* (*)(evolis_model_t model);
using  evolis_print_set_imageb   = int (*)(evolis_t* printer, evolis_face_t face, const char* data, size_t size);

using TextInfoPtr = std::shared_ptr<TextInfo>;
using TaskPtr = std::shared_ptr<Task>;
void Runlog(const char* pFunction,int nLine,const char* pFormat, ...);
void FuncRunlog(const char* pFunction,int nLine);

enum CheckType
{
    Both = 0,
    Internal = 1,
    Bezel = 2
};

class TraceFnTime
{
    high_resolution_clock::time_point tStart;
    string strInfoInput;
    char *lpCmd = nullptr;
    char *lpReply = nullptr;

public:
    TraceFnTime(char *szCmd,char *szReply);
    ~TraceFnTime();
};

class FnTime
{
    high_resolution_clock::time_point tStart;
    string strInfoInput;
public:
    FnTime(string strFunc);
    ~FnTime();
};



class QEvolisPrinter:public DynamicLib
{
    void *pHandle = nullptr;
public:
    evolis_log_set_path     pevolis_log_set_path     = nullptr;
    evolis_log_set_level    pevolis_log_set_level    = nullptr;
    evolis_open2            pevolis_open2            = nullptr;
    evolis_close            pevolis_close            = nullptr;
    evolis_get_state        pevolis_get_state        = nullptr;
    evolis_get_major_string pevolis_get_major_string = nullptr;
    evolis_get_minor_string pevolis_get_minor_string = nullptr;
    evolis_get_infos        pevolis_get_infos        = nullptr;
    evolis_get_ribbon       pevolis_get_ribbon       = nullptr;
    evolis_get_cleaning     pevolis_get_cleaning     = nullptr;
    evolis_command          pfnevolis_command        = nullptr;
    evolis_set_card_pos     pevolis_set_card_pos     = nullptr;
    evolis_print_init       pevolis_print_init       = nullptr;
    evolis_reset            pevolis_reset            = nullptr;
    evolis_print_set_option pevolis_print_set_option = nullptr;
    evolis_print_get_option pevolis_print_get_option = nullptr;
    evolis_print_set_imagep pevolis_print_set_imagep = nullptr;
    evolis_status           pevolis_status           = nullptr;
    evolis_print_exec       pevolis_print_exec       = nullptr;
    evolis_get_devices      pevolis_get_devices      = nullptr;
    evolis_get_mark_name    pevolis_get_mark_name    = nullptr;
    evolis_get_model_name   pevolis_get_model_name   = nullptr;
    evolis_print_set_imageb pevolis_print_set_imageb = nullptr;
    ssize_t  pevolis_command(evolis_t* printer, const char* cmd, size_t cmdSize, char* reply, size_t replyMaxSize)
    {
        TraceFnTime t((char *)cmd,reply);
        //this_thread::sleep_for(chrono::milliseconds(50));
        return pfnevolis_command(printer,cmd,cmdSize,reply,replyMaxSize);
    }

public:
    QEvolisPrinter();
    ~QEvolisPrinter();
//    void ExtractIso();
//    void ExtractFont();
//    list<TaskPtr> listTask;
    mutex mtlistTask;
    mutex mtComplete;
    bool  bEnablelog = false;
    //condition_variable cvComplete,cvTask;
    //volatile bool bRunning = false;
    //void run() ;
    int GetDeviceState(char* majorStatusValue, char* minorStatusValue);
    int GetPrinterStatus(int * RibbonNum,int *Device, int *Media,char *szRCode);

    // 0-无卡；   1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
    // nCheckPos = 0,check printer and bezel,
    // nCheckPos = 1 check printer
    // nCheckPos = 2 check bezel
    int CheckCardPostion(int *Media,char *szRCode,CheckType nCheckPos = Both);

   int Qt_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout);
   int Cv_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout,char *pszRcCode);

    int SetDarkTextRegion(int nLeft,int nTop,int nRight,int nBottom);
    PICINFO m_picInfo;
    list<TextInfoPtr> m_textInfo;
    string  strOverlayer;
    string  strPreviewFile = "";
    string  strGRibbonType = "RC_YMCKO";
    string  strIFDarkLevelValue = "";
    string  strBColorBrightness = "";
    string  strBColorContrast = "";
    string  strFColorBrightness = "";
    string  strFColorContrast = "";
    bool    bFault = false;
    bool    bOffline = false;
    bool    bHardwareReset = false;
    int     nResetDelay = 5000;
    map<string,float> mapFontSize;
    QString strFontSimsun;
    evolis_ribbon_t ribbon;
    bool    bNoRibbon = false;
    bool    bCoverOpened = false;
    Lithograph::LITHOGRAPHBOXUNIT   CardBoxInfo[2];
    void *m_pPrinter = nullptr;
    ReaderPtr  pReader = nullptr;
    bool IsOpen()
    {
        return (m_pPrinter != nullptr);
    }
    void CreateEvolislog();
    int  Open(char *pPort, char *pPortParam, char *pszRcCode);
    int  Close(char *pszRcCode) ;
    int  Reset(long lTimeout, int nResetAction, char *pszRcCode) ;
    int  Eject(long lTimeout, char *pszRcCode);
    int  Retract(long lTimeout, int nBoxNo, char *pszRcCode);
    int  EnableCard(long lTimeout, int nCheckable, char *pszRcCode);
    int  EnableCard2(long lTimeout, int nCheckable, char *pszRcCode);
    int  DisableCard(long lTimeout, char *pszRcCode);
    int  Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode);
    int  GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode);
    int  DevStatus(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode);
    int  InitPrint(long lTimeout, char *pszRcCode);
    int  StartPrint(long lTimeout, char *pszRcCode);
    int  InDraftCard(long lTimeout,char *pRCode,bool bCheckCard = true);
    int  SendCommand(const char *lpCmdIn,LPVOID &lpCmdOut,char *szRcCode);
    char szEvolisReply[64];
    int  MoveCard(CardPostion nDstPos);
    int  CheckCardBox(bool &bNoCard,char* pszRcCode);
    //void AddText(char *szText,int nAngle, float fxPos, float fyPos,  int nFontSize, int nColor);
    void PrintText();
    //void Exit();
    bool LoadFontFromResources();
    bool loadFontFamilyFromFiles(const QString &strFontFileName,QString &strFontFamily);
    int  GetRibbonStatus(char *pszRcCode);

    // 0 = succeed
    // 1 = communicate failed
    // 2 = uncompatible
    int CheckPrintZone(char *pszRcCode);
    // 0 = succeed
    // 1 = communicate failed
    // 2 = uncompatible
    // -1 = no ribbon
    int CheckRibbonZone(char *pszRcCode);
};

#endif // QEVOLISPRINTER_H
