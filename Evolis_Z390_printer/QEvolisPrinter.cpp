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
#include <QSettings>
#include <fstream>
#include <sstream>
#include <turbojpeg.h>
#include <QJsonObject>

#include <QJsonParseError>
#include "SimpleIni.h"
#include "QEvolisPrinter.h"
#include "AES.h"

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

#define MM2Pixel(W,nDPI)        (W*nDPI)/(25.4)

const int DPI = 300;
const int DPI_W = 600;
const int DPI_H = 300;
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
    CHAR szBuffer[0xFFFF] = {0};
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

TraceFnTime::TraceFnTime(char *szCmd,char *szReply,int nLine)
{
    lpCmd = szCmd;
    lpReply = szReply;
    nFileLine = nLine;
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
    sprintf(szCommand,"@Line:%d evolis_command(printer,\"%s\",%d,szReply,16):szReply = %s duration = %d .\n",nFileLine,lpCmd,strlen(lpCmd),lpReply,tSpan.count());
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

/*
 * 1Inch = 72 pt = 25.4mm
印刷系统中吧1英寸定义为72磅(即pt)
这样既可确立PT与毫米的转换关系
即：1pt = 25.2/72 = 0.3527777778≈0.3528
*/
float Pt2MM(float pt)
{
    return pt*0.3528;
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

int  mat2Binary(const  cv::Mat img, int  line_byte, char* data)
{
    int  width = img.cols;
    int  height = img.rows;
    size_t  line_size = line_byte * 8;
    size_t  bit_size = line_size * height;

    char* p = data;  int  offset, v; unsigned  char  temp;
    for (int row = height - 1; row >= 0; row--)
    {
        for (int col = 0; col < width; col++)
        {
            offset = col % 8;
            v = img.at<uchar>(row, col);
            temp = 1;
            temp = temp << (8 - offset - 1);
            if (v == 255)
            {
                *(p + col / 8) |= temp;
            }
            else
            {
                temp = ~temp;
                *(p + col / 8) &= temp;
            }
        }
        p = p + line_byte;
    }
    return  0;
}

struct RGBQUAD {
        uchar    rgbBlue;
        uchar    rgbGreen;
        uchar    rgbRed;
        uchar    rgbReserved;
} ;

#pragma push(pack)
#pragma pack(2)
struct BITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
};

struct BITMAPINFOHEADER{
        DWORD      biSize;
        int        biWidth;
        int        biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        int        biXPelsPerMeter;
        int        biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
};
#pragma pop(pack)

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

//将Mat图像保存为1位bmp图
int  SaveMonoImage(const  cv::Mat imgRGB, std::string dst)
{
    int  width = imgRGB.cols;
    int  height = imgRGB.rows;
    const  int  biBitCount = 1;
    Mat img_Gray;

    cv::threshold(imgRGB, img_Gray, 10, 255, cv::THRESH_BINARY);

    //待存储图像数据每行字节数为4的倍数，计算位图每行占多少个字节
    //int  line_byte = (width * biBitCount >> 3 + 3) / 4 * 4;
    int nLineByte = width / 8;
    int line_byte = nLineByte % 4 == 0 ? nLineByte : (nLineByte / 4 + 1) * 4;
    //int  line_byte = (width * biBitCount / 8 ) / 4 * 4;//+3改为+5,否则无法向上对齐，造成崩溃
    char* p_data = (char*)malloc(line_byte * height + 1024);//后面加1否则会报heap错误
    //memset(p_data, 0x01, line_byte * height+1);

    //将mat数组转换为二进制，255用1表示，0用0表示
    mat2Binary(img_Gray, line_byte, p_data);

    //bmp位图颜色表大小，以字节为单位，灰度图像颜色表为256*4字节，彩色图像颜色表大小为0,二值图为2*4
    int  color_type_num = 2;
    int  colorTablesize = color_type_num * sizeof(RGBQUAD);
    RGBQUAD* pColorTable = new  RGBQUAD[color_type_num];
    for (int i = 0; i < color_type_num; i++) {
        pColorTable[i].rgbBlue       = (uchar)(i * 255);
        pColorTable[i].rgbRed        = (uchar)(i * 255);
        pColorTable[i].rgbGreen      = (uchar)(i * 255);
        pColorTable[i].rgbReserved   = (uchar)0;
    }
    //申请位图文件头结构变量，填写文件头信息
    BITMAPFILEHEADER fileHead;
    fileHead.bfType = 0x4D42;   //bmp类型
    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + line_byte * height;   //bfSize是图像文件4个组成部分之和,图文件的大小，以字节为单位
    fileHead.bfReserved1 = 0;//位图文件保留字，必须为0
    fileHead.bfReserved2 = 0;
    fileHead.bfOffBits = 54 + colorTablesize;               //bfOffBits是图像文件前3个部分所需空间之和,位图数据的起始位置

    //申请位图信息头结构变量，填写信息头信息
    BITMAPINFOHEADER head;
    head.biBitCount = biBitCount;//每个像素所需的位数，必须是1（双色），（29-30字节）4(16色），8(256色）16(高彩色)或24（真彩色）之一
    head.biCompression = BI_RGB; //位图压缩类型, 必须为BI_RGB否则图片打开会有错误
    head.biHeight = height;//位图的高度，以像素为单位
    head.biWidth = width;
    //head.biSize = 40;
    head.biSize = sizeof(BITMAPINFOHEADER);//本结构所占用字节数
    head.biSizeImage = line_byte * height;//位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)
    head.biPlanes = 1;//目标设备的级别，必须为1
    head.biXPelsPerMeter = 23622;//位图水平分辨率，每米像素数，分辨率设为600
    head.biYPelsPerMeter = 23622;//位图垂直分辨率，每米像素数
    head.biClrImportant = 0;//位图显示过程中重要的颜色数
    head.biClrUsed = 0;//位图实际使用的颜色表中的颜色数

    RunlogF("sizeof (RGBQUAD) = %d" ,sizeof(RGBQUAD));
    RunlogF("sizeof (BITMAPINFOHEADER) = %d", sizeof(BITMAPINFOHEADER));
    RunlogF("sizeof (BITMAPFILEHEADER) = %d", sizeof(BITMAPFILEHEADER));
    RunlogF("colorTablesize = %d" , colorTablesize );

    try
    {
        RunlogF("Try to save file:%s",dst.c_str());
        FILE* fp = fopen(dst.c_str(), "wb");
        if (!fp)
        {
            return -1;
        }
        RunlogF("Try to Write Bitmap File header");
        fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);
        RunlogF("Try to Write Bitmap Info header");
        fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);
        RunlogF("Try to Write ColorTable header");
        fwrite(pColorTable, sizeof(RGBQUAD) * color_type_num, 1, fp);
        RunlogF("Try to Write data Height = %d,Linebyte = %d",height,line_byte);
        fwrite(p_data, height * line_byte, 1, fp);
        RunlogF("Try to close fp");
        fclose(fp);
    }

    catch (std::exception& e)
    {
        RunlogF("Catch a exceptoin:%s",e.what());
    }

    return  0;
}

bool QEvolisPrinter::LoadFontFromResources()
{
    return loadFontFamilyFromFiles(":/SIMSUN.ttf",strFontSimsun);
}

