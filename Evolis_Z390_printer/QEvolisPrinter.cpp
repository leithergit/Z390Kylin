#include <string>
#include <string.h>
#include <string>
#include <QtDebug>
#include <memory>
#include <QFileInfo>
#include <chrono>
#include <thread>
#include <cstdio>
#include <ctime>
#include <thread>
#include <algorithm>
#include <QDir>
#include <QDateTime>
#include "QEvolisPrinter.h"
#include "AES.h"
#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc_c.h>
using namespace  cv;

#include <QPainter>
#include <QImage>
#include <qfontdatabase.h>


struct BMP_INFOHDR
{
    unsigned long   biSize;
    long			biWidth;
    long			biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned long   biCompression;
    unsigned long   biSizeImage;
    long			biXPelsPerMeter;
    long			biYPelsPerMeter;
    unsigned long   biClrUsed;
    unsigned long   biClrImportant;
};
#pragma pack(push)
#pragma pack(1)
struct BMP_FILEHDR {                     // BMP file header
    char   bfType[2];                    // "BM"
    qint32  bfSize;                      // size of file
    qint16  bfReserved1;
    qint16  bfReserved2;
    qint32  bfOffBits;                   // pointer to the pixmap bits
};
#pragma pack(pop)

const int BMP_FILEHDR_SIZE = 14;               // size of BMP_FILEHDR data
const int BMP_WIN = 40;                        // Windows BMP v3 size
const int BMP_RGB = 0;                         // no compression
const int nCardWidth = 1016;
const int nCardHeight = 648;
using namespace std;
using namespace chrono;
#define LOBYTE(w)         ((unsigned char)(((ULONG)(w)) & 0xff))
#define Value_R(rgb)      (LOBYTE(rgb))
#define Value_G(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define Value_B(rgb)      (LOBYTE((rgb)>>16))
#define MM2Pixel(W)        (W*DPI)/(25.4)

const int DPI = 300;
extern QFile *pFilelog;
extern bool g_bEnableOutput ;
extern int  g_nLogPeriod;
extern bool g_bEnableLog ;
extern int g_nEvolis_logLevel;
extern bool g_bEnalbeEvolislog;

// AES key
unsigned char g_szAesKey[] =  {0x9d,0x06,0x93,0xce,0x80,0x8e,0x37,0xeb,0xd7,0xee,0x09,0x56,0xb5,0xae,0xb3,0xb0};
// 区码F616的密文
unsigned char g_szZoneKey1[16] =  {0x47,0x66,0xf2,0x21,0x79,0x07,0x4f,0xb7,0x28,0x3f,0x6f,0x3a,0x6d,0x17,0xa6,0xc0};
// 区码F61A的密文
unsigned char g_szZoneKey2[16] =  {0x99,0xf4,0xb4,0x85,0xb1,0x10,0xdf,0x75,0x1c,0xec,0xff,0xc8,0xa8,0x15,0xf7,0x38};



#define RunlogF(...)    Runlog(__PRETTY_FUNCTION__,__LINE__,__VA_ARGS__);
#define Funclog()       //FnTime fTime(__PRETTY_FUNCTION__);*FuncRunlog(__PRETTY_FUNCTION__,__LINE__);  TraceFunction TF(__PRETTY_FUNCTION__,__LINE__);//auto threadID = std::this_thread::get_id();RunlogF("####Current ThreadID:%08x####\n",threadID);

#define     CheckHandle()       if (!pHandle)    \
                                {\
                                    RunlogF("libevolis.so is not loaded!\n");\
                                    strcpy(pszRcCode, "0008");\
                                    return 1;\
                                }
#define     CheckHandle2()       if (!pHandle)    \
                                {\
                                    RunlogF("libevolis.so is not loaded!\n");\
                                    return 1;\
                                }
#define     CheckPrinter(x)     CheckHandle();\
                                if (!x) \
                                {\
                                    RunlogF("Not connected to the printer, call Print_Open() first.\n");\
                                    strcpy(pszRcCode, "0003");\
                                    return 1;\
                                }


extern map<string,list<myFontInfo>> map_Font;
#define __countof(array) (sizeof(array)/sizeof(array[0]))
//#pragma warning (disable:4996)
void Runlog(const char* pFunction,int nLine,const char* pFormat, ...)
{
    va_list args;
    va_start(args, pFormat);
    int nBuff;
    CHAR szBuffer[1024] = {0};
    nBuff = vsnprintf(szBuffer, __countof(szBuffer), pFormat, args);
    va_end(args);

    QDateTime tNow = QDateTime::currentDateTime();
    string strInfo = tNow.toString("hh:mm:ss:zzz").toStdString();
    strInfo += " ";
    strInfo += LibVer;
    strInfo += pFunction;
    strInfo += "@";
    strInfo += to_string(nLine);
    strInfo += ":";
    strInfo += szBuffer;
    if (g_bEnableOutput)
        qDebug()<<strInfo.c_str();
    if (pFilelog && g_bEnableLog)
    {
        strInfo += "\n";
        pFilelog->write(strInfo.c_str());
        pFilelog->flush();
    }
}

void FuncRunlog(const char* pFunction,int nLine)
{
    QDateTime tNow = QDateTime::currentDateTime();
    string strInfo = tNow.toString("hh:mm:ss:zzz").toStdString();
    strInfo += " ";
    strInfo += LibVer;

    strInfo += pFunction;
    strInfo += "@";
    strInfo += to_string(nLine);
    strInfo += ":";
    if (g_bEnableOutput)
        qDebug()<<strInfo.c_str();
    if (pFilelog && g_bEnableLog)
    {
        strInfo += "\n";
        pFilelog->write(strInfo.c_str());
        pFilelog->flush();
    }
}

TraceFnTime::TraceFnTime(char *szCmd,char *szReply)
{
    lpCmd = szCmd;
    lpReply = szReply;
    tStart = high_resolution_clock::now();
}

TraceFnTime::~TraceFnTime()
{
    milliseconds tSpan = duration_cast<milliseconds>(high_resolution_clock::now() - tStart);
    QDateTime tNow = QDateTime::currentDateTime();
    string strInfoOuput = tNow.toString("hh:mm:ss:zzz").toStdString();
    strInfoOuput += " ";
    strInfoOuput += LibVer;

    char szCommand[256] = {};
    sprintf(szCommand,"evolis_command(printer,\"%s\",%d,szReply,16):szReply = %s duration = %d .\n",lpCmd,strlen(lpCmd),lpReply,tSpan.count());
    strInfoOuput += szCommand;

    if (g_bEnableOutput)
        qDebug()<<strInfoOuput.c_str();

    if (pFilelog && g_bEnableLog)
    {
        strInfoOuput += "\n";
        pFilelog->write(strInfoOuput.c_str());
        pFilelog->flush();
    }
}


FnTime::FnTime(string strFunc)
{
    strInfoInput = strFunc;
    tStart = high_resolution_clock::now();
}
FnTime::~FnTime()
{
    milliseconds tSpan = duration_cast<milliseconds>(high_resolution_clock::now() - tStart);
    QDateTime tNow = QDateTime::currentDateTime();
    string strInfoOuput = tNow.toString("hh:mm:ss:zzz").toStdString();
    strInfoOuput += " ";
    strInfoOuput += LibVer;

    char szCommand[256] = {};
    sprintf(szCommand,"%s Timespan = %d",strInfoInput.c_str(),tSpan.count());
    strInfoOuput += szCommand;

    if (g_bEnableOutput)
        qDebug()<<strInfoOuput.c_str();

    if (pFilelog && g_bEnableLog)
    {
        strInfoOuput += "\n";
        pFilelog->write(strInfoOuput.c_str());
        pFilelog->flush();
    }
}


class TraceFunction
{
    std::string strFunction;
    int nLine;
public:
    TraceFunction(const char *szFunction,int nLine)
    {
        strFunction = szFunction;
        this->nLine = nLine;
    }
    ~TraceFunction()
    {
        Runlog(strFunction.c_str(),nLine,"Leave %s.\n",strFunction.c_str());
    }
};

int Convert_dib(char **pBuffer, const QImage& img)
 {
   QImage image;
   switch (img.format())
   {
   case QImage::Format_Mono:
   case QImage::Format_Indexed8:
   case QImage::Format_RGB32:
   case QImage::Format_ARGB32:
       image = img;
       break;
   case QImage::Format_MonoLSB:
       image = img.convertToFormat(QImage::Format_Mono);
       break;
   case QImage::Format_Alpha8:
   case QImage::Format_Grayscale8:
       image = img.convertToFormat(QImage::Format_Indexed8);
       break;
   default:
       if (img.hasAlphaChannel())
           image = img.convertToFormat(QImage::Format_ARGB32);
       else
           image = img.convertToFormat(QImage::Format_RGB32);
       break;
   }

   int nSize = image.byteCount();
   BMP_FILEHDR bf;
   BMP_INFOHDR bi;

   int nOffset = 0;
   char* pImageBuffer = new char[nSize + sizeof(bf) + sizeof(bf)];
   if (!pImageBuffer)
       return 0;

   int nbits;
   int bpl_bmp;
   // Calculate a minimum bytes-per-line instead of using whatever value this QImage is using internally.
   int bpl = ((image.width() * image.depth() + 31) >> 5) << 2;

   if (image.depth() == 8 && image.colorCount() <= 16)
   {
       bpl_bmp = (((bpl + 1) / 2 + 3) / 4) * 4;
       nbits = 4;
   }
   else if (image.depth() == 32)
   {
       bpl_bmp = ((image.width() * 24 + 31) / 32) * 4;
       nbits = 24;
   }
   else
   {
       bpl_bmp = bpl;
       nbits = image.depth();
   }
   // build file header
   memcpy(&bf.bfType, "BM", 2);

   // write file header
   bf.bfReserved1 = 0;
   bf.bfReserved2 = 0;
   bf.bfOffBits = BMP_FILEHDR_SIZE + BMP_WIN + image.colorCount() * 4;
   bf.bfSize = bf.bfOffBits + bpl_bmp * image.height();
   memcpy(&pImageBuffer[nOffset], &bf, sizeof(bf));
   nOffset += sizeof(bf);

   bi.biSize = BMP_WIN;                // build info header
   bi.biWidth = image.width();
   bi.biHeight = image.height();
   bi.biPlanes = 1;
   bi.biBitCount = nbits;
   bi.biCompression = BMP_RGB;
   bi.biSizeImage = bpl_bmp * image.height();
   bi.biXPelsPerMeter = image.dotsPerMeterX() ? image.dotsPerMeterX() : 2834; // 72 dpi default
   bi.biYPelsPerMeter = image.dotsPerMeterY() ? image.dotsPerMeterY() : 2834;
   bi.biClrUsed = image.colorCount();
   bi.biClrImportant = image.colorCount();

   memcpy(&pImageBuffer[nOffset], &bi, sizeof(bi));
   nOffset += sizeof(bi);


   if (image.depth() != 32)
   {                // write color table
       uchar* color_table = new uchar[4 * image.colorCount()];
       uchar* rgb = color_table;
       QVector<QRgb> c = image.colorTable();
       for (int i = 0; i < image.colorCount(); i++)
       {
           *rgb++ = qBlue(c[i]);
           *rgb++ = qGreen(c[i]);
           *rgb++ = qRed(c[i]);
           *rgb++ = 0;
       }
       memcpy(&pImageBuffer[nOffset], color_table, 4 * image.colorCount());
       nOffset += 4 * image.colorCount();

       delete []color_table;
   }

   int y;

   if (nbits == 1 || nbits == 8)
   {                // direct output
       for (y = image.height() - 1; y >= 0; y--)
       {
           memcpy(&pImageBuffer[nOffset], image.constScanLine(y), bpl);
           nOffset += bpl;
       }

       return nOffset;
   }

   uchar* buf = new uchar[bpl_bmp];
   uchar* b, * end;
   const uchar* p;

   memset(buf, 0, bpl_bmp);
   for (y = image.height() - 1; y >= 0; y--)
   {        // write the image bits
       if (nbits == 4)
       {                        // convert 8 -> 4 bits
           p = image.constScanLine(y);
           b = buf;
           end = b + image.width() / 2;
           while (b < end)
       {
               *b++ = (*p << 4) | (*(p + 1) & 0x0f);
               p += 2;
           }
           if (image.width() & 1)
               *b = *p << 4;
       }
       else
       {                                // 32 bits
           const QRgb* p = (const QRgb*)image.constScanLine(y);
           const QRgb* end = p + image.width();
           b = buf;
           while (p < end)
       {
               *b++ = qBlue(*p);
               *b++ = qGreen(*p);
               *b++ = qRed(*p);
               p++;
           }
       }
       memcpy(&pImageBuffer[nOffset], buf, bpl_bmp);
       nOffset += bpl_bmp;
   }
   delete[] buf;
   *pBuffer = pImageBuffer;
   return nOffset;
}

