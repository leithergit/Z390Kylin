﻿#include <string>
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
#include "../SDK/include/evolis.h"
#include "QEvolisPrinter.h"
#include "AES.h"
#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc_c.h>
using namespace  cv;

//#include <QPainter>
//#include <QImage>
//#include <qfontdatabase.h>


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

// AES key
unsigned char g_szAesKey[] =  {0x9d,0x06,0x93,0xce,0x80,0x8e,0x37,0xeb,0xd7,0xee,0x09,0x56,0xb5,0xae,0xb3,0xb0};
// 区码F616的密文
unsigned char g_szZoneKey1[16] =  {0x47,0x66,0xf2,0x21,0x79,0x07,0x4f,0xb7,0x28,0x3f,0x6f,0x3a,0x6d,0x17,0xa6,0xc0};
// 区码F61A的密文
unsigned char g_szZoneKey2[16] =  {0x99,0xf4,0xb4,0x85,0xb1,0x10,0xdf,0x75,0x1c,0xec,0xff,0xc8,0xa8,0x15,0xf7,0x38};


#define RunlogF(...)    Runlog(__PRETTY_FUNCTION__,__LINE__,__VA_ARGS__);
#define Funclog()       FuncRunlog(__PRETTY_FUNCTION__,__LINE__);  TraceFunction TF(__PRETTY_FUNCTION__,__LINE__);auto threadID = std::this_thread::get_id();RunlogF("####Current ThreadID:%08x####\n",threadID);

#define     CheckPrinter(x)     if (!x) {\
                                RunlogF("Printer connection is invalid.\n");\
                                strcpy(pszRcCode, "0001");\
                                return 1;  }

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

    string strInfo = LibVer;
    strInfo += pFunction;
    strInfo += "@";
    strInfo += to_string(nLine);
    strInfo += ":";
    strInfo += szBuffer;
    qDebug()<<strInfo.c_str();
}

