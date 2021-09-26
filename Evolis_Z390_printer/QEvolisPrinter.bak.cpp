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
//#include <opencv4/opencv2/opencv.hpp>
//#include <opencv4/opencv2/freetype.hpp>
//#include <opencv4/opencv2/imgproc/imgproc.hpp>
//#include <opencv4/opencv2/imgcodecs.hpp>
//#include <opencv4/opencv2/imgproc/imgproc_c.h>
//using namespace  cv;

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

// AES key
unsigned char g_szAesKey[] =  {0x9d,0x06,0x93,0xce,0x80,0x8e,0x37,0xeb,0xd7,0xee,0x09,0x56,0xb5,0xae,0xb3,0xb0};
// 区码F616的密文
unsigned char g_szZoneKey1[16] =  {0x47,0x66,0xf2,0x21,0x79,0x07,0x4f,0xb7,0x28,0x3f,0x6f,0x3a,0x6d,0x17,0xa6,0xc0};
// 区码F61A的密文
unsigned char g_szZoneKey2[16] =  {0x99,0xf4,0xb4,0x85,0xb1,0x10,0xdf,0x75,0x1c,0xec,0xff,0xc8,0xa8,0x15,0xf7,0x38};



#define RunlogF(...)    Runlog(__PRETTY_FUNCTION__,__LINE__,__VA_ARGS__);
#define Funclog()       FuncRunlog(__PRETTY_FUNCTION__,__LINE__);  TraceFunction TF(__PRETTY_FUNCTION__,__LINE__);//auto threadID = std::this_thread::get_id();RunlogF("####Current ThreadID:%08x####\n",threadID);

#define     CheckHandle()       if (!pHandle)    \
                                {\
                                    RunlogF("libevolis.so is not loaded!\n");\
                                    strcpy(pszRcCode, "0001");\
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
                                    RunlogF("Printer connection is invalid.\n");\
                                    strcpy(pszRcCode, "0001");\
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
    qDebug()<<strInfo.c_str();
    if (pFilelog)
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
    qDebug()<<strInfo.c_str();
    if (pFilelog)
    {
        strInfo += "\n";
        pFilelog->write(strInfo.c_str());
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
    pevolis_command          = GetProcAddr(pHandle, evolis_command         );
    pevolis_set_card_pos     = GetProcAddr(pHandle, evolis_set_card_pos    );
    pevolis_print_init       = GetProcAddr(pHandle, evolis_print_init      );
    pevolis_reset            = GetProcAddr(pHandle, evolis_reset     );
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
    char szLog[128] = {0};
    QString strCurDir = QDir::currentPath();

    strOverlayer = strCurDir.toStdString() + "/resources/iso.bmp";
    //PrintText();

}


QEvolisPrinter::~QEvolisPrinter()
{
    Funclog();
}

void QEvolisPrinter::Exit()
{
    Funclog();
    bRunning = false;
    cvTask.notify_one();
}

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


int  QEvolisPrinter::On_Print_Open(char *pPort, char *pPortParam, char *pszRcCode)
{
    Funclog();
    evolis_device_t* devices;
    RunlogF("Try to evolis_get_devices.\n");
    if (!pHandle)
    {
        RunlogF("libevolis.so is not loaded.\n");
        strcpy(pszRcCode, "0001");
        return 1;
    }
    int n = 0;
    if ((n = pevolis_get_devices(&devices, 0, 0)) >= 0)
    {
        qDebug("%d printers found\r\n", n);
        for (int i = 0; i < n; ++i)
        {
            char name[64];

            qDebug("- %s\r\n", tocstring(name, devices[i].name));
            qDebug("  Id: %s\r\n", devices[i].id);
            qDebug("  Uri: %s\r\n", devices[i].uri);
            qDebug("  Mark: %s\r\n", pevolis_get_mark_name(devices[i].mark));
            qDebug("  Model: %s\r\n", pevolis_get_model_name(devices[i].model));
            qDebug("  isSupervised: %s\r\n", (devices[i].isSupervised) ? "true" : "false");
            qDebug("  link: %s\r\n",
                    (devices[i].link == EVOLIS_LI_TCP) ? "TCP" :
                    (devices[i].link == EVOLIS_LI_USB) ? "USB" : "?");
        }
        RunlogF("evolis_get_devices Succeed.\n");
    }

    RunlogF("Try to call evolis_open.\n");
    m_pPrinter = pevolis_open2("usb:///dev/usb/lp0", EVOLIS_TY_EVOLIS);
    if (!m_pPrinter)
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }

    char szReply[128] = {0};
    const char *szAdapt[] = {
                            "Rzp",  // 读打印机区码
                            "Rrt;zone"  // 读色带区码
                            };
    const char *szDev[] = {"Printer","Ribbon"};
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
    // 色带区码标签 1046==F61A,98C=F616
    for(auto var:szAdapt)
    {
        //RunlogF("Try to adapt %s.\n",szDev[nIndex]);
        if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in adapting %s.\n",szDev[nIndex]);
            strcpy(pszRcCode, "0001");
            return 1;
        }
        //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",var,szReply);
        string strReply = szReply;
        std::transform(strReply.begin(),strReply.end(),strReply.begin(),::toupper);
        if (strReply != (const char *)szZone1  &&
            strReply != (const char *)szZone2)
        {
            RunlogF("The %s it not compatible with Z390!\n",szDev[nIndex]);
            evolis_close(m_pPrinter);
            m_pPrinter = nullptr;
            return 1;
        }
        //RunlogF("Succeed in adapting %s.\n",szDev[nIndex ++]);
   }
   const char *szCmd[]={
                        "Psmgr;2",     // 防止进卡和出卡阻塞
                        "Pcim;F",      // 从卡箱进卡
                        "Pcem;M",      // 从出卡口出卡
                        "Pneab;E"      // 打印结束后不出卡
                        "Pbc;A;D"
                       };
    //RunlogF("evolis_command(Rzp) Succeed,Reply:%s.\n",szReply);
    for (auto var:szCmd)
    {
        //RunlogF("Try to evolis_command(%s).\n",var);
        if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",var);
            strcpy(pszRcCode, "0001");
            return 1;
        }
        //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",var,szReply);
    }

    //RunlogF("evolis_open Succeed.\n");
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_Close(char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

   // RunlogF("Try to evolis_close.\n");
    pevolis_close(m_pPrinter);
    //RunlogF("evolis_close Succeed.\n");
    m_pPrinter = nullptr;

    strcpy(pszRcCode, "0000");
    return 0;
}