float Pt2MM(float pt)
{
    return pt/2.845;
}



bool QEvolisPrinter::loadFontFamilyFromFiles(const QString &strFontFileName,QString &strFontFamily)
{

//    QFileInfo fi(strFontFileName);
//    if (!fi.isFile())
//        return false;

//    int nFontID = QFontDatabase::addApplicationFont(strFontFileName);
//    if (nFontID == -1)
//    {
//        RunlogF("Failed in ");
//        return false;
//    }

//    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(nFontID);
//    if(loadedFontFamilies.empty())
//        return false;

//    strFontFamily = loadedFontFamilies.at(0);
//    FontTable.insert(strFontFileName,strFontFamily);
//    return true;
}

bool QEvolisPrinter::LoadFontFromResources()
{
    return loadFontFamilyFromFiles(":/SIMSUN.ttf",strFontSimsun);
}

QEvolisPrinter::QEvolisPrinter()
{
    Funclog();
    QString strLib = QDir::currentPath();
    strLib += "/libevolis.so";
    pHandle = dlopen(strLib.toUtf8().data(),RTLD_NOW|RTLD_GLOBAL);
    if (!pHandle)
    {
        qDebug()<<"Failed in load "<<strLib;
        qDebug()<<"Error:"<<strerror(errno);
        return ;
    }
    pevolis_log_set_path     = GetProcAddr(pHandle, evolis_log_set_path    );
    pevolis_log_set_level    = GetProcAddr(pHandle, evolis_log_set_level   );
    pevolis_open2            = GetProcAddr(pHandle, evolis_open2           );
    pevolis_close            = GetProcAddr(pHandle, evolis_close           );
    pevolis_get_state        = GetProcAddr(pHandle, evolis_get_state       );
    pevolis_get_major_string = GetProcAddr(pHandle, evolis_get_major_string);
    pevolis_get_minor_string = GetProcAddr(pHandle, evolis_get_minor_string);
    pevolis_get_infos        = GetProcAddr(pHandle, evolis_get_infos       );
    pevolis_get_ribbon       = GetProcAddr(pHandle, evolis_get_ribbon      );
    pevolis_get_cleaning     = GetProcAddr(pHandle, evolis_get_cleaning    );
    pfnevolis_command        = GetProcAddr(pHandle, evolis_command         );
    pevolis_set_card_pos     = GetProcAddr(pHandle, evolis_set_card_pos    );
    pevolis_print_init       = GetProcAddr(pHandle, evolis_print_init      );
    pevolis_reset            = GetProcAddr(pHandle, evolis_reset           );
    pevolis_print_set_option = GetProcAddr(pHandle, evolis_print_set_option);
    pevolis_print_get_option = GetProcAddr(pHandle, evolis_print_get_option);
    pevolis_print_set_imagep = GetProcAddr(pHandle, evolis_print_set_imagep);
    pevolis_status           = GetProcAddr(pHandle, evolis_status          );
    pevolis_print_exec       = GetProcAddr(pHandle, evolis_print_exec      );
    pevolis_get_devices      = GetProcAddr(pHandle, evolis_get_devices     );
    pevolis_get_mark_name    = GetProcAddr(pHandle, evolis_get_mark_name   );
    pevolis_get_model_name   = GetProcAddr(pHandle, evolis_get_model_name  );
    pevolis_print_set_imageb = GetProcAddr(pHandle, evolis_print_set_imageb);
    auto tTime = std::chrono::system_clock::to_time_t(chrono::system_clock::now());
    struct tm* ptm = localtime(&tTime);

}


QEvolisPrinter::~QEvolisPrinter()
{
    Funclog();
}

//void QEvolisPrinter::Exit()
//{
//    Funclog();
//    bRunning = false;
//    cvTask.notify_one();
//}

#define NotifyandWait(x)     {  cvTask.notify_one();\
                                if (x){\
                                    unique_lock<mutex> lockComplete(mtComplete);\
                                    cvComplete.wait(lockComplete);\
                                    RunlogF("Task complete,Result = %d,RcCode = %s.\n",pTask->nResult,pTask->pRCode);\
                                }\
                                return pTask->nResult;\
                              }

static char* tocstring(char* dest, const wchar_t* src)
{
    int n = wcslen(src);

    wcstombs(dest, src, n);
    dest[n] = '\0';
    return dest;
}

// 0 = succeed
// 1 = communicate failed
// 2 = uncompatible

int QEvolisPrinter::CheckPrintZone(char *pszRcCode)
{
    CheckPrinter(m_pPrinter);
    char szReply[128] = {0};
    auto var = "Rzp";
    unsigned char szZone1[16] = {0};
    unsigned char szZone2[16] = {0};
    memcpy(szZone1,g_szZoneKey1,16);
    memcpy(szZone2,g_szZoneKey2,16);
    AES aes(g_szAesKey);
    aes.DeCrypt(szZone1);
    aes.DeCrypt(szZone2);
    //RunlogF("Zone1 = %s.\n",szZone1);
    //RunlogF("Zone2 = %s.\n",szZone2);
    int nIndex = 0;
    // 色带区码标签 1046==F61A,.98C=F616
    if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in adapting printer\n");
        strcpy(pszRcCode,"0006");
        bFault = true;
        return 1;
    }

    string strReply = szReply;
    std::transform(strReply.begin(),strReply.end(),strReply.begin(),::toupper);
    if (strReply != (const char *)szZone1  &&
        strReply != (const char *)szZone2)
    {
        strcpy(pszRcCode, "0004");
        return 2;
    }
    strcpy(pszRcCode, "0000");
    return 0;
}

// 0 = succeed
// 1 = communicate failed
// 2 = uncompatible
// -1 = no ribbon
int QEvolisPrinter::CheckRibbonZone(char *pszRcCode)
{
    CheckPrinter(m_pPrinter);
    char szReply[128] = {0};
    auto var = "Rrt;zone";

    unsigned char szZone1[16] = {0};
    unsigned char szZone2[16] = {0};
    memcpy(szZone1,g_szZoneKey1,16);
    memcpy(szZone2,g_szZoneKey2,16);
    AES aes(g_szAesKey);
    aes.DeCrypt(szZone1);
    aes.DeCrypt(szZone2);

    int nIndex = 0;
    // 色带区码标签 1046==F61A,.98C=F616
    if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in adapting ribbon.\n");
        strcpy(pszRcCode,"0006");
        bFault = true;
        return 1;
    }
    if (strcmp(szReply,"ERR RIB DETECTION") == 0)
    {
        bNoRibbon = true;
        return -1;
    }
    string strReply = szReply;
    std::transform(strReply.begin(),strReply.end(),strReply.begin(),::toupper);
    if (strReply != (const char *)szZone1  &&
        strReply != (const char *)szZone2)
    {
        RunlogF("The ribbon it not compatible with Z390!\n");
        evolis_close(m_pPrinter);
        m_pPrinter = nullptr;
        strcpy(pszRcCode, "0004");
        return 2;
    }
    strcpy(pszRcCode, "0000");
    return 0;
}

void QEvolisPrinter::CreateEvolislog()
{
    if (g_nLogPeriod > 0 && g_bEnalbeEvolislog)
    {
        QString strCurrentPath = QDir::currentPath();
        strCurrentPath += "/log";

        char szFileName[2048] = {0};

        QDateTime tNow = QDateTime::currentDateTime();

        QDir dir(strCurrentPath);
        if (!dir.exists())
            dir.mkpath(strCurrentPath);
        sprintf(szFileName,"%s/Evolis_%04d%02d%02d_%02d%02d%02d.log",strCurrentPath.toUtf8().data(),
                        tNow.date().year(),
                        tNow.date().month(),
                        tNow.date().day(),
                        tNow.time().hour(),
                        tNow.time().minute(),
                        tNow.time().second());

       pevolis_log_set_path(szFileName);
       pevolis_log_set_level((evolis_log_t)g_nEvolis_logLevel);
    }
}