void FuncRunlog(const char* pFunction,int nLine)
{
    string strInfo = LibVer;
    strInfo += pFunction;
    strInfo += "@";
    strInfo += to_string(nLine);
    strInfo += ":";
    qDebug()<<strInfo.c_str();
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

//int Convert_dib(char **pBuffer, const QImage& img)
// {
//   QImage image;
//   switch (img.format())
//   {
//   case QImage::Format_Mono:
//   case QImage::Format_Indexed8:
//   case QImage::Format_RGB32:
//   case QImage::Format_ARGB32:
//       image = img;
//       break;
//   case QImage::Format_MonoLSB:
//       image = img.convertToFormat(QImage::Format_Mono);
//       break;
//   case QImage::Format_Alpha8:
//   case QImage::Format_Grayscale8:
//       image = img.convertToFormat(QImage::Format_Indexed8);
//       break;
//   default:
//       if (img.hasAlphaChannel())
//           image = img.convertToFormat(QImage::Format_ARGB32);
//       else
//           image = img.convertToFormat(QImage::Format_RGB32);
//       break;
//   }

//   int nSize = image.byteCount();
//   BMP_FILEHDR bf;
//   BMP_INFOHDR bi;

//   int nOffset = 0;
//   char* pImageBuffer = new char[nSize + sizeof(bf) + sizeof(bf)];
//   if (!pImageBuffer)
//       return 0;

//   int nbits;
//   int bpl_bmp;
//   // Calculate a minimum bytes-per-line instead of using whatever value this QImage is using internally.
//   int bpl = ((image.width() * image.depth() + 31) >> 5) << 2;

//   if (image.depth() == 8 && image.colorCount() <= 16)
//   {
//       bpl_bmp = (((bpl + 1) / 2 + 3) / 4) * 4;
//       nbits = 4;
//   }
//   else if (image.depth() == 32)
//   {
//       bpl_bmp = ((image.width() * 24 + 31) / 32) * 4;
//       nbits = 24;
//   }
//   else
//   {
//       bpl_bmp = bpl;
//       nbits = image.depth();
//   }
//   // build file header
//   memcpy(&bf.bfType, "BM", 2);

//   // write file header
//   bf.bfReserved1 = 0;
//   bf.bfReserved2 = 0;
//   bf.bfOffBits = BMP_FILEHDR_SIZE + BMP_WIN + image.colorCount() * 4;
//   bf.bfSize = bf.bfOffBits + bpl_bmp * image.height();
//   memcpy(&pImageBuffer[nOffset], &bf, sizeof(bf));
//   nOffset += sizeof(bf);

//   bi.biSize = BMP_WIN;                // build info header
//   bi.biWidth = image.width();
//   bi.biHeight = image.height();
//   bi.biPlanes = 1;
//   bi.biBitCount = nbits;
//   bi.biCompression = BMP_RGB;
//   bi.biSizeImage = bpl_bmp * image.height();
//   bi.biXPelsPerMeter = image.dotsPerMeterX() ? image.dotsPerMeterX() : 2834; // 72 dpi default
//   bi.biYPelsPerMeter = image.dotsPerMeterY() ? image.dotsPerMeterY() : 2834;
//   bi.biClrUsed = image.colorCount();
//   bi.biClrImportant = image.colorCount();

//   memcpy(&pImageBuffer[nOffset], &bi, sizeof(bi));
//   nOffset += sizeof(bi);


//   if (image.depth() != 32)
//   {                // write color table
//       uchar* color_table = new uchar[4 * image.colorCount()];
//       uchar* rgb = color_table;
//       QVector<QRgb> c = image.colorTable();
//       for (int i = 0; i < image.colorCount(); i++)
//       {
//           *rgb++ = qBlue(c[i]);
//           *rgb++ = qGreen(c[i]);
//           *rgb++ = qRed(c[i]);
//           *rgb++ = 0;
//       }
//       memcpy(&pImageBuffer[nOffset], color_table, 4 * image.colorCount());
//       nOffset += 4 * image.colorCount();

//       delete []color_table;
//   }

//   int y;

//   if (nbits == 1 || nbits == 8)
//   {                // direct output
//       for (y = image.height() - 1; y >= 0; y--)
//       {
//           memcpy(&pImageBuffer[nOffset], image.constScanLine(y), bpl);
//           nOffset += bpl;
//       }

//       return nOffset;
//   }

//   uchar* buf = new uchar[bpl_bmp];
//   uchar* b, * end;
//   const uchar* p;

//   memset(buf, 0, bpl_bmp);
//   for (y = image.height() - 1; y >= 0; y--)
//   {        // write the image bits
//       if (nbits == 4)
//       {                        // convert 8 -> 4 bits
//           p = image.constScanLine(y);
//           b = buf;
//           end = b + image.width() / 2;
//           while (b < end)
//       {
//               *b++ = (*p << 4) | (*(p + 1) & 0x0f);
//               p += 2;
//           }
//           if (image.width() & 1)
//               *b = *p << 4;
//       }
//       else
//       {                                // 32 bits
//           const QRgb* p = (const QRgb*)image.constScanLine(y);
//           const QRgb* end = p + image.width();
//           b = buf;
//           while (p < end)
//       {
//               *b++ = qBlue(*p);
//               *b++ = qGreen(*p);
//               *b++ = qRed(*p);
//               p++;
//           }
//       }
//       memcpy(&pImageBuffer[nOffset], buf, bpl_bmp);
//       nOffset += bpl_bmp;
//   }
//   delete[] buf;
//   *pBuffer = pImageBuffer;
//   return nOffset;
//}

float Pt2MM(float pt)
{
    return pt/2.845;
}

QEvolisPrinter::QEvolisPrinter()
{
    Funclog();

    evolis_log_set_level(EVOLIS_LG_DEBUG);
    evolis_log_set_level(EVOLIS_LG_ERROR);
    auto tTime = std::chrono::system_clock::to_time_t(chrono::system_clock::now());
    struct tm* ptm = localtime(&tTime);
    char szLog[128] = {0};
    QString strCurDir = QDir::currentPath();

    sprintf(szLog,"/log/evolis_%02d%02d%02d.log",ptm->tm_year + 1900,ptm->tm_mon + 1,ptm->tm_mday);
    QString strlogPath = strCurDir + szLog;
    evolis_log_set_path(strlogPath.toStdString().c_str());

    strOverlayer = strCurDir.toStdString() + "/resources/iso.bmp";

//    bRunning = true;
//    pThread = new thread(&QEvolisPrinter::run,this);
//    this_thread::sleep_for(std::chrono::milliseconds(250));
//    ExtractIso();
//    ExtractFont();
//    strOverlayer = "./iso.bmp";
}

//void QEvolisPrinter::ExtractIso()
//{
//    QFileInfo fi("./iso.bmp");
//    if (fi.isFile())
//    {
//        RunlogF("File ./iso.bmp has already exist!\n");
//        return ;
//    }
//    RunlogF("./iso.bmp not exist,try to create...!\n");

//    RunlogF("Try to create file ./iso.bmp!\n");
//    if (!QFile::copy(":/iso.bmp","./iso.bmp"))
//        RunlogF("Failed in creating file ./iso.bmp!\n");

//    strOverlayer = "./iso.bmp";
//}

//void QEvolisPrinter::ExtractFont()
//{
//    QFileInfo fi("./simsun.ttc");
//    if (fi.isFile())
//    {
//        RunlogF("File ./simsun.ttc has already exist!\n");
//        return ;
//    }
//    RunlogF("./simsun.ttc not exist,try to create...!\n");

//    RunlogF("Try to create file ./simsun.ttc!\n");
//    if (!QFile::copy(":/SIMSUN.ttf","./simsun.ttc"))
//        RunlogF("Failed in creating file ./simsun.ttc!\n");
//}

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

int  QEvolisPrinter::Print_Open(char *pPort, char *pPortParam, char *pszRcCode)
{
    Funclog();
    TaskPtr pTask = make_shared<Task>(Task_Print_Open,pszRcCode);
    //RunlogF("Put a task.\n");
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    //RunlogF("Send notify for a new task.\n");
    cvTask.notify_one();
    unique_lock<mutex> lockComplete(mtComplete);
    //RunlogF("Wait for task complete.\n");
    cvComplete.wait(lockComplete);
    RunlogF("Task complete,Result = %d,RcCode = %s.\n",pTask->nResult,pTask->pRCode);
    return pTask->nResult;
}

//0001	入参校验失败（入参为空/格式不符/）
//0002	方法执行失败
//0003	句柄为空
//0004	打开端口失败
//0005	卡片复位失败

int  QEvolisPrinter::Print_Close(char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Close,pszRcCode);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int  QEvolisPrinter::Print_Reset(long lTimeout, int nResetAction, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Reset,pszRcCode);
    pTask->pValue = &nResetAction;
    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(false);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_ResetSync(long lTimeout, int nResetAction, char *pszRcCode)
{
    Q_UNUSED(lTimeout);
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Reset,pszRcCode);
    pTask->pValue = &nResetAction;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

// 退卡到门口
int  QEvolisPrinter::Print_EjectSync(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Eject,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);

}