// nResetAction 1-复位卡保持在设备内；2-复位退卡到打印机取卡位置；3-复位吞卡到打印机回收箱(若硬件支持需实现)
int  QEvolisPrinter::On_Print_Reset(long lTimeout, int nResetAction, char *pszRcCode)
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
        RunlogF("Failed in evolis_reset timeouted(),Wait for device:%d.\n",szTimeouted,nResetDelay);
        this_thread::sleep_for(chrono::milliseconds(nResetDelay));
    }
    else
        RunlogF("Skip device reset.\n");

    RunlogF("Try to prepare device.\n");
    const char *szCmd[]={
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
             strcpy(pszRcCode, "0001");
             return 1;
         }
     }
    RunlogF("Prepare device succeed,Try to CheckCardPostion.\n");
    int nCardPos = 0;

    if (CheckCardPostion(&nCardPos))
    {// nCardPos 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位
        strcpy(pszRcCode, "0001");
        return 1;
    }

    strcpy(pszRcCode, "0000");
    switch (nResetAction)
    {
    default:
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }
    case 1:     // 复位卡保持在设备内
    {
        if (nCardPos == 1)     // 卡在出口卡
        {
           return InDraftCard(lTimeout,pszRcCode);
        }
        break;
    }
    case 2:     // 复位退卡到打印机取卡位置
    {
        switch (nCardPos)
        {
        case 0:
            break;
        case 1:  //卡在出口卡,insert the card and then eject it
        {
            const char* szCommand[]={ "Sib","Seb"};
            int Res = 0;

            Res = pevolis_command(m_pPrinter, szCommand[0],strlen(szCommand[0]), szReply,sizeof(szReply));
            if (Res < 0)
            {
                RunlogF("evolis_command(%s)failed.\n",szCommand[0]);
                strcpy(pszRcCode, "0002");
                return 1;
            }
            if (strstr(szReply,"ERROR"))
            {
                int nCardPos = 0;
                if (CheckCardPostion(&nCardPos))
                {
                    RunlogF("evolis_command(%s)failed.\n",szCommand[0]);
                    strcpy(pszRcCode, "0002");
                    return 1;
                }
                if (nCardPos == 1)
                {
                    strcpy(pszRcCode, "0016");
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

            this_thread::sleep_for(chrono::milliseconds(500));
            Res = pevolis_command(m_pPrinter, szCommand[1],strlen(szCommand[1]), szReply,sizeof(szReply));
            if (Res < 0)
            {
                RunlogF("evolis_command(%s)failed.\n",szCommand[1]);
                strcpy(pszRcCode, "0002");
                return 1;
            }

            break;
        }
        case 2:
        case 3:
        default:
        {
            auto var = "Seb";
            if (pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply)) < 0)
            {
                strcpy(pszRcCode, "0002");
                RunlogF("Failed in evolis_command(%s).\n",var);
                return 1;
            }
        }
        }
        break;
    }
    case 3:     // 复位吞卡到打印机回收箱
    {
        switch (nCardPos)
        {
        case 0:
             break;
        case 1:     //卡在出口卡,insert the card and then eject it
        {
            const char* szCommand[] = {"Ss",
                                       "Sib",
                                       "Pneab;A;1",
                                       "Pcrm;D",    // 设定出卡箱为废卡箱
                                       "Ser" };     // 设置为废卡

            //const char* szCommand[]={ "Sib", "Ser" };
            int Res = 0;
            for (auto var : szCommand)
            {
                Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
                if (Res < 0)
                {
                    RunlogF("evolis_command(%s)failed.\n",var);
                    strcpy(pszRcCode, "0002");
                    return 1;
                }
                RunlogF("evolis_command(%s):%s.\n",var,szReply);
                if (strcmp(var,"Sib") == 0 &&  strstr(szReply,"ERROR"))
                {
                    int nCardPos = 0;
                    if (CheckCardPostion(&nCardPos))
                    {
                        RunlogF("evolis_command(%s)failed.\n",var);
                        strcpy(pszRcCode, "0002");
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
                     this_thread::sleep_for(chrono::milliseconds(200));
                }

            }
            break;
        }
        case 2:
        case 3:
        default:
        {
            // 吞卡到废卡箱
//            auto var = "Ser";
//            if(pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply)) < 0)
//            {
//                RunlogF("evolis_command(%s)failed.\n",var);
//                strcpy(pszRcCode, "0002");
//                return 1;
//            }
            const char* szCommand[] = {"Ss",
                                       "Pneab;A;1",
                                       "Pcrm;D",    // 设定出卡箱为废卡箱
                                       "Ser" };     // 设置为废卡

            int Res = 0;
            for (auto var : szCommand)
            {
                Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
                if (Res < 0)
                {
                    RunlogF("evolis_command(%s)failed.\n",var);
                    strcpy(pszRcCode, "0002");
                    return 1;
                }
            }
            break;
        }
        }
        break;
    }
    }

    bFault = false;
    strcpy(pszRcCode, "0000");
    return 0;

}