int  QEvolisPrinter::Open(char *pPort, char *pPortParam, char *pszRcCode)
{
    Funclog();
    evolis_device_t* devices;
    if (!pPort || !pPortParam)
    {
        strcpy(pszRcCode,"0005");
        return 1;
    }
    CreateEvolislog();
    CheckHandle();

    char szPrinterPort[64] = {0};
    if (strcmp(pPort,"USB") == 0)
    {
        sprintf(szPrinterPort,"usb://%s",pPortParam);
    }
    else if (strcmp(pPort,"SOCKET") ==0 ||
          strcmp(pPort,"TCP") == 0 ||
          strcmp(pPort,"IP") == 0)
    {
        sprintf(szPrinterPort,"socket://%s",pPortParam);
    }
    else
    {
        strcpy(pszRcCode,"0005");
        return 1;
    }
    RunlogF("Try to call evolis_open(%s:%s).",pPort,pPortParam);
    //m_pPrinter = pevolis_open2("usb:///dev/usb/lp0", EVOLIS_TY_EVOLIS);
    m_pPrinter = pevolis_open2(szPrinterPort, EVOLIS_TY_EVOLIS);
    if (!m_pPrinter)
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }

    if (CheckPrintZone(pszRcCode) != 0)
    {
        evolis_close(m_pPrinter);
        m_pPrinter = nullptr;
        return 1;
    }
    int res = CheckRibbonZone(pszRcCode);
    if (res == 1 ||
        res == 2)
    {
        return 1;
    }

   if (bNoRibbon)
   {
       ribbon.capacity = 0;
       ribbon.remaining = 0;
   }
   else if (GetRibbonStatus(pszRcCode))
   {
       bFault = true;
   }
   char szReply[32] = {0};
   const char *szCmd[]={
                        "Pem;2",       // 将打印机自动纠错复位禁用，完全由上位程序发送指令
                        "Psmgr;2",     // 防止进卡和出卡阻塞
                        "Pcim;F",      // 从卡箱进卡
                        "Pcem;I",      // 从出卡口出卡
                        "Pneab;E",     // 打印结束后不出卡
                        "Pcrm;D",      // 设定出卡箱为废卡箱
                        "Pbc;A;D"
                       };

    for (auto var:szCmd)
    {
        //RunlogF("Try to evolis_command(%s).\n",var);
        if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",var);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
    }

    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::GetRibbonStatus(char *pszRcCode)
{
    CheckPrinter(m_pPrinter);
//    auto tNow = high_resolution_clock::now();
//    if (pevolis_get_ribbon(m_pPrinter, &ribbon))
//    {
//        RunlogF("Failed in evolis_get_ribbon.\n");
//        strcpy(pszRcCode,"0006");
//        bFault = true;
//        return 1;
//    }
//    auto tDuration = duration_cast<milliseconds>(high_resolution_clock::now() - tNow);
//    RunlogF("Duration of pevolis_get_ribbon = %d!\n",tDuration.count());

    char szReply[16] = {0};
    auto szQueryCapcity = "Rrt;qty";
    auto tNow = high_resolution_clock::now();
    int nLoop = 0;
//    do
//    {
        if (pevolis_command(m_pPrinter,szQueryCapcity,strlen(szQueryCapcity),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szQueryCapcity);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        nLoop ++;
        RunlogF("evolis_command(Query Ribbon Capcity) Succeed:%s.\n",szReply);
        // the evolis_command(Rrt;qty) may return an invalid result "OK",if the reply is "OK",try it send the command again
//        if (strcmp(szReply,"OK") == 0 && nLoop < 3)
//        {
//            this_thread::sleep_for(chrono::milliseconds(100));
//            continue;
//        }
//        break;
//    }while(true);

//    if (!strcmp(szReply,"OK"))
//    {
//        return 1;
//    }
    if (!strcmp(szReply,"ERR RIB DETECTION"))
    {
        ribbon.capacity = 0;
        ribbon.remaining = 0;
        strcpy(pszRcCode,"0000");
        return 0;
    }
    ribbon.capacity = strtol(szReply,nullptr,10);
    auto szQueryCount = "Rrt;count";
//    nLoop = 0;
//    do
//    {
        if (pevolis_command(m_pPrinter,szQueryCount,strlen(szQueryCount),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szQueryCount);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
//        nLoop ++;
//        // the evolis_command(Rrt;count) may return an invalid result "OK",if the reply is "OK",try it send the command again
//        if (strcmp(szReply,"OK") == 0 && nLoop < 3)
//        {
//            this_thread::sleep_for(chrono::milliseconds(100));
//            continue;
//        }
//        break;
//    }while (true);

//    if (!strcmp(szReply,"OK"))
//    {
//        return 1;
//    }

    RunlogF("evolis_command(Query Ribbon remained) Succeed:%s.\n",szReply);
    if (!strcmp(szReply,"ERR RIB DETECTION"))
    {
        ribbon.capacity = 0;
        ribbon.remaining = 0;
        strcpy(pszRcCode,"0000");
        return 0;
    }
    ribbon.remaining = strtol(szReply,nullptr,10);
    auto tDuration = duration_cast<milliseconds>(high_resolution_clock::now() - tNow);
    RunlogF("Duration of szRibbincmd = %d!\n",tDuration.count());
    strcpy(pszRcCode,"0000");
    return 0;
}

int  QEvolisPrinter::Close(char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

   // RunlogF("Try to evolis_close.\n");
    pevolis_close(m_pPrinter);
    //RunlogF("evolis_close Succeed.\n");
    m_pPrinter = nullptr;

    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return 0;
}

// nResetAction 1-复位卡保持在设备内；2-复位退卡到打印机取卡位置；3-复位吞卡到打印机回收箱(若硬件支持需实现)
int  QEvolisPrinter::Reset(long lTimeout, int nResetAction, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char szTimeouted[32] = {0};
    char szReply[32];
    if (bHardwareReset)
    {
        if (pevolis_reset(m_pPrinter,lTimeout,szTimeouted) != 0)
        {
            strcpy(pszRcCode, "0001");
            return 1;
        }
        RunlogF("Succeed in evolis_reset timeouted(),Wait for device:%d.\n",szTimeouted,nResetDelay);
        this_thread::sleep_for(chrono::milliseconds(nResetDelay));
        RunlogF("Try to prepare device.\n");
        const char *szCmd[]={
                             "Pem;2",
                             "Psmgr;2",     // 防止进卡和出卡阻塞
                             "Pcim;F",      // 从卡箱进卡
                             "Pcem;M",      // 从出卡口出卡
                             "Pneab;E"      // 打印结束后不出卡
                             "Pbc;A;D"
                            };
         for (auto var:szCmd)
         {
             if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
             {
                 RunlogF("Failed in evolis_command(%s).\n",var);
                 strcpy(pszRcCode,"0006");
                 bFault = true;
                 return 1;
             }
         }
        RunlogF("Prepare device succeed,Try to CheckCardPostion.\n");
    }
    else
        RunlogF("Skip device reset.\n");

    int nCardPos = 0;

    if (CheckCardPostion(&nCardPos,pszRcCode))
    {// nCardPos 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位
        return 1;
    }

    strcpy(pszRcCode, "0000");
    switch (nResetAction)
    {
    default:
    {
        strcpy(pszRcCode, "0005");
        return 1;
    }
    case 1:     // 复位卡保持在设备内
    {
         bFault = false;
        if (nCardPos == Pos_Bezel)     // 卡在出口卡
        {
           return InDraftCard(lTimeout,pszRcCode,false);
        }
        break;
    }
    case 2:     // 复位退卡到打印机取卡位置
    {
        bFault = false;
        switch (nCardPos)
        {
        case Pos_Non:
            break;
        case Pos_Bezel:  //卡在出口卡,insert the card and then eject it
        {
            if (InDraftCard(lTimeout,pszRcCode,false))
            {
                return 1;
            }
        }
        case Pos_Contactless:
        case Pos_Contact:
        default:
        {
            auto var = "Seb";
            if (pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply)) < 0)
            {
                RunlogF("Failed in evolis_command(%s).\n",var);
                strcpy(pszRcCode,"0006");
                bFault = true;
                return 1;
            }
            if (CheckCardPostion(&nCardPos,pszRcCode,Bezel))
                return 1;
            if (nCardPos != Pos_Bezel)
            {
                strcpy(pszRcCode,"0016");
                bFault = true;
                return 1;
            }
        }
        }
        break;
    }
    case 3:     // 复位吞卡到打印机回收箱
    {
        bFault = false;
        switch (nCardPos)
        {
        case Pos_Non:
             break;
        case Pos_Bezel:     //卡在出口卡,insert the card and then eject it
        {
            if (InDraftCard(lTimeout,pszRcCode,false))
                 return 1;
        }
        case Pos_Contactless:
        case Pos_Contact:
        default:
        {
            const char* szCommand[] = {"Ss",
                                       "Pneab;A;1",
                                       "Pcrm;D",    // 设定出卡箱为废卡箱
                                       "Ser" };     // 设置为废卡

            int Res = 0;
            for (auto var : szCommand)
            {
                if (pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply)) < 0)
                {
                    RunlogF("evolis_command(%s)failed.\n",var);
                    strcpy(pszRcCode, "0006");
                    bFault = true;
                    return 1;
                }
            }
            if (CheckCardPostion(&nCardPos,pszRcCode,Internal))
                return 1;
            if (nCardPos != Pos_Non)
            {
                strcpy(pszRcCode,"0016");
                return 1;
            }
            break;
        }
        }
        break;
    }
    }

    bFault = false;
    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return 0;

}

int  QEvolisPrinter::Eject(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char szReply[16] = {0};

    int nCardPos = 0;
    if (CheckCardPostion(&nCardPos,pszRcCode))
    {// 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
        return 1;
    }
    switch (nCardPos)
    {
    case 0:
        return 0;
    case 1:  //卡在出口卡,insert the card and then eject it
    {
        if (InDraftCard(lTimeout,pszRcCode,false))
             return 1;

        if (CheckCardPostion(&nCardPos,pszRcCode,Internal))
            return 1;
        if (nCardPos == Pos_Non)
        {
            strcpy(pszRcCode, "0016");
            bFault = true;
            return 1;
        }
    }
    case 2:
    case 3:
    default:
    {
        auto szCommand ={ "Ss",  "Pneab;A;1",  "Se"};
        int Res = 0;

        for (auto var : szCommand)
        {
            Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
            if (Res < 0)
            {
                RunlogF("evolis_command(%s)failed.\n",var);
                strcpy(pszRcCode, "0006");
                bFault = true;
                return 1;
            }

            this_thread::sleep_for(chrono::milliseconds(100));
        }
        if (CheckCardPostion(&nCardPos,pszRcCode,Bezel))
            return 1;
        if (nCardPos != Pos_Bezel)
        {
            strcpy(pszRcCode, "0016");
            bFault = true;
            return 1;
        }
    }
    }
//    const char* szCommand[] = {
//                                "Ss",
//                                "Pneab;A;1",
//                                "Pcem;I",   // 设置退卡模式(eject):
//                                            //I	退卡到出卡口(bezel,only for kc200B)
//                                            //k	退卡到底部卡槽(lower reject slot, only for kc200B)
//                                "Se"};
//    for (auto var :szCommand)
//    {
//        int error = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
//        if (error < 0)
//        {
//            strcpy(pszRcCode, "0006");
//            RunlogF("Failed in evolis_command(%s).\n",var);
//            return 1;
//        }
//    }
    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    bFault = false;
    return 0;
}