int _PhotoCompressParameters::Parser(string strJson,bool bInput)
{
    int nResult = -1;
    do
    {
        QByteArray baJson(strJson.c_str(), strJson.size());
        QJsonParseError jsErr;
        QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
        if (jsdoc.isNull())
        {
            RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
            break;
        }
        QJsonObject jsobj = jsdoc.object();
        if (!jsobj.contains("Width"))
        {
            RunlogF("Can't find 'Width' in %s!",strJsonFile.c_str());
            break;
        }
        if (!jsobj.contains("Height"))
        {
            RunlogF("Can't find 'Height' in %s!",strJsonFile.c_str());
            break;
        }
        if (!jsobj.contains("Size"))
        {
            RunlogF("Can't find 'Size' in %s!",strJsonFile.c_str());
            break;
        }
        nWidth = jsobj.value("Width").toInt();
        nHeight = jsobj.value("Height").toInt();
        nSize = jsobj.value("Size").toInt();
        if (bInput)
            Save(strJson);
        nResult = 0;
    }while (0);
    return nResult;
}

int _PhotoCompressParameters::Save(string &strJson)
{
//    try
//    {
        QFileInfo fi(strJsonFile.c_str());
        if (fi.isFile())
            QFile::remove(strJsonFile.c_str());
        QFile qFile(strJsonFile.c_str());
        if (!qFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
             RunlogF("Failed in open file %s.",strJsonFile.c_str());
             return -1;
        }
        RunlogF("Try to Save PhotoCompressParameters to file %s.",strJsonFile.c_str());
        qFile.write(strJson.c_str(),strJson.size());
        qFile.close();
        return 0;
//    }
//    catch (std::exception &e)
//    {
//        RunlogF("Catch a exception:%s", e.what());
//    }
}

_PhotoCompressParameters::_PhotoCompressParameters() noexcept
{
    nWidth = 150;
    nHeight = 180;
    nSize = 4096;

    try
    {
        strJsonFile = "./PhotoCompressParameters.json";
        QFileInfo PhotoParam(strJsonFile.c_str());
        if (PhotoParam.isFile())
        {
            ifstream ifs(strJsonFile,ios::binary|ios::in);
            stringstream ss;
            ss << ifs.rdbuf();
            if (!Parser(ss.str()))
            {
                return ;
            }
            RunlogF("Width = %d\tHeight = %d\tSize = %d.", nWidth,nHeight,nSize);
        }
    }
    catch (std::exception& e)
    {
        RunlogF("Catch a exception:%s.", e.what());
    }
}

QEvolisPrinter::QEvolisPrinter()
{
    Funclog();
    string strInfo = cv::getBuildInformation();
    qDebug()<<"OpenCV BuildInfo:";
    qDebug()<<strInfo.c_str();
    QString strLib = QDir::currentPath();
    strLib += "/libevolis.so";
    //strLib += "libKT-R85.so";
    pHandle = dlopen(strLib.toUtf8().data(),RTLD_NOW|RTLD_GLOBAL);
    if (!pHandle)
    {
        qDebug()<<"Failed in load "<<strLib;
        qDebug()<<"Error:"<<strerror(errno);
        return ;
    }
    pevolis_log_set_console  = GetProcAddr(pHandle, evolis_log_set_console );
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
    pfnevolis_commandt       = GetProcAddr(pHandle, evolis_commandt        );
    pevolis_set_card_pos     = GetProcAddr(pHandle, evolis_set_card_pos    );
    pevolis_print_init       = GetProcAddr(pHandle, evolis_print_init      );
    pevolis_reset            = GetProcAddr(pHandle, evolis_reset           );
    pfnevolis_print_set_option = GetProcAddr(pHandle, evolis_print_set_option);
    pevolis_print_get_option = GetProcAddr(pHandle, evolis_print_get_option);
    pevolis_print_export_options= GetProcAddr(pHandle, evolis_print_export_options);
    pevolis_print_set_imagep = GetProcAddr(pHandle, evolis_print_set_imagep);
    pevolis_status           = GetProcAddr(pHandle, evolis_status          );
    pevolis_print_exec       = GetProcAddr(pHandle, evolis_print_exec      );
    pevolis_get_devices      = GetProcAddr(pHandle, evolis_get_devices     );
    pevolis_get_mark_name    = GetProcAddr(pHandle, evolis_get_mark_name   );
    pevolis_get_model_name   = GetProcAddr(pHandle, evolis_get_model_name  );
    pevolis_print_set_imageb = GetProcAddr(pHandle, evolis_print_set_imageb);
    pevolis_print_set_imagep = GetProcAddr(pHandle, evolis_print_set_imagep);
    pevolis_print_set_blackp = GetProcAddr(pHandle, evolis_print_set_blackp);
    auto tTime = std::chrono::system_clock::to_time_t(chrono::system_clock::now());
    struct tm* ptm = localtime(&tTime);

}

void QEvolisPrinter::LoadEvolisSettings()
{
    QString strCurrentPath = QDir::currentPath();
    strCurrentPath += "/Evolis_Z390.ini";
    QSettings Setting(strCurrentPath,QSettings::IniFormat);
    Setting.beginGroup("EvolisOptions");
    QStringList strKeyList = Setting.allKeys();
    for(auto var:strKeyList)
    {
        m_mapEvolisSetting[var.toStdString()] = Setting.value(var).toString().toStdString();
        qDebug()<<var <<" = "<<m_mapEvolisSetting[var.toStdString()].c_str();
    }
    Setting.endGroup();
}