int  QEvolisPrinter::On_Print_Eject(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char szReply[16] = {0};

    int nCardPos = 0;
    if (CheckCardPostion(&nCardPos))
    {// 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
        strcpy(pszRcCode, "0001");
        return 1;
    }
    switch (nCardPos)
    {
    case 0:
        return 0;
    case 1:  //卡在出口卡,insert the card and then eject it
    {
        const char* szCommand[]={"Psmgr;2", "Sib","Seb"};
        int Res = 0;
        for (auto var : szCommand)
        {
            Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
            if (Res < 0)
            {
                RunlogF("evolis_command(%s)failed.\n",var);
                strcpy(pszRcCode, "0002");
                bFault = true;
                return 1;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        return 0;
    }
    case 2:
    case 3:
    default:
    {
        const char* szCommand[]={"Psmgr;2", "Seb"};

        int Res = 0;
        for (auto var : szCommand)
        {
            Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
            if (Res < 0)
            {
                RunlogF("evolis_command(%s)failed.\n",var);
                bFault = true;
                strcpy(pszRcCode, "0002");
                return 1;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
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
//            strcpy(pszRcCode, "0001");
//            RunlogF("Failed in evolis_command(%s).\n",var);
//            return 1;
//        }
//    }

    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    char szReply[32] = {0};
    char *szCmd = "Rlr;p";

    if (pevolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd);
        bFault = true;
        return 1;
    }

    if (strcmp(szReply,"OK")  == 0)
    {
        RunlogF("There is no card in printer.\n");
        strcpy(pszRcCode, "0001");
        return 1;
    }

    const char* szCommand[] = {"Ss",
                               "Pneab;A;1",
                               "Pcrm;D",    // 设定出卡箱为废卡箱
                               "Ser" };     // 设置为废卡

    int Res = 0;
    for (auto var : szCommand)
    {
        Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
        if (Res < 0)
        {
            RunlogF("evolis_command(%s)failed.\n",var);
            strcpy(pszRcCode, "0002");
            bFault = true;
            return 1;
        }
    }
    strcpy(pszRcCode, "0000");
    return  0;
}


int  QEvolisPrinter::On_Print_EnableCard(long lTimeout, int nCheckable, char *pszRcCode)
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
        //RunlogF("Try to evolis_command(%s).\n",var);
        Res = pevolis_command(m_pPrinter, var,strlen(var), RespValue,1024);
        if (Res < 0)
        {
            RunlogF("evolis_command(%s)failed.\n",var);
            strcpy(pszRcCode, "0002");
            bFault = true;
            return 1;
        }
        //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",var,RespValue);
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
            this_thread::sleep_for(chrono::milliseconds(500));
            nResult = On_Print_Status(lTimeout,lpStatus,pszRcCode);
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

        On_Print_DisableCard(lTimeout,pszRcCode);
    }

    strcpy(pszRcCode, "0000");
    return  0;
}

int  QEvolisPrinter::On_Print_EnableCard2(long lTimeout, int nCheckable, char *pszRcCode)
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
            strcpy(pszRcCode, "0002");
            bFault = true;
            return 1;
        }
    }
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

int  QEvolisPrinter::InDraftCard(long lTimeout,char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    char szReply[32] = {0};

    char* szCommand[] = {"Sib","Sis"};
    for (auto var:szCommand)
    {
      if (pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply))< 0)
      {
          RunlogF("evolis_command(%s)failed.\n",var);
          strcpy(pszRcCode, "0002");
          bFault = true;
          return 1;
      }

      if (strstr(szReply,"ERROR"))
      {
          int nCardPos = 0;
          if (CheckCardPostion(&nCardPos))
          {
              RunlogF("evolis_command(%s)failed.\n",var);
              strcpy(pszRcCode, "0002");
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

    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_DisableCard(long lTimeout, char *pszRcCode)
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
         //RunlogF("Try to evolis_command(%s).\n",var);
         if (pevolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
         {
             RunlogF("Failed in evolis_command(%s).\n",var);
             strcpy(pszRcCode, "0001");
             bFault = true;
             return 1;
         }
         //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",var,szReply);
     }

    strcpy(pszRcCode, "0000");
    return  0;
}


int  QEvolisPrinter::On_Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    int nCardPos = 0;
    if (CheckCardPostion(&nCardPos))
    {// nCardPos 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位
        strcpy(pszRcCode, "0001");
        return 1;
    }
    if (nCardPos == 1)
    {// only test if the card is at bezel
        strcpy(pszRcCode, "0015");
        return 1;
    }
    int nRes = pevolis_set_card_pos(m_pPrinter,EVOLIS_CP_INSERT);
    if (nRes)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_INSERT).\n");
        bFault = true;
        return 1;
    }

    this_thread::sleep_for(std::chrono::milliseconds(200));
    nRes = pevolis_set_card_pos(m_pPrinter,EVOLIS_CP_CONTACT);
    if (nRes)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_CONTACT).\n");
        bFault = true;
        return 1;
    }
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    const char *szCmd="Rse;f";
    char szReply1[16] = {0};
    // chechk the card box status
    //RunlogF("Try to evolis_command(%s).\n",szCmd);
    //卡箱无卡时：evolis_command(Rse;f) 返回:0,00 Volts
    //卡箱有卡时：evolis_command(Rse;f) 返回:5,00 Volts，这个可以对上了
    if (pevolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply1,sizeof(szReply1)) < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szCmd,szReply1);
        strcpy(pszRcCode, "0001");
        bFault = true;
        return 1;
    }

    szCmd="Rse;n";
    char szReply2[16] = {0};
    // chechk the card box status
    //RunlogF("Try to evolis_command(%s).\n",szCmd);
    //卡箱无卡时：evolis_command(Rse;f) 返回:0,00 Volts
    //卡箱有卡时：evolis_command(Rse;f) 返回:5,00 Volts，这个可以对上了
    if (pevolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply2,sizeof(szReply2)) < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szCmd,szReply2);
        strcpy(pszRcCode, "0001");
        bFault = true;
        return 1;
    }

    CardBoxInfo[0].byType = 1;           //卡箱类型 0:未知，1：发卡箱，2：回收箱
    CardBoxInfo[0].byBoxNumber = 1;      //卡箱号
    CardBoxInfo[0].bHardwareSensor = false;//是否支持故障状态传感器,TRUE:支持，FALSE：不支持
    if (szReply1[0] == '5')
        CardBoxInfo[0].byStatus = 0;     //卡箱状态,0：正常; 1：卡少; 2:无卡; 3：不可操作; 4：不存在; 5：高(快满)；6：满； 7：未知
    else
        CardBoxInfo[0].byStatus = 2;

    if (szReply2[0] > '0')
        CardBoxInfo[0].byStatus = 1;

    CardBoxInfo[1].byType = 2;
    CardBoxInfo[1].byBoxNumber = 2;
    CardBoxInfo[1].byStatus = 0;
    CardBoxInfo[1].bHardwareSensor = false;

    char majorStatusValue[1024] = {0};
    char minorStatusValue[1024] = {0};
    if (GetDeviceState(m_pPrinter, majorStatusValue, minorStatusValue))
    {
        RunlogF("Failed in GetDeviceState.\n");
        strcpy(pszRcCode, "0002");
        return 1;
    }

    RunlogF("majorStatusValue = %s.\n",majorStatusValue);
    RunlogF("minorStatusValue = %s.\n",minorStatusValue);

    if(strstr(minorStatusValue,"FEEDER_EMPTY"))
    {
        CardBoxInfo[0].byStatus = 2;
    }

    lpBoxInfo->nCount = 2;
    lpBoxInfo->lpplist = CardBoxInfo;
    strcpy(pszRcCode, "0000");

    return 0;
}