int  QEvolisPrinter::Retract(long lTimeout, int nBoxNo, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    int nCardpos = 0;
    if (CheckCardPostion(&nCardpos,pszRcCode))
         return 1;

    if (nCardpos == Pos_Bezel)
    {
        if (InDraftCard(lTimeout,pszRcCode))
            return 1;
    }
    const char* szCommand[] = {/*"Ss",*/
                               "Pneab;A;1",
                               "Pcrm;D",    // 设定出卡箱为废卡箱
                               "Ser" };     // 设置为废卡

    int Res = 0;
    char szReply[16] = {0};
    for (auto var : szCommand)
    {
        Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
        if (Res < 0)
        {
            RunlogF("evolis_command(%s) failed.\n",var);
            strcpy(pszRcCode, "0006");
            bFault = true;
            return 1;
        }
    }
    if (CheckCardPostion(&nCardpos,pszRcCode,Internal))
         return 1;
    if (nCardpos != Pos_Non)
    {
        strcpy(pszRcCode, "0016");
        bFault = true;
        return 1;
    }
    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    bFault = false;
    return  0;
}


int  QEvolisPrinter::EnableCard(long lTimeout, int nCheckable, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char RespValue[1024];
    memset(RespValue, 0x00, 1024);
    const char *szCommand[]={
                         "Psmgr;2",     // 防止进卡和出卡阻塞
                         "Pcim;I;S",    // 从卡箱进卡,并移动到接触位
                         "Pcem;I",      // 从出卡口出卡
                         //"Pneab;E"    // 打印结束后不出卡
                        };
    // Pcim;B       // 设置为可同时从卡箱和出卡口手动进卡d
    /* 关于Pcim;B
    1. 如果卡盒有卡时输入进卡指令，机器还是会从卡盒进卡，从缝隙处手动插卡无效
    2. 如果卡盒无卡时输入进卡指令，可以此时放一张卡在卡盒或缝隙，都可以进卡
    3. 输入Pcim;B后，即使卡盒无卡，打印机在驱动里也会显示就绪，此前在Pcim;F同状态下会显示卡片装载的黄色警告
    */

    int Res = 0;
    for (auto var : szCommand)
    {

        Res = pevolis_command(m_pPrinter, var,strlen(var), RespValue,1024);
        if (Res < 0)
        {
            RunlogF("evolis_command(%s)failed.\n",var);
            strcpy(pszRcCode, "0006");
            bFault = true;
            return 1;
        }

    }
    if (nCheckable)
    {
        auto tStart = chrono::system_clock::now();
        bool bCardReady = false;
        Lithograph::LITHOGRAPHSTATUS Status;
        Lithograph::LPLITHOGRAPHSTATUS lpStatus = &Status;
        int nResult = 1;
        while (!bCardReady)
        {
            // 检查卡片是否已经就绪
            this_thread::sleep_for(chrono::milliseconds(100));
            nResult = DevStatus(lTimeout,lpStatus,pszRcCode);
            if (nResult == 0 )
            {
                if ((lpStatus->fwMedia == 2 || lpStatus->fwDevice == 3))
                {
                    RunlogF("The card is ready,leave.\n");
                    //MoveCardContact();
                    break;
                }
            }
            else
            {
                RunlogF("Failed in Get Printer Status,nResult = %d.\n",nResult);
            }
            auto duration = duration_cast<milliseconds>(system_clock::now() - tStart);
            if (duration.count() >= lTimeout)
            {
                RunlogF("Wait timeout,leave.\n",nResult);
                break;
            }
        }

        DisableCard(lTimeout,pszRcCode);
    }

    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return  0;
}

int  QEvolisPrinter::EnableCard2(long lTimeout, int nCheckable, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char szReply[32] = {0};

    const char *szCommand[]={
                         "Psmgr;2",     // 防止进卡和出卡阻塞
                         "Pcim;I;S",    // 从卡箱进卡,并移动到接触位
                         "Pcem;M",      // 从出卡口出卡
                         //"Pneab;E"    // 打印结束后不出卡
                        };
    // Pcim;B       // 设置为可同时从卡箱和出卡口手动进卡
    /* 关于Pcim;B
    1. 如果卡盒有卡时输入进卡指令，机器还是会从卡盒进卡，从缝隙处手动插卡无效
    2. 如果卡盒无卡时输入进卡指令，可以此时放一张卡在卡盒或缝隙，都可以进卡
    3. 输入Pcim;B后，即使卡盒无卡，打印机在驱动里也会显示就绪，此前在Pcim;F同状态下会显示卡片装载的黄色警告
    */

    for (auto var : szCommand)
    {
        if (pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply)) < 0)
        {
            RunlogF("evolis_command(%s)failed.\n",var);
            strcpy(pszRcCode, "0006");
            bFault = true;
            return 1;
        }
    }
    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return  0;
}

//struct CommandAgent
//{
//    QEvolisPrinter *pPrinter = nullptr;
//    char szCommand[32];
//    int &nResult;
//    char **pszRzCode;

//    CommandAgent(QEvolisPrinter *Printer,char *szCommand,char **szRcode,int &nRes)
//        :pPrinter(Printer)
//        ,nResult(nRes)
//        ,pszRzCode(szRcode)
//    {
//        strcpy(this->szCommand,szCommand);
//    }
//    ~CommandAgent()
//    {
//        char szReply[32] = {0};
//        if (pPrinter->pevolis_command(szCommand,strlen(szCommand),szReply,sizeof(szReply)) <0)
//        {
//            pPrinter->bFault = true;
//            strcpy(*pszRzCode,"0002");
//            nResult = 1;
//        }
//        else
//        {
//            strcpy(*pszRzCode,"0000");
//            nResult = 0;
//        }
//    }
//};

int  QEvolisPrinter::InDraftCard(long lTimeout,char *pszRcCode,bool bCheckCard)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    char szReply[32] = {0};
    int nCardPos = 0;
    if (bCheckCard)
    {
        if (CheckCardPostion(&nCardPos,pszRcCode))
        {
            return 1;
        }
        if (nCardPos != 1)
        {
            strcpy(pszRcCode, "0017");
            return 1;
        }
    }

    char* szCommand[] = {"Sib","Sis"};
    //if (pReader->GetVender() == Vender::Minhua)
    //    szCommand[1] = "Sic";
    for (auto var:szCommand)
    {
      if (pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply))< 0)
      {
          RunlogF("evolis_command(%s)failed.\n",var);
          strcpy(pszRcCode, "0006");
          bFault = true;
          return 1;
      }

      if (strstr(szReply,"ERROR"))
      {
          if (CheckCardPostion(&nCardPos,pszRcCode))
          {
              return 1;
          }
          if (nCardPos == 1)
          {
              strcpy(pszRcCode, "0016");
              bFault = true;
              return 1;
          }
          else if (nCardPos == 0)
          {
              strcpy(pszRcCode, "0017");
              return 1;
          }
          else
          {
              strcpy(pszRcCode, "0099");
              return 1;
          }
      }
      this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    if (CheckCardPostion(&nCardPos,pszRcCode))
    {
        return 1;
    }
    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    bFault = false;
    return 0;
}

int  QEvolisPrinter::DisableCard(long lTimeout, char *pszRcCode)
{
    Q_UNUSED(lTimeout);
    Funclog();
    CheckPrinter(m_pPrinter);
    char szReply[128] = {0};
    const char *szCmd[]={
                         "Psmgr;2",     // 防止进卡和出卡阻塞
                         "Pcim;F",      // 从卡箱进卡
                         "Pcem;M",      // 从出卡口出卡
                         //"Pneab;E"      // 打印结束后不出卡
                        };
     for (auto var:szCmd)
     {
         if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
         {
             RunlogF("Failed in evolis_command(%s).\n",var);
             strcpy(pszRcCode, "0006");
             bFault = true;
             return 1;
         }
     }

    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return  0;
}

// nDispPos 发卡位置:1-发卡到读磁位置；2-发卡到接触IC卡位置;3-发卡到非接IC卡位置;4-发卡到打印位置
int  QEvolisPrinter::Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