int QEvolisPrinter::Print_EnableCard(long lTimeout, int nCheckMode, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_EnableCard,pszRcCode,lTimeout,nCheckMode);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int QEvolisPrinter::Print_DisableCard(long lTimeout,  char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_DisableCard,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int  QEvolisPrinter::Print_Eject(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Eject,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(false);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Retract,pszRcCode,lTimeout);
    pTask->pValue = &nBoxNo;
    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(false);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_RetractSync(long lTimeout, int nBoxNo, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Retract,pszRcCode,lTimeout);
    pTask->pValue = &nBoxNo;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int  QEvolisPrinter::Print_DispenseSync(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Dispense,pszRcCode,lTimeout);
    int nArray[2] = {nBox,nDispPos};
    pTask->pValue = nArray;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int  QEvolisPrinter::Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Dispense,pszRcCode,lTimeout);
    int nArray[2] = {nBox,nDispPos};
    pTask->pValue = nArray;
    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        //unique_lock<mutex> lock(mtlistTask);
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(false);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_GetBoxStatus,pszRcCode,lTimeout);
    pTask->pValue = &lpBoxInfo;

    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(true);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_GetBoxStatusSync(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_GetBoxStatus,pszRcCode,lTimeout);
    pTask->pValue = &lpBoxInfo;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int  QEvolisPrinter::Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Status,pszRcCode,lTimeout);
    pTask->pValue = &lpStatus;
    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(true);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_StatusSync(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS &lpStatus, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_Status,pszRcCode,lTimeout);
    pTask->pValue = &lpStatus;
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int  QEvolisPrinter::Print_InitPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_InitPrint,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(false);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_InitPrintSync(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_InitPrint,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

int  QEvolisPrinter::Print_StartPrint(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_StartPrint,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask,std::try_to_lock);
    if (lock.owns_lock())
    {
        listTask.push_back(pTask);
        lock.unlock();
        NotifyandWait(false);
    }
    else
    {
        strcpy(pszRcCode,"0002");
        return 2;
    }
}

int  QEvolisPrinter::Print_StartPrintSync(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    TaskPtr pTask = make_shared<Task>(Task_Print_StartPrint,pszRcCode,lTimeout);
    unique_lock<mutex> lock(mtlistTask);
    listTask.push_back(pTask);
    lock.unlock();
    NotifyandWait(true);
}

void QEvolisPrinter::run()
{
    while(bRunning)
    {
        unique_lock<mutex> locklist(mtlistTask);
        RunlogF("Waiting for a new Task.\n");
        cvTask.wait(locklist);
        if (listTask.size())
        {
            RunlogF("Get a Task,listTask.\n");
            TaskPtr pTask = listTask.front();            
            RunlogF("ProcessTask a Task.\n");
            pTask->nResult = ProcessTask(pTask);
            listTask.pop_front();
            RunlogF("Task Finished.\n");
        }
        //unique_lock<mutex> CompleteLock(mtComplete);
        //RunlogF("Try to Send notify to Requestor.\n");
        cvComplete.notify_one();
    }
    RunlogF("JVM deatch current thread.\n");
}