//WORD fwDevice;		//打印机状态,    0-在线；  1-忙；           2-不在线；      3-故障
//WORD fwMedia;			//介质状态，     0-无卡；  1-卡在门口；      2-卡在内部；    3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
//WORD fwToner;			//平印色带状态,  0-FLLL;  1-LOW;2-OUT;    3-NOTSUPP;    4-UNKNOW
int  QEvolisPrinter::On_Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode)
{
    Q_UNUSED(lTimeout);
    Funclog();
    CheckPrinter(m_pPrinter);

    int RibbonNum = 0;
    int fwDevice = 0;
    int fwMedia = 0;

    if (0 == GetPrinterStatus(m_pPrinter,&RibbonNum,&fwDevice,&fwMedia))
    {
        if (RibbonNum == -98)
            lpStatus->fwToner = 4;
        else
        {
            if (RibbonNum <=20 )
                lpStatus->fwToner = 0;
            else
                lpStatus->fwToner = 1;
        }

        lpStatus->fwDevice = fwDevice;
        lpStatus->fwMedia = fwMedia;
        strcpy(pszRcCode, "0000");
        return 0;
    }
    else
    {
        lpStatus->fwDevice = 2;
        strcpy(pszRcCode, "0000");
        return 0;
    }
}

int  QEvolisPrinter::On_Print_InitPrint(long lTimeout, char *pszRcCode)
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
        strcpy(pszRcCode, "0002");
        return 1;
    }

    this_thread::sleep_for(chrono::milliseconds(10));//1毫秒

    if (pevolis_print_init(m_pPrinter))
    {
        RunlogF("evolis_print_init Failed.\n");
        strcpy(pszRcCode, "0002");
        return 1;
    }
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
    char szReply[128] = { 0 };
    int nRes = 1;

    //RunlogF("Try to evolis_command(%s).\n",pCommand);
    nRes = pevolis_command(m_pPrinter,pCommand, strlen(pCommand), szReply, 128);
    if (nRes < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",pCommand);

        return 1;
    }
    return 0;

}
int  QEvolisPrinter::On_Print_StartPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    if (!m_textInfo.size())
        RunlogF("There is no Text info.\n");


    if (m_textInfo.size() <= 0 && m_picInfo.picpath.size() <= 0)
    {
        RunlogF("There is no Image or Text info.\n");
        strcpy(pszRcCode, "0001");
        return 1;
    }

    //int ec = Cv_PrintCard(m_picInfo, m_textInfo,lTimeout);
    int ec = Qt_PrintCard(m_picInfo, m_textInfo,lTimeout);
    if (ec)
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }
    else
    {
        if (MoveCardContact() == 0)
        {
            strcpy(pszRcCode, "0000");
            return  0;
        }
        else
        {
            strcpy(pszRcCode, "0001");
            return  1;
        }
    }
}