//    if (nBox != 1 ||
//        nDispPos == 1)
//    {
//        RunlogF("Feature not implement,unsupport cardbox or Dispense position.");
//        strcpy(pszRcCode,"0007");
//        return 1;
//    }

    const char *szCmd="Rse;f";
    char szReply1[16] = {0};
    // chechk the card box status
    //RunlogF("Try to evolis_command(%s).\n",szCmd);
    //卡箱无卡时：evolis_command(Rse;f) 返回:0,00 Volts
    //卡箱有卡时：evolis_command(Rse;f) 返回:5,00 Volts，这个可以对上了
    if (pevolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply1,sizeof(szReply1)) < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szCmd,szReply1);
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }

    if (szReply1[0] != '5')
    {
        strcpy(pszRcCode, "0009");
        return 1;
    }

    int nCardPos = 0;
    if (CheckCardPostion(&nCardPos,pszRcCode))
    {// nCardPos 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位
        return 1;
    }

    if (nCardPos == Pos_Bezel)
    {// only test if the card is at bezel
        strcpy(pszRcCode, "0015");
        return 1;
    }

    if ((nCardPos == Pos_Contact && nDispPos == 2) ||
        (nCardPos == Pos_Contactless && nDispPos == 3) ||
        (nCardPos == Pos_Print && nDispPos == 4))
    {
        strcpy(pszRcCode, "0000");
        return 0;
    }

    evolis_pos_e nDestPos;
    auto szCmdArray = {"Smr","Sis","Sic","Si"};
    if (nDispPos == 2 )
    {
//        if (pReader->GetVender() == Vender::Minhua)
//        {
//            nDestPos = EVOLIS_CP_CONTACTLESS;
//            nDispPos = 3;       // convert to 3 if the reader vender is minhua
//            // Sic
//        }
//        else
        {
            nDestPos = EVOLIS_CP_CONTACT;
            // Sis
        }
    }
    else if(nDispPos == 3)
    {
        nDestPos = EVOLIS_CP_CONTACTLESS;
        // Sic
    }
    else if (nDispPos == 4)
    {
        nDestPos = EVOLIS_CP_INSERT;
        // Si
    }
    if (pevolis_set_card_pos(m_pPrinter,nDestPos) < 0)
    {
        strcpy(pszRcCode, "0006");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_INSERT).\n");
        bFault = true;
        return 1;
    }
    if (CheckCardPostion(&nCardPos,pszRcCode,Internal))
    {// nCardPos 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位
        return 1;
    }

    if ((nCardPos != Pos_Contact && nDispPos == 2) ||
        (nCardPos != Pos_Contactless && nDispPos == 3) ||
        (nCardPos != Pos_Print && nDispPos == 4))
    {
        strcpy(pszRcCode, "0016");
        return 1;
    }
    bFault = false;
    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    auto szCmd={"Rse;f",    // Test if there is card in feeder
                            //卡箱无卡时：evolis_command(Rse;f) 返回:0,00 Volts
                            //卡箱有卡时：evolis_command(Rse;f) 返回:5,00 Volts
                "Rse;n",    // Test feeder near empty sensor volt > 1 as:3,92 Volts.
                            // else vol <1 such as :0,17 Volts
                            // if near empty
                "Rlr;h"};   // Test the ejectbox is full
                            // if it's near full,retrn CARD,else return NO CARD
    char szReply[3][16] = {0};
    // chechk the card box status
    //RunlogF("Try to evolis_command(%s).\n",szCmd);
    int i = 0;
    for (auto var:szCmd)
    {
        if (pevolis_command(m_pPrinter,var,strlen(var),szReply[i],sizeof(szReply[i])) < 0)
        {
            RunlogF("Faild in evolis_command(%s),Reply:%s",var,szReply[i]);
            strcpy(pszRcCode, "0006");
            bFault = true;
            return 1;
        }
        i++;
    }

    CardBoxInfo[0].byType = 1;           //卡箱类型 0:未知，1：发卡箱，2：回收箱
    CardBoxInfo[0].byBoxNumber = 1;      //卡箱号
    CardBoxInfo[0].bHardwareSensor = false;//是否支持故障状态传感器,TRUE:支持，FALSE：不支持
    if (szReply[0][0] == '5')
    {
        if (szReply[1][0] > '0')
            CardBoxInfo[0].byStatus = 1;
        else
            CardBoxInfo[0].byStatus = 0;     //卡箱状态,0：正常; 1：卡少; 2:无卡; 3：不可操作; 4：不存在; 5：高(快满)；6：满； 7：未知
    }
    else
    {
        CardBoxInfo[0].byStatus = 2;
    }

    CardBoxInfo[1].byType = 2;
    CardBoxInfo[1].byBoxNumber = 2;
    if (strcmp(szReply[2],"CARD") == 0)
        CardBoxInfo[1].byStatus = 6;
    else
        CardBoxInfo[1].byStatus = 0;
    CardBoxInfo[1].bHardwareSensor = false;

    char majorStatusValue[128] = {0};
    char minorStatusValue[128] = {0};
    if (GetDeviceState(majorStatusValue, minorStatusValue))
    {
        RunlogF("Failed in GetDeviceState.\n");
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }

    RunlogF("majorStatusValue = %s.\n",majorStatusValue);
    RunlogF("minorStatusValue = %s.\n",minorStatusValue);


    lpBoxInfo->nCount = 2;
    lpBoxInfo->lpplist = CardBoxInfo;
   // RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");

    return 0;
}

//WORD fwDevice;		//打印机状态,    0-在线；  1-忙；          2-不在线；      3-故障
//WORD fwMedia;			//介质状态，     0-无卡；  1-卡在门口；     2-卡在内部；    3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
//WORD fwToner;			//平印色带状态,  0-FLLL;  1-LOW;2-OUT;    3-NOTSUPP;    4-UNKNOW
int  QEvolisPrinter::DevStatus(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode)
{
    Q_UNUSED(lTimeout);
    Funclog();
    CheckPrinter(m_pPrinter);

    int RibbonNum = 0;
    int fwDevice = 0;
    int fwMedia = 0;

    if (0 == GetPrinterStatus(&RibbonNum,&fwDevice,&fwMedia,pszRcCode))
    {
        if (bOffline)
        {
            bOffline = false;
            char szReply[32] = {0};
            const char *szCmd[]={
                                 "Psmgr;2",     // 防止进卡和出卡阻塞
                                 "Pcim;F",      // 从卡箱进卡
                                 "Pcem;I",      // 从出卡口出卡
                                 "Pneab;E",     // 打印结束后不出卡
                                 "Pcrm;D",      // 设定出卡箱为废卡箱
                                 "Pbc;A;D"
                                };

             for (auto var:szCmd)
             {
                 //RunlogF("Try to evolis_command(%s).\n",var);
                 if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
                 {
                     RunlogF("Failed in evolis_command(%s).\n",var);
                     strcpy(pszRcCode,"0006");
                     bFault = true;
                     return 1;
                 }
             }
        }
        if (RibbonNum == -98)
            lpStatus->fwToner = 3;
        else
        {
            if (RibbonNum == -1|| RibbonNum == 0)
                lpStatus->fwToner = 2;
            else if (RibbonNum <= 20 )
                lpStatus->fwToner = 1;
            else
                lpStatus->fwToner = 0;
        }

        lpStatus->fwDevice = fwDevice;
        switch (fwMedia)
        {
         case Pos_Non:
        {
            lpStatus->fwMedia = 0;
            lpStatus->fwDevice = 0;
            bFault = false;
        }
            break;
        case Pos_Bezel:
            lpStatus->fwMedia = 1;
            break;
        case Pos_Contactless:
        case Pos_Print:
            lpStatus->fwMedia = 2;
            break;
        case Pos_Contact:
            lpStatus->fwMedia = 3;
            break;
        default:
            lpStatus->fwMedia = 6;
            break;
        }
        if (bFault)
        {
            lpStatus->fwMedia = 5;  // 故障时介质状态为5(jam)
            lpStatus->fwDevice = 3;
        }
        strcpy(pszRcCode, "0000");
        //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
        return 0;
    }
    else
    {
        lpStatus->fwDevice = 2;
        lpStatus->fwMedia = 6;
        lpStatus->fwToner = 4;
        bFault = false;
        bOffline = true;
        strcpy(pszRcCode, "0000");
        //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
        return 0;
    }
}

int  QEvolisPrinter::InitPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    m_textInfo.clear();
    memset(&m_picInfo, 0x00, sizeof(PICINFO));

    //发送Pneab;E
    const char* pCommand = "Pneab;E";       // 打印结束后不出卡
    char szReply[1024] = { 0 };
    int nRes = 1;

    nRes = pevolis_command(m_pPrinter,pCommand, strlen(pCommand), szReply, 1024);
    if (nRes < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",pCommand,szReply);
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }

    this_thread::sleep_for(chrono::milliseconds(10));//1毫秒

    if (pevolis_print_init(m_pPrinter))
    {
        RunlogF("evolis_print_init Failed.\n");
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }
    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return 0;
}

int QEvolisPrinter::MoveCardContact()
{
    Funclog();
    if (!m_pPrinter)
    {
        RunlogF("Printer connection is invalid.\n");
        return 1;
    }

    const char* pCommand = "Sis";       // 卡片移动到接触位
    char szReply[32] = { 0 };

    if (pevolis_command(m_pPrinter,pCommand, strlen(pCommand), szReply, sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",pCommand);
        bFault = true;
        return 1;
    }
    return 0;

}
int  QEvolisPrinter::StartPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    if (!m_textInfo.size())
        RunlogF("There is no Text info.\n");


    if (m_textInfo.size() <= 0 && m_picInfo.picpath.size() <= 0)
    {
        RunlogF("There is no Image or Text info.\n");
        strcpy(pszRcCode, "0022");
        return 1;
    }

    int ec = Cv_PrintCard(m_picInfo, m_textInfo,lTimeout,pszRcCode);
    //int ec = Qt_PrintCard(m_picInfo, m_textInfo,lTimeout);
    if (ec)
        return 1;
    else
    {
        if (MoveCardContact() == 0)
        {
            strcpy(pszRcCode, "0000");
            //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
            return  0;
        }
        else
        {
            strcpy(pszRcCode, "0006");
            bFault = true;
            return  1;
        }
    }
}

int QEvolisPrinter::GetDeviceState(char* majorStatusValue, char* minorStatusValue)
{
    Funclog();
    if (!pHandle)
    {
        RunlogF("libevolis.so is not loaded!\n");\
        return 1;
    }

    evolis_major_state_t major;
    evolis_minor_state_t minor;
    if (pevolis_get_state(m_pPrinter, &major, &minor))
    {
        RunlogF("Failed in evolis_get_state.\n");
        bFault = true;
        return 1;
    }

    strcpy(majorStatusValue, pevolis_get_major_string(major));
    strcpy(minorStatusValue, pevolis_get_minor_string(minor));

    return 0;
}

// Media 介质状态，0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
// nCheckPos = 0,check printer and bezel,
// nCheckPos = 1 check printer
// nCheckPos = 2 check bezel
int QEvolisPrinter::CheckCardPostion(int *Media,char *pszRcCode,CheckType nCheckPos)
{
    Funclog();
    CheckHandle();

    char szReply[32] = {0};

    int nIdx = 0;
    switch (nCheckPos)
    {
    case Both:
    default:
    {
        const char *szCmd[]={"Rlr;p",      // 查询打印内部
                            "Rse;b"};      // 查询出卡口
        if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        RunlogF("evolis_command(Check Printer Internal) Succeed:%s.\n",szReply);
        char szReply2[32] = {0};
        nIdx ++;
        if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply2,sizeof(szReply2)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        RunlogF("evolis_command(Check Bezel) Succeed:%s.\n",szReply);
        //Media 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
        if (strcmp(szReply,"OK") == 0)
        {
            // 0,2x Volts是没卡
            // 4,9x Volts是有卡
            szReply2[1] = '.';
            float fVolt = atof(szReply2);
            RunlogF("%s Status:%.2f.\n",szCmd[nIdx],fVolt);
            if (fVolt >= 1.0)
                *Media = Pos_Bezel;
            else
                *Media = Pos_Non;
        }
        else if (strcmp(szReply,"CARD") == 0)
            *Media = Pos_Print;
        else if (strstr(szReply,"CONTACTLESS"))
            *Media = Pos_Contactless;
        else if (strstr(szReply,"SMART"))
            *Media = Pos_Contact;
        break;
    }
    case Internal:
    {
        auto var = "Rlr;p";      // 查询打印内部
        if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",var);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        RunlogF("evolis_command(Check Printer Internal) Succeed%s:.\n",szReply);
        if (strcmp(szReply,"OK") == 0)
            *Media = Pos_Non;
        else if (strcmp(szReply,"CARD") == 0)
            *Media = Pos_Print;
        else if (strstr(szReply,"CONTACTLESS"))
            *Media = Pos_Contactless;
        else if (strstr(szReply,"SMART"))
            *Media = Pos_Contact;
    }
        break;
    case Bezel:
    {
        auto var = "Rse;b";      // 查询出卡口
        if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",var);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        RunlogF("evolis_command(Check Bezel) Succeed%s:.\n",szReply);
        //Media 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
        // 0,2x Volts是没卡
        // 4,9x Volts是有卡
        szReply[1] = '.';
        float fVolt = atof(szReply);
        RunlogF("%s Status:%.2f.\n",var,fVolt);
        if (fVolt >= 1.0)
            *Media = Pos_Bezel;
        else
            *Media = Pos_Non;
    }
        break;
    }
    if (*Media == Pos_Non)
    {
        bFault = false;
    }
    pReader->SetCardSlot(*Media);
    return 0;
}