int QEvolisPrinter::ProcessTask(TaskPtr pTask)
{
    Funclog();
    RunlogF("Task Type :%d.\n",pTask->nTaskType);
    switch (pTask->nTaskType)
    {
    case Task_Print_Open         :
        return On_Print_Open("evolis_zenius","",pTask->pRCode);
    case Task_Print_Close        :
        return On_Print_Close(pTask->pRCode);
    case Task_Print_Reset        :
        return On_Print_Reset(pTask->nTimeout,*((int*)pTask->pValue), pTask->pRCode);
    case Task_Print_Eject        :
        return On_Print_Eject(pTask->nTimeout, pTask->pRCode);
    case Task_Print_Retract      :
        return On_Print_Retract(pTask->nTimeout,*((int*)pTask->pValue), pTask->pRCode);
    case Task_Print_Dispense     :
    {
        int *pArray = (int*)pTask->pValue;
        return On_Print_Dispense(pTask->nTimeout,pArray[0],pArray[1], pTask->pRCode);
    }
    case Task_Print_GetBoxStatus :
    {
        Lithograph::LPLITHOGRAPHBOXINFO pBoxinfo = *(Lithograph::LPLITHOGRAPHBOXINFO *)pTask->pValue;
        return On_Print_GetBoxStatus(pTask->nTimeout,pBoxinfo, pTask->pRCode);
    }
    case Task_Print_Status       :
    {
        Lithograph::LPLITHOGRAPHSTATUS pStatus = *(Lithograph::LPLITHOGRAPHSTATUS*)pTask->pValue;
        return On_Print_Status(pTask->nTimeout,pStatus, pTask->pRCode);
    }
    case Task_Print_InitPrint    :
        return On_Print_InitPrint(pTask->nTimeout, pTask->pRCode);
    case Task_Print_StartPrint   :
        return On_Print_StartPrint(pTask->nTimeout, pTask->pRCode);
    case Task_Print_EnableCard:
        return On_Print_EnableCard(pTask->nTimeout,pTask->nArrayParam[0],pTask->pRCode);
    case Task_Print_DisableCard:
        return On_Print_DisableCard(pTask->nTimeout, pTask->pRCode);
    }
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
    int n = 0;
    if ((n = evolis_get_devices(&devices, 0, 0)) >= 0)
    {
        qDebug("%d printers found\r\n", n);
        for (int i = 0; i < n; ++i)
        {
            char name[64];

            qDebug("- %s\r\n", tocstring(name, devices[i].name));
            qDebug("  Id: %s\r\n", devices[i].id);
            qDebug("  Uri: %s\r\n", devices[i].uri);
            qDebug("  Mark: %s\r\n", evolis_get_mark_name(devices[i].mark));
            qDebug("  Model: %s\r\n", evolis_get_model_name(devices[i].model));
            qDebug("  isSupervised: %s\r\n", (devices[i].isSupervised) ? "true" : "false");
            qDebug("  link: %s\r\n",
                    (devices[i].link == EVOLIS_LI_TCP) ? "TCP" :
                    (devices[i].link == EVOLIS_LI_USB) ? "USB" : "?");
        }
        RunlogF("evolis_get_devices Succeed.\n");
    }

    RunlogF("Try to call evolis_open.\n");
    m_pPrinter = evolis_open2("usb:///dev/usb/lp0", EVOLIS_TY_EVOLIS);
    //m_pPrinter = evolis_open(L"Evolis Zenius", false);
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
        if (evolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
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
                       };
    //RunlogF("evolis_command(Rzp) Succeed,Reply:%s.\n",szReply);
    for (auto var:szCmd)
    {
        //RunlogF("Try to evolis_command(%s).\n",var);
        if (evolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
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
    evolis_close(m_pPrinter);
    //RunlogF("evolis_close Succeed.\n");
    m_pPrinter = nullptr;

    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_Reset(long lTimeout, int nResetAction, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    if (m_pPrinter)
    {
//        RunlogF("Try to evolis_reset.\n");
//        evolis_reset(m_pPrinter, lTimeout, &timeouted);
//        RunlogF("evolis_reset Succeed.\n");
        switch (nResetAction)
        {
        default:
        case 1:
        {
            strcpy(pszRcCode, "0000");
            return 0;
            break;
        }
        case 2:
        {
            char szReply[32] = {0};
            char *szCmd = "Rlr;p";     // detect whether there's is card in printer
            if (evolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply,sizeof(szReply)) < 0)
            {
                RunlogF("Failed in evolis_command(%s).\n",szCmd);
                return 1;
            }

            if (strcmp(szReply,"OK")  == 0)
            {
                RunlogF("There is no card in printer.\n");
                strcpy(pszRcCode, "0001");
                return 1;
            }

            const char* szCommand[] = {
                                        "Ss",
                                        "Pneab;A;1",
                                        "Pcem;I",   // 设置退卡模式(eject):
                                                    //I	退卡到出卡口(bezel,only for kc200B)
                                                    //k	退卡到底部卡槽(lower reject slot, only for kc200B)
                                        "Se"};
            for (auto var :szCommand)
            {
                int error = evolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
                if (error < 0)
                {
                    strcpy(pszRcCode, "0002");
                    RunlogF("Failed in evolis_command(%s).\n",var);
                    return 1;
                }
            }
        }
        case 3:
        {
            char szReply[32] = {0};
            char *szCmd = "Rlr;p";     // detect whether there's is card in printer
            if (evolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply,sizeof(szReply)) < 0)
            {
                RunlogF("Failed in evolis_command(%s).\n",szCmd);
                return 1;
            }

            if (strcmp(szReply,"OK")  == 0)
            {
                RunlogF("There is no card in printer.\n");
                strcpy(pszRcCode, "0001");
                return 1;
            }
            // 吞卡到废卡箱
            const char* szCommand[] = {"Ss","Pneab;A;1","Pcem;N","Se" };
            for (auto var : szCommand)
            {
                if(evolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply)) < 0)
                {
                    RunlogF("evolis_command(%s)failed.\n",var);
                    strcpy(pszRcCode, "0002");
                    return 1;
                }
            }
            break;
        }
        }

        strcpy(pszRcCode, "0000");
        return 0;
    }
    else
    {
        strcpy(pszRcCode, "0001");
        return 1;
    }
     return 0;
}

int  QEvolisPrinter::On_Print_Eject(long lTimeout, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);
    char szReply[128] = {0};
    const char *szCmd[]={"Rlr;p"};      // 查询打印内部
                         //"Rse;M"};      // 查询出卡口

    int nIdx = 0;
    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        return 1;
    }
//    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
//    char szReply2[128] = {0};
//    nIdx ++;
//    RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
//    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply2,sizeof(szReply2)) < 0)
//    {
//        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
//        return 1;
//    }
//    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply2);
    if (strcmp(szReply,"OK")  == 0)
    {
        RunlogF("There is no card in printer.\n");
        strcpy(pszRcCode, "0001");
        return 1;
    }
