﻿#include <string>
#include <string.h>
#include <cstdio>
#include <ctime>
#include <thread>
#include "./tinyxml/tinyxml.h"
#include "./tinyxml/tables.h"
#include "./tinyxml/ZBase64.h"
#include <QRectF>
#include <QRect>
#include <string>
#include <memory>
#include <QFileInfo>
#include <chrono>
#include <thread>
#include <sstream>
#include <map>
#include "evolis_z390_printer.h"
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <QDateTime>
#include <QFile>
#include <QStringList>
#include <QIODevice>
#include <QDirIterator>
#include <QSettings>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonParseError>
#include <QImage>
#include "./json/cJSON.h"
#include "./json/CJsonObject.hpp"


using namespace std;

map<string,list<myFontInfo>> map_Font;

using namespace std;

using namespace chrono;

const int DPI = 300;
QFile *pFilelog = nullptr;
bool g_bEnableOutput = true;
bool g_bEnableLog = true;
bool g_bEnalbeEvolislog = true;
int  g_nLogPeriod = 30;
int g_nEvolis_logLevel = 0;

const char* g_szCardPosition[] = {
        "Pos_Non",
        "Pos_Mr",
        "Pos_Contact",
        "Pos_Contactless",
        "Pos_Print",
        "Pos_Bezel",
        "Pos_Blocked",
        "Pos_OutofEntrance",
        "Pos_Unknow"
};
//#define RunlogF(...)    Runlog(__PRETTY_FUNCTION__,__LINE__,__VA_ARGS__);
//#define Funclog()       //FuncRunlog(__PRETTY_FUNCTION__,__LINE__);

//#define     SetAndroidEnv()

#define     OpenPrinter()   evolis_device_t* devices;\
                            if (evolis_get_devices(&devices, 0, 0) <= 0)\
                                 return 1;   \
                            m_pPrinter =  evolis_open(L"Evolis Zenius", false);\
                            if (!m_pPrinter) {\
                                strcpy(pszRcCode, "0001");\
                                return 1;  }\
                            shared_ptr<evolis_t> pPrinerRecycle(m_pPrinter,evolis_close);

#define     CheckPrinter(x)  if (!x) {\
                                strcpy(pszRcCode, "0003");\
                                return 1;  }

#define     CheckResult(x)  if (!x){\
                                strcpy(pszRcCode, "0020");\
                                return 1;}

#define     CheckResult2(x)  if (!x){\
                                RunlogF("msg = %s.",msg.c_str());\
                                return 1;};\
                                RunlogF("msg = %s.",msg.c_str());

#define TOHEXA(a, b) {*b++ = chHexTableA[a >> 4]; *b++ = chHexTableA[a & 0xf];}
// 功能描述		内存数据转换为16进制ASCII字符串
// pHex			输入数据流
// nHexLen		输入数据流长度
// szAscString	输出16进制ASCII字符串缓冲区
// nBuffLen		输出缓冲区最大长度