//WORD fwDevice;		//打印机状态,    0-在线；   1-忙；     2-不在线；3-故障
//WORD fwMedia;			//介质状态，     0-无卡；   1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
//WORD fwToner;			//平印色带状态,  0-FLLL;  1-LOW;2-OUT;3-NOTSUPP;4-UNKNOW
/*
Rlr;p的对应结果
机器内没卡-->OK
卡在打印位-->CARD
卡在芯片位-->SMART CARD
卡在非接位-->CONTACTLESS CARD
// 出卡口
Rse;M
0.2x Volts是没卡
4.9x Volts是有卡
*/
int QEvolisPrinter::GetPrinterStatus(int * RibbonNum,int *Device, int *Media,char *pszRcCode)
{
    Funclog();
    CheckHandle();

    *Media = 0;        // No card

//  test the duratoin of gettting ribbon info
//
//    auto tNow = high_resolution_clock::now();
//    if (pevolis_get_ribbon(m_pPrinter, &ribbon))
//    {
//        RunlogF("Failed in evolis_get_ribbon.\n");
//        strcpy(pszRcCode,"0006");
//        bFault = true;
//        return 1;
//    }
//    auto tDuration = duration_cast<milliseconds>(high_resolution_clock::now() - tNow);
//    RunlogF("Duration of pevolis_get_ribbon = %d!\n",tDuration.count());
//    auto szRibbincmd = {"Rrt;qty","Rrt;count"};
//    tNow = high_resolution_clock::now();
//    char szReply[32] = {0};
//    for (auto var:szRibbincmd)
//    {
//        //RunlogF("Try to evolis_command(%s).\n",var);
//        if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
//        {
//            RunlogF("Failed in evolis_command(%s).\n",var);
//            strcpy(pszRcCode,"0006");
//            bFault = true;
//            return 1;
//        }
//    }
//    tDuration = duration_cast<milliseconds>(high_resolution_clock::now() - tNow);
//    RunlogF("Duration of szRibbincmd = %d!\n",tDuration.count());
    char majorStatusValue[128] = {0};
    char minorStatusValue[128] = {0};
    if (GetDeviceState(majorStatusValue,minorStatusValue))
    {
        RunlogF("Failed in GetDeviceState.\n");
        strcpy(pszRcCode,"0006");
        bFault = true;
        return 1;
    }

    auto szDetectCover = "Rse;o";       // read cover sensor
    char szReply[32] = {0};
    //设备上盖开后：evolis_command(Rse;o) 返回:0,00 Volts
    //设备上盖闭合：evolis_command(Rse;f) 返回:5,00 Volts
    if (pevolis_command(m_pPrinter,szDetectCover,strlen(szDetectCover),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szDetectCover,szReply);
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }
    RunlogF("evolis_command(Check cover) Succeed%s:.\n",szReply);
    if (szReply[0] == '0')
    {
        bCoverOpened = true;
        if (GetRibbonStatus(pszRcCode))
            return 1;
    }
    else if (bCoverOpened)
    {
        bCoverOpened = false;
        if (GetRibbonStatus(pszRcCode))
            return 1;
    }

    RunlogF("ribbon.capacity = %d\tribbon.remaining = %d.",ribbon.capacity,ribbon.remaining);
    if (ribbon.capacity == 0 &&
        ribbon.remaining == 0)
    {
        *RibbonNum = -1;
    }
    else
        *RibbonNum= ribbon.remaining;
    // 无卡        majorStatusValue = READY         minorStatusValue = DEVICE_READY
    // 卡在机器内部  majorStatusValue = WARNING       minorStatusValue = DEVICE_BUSY
    // 卡在出卡口    majorStatusValue = WARNING       minorStatusValue = STDEVOLIS_DEF_CARD_ON_EJECT
    //             majorStatusValue = WARNING       minorStatusValue = EVOLIS_MI_STDEVOLIS_DEF_COVER_OPEN

    if (strcmp(minorStatusValue, "DEF_UNSUPPORTED_RIBBON") == 0)
        *RibbonNum = -98;

    if(strcmp(minorStatusValue,"PRINTER_OFFLINE") == 0)
    {
        *Device = 2;//是否离线
        *Media = 6;
        strcpy(pszRcCode,"0006");
        return 1;
    }
    else  if(CheckCardPostion(Media,pszRcCode))
        return 1;

    evolis_status_t es;
    //RunlogF("Try to evolis_status.\n");
    if (pevolis_status(m_pPrinter, &es) != 0)
    {
        RunlogF("Failed evolis_status.\n");
        strcpy(pszRcCode,"0006");
        bFault = true;
        *Device = 2;    // 离线
    }
    else
    {
        if (bFault)
        {
            *Device = 3;    // fault
            *Media = 5;
        }
        else
            *Device = 0;    // 在线
        RunlogF("evolis_status return:config = %d\tinformation = %d\twarning = %d\terror = %d.\n", es.config,es.information, es.warning,es.error);
        for (int i = 0;i < EVOLIS_STATUS_EX_COUNT;i ++)
            RunlogF("evolis_status return exts[%d] = %d.\n",i, es.exts[i]);
    }

    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    return 0;
}

int QEvolisPrinter::SetDarkTextRegion(int nLeft, int nTop, int nRight, int nBottom)
{
    if (!pHandle)
        return 1;

    if (nLeft < 0 || nTop <0 || nRight < 0 || nBottom<0 )
        return 1;

    if (pevolis_print_set_option(m_pPrinter, "FBlackManagement", " TEXTINBLACK"))
    {
         RunlogF("Failed in evolis_print_set_option(FBlackManagement,TEXTINBLACK).\n");
         bFault = true;
         return 1;
    }

    pevolis_print_set_option(m_pPrinter, "IFTextRegion", "0x0x1000x600");

    if (pevolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "40"))
    {
         RunlogF("Failed in evolis_print_set_option(IFBlackLevelValue,40).\n");
         bFault = true;
         return 1;
    }

}


#define LOBYTE(w)         ((unsigned char)(((ULONG)(w)) & 0xff))
#define Value_R(rgb)      (LOBYTE(rgb))
#define Value_G(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define Value_B(rgb)      (LOBYTE((rgb)>>16))