//    else
//    {
//        // 0,2x Volts是没卡
//        // 4,9x Volts是有卡
//        szReply2[1] = '.';
//        float fVolt = atof(szReply2);
//        RunlogF("Rse;b Status:%.2f.\n",fVolt);  // read the kc200 Bezel Sensor
//        if (fVolt > 3.0)
//        {
//            RunlogF("There is a card in printer.\n");
//            strcpy(pszRcCode, "0001");
//            return 1;
//        }
//    }

    const char* szCommand[] = {
                                "Ss",
                                "Pneab;A;1",
                                "Pcem;I",   // 设置退卡模式(eject):
                                            //I	退卡到出卡口(bezel,only for kc200B)
                                            //k	退卡到底部卡槽(lower reject slot, only for kc200B)
                                "Se"};
    for (auto var :szCommand)
    {
        //RunlogF("Try to evolis_command(%s).\n",var);
        int error = evolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
        if (error < 0)
        {
            strcpy(pszRcCode, "0001");
            RunlogF("Failed in evolis_command(%s).\n",var);
            return 1;
        }
        //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",var,szReply);
    }

    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_Retract(long lTimeout, int nBoxNo, char *pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    char szReply[32] = {0};
    char *szCmd = "Rlr;p";

    if (evolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd);
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
        Res = evolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
        if (Res < 0)
        {
            RunlogF("evolis_command(%s)failed.\n",var);
            strcpy(pszRcCode, "0002");
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
                         "Pcem;M",      // 从出卡口出卡
                         //"Pneab;E"    // 打印结束后不出卡
                        };
    // Pcim;B       // 设置为可同时从卡箱和出卡口手动进卡
    /* 关于Pcim;B
    1. 如果卡盒有卡时输入进卡指令，机器还是会从卡盒进卡，从缝隙处手动插卡无效
    2. 如果卡盒无卡时输入进卡指令，可以此时放一张卡在卡盒或缝隙，都可以进卡
    3. 输入Pcim;B后，即使卡盒无卡，打印机在驱动里也会显示就绪，此前在Pcim;F同状态下会显示卡片装载的黄色警告
    */

    int Res = 0;
    for (auto var : szCommand)
    {
        //RunlogF("Try to evolis_command(%s).\n",var);
        Res = evolis_command(m_pPrinter, var,strlen(var), RespValue,1024);
        if (Res < 0){
            RunlogF("evolis_command(%s)failed.\n",var);
            strcpy(pszRcCode, "0002");
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
         if (evolis_command(m_pPrinter,var,strlen(var),szReply,sizeof(szReply)) < 0)
         {
             RunlogF("Failed in evolis_command(%s).\n",var);
             strcpy(pszRcCode, "0001");
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

//    RunlogF("Try to evolis_set_input_tray(EVOLIS_IT_FEEDER).\n");
//    int nRes = evolis_set_input_tray(m_pPrinter,EVOLIS_IT_FEEDER);
//    if (nRes)
//    {
//        strcpy(pszRcCode, "0001");
//        RunlogF("Failed in evolis_set_input_tray(EVOLIS_IT_FEEDER).\n");
//        return 1;
//    }
//    RunlogF("evolis_set_input_tray(EVOLIS_IT_FEEDER) Succeed.\n");

    //RunlogF("Try to evolis_set_card_pos(EVOLIS_CP_INSERT).\n");
    int nRes = evolis_set_card_pos(m_pPrinter,EVOLIS_CP_INSERT);
    if (nRes)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_INSERT).\n");
        return 1;
    }
    //RunlogF("evolis_set_card_pos(EVOLIS_CP_INSERT) Succeed.\n");

    this_thread::sleep_for(std::chrono::milliseconds(200));
    //RunlogF("Try to evolis_set_card_pos(EVOLIS_CP_CONTACT).\n");
    nRes = evolis_set_card_pos(m_pPrinter,EVOLIS_CP_CONTACT);
    if (nRes)
    {
        strcpy(pszRcCode, "0001");
        RunlogF("Failed in evolis_set_card_pos(EVOLIS_CP_CONTACT).\n");
        return 1;
    }
    //RunlogF("evolis_set_card_pos(EVOLIS_CP_CONTACT) Succeed.\n");
    nCardPosition = Pos_Contact;
    strcpy(pszRcCode, "0000");
    return 0;
}

int  QEvolisPrinter::On_Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    const char *szCmd="Rse;f";
    char szReply[128] = {0};
    // chechk the card box status
    //RunlogF("Try to evolis_command(%s).\n",szCmd);
    //卡箱无卡时：evolis_command(Rse;f) 返回:0,00 Volts
    //卡箱有卡时：evolis_command(Rse;f) 返回:5,00 Volts，这个可以对上了
    if (evolis_command(m_pPrinter,szCmd,strlen(szCmd),szReply,128) < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szCmd,szReply);
        strcpy(pszRcCode, "0001");
        return 1;
    }
    //RunlogF("evolis_command(%s) succeed,Reply:%s",szCmd,szReply);

    CardBoxInfo[0].byType = 1;           //卡箱类型 0:未知，1：发卡箱，2：回收箱
    CardBoxInfo[0].byBoxNumber = 1;      //卡箱号
    CardBoxInfo[0].bHardwareSensor = false;//是否支持故障状态传感器,TRUE:支持，FALSE：不支持
    if (szReply[0] == '5')
        CardBoxInfo[0].byStatus = 0;     //卡箱状态,0：正常; 1：卡少; 2:无卡; 3：不可操作; 4：不存在; 5：高(快满)；6：满； 7：未知
    else
        CardBoxInfo[0].byStatus = 2;

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
//    else
//    {
//        CardBoxInfo[0].byStatus = 0;
//    }

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
        strcpy(pszRcCode, "0002");
        return 1;
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

    //RunlogF("Try to evolis_command(Pneab;E).\n");
    nRes = evolis_command(m_pPrinter,pCommand, strlen(pCommand), szReply, 1024);
    if (nRes < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",pCommand,szReply);
        strcpy(pszRcCode, "0002");
        return 1;
    }

    this_thread::sleep_for(chrono::milliseconds(10));//1毫秒

    //RunlogF("Try to evolis_print_init.\n");
    if (evolis_print_init(m_pPrinter))
    {
        RunlogF("evolis_print_init Failed.\n");
        strcpy(pszRcCode, "0002");
        return 1;
    }
    //RunlogF("evolis_print_init Succeed.\n");
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
    nRes = evolis_command(m_pPrinter,pCommand, strlen(pCommand), szReply, 128);
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

    int ec = Cv_PrintCard(m_picInfo, m_textInfo,lTimeout);
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
    if (!pPrinter)
        return 1;

    evolis_major_state_t major;
    evolis_minor_state_t minor;
    if (evolis_get_state(pPrinter, &major, &minor))
    {
        RunlogF("Failed in evolis_get_state.\n");
        return 1;
    }

    strcpy(majorStatusValue, evolis_get_major_string(major));
    strcpy(minorStatusValue,evolis_get_minor_string(minor));

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
    if (!pPrinter)
        return 1;

    *Media = 0;        // No card

    DWORD  ec;
    evolis_ribbon_t ribbon;
    /// Get ribbon infos.
    //RunlogF("Try to evolis_get_ribbon.\n");
    ec = evolis_get_ribbon(pPrinter, &ribbon);
    if (ec != 0)
    {
        RunlogF("Failed in evolis_get_ribbon.\n");
        return 1;
    }
    RunlogF("ribbon.capacity = %d\tribbon.remaining = %d.\n",ribbon.capacity,ribbon.remaining);
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
    //RunlogF("majorStatusValue = %s\tminorStatusValue = %s.\n",majorStatusValue,minorStatusValue);
    if (strcmp(minorStatusValue, "DEF_UNSUPPORTED_RIBBON") == 0)
        *RibbonNum = -98;
    if(strcmp(minorStatusValue,"PRINTER_OFFLINE") == 0)
        *Device = 2;//是否离线

    const char *szCmd[]={"Rlr;p",      // 查询打印内部
                         "Rse;b"};      // 查询出卡口
    char szReply[16] = {0};

    int nIdx = 0;
    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,16) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        return 1;
    }
    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);

    char szReply2[16] = {0};
    nIdx ++;
    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply2,16) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        return 1;
    }
    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply2);
    //Media 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
    if (strcmp(szReply,"OK") == 0)
    {
        // 0,2x Volts是没卡
        // 4,9x Volts是有卡
        szReply2[1] = '.';
        float fVolt = atof(szReply2);
        RunlogF("%s Status:%.2f.\n",szCmd[nIdx],fVolt);
        if (fVolt >= 3.0)
            *Media = 1;
        else
            *Media = 0;
    }
    else if (strcmp(szReply,"CARD") == 0||
             strcmp(szReply,"CONTACTLESS CARD"))
        *Media = 2;
    else if (strcmp(szReply,"SMART CARD") == 0)
        *Media = 3;

    evolis_status_t es;
    //RunlogF("Try to evolis_status.\n");
    if (evolis_status(pPrinter, &es) != 0)
    {
        RunlogF("Error evolis_status.\n");
        *Device = 2;    // 离线
    }
    else
    {
        *Device = 0;    // 在线
        RunlogF("evolis_status return:config = %d\tinformation = %d\twarning = %d\terror = %d.\n", es.config,es.information, es.warning,es.error);
        for (int i = 0;i < EVOLIS_STATUS_EX_COUNT;i ++)
            RunlogF("evolis_status return exts[%d] = %d.\n",i, es.exts[i]);
    }

    return 0;
}