int QEvolisPrinter::GetDeviceState(void  *pPrinter, char* majorStatusValue, char* minorStatusValue)
{
    Funclog();
    if (!pHandle)
    {
        RunlogF("libevolis.so is not loaded!\n");\
        return 1;
    }

    evolis_major_state_t major;
    evolis_minor_state_t minor;
    if (pevolis_get_state(pPrinter, &major, &minor))
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
int QEvolisPrinter::CheckCardPostion(int *Media)
{
    Funclog();
    CheckHandle2();
    const char *szCmd[]={"Rlr;p",      // 查询打印内部
                         "Rse;b"};      // 查询出卡口
    char szReply[16] = {0};

    int nIdx = 0;
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,16) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        return 1;
    }

    char szReply2[16] = {0};
    nIdx ++;
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply2,16) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        return 1;
    }
    //Media 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
    if (strcmp(szReply,"OK") == 0)
    {
        // 0,2x Volts是没卡
        // 4,9x Volts是有卡
        szReply2[1] = '.';
        float fVolt = atof(szReply2);
        RunlogF("%s Status:%.2f.\n",szCmd[nIdx],fVolt);
        if (fVolt >= 1.0)
            *Media = 1;
        else
            *Media = 0;
    }
    else if (strcmp(szReply,"CARD") == 0||
             strcmp(szReply,"CONTACTLESS CARD"))
        *Media = 2;
    else if (strcmp(szReply,"SMART CARD") == 0)
        *Media = 3;
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
int QEvolisPrinter::GetPrinterStatus(void  *pPrinter,int * RibbonNum,int *Device, int *Media)
{
    Funclog();
    CheckHandle2();

    *Media = 0;        // No card

    DWORD  ec;
    evolis_ribbon_t ribbon;
    /// Get ribbon infos.
    //RunlogF("Try to evolis_get_ribbon.\n");
    ec = pevolis_get_ribbon(pPrinter, &ribbon);
    if (ec != 0)
    {
        RunlogF("Failed in evolis_get_ribbon.\n");
        return 1;
    }

    *RibbonNum= ribbon.capacity - ribbon.remaining;
    // 无卡        majorStatusValue = READY         minorStatusValue = DEVICE_READY
    // 卡在机器内部  majorStatusValue = WARNING       minorStatusValue = DEVICE_BUSY
    // 卡在出卡口    majorStatusValue = WARNING       minorStatusValue = STDEVOLIS_DEF_CARD_ON_EJECT
    //             majorStatusValue = WARNING       minorStatusValue = EVOLIS_MI_STDEVOLIS_DEF_COVER_OPEN
    char majorStatusValue[128] = {0};
    char minorStatusValue[128] = {0};
    if (GetDeviceState(m_pPrinter,majorStatusValue,minorStatusValue))
    {
        RunlogF("Failed in GetDeviceState.\n");
        return 1;
    }

    if (strcmp(minorStatusValue, "DEF_UNSUPPORTED_RIBBON") == 0)
        *RibbonNum = -98;

    if(strcmp(minorStatusValue,"PRINTER_OFFLINE") == 0)
        *Device = 2;//是否离线

    if (CheckCardPostion(Media))
    {
        RunlogF("Failed in CheckCardPostion.\n");
        return 1;
    }

    evolis_status_t es;
    //RunlogF("Try to evolis_status.\n");
    if (pevolis_status(pPrinter, &es) != 0)
    {
        RunlogF("Failed evolis_status.\n");
        *Device = 2;    // 离线
    }
    else
    {
        if (bFault)
            *Device = 3;// fault
        else
            *Device = 0;    // 在线
        RunlogF("evolis_status return:config = %d\tinformation = %d\twarning = %d\terror = %d.\n", es.config,es.information, es.warning,es.error);
        for (int i = 0;i < EVOLIS_STATUS_EX_COUNT;i ++)
            RunlogF("evolis_status return exts[%d] = %d.\n",i, es.exts[i]);
    }

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
         return 1;
    }

    pevolis_print_set_option(m_pPrinter, "IFTextRegion", "0x0x1000x600");

    if (pevolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "40"))
    {
         RunlogF("Failed in evolis_print_set_option(IFBlackLevelValue,40).\n");
         return 1;
    }

}
#include <QHash>
bool loadFontFamilyFromFiles(const QString &strFontFileName,QString &strFontFamily)
{
    static QHash<QString, QString> FontTable;
    if (FontTable.contains(strFontFileName))
    {
        strFontFamily = FontTable.value(strFontFileName);
        return true;
    }
    QFileInfo fi(strFontFileName);
    if (!fi.isFile())
        return false;

    int nFontID = QFontDatabase::addApplicationFont(strFontFileName);
    if (nFontID == -1)
        return false;

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(nFontID);
    if(loadedFontFamilies.empty())
        return false;

    strFontFamily = loadedFontFamilies.at(0);
    FontTable.insert(strFontFileName,strFontFamily);
    return true;
}