int QEvolisPrinter::Qt_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout)
{
	Funclog();
	CheckHandle2();

	if (!m_pPrinter)
	   return 1;
    auto tStart = high_resolution_clock::now();
	QFileInfo fipic(inPicInfo.picpath.c_str());
	if (!fipic.isFile() && !inTextVector.size())
	{
	   RunlogF("There is no any content to print!\n");
	   return 1;
	}
    
    if (pevolis_print_init((evolis_t*)m_pPrinter))
    {
        RunlogF("evolis_print_init failed.\n");
        return 1;
    }

   if (strFColorBrightness.size())
   {
       if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"FColorBrightness",strFColorBrightness.c_str()))
       {
            RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorBrightness",strFColorBrightness.c_str());
            bFault = true;
       }
   }

   if (strBColorBrightness.size())
   {
       if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"BColorBrightness",strBColorBrightness.c_str()))
       {
            RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorBrightness",strBColorBrightness.c_str());
            bFault = true;
       }
   }

   if (strFColorContrast.size())
   {
       if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"FColorContrast",strFColorContrast.c_str()))
       {
            RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorContrast",strFColorContrast.c_str());
            bFault = true;
       }
   }

   if (strBColorContrast.size())
   {
       if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"BColorContrast",strBColorContrast.c_str()))
       {
            RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorContrast",strBColorContrast.c_str());
            bFault = true;
       }
   }
    const char *szCmd[]={"Rrt;count","Pneab;A;0","Pneab;A;1"};
    char szReply[128] = {0};
    int nIdx = 0;
    RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        bFault = true;
        return 1;
    }

    int nRibbonCount = strtol(szReply,nullptr,10);
    if (nRibbonCount <= 0)
    {
        RunlogF("The ribbon is exhausted and printing failed.\n");
        return 1;
    }
    nIdx++;
    RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        bFault = true;
        return 1;
    }

    nIdx++;

    QImage Convas(nCardWidth,nCardHeight,  QImage::Format_RGB888);
    Convas.fill(Qt::white);
    QPainter painter(&Convas);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    QImage FileImage;
    if (!FileImage.load(QString::fromStdString( inPicInfo.picpath)))
    {
        RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
        return 1;
    }
    RunlogF("Graph:%s\txPos = %.1f\tyPos = %.1f\tAngle = %d.\n",inPicInfo.picpath.c_str(),inPicInfo.fxPos,inPicInfo.fyPos,inPicInfo.nAngle);
    painter.save();
    painter.translate(nCardWidth/2,nCardHeight/2);
    painter.rotate(inPicInfo.nAngle);
    painter.translate(-nCardWidth/2,-nCardHeight/2);
    painter.drawImage(QRect(MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),MM2Pixel(inPicInfo.fWidth),MM2Pixel(inPicInfo.fHeight)),FileImage);
    painter.restore();

    int nDarkLeft = 0,nDarkTop = 0,nDarkRight = 0,nDarkBottom = 0;
    for (auto var : inTextVector)
    {
        QString imageText = QString::fromStdString(var->sText);

       RunlogF("Text = %s,FontSize = %d,xPos = %.02f,yPos = %.2f.\n",var->sText.c_str(),var->nFontSize);
       QRect rtROI(MM2Pixel(var->fxPos), MM2Pixel(var->fyPos),nCardWidth - MM2Pixel(var->fxPos) - 2, nCardHeight - MM2Pixel(var->fyPos) - 2);
       if (!nDarkLeft)
           nDarkLeft = rtROI.left();
       else
           nDarkLeft = nDarkLeft<=rtROI.left()?nDarkLeft:rtROI.left();
       if (!nDarkTop)
           nDarkTop = rtROI.top();
       else
           nDarkTop = nDarkTop<=rtROI.top()?nDarkTop:rtROI.top();
       if (!nDarkRight)
           nDarkRight = rtROI.left() + rtROI.width();
       else
           nDarkRight = nDarkRight>=(rtROI.left() + rtROI.width())?nDarkRight:(rtROI.left() + rtROI.width());
       if (!nDarkBottom)
           nDarkBottom = rtROI.top() + rtROI.height();
       else
           nDarkBottom = nDarkBottom>=(rtROI.top() + rtROI.height())?nDarkBottom:(rtROI.top() + rtROI.height());


        QFont font(strFontSimsun);
        //设置显示字体的大小
        int nFontSize = (int)round(MM2Pixel(Pt2MM(var->nFontSize)));
        font.setPixelSize(nFontSize);
        QFontMetrics fm(font);

        auto TextRect = fm.boundingRect(imageText);
        painter.setFont(font);
        QPen pen = painter.pen();

        pen.setColor(QColor(Value_R(var->nColor),Value_G(var->nColor),Value_B(var->nColor)));
        painter.setPen(pen);
        RunlogF("Text:%s\txPos = %.1f\tyPos = %.1f\tAngle = %d.\n",var->sText.c_str(),var->fxPos,var->fyPos,var->nAngle);
        painter.save();
        painter.translate(nCardWidth/2,nCardHeight/2);
        painter.rotate(var->nAngle);
        painter.translate(-nCardWidth/2,-nCardHeight/2);
        painter.drawText(QRectF(MM2Pixel(var->fxPos), MM2Pixel(var->fyPos), MM2Pixel(TextRect.width()), MM2Pixel(TextRect.height())), Qt::AlignLeft | Qt::AlignTop, imageText);
        painter.restore();
    }
    QString strTempFile = QDir::currentPath();
    strTempFile += "/PrintTemp.bmp";
    Convas.save(strTempFile);

    char *pFileBuffer = nullptr;
    int nBufferSize = Convert_dib(&pFileBuffer,Convas);
    if (!nBufferSize)
    {
        if (pFileBuffer)
            delete []pFileBuffer;
        RunlogF("failed in  Convert_dib:%s.\n");
        return 1;
    }
    shared_ptr<char> Buffdealloc(pFileBuffer);
   strOverlayer = QDir::currentPath().toStdString();
   strOverlayer += "/resources/iso.bmp";
   QFileInfo fi(strOverlayer.c_str());
   if (!fi.isFile())
   {
       RunlogF("Can't open file %s.\n",strOverlayer.c_str());
       return 1;
   }
   if (pevolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
   {
       RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
       return 1;
   }

   if (strGRibbonType.size())
   {
       if (pevolis_print_set_option(m_pPrinter, "GRibbonType", strGRibbonType.c_str()))
       {
           RunlogF("Failed in evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
           return 1;
       }
   }

   if (pevolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
   {
       RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
       bFault = true;
       return 1;
   }

   if (pevolis_print_set_option(m_pPrinter, "FBlackManagement", "TEXTINBLACK"))
   {
       RunlogF("Failed in evolis_print_set_option(FBlackManagement,TEXTINBLACK).\n");
       bFault = true;
       return 1;
   }

   char szDarkZone[32] = {0};
   if (inPicInfo.nAngle > 0)
       sprintf(szDarkZone,"%dx%dx%dx%d",0,0,nCardWidth - nDarkLeft,nCardHeight - nDarkTop);
   else
       sprintf(szDarkZone,"%dx%dx%dx%d",nDarkLeft,nDarkTop,nDarkRight,nDarkBottom);

   if (pevolis_print_set_option(m_pPrinter, "IFTextRegion", szDarkZone))
   {
       RunlogF("Failed in evolis_print_set_option(IFTextRegion,%s).\n",szDarkZone);
       bFault = true;
       return 1;
   }

   if (pevolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "50"))
   {
       RunlogF("Failed in evolis_print_set_option(IFBlackLevelValue,50).\n");
       bFault = true;
       return 1;
   }

   if (pevolis_print_set_option(m_pPrinter, "IFDarkLevelValue ", "30"))
   {
       RunlogF("Failed in evolis_print_set_option(IFDarkLevelValue,30).\n");
       bFault = true;
       return 1;
   }

//    if (pevolis_print_set_imageb(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT, pFileBuffer,nBufferSize))
//    {
//        RunlogF("evolis_print_set_imageb failed:%s.\n",inPicInfo.picpath.c_str());
//        return 1;
//    }
    if (pevolis_print_set_imagep(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT,strTempFile.toUtf8().data()))
    {
        RunlogF("evolis_print_set_imagep failed:%s.\n",strTempFile.toUtf8().data());
        bFault = true;
        return 1;
    }
    auto tSpan = duration_cast<milliseconds>(tStart - high_resolution_clock::now());
    RunlogF("Prepare print duration:%d.",tSpan.count());
    pevolis_print_exec((evolis_t*)m_pPrinter);
    int             printed = 0;
    evolis_status_t es;
    auto tNow = system_clock::now();
    auto tLast = tNow;
    while (!printed)
    {
        if (pevolis_status(m_pPrinter, &es) != 0)
        {
            RunlogF("Error reading printer status\n");
            return 0;
        }
        printed = !(es.information & EVOLIS_INF_BUSY);
        tNow = system_clock::now();
        auto tDeration = duration_cast<milliseconds>(tNow - tLast);
        if (tDeration.count() > 500)
        {
            tLast = tNow;
            RunlogF("Waiting printer task stop\n");
        }
		this_thread::sleep_for(chrono::milliseconds(50));
    }
    return 0;
}

Mat matRotateClockWise90(Mat src)
{
    if (src.empty())
    {
        qDebug()<<"RorateMat src is empty!";
    }
    // 矩阵转置
    transpose(src, src);
    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
    flip(src, src, 1);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
    return src;
}

Mat matRotateClockWise180(Mat src)//顺时针180
{
    if (src.empty())
    {
        qDebug() << "RorateMat src is empty!";
    }

    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
    flip(src, src, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
    flip(src, src, 1);
    return src;
    //transpose(src, src);// 矩阵转置
}

Mat matRotateClockWise270(Mat src)//顺时针270
{
    if (src.empty())
    {
        qDebug() << "RorateMat src is empty!";
    }
    // 矩阵转置
    //transpose(src, src);
    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
    transpose(src, src);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
    flip(src, src, 0);
    return src;
}

Mat myRotateAntiClockWise90(Mat src)//逆时针90°
{
    if (src.empty())
    {
      qDebug()<<"mat is empty!";
    }
    transpose(src, src);
    flip(src, src, 0);
}

#define FailedCode(x)  strcpy(pszRcCode,#x);return 1;

int QEvolisPrinter::Cv_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout,char *pszRcCode)
{
    Funclog();
    CheckHandle2();
    if (!m_pPrinter)
        return 1;
    auto tStart = high_resolution_clock::now();
    QFileInfo fipic(inPicInfo.picpath.c_str());
    if (!fipic.isFile() && !inTextVector.size())
    {
        RunlogF("There is nothing to print!\n");
        FailedCode(0022);
    }

    int res = CheckRibbonZone(pszRcCode);
    if (res == 1 ||
        res == 2)
    {
        return 1;
    }
// 此处待完善
//    evolis_status_t es;
//    if (evolis_status(m_pPrinter, &es) != 0)
//    {
//        RunlogF("Error reading printer status\n");
//        return 0;
//    }

    //RunlogF("Try to evolis_print_init.\n");
    if (pevolis_print_init((evolis_t*)m_pPrinter))
    {
        RunlogF("evolis_print_init failed.\n");
        FailedCode(0006);
         bFault = true;
    }
    if (strFColorBrightness.size())
    {
        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"FColorBrightness",strFColorBrightness.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorBrightness",strFColorBrightness.c_str());
             bFault = true;
             FailedCode(0006);
        }
    }

    if (strBColorBrightness.size())
    {
        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"BColorBrightness",strBColorBrightness.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorBrightness",strBColorBrightness.c_str());
             bFault = true;
             FailedCode(0006);
        }
    }

    if (strFColorContrast.size())
    {
        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"FColorContrast",strFColorContrast.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorContrast",strFColorContrast.c_str());
             bFault = true;
             FailedCode(0006);
        }
    }

    if (strBColorContrast.size())
    {
        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"BColorContrast",strBColorContrast.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorContrast",strBColorContrast.c_str());
             bFault = true;
             FailedCode(0006);
        }
    }

    const char *szCmd[]={"Rrt;count","Pneab;A;0","Pneab;A;1"};
    char szReply[128] = {0};
    int nIdx = 0;
    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        bFault = true;
        FailedCode(0006);
    }
    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
    int nRibbonCount = strtol(szReply,nullptr,10);
    if (nRibbonCount <= 0)
    {
        RunlogF("Run out of ribbon and print failed.\n");
        FailedCode(0021);
    }
    nIdx++;
    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s):%s.\n",szCmd[nIdx]);
        bFault = true;
        FailedCode(0006);
    }
    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
    nIdx++;

    Mat canvas(nCardHeight,nCardWidth,CV_8UC3,Scalar(255,255,255));

    //RunlogF("Try to load file %s.\n",inPicInfo.picpath.c_str());

    Mat HeaderImage = imread(inPicInfo.picpath, IMREAD_ANYCOLOR);
    if (!HeaderImage.data)
    {
         RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
         FailedCode(0013);
    }
    Mat HeaderPrint;
    resize(HeaderImage, HeaderPrint, Size(MM2Pixel(inPicInfo.fWidth) , MM2Pixel(inPicInfo.fHeight)), 0, 0, INTER_NEAREST);

    Mat HeaderROI = canvas(Rect(MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),MM2Pixel(inPicInfo.fWidth),MM2Pixel(inPicInfo.fHeight)));
    HeaderPrint.copyTo(HeaderROI);

    int nDarkLeft = 0,nDarkTop = 0,nDarkRight = 0,nDarkBottom = 0;

    for (auto var : inTextVector)
    {
        //RunlogF("Text = %s,FontSize = %d,xPos = %.02f,yPos = %.2f.\n",var->sText.c_str(),var->nFontSize);
        Rect rtROI(MM2Pixel(var->fxPos), MM2Pixel(var->fyPos),nCardWidth - MM2Pixel(var->fxPos) - 2, nCardHeight - MM2Pixel(var->fyPos) - 2);
        if (!nDarkLeft)
            nDarkLeft = rtROI.x;
        else
            nDarkLeft = nDarkLeft<=rtROI.x?nDarkLeft:rtROI.x;
        if (!nDarkTop)
            nDarkTop = rtROI.y;
        else
            nDarkTop = nDarkTop<=rtROI.y?nDarkTop:rtROI.y;
        if (!nDarkRight)
            nDarkRight = rtROI.x + rtROI.width;
        else
            nDarkRight = nDarkRight>=(rtROI.x + rtROI.width)?nDarkRight:(rtROI.x + rtROI.width);
        if (!nDarkBottom)
            nDarkBottom = rtROI.y + rtROI.height;
        else
            nDarkBottom = nDarkBottom>=(rtROI.y + rtROI.height)?nDarkBottom:(rtROI.y + rtROI.height);

        Mat FontROI = canvas(rtROI);
        int fontHeight = (int)round(MM2Pixel(Pt2MM(var->nFontSize)));
        //RunlogF("Text = %s,FontSize = %.2f, FontPixel = %d.\n",var->sText.c_str(),var->nFontSize,fontHeight);
        try
        {
            cv::Ptr<cv::freetype::FreeType2> ft2;
            ft2 = cv::freetype::createFreeType2();

            ft2->loadFontData(var->pFontName, 0); //加载字库文件
            ft2->putText(FontROI, var->sText.c_str(), cv::Point(0, 0), fontHeight, CV_RGB(0, 0, 0), -1, CV_AA, false,var->nFontStyle == Style_Bold);
        }
        catch (std::exception &e)
        {
            RunlogF("Catch a exception:%s.\n",e.what());
            FailedCode(0018);
        }
    }

    if (strPreviewFile.size())
        imwrite(strPreviewFile.c_str(),canvas);
    QString strTempFile = QDir::currentPath();
    strTempFile += "/PrintTemp.bmp";
    if (inPicInfo.nAngle > 0)
    {
        //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
        flip(canvas, canvas, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
        flip(canvas, canvas, 1);
    }
    imwrite(strTempFile.toUtf8().data(),canvas);
    strOverlayer = QDir::currentPath().toStdString();
    strOverlayer += "/resources/iso.bmp";
    QFileInfo fi(strOverlayer.c_str());
    if (!fi.isFile())
    {
        RunlogF("Can't open file %s.\n",strOverlayer.c_str());
        FailedCode(0013);
    }
    
    if (pevolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
        bFault = true;
        FailedCode(0006);
    }

    if (strGRibbonType.size())
    {
        if (pevolis_print_set_option(m_pPrinter, "GRibbonType", strGRibbonType.c_str()))
        {
            RunlogF("Failed in evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
            bFault = true;
            FailedCode(0006);
        }
    }

    if (pevolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
        bFault = true;
        FailedCode(0006);
    }

    if (pevolis_print_set_option(m_pPrinter, "FBlackManagement", "TEXTINBLACK"))
    {
        RunlogF("Failed in evolis_print_set_option(FBlackManagement,TEXTINBLACK).\n");
        bFault = true;
        FailedCode(0006);
    }

    char szDarkZone[32] = {0};
    if (inPicInfo.nAngle > 0)
        sprintf(szDarkZone,"%dx%dx%dx%d",0,0,nCardWidth - nDarkLeft,nCardHeight - nDarkTop);
    else
        sprintf(szDarkZone,"%dx%dx%dx%d",nDarkLeft,nDarkTop,nDarkRight,nDarkBottom);

    if (pevolis_print_set_option(m_pPrinter, "IFTextRegion", szDarkZone))
    {
        RunlogF("Failed in evolis_print_set_option(IFTextRegion,%s).\n",szDarkZone);
        bFault = true;
        FailedCode(0006);
    }

    if (pevolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "50"))
    {
        RunlogF("Failed in evolis_print_set_option(IFBlackLevelValue,50).\n");
        bFault = true;
        FailedCode(0006);
    }

    if (pevolis_print_set_option(m_pPrinter, "IFDarkLevelValue ", "30"))
    {
        RunlogF("Failed in evolis_print_set_option(IFDarkLevelValue,30).\n");
        bFault = true;
        FailedCode(0006);
    }

    if (pevolis_print_set_imagep(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT,strTempFile.toUtf8().data()))
    {
        RunlogF("evolis_print_set_imagep failed:%s.\n",strTempFile.toUtf8().data());
        bFault = true;
        FailedCode(0006);
    }
    auto tSpan = duration_cast<milliseconds>( high_resolution_clock::now() - tStart);
    //RunlogF("Prepare print duration:%d.",tSpan.count());

    RunlogF("Try to evolis_print_exec.\n");
    pevolis_print_exec((evolis_t*)m_pPrinter);
    int             printed = 0;
    evolis_status_t es;
    auto tLast = high_resolution_clock::now();
    while (!printed)
    {
        if (pevolis_status(m_pPrinter, &es) != 0)
        {
            RunlogF("Error reading printer status\n");
            bFault = true;
            FailedCode(0006);
        }
        printed = !(es.information & EVOLIS_INF_BUSY);
        this_thread::sleep_for(chrono::milliseconds(100));
//        auto tNow = high_resolution_clock::now();
//        auto tDeration = duration_cast<milliseconds>(tNow - tLast);
//        if (tDeration.count() > nTimeout*1000)
//        {
//            RunlogF("Waiting printer task stop\n");
//            break;
//        }
    }
    if (GetRibbonStatus(pszRcCode))
    {
        RunlogF("Failed in GetRibbonStatus.\n");
        return 1;
    }
    auto tDeration = duration_cast<milliseconds>(high_resolution_clock::now() - tLast);
    RunlogF("print duration:%d ms.",tDeration.count());
    return 0;
}