int QEvolisPrinter::SetDarkTextRegion(int nLeft, int nTop, int nRight, int nBottom)
{
    if (!m_pPrinter)
        return 1;

    if (nLeft < 0 || nTop <0 || nRight < 0 || nBottom<0 )
        return 1;

    if (evolis_print_set_option(m_pPrinter, "FBlackManagement", " TEXTINBLACK"))
    {
         RunlogF("Failed in evolis_print_set_option(FBlackManagement,TEXTINBLACK).\n");
         return 1;
    }
    //RunlogF("try to evolis_print_set_option(IFTextRegion,%s).\n","0x0x1000x600");
    evolis_print_set_option(m_pPrinter, "IFTextRegion", "0x0x1000x600");

    //RunlogF("Try to evolis_print_set_option(IFBlackLevelValue, 40).\n");
    if (evolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "40"))
    {
         RunlogF("Failed in evolis_print_set_option(IFBlackLevelValue,40).\n");
         return 1;
    }

}
//#include <QHash>
//bool loadFontFamilyFromFiles(const QString &strFontFileName,QString &strFontFamily)
//{
//    static QHash<QString, QString> FontTable;
//    if (FontTable.contains(strFontFileName))
//    {
//        strFontFamily = FontTable.value(strFontFileName);
//        return true;
//    }
//    QFileInfo fi(strFontFileName);
//    if (!fi.isFile())
//        return false;

//    int nFontID = QFontDatabase::addApplicationFont(strFontFileName);
//    if (nFontID == -1)
//        return false;

//    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(nFontID);
//    if(loadedFontFamilies.empty())
//        return false;

//    strFontFamily = loadedFontFamilies.at(0);
//    FontTable.insert(strFontFileName,strFontFamily);
//    return true;
//}

#define LOBYTE(w)         ((unsigned char)(((ULONG)(w)) & 0xff))
#define Value_R(rgb)      (LOBYTE(rgb))
#define Value_G(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define Value_B(rgb)      (LOBYTE((rgb)>>16))

//int QEvolisPrinter::Qt_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout)
//{
//    char pszRcCode[1024] = {0};
//    CheckPrinter(m_pPrinter);
//    if (evolis_print_init((evolis_t*)m_pPrinter))
//    {
//        RunlogF("evolis_print_init failed.\n");
//        return 1;
//    }

//    const char *szCmd[]={"Rrt;count","Pneab;A;0","Pneab;A;1"};
//    char szReply[128] = {0};
//    int nIdx = 0;
//    RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
//    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
//    {
//        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
//        return 1;
//    }
//    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
//    int nRibbonCount = strtol(szReply,nullptr,10);
//    if (nRibbonCount <= 0)
//    {
//        RunlogF("The ribbon is exhausted and printing failed.\n");
//        return 1;
//    }
//    nIdx++;
//    RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
//    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
//    {
//        RunlogF("Failed in evolis_command(%s):%s.\n",szCmd[nIdx]);
//        return 1;
//    }
//    RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
//    nIdx++;