#define LOBYTE(w)         ((unsigned char)(((ULONG)(w)) & 0xff))
#define Value_R(rgb)      (LOBYTE(rgb))
#define Value_G(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define Value_B(rgb)      (LOBYTE((rgb)>>16))

int QEvolisPrinter::Qt_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout)
{
    char pszRcCode[1024] = {0};
    CheckPrinter(m_pPrinter);
    if (evolis_print_init((evolis_t*)m_pPrinter))
    {
        RunlogF("evolis_print_init failed.\n");
        return 1;
    }

    const char *szCmd[]={"Rrt;count","Pneab;A;0","Pneab;A;1"};
    char szReply[128] = {0};
    int nIdx = 0;
    RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        return 1;
    }
    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
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
        RunlogF("Failed in evolis_command(%s):%s.\n",szCmd[nIdx]);
        return 1;
    }
    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
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

    for (auto var : inTextVector)
    {
        QString imageText = QString::fromStdString(var->sText);
        QString strFontFamily;
        if (!loadFontFamilyFromFiles(var->pFontName.c_str(),strFontFamily))
        {
            RunlogF("Failed in load font %s.\n",var->pFontName.c_str());
            continue;
        }

        QFont font(strFontFamily);
        //设置显示字体的大小
        font.setPointSize(var->nFontSize);
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
        painter.drawText(QRectF(MM2Pixel(var->fxPos), MM2Pixel(var->fxPos), MM2Pixel(TextRect.width()), MM2Pixel(TextRect.height())), Qt::AlignLeft | Qt::AlignTop, imageText);
        painter.restore();
    }
    Convas.save("./Preview.bmp");

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
    QFileInfo fi(strOverlayer.c_str());
    if (pevolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
        strcpy(pszRcCode, "0001");
        return 1;
    }

    if (pevolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
        strcpy(pszRcCode, "0001");
        return 1;
    }

    if (!fi.isFile())
    {
        RunlogF("Can't find file %s.\n",strOverlayer.c_str());
        strcpy(pszRcCode, "0001");
        return 1;
    }

    if (pevolis_print_set_imageb(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT, pFileBuffer,nBufferSize))
    {
        RunlogF("evolis_print_set_imageb failed:%s.\n",inPicInfo.picpath.c_str());
        return 1;
    }

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
    }
    return 0;
}