int  QEvolisPrinter::SendCommand(const char *lpCmdIn,LPVOID &lpCmdOut,char *pszRcCode)
{
     Funclog();
     CheckHandle();

     if (pevolis_command(m_pPrinter,lpCmdIn,strlen(lpCmdIn),szEvolisReply,sizeof(szEvolisReply)) < 0)
     {
         strcpy(pszRcCode,"0006");
         return 1;
     }
     else
     {
         lpCmdOut = &szEvolisReply;
         strcpy(pszRcCode,"0000");
         return 0;
     }
}


//void  QEvolisPrinter::PrintText()
//{
//    int nAngle = 180;
//    int size = 8;

//    QString strUser    = "姓名  测试用户";
//    QString strID      = "社会保障号码  123456789012345678";
//    QString strCard    = "社会保障卡号  ABCDEFGHIJKLMN";
//    QString strDate    = "发卡日期  2019年9月27日";
//    AddText(strUser.toLocal8Bit().data(),nAngle,28,14.5,size,0);
//    AddText(strID.toLocal8Bit().data(),nAngle,28,19,size,0);
//    AddText(strCard.toLocal8Bit().data(),nAngle,28,23.5,size,0);
//    AddText(strDate.toLocal8Bit().data(),nAngle,28,28,size,0);

//    Mat canvas(nCardHeight,nCardWidth,CV_8UC3,Scalar(255,255,255));

//    int nDarkLeft = 0,nDarkTop = 0,nDarkRight = 0,nDarkBottom = 0;

//    for (auto var : m_textInfo)
//    {
//        //RunlogF("Text = %s,FontSize = %d,xPos = %.02f,yPos = %.2f.\n",var->sText.c_str(),var->nFontSize);
//        Rect rtROI(MM2Pixel(var->fxPos), MM2Pixel(var->fyPos),nCardWidth - MM2Pixel(var->fxPos) - 2, nCardHeight - MM2Pixel(var->fyPos) - 2);
//        if (!nDarkLeft)
//            nDarkLeft = rtROI.x;
//        else
//            nDarkLeft = nDarkLeft<=rtROI.x?nDarkLeft:rtROI.x;
//        if (!nDarkTop)
//            nDarkTop = rtROI.y;
//        else
//            nDarkTop = nDarkTop<=rtROI.y?nDarkTop:rtROI.y;
//        if (!nDarkRight)
//            nDarkRight = rtROI.x + rtROI.width;
//        else
//            nDarkRight = nDarkRight>=(rtROI.x + rtROI.width)?nDarkRight:(rtROI.x + rtROI.width);
//        if (!nDarkBottom)
//            nDarkBottom = rtROI.y + rtROI.height;
//        else
//            nDarkBottom = nDarkBottom>=(rtROI.y + rtROI.height)?nDarkBottom:(rtROI.y + rtROI.height);

//        Mat FontROI = canvas(rtROI);
//        int fontHeight = (int)round(MM2Pixel(Pt2MM(var->nFontSize)));
//        RunlogF("Text = %s,FontSize = %.2f, FontPixel = %d.\n",var->sText.c_str(),var->nFontSize,fontHeight);
//        try
//        {
//            cv::Ptr<cv::freetype::FreeType2> ft2;
//            ft2 = cv::freetype::createFreeType2();

//            ft2->loadFontData(var->pFontName, 0); //加载字库文件
//            ft2->putText(FontROI, var->sText.c_str(), cv::Point(0, 0), fontHeight, CV_RGB(0, 0, 0), -1, CV_AA, false/*,var->nFontStyle == 2*/);
//        }
//        catch (std::exception &e)
//        {
//            RunlogF("Catch a exception:%s.\n",e.what());
//            return ;
//        }
//    }

//    if (strPreviewFile.size())
//        imwrite(strPreviewFile.c_str(),canvas);
//    QString strTempFile = QDir::currentPath();
//    strTempFile += "/PrintTemp.bmp";

//    flip(canvas, canvas, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
//    flip(canvas, canvas, 1);

//    imwrite(strTempFile.toUtf8().data(),canvas);
//    strOverlayer = QDir::currentPath().toStdString();
//    strOverlayer += ":/iso.bmp";
//    QFileInfo fi(strOverlayer.c_str());
//    if (!fi.isFile())
//    {
//        RunlogF("Can't open file %s.\n",strOverlayer.c_str());
//        return ;
//    }

//}
//void QEvolisPrinter::AddText(char *szText,int nAngle, float fxPos, float fyPos, int nFontSize, int nColor)
//{
//    TextInfoPtr inTextInfo = std::make_shared<TextInfo>();
//    char szUtf8[256] = {0};

//    inTextInfo->sText = szText;
//    inTextInfo->nAngle = nAngle;
//    inTextInfo->fxPos = fxPos;
//    inTextInfo->fyPos = fyPos;
//    inTextInfo->pFontName = ":/Sumsun.ttf";
//    inTextInfo->nFontSize = nFontSize;
//    inTextInfo->nFontStyle = 1;
//    inTextInfo->nColor = nColor;
//    inTextInfo->nType = 0;  //文字是0
//    m_textInfo.push_back(inTextInfo);
//}