//    QImage Convas(nCardWidth,nCardHeight,  QImage::Format_RGB888);
//    Convas.fill(Qt::white);
//    QPainter painter(&Convas);
//    painter.setCompositionMode(QPainter::CompositionMode_Source);
//    QImage FileImage;
//    if (!FileImage.load(QString::fromStdString( inPicInfo.picpath)))
//    {
//        RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
//        return 1;
//    }
//    RunlogF("Graph:%s\txPos = %.1f\tyPos = %.1f\tAngle = %d.\n",inPicInfo.picpath.c_str(),inPicInfo.fxPos,inPicInfo.fyPos,inPicInfo.nAngle);
//    painter.save();
//    painter.translate(nCardWidth/2,nCardHeight/2);
//    painter.rotate(inPicInfo.nAngle);
//    painter.translate(-nCardWidth/2,-nCardHeight/2);
//    painter.drawImage(QRect(MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),MM2Pixel(inPicInfo.fWidth),MM2Pixel(inPicInfo.fHeight)),FileImage);
//    painter.restore();

//    for (auto var : inTextVector)
//    {
//        QString imageText = QString::fromStdString(var->sText);
//        QString strFontFamily;
//        if (!loadFontFamilyFromFiles(var->pFontName.c_str(),strFontFamily))
//        {
//            RunlogF("Failed in load font %s.\n",var->pFontName.c_str());
//            continue;
//        }

//        QFont font(strFontFamily);
//        //设置显示字体的大小
//        font.setPointSize(var->nFontSize);
//        QFontMetrics fm(font);

//        auto TextRect = fm.boundingRect(imageText);
//        painter.setFont(font);
//        QPen pen = painter.pen();

//        pen.setColor(QColor(Value_R(var->nColor),Value_G(var->nColor),Value_B(var->nColor)));
//        painter.setPen(pen);
//        RunlogF("Text:%s\txPos = %.1f\tyPos = %.1f\tAngle = %d.\n",var->sText.c_str(),var->fxPos,var->fyPos,var->nAngle);
//        painter.save();
//        painter.translate(nCardWidth/2,nCardHeight/2);
//        painter.rotate(var->nAngle);
//        painter.translate(-nCardWidth/2,-nCardHeight/2);
//        painter.drawText(QRectF(MM2Pixel(var->fxPos), MM2Pixel(var->fxPos), MM2Pixel(TextRect.width()), MM2Pixel(TextRect.height())), Qt::AlignLeft | Qt::AlignTop, imageText);
//        painter.restore();
//    }
//    Convas.save("./Preview.bmp");

//    char *pFileBuffer = nullptr;
//    int nBufferSize = Convert_dib(&pFileBuffer,Convas);
//    if (!nBufferSize)
//    {
//        if (pFileBuffer)
//            delete []pFileBuffer;
//        RunlogF("failed in  Convert_dib:%s.\n");
//        return 1;
//    }
//    shared_ptr<char> Buffdealloc(pFileBuffer);
//    QFileInfo fi(strOverlayer.c_str());
//    if (evolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
//    {
//        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
//        strcpy(pszRcCode, "0001");
//        return 1;
//    }

//    if (evolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
//    {
//        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
//        strcpy(pszRcCode, "0001");
//        return 1;
//    }

//    if (!fi.isFile())
//    {
//        RunlogF("Can't find file %s.\n",strOverlayer.c_str());
//        strcpy(pszRcCode, "0001");
//        return 1;
//    }
//    if (evolis_print_set_imageb(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT, pFileBuffer,nBufferSize))
//    {
//        RunlogF("evolis_print_set_imageb failed:%s.\n",inPicInfo.picpath.c_str());
//        return 1;
//    }

//    evolis_print_exec((evolis_t*)m_pPrinter);
//    int             printed = 0;
//    evolis_status_t es;
//    auto tNow = system_clock::now();
//    auto tLast = tNow;
//    while (!printed)
//    {
//        if (evolis_status(m_pPrinter, &es) != 0)
//        {
//            RunlogF("Error reading printer status\n");
//            return 0;
//        }
//        printed = !(es.information & EVOLIS_INF_BUSY);
//        tNow = system_clock::now();
//        auto tDeration = duration_cast<milliseconds>(tNow - tLast);
//        if (tDeration.count() > 500)
//        {
//            tLast = tNow;
//            RunlogF("Waiting printer task stop\n");
//        }
//    }
//    return 0;
//}