//Mat matRotateClockWise90(Mat src)
//{
//    if (src.empty())
//    {
//        qDebug()<<"RorateMat src is empty!";
//    }
//    // 矩阵转置
//    transpose(src, src);
//    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
//    flip(src, src, 1);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
//    return src;
//}

//Mat matRotateClockWise180(Mat src)//顺时针180
//{
//    if (src.empty())
//    {
//        qDebug() << "RorateMat src is empty!";
//    }

//    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
//    flip(src, src, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
//    flip(src, src, 1);
//    return src;
//    //transpose(src, src);// 矩阵转置
//}

//Mat matRotateClockWise270(Mat src)//顺时针270
//{
//    if (src.empty())
//    {
//        qDebug() << "RorateMat src is empty!";
//    }
//    // 矩阵转置
//    //transpose(src, src);
//    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
//    transpose(src, src);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
//    flip(src, src, 0);
//    return src;
//}

//Mat myRotateAntiClockWise90(Mat src)//逆时针90°
//{
//    if (src.empty())
//    {
//      qDebug()<<"mat is empty!";
//    }
//    transpose(src, src);
//    flip(src, src, 0);
//}

//int QEvolisPrinter::Cv_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout)
//{
//    Funclog();
//    CheckHandle2();
//    if (!m_pPrinter)
//        return 1;

//    QFileInfo fipic(inPicInfo.picpath.c_str());
//    if (!fipic.isFile() && !inTextVector.size())
//    {
//        RunlogF("There is no any content to print!\n");
//        return 1;
//    }

//// 此处待完善
////    evolis_status_t es;
////    if (evolis_status(m_pPrinter, &es) != 0)
////    {
////        RunlogF("Error reading printer status\n");
////        return 0;
////    }

//    //RunlogF("Try to evolis_print_init.\n");
//    if (pevolis_print_init((evolis_t*)m_pPrinter))
//    {
//        RunlogF("evolis_print_init failed.\n");
//        return 1;
//    }
//    if (strFColorBrightness.size())
//    {
//        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"FColorBrightness",strFColorBrightness.c_str()))
//        {
//             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorBrightness",strFColorBrightness.c_str());
//        }
//    }

//    if (strBColorBrightness.size())
//    {
//        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"BColorBrightness",strBColorBrightness.c_str()))
//        {
//             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorBrightness",strBColorBrightness.c_str());
//        }
//    }

//    if (strFColorContrast.size())
//    {
//        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"FColorContrast",strFColorContrast.c_str()))
//        {
//             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorContrast",strFColorContrast.c_str());
//        }
//    }

//    if (strBColorContrast.size())
//    {
//        if (!pevolis_print_set_option((evolis_t*)m_pPrinter,"BColorContrast",strBColorContrast.c_str()))
//        {
//             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorContrast",strBColorContrast.c_str());
//        }
//    }

//    const char *szCmd[]={"Rrt;count","Pneab;A;0","Pneab;A;1"};
//    char szReply[128] = {0};
//    int nIdx = 0;
//    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
//    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
//    {
//        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
//        return 1;
//    }
//    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
//    int nRibbonCount = strtol(szReply,nullptr,10);
//    if (nRibbonCount <= 0)
//    {
//        RunlogF("The ribbon is exhausted and printing failed.\n");
//        return 1;
//    }
//    nIdx++;
//    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
//    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
//    {
//        RunlogF("Failed in evolis_command(%s):%s.\n",szCmd[nIdx]);
//        return 1;
//    }
//    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
//    nIdx++;

//    Mat canvas(nCardHeight,nCardWidth,CV_8UC3,Scalar(255,255,255));

//    //RunlogF("Try to load file %s.\n",inPicInfo.picpath.c_str());

//    Mat HeaderImage = imread(inPicInfo.picpath, IMREAD_ANYCOLOR);
//    if (!HeaderImage.data)
//    {
//         RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
//         return 1;
//    }
//    Mat HeaderPrint;
//    resize(HeaderImage, HeaderPrint, Size(MM2Pixel(inPicInfo.fWidth) , MM2Pixel(inPicInfo.fHeight)), 0, 0, INTER_NEAREST);