void QEvolisPrinter::InitEvolisSettings()
{
    if (!m_pPrinter)
    {
        RunlogF("Printer is not opened!");
        return;
    }
    for (auto var:m_mapEvolisSetting)
    {
        pevolis_print_set_option(m_pPrinter,var.first.c_str(),var.second.c_str());
    }

//    m_mapEvolisSetting["IGMonochromeSpeed"]  = "VAL10";         // 0~10
//    m_mapEvolisSetting["IGSendIQLA"]         = "ON";            // ON~OFF
//    m_mapEvolisSetting["IGIQLABY"]           = "VAL10";         // 0~20
//    m_mapEvolisSetting["IGIQLACY"]           = "VAL10";         // 0~20
//    m_mapEvolisSetting["IGIQLABM"]           = "VAL10";         // 0~10
//    m_mapEvolisSetting["IGIQLACM"]           = "VAL10";         // 0~10
//    m_mapEvolisSetting["IGIQLABC"]           = "VAL10";         // 0~10
//    m_mapEvolisSetting["IGIQLACC"]           = "VAL10";         // 0~10
//    //m_mapEvolisSetting["BBlackManagement"]   = "";//"TEXTINBLACK";   // NOBLACKPOINT,ALLBLACKPOINT,TEXTINBLACK
//    m_mapEvolisSetting["FBlackManagement"]   = "BMPVARNISH";    // NOBLACKPOINT,ALLBLACKPOINT,TEXTINBLACK
//    //m_mapEvolisSetting["BColorBrightness"]   = "";//"VAL10";         // 0~20
//    m_mapEvolisSetting["FColorBrightness"]   = "VAL10";         // 0~20
//    //m_mapEvolisSetting["BColorContrast"]     = "";//"VAL10";         // 0~20
//    m_mapEvolisSetting["FColorContrast"]     = "VAL10";         // 0~20
//    //m_mapEvolisSetting["BHalftoning"]        = "";//"VAL10";      // THRESHOLD,FLOYD,DITHERING,CLUSTERED_DITHERING
//    //m_mapEvolisSetting["FHalftoning"]        = "";//"VAL10";      // THRESHOLD,FLOYD,DITHERING,CLUSTERED_DITHERING
//    m_mapEvolisSetting["FMonochromeContrast"]= "VAL10";        // 0~20
//    //m_mapEvolisSetting["BMonochromeContrast"]= "";//"VAL10";        // 0~20
//    m_mapEvolisSetting["FOverlayContrast"]   = "";//"VAL10";         // 0~20
//    //m_mapEvolisSetting["BOverlayContrast"]   = "";//"VAL10";         // 0~20
//    m_mapEvolisSetting["FOverlayManagement"]   = "BMPVARNISH";    // NOVARNISH,FULLVARNISH,BMPVARNISH
//    //m_mapEvolisSetting["BOverlayManagement"] = "";//"BMPVARNISH";    // NOVARNISH,FULLVARNISH,BMPVARNISH
//    //m_mapEvolisSetting["FPageRotate180"]     = "";//"OFF";           // ON,OFF
//    //m_mapEvolisSetting["BPageRotate180"]     = "";//"OFF";           // ON,OFF
//    m_mapEvolisSetting["GHighQualityMode"]     = "ON";            // ON,OFF
//    //m_mapEvolisSetting["GInputTray"]         = "";//"FEEDER";      // PRINTER,FEEDER,AUTO,BEZEL
//    //m_mapEvolisSetting["GOutputTray"]        = "";//"BEZEL";       // PRINTER,HOPPER,REAR,BEZEL,REJECT
//    //m_mapEvolisSetting["GRejectBox"]         = "";//"REJECT";      // PRINTER,DEFAULTREJECT,HOPPER,REJECT
//    ///m_mapEvolisSetting["Duplex"]             = "";//"NONE";          // NONE,HORIZONTAL
//    //m_mapEvolisSetting["GDuplexType"]        = "";//"DUPLEX_MM";     // DUPLEX_CC,DUPLEX_CM,DUPLEX_MC,DUPLEX_MM
//    m_mapEvolisSetting["GRibbonType"]        = "RC_YMCKO";      // RC_YMCKO,RC_YMCKOS,RC_YMCKOK,RC_YMCKOKOS,RM_KO,RM_KBLACK,RM_KWHITE,RM_KRED,RM_KGREEN,RM_KBLUE,RM_KSCRATCH,RM_KMETALSILVER,RM_KMETALGOLD,RM_KSIGNATURE,RM_KWAX,RM_KPREMIUM,RM_HOLO
//    m_mapEvolisSetting["GSmoothing"]         = "ADVSMOOTH";     // STDSMOOTH,ADVSMOOTH,NOSMOOTH
//    //m_mapEvolisSetting["IBBlackLevelValue"]  = "";//"253";           // 1,255
//    m_mapEvolisSetting["IFBlackLevelValue"]  = "253";           // 1,255
//    //m_mapEvolisSetting["IBTextRegion"]       = "";//"TEXT";          // BLOB
//    //m_mapEvolisSetting["IFTextRegion"]       = "";//"TEXT";          // BLOB
//    //m_mapEvolisSetting["IFOverlayDefaultContent"]= "";//"BLOB";      // BLOB
//    //m_mapEvolisSetting["IBOverlayDefaultContent"]= "";//"BLOB";      // BLOB
//    //m_mapEvolisSetting["IBOverlayCustom"]    = strOverlay;
//    m_mapEvolisSetting["IFOverlayCustom"]    =  strOverlayer;
//    //m_mapEvolisSetting["IBThresholdValue"]   = "";//"128";           // 1~255
//    //m_mapEvolisSetting["IFThresholdValue"]   = "";//"128";           // 1~255
//    //m_mapEvolisSetting["IBDarkLevelValue"]   = "";//"200";           // 0~255
//    m_mapEvolisSetting["IFDarkLevelValue"]   = "200";           // 0~255
//    //m_mapEvolisSetting["IFBlackCustom"]      = "";//"TEXT";          // TEXT
//    //m_mapEvolisSetting["IBBlackCustom"]      = "";//"TEXT";          // TEXT
//    m_mapEvolisSetting["Resolution"]         = "DPI300";               // DPI300,DPI600300,DPI120030
//    QString strSettingPath = QDir::currentPath();
//    strSettingPath += "/EvolisOptions.txt";
//    if (!pevolis_print_export_options(m_pPrinter,strSettingPath.toStdString().c_str(),'|'))
//    {
//        RunlogF("Succeed inpevolis_print_export_options");
//    }
//    for (auto var:m_mapEvolisSetting)
//    {
//        RunlogF("%s = %s",var.first.c_str(),pevolis_print_get_option(m_pPrinter,var.first.c_str()));
//    }
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
    char szReply[64] = {0};
    auto var = "Rzp";
    unsigned char szZone1[64] = {0};
    unsigned char szZone2[64] = {0};
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
        RunlogF("Failed in evolis_command(%s)\n",var);
        strcpy(pszRcCode,"0006");
        bFault = true;
        return 1;
    }

    string strReply = szReply;
    std::transform(strReply.begin(),strReply.end(),strReply.begin(),::toupper);
    if (strReply != (const char *)szZone1  &&
        strReply != (const char *)szZone2)
    {
        RunlogF("Failed in match printer serial number.\n");
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
    char szReply[64] = {0};
    auto var = "Rrt;zone";

    unsigned char szZone1[64] = {0};
    unsigned char szZone2[64] = {0};
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

char *g_strEvolisLevel[] =
{
        "EVOLIS_LG_TRACE",     //!< Show ERROR, WARNING, INFO, DEBUG TRACE messages.
        "EVOLIS_LG_DEBUG",    //!< Show ERROR, WARNING, INFO, DEBUG messages.
        "EVOLIS_LG_INFO",     //!< Show ERROR, WARNING, INFO messages.
        "EVOLIS_LG_WARNING",  //!< Show ERROR, WARNING messages.
        "EVOLIS_LG_ERROR"    //!< Show ERROR messages.
};

void QEvolisPrinter::CreateEvolislog()
{
    if (g_nEvolis_logLevel >= 0 && g_nEvolis_logLevel <= 4)
    {
        RunlogF("Prepare for Evolis log,Logperiod = %d\tEnalbeEvolislog = %d\tEvolis_loglevel = %s!\n",g_nLogPeriod,g_bEnalbeEvolislog,g_strEvolisLevel[g_nEvolis_logLevel]);
    }
    else
    {
        RunlogF("Prepare for Evolis log,Logperiod = %d\tEnalbeEvolislog = %d\tEvolis_loglevel = %d!\n",g_nLogPeriod,g_bEnalbeEvolislog,g_nEvolis_logLevel);
    }
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
       RunlogF("evolis_log_set_path(%s)",szFileName);
       pevolis_log_set_console(true);
       pevolis_log_set_path(szFileName);
       if (g_nEvolis_logLevel < 0 || g_nEvolis_logLevel > 4)
           g_nEvolis_logLevel = 0;
       RunlogF("evolis_log_set_level(%d)",g_nEvolis_logLevel);
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
    InitEvolisSettings();
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

//   const char *szResolution = pevolis_print_get_option(m_pPrinter,"Resolution");
//   RunlogF("Current Resolution:%s.",szResolution);

   char szReply[64] = {0};
   const char *szCmd[]={
                        "Pem;2",       // 将打印机自动纠错复位禁用，完全由上位程序发送指令
                        "Pcim;F",      // 从卡箱进卡
                        "Pcem;I",      // 从出卡口出卡
                        "Psmgr;2",     // discard for the error no -22 of evolis_print_exec
                        "Pneab;E",     // 打印结束后不出卡
                        "Pcrm;D",      // 设定出卡箱为废卡箱
                        //"Pbc;D;30",   // Eject card timeout 30s
                        "Pbc;A;D",      // disable timeout action
                        //"Pbc;A;N",    // if timeout reinsert card to Printer
                        //"Pbc;A;R"     // if timeout eject card to the eject box
                        "Pfs;700;660;1200;500;1700"//decrease the speed of depense
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
    char szSN[128] = {0};
    if (GetSN(szSN))
    {
        strcpy(pszRcCode,"0006");
        RunlogF("Failed in GetSN.\n");
        return 1;
    }
    SaveSN(szSN);

    //RunlogF("%s Succeed.\n",__PRETTY_FUNCTION__);
    strcpy(pszRcCode, "0000");
    return 0;
}

bool IsNumString(char *pString)
{
    if (!pString || !*pString)
        return false;
    while (*pString)
    {
        if (!isdigit(*pString++))
        {
            return false;
        }
    }
    return true;
}

int  QEvolisPrinter::SaveSN(char *szSN)
{
    string strIni = "/etc/str.conf";
    /*FILE *fp = fopen(szCnfPath,"a");
    if  (!fp)
    {
         RunlogF("Failed in fopen %s.\n",szCnfPath);
         return 1;
    }
    else
        fclose(fp);*/

    CSimpleIniA Setting;
    QFileInfo fi(strIni.c_str());
    bool bCreateNewFile = false;
    string strSN;
    if (fi.isFile())
    {
        if (Setting.LoadFile(strIni.c_str()) != SI_OK)
        {
            RunlogF("Failed in loading file %s.\n",strIni.c_str());
            return 1;
        }
        strSN = Setting.GetValue("set","sn");
    }
    else
        bCreateNewFile = true;

    if (bCreateNewFile ||strSN != szSN)
    {
        Setting.SetValue("set","sn",szSN);
        if (Setting.SaveFile(strIni.c_str()) != SI_OK)
        {
            RunlogF("Failed in writing file %s.\n",strIni.c_str());
            return 1;
        }
        else
        {
             RunlogF("Succeed in writing file %s.\n",strIni.c_str());
        }
    }

    return 0;
}

int  QEvolisPrinter::GetSN(char *szSN)
{
    if (!szSN)
    {
        return 1;
    }
    char pszRcCode[128] = {0};
    CheckPrinter(m_pPrinter);
    const char* szRSN = "Rsn";
    char szReply[128] = {0};
    if (pevolis_command(m_pPrinter,szRSN,strlen(szRSN),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szRSN);
        strcpy(pszRcCode,"0006");
        bFault = true;
        return 1;
    }
    strcpy(szSN,szReply);
    return 0;
}

int  QEvolisPrinter::GetRibbonStatus(char *pszRcCode)
{
    CheckPrinter(m_pPrinter);

    char szReply[64] = {0};
    auto szQueryCapcity = "Rrt;qty";
    auto tNow = high_resolution_clock::now();
    int nLoop = 0;
    do
    {
        if (pevolis_command(m_pPrinter,szQueryCapcity,strlen(szQueryCapcity),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szQueryCapcity);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        nLoop ++;
        // the evolis_command(Rrt;qty) may return an invalid result "OK",if the reply is "OK",try it send the command again
        if (strcmp(szReply,"ERR RIB DETECTION") == 0)
        {
            break;
        }
        else
        {
            QString strReply = szReply;
            QString strTrimed = strReply.trimmed();
            if (!IsNumString((char *)strTrimed.toStdString().c_str()) && nLoop < 3)
            {
                 RunlogF("Error respond of in evolis_command(%s).\n",szQueryCapcity);
                this_thread::sleep_for(chrono::milliseconds(100));
                continue;
            }
        }
        break;
    }while(true);


    if (!strcmp(szReply,"ERR RIB DETECTION"))
    {
        ribbon.capacity = 0;
        ribbon.remaining = 0;
        strcpy(pszRcCode,"0000");
        return 0;
    }
    ribbon.capacity = strtol(szReply,nullptr,10);
    auto szQueryCount = "Rrt;count";
    nLoop = 0;
    do
    {
        if (pevolis_command(m_pPrinter,szQueryCount,strlen(szQueryCount),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szQueryCount);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        nLoop ++;
        // the evolis_command(Rrt;qty) may return an invalid result "OK",if the reply is "OK",try it send the command again
        if (strcmp(szReply,"ERR RIB DETECTION") == 0)
        {
            break;
        }
        else
        {
            QString strReply = szReply;
            QString strTrimed = strReply.trimmed();
            if (!IsNumString((char *)strTrimed.toStdString().c_str()) && nLoop < 3)
            {
                RunlogF("Error respond of in evolis_command(%s).\n",szQueryCount);
                this_thread::sleep_for(chrono::milliseconds(100));
                continue;
            }
        }
        break;
    }while (true);

    if (!strcmp(szReply,"ERR RIB DETECTION"))
    {
        ribbon.capacity = 0;
        ribbon.remaining = 0;
        strcpy(pszRcCode,"0000");
        return 0;
    }
    ribbon.remaining = strtol(szReply,nullptr,10);
    if (ribbon.remaining > ribbon.capacity)
        ribbon.remaining = ribbon.capacity;
//    auto tDuration = duration_cast<milliseconds>(high_resolution_clock::now() - tNow);
//    RunlogF("Duration of szRibbincmd = %d!\n",tDuration.count());
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
    char szReply[64];
    if (bHardwareReset)
    {
//        if (pevolis_reset(m_pPrinter,lTimeout,szTimeouted) != 0)
//        {
//            strcpy(pszRcCode, "0001");
//            return 1;
//        }
        auto tNow = high_resolution_clock::now();
        char *szReset = "Srs";
        if (pevolis_command(m_pPrinter,szReset,strlen(szReset),szReply,sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szReset);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        this_thread::sleep_for(chrono::seconds(5));
        int nRetry = 3;
        do
        {
            if (pevolis_command(m_pPrinter,"Rok",3,szReply,sizeof(szReply)) < 0)
            {
                nRetry--;
            }
            else
                break;
        }while (nRetry > 0);
        auto tDuration = high_resolution_clock::now() - tNow;
        RunlogF("Time duration:%d",tDuration.count());

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
            if (CheckCardPostion(&nCardPos,pszRcCode,Internal))
                return 1;
            if (nCardPos != Pos_Non)
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
    char szReply[128] = {0};

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
        char* var[] ={"Pneab;A;1",  "Se"};
        int Res = pevolis_command(m_pPrinter, var[0],strlen(var[0]), szReply,sizeof(szReply));
        if (Res < 0)
        {
            RunlogF("evolis_command(%s)failed:%d.\n",var[0],Res);
            strcpy(pszRcCode, "0006");
            bFault = true;
            return 1;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
        auto tStart = chrono::high_resolution_clock::now();
        Res = pevolis_commandt(m_pPrinter, var[1],strlen(var[1]), szReply,sizeof(szReply),60*1000);
        if (Res < 0)
        {
            RunlogF("evolis_commandt(%s)failed:%d.\n",var[1],Res);
            strcpy(pszRcCode, "0006");
            bFault = true;
            return 1;
        }

        auto tDuration =  chrono::duration_cast<seconds>(chrono::high_resolution_clock::now() - tStart);
        if (CheckCardPostion(&nCardPos,pszRcCode,Internal))
            return 1;
        if (tDuration.count() >= 15)
        {// Eject card timeout and reinsert card to printer
            char *szCommand[] = {"Pcrm;D",    // 设定出卡箱为废卡箱
                                 "Ser" };     // 设置为废卡
            for(auto var:szCommand)
            {
                Res = pevolis_command(m_pPrinter, var,strlen(var), szReply,sizeof(szReply));
                if (Res < 0)
                {
                    RunlogF("evolis_command(%s)failed:%d.\n",var[0],Res);
                    strcpy(pszRcCode, "0006");
                    bFault = true;
                    return 1;
                }
            }
            strcpy(pszRcCode,"Retract");
            return 0;
        }
        else
        {// card is removed
            if (nCardPos != Pos_Non)
            {
                strcpy(pszRcCode, "0016");
                bFault = true;
                return 1;
            }
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
    char szReply[64] = {0};
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
                         //"Psmgr;2",     // 防止进卡和出卡阻塞
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
    char szReply[64] = {0};

    const char *szCommand[]={
                         //"Psmgr;2",     // 防止进卡和出卡阻塞
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
//        char szReply[64] = {0};
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

    char szReply[64] = {0};
    int nCardPos = 0;
    if (bCheckCard)
    {
        if (CheckCardPostion(&nCardPos,pszRcCode))
        {
            return 1;
        }
        if (nCardPos == Pos_Non)
        {
            strcpy(pszRcCode, "0017");
            return 1;
        }
        else if (nCardPos == Pos_Contact ||
                 nCardPos == Pos_Contactless ||
                 nCardPos == Pos_Print)
        {
            strcpy(pszRcCode, "0015");
            return 1;
        }
    }

    char* szCommand[] = {"Sib","Sis"};
//    if (pReader->GetVender() == Vender::Minhua)
//        szCommand[1] = "Sic";
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
          if (nCardPos == Pos_Bezel)
          {
              strcpy(pszRcCode, "0016");
              bFault = true;
              return 1;
          }
          else if (nCardPos == Pos_Non)
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
    char szReply[64] = {0};
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

// if there is no card bExistCard = false,else bExistCard = true
int QEvolisPrinter::CheckCardBox(bool &bExistCard,char* pszRcCode)
{
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

    if (szReply1[0] == '0')
    {
        strcpy(pszRcCode, "0009");
        bExistCard = false;
    }
    else
        bExistCard = true;
    return 0;
}
// nDispPos 发卡位置:1-发卡到读磁位置；2-发卡到接触IC卡位置;3-发卡到非接IC卡位置;4-发卡到打印位置
int  QEvolisPrinter::Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
{
    Funclog();
    CheckPrinter(m_pPrinter);

    if (nDispPos <1 || nDispPos > 4)
    {
        strcpy(pszRcCode, "0005");
        return 1;
    }

    int nCardPos = 0;
    if (CheckCardPostion(&nCardPos,pszRcCode))
    {// nCardPos 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位
        return 1;
    }
    if (nCardPos == Pos_Non)       // only if there is nocard in the printer ,it's needed to check the cardbox
    {
        bool bExistCard = false;
        if (CheckCardBox(bExistCard,pszRcCode))
            return 1;
        if (!bExistCard)
        {
            strcpy(pszRcCode,"0009");
            return 1;
        }
    }
    else if (nCardPos == Pos_Bezel)
    {// only test if the card is at bezel
        strcpy(pszRcCode, "0015");
        return 1;
    }
    else if ((nCardPos == nDispPos) ||
             (nCardPos == Pos_Print && nDispPos == Pos_Mr))
    {
        strcpy(pszRcCode, "0000");
        return 0;
    }

    const char *szCmdArray[] = {"Siscan","Sis","Sic","Si"};
    char szReply[64] = {0};
    int nIdx = nDispPos -1;
    if (pevolis_command(m_pPrinter,szCmdArray[nIdx],strlen(szCmdArray[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szCmdArray[nIdx],szReply);
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }

    if (strcmp(szReply,"OK") != 0 )
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szCmdArray[nIdx],szReply);
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }

    if (CheckCardPostion(&nCardPos,pszRcCode,Internal))
    {// nCardPos 0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位
        return 1;
    }

    if ((nCardPos != nDispPos) && !(nDispPos ==Pos_Mr && nCardPos == Pos_Print))
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
    char szReply[3][64] = {0};
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
            char szReply[64] = {0};
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
    char szReply[64] = { 0 };
    int nRes = 1;

    nRes = pevolis_command(m_pPrinter,pCommand, strlen(pCommand), szReply, sizeof(szReply));
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

int QEvolisPrinter::MoveCard(CardPostion nDstPos,bool bCheckPos)
{
    Funclog();
    if (!m_pPrinter)
    {
        RunlogF("Printer connection is invalid.\n");
        return 1;
    }
    char szReply[64] = { 0 };
    if (bCheckPos)
    {
        if (pevolis_command(m_pPrinter,"Rlr;p", 5, szReply, sizeof(szReply)) < 0)
        {
            RunlogF("Failed in evolis_command(Rlr;p).");
            bFault = true;
            return 1;
        }
        if ((nDstPos == Pos_Contact && strcmp(szReply,"SMART CARD") == 0)||
            (nDstPos == Pos_Contactless && strcmp(szReply,"CONTACTLESS CARD") == 0))
            return 0;
    }

    const char* pCommand[2] = {"Sis",       // 卡片移动到接触位
                              "Sic"};
    int nIdx = 0;
    if (nDstPos == Pos_Contactless)
        nIdx = 1;

    if (pevolis_command(m_pPrinter,pCommand[nIdx], strlen(pCommand[nIdx]), szReply, sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",pCommand[nIdx]);
        bFault = true;
        return 1;
    }
    pevolis_command(m_pPrinter,"Rlr;p", 5, szReply, sizeof(szReply));
    RunlogF("Card Postition:%s.\n",szReply);

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
        strcpy(pszRcCode, "0000");
        return  0;
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

    char szReply[64] = {0};

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
        //RunlogF("evolis_command(Check Printer Internal) Succeed:%s.\n",szReply);
        char szReply2[32] = {0};
        nIdx ++;
        if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply2,sizeof(szReply2)) < 0)
        {
            RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
            strcpy(pszRcCode,"0006");
            bFault = true;
            return 1;
        }
        //RunlogF("evolis_command(Check Bezel) Succeed:%s.\n",szReply);
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
        RunlogF("evolis_command(Check Printer Internal) Succeed :%s.\n",szReply);
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
    else
    {
        pReader->SetCardSlot(*Media);
        RunlogF("SetCardSlot %d",*Media);
    }
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
    char szReply[64] = {0};
    //设备上盖开后：evolis_command(Rse;o) 返回:0,00 Volts
    //设备上盖闭合：evolis_command(Rse;f) 返回:5,00 Volts
    if (pevolis_command(m_pPrinter,szDetectCover,strlen(szDetectCover),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Faild in evolis_command(%s),Reply:%s",szDetectCover,szReply);
        strcpy(pszRcCode, "0006");
        bFault = true;
        return 1;
    }
    if (szReply[0] == '0')
    {
        bCoverOpened = true;
        RunlogF("The cover is opened");
        if (GetRibbonStatus(pszRcCode))
            return 1;
    }
    else if (bCoverOpened)
    {
        bCoverOpened = false;
        RunlogF("The cover is closed");
        if (GetRibbonStatus(pszRcCode))
            return 1;
    }
    if (ribbon.capacity == 0 ||
        ribbon.remaining == 0)
    {
        RunlogF("ribbon.capacity = %d\tribbon.remaining = %d.",ribbon.capacity,ribbon.remaining);
    }
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
        //for (int i = 0;i < EVOLIS_STATUS_EX_COUNT;i ++)
        //    RunlogF("evolis_status return exts[%d] = %d.\n",i, es.exts[i]);
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

#define FailedCode(x)  strcpy(pszRcCode,x);return 1;

int QEvolisPrinter::SetPrinterOptions(evolis_t* printer,string strDPI,string strOverlayer)
{
//    if (strDPI == "DPI300300")
//        pevolis_print_set_option(printer, "IFTextRegion", "0x0x686x477");
//    else
//        pevolis_print_set_option(printer, "IFTextRegion", "0x0x1372x954");

    pevolis_print_set_option(printer, "FColorBrightness", "VAL10");
    pevolis_print_set_option(printer, "FColorContrast", "VAL10");
    pevolis_print_set_option(printer, "FOverlayContrast", "VAL10");
    pevolis_print_set_option(printer, "GRibbonType", "RC_YMCKO");
    pevolis_print_set_option(printer, "FOverlayManagement", "BMPVARNISH");
    pevolis_print_set_option(printer, "IFOverlayCustom",strOverlayer.c_str());
    pevolis_print_set_option(printer, "IFBlackLevelValue", "40");
    pevolis_print_set_option(printer, "IFDarkLevelValue", "100");
    pevolis_print_set_option(printer, "FMonochromeContrast", "VAL10");
    pevolis_print_set_option(printer, "IGMonochromeSpeed", "VAL10");
    pevolis_print_set_option(printer, "GSmoothing", "ADVSMOOTH");
    pevolis_print_set_option(printer, "IGIQLABC", "VAL10");
    pevolis_print_set_option(printer, "IGIQLABM", "VAL10");
    pevolis_print_set_option(printer, "IGIQLABY", "VAL10");
    pevolis_print_set_option(printer, "IGIQLACC", "VAL10");
    pevolis_print_set_option(printer, "IGIQLACM", "VAL10");
    pevolis_print_set_option(printer, "IGIQLACY", "VAL10");
    return 0;
}

int ReadJpeg(string strFile,string &strBuffer,int &nWidth,int &nHeight)
{
    tjhandle tjHandle = nullptr;
    char *pRGBBuff = nullptr;
    int nBufferSize = 0;
    try
    {
        bool bResult = false;
        bool  bRepairEnd = false;
        tjHandle =tjInitDecompress();
        string strFileBuffer;
        ifstream ifs(strFile,ios::binary|ios::in);
        int nFileSize = 0;
        do
        {
            if (!tjHandle)
                break;

            if (!nFileSize)
            {
                ifs.seekg(0,ios::end);
                nFileSize = ifs.tellg();
                if (!nFileSize)
                    break;
                strFileBuffer.resize(nFileSize + 1);
                ifs.seekg(0,ios::beg);
                ifs.read(&strFileBuffer[0],strFileBuffer.length() - 1);
            }
            RunlogF("File %s Tail:%02X %02X.\n",strFile.c_str(),(UCHAR)strFileBuffer[nFileSize - 2],(UCHAR)strFileBuffer[nFileSize - 1]);

            if (bRepairEnd)
            {
                if (strFileBuffer[nFileSize - 1] == 0xFF)
                {
                    RunlogF("Repair jpeg Tail of file:%s.\n",strFile.c_str());
                    strFileBuffer[nFileSize] = 0xD9;
                    nFileSize ++;
                }
            }

            int subsample,colorspace;
            if (tjDecompressHeader3(tjHandle,(unsigned char *)strFileBuffer.c_str(),(unsigned long)nFileSize, &nWidth, &nHeight,&subsample,&colorspace))
            {
                qDebug() << "Failed in tjDecompressHeader3" << tjGetErrorStr();
                break;
            }

            int nPixelFormat = TJPF_BGR;
//            tjscalingfactor &factor =*scalingFactor;
//            int scaledHeight = TJSCALED(nHeight,factor);
//            int scaledWidth = TJSCALED(nWidth,factor);
//            int nPitch = scaledWidth * tjPixelSize[TJPF_BGR];
            if (!pRGBBuff)
            {
                nBufferSize = nWidth*nHeight*tjPixelSize[nPixelFormat];
                pRGBBuff = new char[nBufferSize + 1];
            }

            if (tjDecompress2(tjHandle,(unsigned char *)strFileBuffer.c_str(),(unsigned long)nFileSize,(unsigned char *)pRGBBuff,0,0,nHeight,nPixelFormat,0))
            {
                string strErrorEnd = "Premature end of JPEG file";
                qDebug() << "Failed in tjDecompress2,errCode" << tjGetErrorCode(tjHandle)<<",Error string:"<< tjGetErrorStr();
                if (strErrorEnd ==  tjGetErrorStr())
                {
                    bRepairEnd = true;
                    continue;
                }
                else
                    break;
            }
            bResult = true;
        }while(!bResult) ;
        if (tjHandle)
            tjDestroy(tjHandle);
        if (bResult)
        {
            strBuffer.assign(pRGBBuff,nBufferSize);
            return 0;
        }
        else
            return -1;
    }
    catch(std::exception &e)
    {
        qDebug() <<e.what();
        return -1;
    }
}

int WriteJpeg(string strFile,uchar *strBuffer,int nWidth,int nHeight,int nQuality)
{
    if (!strBuffer)
    {
        RunlogF("Failed in writing jpeg,the buffer is emplty!")
        return -1;
    }
    QFileInfo fi(strFile.c_str());
    if (fi.isFile())
    {
        RunlogF("Remove file %s!",strFile.c_str());
        QFile::remove(strFile.c_str());
    }
    else if (fi.isDir())
    {
        RunlogF("The file %s is a directory,please remove or rename it to another name!",strFile.c_str());
        return false;
    }
    tjhandle compressor = tjInitCompress();
    if (!compressor)
    {
        RunlogF("Failed in tjInitCompress,the buffer is empty!")
        return -1;
    }
    int nPitch = tjPixelSize[TJPF_BGR]*nWidth;
    unsigned long nCompressDataSize = 0;
    unsigned char *szCompressedData =   nullptr;
    /*
     int tjCompress2(tjhandle handle, const unsigned char *srcBuf,
                          int width, int pitch, int height, int pixelFormat,
                          unsigned char **jpegBuf, unsigned long *jpegSize,
                          int jpegSubsamp, int jpegQual, int flags);
    */

    bool bSucceed = false;
    do
    {

        int nStatus = tjCompress2(compressor,(const unsigned char *)strBuffer,
                                       nWidth,nPitch,nHeight,TJPF_BGR,
                                       &szCompressedData,&nCompressDataSize,
                                       TJSAMP_444,nQuality,TJFLAG_ACCURATEDCT);
        if (nStatus != 0)
        {
            RunlogF("Failed in compress JPEG!");
            break;
        }
        QFile jpegFile(strFile.c_str());
        if (!jpegFile.open(QIODevice::WriteOnly))
        {
            RunlogF("Failed in creating file %s!",strFile.c_str());
            break;
        }
        if (jpegFile.write((char *)szCompressedData,nCompressDataSize) == -1)
        {
            RunlogF("Failed in writing daa into file %s!",strFile.c_str());
            break;
        }
        jpegFile.close();
        bSucceed = true;
    } while(0) ;
    tjDestroy(compressor);
    return bSucceed;
}

int QEvolisPrinter::MakeImage(PICINFO& inPicInfo, list<TextInfoPtr>& inTextVector,string &strImagePath,string &strTextPath,char *pszRcCode,float fScale,float fScale2)
{
    string strJpegBuffer;
    int nPicWidth = 0,nPicHeight = 0;
    if (!inPicInfo.picpath.size())
    {
        RunlogF("Header Image:%s can't be empty!.\n",inPicInfo.picpath.c_str());
        FailedCode("0013");
    }
    QFileInfo fi(inPicInfo.picpath.c_str());
    if (!fi.isFile())
    {
        RunlogF("Can't locate Header image:%s.\n",inPicInfo.picpath.c_str());
        FailedCode("0013");
    }
    RunlogF("Try to load file %s.",inPicInfo.picpath.c_str());
    string strPicFile = inPicInfo.picpath;
    std::transform(strPicFile.begin(),strPicFile.end(),strPicFile.begin(),::toupper);

    Mat HeaderImage(nPicHeight,nPicWidth,CV_8UC3);
//    Mat HeaderImage = cv::imread(strPicFile);
//    if (HeaderImage.empty())
//    {
//        RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
//        FailedCode("0013");
//    }

    if (strPicFile.find("JPG") != string::npos ||
        strPicFile.find("JPEG") != string::npos)
    {
        RunlogF("Load a jpeg file :%s.\n",inPicInfo.picpath.c_str());
        if (ReadJpeg(inPicInfo.picpath,strJpegBuffer,nPicWidth,nPicHeight))
        {
            RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
            FailedCode("0013");
        }
        HeaderImage.create(nPicHeight,nPicWidth,CV_8UC3);
        if (HeaderImage.data)
            memcpy(HeaderImage.data,strJpegBuffer.c_str(),strJpegBuffer.size());
        else
        {
            RunlogF("Insufficent memory in load file :%s.\n",inPicInfo.picpath.c_str());
            FailedCode("0013");
        }
    }
    else
    {
        RunlogF("Load a bmp file :%s.\n",inPicInfo.picpath.c_str());
        HeaderImage = imread(inPicInfo.picpath, IMREAD_ANYCOLOR);
        if (!HeaderImage.data)
        {
             RunlogF("Failed in load file :%s.\n",inPicInfo.picpath.c_str());
             FailedCode("0013");
        }
    }

    Mat canvasImage(nCardHeight*fScale,nCardWidth*fScale,CV_8UC3,Scalar(255,255,255));
    Mat HeaderPrint;
    resize(HeaderImage, HeaderPrint, Size(MM2Pixel(inPicInfo.fWidth*fScale,nDPI_W) , MM2Pixel(inPicInfo.fHeight*fScale,nDPI_H)), 0, 0, INTER_NEAREST);
    Mat HeaderROI = canvasImage(Rect(MM2Pixel(inPicInfo.fxPos*fScale,nDPI_W),MM2Pixel(inPicInfo.fyPos*fScale,nDPI_H),MM2Pixel(inPicInfo.fWidth*fScale,nDPI_W),MM2Pixel(inPicInfo.fHeight*fScale,nDPI_H)));
    HeaderPrint.copyTo(HeaderROI);
    if (inPicInfo.nAngle > 0)
    {
        //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
        flip(canvasImage, canvasImage, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
        flip(canvasImage, canvasImage, 1);
    }
    QString strImageFile = QDir::currentPath();
    QString strTextFile = strImageFile;
    strImageFile += "/PrintImage.bmp";
    imwrite(strImageFile.toStdString(),canvasImage);

    string strFontName ;
    Mat canvasText(nCardHeight*fScale,nCardWidth*fScale,CV_8UC3,Scalar(255,255,255));
    for (auto var : inTextVector)
    {
        //RunlogF("Text = %s,FontSize = %d,xPos = %.02f,yPos = %.2f.\n",var->sText.c_str(),var->nFontSize);
        Rect rtROI(MM2Pixel(var->fxPos*fScale,nDPI_W), MM2Pixel(var->fyPos*fScale,nDPI_H),nCardWidth*fScale - MM2Pixel(var->fxPos*fScale,nDPI_W) - 2, nCardHeight*fScale - MM2Pixel(var->fyPos*fScale,nDPI_H) - 2);
        strFontName = var->pFontName;
        Mat FontROI = canvasText(rtROI);
        int fontHeight = (int)round(MM2Pixel(Pt2MM(var->nFontSize*fScale),nDPI_H));
        try
        {
            cv::Ptr<cv::freetype::FreeType2> ft2;
            ft2 = cv::freetype::createFreeType2();

            ft2->loadFontData(var->pFontName, 0); //加载字库文件
            if (var->nFontStyle == Style_Bold)
                RunlogF("Print '%s' with bold Font.\n", var->sText.c_str());
            ft2->putText(FontROI, var->sText.c_str(), cv::Point(0, 0), fontHeight, CV_RGB(0, 0, 0), -1, CV_AA, false,var->nFontStyle == Style_Bold);
        }
        catch (std::exception &e)
        {
            RunlogF("Catch a exception:%s.\n",e.what());
            FailedCode("0018");
        }
    }

    string strMark ="";
    if (bMarkDateTime)
    {
        QDateTime tNow = QDateTime::currentDateTime();
        strMark = tNow.toString("yyyy-MM-dd hh:mm:ss").toStdString();
    }
    if (bMarkDPI)
    {
        string strDPI =  "DPI300300";
        if (strResolution.size() != 0)
            strDPI = strResolution;
        strMark += "  ";
        strMark += strDPI;
    }

    if(strMark.size())
    {
        try
        {
            cv::Ptr<cv::freetype::FreeType2> ft2;
            ft2 = cv::freetype::createFreeType2();
            ft2->loadFontData(strFontName, 0); //加载字库文件
            string strDPI = "DPI300300";
            if (strResolution.size() != 0)
                strDPI = strResolution;
            int nFontSize = 6;
            bool bBold = false;
            int nTextHeight = (int)round(MM2Pixel(Pt2MM(nFontSize*fScale),nDPI_H));
            int nBaseline = 0;
            Size TextSize = ft2->getTextSize(strMark,nTextHeight,0,&nBaseline);
            int nStartX = nCardWidth*fScale - TextSize.width - MM2Pixel(1,nDPI_W);
            int nStartY = nCardHeight*fScale - MM2Pixel(1,nDPI_H) - nTextHeight;
            Rect rtROI(nStartX, nStartY,TextSize.width,nTextHeight);
            Mat FontROI = canvasText(rtROI);

            ft2->putText(FontROI,strMark.c_str(), cv::Point(0, 0), nTextHeight, CV_RGB(0, 0, 0), -1, CV_AA, false,bBold);
        }
        catch (std::exception &e)
        {
            RunlogF("Catch a exception:%s.\n",e.what());
            FailedCode("0018");
        }
    }

    if (inPicInfo.nAngle > 0)
    {
       flip(canvasText, canvasText, 0);// 翻转模式，flipCode == 0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
       flip(canvasText, canvasText, 1);
    }

    QString strTempFile = QDir::currentPath();
    strTempFile +="/PrintTemp.bmp";
    imwrite(strTempFile.toStdString(),canvasText);

    strTextFile += "/PrintText.bmp";
    SaveMonoBmp(canvasText,strTextFile.toUtf8().data());


//        Point2f srcTriagle[3],dstTriangle[3];
//        cv::Mat dstImage;
//        cv::Mat warpMat(2,3,CV_32FC1);
//        dstImage = cv::Mat::zeros(canvas.rows*fScale2,canvas.cols*fScale2,canvas.type());
//        srcTriagle[0] = Point2f(0,0);
//        srcTriagle[1] = Point2f(canvas.cols - 1,0);
//        srcTriagle[2] = Point2f(0,canvas.rows - 1);

//        dstTriangle[0] = Point2f(0,0);
//        dstTriangle[1] = Point2f(canvas.cols*fScale2,0);
//        dstTriangle[2] = Point2f(0,canvas.rows*fScale2);
//        warpMat = getAffineTransform(srcTriagle,dstTriangle);
//        warpAffine(canvas,dstImage,warpMat,dstImage.size());
//        if (strPreviewFile.size())
//            imwrite(strPreviewFile.c_str(),dstImage);
//        imwrite(strTempFile.toUtf8().data(),dstImage);

    strTextPath = strTextFile.toStdString();
    strImagePath = strImageFile.toStdString();
    return 0;
}

int  QEvolisPrinter::SaveMonoBmp(cv::Mat& canvas,const char* strFile)
{
    Mat CardGray,CardMono;
    cv::cvtColor(canvas, CardGray, cv::COLOR_BGR2GRAY); //灰度化源图像，便于我们观察结果
    adaptiveThreshold(CardGray, CardMono, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 511, 1);
    SaveMonoImage(CardMono, strFile);
    //RunlogF("Succeed in save mono file.");
    return 0;
}

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
        FailedCode("0022");
    }

    int res = CheckRibbonZone(pszRcCode);
    if (res == 1 ||
        res == 2)
    {
        return 1;
    }

    string strImagePath,strTextPath;
    if (MakeImage(inPicInfo,inTextVector,strImagePath,strTextPath,pszRcCode,1.0,1.0))
    {
        return 1;
    }
    RunlogF("Image Path = %s",strImagePath.c_str());

    if (pevolis_print_init((evolis_t*)m_pPrinter))
    {
        RunlogF("evolis_print_init failed.\n");
        FailedCode("0006");
         bFault = true;
    }

    const char *szCmd[]={"Rrt;count","Pneab;A;0","Pneab;A;1"};
    char szReply[64] = {0};
    int nIdx = 0;
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s).\n",szCmd[nIdx]);
        bFault = true;
        FailedCode("0006");
    }
    int nRibbonCount = strtol(szReply,nullptr,10);
    if (nRibbonCount <= 0)
    {
        RunlogF("Run out of ribbon and print failed:%s.\n",szReply);
        FailedCode("0021");
    }
    nIdx++;
    if (pevolis_command(m_pPrinter,szCmd[nIdx],strlen(szCmd[nIdx]),szReply,sizeof(szReply)) < 0)
    {
        RunlogF("Failed in evolis_command(%s):%s.\n",szCmd[nIdx]);
        bFault = true;
        FailedCode("0006");
    }
    nIdx++;
    LoadEvolisSettings();
    //RunlogF("pevolis_print_set_option(Resolution,%s).\n",strResolution.c_str());
    //pevolis_print_set_option(m_pPrinter, "Resolution", strResolution.c_str());
    InitEvolisSettings();
    if (pevolis_print_set_imagep(m_pPrinter, EVOLIS_FA_FRONT,strImagePath.c_str()))
    {
        RunlogF("evolis_print_set_imagep failed:%s.\n",strImagePath.c_str());
        bFault = true;
        FailedCode("0006");
    }
    if (pevolis_print_set_blackp(m_pPrinter, EVOLIS_FA_FRONT,strTextPath.c_str()))
    {
        RunlogF("evolis_print_set_imagep failed:%s.\n",strTextPath.c_str());
        bFault = true;
        FailedCode("0006");
    }

    strOverlayer = QDir::currentPath().toStdString();
    strOverlayer += "/resources/iso.bmp";
    QFileInfo fi(strOverlayer.c_str());
    if (!fi.isFile())
    {
        RunlogF("Can't open file %s.\n",strOverlayer.c_str());
        FailedCode("0013");
    }

    auto tSpan = duration_cast<milliseconds>( high_resolution_clock::now() - tStart);
    //RunlogF("Prepare print duration:%d.",tSpan.count());
    //SetPrinterOptions(m_pPrinter,strResolution,strOverlayer);
    evolis_status_t es;
    RunlogF("Try to evolis_print_exec.\n");
    int nStatus = pevolis_print_exec((evolis_t*)m_pPrinter);
    if (nStatus != 0)
    {
        RunlogF("evolis_print_exec failed:%d.\n",nStatus);
        if (pevolis_status(m_pPrinter, &es) != 0)
        {
            RunlogF("Error reading printer status\n");
            bFault = true;
            FailedCode("0006");
        }
        RunlogF("Printer Status:\n\tconfig = %d\n\tinformation = %d.\n\twarning = %d\n\terror = %d\n\texts[0] = %d\n\texts[1] = %d\n\texts[2] = %d\n\texts[3] = %d\n",
                es.config,es.information,es.warning,es.error,es.exts[0],es.exts[1],es.exts[2],es.exts[3]);
        FailedCode("0006");
    }
    int             printed = 0;    

    auto tLast = high_resolution_clock::now();
    while (!printed)
    {
        auto tNow = high_resolution_clock::now();
        if (pevolis_status(m_pPrinter, &es) != 0)
        {
            RunlogF("Error reading printer status\n");
            bFault = true;
            FailedCode("0006");
        }
        auto tDuration = duration_cast<milliseconds>(high_resolution_clock::now() - tNow);
        RunlogF("pevolis_status duration %d ms\n",tDuration.count());
        printed = !(es.information & EVOLIS_INF_BUSY);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    this_thread::sleep_for(chrono::milliseconds(200));
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

     if (pevolis_commandt(m_pPrinter,lpCmdIn,strlen(lpCmdIn),szEvolisReply,sizeof(szEvolisReply),60*1000) < 0)
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