int QEvolisPrinter::Cv_PrintCard(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,long nTimeout)
{
    Funclog();
    if (!m_pPrinter)
        return 1;

    QFileInfo fipic(inPicInfo.picpath.c_str());
    if (!fipic.isFile() && !inTextVector.size())
    {
        RunlogF("There is no any content to print!\n");
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
    if (evolis_print_init((evolis_t*)m_pPrinter))
    {
        RunlogF("evolis_print_init failed.\n");
        return 1;
    }
    if (strFColorBrightness.size())
    {
        if (!evolis_print_set_option((evolis_t*)m_pPrinter,"FColorBrightness",strFColorBrightness.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorBrightness",strFColorBrightness.c_str());
        }
    }

    if (strBColorBrightness.size())
    {
        if (!evolis_print_set_option((evolis_t*)m_pPrinter,"BColorBrightness",strBColorBrightness.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorBrightness",strBColorBrightness.c_str());
        }
    }

    if (strFColorContrast.size())
    {
        if (!evolis_print_set_option((evolis_t*)m_pPrinter,"FColorContrast",strFColorContrast.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","FColorContrast",strFColorContrast.c_str());
        }
    }

    if (strBColorContrast.size())
    {
        if (!evolis_print_set_option((evolis_t*)m_pPrinter,"BColorContrast",strBColorContrast.c_str()))
        {
             RunlogF("Failed in evolis_print_set_option(%s,%s).\n","BColorContrast",strBColorContrast.c_str());
        }
    }

    const char *szCmd[]={"Rrt;count","Pneab;A;0","Pneab;A;1"};
    char szReply[128] = {0};
    int nIdx = 0;
    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        return 1;
    }
    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
    int nRibbonCount = strtol(szReply,nullptr,10);
    if (nRibbonCount <= 0)
    {
        RunlogF("The ribbon is exhausted and printing failed.\n");
        return 1;
    }
    nIdx++;
    //RunlogF("Try to evolis_command(%s).\n",szCmd[nIdx]);
    if (evolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s):%s.\n",szCmd[nIdx]);
        return 1;
    }
    //RunlogF("evolis_command(%s) Succeed,Reply:%s.\n",szCmd[nIdx],szReply);
    nIdx++;

    Mat canvas(nCardHeight,nCardWidth,CV_8UC3,Scalar(255,255,255));

    //RunlogF("Try to load file %s.\n",inPicInfo.picpath.c_str());

    Mat HeaderImage = imread(inPicInfo.picpath, IMREAD_ANYCOLOR);
    if (!HeaderImage.data)
    {
         RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
         return 1;
    }
    Mat HeaderPrint;
    resize(HeaderImage, HeaderPrint, Size(MM2Pixel(inPicInfo.fWidth) , MM2Pixel(inPicInfo.fHeight)), 0, 0, INTER_NEAREST);

    Mat HeaderROI = canvas(Rect(MM2Pixel(inPicInfo.fxPos),MM2Pixel(inPicInfo.fyPos),MM2Pixel(inPicInfo.fWidth),MM2Pixel(inPicInfo.fHeight)));
    HeaderPrint.copyTo(HeaderROI);

    int nDarkLeft = 0,nDarkTop = 0,nDarkRight = 0,nDarkBottom = 0;

    for (auto var : inTextVector)
    {
        RunlogF("Text = %s,FontSize = %d,xPos = %.02f,yPos = %.2f.\n",var->sText.c_str(),var->nFontSize);
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
        RunlogF("Text = %s,FontSize = %.2f, FontPixel = %d.\n",var->sText.c_str(),var->nFontSize,fontHeight);
        try
        {
            cv::Ptr<cv::freetype::FreeType2> ft2;
            ft2 = cv::freetype::createFreeType2();

            ft2->loadFontData(var->pFontName, 0); //加载字库文件
            ft2->putText(FontROI, var->sText.c_str(), cv::Point(0, 0), fontHeight, CV_RGB(0, 0, 0), -1, CV_AA, false/*,var->nFontStyle == 2*/);
        }
        catch (std::exception &e)
        {
            RunlogF("Catch a exception:%s.\n",e.what());
            return 1;
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
        return 1;
    }
    if (evolis_print_set_option(m_pPrinter,"IFOverlayCustom",strOverlayer.c_str()))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n",strOverlayer.c_str());
        return 1;
    }

    if (strGRibbonType.size())
    {
        if (evolis_print_set_option(m_pPrinter, "GRibbonType", strGRibbonType.c_str()))
        {
            RunlogF("Failed in evolis_print_set_option(GRibbonType,%s).\n",strGRibbonType.c_str());
            return 1;
        }
    }

    if (evolis_print_set_option(m_pPrinter,"FOverlayManagement", "BMPVARNISH"))
    {
        RunlogF("Failed in evolis_print_set_option(FOverlayManagement,BMPVARNISH).\n");
        return 1;
    }

    evolis_print_set_option(m_pPrinter, "FBlackManagement", "TEXTINBLACK");

    char szDarkZone[32] = {0};
    if (inPicInfo.nAngle > 0)
        sprintf(szDarkZone,"%dx%dx%dx%d",0,0,nCardWidth - nDarkLeft,nCardHeight - nDarkTop);
    else
        sprintf(szDarkZone,"%dx%dx%dx%d",nDarkLeft,nDarkTop,nDarkRight,nDarkBottom);

    evolis_print_set_option(m_pPrinter, "IFTextRegion", szDarkZone);

    evolis_print_set_option(m_pPrinter, "IFBlackLevelValue", "50");

    evolis_print_set_option(m_pPrinter, "IFDarkLevelValue ", "30");

    if (evolis_print_set_imagep(m_pPrinter, evolis_face_t::EVOLIS_FA_FRONT,strTempFile.toUtf8().data()))
    {
        RunlogF("evolis_print_set_imagep failed:%s.\n",strTempFile.toUtf8().data());
        return 1;
    }

    RunlogF("Try to evolis_print_exec.\n");
    evolis_print_exec((evolis_t*)m_pPrinter);
    int             printed = 0;
    evolis_status_t es;
    auto tNow = system_clock::now();
    auto tLast = tNow;
    while (!printed)
    {
        if (evolis_status(m_pPrinter, &es) != 0)
        {
            RunlogF("Error reading printer status\n");
            return 0;
        }
        printed = !(es.information & EVOLIS_INF_BUSY);
        tNow = system_clock::now();
        auto tDeration = duration_cast<milliseconds>(tNow - tLast);
        if (tDeration.count() > nTimeout*1000)
        {
            tLast = tNow;
            RunlogF("Waiting printer task stop\n");
            break;
        }
    }
    return 0;
}