//    Mat HeaderROI = canvas(Rect(MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),MM2Pixel(inPicInfo.fWidth),MM2Pixel(inPicInfo.fHeight)));
//    HeaderPrint.copyTo(HeaderROI);

//    int nDarkLeft = 0,nDarkTop = 0,nDarkRight = 0,nDarkBottom = 0;

//    for (auto var : inTextVector)
//    {
//        RunlogF("Text = %s,FontSize = %d,xPos = %.02f,yPos = %.2f.\n",var->sText.c_str(),var->nFontSize);
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
//            return 1;
//        }
//    }

//    if (strPreviewFile.size())
//        imwrite(strPreviewFile.c_str(),canvas);
//    QString strTempFile = QDir::currentPath();
//    strTempFile += "/PrintTemp.bmp";
//    if (inPicInfo.nAngle > 0)
//    {
//        //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
//        flip(canvas, canvas, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
//        flip(canvas, canvas, 1);
//    }
//    imwrite(strTempFile.toUtf8().data(),canvas);
//    strOverlayer = QDir::currentPath().toStdString();
//    strOverlayer += "/resources/iso.bmp";
//    QFileInfo fi(strOverlayer.c_str());
//    if (!fi.isFile())
//    {
//        RunlogF("Can't open file %s.\n",strOverlayer.c_str());
//        return 1;
//    }
//    if (pevolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
//    {
//        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
//        return 1;
//    }

//    if (strGRibbonType.size())
//    {
//        if (pevolis_print_set_option(m_pPrinter, "GRibbonType", strGRibbonType.c_str()))
//        {
//            RunlogF("Failed in evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
//            return 1;
//        }
//    }

//    if (pevolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
//    {
//        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
//        return 1;
//    }

//    pevolis_print_set_option(m_pPrinter, "FBlackManagement", "TEXTINBLACK");

//    char szDarkZone[32] = {0};
//    if (inPicInfo.nAngle > 0)
//        sprintf(szDarkZone,"%dx%dx%dx%d",0,0,nCardWidth - nDarkLeft,nCardHeight - nDarkTop);
//    else
//        sprintf(szDarkZone,"%dx%dx%dx%d",nDarkLeft,nDarkTop,nDarkRight,nDarkBottom);

//    pevolis_print_set_option(m_pPrinter, "IFTextRegion", szDarkZone);

//    pevolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "50");

//    pevolis_print_set_option(m_pPrinter, "IFDarkLevelValue ", "30");

//    if (pevolis_print_set_imagep(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT,strTempFile.toUtf8().data()))
//    {
//        RunlogF("evolis_print_set_imagep failed:%s.\n",strTempFile.toUtf8().data());
//        return 1;
//    }

//    RunlogF("Try to evolis_print_exec.\n");
//    pevolis_print_exec((evolis_t*)m_pPrinter);
//    int             printed = 0;
//    evolis_status_t es;
//    auto tNow = system_clock::now();
//    auto tLast = tNow;
//    while (!printed)
//    {
//        if (pevolis_status(m_pPrinter, &es) != 0)
//        {
//            RunlogF("Error reading printer status\n");
//            return 0;
//        }
//        printed = !(es.information & EVOLIS_INF_BUSY);
//        tNow = system_clock::now();
//        auto tDeration = duration_cast<milliseconds>(tNow - tLast);
//        if (tDeration.count() > nTimeout*1000)
//        {
//            tLast = tNow;
//            RunlogF("Waiting printer task stop\n");
//            break;
//        }
//    }
//    return 0;
//}

int  QEvolisPrinter::SendCommand(const char *lpCmdIn,char *lpCmdOut,char *szRcCode)
{
     Funclog();
     CheckHandle2();

     return pevolis_command(m_pPrinter,lpCmdIn,strlen(lpCmdIn),lpCmdOut,16);
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
void QEvolisPrinter::AddText(char *szText,int nAngle, float fxPos, float fyPos, int nFontSize, int nColor)
{
    TextInfoPtr inTextInfo = std::make_shared<TextInfo>();
    char szUtf8[256] = {0};

    inTextInfo->sText = szText;
    inTextInfo->nAngle = nAngle;
    inTextInfo->fxPos = fxPos;
    inTextInfo->fyPos = fyPos;
    inTextInfo->pFontName = ":/Sumsun.ttf";
    inTextInfo->nFontSize = nFontSize;
    inTextInfo->nFontStyle = 1;
    inTextInfo->nColor = nColor;
    inTextInfo->nType = 0;  //文字是0
    m_textInfo.push_back(inTextInfo);
}