// 返回值		<0时 输入参数不合法
//				>0 返回转换后的ASCII符串的长度
int Bin2Hexstring(unsigned char* pHex, int nHexLen, char* szAscString, int nBuffLen)
{
    static const  char chHexTableA[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
    if (!pHex ||
        !szAscString ||
        !nBuffLen)
        return -1;
    unsigned char nMult = 2;
    CHAR chSeperator = '\0';
//    if (chSeperator == '\0')
//        nMult = 2;
    if (nHexLen * nMult > nBuffLen)
        return -1;
    CHAR* p = &szAscString[0];

    int n;
    for (n = 0; n < nHexLen; n++)
    {
        TOHEXA(pHex[n], p);
        if (nMult == 2)
            continue;
        *p++ = chSeperator;
    }
    return n * nMult;
}


extern "C"
{
//    void  _init()
//    {
//    }
//    void  _finit()
//    {

//    }

#include <iconv.h>


    int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
    {
        iconv_t cd;
        int rc;
        char **pin = &inbuf;
        char **pout = &outbuf;

        cd = iconv_open(to_charset,from_charset);
        if (cd==0)
            return -1;
        if (iconv(cd,pin,&inlen,pout,&outlen)==-1)
            return -1;
        iconv_close(cd);
        return 0;
    }

    // utf8码转为GB2312码
    int UTF8_GB2312(char *inbuf,int inlen,char *outbuf,int outlen)
    {
        return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
    }

    // GB2312码转为utf8
    int GB2312_UTF8(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
    {
        return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
    }

    // utf8转为gbk
    string Utf8_GBK(const char *inbuf)
    {
        int inlen=strlen(inbuf);
        string strRet;
        strRet.resize(inlen*2+2);
        if(code_convert("utf-8","gbk",const_cast<char *>(inbuf),inlen,&strRet[0],strRet.size()))
            return inbuf;
        return strRet;
    }

    // gbk转为utf8
    string GBK_Utf8(const char *inbuf)
    {
        int inlen=strlen(inbuf);
        string strRet;
        strRet.resize(inlen*2+2);
        if(code_convert("gbk","utf-8",const_cast<char *>(inbuf),inlen,&strRet[0],strRet.size()))
            return inbuf;
        return strRet;
    }

    void ClearLog(QString strPath,int nDays = 30)
    {
        QStringList filters;
        filters << "*.log";
        QDir dir;
        dir.setPath(strPath);
        dir.setNameFilters(filters);
        QDirIterator iter(dir,QDirIterator::Subdirectories);
        while(iter.hasNext())
        {
            iter.next();
            QFileInfo info = iter.fileInfo();
            if (info.isFile() &&
                info.created().addDays(nDays) <= QDateTime::currentDateTime())
            {
                QFile fileTemp(info.absoluteFilePath());
                fileTemp.remove();
            }
        }
    }


    LPVOID  CreateInstance(LPVOID lpReserve)
    {
        Q_UNUSED(lpReserve);
        QString strCurrentPath = QDir::currentPath();

        strCurrentPath += "/Evolis_Z390.ini";
        QSettings Setting(strCurrentPath,QSettings::IniFormat);
        if (Setting.contains("Config/LogPeriod"))
            g_nLogPeriod = Setting.value("Config/LogPeriod").toInt();
        if (g_nLogPeriod > 90 || g_nLogPeriod < 0)
            g_nLogPeriod = 30;
        if (Setting.contains("Config/Evolis_log"))
            g_bEnalbeEvolislog = Setting.value("Config/Evolis_log").toBool();
        if (Setting.contains("Config/Evolis_loglevel"))
            g_nEvolis_logLevel = Setting.value("Config/Evolis_loglevel").toInt();

        if (g_nLogPeriod)
        {
            strCurrentPath = QDir::currentPath();
            strCurrentPath += "/log";
            ClearLog(strCurrentPath,g_nLogPeriod);
            if (!pFilelog)
            {
                char szFileName[2048] = {0};
                char szFileName2[2048] = {0};
                QDateTime tNow = QDateTime::currentDateTime();

                QDir dir(strCurrentPath);
                if (!dir.exists())
                    dir.mkpath(strCurrentPath);
                sprintf(szFileName,"%s/Z390_%04d%02d%02d.log",strCurrentPath.toUtf8().data(),
                                tNow.date().year(),
                                tNow.date().month(),
                                tNow.date().day());
//                                tNow.time().hour(),
//                                tNow.time().minute(),
//                                tNow.time().second());
//                sprintf(szFileName2,"%s/Evolis_%04d%02d%02d_%02d%02d%02d.log",strCurrentPath.toUtf8().data(),
//                                tNow.date().year(),
//                                tNow.date().month(),
//                                tNow.date().day(),
//                                tNow.time().hour(),
//                                tNow.time().minute(),
//                                tNow.time().second());
                pFilelog = new QFile(szFileName);
                if (pFilelog)
                { /*
                        enum OpenModeFlag {
                        NotOpen = 0x0000,
                        ReadOnly = 0x0001,
                        WriteOnly = 0x0002,
                        ReadWrite = ReadOnly | WriteOnly,
                        Append = 0x0004,
                        Truncate = 0x0008,
                        Text = 0x0010,
                        Unbuffered = 0x0020
                    };
                    */

                   pFilelog->open(QIODevice::Append|QIODevice::Text);
                   pFilelog->write("Try to allocate Evolis_Z390_Printer!");
                }
            }
        }

        return  (void *)new Evolis_Z390_Printer();
    }
    void   FreeInstance(LPVOID lpDev)
    {
        if (lpDev)
        {
            delete (Evolis_Z390_Printer*)lpDev;
        }
        if (pFilelog)
            pFilelog->write("Try to Deallocate Evolis_Z390_Printer!");
        delete pFilelog;
        pFilelog = nullptr;
    }

    //system函数扩展，加入超时值(0表示永久等待)
    //超时时返回-2，其他情况返回不变。
    int system_ex(const char *cmdstring, unsigned int timeout)   /* with appropriate signal handling */
    {
        pid_t               pid;
        int                 status;
        struct sigaction    ignore, saveintr, savequit;
        sigset_t            chldmask, savemask;

        //精度换成十分之一秒
        timeout *= 10;
        if (timeout == 0)
            timeout = 0xFFFFFFFF;

        if (cmdstring == NULL)
            return(1);      /* always a command processor with UNIX */

        ignore.sa_handler = SIG_IGN;    /* ignore SIGINT and SIGQUIT */
        sigemptyset(&ignore.sa_mask);
        ignore.sa_flags = 0;
        if (sigaction(SIGINT, &ignore, &saveintr) < 0)
            return(-1);
        if (sigaction(SIGQUIT, &ignore, &savequit) < 0)
            return(-1);
        sigemptyset(&chldmask);         /* now block SIGCHLD */
        sigaddset(&chldmask, SIGCHLD);
        if (sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0)
            return(-1);

        if ((pid = fork()) < 0) {
            return -1;    /* probably out of processes */
        } else if (pid == 0) {          /* child */
            /* restore previous signal actions & reset signal mask */
            sigaction(SIGINT, &saveintr, NULL);
            sigaction(SIGQUIT, &savequit, NULL);
            sigprocmask(SIG_SETMASK, &savemask, NULL);
            /*通常exec会放在fork() 函数的子进程部分, 来替代子进程执行,
             * 执行成功后子程序就会消失,  但是执行失败的话, 必须用exit()
             * 函数来让子进程退出!*/
            execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
            /*<strong>exec函数会取代执行它的进程,  也就是说, 一旦exec函数执行成功, 它就不会返回了, 进程结束.
             *但是如果exec函数执行失败, 它会返回失败的信息,  而且进程继续执行后面的代码!</strong>*/
            _exit(127);     /* exec error */
        }

        /* parent */
        int ret = 0;
        while (timeout-- > 0 &&
            (ret = waitpid(pid, &status, WNOHANG)) == 0)
            usleep(100*1000);

        /* restore previous signal actions & reset signal mask */
        if (sigaction(SIGINT, &saveintr, NULL) < 0)
            return(-1);
        if (sigaction(SIGQUIT, &savequit, NULL) < 0)
            return(-1);
        if (sigprocmask(SIG_SETMASK, &savemask, NULL) < 0)
            return(-1);

        if (ret < 0)
            return -1;

        if (ret > 0)
            return status;

        kill(pid, SIGKILL);
        waitpid(pid, &status, 0);
        return -2;
    }


    /**
     *@brief          执行系统命令，并返回输出的结果
     *@param[in]      cmdstring，命令串
     *@param[in]      buf，存放命令结果的缓冲区
     *@param[in]      size，缓冲区的大小
     *@param[out]
     *@return         返回写入到buf中的字符串长度，不含\0 ; -1: 失败;
     *@remark
     *@version        V1.0.0
     *@note           buf中最多返回size-1个字符，字符串始终以\0结尾。
    */
    int get_cmd_results(const char *cmdstring, char *pBuffer, int &nSize)
    {
        char cmd_string[200] = {0};
        char tmpfile[100] = {0};
        char tmp_buf[100] = {0};
        int fd;
        int tmp_fd;
        int nbytes;

        if((cmdstring == NULL) ||
            (strlen(cmdstring) > (sizeof(tmpfile) + 8)) ||
            ((strlen(cmdstring) + strlen(tmpfile) + 5) > sizeof(cmd_string)))
        {
            printf("cmd is too long or NULL!\n");
            return -1;
        }
        sscanf(cmdstring, "%[a-Z]", tmp_buf);/*%[a-z] 表示匹配a到z中任意字符，贪婪性(尽可能多的匹配) */
        sprintf(tmpfile, "/tmp/%s-XXXXXX", tmp_buf);

        tmp_fd = mkstemp(tmpfile);
        if(tmp_fd < 0)
        {
            printf("mkstemp failed\n");
            return -1;
        }
        close(tmp_fd);

        sprintf(cmd_string, "%s > %s 2>&1", cmdstring, tmpfile);/*标准输出（1），标准错误（2）都输出到临时文件*/
        if(system_ex(cmd_string, 20) < 0)
        {
            printf("run \"%s\" ret < 0!\n", cmd_string);
        }

        fd = open(tmpfile, O_RDONLY);
        if(fd < 0)
        {
            printf("open %s failed!\n", tmpfile);
            nbytes = -1;
        }
        else
        {
            int nFileSize = lseek(fd,0,SEEK_END);
            lseek(fd,0,SEEK_SET);
//            char *pBuffer = new char[nFileSize + 1];
//            if (!pBuffer)
//            {
//                close(fd);
//                return -1;
//            }
            nbytes = read(fd, pBuffer, nFileSize);
            pBuffer[nFileSize + 1] = 0;            
            nSize = nFileSize ;
            close(fd);
        }

        memset(cmd_string, 0, sizeof(cmd_string));
        sprintf(cmd_string, "rm -rf /tmp/%s-*", tmp_buf);
        system_ex(cmd_string, 20);

        return nbytes;
    }

    void trim(string &s)
    {
       if( !s.empty() )
       {
          s.erase(0,s.find_first_not_of(" "));
          s.erase(s.find_last_not_of(" ") + 1);
       }
    }

    void InitializeFont()
    {
        char szBuffer[0xFFFF] = {0};
        int nBufferSize = 0;

        if (get_cmd_results("/usr/bin/fc-list :lang=zh",szBuffer,nBufferSize) < 0)
        {
            qDebug()<<__PRETTY_FUNCTION__<<" Failed in initializing System font.";
            return ;
        }

        istringstream iss(szBuffer);
        string strLine;
        while (std::getline(iss,strLine))
        {
            char szFont[1024] = {0};
            char szName[1024] = {0};
            char szStyle[1024] = {0};
            int nCount = sscanf(strLine.c_str(),"%[^:]:%[^:]:%[^:]",szFont,szName,szStyle);
            if (nCount == 3)
            {
                char szSimpleName[128] = {0};
                char szSimpleStyle[32] = {0};
                sscanf(szName,"%[^,]",szSimpleName);
                sscanf(szStyle,"style=%s",szSimpleStyle);
                string strSimpleName = szSimpleName;
                string strSimpleStyle = szSimpleStyle;
                string strSimpleFont = szFont;
                trim(strSimpleName);
                trim(strSimpleStyle);
                trim(strSimpleFont);
                qDebug("Font = %s\tStyle = %s\tFontPath = %s.\n",strSimpleName.c_str(),strSimpleStyle.c_str(),strSimpleFont.c_str());
                auto itFind = map_Font.find(szSimpleName);
                if (itFind == map_Font.end())
                {
                    map_Font.insert(make_pair(strSimpleName,list<myFontInfo>({myFontInfo(strSimpleFont,strSimpleStyle)})));
                }
                else
                {
                    itFind->second.push_back({myFontInfo(strSimpleFont,strSimpleStyle)});
                }
            }
        }

    }
    Evolis_Z390_Printer::Evolis_Z390_Printer()
    {
        RunlogF("Prepare QEvolisPrinter.\n");
        pEvolisPriner = new QEvolisPrinter();
        pReader = CreateReader();
        if (pReader)
        {
            char szVer[32] = {0};
            pReader->GetDevVer((uchar *)szVer);
            RunlogF("Reader version:%s",szVer);
        }
        //InitializeFont();
    }

    Evolis_Z390_Printer::~Evolis_Z390_Printer()
    {
        Funclog();
        if (pReader)
        {
            pReader = nullptr;
        }
        if (cpOutMsg)
            delete []cpOutMsg;

        //pEvolisPriner->Exit();
        delete pEvolisPriner;
    }

    /** @ingroup CLithographPrinter Function declaration
      * @brief  打开设备
      * @param[in] pPort 端口号,U口设备：USB或usb；串口设备：COM+串口数，例如COM1；网口设备: IP
      * @param[in] pPortParam 端口参数，U口设备:  USB设备参数，VID\PID;串口设备：格式为波特率+奇偶检校位+起始位+终止位，例如：57600,n,8,1;网口设备：IP地址， 例如 172.0.0.1
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_Open(char* pPort, char* pPortParam, char* pszRcCode)
    {
        Funclog()
        if (!pReader)
        {
            strcpy(pszRcCode, "0001");  // 打开读卡器失败
            RunlogF("Failed in detect reader.\n");
            return 1;
        }
        else
            pReader->CloseDev();

        if (!pReader->OpenDev(nullptr,0,nullptr))
        {
            strcpy(pszRcCode, "0001");  // 打开读卡器失败
            string strError;
            pReader->GetErrorMsg(strError);
            RunlogF("Failed in OpenDev:%s.\n",strError.c_str());
            return 1;
        }

        if (pEvolisPriner)
        {
            if (pEvolisPriner->IsOpen())
            {
                strcpy(pszRcCode, "0002");  // 设备已经连接
                return 1;
            }
            else
            {
                int nResult = pEvolisPriner->Open(pPort,pPortParam,pszRcCode);
                if (nResult == 0)
                {
                    pEvolisPriner->pReader = pReader;
                    // Media 介质状态，0-无卡；1-卡在门口；2-卡在内部；3-卡在上电位，4-卡在闸门外；5-堵卡；6-卡片未知（根据硬件特性返回,必须支持有无卡检测）
                    // nCheckPos = 0,check printer and bezel,
                    // nCheckPos = 1 check printer
                    // nCheckPos = 2 check bezel
                    int nMedia = -1;
                    char szCode[16] = {0};
                    pEvolisPriner->CheckCardPostion(&nMedia,szCode);
                }
                return nResult;
            }
        }
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  关闭设备
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_Close(char* pszRcCode)
    {
        Funclog()
        if (pReader)
            pReader->CloseDev();

        if (pEvolisPriner)
            return pEvolisPriner->Close(pszRcCode);
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  复位设备
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] nResetAction 1-复位卡保持在设备内；2-复位退卡到打印机取卡位置；3-复位吞卡到打印机回收箱(若硬件支持需实现)
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_Reset(long lTimeout, int nResetAction, char* pszRcCode)
    {
        Funclog();
        if (pReader)
            pReader->Reset(lTimeout);
         if (pEvolisPriner)
             return pEvolisPriner->Reset(lTimeout,nResetAction,pszRcCode);
         else
         {
             strcpy(pszRcCode,"0001");
             return 1;
         }
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  获取固件版本以及动态库版本信息
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] pVersionInfo 固件版本信息
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_GetVersion(long lTimeout, char* pVersionInfo, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
		time_t rawtime;
		struct tm* ptminfo;
		time(&rawtime);
		ptminfo = localtime(&rawtime);
		sprintf(pVersionInfo,"Evolis_z390_V1.0.0.1_%02d%02d%02d",	ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday);
        strcpy(pszRcCode, "0000");
        return 0;
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  允许进卡
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] nCheckMode 是否检磁,0-不检磁；1-检磁(若支持需实现,否则参数忽略)
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
      * 暂不实现
     **/
    int Evolis_Z390_Printer::Print_EnableCard(long lTimeout, int nCheckMode, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->EnableCard2(lTimeout,nCheckMode,pszRcCode);
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  取消进卡
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
      * 暂不实现
     **/
    int Evolis_Z390_Printer::Print_DisableCard(long lTimeout, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->DisableCard(lTimeout,pszRcCode);
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  退卡到门口
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_Eject(long lTimeout, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->Eject(lTimeout,pszRcCode);
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  吞卡到打印机回收箱
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] nBoxNo 吞卡卡箱号
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_Retract(long lTimeout, int nBoxNo, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->Retract(lTimeout,nBoxNo,pszRcCode);
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  读磁条(若支持需实现)
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] wTrackId 需要读取的磁道,0x0001-读1磁；0x0002-读2磁；0x0004-读3磁,当需要读多磁条时,异或即可,譬如读1、2、3磁条,则传入0x0007
      * @param[out] pTrackInfo 读出的磁条数据,具体见结构体定义
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
      * 暂不实现
     **/
    int Evolis_Z390_Printer::Print_ReadTrack(long lTimeout, WORD wTrackId, Lithograph::LITHOGRAPHMAGINFO* pTrackInfo, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Q_UNUSED(wTrackId);
        Q_UNUSED(pTrackInfo);
        Q_UNUSED(pszRcCode);
        strcpy(pszRcCode,"0007");
        return 1;

    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  写磁条(若支持需实现)
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] wTrackId 需要写磁道,0x0001-读1磁；0x0002-读2磁；0x0004-读3磁,当需要写多磁条时,异或即可,譬如写1、2、3磁条,则传入0x0007
      * @param[in] pTrackInfo 需要写的磁条数据,结构体定义同读磁条接口
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
      * 暂不实现
     **/
    int Evolis_Z390_Printer::Print_WriteTrack(long lTimeout, WORD wTrackId, Lithograph::LITHOGRAPHMAGINFO* pTrackInfo, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Q_UNUSED(wTrackId);
        Q_UNUSED(pTrackInfo);
        Q_UNUSED(pszRcCode);
        strcpy(pszRcCode,"0007");
        return 1;
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  IC卡上电(若支持需实现)
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] byOutAtr ATR数据,HEX
      * @param[out] nAtrlen ATR数据长度
      * @param[out] byUID 当使用非接时需返回UID,否则返回空""
      * @param[out] nUidLen 当使用非接时返回UID长度,否则长度为0
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_IcPowerOn(long lTimeout, unsigned char* byOutAtr, int& nAtrlen, unsigned char* byUID, int& nUidLen, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Q_UNUSED(byUID);
        Q_UNUSED(nUidLen);
        Funclog();
        if (!pEvolisPriner)
        {
            RunlogF("Printer connection is invalid.\n");
            return 1;
        }

        if (pReader)
        {
            int ret = 0;
            char dataBuffer[1024] = { 0 };
            int nRes = -1;

            char szReply[64] = { 0 };
            if (pEvolisPriner->ppevolis_command(pEvolisPriner->m_pPrinter,"Rlr;p", 5, szReply, sizeof(szReply),__LINE__) < 0)
            {
                RunlogF("Failed in evolis_command(Rlr;p).");
                pEvolisPriner->bFault = true;
                return 1;
            }

            vector<CardPostion>vecPostoMove ;
            CardPostion nPowerPos = Pos_Unknow;
            CardPostion nNextPos = Pos_Unknow;
            if (strcmp(szReply,"OK") == 0 )
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            else if (strcmp(szReply,"SMART CARD") == 0)
            {
                nPowerPos = Pos_Contact;
                nNextPos = Pos_Contactless;
            }
            else if(strcmp(szReply,"CONTACTLESS CARD") == 0)
            {
                nPowerPos = Pos_Contactless;
                nNextPos = Pos_Contact;
            }
            else
            {
                if (pEvolisPriner->MoveCard(Pos_Contact),false)
                {
                    RunlogF("Failed in MoveCard %s.",g_szCardPosition[Pos_Contact]);
                    strcpy(pszRcCode,"0001");
                    return 1;
                }
                nPowerPos = Pos_Contact;
                nNextPos = Pos_Contactless;
            }
            pReader->SetCardSlot(nPowerPos);
            RunlogF("Try to PowerOn card on %s.\n",g_szCardPosition[nPowerPos]);
            nRes = pReader->PowerOn(dataBuffer,ret);
            if (nRes != 0)
            {
                RunlogF("Failed in PowerOn card on %s.\n",g_szCardPosition[nPowerPos]);
                RunlogF("Try to move card to %s.",g_szCardPosition[nNextPos]);
                if (pEvolisPriner->MoveCard((CardPostion)nNextPos,false))
                {
                    RunlogF("Failed in MoveCard %s.",g_szCardPosition[nNextPos]);
                    strcpy(pszRcCode,"0001");
                    return 1;
                }
                pReader->SetCardSlot(nNextPos);
                RunlogF("Try to PowerOn card on %s.\n",g_szCardPosition[nNextPos]);
                nRes = pReader->PowerOn(dataBuffer,ret);
                if (nRes != 0)
                {
                    RunlogF("Failed in PowerOn card on %s.",g_szCardPosition[nNextPos]);
                }
                else
                {
                    RunlogF("Succed in PowerOn card on %s.",g_szCardPosition[nNextPos]);
                }
            }
            else
            {
                RunlogF("Succed in PowerOn card on %s.",g_szCardPosition[nPowerPos]);
            }

            if (nRes)
            {
                strcpy(pszRcCode, "0019");
                string strError;
                pReader->GetErrorMsg(strError);
                RunlogF("PowerOn Failed:%s.\n",strError.c_str());
                return 1;
            }
            RunlogF("ATR Data:%s",dataBuffer);

            CardATR = dataBuffer;
            nAtrlen = CardATR.size();
            int nOffset = CardATR.size() - 26;
            m_CardInfo.ATR = CardATR.substr(nOffset,26);
            strcpy((char*)byOutAtr, dataBuffer);
            pReader->a_hex((unsigned char *)dataBuffer,(unsigned char *)byOutAtr,strlen(dataBuffer));
            nAtrlen /=2;
            RunlogF("ATR=%s",byOutAtr);
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else
        {
            strcpy(pszRcCode, "0001");
            return 1;
        }

    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  IC卡下电(若支持需实现)
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_IcPowerOff(long lTimeout, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Funclog();
        if (pReader)
            pReader->PowerOff();

        strcpy(pszRcCode, "0000");
        return 0;
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  IC卡数据交互(若支持需实现)
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] byIndata 输入指令HEX
      * @param[in] nInDataLen 输入指令长度
      * @param[out] pOutData 响应数据HEX
      * @param[out] nOutLen 响应数据长度
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_IcExchange(long lTimeout, uchar* byIndata, int nInDataLen, uchar* pOutData, int& nOutLen, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Funclog();
        if (!pReader)
        {
            strcpy(pszRcCode, "0003");
            return 1;
        }

        unsigned char szBuffer[256] = {0};
        Binary2Hexstring(byIndata,nInDataLen,szBuffer,256,0);
        RunlogF("byIndata[%d] = %s.",nInDataLen,szBuffer);
        uint nRecvLen = 0;
        RunlogF("Try to ApduInt(%s).\n",byIndata);
        int nRet = pReader->ApduInt((const uchar *)byIndata,nInDataLen, pOutData, nRecvLen);
        if (nRet < 0)
        {
            string strError;
            pReader->GetErrorMsg(strError);
            RunlogF("Failed in ApduInt:%s.\n",strError.c_str());
            strcpy(pszRcCode, "0019");
            return 1;
        }
        CHAR  szText[4096] = {0};

        nOutLen = Binary2Hexstring((unsigned char *)pOutData,nRecvLen,(unsigned char *)szText,4096,0);
        RunlogF("dc_cpuapduInt return %d bytes:%s.\n",nRecvLen,szText);
        nOutLen= nRecvLen;
        RunlogF("nOutLen =  %d OutData = %s.\n",nOutLen,pOutData);
		strcpy(pszRcCode, "0000");
        return 0;
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  从卡箱发卡(若支持需实现)
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] nBox 卡箱号
      * @param[in] nDispPos 发卡位置:1-发卡到读磁位置；2-发卡到接触IC卡位置;3-发卡到非接IC卡位置;4-发卡到打印位置
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_Dispense(long lTimeout, int nBox, int nDispPos, char* pszRcCode)
	{
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->Dispense(lTimeout,nBox,nDispPos,pszRcCode);
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    };

    /** @ingroup CLithographPrinter Function declaration
      * @brief  获取卡箱状态,包括发卡箱、回收箱、废卡箱等（若硬件支持需实现）
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] lpBoxInfo 卡箱状态信息结构体
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_GetBoxStatus(long lTimeout, Lithograph::LPLITHOGRAPHBOXINFO& lpBoxInfo, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->GetBoxStatus(lTimeout, lpBoxInfo,  pszRcCode);
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    };

    //   WORD fwDevice;		//打印机状态,   需要有0和1和2，0在线，1有卡在内部工作中，2离线,2021.06.09 13.21 胡杰 定义
    //   WORD fwMedia;		//介质状态，     0-无卡；2-卡在内部；2021.06.09 13.21 胡杰 定义
    //   WORD fwToner;		//平印色带状态,  0-FLLL;1-LOW，4-未知 2021.06.09 13.38 胡杰 定义

    /** @ingroup CLithographPrinter Function declaration
      * @brief  获取打印机状态
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] lpStatus 打印机信息结构体,见定义
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_Status(long lTimeout, Lithograph::LPLITHOGRAPHSTATUS& lpStatus, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->DevStatus(lTimeout, lpStatus,  pszRcCode);
        else
        {
            strcpy(pszRcCode,"0003");
            return 1;
        }
    };
    /** @ingroup CLithographPrinter Function declaration
      * @brief  初始化打印,在打印图片、文字、条码前调用
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_InitPrint(long lTimeout, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
            return pEvolisPriner->InitPrint(lTimeout, pszRcCode);
        else
        {
            strcpy(pszRcCode,"0003");
            return 1;
        }
    };
    /** @ingroup CLithographPrinter Function declaration
      * @brief  预打印图片
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] pImage 图片全路径
      * @param[in] nAngle 图片顺时针旋转角度
      * @param[in] fxPos 打印X坐标
      * @param[in] fyPos 打印Y坐标
      * @param[in] fHeight 打印高度
      * @param[in] fWidth 打印宽度
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_PrintImage(long lTimeout, char* pImage, int nAngle, float fxPos, float fyPos, float fHeight, float fWidth, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Funclog();
        if (!pEvolisPriner)
        {
             strcpy(pszRcCode,"0003");
             return 1;
        }
        if (fxPos < 0 || fyPos < 0 || fHeight <0 ||fWidth < 0)
        {
            strcpy(pszRcCode, "0005");
            return 1;
        }

        if ((fxPos + fWidth)> 85.6 || (fyPos + fWidth) >54.0)
        {
            strcpy(pszRcCode, "0005");
            return 1;
        }
        QFileInfo fi(pImage);
        if (!fi.exists())
        {
            strcpy(pszRcCode, "0013");
            return 1;
        }
        memset(&pEvolisPriner->m_picInfo, 0x00, sizeof(PICINFO));
        pEvolisPriner->m_picInfo.picpath = pImage;
        pEvolisPriner->m_picInfo.nAngle = nAngle;
        pEvolisPriner->m_picInfo.fxPos = fxPos;
        pEvolisPriner->m_picInfo.fyPos = fyPos;
        pEvolisPriner->m_picInfo.fHeight = fHeight;
        pEvolisPriner->m_picInfo.fWidth = fWidth;
        pEvolisPriner->m_picInfo.nType = 1;

        RunlogF("pImage = %s, nAngle = %d, fxPos = %f, fyPos = %f, fHeight  = %f, fWidth  = %f", pImage, nAngle, fxPos, fyPos, fHeight, fWidth);

        if (pEvolisPriner->m_picInfo.picpath == "")
		{
            strcpy(pszRcCode, "0005");
			return 1;
		}
		else
		{
			strcpy(pszRcCode, "0000");
			return 0;
		}
        return 0;
    };

    /** @ingroup CLithographPrinter Function declaration
      * @brief  预打印文字
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] pText 打印的内容
      * @param[in] nAngle 顺时针旋转角度,用于打印水平或竖直文字
      * @param[in] fxPos 打印X坐标
      * @param[in] fyPos 打印Y坐标
      * @param[in] pFontName 打印字体,例如"宋体"
      * @param[in] nFontSize 字体大小
      * @param[in] nFontStyle 字体风格：1-常规；2-粗体；4-斜体；8-黑体
      * @param[in] nColor 打印字体颜色,RGB颜色值
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
    int Evolis_Z390_Printer::Print_PrintText(long lTimeout, char* pText, int nAngle, float fxPos, float fyPos, char* pFontName, int nFontSize, int nFontStyle, int nColor, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Funclog();
        if (!pEvolisPriner)
        {
             strcpy(pszRcCode,"0003");
             return 1;
        }
        if (fxPos < 0 || fyPos < 0 )
        {
            strcpy(pszRcCode, "0005");
            return 1;
        }
//        pFontName = ":/Sumsun.ttf";
//        auto FontFind = map_Font.find(pFontName);
//        if (FontFind == map_Font.end())
//        {
//            RunlogF("There is no font match with %s.",pFontName);
//            strcpy(pszRcCode,"0014");
//            return 1;
//        }
//        char szFontStyle[16] = {0};
//        if (nFontStyle == 1)
//            strcpy(szFontStyle, "Regular");
//        else if (nFontStyle = 2)
//          strcpy(szFontStyle, "Bold");
//        else if (nFontStyle = 4)
//          strcpy(szFontStyle, "Italic");
//        else if (nFontStyle = 8)
//          strcpy(szFontStyle, "Black");
//        else
//        {
//            strcpy(pszRcCode,"0001");
//            return 1;
//        }
//        list<myFontInfo> &FontList = FontFind->second;
//        auto FontInfo = find_if(FontList.begin(),FontList.end(),[&](myFontInfo &info)
//        {
//            return (info.strStyle.find(szFontStyle) != string::npos);
//        });
//        if (FontInfo == FontList.end())
//        {
//            RunlogF("There is no font style match with %s.",pFontName);
//            strcpy(pszRcCode,"0014");
//            return 1;
//        }

        RunlogF("pText = %s, nAngle = %d, fxPos = %f, fyPos = %f, pFontName = %s,nFontSize  = %d, nFontStyle  = %d, nColor = %d", pText, nAngle, fxPos, fyPos, pFontName, nFontSize, nFontStyle, nColor);
        TextInfoPtr inTextInfo = std::make_shared<TextInfo>();

        char szUtf8[256] = {0};
        char szFont[256] = {0};

        GB2312_UTF8(pText,strlen(pText),szUtf8,256);
        GB2312_UTF8(pFontName,strlen(pFontName),szFont,256);
        QString strUTF8FontName = QString(szFont);
//        qDebug()<<"strUTFFont = "<< strUTF8FontName;   //output:"宋体\r"
//        QString strUTFFont1 = QString::fromLocal8Bit(szFont);
//        qDebug()<<"strUTFFont1 = "<< strUTFFont1; //output:"宋体\r"
//        QTextCodec *pUTF8 = QTextCodec::codecForName("UTF8");
//        QTextCodec *pGBK = QTextCodec::codecForName("GBK");
//        if (!pUTF8)
//        {
//            qDebug()<<"Failed in get code for UTF-8-BOM";
//            return 1;
//        }

//        if (!pGBK)
//        {
//            qDebug()<<"Failed in get code for GBK";
//            return 1;
//        }

//        QString strFontUnicode = pGBK->toUnicode(pFontName);
//        qDebug()<<"strFontUnicode = "<< strFontUnicode;   //output: "宋体\r"
//        QString strFontUTF8_BOM = pUTF8_BOM->fromUnicode(strFontUnicode);
//        qDebug()<<"strFontUTF8_BOM = "<< strFontUTF8_BOM;//output: "宋体\r"

        inTextInfo->sText = szUtf8;
        inTextInfo->nAngle = nAngle;
        inTextInfo->fxPos = fxPos;
        inTextInfo->fyPos = fyPos;

        QString strFontPath = QDir::currentPath();
        strFontPath += "/resources/";
        if (strUTF8FontName.contains("宋体"))
            strFontPath += "SIMSUN";
        else
            strFontPath += pFontName;
        strFontPath += ".ttf";
        QFileInfo fi(strFontPath);
        if (!fi.exists())
        {
            RunlogF("Can't find font file:%s",strFontPath.toLocal8Bit().data());
            strcpy(pszRcCode,"0014");
            return 1;
        }

        inTextInfo->pFontName =strFontPath.toStdString().c_str();
        RunlogF("Font = %s\t File = %s.",pFontName,inTextInfo->pFontName.c_str());
        inTextInfo->nFontSize = nFontSize;
        inTextInfo->nFontStyle = (FontStyle)nFontStyle;
        inTextInfo->nColor = nColor;
        inTextInfo->nType = 0;  //文字是0
        pEvolisPriner->m_textInfo.push_back(inTextInfo);

		strcpy(pszRcCode, "0000");
        return 0;
    };
    /** @ingroup CLithographPrinter Function declaration
      * @brief  预打印条码
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] nAngle 条码旋转角度,用于竖直或水平打印条码
      * @param[in] pBarcodeType 条码类型,譬如"c39"、"2/5"、"c128"、"c128c"、"ean13"等,若支持其他条码类型,请在说明文档中进行说明
      * @param[in] fxPos 打印X坐标
      * @param[in] fyPos 打印Y坐标
      * @param[in] nMultiplier 倍频系数
      * @param[in] nHeight 条码高度
      * @param[in] nIsActive 数据值激活,0不激活,1激活
      * @param[in] pBarcodeText 打印的条码数据
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
      * 暂不实现
     **/
    int Evolis_Z390_Printer::Print_PrintBarcode(long lTimeout, int nAngle, char* pBarcodeType, float fxPos, float fyPos, int nMultiplier, int nHeight, int nIsActive, char* pBarcodeText, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Q_UNUSED(pszRcCode);
        Q_UNUSED(nAngle);
        Q_UNUSED(pBarcodeType);
        Q_UNUSED(fxPos);
        Q_UNUSED(fyPos);
        Q_UNUSED(nMultiplier);
        Q_UNUSED(nHeight);
        Q_UNUSED(nIsActive);
        Q_UNUSED(pBarcodeText);
        strcpy(pszRcCode,"0007");

        return 1;
    };


   bool Evolis_Z390_Printer::RunApdu(string cmd,string &OutMsg,bool bCheckResult )
	{
        Funclog();
        char dataBuff[1024] = { 0 };
        OutMsg = "";
        if (!pReader)
            return false;

        uint nRecvLen = 0;
        RunlogF("Try to dc_cpuapduInt_hex(%s).\n",cmd.c_str());
        short nRet = pReader->ApduIntHex(cmd.c_str(),cmd.length() / 2, dataBuff, nRecvLen);
        if (nRet <0)
        {
            string strError;
            pReader->GetErrorMsg(strError);
            RunlogF("Failed in ApduIntHex(%s):%s(%d).\n",cmd.c_str(),strError.c_str(),nRet);
            return false;
        }
        RunlogF("dc_cpuapduInt_hex return %d bytes:%s.\n",nRecvLen,dataBuff);
        RunlogF("nRecvLen =  %d OutData = %s.\n",nRecvLen*2,dataBuff);
        if (nRecvLen)
            OutMsg = dataBuff;
        else
            return false;
        if (!bCheckResult)
       {
           RunlogF("Skip Result check.\n");
           return true;
       }
       if ("9000" != OutMsg.substr(OutMsg.length() - 4, 4))
       {
           RunlogF("The recieved data is invalid.\n");
           return false;
       }
        return true;
    }

    /** @ingroup CLithographPrinter Function declaration
      * @brief  提交任务,开始打印
      * @param[in] lTimeout 超时时间,单位ms
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/

    int Evolis_Z390_Printer::Print_StartPrint(long lTimeout, char* pszRcCode)
    {
        Funclog();
        if (pEvolisPriner)
        {
            if (0 == pEvolisPriner->StartPrint(lTimeout,  pszRcCode))
            {
                int ret = 0;
                char dataBuffer[1024] = { 0 };
                if (!pReader)
                {
                    RunlogF("The pReader is closed.");
                    strcpy(pszRcCode,"0001");
                    return 1;
                }
                bool bSucceed = false;
                for (int nPos = Pos_Contact;nPos <= Pos_Contactless;nPos ++)
                {
                    RunlogF("Try to move card to %s.",g_szCardPosition[nPos]);
                    if (pEvolisPriner->MoveCard((CardPostion)nPos),false)
                    {
                        RunlogF("Failed in MoveCard %s.",g_szCardPosition[nPos]);
                        strcpy(pszRcCode,"0001");
                        return 1;
                    }
                    pReader->SetCardSlot(nPos);
                    RunlogF("Try to PowerOn card on Pos_Contact.");
                    if (pReader->PowerOn(dataBuffer,ret))
                    {
                        RunlogF("Failed in PowerOn card on %s.",g_szCardPosition[nPos]);
                    }
                    else
                    {
                        bSucceed = true;
                        RunlogF("Succeed in PowerOn card on %s.",g_szCardPosition[nPos]);
                        break;
                    }
                }
                if (bSucceed)
                {
                    strcpy(pszRcCode,"0000");
                    return 0;
                }
                else
                {
                    strcpy(pszRcCode,"0001");
                    return 1;
                }
            }
        }
        else
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
    };
    /** @ingroup CLithographPrinter Function declaration
      * @brief  翻转卡片
      * @param[in] lTimeout 超时时间,单位ms
      * @param[in] nAngle 旋转角度,一般只支持180度翻转卡片
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
      * 暂不实现
     **/
    int Evolis_Z390_Printer::Print_Rotate(long lTimeout, int nAngle, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Q_UNUSED(pszRcCode);
        Q_UNUSED(nAngle);
        strcpy(pszRcCode,"0007");
        return 1;
    }

    /** @ingroup CLithographPrinter Function declaration
      * @brief  关闭休眠
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
      * 暂不实现
     **/
    int Evolis_Z390_Printer::Print_CloseSleepMode(long lTimeout, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);

        Q_UNUSED(pszRcCode);
        strcpy(pszRcCode,"0007");
        return 1;
    }
    /** @ingroup CLithographPrinter Function declaration
      * @brief  扩展命令
      * @param[in] pCommand 命令字符串
      * @param[in] lpCmdIn 输入数据
      * @param[in] lpCmdOut 输出数据
      * @param[out] pszRcCode 失败时返回4位错误码，成功时返回"0000"
      * @return 0：成功；1：失败
     **/
	
    int Evolis_Z390_Printer::ReadBankCard2(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Q_UNUSED(lpCmdIn);
        Funclog();
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";
		memset(dataBuff, 0x00, 1024);
        if (!RunApdu( "00A404000E315041592E5359532E4444463031",msg))
        {
            strcpy(pszRcCode, "0020");
            RunlogF("RunApdu Failed.\n");
            return 1;
        }

        if (!RunApdu( "00A4040008A000000333010101",msg))
		{
            strcpy(pszRcCode, "0020");
            RunlogF("RunApdu Failed.\n");
			return 1;
        }

        if (!RunApdu( "00B2011400",msg))
        {
            strcpy(pszRcCode, "0020");
            RunlogF("RunApdu Failed.\n");
            return 1;
        }

        if (FindCard(msg,m_CardInfo.bankNumber))
        {
            String2Char(m_CardInfo.bankNumber);
            lpCmdOut = cpOutMsg;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else
        {
            strcpy(pszRcCode, "0020");
            return 1;
        }
	}

    bool Evolis_Z390_Printer::FindCard(string &strBuffer,string &strNumber)
    {
        RunlogF("Try to match the Tag:5A.\n");
        //Tag Length    card(19位)         补码
        //5A  0A        6228230029000005274F
        int nIndex = strBuffer.find("5A");
        if (nIndex < 0)
           return false;

        char *szEndPtr = nullptr;
        RunlogF("Match the Tag:5A succeed,substring = %s.\n",strBuffer.substr(nIndex,strBuffer.size() - nIndex - 1).c_str());
        nIndex += 2;

        int nStringCount = strtol(strBuffer.substr(nIndex,2).c_str(),&szEndPtr,16);
        if (nStringCount <=0 )
        {
           RunlogF("String Count is invalid.\n");
           return false;
        }
        nIndex += 2;

        if ((nIndex + nStringCount*2) > (int)strBuffer.size())
        {
           RunlogF("Card number size is less then 19,illegal.\n");
           return false;
        }
        strNumber = strBuffer.substr(nIndex, nStringCount*2);
        if (strNumber[strNumber.size() -1] == 'F')
            strNumber[strNumber.size() -1] = '\0';
        RunlogF("Card number = %s.\n",strNumber.c_str());
        return true;
    }

    bool Evolis_Z390_Printer::GetCardNumber(string strMsg,int nGPOIndex,int nRecsize,string &strCardNumber)
    {
        if (nRecsize %8 != 0)
        {
            RunlogF("nRecsize = %d,it's not multiple of 8!",nRecsize);
            return false;
        }

        int nRecCount = nRecsize/8;
        char *szEndPtr = nullptr;
        int nOffset = nGPOIndex;
        for (int i = 0;i < nRecCount;i ++)
        {
            // Rec Sample 08 01 03 00
            string strRec = strMsg.substr(nOffset,4);
            nOffset += 8;
            // Cmd Sample 00 B2 01 0C
            string strCmd = "00B2";
            strCmd += strRec.substr(2,2);   // -->00 B2 01
            int nTag = strtol(strRec.substr(0,2).c_str(),&szEndPtr,16);
            nTag += 4;
            char szTag[4] = {0};
            sprintf(szTag,"%02X",nTag);
            strCmd += szTag;                // -->00 B2 01 0C
            strCmd += "00";                 // -->00 B2 01 0C 00
            string strBuffer;
            if (!RunApdu(strCmd,strBuffer))
            {
                RunlogF("Failed RunApdu(%s).\n",strCmd.c_str());
                return false;
            }
            if (FindCard(strBuffer,strCardNumber))
                return true;
        }
        return false;
    }
    int Evolis_Z390_Printer::ReadBankCard(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
    {
        if (!pReader)
        {
            strcpy(pszRcCode, "0003");
            return 1;
        }
        int nResult = 0;
        if ((nResult = ReadBankCard1(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode)) == 0)
        {
            return nResult;
        }
        else if ((nResult = ReadBankCard2(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode)) == 0)
            return nResult;
        else
        {
            strcpy(pszRcCode, "0020");
            return 1;
        }

//        char szBankCardNo[128] = {0};
//        if (pReader->GetBankcardNo((unsigned char *)szBankCardNo) == 0)
//        {
//            m_CardInfo.bankNumber = szBankCardNo;
//            lpCmdOut = (LPVOID)m_CardInfo.bankNumber.c_str();
//            strcpy(pszRcCode, "0000");
//            return 0;
//        }
//        else
//        {
//            strcpy(pszRcCode, "0020");
//            return 1;
//        }
    }
    // 根据 长城贺工提供的方式读取社保卡卡号
    // 1.发送选择应用类型指令，类型:社保卡应用，指令：00A4040008A00000033301010100
    // 2.发送获取处理选项（GPO）命令,指令：80A800000B830900000000000000015600
    // 3.根据短文件返回值，逐一读取记录，从记录中查找5A开头，19字符的卡号
    int Evolis_Z390_Printer::ReadBankCard1(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
    {
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Q_UNUSED(lpCmdIn);
        Funclog();
        char dataBuff[1024];
        string cmd;
        string msg;
        cmd = "";
        memset(dataBuff, 0x00, 1024);
        if (!RunApdu( "00A4040008A00000033301010100",msg))
        {
            strcpy(pszRcCode, "0020");
            RunlogF("RunApdu Failed.\n");
            return 1;
        }

        if (!RunApdu( "80A800000B830900000000000000015600",msg))
        {
            strcpy(pszRcCode, "0020");
            RunlogF("RunApdu Failed.\n");
            return 1;
        }
        // 返回值示例
        // 80127c00 08010200 10010400 18010300 20010100 9000
        // 其中
        // 80127c00     80为标记，12为长度定位器长度，减掉末尾9000两字节校验，得到有效定位器的长度，即0x10=16，每个定位器4字节。可以判定有4个有效定位器
        // 08010200     为定位器，08为标记，01为起始记录号，02为结束记录号，总共2条记录
        int nSize = msg.size();
        if (nSize <= 12)
        {
            strcpy(pszRcCode, "0020");
            RunlogF("The return value is less 12 bytes.\n");
            return 1;
        }
        if (msg.substr(nSize - 4,4) != "9000")
        {
            RunlogF("Can't find 9000 at the tail of the return value .\n");
            strcpy(pszRcCode, "0020");
            return 1;
        }
        if (msg.substr(0,2) == "80")
        {
            int nGPOIndex = 8;
            int nRecSize = strtol(msg.substr(2,2).c_str(),nullptr,16) - 2;
            GetCardNumber(msg,nGPOIndex, nRecSize*2,m_CardInfo.bankNumber);
            lpCmdOut = (LPVOID)m_CardInfo.bankNumber.c_str();
        }
        else if (msg.substr(0,2) == "77" )
        {//  77 0E 82 02 1C 00 94 08    08 01 01 00   10 01 03 01    90 00
            // 94 为GPO起始位，08为长度，即两个记录
            int nGPOIndex = msg.find("94");
            if (nGPOIndex > 0)
            {
                int nRecSize = strtol(msg.substr(nGPOIndex+2,2).c_str(),nullptr,10) ;
                nGPOIndex += 4;
                GetCardNumber(msg,nGPOIndex, nRecSize*2,m_CardInfo.bankNumber);
                lpCmdOut = (LPVOID)m_CardInfo.bankNumber.c_str();
            }
        }
        else
        {
           strcpy(pszRcCode, "0099");
           return 1;
        }

        strcpy(pszRcCode, "0000");
        return 0;
    }

    int Evolis_Z390_Printer::ResetCard(char *pszRCode)
    {
        if (!pReader)
        {
            strcpy(pszRCode, "0001");
            RunlogF("ResetCard Failed.\n");
            return 1;
        }
        int ret = 0;
        char dataBuffer[1024] = { 0 };
        int nRetry = 0;
        CardPostion nCurCardPos = Pos_Unknow;
        do
        {
            RunlogF("Try to PowerOn:%d.",nRetry + 1)
            if (pReader->PowerOn(dataBuffer,ret) == 0)              // 卡片复位
                break;
            RunlogF("Failed in PowerOn:%d,try to move card and PowerOn again.",++nRetry);
            if (Pos_Unknow == nCurCardPos)
            {
                if (pEvolisPriner->CheckCardPostion((int *)&nCurCardPos,pszRCode,Internal) !=0)
                {
                    RunlogF("Failed in get card position.");
                    return -1;
                }
            }
            else
            {
                pEvolisPriner->MoveCard(Pos_Print,false);
                pEvolisPriner->MoveCard(nCurCardPos,false);
                pReader->SetCardSlot(nCurCardPos);
            }

        }while(nRetry < 3);

        CardATR = dataBuffer;
        RunlogF("ATR Data:%s",dataBuffer);
        int nAtrlen = 26;
        int nOffset = CardATR.size() - nAtrlen;;
        m_CardInfo.ATR = CardATR.substr(nOffset,nAtrlen);
        RunlogF("ATR = %s",m_CardInfo.ATR.c_str());
        strcpy(pszRCode,"0000");
        return 0;
    }

    //
    int Evolis_Z390_Printer::ResetCard_RF(string &strRFATR,char * pszRCode)
    {
        int nCardPos = Pos_Non;
        if (pEvolisPriner->CheckCardPostion(&nCardPos,pszRCode,Internal))
        {
            RunlogF("Failed in get card position.");
            return 1;
        }

        char code[128] = { 0 };
        if (nCardPos == Pos_Contact)
        {
            if (pEvolisPriner->MoveCard(Pos_Contactless))
            {
                RunlogF("Failed in move card to Pos_Contactless.");
                return 1;
            }
           pReader->SetCardSlot(Pos_Contactless);
        }
        ResetCard(code);
        strRFATR = CardATR;
        if (nCardPos == Pos_Contact)        // restore the card position
        {
            if (pEvolisPriner->MoveCard(Pos_Contact))
            {
                RunlogF("Failed in move card to Pos_Contactless.");
                strcpy(pszRCode,"0001");
                return 1;
            }
            pReader->SetCardSlot(Pos_Contact);
            ResetCard(code);    // Reset again
        }

        if (!strRFATR.size())
        {
            RunlogF("Failed in get RFATR.");
            return 1;
        }
        RunlogF("RF ATR = %s",CardATR.c_str());
        return 0;
    }

    int Evolis_Z390_Printer::WriteCard1(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Q_UNUSED(lpCmdIn);
        Funclog();
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";
		memset(dataBuff, 0x00, 1024);
        RunlogF("lpCmdIn = %s.\n",(char *)lpCmdIn);

		string random1;
		string random2;
		string random3;
		string random4;
		string tradeCode = "9013";

		string identifyNum;
		string regionCode;
		string cardVersion;

//		if (CardATR.empty())
//		{
//			strcpy(pszRcCode, "0002");
//			RunlogF(pszRcCode);
//			return 1;
//		}
        //m_CardInfo.ATR = CardATR.substr(8, 26);

        CheckResult(RunApdu("00A40000023F00",msg,false));   // for Henbao Card,must select dir 3F00

        CheckResult(RunApdu("00A404000F7378312E73682EC9E7BBE1B1A3D5CF",msg));

        CheckResult(RunApdu("00A4000002EF05",msg));

        CheckResult(RunApdu("00B2010400",msg)); //卡识别码	 0110 330300D15600000599110145FFFFFFFF 9000

		m_CardInfo.identifyNum = msg.substr(4, msg.length() - 8);
		m_CardInfo.regionCode = m_CardInfo.identifyNum.substr(0, 6);

        CheckResult(RunApdu( "00B2030400",msg));//初始化机构编号 0304 332E3030 9000

		char cas[64] = { 0 };
        pReader->a_hex((unsigned char*)msg.substr(4, msg.length() - 4 - 4).c_str(), (unsigned char*)cas, msg.length());
		m_CardInfo.cardVersion = cas;
        if (m_CardInfo.cardVersion.substr(0, 1) == "1")
            m_CardInfo.cardVersion = "1.00";
        else if (m_CardInfo.cardVersion.substr(0, 1) == "2")
            m_CardInfo.cardVersion = "2.00";
        else if (m_CardInfo.cardVersion.substr(0, 1) == "3")
            m_CardInfo.cardVersion = "3.00";
		else m_CardInfo.cardVersion = "3.00";

        if (!RunApdu( "0084000008",random1)||
            !RunApdu( "0084000008",random2)||
            !RunApdu( "0084000008",random3)||
            !RunApdu( "0084000008",random4))
		{
			strcpy(pszRcCode, "0002");
			return 1;
		}

		string retMsg = "";
		retMsg = "<ROOT>"
			+ CreateNode("AAB301", m_CardInfo.regionCode)
			+ CreateNode("AAZ507", m_CardInfo.ATR)
			+ CreateNode("AFLAG", m_CardInfo.algoCode)
			+ CreateNode("AAZ501", m_CardInfo.identifyNum)
			+ CreateNode("SJS1", random1.substr(0, 16))
			+ CreateNode("SJS2", random2.substr(0, 16))
			+ CreateNode("SJS3", random4.substr(0, 16))
			+ CreateNode("SJS4", random3.substr(0, 16))
			+ CreateNode("MSGNO", tradeCode)
			+ "</ROOT>";		
		String2Char(retMsg.c_str());
		lpCmdOut = cpOutMsg;		
        RunlogF("lpCmdOut = %s.\n",(char*)lpCmdOut);
		strcpy(pszRcCode, "0000");
		return 0;
	}

    int Evolis_Z390_Printer::WriteCard2(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Funclog();
		int ret = 1;
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";
		memset(dataBuff, 0x00, 1024);
        RunlogF("lpCmdIn = %s.\n",(char *)lpCmdIn);
		//解析xml
		string keyAddr = "0094";
		string disFactor;
		string originData;
		string tradeCode = "9011";
		string cardInfo;
		string result1 = "";
		string result2 = "";
		string tempCardNumber = "";

		char temp[1024] = { 0 };
		char szErr[1024] = { 0 };
		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));

        ret = fXMLExtract((char*)lpCmdIn, (char *)"RESULT1", temp, szErr);
        //RunlogF(szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			RunlogF((char*)pszRcCode);
			return 1;
		}
		result1 = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn,(char *) "RESULT2", temp, szErr);
        //RunlogF(szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			RunlogF(pszRcCode);
			return 1;
		}
		result2 = temp;
		cmd = "0082000A10" + result2;
        CheckResult(RunApdu( cmd,msg))//外部认证

        cmd = "00B2070400";
        CheckResult(RunApdu( cmd,msg));//外部认证

		m_CardInfo.cardNumber = msg.substr(4, msg.length() - 4 - 4);
		checkName(m_CardInfo.cardNumber, "0", "");
        Bin2Hexstring((unsigned char*)m_CardInfo.cardNumber.c_str(), (int) m_CardInfo.cardNumber.length(),( char*)temp,1024);

		tempCardNumber = temp;
        CheckResult(RunApdu( "00A4000002EF06",msg));//外部认证

        cmd = "00B2010400";
        CheckResult(RunApdu( cmd,msg));//外部认证

		memset(temp, 0x00, 1024);
		string cardID = msg.substr(4, msg.length() - 4 - 4);
        pReader->a_hex((unsigned char*)cardID.c_str(), (unsigned char*)temp, cardID.length());
		cardID = temp;
		m_CardInfo.cardID = cardID;
		checkName(m_CardInfo.cardID, "0", "");

        cmd = "00B2020400";
        CheckResult(RunApdu( cmd,msg));//外部认证

		memset(temp, 0x00, 1024);
		string name = msg.substr(4, msg.length() - 4 - 4);
        pReader->a_hex((unsigned char*)name.c_str(), (unsigned char*)temp, name.length());
		name = temp;
		m_CardInfo.name = name;
		checkName(m_CardInfo.name, "0", "");

        CheckResult(RunApdu( "0084000008",originData));

        CheckResult(RunApdu( "0084000008",disFactor));

		cardInfo = "";
		cardInfo += m_CardInfo.regionCode;		cardInfo += "|";
		cardInfo += m_CardInfo.cardID;			cardInfo += "|";
		cardInfo += m_CardInfo.cardNumber;		cardInfo += "|";
		cardInfo += m_CardInfo.identifyNum;		cardInfo += "|";
		cardInfo += m_CardInfo.name;			cardInfo += "|";
		cardInfo += m_CardInfo.ATR;				cardInfo += "|";
		cardInfo += m_CardInfo.cardVersion;		cardInfo += "|";
		cardInfo += m_CardInfo.cardReleaseDate;	cardInfo += "|";
		cardInfo += m_CardInfo.cardValidDate;	cardInfo += "|";
		cardInfo += "000000000000|00037261202007000128"; cardInfo += "|";

		cardInfo_temp = cardInfo;

		string retMsg = "<ROOT>"

			+ CreateNode("AAB301", m_CardInfo.regionCode)
			+ CreateNode("AAC002", m_CardInfo.cardID)
			+ CreateNode("AAZ500", m_CardInfo.cardNumber)
			+ CreateNode("AAZ501", m_CardInfo.identifyNum)
			+ CreateNode("AAC003", m_CardInfo.name)
			+ CreateNode("AAZ507", m_CardInfo.ATR)
			+ CreateNode("AFLAG", m_CardInfo.algoCode)
			+ CreateNode("KEYADD", keyAddr)
			+ CreateNode("KEYFAC", disFactor.substr(0, 16))
			+ CreateNode("ODATA", originData.substr(0, 16))
			+ CreateNode("MSGNO", tradeCode)
			+ CreateNode("CARDINFO", cardInfo)
			+ "</ROOT>";

		String2Char(retMsg.c_str());
        lpCmdOut = cpOutMsg;
        RunlogF("lpCmdOut = %s.\n",(char*)lpCmdOut);
		strcpy(pszRcCode, "0000");
		return 0;
	}

    int Evolis_Z390_Printer::WriteCard3(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Funclog();
		int ret = 1;
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";
        RunlogF("lpCmdIn = %s.\n",(char *)lpCmdIn);
        //string strResult = "<ROOT><AAB301></AAB301><AAC002>330821196605154950</AAC002><AAZ500></AAZ500><AAZ501></AAZ501><AAC003>3F3F3F000000000000000000000000000000000000000000000000000000</AAC003><AAZ507></AAZ507><AFLAG>03</AFLAG><KEYADD>0096</KEYADD><KEYFAC>2F59BA07857AFFB0</KEYFAC><ODATA>EC8C0C0939B315B7</ODATA><MSGNO>9011</MSGNO><CARDINFO>|330821196605154950|||3F3F3F000000000000000000000000000000000000000000000000000000|||||000000000000|00037261202007000128|</CARDINFO><ROOT>";
        //strcpy(cpOutMsg,strResult.c_str());
        //lpCmdOut = cpOutMsg;
        //return 0;
		memset(dataBuff, 0x00, 1024);

		string keyAddr = "0096";
		string MSGNO = "9011";
		string result = "";
		string tempCardID = "";
		string tempName = "";
		string tempSex = "";
		char temp[1024] = { 0 };
		char szErr[1024] = { 0 };
		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));

        ret = fXMLExtract((char*)lpCmdIn,(char*)"RESULT", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			RunlogF(pszRcCode);
			return 1;
		}
		result = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"SHBZHM", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			RunlogF(pszRcCode);
			return 1;
		}
		tempCardID = m_newInfos.cardID = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"XM", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
            return 1;
        }
        tempName = temp;
        //QString strUTF8 = QString::fromLocal8Bit(tempName.c_);
        RunlogF("Input Name = %s",temp);
        string strGBKName = Utf8_GBK(tempName.c_str());
        RunlogF("GBK Name = %s",strGBKName.c_str());
        tempName = strGBKName;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"XB", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		tempSex = m_newInfos.sex = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"MZ", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		m_newInfos.nation = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"CSRQ", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		m_newInfos.birthday = temp;

        cmd = "0082000410" + result;
        CheckResult(RunApdu( cmd,msg));

		//写EF06数据
		memset(temp, 0x00, 1024);
        Bin2Hexstring((unsigned char*)tempCardID.c_str(), tempCardID.length(),(char*)temp, 1024);
		//m_newInfos.cardID = temp;
		tempCardID = temp;
        //RunlogF(tempCardID.c_str());

        memset(temp, 0x00, 1024);
        Bin2Hexstring((unsigned char*)strGBKName.c_str(),strGBKName.size(), (char*)temp, 1024);
		//m_newInfos.name = temp;
		tempName = temp;
        //RunlogF(tempName.c_str());
		int nameLen = (60 - tempName.length());
		for (int i = 0; i < nameLen; i++)
		{
			tempName += "0";
		}
        m_newInfos.name = tempName;
        //RunlogF(tempName.c_str());

		memset(temp, 0x00, 1024);
        Bin2Hexstring((unsigned char*)tempSex.c_str(), (int)tempSex.length(),(char*)temp, 1024);
		tempSex = temp;
        //RunlogF(tempSex.c_str());

        CheckResult(WriteFile( "01", "08", tempCardID.c_str(), msg));

        CheckResult(WriteFile( "02", "09", tempName.c_str(), msg));

        CheckResult(WriteFile( "04", "0A", tempSex.c_str(), msg));

        CheckResult(WriteFile( "05", "0B", m_newInfos.nation, msg));

        CheckResult(WriteFile( "07", "0D", m_newInfos.birthday, msg));

        CheckResult(RunApdu( "0084000008",msg));
        string disFactor = msg;

        CheckResult(RunApdu( "0084000008",msg));
        string originData = msg;

		string cardInfo = "";
		cardInfo += m_CardInfo.regionCode;					cardInfo += "|";
		cardInfo += m_newInfos.cardID;						cardInfo += "|";
		cardInfo += m_CardInfo.cardNumber;					cardInfo += "|";
		cardInfo += m_CardInfo.identifyNum;					cardInfo += "|";
		cardInfo += m_newInfos.name;						cardInfo += "|";
		cardInfo += m_CardInfo.ATR;							cardInfo += "|";
		cardInfo += m_CardInfo.cardVersion;					cardInfo += "|";
		cardInfo += m_CardInfo.cardReleaseDate;				cardInfo += "|";
		cardInfo += m_CardInfo.cardValidDate;				cardInfo += "|";
		cardInfo += "000000000000|00037261202007000128";	cardInfo += "|";

		cardInfo_temp = cardInfo;

		string retMsg = "<ROOT>"
			+ CreateNode("AAB301", m_CardInfo.regionCode)
			+ CreateNode("AAC002", m_newInfos.cardID)
			+ CreateNode("AAZ500", m_CardInfo.cardNumber)
			+ CreateNode("AAZ501", m_CardInfo.identifyNum)
			+ CreateNode("AAC003", m_newInfos.name)
			+ CreateNode("AAZ507", m_CardInfo.ATR)
			+ CreateNode("AFLAG", m_CardInfo.algoCode)
			+ CreateNode("KEYADD", keyAddr)
			+ CreateNode("KEYFAC", disFactor.substr(0, 16))
			+ CreateNode("ODATA", originData.substr(0, 16))
			+ CreateNode("MSGNO", MSGNO)
			+ CreateNode("CARDINFO", cardInfo)
			+ "<ROOT>";
        //RunlogF(retMsg.c_str());

		String2Char(retMsg.c_str());
		//	RunlogF(cpOutMsg);
		lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();

		//lpCmdOut = (LPVOID)retMsg.c_str();
        RunlogF("lpCmdOut = %s.\n",(char*)lpCmdOut);
		strcat(pszRcCode, "0000");
		return 0;
	}

    int Evolis_Z390_Printer::WriteCard4(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Funclog();
		int ret = 1;
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";
		memset(dataBuff, 0x00, 1024);
        RunlogF("lpCmdIn = %s.\n",(char *)lpCmdIn);

		char temp[1024] = { 0 };
		char szErr[1024] = { 0 };
		string tempCardVersion = "";
		string tempcardNumber = "";
		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"KSBM", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		m_newInfos.identifyNum = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"KLB", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		string KLB = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"GFBB", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		tempCardVersion = m_newInfos.cardVersion = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn,(char*) "CSHJGBM", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		string orgCode = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"FKRQ", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		m_newInfos.cardReleaseDate = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"KYXQ", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		m_newInfos.cardValidDate = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn,(char*) "KH", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		tempcardNumber = m_newInfos.cardNumber = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"RESULT", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		string result = temp;

        CheckResult(RunApdu( "00A4000002EF05",msg));//

        CheckResult(WriteFile( "01", "01", m_newInfos.identifyNum, msg));

		memset(temp, 0x00, 1024);
        Bin2Hexstring((unsigned char*)KLB.c_str(), (int) KLB.length(),(char*)temp,1024);
        KLB = temp;
        CheckResult(WriteFile( "02", "02", KLB, msg));

		memset(temp, 0x00, 1024);
        Bin2Hexstring((unsigned char*)tempCardVersion.c_str(), (int)tempCardVersion.length(),(char*)temp, 1024);
		tempCardVersion = temp;
		transform(tempCardVersion.begin(), tempCardVersion.end(), tempCardVersion.begin(), ::toupper);

        CheckResult(WriteFile( "03", "03", tempCardVersion, msg));

        CheckResult(WriteFile( "04", "04", orgCode, msg));

        CheckResult(WriteFile( "05", "05", m_newInfos.cardReleaseDate, msg));

        CheckResult(WriteFile( "06", "06", m_newInfos.cardValidDate, msg));

		memset(temp, 0x00, 1024);
        Bin2Hexstring((unsigned char*)tempcardNumber.c_str(), (int)tempcardNumber.length(),(char*)temp, 1024);
        tempcardNumber = temp;
        CheckResult(WriteFile( "07", "07", tempcardNumber, msg));

        CheckResult(RunApdu( "0084000008",msg));
        string random1 = msg;
        CheckResult(RunApdu( "0084000008",msg));
        string random2 = msg;
        CheckResult(RunApdu( "0084000008",msg));
        string random3 = msg;
        CheckResult(RunApdu( "0084000008",msg));
        string random4 = msg;

		string retMsg = "<ROOT>"
			+ CreateNode("AAB301", m_CardInfo.regionCode)
			+ CreateNode("AAZ507", m_CardInfo.ATR)
			+ CreateNode("AFLAG", m_CardInfo.algoCode)
			+ CreateNode("AAZ501", m_CardInfo.identifyNum)
			+ CreateNode("SJS1", random1.substr(0, 16))
			+ CreateNode("SJS2", random2.substr(0, 16))
			+ CreateNode("SJS3", random4.substr(0, 16))
			+ CreateNode("SJS4", random3.substr(0, 16))
			+ CreateNode("MSGNO", "9013")
			+ "</ROOT>";
        //RunlogF(retMsg.c_str());
		String2Char(retMsg.c_str());
        //RunlogF(cpOutMsg);
		lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();
        RunlogF("lpCmdOut = %s.\n",(char*)lpCmdOut);
		strcpy(pszRcCode, "0000");
		return 0;
	}

    int Evolis_Z390_Printer::WriteCard5(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Funclog();
		int ret = 1;
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";
		memset(dataBuff, 0x00, 1024);
        RunlogF("lpCmdIn = %s.\n",(char *)lpCmdIn);

		char temp[1024] = { 0 };
		char szErr[1024] = { 0 };

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"RESULT1", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		string result1 = temp;

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"RESULT2", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		string result2 = temp;

        CheckResult(RunApdu( "00B2010400",msg)); //卡识别码	 0110 330300D15600000599110145FFFFFFFF 9000

		string identifyNumber = msg.substr(4, msg.length() - 8);
		string regionCode = identifyNumber.substr(0, 6);

        CheckResult(RunApdu( "00B2030400",msg));//初始化机构编号 0304 332E3030 9000
		string version = "";

		char cas[64] = { 0 };
        pReader->a_hex((unsigned char*)msg.substr(4, msg.length() - 4 - 4).c_str(), (unsigned char*)cas, msg.length());
		version = cas;
		if (version.substr(0, 1) == "1")version = "1.00";
		else if (version.substr(0, 1) == "2")version = "2.00";
		else if (version.substr(0, 1) == "3")version = "3.00";
		else version = "3.00";

        CheckResult(RunApdu( "00B2050400",msg));//
		string releaseDate = msg.substr(4, msg.length() - 4 - 4);

        CheckResult(RunApdu( "00B2060400",msg));//
		string validDate = msg.substr(4, msg.length() - 4 - 4);

        CheckResult(RunApdu( "00B2070400",msg));
		memset(temp, 0x00, 1024);
		string cardNumber = msg.substr(4, msg.length() - 4 - 4);
        pReader->a_hex((unsigned char*)cardNumber.c_str(), (unsigned char*)temp, cardNumber.length());
		cardNumber = temp;

        CheckResult(RunApdu( "00A4000002EF06",msg));//

        CheckResult(RunApdu( "00B2010400",msg));//

		memset(temp, 0x00, 1024);
		string cardID = msg.substr(4, msg.length() - 4 - 4);
        pReader->a_hex((unsigned char*)cardID.c_str(), (unsigned char*)temp, cardID.length());
		cardID = temp;

        CheckResult(RunApdu( "00B2020400",msg));//

		memset(temp, 0x00, 1024);
		string name = msg.substr(4, msg.length() - 4 - 4);
        pReader->a_hex((unsigned char*)name.c_str(), (unsigned char*)temp, name.length());
		name = temp;

		string cardInfo = "";			//cardInfo += "|";
		cardInfo += regionCode;				cardInfo += "|";
		cardInfo += cardID;					cardInfo += "|";
		cardInfo += cardNumber;				cardInfo += "|";
		cardInfo += identifyNumber;			cardInfo += "|";
		cardInfo += name;					cardInfo += "|";
		cardInfo += m_CardInfo.ATR;			cardInfo += "|";
		cardInfo += version;				cardInfo += "|";
		cardInfo += releaseDate;			cardInfo += "|";
		cardInfo += validDate;				cardInfo += "|";
		cardInfo += "000000000000|00037261202007000128"; cardInfo += "|";

		string retMsg = "<ROOT>"
			+ CreateNode("CARDINFO", cardInfo)
			+ "</ROOT>";
        //RunlogF(retMsg.c_str());
		//strcat((char*)lpCmdOut, retMsg.c_str());
		String2Char(retMsg.c_str());
        //RunlogF(cpOutMsg);
		lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();

        RunlogF("lpCmdOut = %s.\n",(char*)lpCmdOut);
		strcpy(pszRcCode, "0000");
		return 0;
	}

    int Evolis_Z390_Printer::WriteCard6(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Funclog();
		int ret = 1;
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";       
		memset(dataBuff, 0x00, 1024);
        RunlogF("lpCmdIn = %s.",(char *)lpCmdIn);

		char temp[1024] = { 0 };
		char szErr[1024] = { 0 };

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));

        ret = fXMLExtract((char*)lpCmdIn, (char*)"USERPIN", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}

        string userPIN = temp;//00A404000C504B492EC9E7BBE1B1A3D5CF
        CheckResult(RunApdu( "00A40000023F00",msg,false));

        CheckResult(RunApdu( "00A404000C504B492EC9E7BBE1B1A3D5CF",msg)); //卡识别码	 0110 330300D15600000599110145FFFFFFFF 9000

        CheckResult(RunApdu( "00A404000C53532E434552542E41444631",msg)); //卡识别码	 0110 330300D15600000599110145FFFFFFFF 9000

        CheckResult(RunApdu( "00A40000020012",msg)); //卡识别码	 0110 330300D15600000599110145FFFFFFFF 9000

        CheckResult(RunApdu( "00B0000042",msg)); //卡识别码	 0110 330300D15600000599110145FFFFFFFF 9000

        //判断是否全部为0
        std::string tempStr = msg.substr(0, msg.length() - 4);
        bool reAPDU = true;
        for (size_t i = 0; i < tempStr.length(); i++)
        {
            if (tempStr[i] != '0')
            {
                reAPDU = false;
                break;
            }
        }

        if (reAPDU)
        {
            //校验PIN
            cmd = "80C4010305C103" + userPIN + "00";
            CheckResult(RunApdu(cmd.c_str(),msg));
            std::string HASH = msg.substr(0, msg.length() - 4);

            CheckResult(RunApdu("0084000010",msg));
            std::string RAM = msg.substr(0, 32);
            std::string CTK = HASH.substr(0, 32);
            std::string ENDDATA = SM4Enc(CTK, RAM);
            //转大写
            ::transform(ENDDATA.begin(),ENDDATA.end(),ENDDATA.begin(),::toupper);

            cmd = "0020018110" + ENDDATA;
            CheckResult(RunApdu(cmd.c_str(),msg));
            //触发公私钥对
            CheckResult(RunApdu("8040000008C0020012C2020013",msg));

            CheckResult(RunApdu("80C9820008C0020012C2020013",msg));

            CheckResult(RunApdu("80C9828000",msg));
        }

		string signatureKey = msg.substr(4, msg.length() - 8);
		string retMsg = "<ROOT>" + CreateNode("QMGY", signatureKey) + "</ROOT>";
        //RunlogF(retMsg.c_str());
		strcpy(pszRcCode, "0000");
		//strcat((char*)lpCmdOut, retMsg.c_str());
		String2Char(retMsg.c_str());
        //RunlogF(cpOutMsg);
		lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();
        RunlogF("lpCmdOut = %s.\n",(char*)lpCmdOut);
		return 0;
	}


    int Evolis_Z390_Printer::WriteCard7(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
	{
        Q_UNUSED(lTimeout);
        Q_UNUSED(pCommand);
        Q_UNUSED(lpCmdOut);
        if (!lpCmdIn)
        {
            strcpy(pszRcCode,"0001");
            return 1;
        }
		int ret = 1;
		char dataBuff[1024];
		string cmd;
		string msg;
		cmd = "";
		memset(dataBuff, 0x00, 1024);
        RunlogF("lpCmdIn = %s.\n",(char *)lpCmdIn);

		char temp[102400] = { 0 };
		char szErr[1024] = { 0 };
		string HASH = "";
		string UPIN = "";
		string QMZS = "";
		string JMZS = ""; 
		string JMMY = "";
		string OLDGLYPIN = ""; 
		string GLYPIN = ""; 
		string OLDZKMY = ""; 
		string ZKMY = "";
		string ENCRYPT = "";
		string  PRKEY = "";
		string PUKEY = "";
		string RAM = "";
		string CTK = "";

		string sm4Key = "";
		string ENDDATA = "";
        //string strTempCommand = "";
		string ENDATA = "";
		string DATA = "";
		string MAC = "";

        RunlogF("Try to Resetcard.");
        if (ResetCard(pszRcCode) != 0)              // 卡片复位
        {
            RunlogF("Failed in resetcard.");
            return 1;
        }

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));

        ret = fXMLExtract((char*)lpCmdIn, (char*)"USERPIN", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		UPIN = temp;
        RunlogF("UserPin = %s.",UPIN.c_str());

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"QMZS", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		QMZS = temp;
        RunlogF("QMZS = %s",QMZS.c_str());

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"JMZS", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		JMZS = temp;
        RunlogF("JMZS = %s",JMZS.c_str());

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"JMMY", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		JMMY = temp;
        RunlogF("JMMY = %s",JMMY.c_str());

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"OLDGLYPIN", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		OLDGLYPIN = temp;
        RunlogF("OLDGLYPIN = %s",OLDGLYPIN.c_str());

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"GLYPIN", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		GLYPIN = temp;
        RunlogF("GLYPIN = %s",GLYPIN.c_str());
		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"OLDZKMY", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		OLDZKMY = temp;
        RunlogF("OLDZKMY = %s",OLDZKMY.c_str());

		memset(temp, 0x00, sizeof(temp));
		memset(szErr, 0x00, sizeof(szErr));
        ret = fXMLExtract((char*)lpCmdIn, (char*)"ZKMY", temp, szErr);
		if (ret != 0)
		{
			strcpy(pszRcCode, "0001");
			//strcat(pszRcCode, szErr);
			RunlogF(pszRcCode);
			return 1;
		}
		ZKMY = temp;
        RunlogF("ZKMY = %s",ZKMY.c_str());

		//传入数据是BASE64,先转出来然后再转成16进制
		char tempBase64[102400] = { 0 };
		int Base64Len = 0;
		memset(tempBase64, 0x00, 102400);

		replace_all(QMZS, "\\", "");
        RunlogF("QMZS = %s",QMZS.c_str());
		Base64Len = BASE64_Decode(QMZS.c_str(), QMZS.length(), (unsigned char*)tempBase64);
		if (Base64Len <= 0)
		{
			strcpy(pszRcCode, "0001");
			RunlogF("BASE64_Decode 转换错误");
			return 1;
		}
        RunlogF("Base64Len = %d",Base64Len);
        //RunlogF("tempBase64 = %s",tempBase64);

		memset(temp, 0x00, 102400);
        //int nConvert = pReader->hex_a((unsigned char*)tempBase64, (unsigned char*)temp, Base64Len);
        int nConvert = Bin2Hexstring((unsigned char*)tempBase64,Base64Len,(char *)temp,102400);
        RunlogF("nConvert = %d",nConvert);
		QMZS = temp;
        RunlogF("QMZS = %s",QMZS.c_str());

		memset(tempBase64, 0x00, 102400);
		replace_all(JMZS, "\\", "");
        RunlogF("JMZS = %s",JMZS.c_str());
		Base64Len = BASE64_Decode((char*)JMZS.c_str(), JMZS.length(), (unsigned char*)tempBase64);
        RunlogF("Base64Len = %d",Base64Len);
		if (Base64Len <= 0)
		{
			strcpy(pszRcCode, "0001");
			RunlogF("BASE64_Decode 转换错误");
			return 1;
		}
        //RunlogF("tempBase64 = %s",tempBase64);
		memset(temp, 0x00, 102400);
        //pReader->hex_a((unsigned char*)tempBase64, (unsigned char*)temp, Base64Len);
        nConvert = Bin2Hexstring((unsigned char*)tempBase64,Base64Len,(char *)temp,102400);
		JMZS = temp;
        RunlogF("JMZS = %s",JMZS.c_str());

		memset(tempBase64, 0x00, 102400);
		replace_all(JMMY, "\\", "");
        RunlogF("JMMY = %s",JMMY.c_str());
		Base64Len = BASE64_Decode((char*)JMMY.c_str(), JMMY.length(), (unsigned char*)tempBase64);
		if (Base64Len <= 0)
		{
			strcpy(pszRcCode, "0001");
			RunlogF("BASE64_Decode 转换错误");
			return 1;
		}
		memset(temp, 0x00, 102400);
        Bin2Hexstring((unsigned char*)tempBase64, Base64Len, (char*)temp,102400);
		JMMY = temp;
        RunlogF("JMMY = %s",JMMY.c_str());

		ENCRYPT = JMMY.substr(480, 64) + JMMY.substr(608, 64) + JMMY.substr(672, 64) + JMMY.substr(744, 32);
		PRKEY = JMMY.substr(88, 64);
		PUKEY = JMMY.substr(224, 64) + JMMY.substr(352, 64);
        RunlogF("ENCRYPT = %s",ENCRYPT.c_str());
        RunlogF("PRKEY = %s",PRKEY.c_str());
        RunlogF("PUKEY = %s",PUKEY.c_str());

		//return 1;
#if 1
        CheckResult(RunApdu("00A40000023F00",msg,false));

        CheckResult(RunApdu("00A404000C504B492EC9E7BBE1B1A3D5CF",msg)); //

        CheckResult(RunApdu("00A404000C53532E434552542E41444631",msg)); //

        cmd = "80C4010305C103" + UPIN + "00";
        CheckResult(RunApdu( cmd,msg)); //"3DE741F445DD357BEA4E6C3FE6437036F62B5E3EC0F00D62796EDB4305ED627F9000";
		HASH = msg.substr(0, msg.length() - 4);

        CheckResult(RunApdu( "0084000010",msg));

		if (msg.length() < 32 && HASH.length() < 32)
		{
			strcpy(pszRcCode, "0002");
            RunlogF("数据长度不符:");
            RunlogF(msg.c_str());
            RunlogF(HASH.c_str());
			return 1;
		}
		RAM = msg.substr(0, 32);
		CTK = HASH.substr(0, 32);

		sm4Key = SM4Enc(CTK, RAM);		// RAM = "178CEDCF32C5D7990216632170C7CD37"; CTK = "3DE741F445DD357BEA4E6C3FE6437036"
		//"358B51056B9B9EB2C96B20E7DB7A6B8F"
		if (sm4Key.length() <= 0)
		{
			strcpy(pszRcCode, "0002");
			//strcat(pszRcCode, msg.c_str());
			RunlogF("SM4转换失败");
			return 1;
		}
		ENDDATA = sm4Key;

        cmd = "0020018110" + ENDDATA;
        CheckResult(RunApdu( cmd,msg)); //

		//校验MPIN
        cmd = "80C4010306C104" + OLDGLYPIN + "00";
        CheckResult(RunApdu( cmd,msg));

		HASH = msg.substr(0, msg.length() - 4);
        CheckResult(RunApdu( "0084000010",msg));

		RAM = msg.substr(0, 32);
		CTK = HASH.substr(0, 32);

		if (msg.length() < 32 && HASH.length() < 32)
		{
			strcpy(pszRcCode, "0002");
            RunlogF("数据长度不符:");
            RunlogF(msg.c_str());
            RunlogF(HASH.c_str());
			return 1;
		}

		sm4Key = SM4Enc(CTK, RAM);//"8349E02CA888AF5E9137A64333881DBF"
		if (sm4Key.length() <= 0)
		{
			strcpy(pszRcCode, "0002");
			//strcat(pszRcCode, msg.c_str());
			RunlogF("SM4转换失败");
			return 1;
		}

		ENDDATA = sm4Key;
		cmd = "0020010010";
        cmd += ENDDATA;
        CheckResult(RunApdu( cmd,msg));

		//写CA        
        CheckResult(WriteCA( "804E0001", "C2020013C18200", ENCRYPT, msg)); ////提供签名公钥给CA，返回后续写入内容

		string KEY = msg.substr(0, msg.length() - 4);

        CheckResult(WriteCA( "804A0210", "C1", KEY, msg)); ////导入SM4 804A02 + 10 + L(C1 + L(KEY))

        cmd = "80C2400046C0020014CA40" + PUKEY;         //导入公钥 80C2400046+C0020014+CA40+PUKEY
        CheckResult(RunApdu( cmd,msg));

        cmd = "80C2C00126C2020015CB20" + PRKEY;         //导入私钥 80C2C00126+C2020015+CB20+PRKEY
        CheckResult(RunApdu( cmd,msg));

        //#region 写0018签名证书
        CheckResult(RunApdu( "00A40000020018",msg)); //

        cmd = "00D6000070" + QMZS.substr(0, 224);;
        CheckResult(RunApdu( cmd,msg));

        cmd = "00D6007070" + QMZS.substr(224, 224);;
        CheckResult(RunApdu( cmd,msg)); //

        cmd = "00D600E070" + QMZS.substr(448, 224);;
        CheckResult(RunApdu( cmd,msg)); //

        cmd = "00D6015070" + QMZS.substr(672, 224);;
        CheckResult(RunApdu( cmd,msg)); //

		memset(temp, 0x00, 1024);
		sprintf(temp, "%02X", QMZS.substr(896, QMZS.length() - 896).length() / 2);
		cmd = "00D601C0";
		cmd += temp;
        cmd += QMZS.substr(896, QMZS.length() - 896);
        CheckResult(RunApdu( cmd,msg)); //

        //#region 写0019加密证书
        CheckResult(RunApdu( "00A40000020019",msg)); //

        cmd = "00D6000070" + JMZS.substr(0, 224);;
        CheckResult(RunApdu( cmd,msg)); //

        cmd = "00D6007070" + JMZS.substr(224, 224);;
        CheckResult(RunApdu( cmd,msg)); //

        cmd = "00D600E070" + JMZS.substr(448, 224);;
        CheckResult(RunApdu( cmd,msg)); //

        cmd = "00D6015070" + JMZS.substr(672, 224);;
        CheckResult(RunApdu( cmd,msg)); //

		memset(temp, 0x00, 102400);
		sprintf(temp, "%02X", JMZS.substr(896, JMZS.length() - 896).length() / 2);
		cmd = "00D601C0";
		cmd += temp;
        cmd += JMZS.substr(896, JMZS.length() - 896);
        CheckResult(RunApdu( cmd,msg)); //

		//#region 修改MPIN
        //写0019加密证书
        CheckResult(WriteCA( "80C40103", "C1", OLDGLYPIN, msg));

		HASH = msg.substr(0, msg.length() - 4);
		CTK = HASH.substr(0, 32);

		string tempSm4RAM = "";
		memset(temp, 0x00, 102400);
		sprintf(temp, "%02X", GLYPIN.length() / 2);
		tempSm4RAM = temp;
		tempSm4RAM += GLYPIN;
		tempSm4RAM += "8000000000000000000000";

		sm4Key = SM4Enc(CTK, tempSm4RAM);
		if (sm4Key.length() <= 0)
		{
			strcpy(pszRcCode, "0002");
			//strcat(pszRcCode, msg.c_str());
			RunlogF("SM4转换失败");
			return 1;
		}

		ENDATA = sm4Key;
        CheckResult(RunApdu( "0084000010",msg));

		RAM = msg.substr(0, 32);
		DATA = "845E010014" + ENDATA + "8000000000000000000000";
		MAC = SM4_MAC(RAM, DATA, CTK);
        cmd = "845E010014" + ENDATA + MAC;
        CheckResult(RunApdu( cmd,msg)); //

        //#region UPDATE ADF MK
        CheckResult(RunApdu( "00A404000C504B492EC9E7BBE1B1A3D5CF",msg)); //

        CheckResult(RunApdu( "00A404000C53532E434552542E41444631",msg)); //

        CheckResult(RunApdu( "0084000010",msg)); //

		RAM = msg.substr(0, 32);
		ENDATA = SM4Enc(OLDZKMY, RAM);

        cmd = "0082000010" + ENDATA;
        CheckResult(RunApdu( cmd,msg)); //

        CheckResult(RunApdu( "0084000010",msg));

		RAM = msg.substr(0, 32);

		ENDATA = SM4Enc(OLDZKMY, "13000000" + ZKMY + "800000000000000000000000");
		DATA = "84D4010024" + ENDATA + "8000000000000000000000";
		MAC = GetMAC(RAM, DATA, OLDZKMY);

        cmd = "84D4010024" + ENDATA + MAC;
        CheckResult(RunApdu( cmd,msg)); //

        //#region UPDATE DDF MK
        CheckResult(RunApdu( "00A404000C504B492EC9E7BBE1B1A3D5CF",msg)); //

        CheckResult(RunApdu( "0084000010",msg)); //

		RAM = msg.substr(0, 32);
		ENDATA = SM4Enc(OLDZKMY, RAM);
        cmd = "0082000010" + ENDATA;
        CheckResult(RunApdu( cmd,msg)); //

        CheckResult(RunApdu( "0084000010",msg)); //

#endif // 0

		RAM = msg.substr(0, 32);
		ENDATA = SM4Enc(OLDZKMY, "13000000" + ZKMY + "800000000000000000000000");

		cmd = "84D4010024" + ENDATA + MAC;
		MAC = GetMAC(RAM, DATA, OLDZKMY);

        cmd = "84D4010024" + ENDATA + MAC;
        CheckResult(RunApdu( cmd,msg)); //

		strcpy(pszRcCode, "0000");
		return 0;
		
	}

    int Evolis_Z390_Printer::Print_ExtraCommand(long lTimeout, char* pCommand, LPVOID lpCmdIn, LPVOID& lpCmdOut, char* pszRcCode)
    {
		RunlogF(pCommand);
        RunlogF("Version = %s",LibVer);
        QString strCommand(pCommand);
        char *pDest = nullptr;
        if (strCommand.contains("WriteCardEx_WenZhou"))
        {
            return WZ_CitizenCardExAPDU(pCommand, (char *)lpCmdIn,lpCmdOut, pszRcCode);
        }
        else if (0 == strcmp(pCommand,"GetVersion"))
        {
            strcpy(pszRcCode,LibVer);
            return 0;
        }
        else if (0 == strcmp(pCommand,"EnableOutput"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            char *szEnable =(char *)lpCmdIn;
            if (strcmp(szEnable,"True") == 0)
            {
                g_bEnableOutput = true;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            if (strcmp(szEnable,"False") ==0)
            {
                RunlogF("Warning:SDK Output is disabled!");
                g_bEnableOutput = false;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else
            {
                strcpy(pszRcCode, "0005");
                return 1;
            }
            return 0;
        }
        else if (0 == strcmp(pCommand,"RibbonStatus"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            sprintf((char *)pEvolisPriner->szEvolisReply,"Capcity = %d remained = %d",pEvolisPriner->ribbon.capacity,pEvolisPriner->ribbon.remaining);
            lpCmdOut = &pEvolisPriner->szEvolisReply;
            return 0;
        }
        else if (0 == strcmp(pCommand,"Enablelog"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            char *szEnable =(char *)lpCmdIn;
            if (strcmp(szEnable,"True") == 0)
            {
                g_bEnableLog = true;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            if (strcmp(szEnable,"False") ==0)
            {
                RunlogF("Warning:SDK log is disabled!");
                g_bEnableLog = false;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else
            {
                strcpy(pszRcCode, "0005");
                return 1;
            }
            return 0;
        }
        else if (0 == strcmp(pCommand,"EvolisCommand"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            return pEvolisPriner->SendCommand((const char *)lpCmdIn,lpCmdOut,pszRcCode);
        }
        else if (0 == strcmp(pCommand,"Reset Delay"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            int nResetDelay = QString((char *)lpCmdIn).toInt();
            if (nResetDelay < 5000 || nResetDelay > 30000)
                nResetDelay = 5000;
            pEvolisPriner->nResetDelay = nResetDelay;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"Set DeviceReset"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            char *szEnable =(char *)lpCmdIn;
            if (strcmp(szEnable,"True") == 0)
            {
                pEvolisPriner->bHardwareReset = true;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            if (strcmp(szEnable,"False") ==0)
            {
                pEvolisPriner->bHardwareReset = false;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else
            {
                strcpy(pszRcCode, "0005");
                return 1;
            }
        }
        else if (0 == strcmp(pCommand,"FColorBrightness"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            pEvolisPriner->strFColorBrightness = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"BColorBrightness"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            pEvolisPriner->strBColorBrightness = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"FColorContrast"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            pEvolisPriner->strFColorBrightness = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"BColorContrast"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            pEvolisPriner->strBColorContrast = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"IFDarkLevelValue"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }

            pEvolisPriner->strIFDarkLevelValue = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"IFBlackLevelValue"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }

            pEvolisPriner->strIFBlackLevelValue = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"Resolution"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            RunlogF("Command = %s\tlpCmdIn = %s.",pCommand,(const char *)lpCmdIn);
            pEvolisPriner->strResolution = (const char *)lpCmdIn;
            if (pEvolisPriner->strResolution == "DPI600300")
            {
                pEvolisPriner->nDPI_H = 300;
                pEvolisPriner->nDPI_W = 600;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else if (pEvolisPriner->strResolution == "DPI300300")
            {
                pEvolisPriner->nDPI_H = 300;
                pEvolisPriner->nDPI_W = 300;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else
            {
                strcpy(pszRcCode, "0005");
                return 1;
            }
        }
        else if (0 == strcmp(pCommand,"Print Resolution"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            char *szEnable =(char *)lpCmdIn;
            if (strcmp(szEnable,"True") == 0)
            {
                pEvolisPriner->bMarkDPI = true;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            if (strcmp(szEnable,"False") ==0)
            {
                pEvolisPriner->bMarkDPI = false;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else
            {
                strcpy(pszRcCode, "0005");
                return 1;
            }
        }
        else if (0 == strcmp(pCommand,"Print Datetime"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }
            char *szEnable =(char *)lpCmdIn;
            if (strcmp(szEnable,"True") == 0)
            {
                pEvolisPriner->bMarkDateTime = true;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            if (strcmp(szEnable,"False") ==0)
            {
                pEvolisPriner->bMarkDateTime = false;
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else
            {
                strcpy(pszRcCode, "0005");
                return 1;
            }
        }
        else if (0 == strcmp(pCommand,"Set GRibbonType"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }

            pEvolisPriner->strGRibbonType = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"Set SavePreview"))
        {
            if (!pEvolisPriner)
            {
                strcpy(pszRcCode, "0001");
                return 1;
            }

            pEvolisPriner->strPreviewFile = (const char *)lpCmdIn;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"Set Overlayer"))
        {
            if (!pEvolisPriner)
            {
                 strcpy(pszRcCode, "0001");
                 return 1;
            }
            char szOverlayer[1024] = {0};
            strcpy(szOverlayer,(char *)lpCmdIn);
            QFileInfo fi(szOverlayer);
            if (!fi.isFile())
            {
                RunlogF("Can't find file %s.\n",szOverlayer);
                if (lpCmdOut)
                    sprintf((char *)lpCmdOut,"Can't find file:%s.",szOverlayer);
                strcpy(pszRcCode, "0001");
                return 1;
            }
            pEvolisPriner->strOverlayer = szOverlayer;
            strcpy(pszRcCode, "0000");
            return 0;
        }
        else if (0 == strcmp(pCommand,"IndraftCard"))
        {
            if (!pEvolisPriner)
            {
                 strcpy(pszRcCode, "0001");
                 return 1;
            }
            return pEvolisPriner->InDraftCard(lTimeout,pszRcCode);

        }
        else if (0 == strcmp(pCommand, "ReadBankNo")) //读取银行卡号
		{
			return ReadBankCard(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
		}
        else if (0 == strcmp(pCommand, "CompressPicture")) //读取银行卡号
        {
            string strParam = (char *)lpCmdIn;
            int nOffset = strParam.find(",");
            string strSource = strParam.substr(0,nOffset);
            string strDest = strParam.substr(nOffset + 1);
            RunlogF("SourceFile = %s\tDestFile = %s",strSource.c_str(),strDest.c_str());
            if (CompressPicture(strSource,strDest, 60))
            {
                strcpy(pszRcCode,"0000");
                return 0;
            }
            else
            {
                strcpy(pszRcCode,"0001");
                return 1;
            }
        }
        else if (0 == strcmp(pCommand, "WriteImage"))
        {
            string strParam = (char *)lpCmdIn;
            RunlogF("Try to read file %s!",strParam.c_str());
            QFile fin((char *)lpCmdIn);
            if (!fin.open(QIODevice::ReadOnly))
            {
                RunlogF("Failed in reading file %s!",lpCmdIn);
                return 1;
            }
            QByteArray byBuffer = fin.readAll();
            //QByteArray byBase64 = byBuffer.toBase64();
            //RunlogF("PIC Base64:%s",byBase64.data());
            string strCompressedPic = string(byBuffer.data(), byBuffer.size());
            //写入照片数据
            if(WriteImageInfo(strCompressedPic,pszRcCode) != 0)
            {
                return 1;
            }
            else
                return 0;
        }
        else if (strstr(pCommand,"WriteCard"))
		{
			const char* szWriteCard = "WriteCard:";
            size_t nLen = strlen(szWriteCard);
			if (strlen(pCommand) < nLen + 1)
			{
				return 1;
			}
			if (0 != strncmp(pCommand, szWriteCard, nLen))
			{
				strcpy(pszRcCode, "0001");
				return 1;
			}

             if ((pDest = strstr(pCommand,"WriteCard")))
             {
                 int nOffset = pDest - pCommand + strlen("WriteCard") + 1;
                 switch (pCommand[nOffset])
                 {
                 case '1':
                     return WriteCard1(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
                     break;
                 case '2':
                     return WriteCard2(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
                     break;
                 case '3':
                     return WriteCard3(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
                     break;
                 case '4':
                     return WriteCard4(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
                     break;
                 case '5':
                     return WriteCard5(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
                     break;
                 case '6':
                     return WriteCard6(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
                     break;
                 case '7':
                     return WriteCard7(lTimeout, pCommand, lpCmdIn, lpCmdOut, pszRcCode);
                     break;
                 default:
                 {
                     //温度市民卡写入扩展
//                     if ((pDest = strstr(pCommand,"WriteCardEx_WenZhou")))
//                     {
//                         return WZ_CitizenCardExAPDU(pCommand, (char*)lpCmdIn,lpCmdOut, pszRcCode);
//                     }
//                     else
                     {
                         strcpy(pszRcCode, "0002");
                         return 1;
                     }
                 }
                     break;
                 }
             }
		}
        else if ((pDest = strstr(pCommand,"WriteCitizenCard")))
        {
            int nResult = -1;
            if (!lpCmdIn)
            {
                RunlogF("lpCmdIn is invalid.");
                strcpy(pszRcCode,"0001");
                return 1;
            }
            do
            {
                //温州市民卡
                if ((pDest = strstr(pCommand,"WriteCitizenCard_3303")))
                {
                    nResult = WZ_CitizenCardAPDU(pCommand, (char *)lpCmdIn, pszRcCode);
                    break;
                }
                //嘉兴市民卡
                else if ((pDest = strstr(pCommand,"WriteCitizenCard_3304")))
                {
                    nResult = JX_CitizenCardAPDU(pCommand, (char *)lpCmdIn, pszRcCode);
                    break;
                }
                //舟山市民卡
                else if ((pDest = strstr(pCommand,"WriteCitizenCard_3309")))
                {
                    nResult = ZS_CitizenCardAPDU(pCommand, (char *)lpCmdIn, pszRcCode);
                    break;
                }
                //金华市民卡
                else if ((pDest = strstr(pCommand,"WriteCitizenCard_3307")))
                {
                    nResult = JH_CitizenCardAPDU(pCommand, (char *)lpCmdIn, pszRcCode);
                    break;
                }
                //绍兴市民卡
                else if ((pDest = strstr(pCommand,"WriteCitizenCard_3306")))
                {
                    nResult = SX_CitizenCardAPDU(pCommand, (char *)lpCmdIn, pszRcCode);
                    break;
                }
                //义乌市民卡
                else if ((pDest = strstr(pCommand,"WriteCitizenCard_330782")))
                {
                    nResult = YW_CitizenCardAPDU(pCommand, (char *)lpCmdIn, pszRcCode);
                    break;
                }
                else
                {
                    RunlogF("Uknow command:%s",pCommand);
                    strcpy(pszRcCode, "0001");
                    return 1;
                }
            }while(0);

            if (nResult == 0)
            {
                lpCmdOut = cpOutMsg;
                RunlogF((char *)lpCmdOut);
                strcpy(pszRcCode, "0000");
                return 0;
            }
            else
                return nResult;
        }
//        else if ((strstr(pCommand,"WriteCardEx_WenZhou")))
//        {
//            return WZ_CitizenCardExAPDU(pCommand, (char *)lpCmdIn,lpCmdOut, pszRcCode);
//        }
        else if (0 == strcmp(pCommand,"EvolisStatus"))
        {
            evolis_status_t es;
            auto tNow = high_resolution_clock::now();
            if (pEvolisPriner->pevolis_status(pEvolisPriner->m_pPrinter, &es) != 0)
            {
                RunlogF("Error reading printer status\n");
                strcpy(pszRcCode, "0001");
                return 1;
            }
            auto tDuration = duration_cast<milliseconds>(high_resolution_clock::now() - tNow);
            RunlogF("pevolis_status duration %d ms\n",tDuration.count());
            strcpy(pszRcCode, "0000");
            return 0;
        }
        return 0;
    }

    // get value from a tlv string
    string Evolis_Z390_Printer::GetTlvValue(string strMsg,int nSize)
    {
        char szTemp[1024] = {0};
        pReader->a_hex((unsigned char*)&strMsg.c_str()[4], (unsigned char*)szTemp, nSize - 4);
        return szTemp;
    }

    // convert strValue to a tlv string
    string Evolis_Z390_Printer::GetTlvString(string strValue,int nSize)
    {
        unsigned char szTemp[1024] = {0};
        pReader->hex_a((unsigned char *)strValue.c_str(), (unsigned char *)szTemp, strValue.length());
        strValue = (char *)szTemp;
        int nExpandnum = nSize - strlen((char *)szTemp);//补齐数
        return((const char *)szTemp + string(nExpandnum, '0'));
    }

    bool Evolis_Z390_Printer::ReadFile(std::string fileID,string &msg)
    {
         std::string cmd = "00B2" + fileID + "0400";
         int rLen = 0;
         char dataBuff[1024] = { 0 };
         RunlogF("cmd = %s",cmd.c_str());

         if(!RunApdu(cmd.c_str(),msg))
            return false;
         else  if ("9000" != msg.substr(msg.length() - 4, 4))
            return false;
        return true;
    }

    bool Evolis_Z390_Printer::WriteFile(string fileID, string tag, string val,string &msg)
	{
        Funclog();
		char valHex[32] = { 0 };
		sprintf(valHex, "%02X", val.length() / 2);
		string tlv = tag + valHex + val;
		memset(valHex, 0x00, 32);
		sprintf(valHex, "%02X", tlv.length() / 2);
        string cmd = "00DC" + fileID + "04" + valHex + tlv;
        if(!RunApdu(cmd.c_str(),msg))
           return false;
        else  if ("9000" != msg.substr(msg.length() - 4, 4))
           return false;
       return true;
	}

    bool Evolis_Z390_Printer::WriteCA(string fileID, string tag, string val, string &msg)
	{
		char valHex[32] = { 0 };
		sprintf(valHex, "%02X", val.length() / 2);
		string tlv = tag + valHex + val;
		memset(valHex, 0x00, 32);
		sprintf(valHex, "%02X", tlv.length() / 2);
        string cmd = fileID + valHex + tlv;;
        if (!RunApdu( cmd,msg))
           return false;
        else  if ("9000" != msg.substr(msg.length() - 4, 4))
            return false;
        return true;
	}

    bool Evolis_Z390_Printer::SelectSSSE(string &msg)
    {
        if (!pReader)
            return false;
        CheckResult2(RunApdu("00A404000F7378312E73682EC9E7BBE1B1A3D5CF",msg));
        return true;
    }

    bool Evolis_Z390_Printer::SelectDir(string strDirID, string &msg)
    {
        if (!pReader)
            return false;
        string strCMD = "00A4000002" + strDirID;
        CheckResult2(RunApdu(strCMD,msg));
        return true;
    }

    bool Evolis_Z390_Printer::ExternalAuth(string strAddr, string strKey,string &msg)
    {
        if (!pReader)
            return false;
        string strCmd = "008200" + strAddr + strKey;
        CheckResult2(RunApdu(strCmd,msg));
        return true;
    }

    bool Evolis_Z390_Printer::GetRandom(string &msg)
    {
        if (!pReader)
            return false;
        CheckResult2(RunApdu("0084000008",msg));
        return true;
    }

    void Evolis_Z390_Printer::hexstrxor(char* HexStr1, char* HexStr2, char* HexStr)
	{
		int i, iHexStr1Len, iHexStr2Len, iHexStrLenLow, iHexStrLenGap;

		//转换成大写并求长度, strupr是非标准的C函数，在Linux下不支持，所以需要自己实现或者使用glib中的g_string_ascii_up ()
        strtoupper(HexStr1);
        strtoupper(HexStr2);
		iHexStr1Len = strlen(HexStr1);
		iHexStr2Len = strlen(HexStr2);

		//获取最小的长度
		iHexStrLenLow = iHexStr1Len < iHexStr2Len ? iHexStr1Len : iHexStr2Len;

		//获取长度差值
		iHexStrLenGap = abs(iHexStr1Len - iHexStr2Len);

		//两个十六进制的字符串进行异或
		for (i = 0; i < iHexStrLenLow; i++)
		{
			*(HexStr + i) = char2int(HexStr1[i]) ^ char2int(HexStr2[i]);
			*(HexStr + i) = int2char(*(HexStr + i));
		}
		if (iHexStr1Len > iHexStr2Len)
			memcpy(HexStr + i, HexStr1 + i, iHexStrLenGap);
		else if (iHexStr1Len < iHexStr2Len)
			memcpy(HexStr + i, HexStr2 + i, iHexStrLenGap);
		*(HexStr + iHexStrLenLow + iHexStrLenGap) = 0x00;
	}

    string Evolis_Z390_Printer::SM4_MAC(string RAM, string DATA, string CTK)
	{
		//RunlogF("RAM:" + RAM);
		//RunlogF("DATA:" + DATA);
		//RunlogF("CTK:" + CTK);
		//PIN码mac计算
		char result[64] = { 0 };
		string DATA1 = RAM;
		string DATA2 = DATA.substr(0, 32);
		//RunlogF("DATA1:" + DATA1);
		//RunlogF("DATA2:" + DATA2);

		memset(result, 0x00, 64);
		hexstrxor((char*)DATA1.c_str(), (char*)DATA2.c_str(), result);
		string DATA3 = result;
		//RunlogF("DATA3:" + DATA3);
		DATA1 = SM4Enc(CTK, DATA3);
		//RunlogF("DATA1:" + DATA1);
		DATA2 = DATA.substr(32, 32);
		//RunlogF("DATA2:" + DATA2);
		memset(result, 0x00, 64);
		hexstrxor((char*)DATA1.c_str(), (char*)DATA2.c_str(), result);
		DATA3 = result;
		//RunlogF("DATA3:" + DATA3);
		DATA1 = SM4Enc(CTK, DATA3);
		//RunlogF("DATA1:" + DATA1);
		string MAC = DATA1.substr(DATA1.length() - 32, 32);
		string MAC1 = MAC.substr(0, 8);
		string MAC2 = MAC.substr(8, 8);
		string MAC3 = MAC.substr(16, 8);
		string MAC4 = MAC.substr(24, 8);
		//RunlogF("MAC:" + MAC);
		//RunlogF("MAC1:" + MAC1);
		//RunlogF("MAC2:" + MAC2);
		//RunlogF("MAC3:" + MAC3);
		//RunlogF("MAC4:" + MAC4);
		memset(result, 0x00, 64);
		hexstrxor((char*)MAC1.c_str(), (char*)MAC2.c_str(), result);
		MAC = result;
		//RunlogF("MAC:" + MAC);
		memset(result, 0x00, 64);
		hexstrxor((char*)MAC.c_str(), (char*)MAC3.c_str(), result);
		MAC = result;
		//RunlogF("MAC:" + MAC);
		memset(result, 0x00, 64);
		hexstrxor((char*)MAC.c_str(), (char*)MAC4.c_str(), result);
		MAC = result;
		//RunlogF("MAC:" + MAC);
		return MAC;
	}

    string Evolis_Z390_Printer::GetMAC(string random_32, string data, string key)
	{
		char result[32] = { 0 };
		string temp = "";
		//密钥MAC计算
		memset(result, 0x00, 32);
		hexstrxor((char*)random_32.substr(0, 16).c_str(), (char*)data.substr(0, 16).c_str(), result);
		string data1 = result;

		memset(result, 0x00, 32);
		hexstrxor((char*)random_32.substr(16, 16).c_str(), (char*)data.substr(16, 16).c_str(), result);
		string data2 = result;

		string mac1 = SM4Enc(key, data1 + data2);

		memset(result, 0x00, 32);
		hexstrxor((char*)mac1.substr(0, 16).c_str(), (char*)data.substr(32, 16).c_str(), result);
		string data3 = result;

		memset(result, 0x00, 32);
		hexstrxor((char*)mac1.substr(16, 16).c_str(), (char*)data.substr(48, 16).c_str(), result);
		string data4 = result;

		string mac2 = SM4Enc(key, data3 + data4);

		memset(result, 0x00, 32);
		temp = data.substr(64, 10);
		temp += "800000";
		hexstrxor((char*)mac2.substr(0, 16).c_str(), (char*)temp.c_str(), result);
		string data5 = result;

		memset(result, 0x00, 32);
        char szZero[32] = {0};
        strcpy(szZero,(char *)"0000000000000000");
        hexstrxor((char*)mac2.substr(16, 16).c_str(), szZero, result);
		string data6 = result;

		string mac3 = SM4Enc(key, data5 + data6);

		memset(result, 0x00, 32);
		hexstrxor((char*)mac3.substr(0, 8).c_str(), (char*)mac3.substr(8, 8).c_str(), result);
		string mac4 = result;

		memset(result, 0x00, 32);
		hexstrxor((char*)mac4.c_str(), (char*)mac3.substr(16, 8).c_str(), result);
		string mac5 = result;

		memset(result, 0x00, 32);
		hexstrxor((char*)mac5.c_str(), (char*)mac3.substr(24, 8).c_str(), result);

		string mac6 = result;

		return mac6;
	}

    int Evolis_Z390_Printer::String2Char(string tempstring)
	{
        int nInputLen = tempstring.length();
        bool bRealloc = false;

        while(nOutMsgSize < (nInputLen +1))
        {
            nOutMsgSize*=2;
            bRealloc = true;
        }
        if (bRealloc)
        {
            delete []cpOutMsg;
            cpOutMsg = new char[nOutMsgSize];
            if (!cpOutMsg)
                return -1;
        }
        memset(cpOutMsg,0,nOutMsgSize);
        strncpy(cpOutMsg,tempstring.c_str(),nInputLen);
        return 0;
	}

    string& Evolis_Z390_Printer::replace_all(string& str, const   string& old_value, const   string& new_value)
	{
		while (true) {
			string::size_type   pos(0);
			if ((pos = str.find(old_value)) != string::npos)
				str.replace(pos, old_value.length(), new_value);
			else   break;
		}
		return   str;
	}

    string Evolis_Z390_Printer::CreateNode(std::string nodeName, std::string nodeVal)
	{
		std::string OutMsg = "";
		OutMsg = "<" + nodeName + ">";
		OutMsg += nodeVal;
		OutMsg += "</" + nodeName + ">";

		//Writelog_C(OutMsg, __FUNCTION__, __LINE__);
		return OutMsg;
	}

	/* 函数名:   fPubExtractXML
	* 函数功能：@xml解析程序，得到以<aaaa></aaaa>包裹的xml报文节点字符串。
	* 作者:     dongxiao
	* 字段说明:
	*     xmlStr:   输入的xml字符串
	*     code:     要提取的节点名
	*     content: 提取的节点值
	*     retMsg:   错误代码
	*/
    int Evolis_Z390_Printer::fPubExtractXML(char* xmlStr, char* code, char* content, char* retMsg)
	{
		std::string Resp;
		TiXmlDocument* spXmlDoc = new TiXmlDocument();
		spXmlDoc->Parse(xmlStr);//前7位是长度
		TiXmlElement* rootElement = spXmlDoc->RootElement();  //School元素   
		int RCode = 1;
		if (!rootElement)
		{
			delete spXmlDoc;
			Resp = "";
			return -2;
		}
		Resp = QueryNode_Text(rootElement, code);
		if (Resp == "")
		{
			strcpy(content, "");
			strcpy(retMsg, "获取节点失败");
			RCode = -1;
		}
		else
		{
			strcpy(content, Resp.c_str());
			strcpy(retMsg, "");
			RCode = 0;
		}
		delete spXmlDoc;
		return RCode;

		//	//	char fpubXmlStr[1024] = {0};
		//	//	strcat(fpubXmlStr,xmlStr);
		//	int m = 0;
		//	int t = 0;
		//
		//	char* buf = strstr(xmlStr, code);
		//	if (strcmp(xmlStr, "") == 0)
		//	{
		//		sprintf(retMsg, "传入数据为空:%d！", 23);
		//		return -2;
		//	}
		//	/* 若buf为空，表示没有取道code的值，即节点不存在 */
		//	if (buf == nullptr)
		//	{
		//		// printf( "节点%s不存在！！\n ",code); 
		//		sprintf(retMsg, "节点%s不存在！", code);
		//		return -1;
		//	}
		//
		//
		//	while (buf != nullptr)
		//	{
		//		buf[0] = '\0';
		//		t++;
		//		/* 循环第一次得到的xmlStr的值为code之前的字符串,
		//		* 循环第二次得到的即为 前后被">"和"</"包裹的节点值。
		//		* 需要掐头去尾
		//		*/
		//		if (t == 2) {
		//			m = strlen(xmlStr);
		//			/* 去除第一个字符和最后两个字符 */
		//			strncpy(content, xmlStr + 1, m - 3);
		//			/* trim结果值，去除尾部空格 */
		//			content[m - 3] = '\0';
		//		}
		//		xmlStr = buf + strlen(code);
		//		/* 得到下一个位置: */
		//		buf = strstr(xmlStr, code);
		//	}
		//	/* 若节点为空,不管是<aaaa></aaaa>格式或是<aaaa/>格式，得到的content都为空值 */
		//	// printf( "+++++++++++ 节点%s的值为%s\n ",code, content);
		//
		//	return 0;
		//	*/
		//
	}

    //string Evolis_Z390_Printer::QueryNode_Text(TiXmlElement* pRootEle, const char* strNodeName)
    string Evolis_Z390_Printer::QueryNode_Text(void* pRootEle1, const char* strNodeName)
    {
        TiXmlElement* pRootEle = (TiXmlElement*)pRootEle1;
		std::string strText = "";
		if (nullptr == pRootEle)
		{
			return "";
		}
		TiXmlElement* pNode = nullptr;
        GetNodePointerByName((void*)pRootEle, strNodeName, (void *&)pNode);
		if (nullptr != pNode)
		{
			strText = pNode->GetText();

		}
		return strText;
	}

   // using pTiXmlElement = pTiXmlElement*;
    bool Evolis_Z390_Printer::GetNodePointerByName(void* pRootEle1, const char* strNodeName, void*& vNode)
    {
        TiXmlElement* pRootEle = (TiXmlElement*) pRootEle1;
        TiXmlElement*& Node = (TiXmlElement*& )vNode;

		std::string temp;
        temp = strNodeName;
		int index = temp.find("/");
		if (index != -1)
		{
			if (strcmp(temp.substr(index + 1, temp.length() - index - 1).c_str(), pRootEle->Value()) == 0)
			{
				if (strcmp(temp.substr(0, index).c_str(), pRootEle->Parent()->Value()) == 0)
				{
					Node = pRootEle;
					return TRUE;
				}
			}
		}
		else if (strcmp(strNodeName, pRootEle->Value()) == 0)
		{
			Node = pRootEle;
			return TRUE;
		}
		TiXmlElement* pEle = pRootEle;
		for (pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
		{
            if (GetNodePointerByName(pEle, strNodeName, (void *&)Node))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

    int Evolis_Z390_Printer::fXMLExtract(char* xmlstr, char* code, char* content, char* retMsg)
	{
		//char szData[102400] = { 0 };
		int ret = 1;
		//strcat(szData, xmlstr);
		ret = fPubExtractXML(xmlstr, code, content, retMsg);
		if (ret != 0) return ret;
		return 0;
    }

    string Evolis_Z390_Printer::SM4Enc(string Key, string DATA)
	{
		//Writelog_C("KEY:" + Key + ",DATA:" + DATA, __FUNCTION__, __LINE__);
		unsigned char alResult[64] = { 0 };
		unsigned char alKey[64] = { 0 };
		int ilKeyLen = 0;
		unsigned char alData[64] = { 0 };
		int ilDataLen = 0;

		if (DATA.length() % 16 != 0)
		{
            size_t ilSub = 16 - (DATA.length() % 16 == 0 ? 16 : DATA.length() % 16);
			for (size_t i = 0; i < ilSub; i++)
			{
				DATA += "0";
			}
		}
		//Writelog_C("Padding DATA:" + DATA, __FUNCTION__, __LINE__);

		std::string outbuffer = "";
		for (size_t i = 0; i < DATA.length() / 32; i++)
		{
			memset(alKey, 0x00, 64);
			memset(alData, 0x00, 64);
			memset(alResult, 0x00, 64);
			if (hex_str2bytes((unsigned char*)Key.c_str(), alKey, &ilKeyLen))
				return "";
			if (hex_str2bytes((unsigned char*)DATA.substr(i * 32, 32).c_str(), alData, &ilDataLen))
				return "";
            proc_enc_dec((char *)"1", alKey, alData, alResult);
			outbuffer += print_hex(alResult, 16);
		}
		//Writelog_C("outbuffer:" + outbuffer, __FUNCTION__, __LINE__);
        return outbuffer;
    }

    int Evolis_Z390_Printer::WZ_CitizenCardAPDU(std::string Cmd, std::string CmdParam, char *pszRcCode)
    {
        Funclog();
        //温州市民卡命令
        RunlogF("CmdParam = %s",CmdParam.c_str());
        std::string  msg;
        std::string  retJSON = "";
        int index = Cmd.find(':');
        std::string temp = Cmd.substr(0, index);
        Cmd = Cmd.substr(index + 1);

        const char* pError;
        cJSON *json = cJSON_Parse((char *)CmdParam.c_str(),&pError);
        if (!json)
        {
            RunlogF("无法解析效的命令参数:%s",CmdParam.c_str());
            strcpy(pszRcCode,"0001");
            return 1;
        }

        if (0 == strcmp(temp.c_str(), "WriteCitizenCard_3303"))
        {
            //char outContent[1024] = { 0 };
            char szErr[1024] = { 0 };
            char temp[1024] = { 0 };
            char *out;

            switch (atoi(Cmd.c_str()))
            {
            case 1:
            {
                Funclog();
                std::string QYBZ;//启用标志
                std::string SJS;//随机数
                std::string FSYZ;//分散因子
                std::string FJKH;//非接卡号
                std::string CMD;//写卡数据

                cJSON *jsonQYBZ;
                jsonQYBZ = cJSON_GetObjectItem(json, "QYBZ");
                QYBZ = jsonQYBZ->valuestring;
                if (QYBZ.empty())
                {
                    RunlogF("解析QYBZ失败");
                    return 1;
                }

                CheckResult(RunApdu("00A4040008A000000632010105",msg));	//打开文件夹

                CheckResult(RunApdu("00B0950A0A",msg));	//获取对应数据
                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);
                FSYZ = msg.substr(4);


                CheckResult(RunApdu("0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";


                transform(QYBZ.begin(), QYBZ.end(), QYBZ.begin(), ::toupper);
                CMD = "04D6950905" + QYBZ;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "FACTOR", cJSON_CreateString(FSYZ.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(FJKH.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            case 2:
            {
                Funclog();
                std::string INSTRUCTION = "";//指令集(需写入)
                std::string QDRQ = "";//启动日期
                std::string YXRQ = "";//有效日期
                std::string SJS = "";//随机数
                std::string CMD = "";//写卡数据

                cJSON *jsonInstruction, *jsonQDRQ, *jsonYXRQ;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu(INSTRUCTION,msg));

                jsonQDRQ = cJSON_GetObjectItem(json, "QDRQ");
                QDRQ = jsonQDRQ->valuestring;
                if (QDRQ.empty())
                {
                    QDRQ = szErr;
                    RunlogF("解析QDRQ失败");
                    return 1;
                }

                jsonYXRQ = cJSON_GetObjectItem(json, "YXRQ");
                YXRQ = jsonYXRQ->valuestring;
                if (YXRQ.empty())
                {
                    YXRQ = szErr;
                    RunlogF("解析YXRQ失败");
                    return 1;
                }

                CheckResult(RunApdu("0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                CMD = "04D695140C" + QDRQ + YXRQ;
                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);

            }
            break;
            case 3:
            {
                Funclog();
                std::string INSTRUCTION = "";   //指令集(需写入)
                std::string MAINTYPE = "";      //主类型
                std::string SUBTYPE = "";       //子类型
                std::string SJS = "";           //随机数
                std::string CMD = "";           //写卡数据

                cJSON *jsonInstruction, *jsonMainType, *jsonSubType;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }
                //写入指令集
                CheckResult(RunApdu(INSTRUCTION,msg));

                jsonMainType = cJSON_GetObjectItem(json, "MAINTYPE");
                MAINTYPE = jsonMainType->valuestring;
                if (MAINTYPE.empty())
                {
                    MAINTYPE = szErr;
                    RunlogF("解析MAINTYPE失败");
                    return 1;
                }

                jsonSubType = cJSON_GetObjectItem(json, "SUBTYPE");
                SUBTYPE = jsonSubType->valuestring;
                if (SUBTYPE.empty())
                {
                    SUBTYPE = szErr;
                    RunlogF("解析SUBTYPE失败");
                    return 1;
                }

                CheckResult(RunApdu("0084000004",msg));
                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                CMD = "04D6851F06" + MAINTYPE + SUBTYPE;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            case 4:
            {
                Funclog();
                std::string INSTRUCTION = "";//指令集(需写入)
                std::string NAME = "";//姓名
                std::string CERTTYPE = "";//证件类型
                std::string CERTNUMBER = "";//证件号
                std::string SJS = "";//随机数
                std::string CMD = "";//写卡数据

                cJSON *jsonInstruction, *jsonName, *jsonCertType, *jsonCertNumber;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu(INSTRUCTION,msg));

                jsonName = cJSON_GetObjectItem(json, "NAME");
                NAME = jsonName->valuestring;
                if (NAME.empty())
                {
                    NAME = szErr;
                    RunlogF("解析NAME失败");
                    return 1;
                }
                jsonCertType = cJSON_GetObjectItem(json, "CERTTYPE");
                CERTTYPE = jsonCertType->valuestring;
                if (CERTTYPE.empty())
                {
                    CERTTYPE = szErr;
                    RunlogF("解析CERTTYPE失败");
                    return 1;
                }

                jsonCertNumber = cJSON_GetObjectItem(json, "CERTNUMBER");
                CERTNUMBER = jsonCertNumber->valuestring;
                if (CERTNUMBER.empty())
                {
                    CERTNUMBER = szErr;
                    RunlogF("解析CERTNUMBER失败");
                    return 1;
                }

                CheckResult(RunApdu("0084000004",msg));
                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                string strGBKName = Utf8_GBK(NAME.c_str());
                RunlogF("UTF-8 Name = %s --->GBK:%s.",NAME.c_str(),strGBKName.c_str());
                NAME = strGBKName;

                //20字节ASCII姓名+32字节ASCII证件号码+1字节BCD证件类型
                //hex_a((unsigned char*)NAME.c_str(), (unsigned char*)temp, NAME.length());
                Bin2Hexstring((unsigned char*)NAME.c_str(),NAME.size(),temp,1024);

                NAME = temp;
                transform(NAME.begin(), NAME.end(), NAME.begin(), ::toupper);
                int num = 0;
                num = 40 - NAME.length();//补齐数
                NAME = NAME + std::string(num, '0');

                memset(temp, 0, 1024);
                Bin2Hexstring((unsigned char*)CERTNUMBER.c_str(),CERTNUMBER.size(),temp,1024);

                CERTNUMBER = temp;
                transform(CERTNUMBER.begin(), CERTNUMBER.end(), CERTNUMBER.begin(), ::toupper);
                num = 64 - CERTNUMBER.length();//补齐数
                CERTNUMBER = CERTNUMBER + std::string(num, '0');

                CMD = "04D6960239" + NAME + CERTNUMBER + CERTTYPE;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            case 5:
            {
                Funclog();
                std::string INSTRUCTION = "";//指令集(需写入)

                cJSON *jsonInstruction;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }
                //写入指令集
                CheckResult(RunApdu(INSTRUCTION,msg));
                String2Char(retJSON);
            }
            break;
            case 6:
            {
                Funclog();
                std::string RFATR = "";//非接芯片ATR
                std::string FJKH = "";//非接卡号

                CheckResult(RunApdu("00B0950A0A",msg));	//获取对应数据
                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);

                //if (ReadType == DC_CARD)

                if (ResetCard_RF(RFATR, pszRcCode))
                {
                    RunlogF("Failed in reset card,can't get CardATR.");
                    return 1;
                }
                //RFATR = m_CardInfo.ATR;
                if (RFATR.empty())
                {
                    RunlogF("获取ATR失败");
                    return 1;
                }


                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "RFATR", cJSON_CreateString(RFATR.c_str()));
                cJSON_AddItemToObject(root_json, "FJKH", cJSON_CreateString(FJKH.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;

            default:
            {
                RunlogF("未知的命令: %s", Cmd.c_str());
                strcpy(pszRcCode, "0003");
                return 1;
            }
            break;
            }
            cJSON_Delete(json);
        }
        else
        {
            return 1;
        }

        return 0;
    }

    int Evolis_Z390_Printer::WZ_CitizenCardExAPDU(std::string Cmd, std::string CmdParam,LPVOID &lpCmdOut, char *pszRcCode)
    {
        std::string  msg;
        std::string  retJSON = "";
        int index = Cmd.find(':');
        std::string temp = Cmd.substr(0, index);
        RunlogF("Cmd = %s",Cmd.c_str());
        RunlogF("CmdParam = %s",CmdParam.c_str());
        Cmd = Cmd.substr(index + 1);
        RunlogF("CmdIndex = %s",Cmd.c_str());

//        QByteArray baJson((char*)CmdParam.c_str(), CmdParam.size());
//        QJsonParseError jsErr;
//        QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
//        if (jsdoc.isNull())
//        {
//            RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
//            // try to analysis with GBK
//            string strJson = GBK_Utf8(CmdParam.c_str());
//            QByteArray baJson((char*)strJson.c_str(), strJson.size());
//            jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
//            if (jsdoc.isNull())
//            {
//                RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
//                return -1;
//            }

//        }
//        QJsonObject jsobj = jsdoc.object();
        //char szErr[1024] = { 0 };
        //strcpy(cpOutMsg,"Test String");
        //lpCmdOut = cpOutMsg;

        switch (atoi(Cmd.c_str()))
        {
        case 1:
        {
            QByteArray baJson((char*)CmdParam.c_str(), CmdParam.size());
            QJsonParseError jsErr;
            QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
            if (jsdoc.isNull())
            {
                RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                // try to analysis with GBK
                string strJson = GBK_Utf8(CmdParam.c_str());
                QByteArray baJson((char*)strJson.c_str(), strJson.size());
                jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
                if (jsdoc.isNull())
                {
                    RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                    return -1;
                }

            }
            QJsonObject jsobj = jsdoc.object();
            RunlogF("Step1 = %s",Cmd.c_str());
            cJSON *root_json = cJSON_CreateObject();
            root_json = cJSON_CreateObject();//创建项目

            //CheckResult(RunApdu(m_hReader, "00A40000023F00",msg));
            // 复位以获得ATR
            ResetCard(pszRcCode);
            CheckResult(SelectSSSE(msg));
            CheckResult(SelectDir("EF05",msg));
            CheckResult(ReadFile("01",msg));

            //msg = "0110330300D15600000501005794E4B2CBAC9000";
            m_CardInfo.identifyNum = msg.substr(4, msg.length() - 8);
            m_CardInfo.regionCode = m_CardInfo.identifyNum.substr(0, 6);

            CheckResult(ReadFile("03",msg));      // 获取卡版本号
            m_CardInfo.cardVersion = GetTlvValue(msg,msg.size() - 4);
            RunlogF("CardVersion = %s",m_CardInfo.cardVersion.c_str());

            CheckResult(ReadFile("04",msg));      // 获取Initial Organize

            CheckResult(ReadFile("05",msg));      // 获取CardReleaseDate
            m_CardInfo.cardReleaseDate = GetTlvValue(msg,msg.size() - 4);
            RunlogF("cardReleaseDate = %s",m_CardInfo.cardReleaseDate.c_str());

            CheckResult(ReadFile("06",msg));      // 获取CardValidDate
            m_CardInfo.cardValidDate = GetTlvValue(msg,msg.size() - 4);
            RunlogF("cardValidDate = %s",m_CardInfo.cardValidDate.c_str());

            CheckResult(ReadFile("07",msg));      // 获取CardNumber
            m_CardInfo.cardNumber = GetTlvValue(msg,msg.size() -4);
            RunlogF("cardNumber = %s",m_CardInfo.cardNumber.c_str());

            //四个随机数
            std::vector<std::string> vRandom;
            for (int i = 0; i < 4; i++)
            {
                //msg = "E1B5773ACE7F699C9000";
                CheckResult(GetRandom(msg));
                vRandom.push_back(msg.substr(0, 16));
            }

            cJSON_AddItemToObject(root_json, "AAB301", cJSON_CreateString(m_CardInfo.regionCode.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ507", cJSON_CreateString(m_CardInfo.ATR.c_str()));
            cJSON_AddItemToObject(root_json, "AFLAG", cJSON_CreateString("03"));
            cJSON_AddItemToObject(root_json, "AAZ501", cJSON_CreateString(m_CardInfo.identifyNum.c_str()));
            cJSON_AddItemToObject(root_json, "SJS1", cJSON_CreateString(vRandom[0].c_str()));
            cJSON_AddItemToObject(root_json, "SJS2", cJSON_CreateString(vRandom[1].c_str()));
            cJSON_AddItemToObject(root_json, "SJS3", cJSON_CreateString(vRandom[3].c_str()));
            cJSON_AddItemToObject(root_json, "SJS4", cJSON_CreateString(vRandom[2].c_str()));
            cJSON_AddItemToObject(root_json, "MSGNO", cJSON_CreateString("9013"));

            retJSON = cJSON_PrintUnformatted(root_json);
            cJSON_Delete(root_json);
            String2Char(retJSON);
            lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();
            RunlogF("lpCmdout = %s.",(char*)lpCmdOut);
        }
        break;
        case 2:
        {
            QByteArray baJson((char*)CmdParam.c_str(), CmdParam.size());
            QJsonParseError jsErr;
            QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
            if (jsdoc.isNull())
            {
                RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                // try to analysis with GBK
                string strJson = GBK_Utf8(CmdParam.c_str());
                QByteArray baJson((char*)strJson.c_str(), strJson.size());
                jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
                if (jsdoc.isNull())
                {
                    RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                    return -1;
                }
            }
            QJsonObject jsobj = jsdoc.object();
            RunlogF("Step2 = %s",Cmd.c_str());
            std::string	RESULT1 = "",RESULT2 = "";
            if (!jsobj.contains("RESULT2"))
            {
                RunlogF("Can't find 'RESULT2' object!");
                strcpy(pszRcCode, "0002");
                return -1;
            }

            RESULT2 = jsobj.value("RESULT2").toString().toStdString();
            if (RESULT2.empty())
            {
                strcpy(pszRcCode, "0003");
                RunlogF("RESULT2 can't be empty!");
                return 1;
            }
            RunlogF("RESULT2 = %s",RESULT2.c_str());

            CheckResult(ExternalAuth("0A10",RESULT2,msg));      // 读认证

            CheckResult(ReadFile("07",msg));
            m_CardInfo.cardNumber = GetTlvValue(msg,msg.size() -4);
            RunlogF("Card Number = %s",m_CardInfo.cardNumber.c_str());

            CheckResult(SelectDir("EF06",msg));
            CheckResult(ReadFile("01",msg));
            m_CardInfo.cardID = GetTlvValue(msg,msg.size() -4);
            RunlogF("CardID = %s",m_CardInfo.cardID.c_str());

            CheckResult(ReadFile("02",msg));
            m_CardInfo.name = GBK_Utf8(GetTlvValue(msg,msg.size() -4).c_str());
            RunlogF("Name = %s",m_CardInfo.name.c_str());

            //checkName(m_CardInfo.cardID, "0", "");

            RunlogF("Region Code = %s",m_CardInfo.regionCode.c_str());

            CheckResult(RunApdu("0084000008",msg));
            //msg = "E1B5773ACE7F699C9000";
            string strOData = msg.substr(0, 16);
            //msg = "13184466C59FB6B49000";
            CheckResult(RunApdu("0084000008",msg));
            string strDisFac = msg.substr(0, 16);

            cJSON *root_json = cJSON_CreateObject();
            cJSON_AddItemToObject(root_json, "AAB301", cJSON_CreateString(m_CardInfo.regionCode.substr(0, 6).c_str()));
            cJSON_AddItemToObject(root_json, "AAC002", cJSON_CreateString(m_CardInfo.cardID.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ500", cJSON_CreateString(m_CardInfo.cardNumber.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ501", cJSON_CreateString(m_CardInfo.identifyNum.c_str()));
            cJSON_AddItemToObject(root_json, "AAC003", cJSON_CreateString(m_CardInfo.name.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ507", cJSON_CreateString(m_CardInfo.ATR.c_str()));
            cJSON_AddItemToObject(root_json, "AFLAG", cJSON_CreateString("03"));
            cJSON_AddItemToObject(root_json, "KEYADD", cJSON_CreateString("0094"));
            cJSON_AddItemToObject(root_json, "KEYFAC", cJSON_CreateString(strDisFac.c_str()));
            cJSON_AddItemToObject(root_json, "ODATA", cJSON_CreateString(strOData.c_str()));
            cJSON_AddItemToObject(root_json, "MSGNO", cJSON_CreateString("9011"));

            retJSON = cJSON_PrintUnformatted(root_json);
            String2Char(retJSON);
            lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();
            RunlogF("lpCmdout = %s.",(char*)lpCmdOut);
            cJSON_Delete(root_json);
        }
        break;
        case 3:
        {
            QByteArray baJson((char*)CmdParam.c_str(), CmdParam.size());
            QJsonParseError jsErr;
            QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
            if (jsdoc.isNull())
            {
                RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                // try to analysis with GBK
                string strJson = GBK_Utf8(CmdParam.c_str());
                QByteArray baJson((char*)strJson.c_str(), strJson.size());
                jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
                if (jsdoc.isNull())
                {
                    RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                    return -1;
                }

            }
            QJsonObject jsobj = jsdoc.object();
            RunlogF("Step3 = %s",Cmd.c_str());
            if (!jsobj.contains("RESULT") || !jsobj.contains("PIC"))
            {
                RunlogF("Can't find 'RESULT' or 'PIC' object!");
                strcpy(pszRcCode, "0002");
                return -1;
            }

            std::string	RESULT = jsobj.value("RESULT").toString().toStdString();
            RunlogF("Result = %s",RESULT.c_str());
            std::string	PIC = jsobj.value("PIC").toString().toStdString();
            if (RESULT.empty() ||PIC.empty())
            {
                RunlogF("RESULT or Pic can't be empty!");
                strcpy(pszRcCode, "0002");
                return -1;
            }

            // 注意：同样的参数，用cjson解析会失败，找不到Result字段，用QJson则不会
            /*cJSON *json_RESULT = cJSON_GetObjectItem(json, "RESULT");
            RunlogF("json string = %s.", cJSON_PrintUnformatted(json));
            if (nullptr == json_RESULT )
            {
                RunlogF("Can't find 'RESULT' object!");
                strlcpy(pszRcCode, "0002",8);
                return -1;
            }

            std::string	PIC = "";
            RESULT = json_RESULT->valuestring;
            if (RESULT.empty())
            {
                strlcpy(pszRcCode, "0002",8);
                RunlogF("解析RESULT失败:%s",RESULT.c_str());
                return 1;
            }
            RunlogF("Result = %s",RESULT.c_str());

            cJSON *json_PIC = cJSON_GetObjectItem(json, "PIC");
            if (nullptr == json_PIC)
            {
                RunlogF("Can't find 'PIC' object!");
                strlcpy(pszRcCode, "0002",8);
                return -1;
            }
            PIC = json_PIC->valuestring;
            if (PIC.empty())
            {
                strlcpy(pszRcCode, "0002",8);
                RunlogF("解析RESULT失败:%s",PIC.c_str());
                return 1;
            }*/

            //压缩图片数据
            std::string strCompressedPic = "";
            if (CompressPic(PIC, strCompressedPic) != 0)
            {
                RunlogF("压缩图片失败:%s");
                strcpy(pszRcCode, "0005");
                return 1;
            }

            CheckResult(ExternalAuth("0410",RESULT,msg));       // 写认证
            CheckResult(SelectDir("EF08",msg));

            //写入照片数据
            if(WriteImageInfo(strCompressedPic,pszRcCode) != 0)
            {
                return 1;
            }

            CheckResult(SelectDir("DF01",msg));
            CheckResult(GetRandom(msg));
            //msg = "E1B5773ACE7F699C9000";
            string strOData = msg.substr(0, 16);

            CheckResult(GetRandom(msg));
            //msg = "13184466C59FB6B49000";
            string strDisFac = msg.substr(0, 16);

            RunlogF("Try to Create json object!");
            cJSON *root_json = cJSON_CreateObject();
            //shared_ptr<cJSON> JsonDestruct(root_json,cJSON_Delete);
            cJSON_AddItemToObject(root_json, "AAB301", cJSON_CreateString(m_CardInfo.identifyNum.substr(0, 6).c_str()));
            cJSON_AddItemToObject(root_json, "AAC002", cJSON_CreateString(m_CardInfo.cardID.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ500", cJSON_CreateString(m_CardInfo.cardNumber.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ501", cJSON_CreateString(m_CardInfo.identifyNum.c_str()));
            cJSON_AddItemToObject(root_json, "AAC003", cJSON_CreateString(m_CardInfo.name.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ507", cJSON_CreateString(m_CardInfo.ATR.c_str()));
            cJSON_AddItemToObject(root_json, "AFLAG", cJSON_CreateString("03"));
            cJSON_AddItemToObject(root_json, "KEYADD", cJSON_CreateString("0179"));
            cJSON_AddItemToObject(root_json, "KEYFAC", cJSON_CreateString(strDisFac.c_str()));
            cJSON_AddItemToObject(root_json, "ODATA", cJSON_CreateString(strOData.c_str()));
            cJSON_AddItemToObject(root_json, "MSGNO", cJSON_CreateString("9011"));

            retJSON = cJSON_PrintUnformatted(root_json);
            String2Char(retJSON);
            //strcpy(cpOutMsg,"Test String!");
            lpCmdOut = cpOutMsg;
            cJSON_Delete(root_json);
            RunlogF("lpCmdout = %s.",(char*)lpCmdOut);
        }
        break;
        case 4:
        {
            RunlogF("Step4 = %s",Cmd.c_str());
//            QByteArray baJson((char*)CmdParam.c_str(), CmdParam.size());
//            QJsonParseError jsErr;
//            QJsonDocument jsdoc = QJsonDocument::fromRawData(baJson, &jsErr);
//            if (jsdoc.isNull())
//            {
//                RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
//                // try to analysis with GBK
//                string strJson = GBK_Utf8(CmdParam.c_str());
//                QByteArray baJson((char*)strJson.c_str(), strJson.size());
//                jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
//                if (jsdoc.isNull())
//                {
//                    RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
//                    return -1;
//                }
//            }
//            QJsonObject jsobj = jsdoc.object();
            // {"RESULT":"8D1CF983465BB67EAEB20632C4D0F7AD","ADDR":"浙江省温州市龙湾区张家","PHONE":"18888179633"}
//            std::string RANDOM1, RANDOM2;
//            if (!jsobj.contains("RESULT") ||
//                !jsobj.contains("ADDR") ||
//                !jsobj.contains("PHONE") ||
//                !jsobj.contains("ADDRCODE"))
//            {
//                RunlogF("Can't find 'RESULT','ADDR','ADDRCODE' or 'PHONE' object!");
//                strcpy(pszRcCode, "0002");
//                return -1;
//            }
//            string strResult     = jsobj.value("RESULT").toString().toStdString();
//            string strAddr       = jsobj.value("ADDR").toString().toStdString();
//            string strAddrCode   = jsobj.value("ADDRCODE").toString().toStdString();
//            string strPhone      = jsobj.value("PHONE").toString().toStdString();
//            if (strResult.empty() || strAddr.empty() || strAddrCode.empty() || strPhone.empty())
//            {
//                RunlogF("Can't find 'RESULT','ADDR','ADDRCODE' or 'PHONE' can't be empty!");
//                strcpy(pszRcCode, "0002");
//                return -1;
//            }
            const char* pError;
            cJSON *json = cJSON_Parse((char *)CmdParam.c_str(),&pError);
            if (!json)
            {
                RunlogF("无法解析效的命令参数:%s",CmdParam.c_str());
                strcpy(pszRcCode,"0001");
                return 1;
            }
            cJSON* root_json = cJSON_CreateObject();
            std::string	strResult = "", strAddr = "", strAddrCode = "", strPhone = "";
            cJSON* json_RESULT, * json_ADDR, * json_ADDRCODE, * json_PHONE;
            json_RESULT = cJSON_GetObjectItem(json, "RESULT");
            strResult = json_RESULT->valuestring;
            if (strResult.empty())
            {
                strcpy(pszRcCode, "0002");
                RunlogF("解析RESULT失败,Err:%s" ,strResult.c_str());
                return 1;
            }

            json_ADDR = cJSON_GetObjectItem(json, "ADDR");
            strAddr = json_ADDR->valuestring;
            if (strAddr.empty())
            {
                strcpy(pszRcCode, "0003");
                RunlogF("解析ADDR失败,Err:%s" ,strAddr.c_str());
                return 1;
            }

            json_ADDRCODE = cJSON_GetObjectItem(json, "ADDRCODE");
            strAddrCode = json_ADDRCODE->valuestring;
//            if (strAddrCode.empty())
//            {
//                strcpy(pszRcCode, "0004");
//                RunlogF("解析ADDRCODE失败,Err:%s" , strAddrCode.c_str());
//                return 1;
//            }

            json_PHONE = cJSON_GetObjectItem(json, "PHONE");
            strPhone = json_PHONE->valuestring;
            if (strPhone.empty())
            {
                strcpy(pszRcCode, "0005");
                RunlogF("解析PHONE失败,Err:%s" ,strPhone.c_str());
                return 1;
            }
            RunlogF("Result = %s\tAddr = %s\tAddrCode = %s\tPhone = %s",strResult.c_str(),strAddr.c_str(),strAddrCode.c_str(),strPhone.c_str());

            RunlogF("GBK(Addr) = %s",Utf8_GBK(strAddr.c_str()).c_str());
            CheckResult(ExternalAuth("8610",strResult, msg));       // 写认证

            strAddr = GetTlvString(Utf8_GBK(strAddr.c_str()).c_str(),160);

            strPhone = GetTlvString(strPhone,30);

            CheckResult(SelectDir("EF06",msg));
            CheckResult(WriteFile("01", "23", strAddr.c_str(),msg));
            CheckResult(WriteFile("02", "24", strAddrCode,msg));
            CheckResult(WriteFile("03", "28", strPhone,msg));

            CheckResult(GetRandom(msg));
            string strOData = msg.substr(0, 16);

            CheckResult(GetRandom(msg));
            string strDisFac = msg.substr(0, 16);

            root_json = cJSON_CreateObject();
            //shared_ptr<cJSON> JsonDestruct(root_json,cJSON_Delete);
            cJSON_AddItemToObject(root_json, "AAB301", cJSON_CreateString(m_CardInfo.regionCode.substr(0, 6).c_str()));
            cJSON_AddItemToObject(root_json, "AAC002", cJSON_CreateString(m_CardInfo.cardID.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ500", cJSON_CreateString(m_CardInfo.cardNumber.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ501", cJSON_CreateString(m_CardInfo.identifyNum.c_str()));
            cJSON_AddItemToObject(root_json, "AAC003", cJSON_CreateString(m_CardInfo.name.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ507", cJSON_CreateString(m_CardInfo.ATR.c_str()));
            cJSON_AddItemToObject(root_json, "AFLAG", cJSON_CreateString("03"));
            cJSON_AddItemToObject(root_json, "KEYADD", cJSON_CreateString("009A"));
            cJSON_AddItemToObject(root_json, "KEYFAC", cJSON_CreateString(strDisFac.c_str()));
            cJSON_AddItemToObject(root_json, "ODATA", cJSON_CreateString(strOData.c_str()));
            cJSON_AddItemToObject(root_json, "MSGNO", cJSON_CreateString("9011"));

            retJSON = cJSON_PrintUnformatted(root_json);
            cJSON_Delete(root_json);
            String2Char(retJSON);
            lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();
            RunlogF("lpCmdout = %s.",(char*)lpCmdOut);
        }
        break;
        case 5:
        {
            QByteArray baJson((char*)CmdParam.c_str(), CmdParam.size());
            QJsonParseError jsErr;
            QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
            if (jsdoc.isNull())
            {
                RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                // try to analysis with GBK
                string strJson = GBK_Utf8(CmdParam.c_str());
                QByteArray baJson((char*)strJson.c_str(), strJson.size());
                jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
                if (jsdoc.isNull())
                {
                    RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                    return -1;
                }

            }
            QJsonObject jsobj = jsdoc.object();
            RunlogF("Step5 = %s",Cmd.c_str());
            std::string	RESULT = "", ORGCODE = "";
            if (!jsobj.contains("RESULT") ||
                !jsobj.contains("ORGCODE"))
            {
                RunlogF("Can't find 'RESULT' or 'ORGCODE' object!");
                strcpy(pszRcCode, "0002");
                return -1;
            }
            RESULT = jsobj.value("RESULT").toString().toStdString();
            ORGCODE = jsobj.value("ORGCODE").toString().toStdString();

            if (RESULT.empty())
            {
                strcpy(pszRcCode, "0002");
                RunlogF("'RESULT' can't be empty!");
                return 1;
            }

            RunlogF("RESULT = %s\tORGCODE = %s",RESULT.c_str(),ORGCODE.c_str());

            CheckResult(ExternalAuth("8410", RESULT,msg));      // 写入认证

            CheckResult(SelectDir("EF09",msg));

            ORGCODE = GetTlvString(ORGCODE,18);

            CheckResult(WriteFile("02", "30", ORGCODE, msg));

            CheckResult(GetRandom(msg));
            string strOData = msg.substr(0, 16);

            CheckResult(GetRandom(msg));
            string strDisFac = msg.substr(0, 16);

            cJSON *root_json = cJSON_CreateObject();
            cJSON_AddItemToObject(root_json, "AAB301", cJSON_CreateString(m_CardInfo.regionCode.substr(0, 6).c_str()));
            cJSON_AddItemToObject(root_json, "AAC002", cJSON_CreateString(m_CardInfo.cardID.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ500", cJSON_CreateString(m_CardInfo.cardNumber.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ501", cJSON_CreateString(m_CardInfo.identifyNum.c_str()));
            cJSON_AddItemToObject(root_json, "AAC003", cJSON_CreateString(m_CardInfo.name.c_str()));
            cJSON_AddItemToObject(root_json, "AAZ507", cJSON_CreateString(m_CardInfo.ATR.c_str()));
            cJSON_AddItemToObject(root_json, "AFLAG", cJSON_CreateString("03"));
            cJSON_AddItemToObject(root_json, "KEYADD", cJSON_CreateString("009D"));
            cJSON_AddItemToObject(root_json, "KEYFAC", cJSON_CreateString(strDisFac.c_str()));
            cJSON_AddItemToObject(root_json, "ODATA", cJSON_CreateString(strOData.c_str()));
            cJSON_AddItemToObject(root_json, "MSGNO", cJSON_CreateString("9011"));

            retJSON = cJSON_PrintUnformatted(root_json);
            String2Char(retJSON);
            cJSON_Delete(root_json);
            lpCmdOut = cpOutMsg;// (LPVOID)retMsg.c_str();
            RunlogF("lpCmdout = %s.",(char*)lpCmdOut);
        }
        break;
        case 6:
        {
            QByteArray baJson((char*)CmdParam.c_str(), CmdParam.size());
            QJsonParseError jsErr;
            QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
            if (jsdoc.isNull())
            {
                RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                // try to analysis with GBK
                string strJson = GBK_Utf8(CmdParam.c_str());
                QByteArray baJson((char*)strJson.c_str(), strJson.size());
                jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
                if (jsdoc.isNull())
                {
                    RunlogF("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
                    return -1;
                }

            }
            QJsonObject jsobj = jsdoc.object();
            RunlogF("Step6 = %s",Cmd.c_str());
            std::string	RESULT = "", COUNTRY = "";
            if (!jsobj.contains("RESULT") ||
                !jsobj.contains("COUNTRY"))
            {
                RunlogF("Can't find 'RESULT' or 'COUNTRY' object!");
                strcpy(pszRcCode, "0002");
                return -1;
            }
            RESULT = jsobj.value("RESULT").toString().toStdString();
            COUNTRY = jsobj.value("COUNTRY").toString().toStdString();

            if (RESULT.empty() )
            {
                strcpy(pszRcCode, "0002");
                RunlogF("'RESULT' can't be empty!");
                return 1;
            }
            RunlogF("RESULT = %s\tORGCODE = %s",RESULT.c_str(),COUNTRY.c_str());
            CheckResult(ExternalAuth("8710", RESULT,msg));
            CheckResult(SelectDir("EF0A",msg));

            COUNTRY = GetTlvString(COUNTRY,6);

            CheckResult(WriteFile("01", "37", COUNTRY, msg));
        }
        break;
        default:
        {
            RunlogF("未知的命令: %s", Cmd.c_str());
            strcpy(pszRcCode, "0100");
            return 1;
        }
        break;
        }

        strcpy(pszRcCode, "0000");
        return 0;
    }

    int Evolis_Z390_Printer::JX_CitizenCardAPDU(std::string Cmd, std::string CmdParam, char *pszRcCode)
    {
        //嘉兴市民卡
        RunlogF("CmdParam = %s",CmdParam.c_str());
        std::string  msg;
        std::string  retJSON = "";
        int index = Cmd.find(':');
        std::string temp = Cmd.substr(0, index);
        Cmd = Cmd.substr(index + 1);

        const char* pError;
        cJSON *json = cJSON_Parse((char *)CmdParam.c_str(),&pError);
        if (!json)
        {
            RunlogF("无法解析效的命令参数:%s",CmdParam.c_str());
            strcpy(pszRcCode,"0001");
            return 1;
        }

        if (0 == strcmp(temp.c_str(), "WriteCitizenCard_3304"))
        {
            char *out;
            switch (atoi(Cmd.c_str()))
            {
            case 1:
            {
                std::string RFATR = "";//非接芯片ATR
                std::string FJKH = "";//非接卡号

                CheckResult(RunApdu("00A4040008A000000632010105",msg));	//打开文件夹
                if (msg.substr(msg.length() - 4, 4) != "9000")
                {
                    RunlogF("打开文件夹失败");
                }

                CheckResult(RunApdu( "00B0950A0A",msg));	//获取对应数据
                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);

                if (ResetCard_RF(RFATR, pszRcCode))
                {
                    RunlogF("Failed in reset card,can't get CardATR.");
                    return 1;
                }
                //RFATR = m_CardInfo.ATR;
                if (RFATR.empty())
                {
                    RunlogF("获取ATR失败");
                    return 1;
                }

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "RFATR", cJSON_CreateString(RFATR.c_str()));
                cJSON_AddItemToObject(root_json, "FJKH", cJSON_CreateString(FJKH.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;

            default:
            {
                RunlogF("未知的命令: %s", Cmd.c_str());
                strcpy(pszRcCode, "0003");
                return 1;
            }
            break;
            }
            cJSON_Delete(json);
        }
        else
        {
            return 1;
        }
        return 0;
    }

    int Evolis_Z390_Printer::SX_CitizenCardAPDU(std::string Cmd, std::string CmdParam, char * pszRcCode)
    {
        RunlogF("CmdParam = %s",CmdParam.c_str());
        std::string  msg;
        std::string  retJSON = "";
        int index = Cmd.find(':');
        std::string temp = Cmd.substr(0, index);
        Cmd = Cmd.substr(index + 1);

        const char* pError;
        cJSON *json = cJSON_Parse((char *)CmdParam.c_str(),&pError);
        if (!json)
        {
            RunlogF("无法解析效的命令参数:%s",CmdParam.c_str());
            strcpy(pszRcCode,"0001");
            return 1;
        }

        if (0 == strcmp(temp.c_str(), "WriteCitizenCard_3306"))
        {
            char szErr[1024] = { 0 };
            char temp[1024] = { 0 };
            char *out;

            switch (atoi(Cmd.c_str()))
            {
            case 1:
            {
                std::string SJS;//随机数
                std::string FSYZ;//分散因子
                std::string FJKH;//非接卡号
                std::string CMD;//写卡数据
                std::string MAINTYPE;//主类型
                std::string SUBTYPE;//子类型

                cJSON *jsonMAINTYPE;
                jsonMAINTYPE = cJSON_GetObjectItem(json, "MAINTYPE");
                MAINTYPE = jsonMAINTYPE->valuestring;
                if (MAINTYPE.empty())
                {
                    MAINTYPE = szErr;
                    RunlogF("解析MAINTYPE失败");
                    return 1;
                }

                cJSON *jsonSUBTYPE;
                jsonSUBTYPE = cJSON_GetObjectItem(json, "SUBTYPE");
                SUBTYPE = jsonSUBTYPE->valuestring;
                if (SUBTYPE.empty())
                {
                    SUBTYPE = szErr;
                    RunlogF("解析SUBTYPE失败");
                    return 1;
                }

                CheckResult(RunApdu( "00A4040008A000000632010105",msg));	//打开文件夹

                CheckResult(RunApdu( "00B0950A0A",msg));	//获取对应数据

                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);
                FSYZ = msg.substr(4);

                CheckResult(RunApdu( "0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                CMD = "04D6950905" + MAINTYPE + SUBTYPE;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "FACTOR", cJSON_CreateString(FSYZ.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(FJKH.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            case 2:
            {
                std::string INSTRUCTION = "";//指令集(需写入)
                std::string NAME = "";//姓名
                std::string CERTTYPE = "";//证件类型
                std::string CERTNUMBER = "";//证件号
                std::string SJS = "";//随机数
                std::string CMD = "";//写卡数据

                cJSON *jsonInstruction, *jsonName, *jsonCertType, *jsonCertNumber;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu( INSTRUCTION,msg));

                RunlogF("写入指令集失败");
                return 1;


                jsonName = cJSON_GetObjectItem(json, "NAME");
                NAME = jsonName->valuestring;
                if (NAME.empty())
                {
                    NAME = szErr;
                    RunlogF("解析NAME失败");
                    return 1;
                }

                jsonCertType = cJSON_GetObjectItem(json, "CERTTYPE");
                CERTTYPE = jsonCertType->valuestring;
                if (CERTTYPE.empty())
                {
                    CERTTYPE = szErr;
                    RunlogF("解析CERTTYPE失败");
                    return 1;
                }

                jsonCertNumber = cJSON_GetObjectItem(json, "CERTNUMBER");
                CERTNUMBER = jsonCertNumber->valuestring;
                if (CERTNUMBER.empty())
                {
                    CERTNUMBER = szErr;
                    RunlogF("解析CERTNUMBER失败");
                    return 1;
                }

                CheckResult(RunApdu( "0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                //20字节ASCII姓名+32字节ASCII证件号码+1字节BCD证件类型
                pReader->hex_a((unsigned char*)NAME.c_str(), (unsigned char*)temp, NAME.length());
                NAME = temp;
                transform(NAME.begin(), NAME.end(), NAME.begin(), ::toupper);
                int num = 0;
                num = 40 - NAME.length();//补齐数
                NAME = NAME + std::string(num, '0');

                memset(temp, 0, 1024);
                pReader->hex_a((unsigned char*)CERTNUMBER.c_str(), (unsigned char*)temp, CERTNUMBER.length());
                CERTNUMBER = temp;
                transform(CERTNUMBER.begin(), CERTNUMBER.end(), CERTNUMBER.begin(), ::toupper);
                num = 64 - CERTNUMBER.length();//补齐数
                CERTNUMBER = CERTNUMBER + std::string(num, '0');

                CMD = "04D6960239" + NAME + CERTNUMBER + CERTTYPE;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);

                String2Char(retJSON);
            }
            break;
            case 3:
            {
                std::string SJS = "";//随机数
                std::string CMD = "";//写卡数据
                std::string YXRQ = "";//有效日期
                cJSON *jsonYXRQ;
                jsonYXRQ = cJSON_GetObjectItem(json, "YXRQ");
                YXRQ = jsonYXRQ->valuestring;
                if (YXRQ.empty())
                {
                    YXRQ = szErr;
                    RunlogF("解析YXRQ失败");
                    return 1;
                }

                std::string INSTRUCTION = "";//指令集(需写入)
                cJSON *jsonInstruction;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                CheckResult(RunApdu( "0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";
                CMD = "04D6951808" + YXRQ;

                //写入指令集
                CheckResult(RunApdu( INSTRUCTION,msg));

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);

                String2Char(retJSON);
            }
            break;
            case 4:
            {
                std::string INSTRUCTION = "";//指令集(需写入)
                cJSON *jsonInstruction;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu( INSTRUCTION,msg));
            }
            break;
            case 5:
            {
                std::string RFATR = "";//非接芯片ATR
                std::string FJKH = "";//非接卡号

                CheckResult(RunApdu( "00B0950A0A",msg));	//获取对应数据
                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);

                if (ResetCard_RF(RFATR, pszRcCode))
                {
                    RunlogF("Failed in reset card,can't get CardATR.");
                    return 1;
                }
                //RFATR = m_CardInfo.ATR;
                if (RFATR.empty())
                {
                    RunlogF("获取ATR失败");
                    return 1;
                }


                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "RFATR", cJSON_CreateString(RFATR.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(FJKH.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            default:
            {
                RunlogF("未知的命令: %s", Cmd.c_str());
                strcpy(pszRcCode, "0003");
                return 1;
            }
            break;
            }
            cJSON_Delete(json);
        }
        else
        {
            return 1;
        }
        return 0;
    }

    int Evolis_Z390_Printer::ZS_CitizenCardAPDU(std::string Cmd, std::string CmdParam, char * pszRcCode)
    {
        RunlogF("CmdParam = %s",CmdParam.c_str());
        std::string  msg;
        std::string  retJSON = "";
        int index = Cmd.find(':');
        std::string temp = Cmd.substr(0, index);
        Cmd = Cmd.substr(index + 1);

        const char* pError;
        cJSON *json = cJSON_Parse((char *)CmdParam.c_str(),&pError);
        if (!json)
        {
            RunlogF("无法解析效的命令参数:%s",CmdParam.c_str());
            strcpy(pszRcCode,"0001");
            return 1;
        }

        if (0 == strcmp(temp.c_str(), "WriteCitizenCard_3309"))
        {
            //char szErr[1024] = { 0 };
            char temp[1024] = { 0 };
            char *out;
            switch (atoi(Cmd.c_str()))
            {
            case 1:
            {
                std::string SJS;//随机数
                std::string FSYZ;//分散因子
                std::string FJKH;//非接卡号
                std::string CMD;//写卡数据
                std::string NAME;//姓名
                std::string CERTTYPE;//证件类型
                std::string CERTNUMBER;//证件号码
                std::string SEX;//性别
                std::string CONTACTNUMBER;//接触卡号
                std::string BANKNUMBER;//银行卡号


                cJSON *jsonNAME;
                jsonNAME = cJSON_GetObjectItem(json, "NAME");
                NAME = jsonNAME->valuestring;
                if (NAME.empty())
                {
                    RunlogF("解析姓名失败");
                    return 1;
                }

                cJSON *jsonCERTTYPE;
                jsonCERTTYPE = cJSON_GetObjectItem(json, "CERTTYPE");
                CERTTYPE = jsonCERTTYPE->valuestring;
                if (CERTTYPE.empty())
                {
                    RunlogF("解析证件类型失败");
                    return 1;
                }


                cJSON *jsonCERTNUMBER;
                jsonCERTNUMBER = cJSON_GetObjectItem(json, "CERTNUMBER");
                CERTNUMBER = jsonCERTNUMBER->valuestring;
                if (CERTNUMBER.empty())
                {
                    RunlogF("解析证件号码失败");
                    return 1;
                }

                cJSON *jsonSEX;
                jsonSEX = cJSON_GetObjectItem(json, "SEX");
                SEX = jsonSEX->valuestring;
                if (SEX.empty())
                {
                    RunlogF("解析性别失败");
                    return 1;
                }

                cJSON *jsonCONTACTNUMBER;
                jsonCONTACTNUMBER = cJSON_GetObjectItem(json, "CONTACTNUMBER");
                CONTACTNUMBER = jsonCONTACTNUMBER->valuestring;
                if (CONTACTNUMBER.empty())
                {
                    RunlogF("解析接触卡号失败");
                    return 1;
                }

                cJSON *jsonBANKNUMBER;
                jsonBANKNUMBER = cJSON_GetObjectItem(json, "BANKNUMBER");
                BANKNUMBER = jsonBANKNUMBER->valuestring;
                if (BANKNUMBER.empty())
                {
                    RunlogF("解析银行卡号失败");
                    return 1;
                }

                CheckResult(RunApdu("00A4040008A000000632010105",msg));	//打开文件夹

                CheckResult(RunApdu("00B0950A0A",msg));	//获取对应数据
                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);
                FSYZ = msg.substr(4);

                CheckResult(RunApdu("0084000008",msg));

                SJS = msg.substr(0, msg.length() - 4);

                string strGBKName = Utf8_GBK(NAME.c_str());
                RunlogF("UTF-8 Name = %s --->GBK:%s.",NAME.c_str(),strGBKName.c_str());
                NAME = strGBKName;

                //NAME转ASC
                pReader->hex_a((unsigned char*)NAME.c_str(), (unsigned char*)temp, NAME.length());
                NAME = temp;
                transform(NAME.begin(), NAME.end(), NAME.begin(), ::toupper);
                int num = 0;
                num = 40 - NAME.length();//补齐数
                if (num > 0)
                {
                    NAME = NAME + std::string(num, '0');
                }

                //CERTNUMBER转ASC
                pReader->hex_a((unsigned char*)CERTNUMBER.c_str(), (unsigned char*)temp, CERTNUMBER.length());
                CERTNUMBER = temp;
                transform(CERTNUMBER.begin(), CERTNUMBER.end(), CERTNUMBER.begin(), ::toupper);
                num = 0;
                num = 64 - CERTNUMBER.length();//补齐数
                if (num > 0)
                {
                    CERTNUMBER = CERTNUMBER + std::string(num, '0');
                }

                //SEX转ASC
                pReader->hex_a((unsigned char*)SEX.c_str(), (unsigned char*)temp, SEX.length());
                SEX = temp;
                transform(SEX.begin(), SEX.end(), SEX.begin(), ::toupper);

                //CONTACTNUMBER转ASC
                pReader->hex_a((unsigned char*)CONTACTNUMBER.c_str(), (unsigned char*)temp, CONTACTNUMBER.length());
                CONTACTNUMBER = temp;
                transform(CONTACTNUMBER.begin(), CONTACTNUMBER.end(), CONTACTNUMBER.begin(), ::toupper);

                CMD = "04D696023A" + NAME + CERTNUMBER + CERTTYPE + SEX;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "FACTOR", cJSON_CreateString(FSYZ.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(FJKH.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);

            }
            break;
            case 2:
            {
                std::string INSTRUCTION;

                cJSON *jsonINSTRUCTION;
                jsonINSTRUCTION = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonINSTRUCTION->valuestring;
                if (INSTRUCTION.empty())
                {
                    RunlogF("解析指令集失败");
                    return 1;
                }

                CheckResult(RunApdu(INSTRUCTION,msg));

                String2Char(retJSON);
            }
            break;
            case 3:
            {
                std::string RFATR = "";//非接芯片ATR
                std::string FJKH = "";//非接卡号

                CheckResult(RunApdu("00B0950A0A",msg));	//获取对应数据
                if (msg.substr(msg.length() - 4, 4) == "9000")
                {
                    msg = msg.substr(0, msg.length() - 4);
                    FJKH = msg.substr(1);
                }

                if (ResetCard_RF(RFATR, pszRcCode))
                {
                    RunlogF("Failed in reset card,can't get CardATR.");
                    return 1;
                }
                //RFATR = m_CardInfo.ATR;
                if (RFATR.empty())
                {
                    RunlogF("获取ATR失败");
                    return 1;
                }

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "RFATR", cJSON_CreateString(RFATR.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(FJKH.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);

            }
            break;

            default:
            {
                RunlogF("未知的命令: %s", Cmd.c_str());
                strcpy(pszRcCode, "0003");
                return 1;
            }
            break;
            }
            cJSON_Delete(json);
        }
        else
        {
            return 1;
        }
        return 0;
    }

    int Evolis_Z390_Printer::JH_CitizenCardAPDU(std::string Cmd, std::string CmdParam, char * pszRcCode)
    {
        RunlogF("CmdParam = %s",CmdParam.c_str());
        std::string  msg;
        std::string  retJSON = "";
        int index = Cmd.find(':');
        std::string temp = Cmd.substr(0, index);
        Cmd = Cmd.substr(index + 1);

        const char* pError;
        cJSON *json = cJSON_Parse((char *)CmdParam.c_str(),&pError);
        if (!json)
        {
            RunlogF("无法解析效的命令参数:%s",CmdParam.c_str());
            strcpy(pszRcCode,"0001");
            return 1;
        }

        if (0 == strcmp(temp.c_str(), "WriteCitizenCard_3307"))
        {
            char szErr[1024] = { 0 };
            char temp[1024] = { 0 };
            char *out;

            switch (atoi(Cmd.c_str()))
            {
            case 1:
            {
                std::string SJS;//随机数
                std::string FSYZ;//分散因子
                std::string FJKH;//非接卡号
                std::string CMD;//写卡数据
                std::string MAINTYPE;//主类型
                std::string SUBTYPE;//子类型

                cJSON *jsonMAINTYPE;
                jsonMAINTYPE = cJSON_GetObjectItem(json, "MAINTYPE");
                MAINTYPE = jsonMAINTYPE->valuestring;
                if (MAINTYPE.empty())
                {
                    MAINTYPE = szErr;
                    RunlogF("解析MAINTYPE失败");
                    return 1;
                }

                cJSON *jsonSUBTYPE;
                jsonSUBTYPE = cJSON_GetObjectItem(json, "SUBTYPE");
                SUBTYPE = jsonSUBTYPE->valuestring;
                if (SUBTYPE.empty())
                {
                    SUBTYPE = szErr;
                    RunlogF("解析SUBTYPE失败");
                    return 1;
                }
                CheckResult(RunApdu( "00A40000023F00",msg,false));
                RunlogF(msg.c_str());
                CheckResult(RunApdu( "00A4040008A000000632010105",msg));	//打开文件夹

                CheckResult(RunApdu( "00B0950A0A",msg));	//获取对应数据
                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);
                FSYZ = msg.substr(4);

                CheckResult(RunApdu( "0084000004",msg));
                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                int tempNum = atoi(MAINTYPE.c_str());
                sprintf(temp, "%02X", tempNum);
                MAINTYPE = temp;

                transform(MAINTYPE.begin(), MAINTYPE.end(), MAINTYPE.begin(), ::toupper);

                CMD = "04D6851C06" + MAINTYPE + SUBTYPE;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "FACTOR", cJSON_CreateString(FSYZ.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(FJKH.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            case 2:
            {
                std::string INSTRUCTION = "";//指令集(需写入)
                cJSON *jsonInstruction;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu( INSTRUCTION,msg));
            }
            break;
            case 3:
            {
                std::string strRFATR = "";//非接芯片ATR
                std::string strFJKH = "";//非接卡号
                CheckResult(RunApdu( "00B0950A0A",msg ));	//获取对应数据

                msg = msg.substr(0, msg.length() - 4);
                strFJKH = msg.substr(1);

                if (ResetCard_RF(strRFATR,pszRcCode))
                    return 1;

                if (strRFATR.empty())
                {
                    RunlogF("获取ATR失败");
                    return 1;
                }
                RunlogF("RF ATR = %s.\n",strRFATR.c_str());
                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "RFATR", cJSON_CreateString(strRFATR.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(strFJKH.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            default:
            {
                RunlogF("未知的命令: %s", Cmd.c_str());
                strcpy(pszRcCode, "0003");
                return 1;
            }

            break;
            }
            cJSON_Delete(json);
        }
        else
        {
            return 1;
        }
        return 0;
    }

    int Evolis_Z390_Printer::YW_CitizenCardAPDU(std::string Cmd, std::string CmdParam, char * pszRcCode)
    {
        //义乌市民卡命令
        RunlogF("CmdParam = %s",CmdParam.c_str());
        std::string  msg;
        std::string  retJSON = "";
        int index = Cmd.find(':');
        std::string temp = Cmd.substr(0, index);
        Cmd = Cmd.substr(index + 1);

        const char* pError;
        cJSON *json = cJSON_Parse((char *)CmdParam.c_str(),&pError);
        if (!json)
        {
            RunlogF("无法解析效的命令参数:%s",CmdParam.c_str());
            strcpy(pszRcCode,"0001");
            return 1;
        }

        if (0 == strcmp(temp.c_str(), "WriteCitizenCard_330782"))
        {
            //char outContent[1024] = { 0 };
            char szErr[1024] = { 0 };
            char temp[1024] = { 0 };
            char *out;

            switch (atoi(Cmd.c_str()))
            {
            case 1:
            {
                std::string QYBZ;//启用标志
                std::string SJS;//随机数
                std::string FSYZ;//分散因子
                std::string FJKH;//非接卡号
                std::string CMD;//写卡数据

                cJSON *jsonQYBZ;
                jsonQYBZ = cJSON_GetObjectItem(json, "QYBZ");
                QYBZ = jsonQYBZ->valuestring;
                if (QYBZ.empty())
                {
                    QYBZ = szErr;
                    RunlogF("解析QYBZ失败");
                    return 1;
                }

                CheckResult(RunApdu( "00A4040008A000000632010105",msg ));	//打开文件夹

                CheckResult(RunApdu( "00B0950A0A",msg));	//获取对应数据

                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);
                FSYZ = msg.substr(4);

                CheckResult(RunApdu( "0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                transform(QYBZ.begin(), QYBZ.end(), QYBZ.begin(), ::toupper);
                CMD = "04D6950905" + QYBZ;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "FACTOR", cJSON_CreateString(FSYZ.c_str()));
                cJSON_AddItemToObject(root_json, "CARD_NO", cJSON_CreateString(FJKH.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);

            }
            break;
            case 2:
            {
                std::string INSTRUCTION = "";//指令集(需写入)
                std::string QDRQ = "";//启动日期
                std::string YXRQ = "";//有效日期
                std::string SJS = "";//随机数
                std::string CMD = "";//写卡数据

                cJSON *jsonInstruction, *jsonQDRQ, *jsonYXRQ;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;

                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu(INSTRUCTION,msg ));

                jsonQDRQ = cJSON_GetObjectItem(json, "QDRQ");
                QDRQ = jsonQDRQ->valuestring;
                if (QDRQ.empty())
                {
                    QDRQ = szErr;
                    RunlogF("解析QDRQ失败");
                    return 1;
                }

                jsonYXRQ = cJSON_GetObjectItem(json, "YXRQ");
                YXRQ = jsonYXRQ->valuestring;
                if (YXRQ.empty())
                {
                    YXRQ = szErr;
                    RunlogF("解析YXRQ失败");
                    return 1;
                }

                CheckResult(RunApdu( "0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                CMD = "04D695140C" + QDRQ + YXRQ;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            case 3:
            {
                std::string INSTRUCTION = "";//指令集(需写入)
                std::string MAINTYPE = "";//主类型
                std::string SUBTYPE = "";//子类型
                std::string SJS = "";//随机数
                std::string CMD = "";//写卡数据

                cJSON *jsonInstruction, *jsonMainType, *jsonSubType;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu( INSTRUCTION,msg));

                jsonMainType = cJSON_GetObjectItem(json, "MAINTYPE");
                MAINTYPE = jsonMainType->valuestring;

                if (MAINTYPE.empty())
                {
                    MAINTYPE = szErr;
                    RunlogF("解析MAINTYPE失败");
                    return 1;
                }

                jsonSubType = cJSON_GetObjectItem(json, "SUBTYPE");
                SUBTYPE = jsonSubType->valuestring;

                if (SUBTYPE.empty())
                {
                    SUBTYPE = szErr;
                    RunlogF("解析SUBTYPE失败");
                    return 1;
                }

                CheckResult(RunApdu( "0084000004",msg));

                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                CMD = "04D6851C06" + MAINTYPE + SUBTYPE;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;
            case 4:
            {
                std::string INSTRUCTION = "";//指令集(需写入)
                std::string NAME = "";//姓名
                std::string CERTTYPE = "";//证件类型
                std::string CERTNUMBER = "";//证件号
                std::string SJS = "";//随机数
                std::string CMD = "";//写卡数据

                cJSON *jsonInstruction, *jsonName, *jsonCertType, *jsonCertNumber;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;

                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }

                //写入指令集
                CheckResult(RunApdu( INSTRUCTION,msg));

                jsonName = cJSON_GetObjectItem(json, "NAME");
                NAME = jsonName->valuestring;

                if (NAME.empty())
                {
                    NAME = szErr;
                    RunlogF("解析NAME失败");
                    return 1;
                }
                jsonCertType = cJSON_GetObjectItem(json, "CERTTYPE");
                CERTTYPE = jsonCertType->valuestring;
                if (CERTTYPE.empty())
                {
                    CERTTYPE = szErr;
                    RunlogF("解析CERTTYPE失败");
                    return 1;
                }

                jsonCertNumber = cJSON_GetObjectItem(json, "CERTNUMBER");
                CERTNUMBER = jsonCertNumber->valuestring;
                if (CERTNUMBER.empty())
                {
                    CERTNUMBER = szErr;
                    RunlogF("解析CERTNUMBER失败");
                    return 1;
                }

                CheckResult(RunApdu( "0084000004",msg));
                SJS = msg.substr(0, msg.length() - 4);
                SJS += "00000000";

                //20字节ASCII姓名+32字节ASCII证件号码+1字节BCD证件类型
                pReader->hex_a((unsigned char*)NAME.c_str(), (unsigned char*)temp, NAME.length());
                NAME = temp;
                transform(NAME.begin(), NAME.end(), NAME.begin(), ::toupper);
                int num = 40 - NAME.length();//补齐数
                NAME = NAME + std::string(num, '0');
                memset(temp, 0, 1024);
                pReader->hex_a((unsigned char*)CERTNUMBER.c_str(), (unsigned char*)temp, CERTNUMBER.length());
                CERTNUMBER = temp;
                transform(CERTNUMBER.begin(), CERTNUMBER.end(), CERTNUMBER.begin(), ::toupper);
                num = 64 - CERTNUMBER.length();//补齐数
                CERTNUMBER = CERTNUMBER + std::string(num, '0');

                CMD = "04D6960239" + NAME + CERTNUMBER + CERTTYPE;

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "SJS", cJSON_CreateString(SJS.c_str()));
                cJSON_AddItemToObject(root_json, "CMD", cJSON_CreateString(CMD.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);

                String2Char(retJSON);
            }
            break;
            case 5:
            {
                std::string INSTRUCTION = "";//指令集(需写入)

                cJSON *jsonInstruction;
                jsonInstruction = cJSON_GetObjectItem(json, "INSTRUCTION");
                INSTRUCTION = jsonInstruction->valuestring;
                if (INSTRUCTION.empty())
                {
                    INSTRUCTION = szErr;
                    RunlogF("解析INSTRUCTION失败");
                    return 1;
                }
                //写入指令集
                CheckResult(RunApdu( INSTRUCTION,msg));
                String2Char(retJSON);
            }
            break;
            case 6:
            {
                std::string RFATR = "";//非接芯片ATR
                std::string FJKH = "";//非接卡号
                CheckResult(RunApdu( "00B0950A0A",msg));	//获取对应数据

                msg = msg.substr(0, msg.length() - 4);
                FJKH = msg.substr(1);

                if (ResetCard_RF(RFATR, pszRcCode))
                {
                    RunlogF("Failed in reset card,can't get CardATR.");
                    return 1;
                }
                //RFATR = m_CardInfo.ATR;
                if (RFATR.empty())
                {
                    RunlogF("获取ATR失败");
                    return 1;
                }

                cJSON *root_json;
                root_json = cJSON_CreateObject();//创建项目

                cJSON_AddItemToObject(root_json, "RFATR", cJSON_CreateString(RFATR.c_str()));
                cJSON_AddItemToObject(root_json, "FJKH", cJSON_CreateString(FJKH.c_str()));

                out = cJSON_PrintUnformatted(root_json);
                retJSON = out;
                cJSON_Delete(root_json);
                String2Char(retJSON);
            }
            break;

            default:
            {
                RunlogF("未知的命令: %s", Cmd.c_str());
                strcpy(pszRcCode, "0003");
                return 1;
            }
            break;
            }
            cJSON_Delete(json);
        }
        else
        {
            return 1;
        }

        return 0;
    }
    int Evolis_Z390_Printer::CompressPic(std::string inPic, std::string & outPic)
    {
        QByteArray baPic = QByteArray::fromBase64(inPic.c_str());
        outPic = string((char*)baPic.data(), baPic.size());
        static const char *szCompressedFileName = "./CompressedPhoto.jpg";
        static const char *szSourceFileName = "./SourcePhoto.jpg";
        //static const char *szCompressedFileName = "/usr/KIOSK/Data/CompressedPhoto.jpg";
        //static const char *szSourceFileName = "/usr/KIOSK/Data/SourcePhoto.jpg";
        QFileInfo fi(szCompressedFileName);
        if (fi.isFile())
        {
            RunlogF("%s already exist,now remove it!",szCompressedFileName);
            QFile::remove(szCompressedFileName);
        }
        QFileInfo fi2(szSourceFileName);
        if (fi2.isFile())
        {
            RunlogF("%s already exist,now remove it!",szSourceFileName);
            QFile::remove(szSourceFileName);
        }

        QFile fout(szSourceFileName);
        fout.open(QIODevice::ReadWrite);
        fout.write(outPic.c_str(), outPic.size());
        fout.close();
        if (outPic.size() > (size_t)PhotoCompressParam.nSize)
        {
            RunlogF("Now Try to compress file:%s",szCompressedFileName);
            if (!CompressPicture(szSourceFileName, szCompressedFileName))
            {
                RunlogF("Failed in compress file:%s",szCompressedFileName);
                return 1;
            }
        }
        else
        {
            RunlogF("The size of input picture is less than %dK,skip compressing!",PhotoCompressParam.nSize/1024);
        }

        RunlogF("Try to read file %s!",szCompressedFileName);
        QFile fin(szCompressedFileName);
        fin.open(QIODevice::ReadOnly);
        QByteArray byBuffer = fin.readAll();
        //QByteArray byBase64 = byBuffer.toBase64();
        //RunlogF("PIC Base64:%s",byBase64.data());
        outPic = string(byBuffer.data(), byBuffer.size());
        return 0;
    }

    int Evolis_Z390_Printer::WriteImageInfo(std::string &strPic, char *pszRcCode)
    {
        size_t nHexDataLen = strPic.size() *2;
        string strBuff(nHexDataLen,0);
        unsigned char *pHexData = (unsigned char *)strBuff.data();
        pReader->hex_a((unsigned char *)strPic.c_str(),pHexData,strPic.size());
        //char szSlice[512] = { 0 };
        char szOffset[16] = {0};
        char szSliceSize[16] = {0};
        RunlogF("Try to write picture.");
        int nSliceSize = 510;
        RunlogF("DataBuffer = %s",pHexData);
        for (size_t i = 0; i < nHexDataLen; i+= 510)	//循环分段写入数据
        {
            if((i + 510 )>= nHexDataLen)
                nSliceSize = nHexDataLen - i;

            snprintf(szOffset,16,"%04X",i/2);
            snprintf(szSliceSize,16,"%02X",nSliceSize/2);
            //写入数据 00D6 + 起始位置(十六进制) + 写入片段长度(十六进制) + 写入片段
            string strCmd = "00D6";
            strCmd += szOffset;
            strCmd += szSliceSize;
            strCmd += strBuff.substr(i,nSliceSize);
            string msg;
            RunlogF("strCmd = %s",strCmd.c_str());
            CheckResult(RunApdu(strCmd,msg));
        }
        RunlogF("Picture wrote succeed!");
        return 0;
    }

    bool Evolis_Z390_Printer::CompressPicture(std::string strSource,string strDest, int nZoomPercent)
    {
        QFileInfo fi(strSource.c_str());
        if (!fi.isFile())
        {
            RunlogF("Can't locate file %s!",strSource.c_str());
            return false;
        }
        cv::Mat img = cv::imread(strSource,IMREAD_ANYCOLOR);
        if (!img.data)
        {
            string strJpegBuffer;
            int nPicWidth = 0,nPicHeight = 0;
            //RunlogF("Failed in read %s,it may be a invalid image!",strSource.c_str());
            RunlogF("Load a jpeg file :%s.\n",strSource.c_str());

            if (ReadJpeg(strSource,strJpegBuffer,nPicWidth,nPicHeight))
            {
                RunlogF("Failed in load file :%s.\n",strSource.c_str());
                return false;
            }
            img.create(nPicHeight,nPicWidth,CV_8UC3);
            if (img.data)
                memcpy(img.data,strJpegBuffer.c_str(),strJpegBuffer.size());
            else
            {
                RunlogF("Insufficent memory in load file :%s.\n",strSource.c_str());
                return false;
            }
        }
        int width = img.cols;
        int height = img.rows;

//        QImage qjpeg(strSource.c_str());
//        if (!qjpeg.isNull())
//        {
//             RunlogF("Failed in load file :%s.\n",strSource.c_str());
//             return false;
//        }
//        int width = qjpeg.width();
//        int heigrht = qjpeg.height();
        RunlogF("Image Width = %d,Height = %d.",width,height);

        int  nCompressRate = 90;
        int  nDestWidth = PhotoCompressParam.nWidth;
        int  nDestHeight= PhotoCompressParam.nHeight;
        //double ratio = nZoomPercent / 100.0f;
        //int  nDestHeight = height * ratio;
        //int  nDestWidth = width * ratio;
        do
        {
            if (nCompressRate == 0)
                break;
            RunlogF("Try to compress pic with rate:%d%%!", nCompressRate);
            //qjpeg = qjpeg.scaled(nDestWidth,nDestHeight);
            cv::resize(img, img, cv::Size(nDestWidth, nDestHeight));
            RunlogF("Dest File = %s",strDest.c_str());
           // cv::imwrite("./Dest.bmp", img);

            WriteJpeg(strDest,img.data,nDestWidth,nDestHeight,nCompressRate);

            QFileInfo fi(strDest.c_str());
            if (fi.size() <= PhotoCompressParam.nSize)
            {
                RunlogF("compress rate = %d%%,File size = %d,", nCompressRate, fi.size());
                break;
            }
            else
            {
                int nNewRate = nCompressRate - 5;
                RunlogF("compress rate = %d%%,File size = %d,over size!try to decrease compress rate to %d%%", nCompressRate, fi.size(), nNewRate);
                nCompressRate -= 5;
                continue;
            }
        } while (true);
        RunlogF("Compress finished!");
        if (nCompressRate == 0)
            return false;
        return true;
    }
};

