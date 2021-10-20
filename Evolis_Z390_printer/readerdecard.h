#ifndef Readerdecard_H
#define Readerdecard_H
#include "dynamiclib.h"
#include "readerbase.h"
#include <QDebug>

using dc_init            = int    (*)(short port, int baud);
using dc_exit            = short  (*)(DEVHANDLE icdev);
using dc_reset           = short  (*)(DEVHANDLE icdev, unsigned short _Msec);
using dc_setcpu          = short  (*)(DEVHANDLE icdev, unsigned char _Byte);
using dc_cpureset        = short  (*)(DEVHANDLE icdev, unsigned char *rlen, unsigned char *databuffer);
using dc_cpureset_hex    = short  (*)(DEVHANDLE icdev, unsigned char *rlen, char *databuffer);
using dc_cpudown         = short  (*)(DEVHANDLE icdev);
using dc_cpuapduInt      = short  (*)(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);
using dc_cpuapduInt_hex  = short  (*)(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);
//    using hex_a              = short  (*)(unsigned char *hex, unsigned char *a, short length);
//    using a_hex              = short  (*)(unsigned char *a, unsigned char *hex, short len);
using dc_readdevsnr      = short  (*)(DEVHANDLE icdev,unsigned char *szSN);
using dc_getver          = short  (*)(DEVHANDLE icdev,unsigned char *szVer);
using dc_GetBankAccountNumber = short(*)(DEVHANDLE icdev,int nType,char *szNumber);
using dc_beep            = short (*)(DEVHANDLE iddev,unsigned short _Msec);

class Readerdecard :public ReaderBase
{
private:
    void *pHandle = nullptr;
    dc_init           pdc_init           = nullptr;
    dc_exit           pdc_exit           = nullptr;
    dc_reset          pdc_reset          = nullptr;
    dc_setcpu         pdc_setcpu         = nullptr;
    //dc_cpureset       pdc_cpureset       = nullptr;
    dc_cpureset_hex   pdc_cpureset_hex   = nullptr;    
    dc_cpudown        pdc_cpudown        = nullptr;
    dc_cpuapduInt     pdc_cpuapduInt     = nullptr;
    dc_cpuapduInt_hex pdc_cpuapduInt_hex = nullptr;
    dc_readdevsnr     pdc_readdevsnr     = nullptr;
    dc_getver         pdc_getver         = nullptr;
    dc_GetBankAccountNumber pdc_GetBankAccountNumber = nullptr;
    dc_beep           pdc_beep           = nullptr;
//    hex_a             phex_a             = nullptr;
//    a_hex             pa_hex             = nullptr;
public:
    Readerdecard()
    {
        nVenderID = Vender::Decard;
        QString strLib = QDir::currentPath();
        strLib += "/libdcrf32.so";
        pHandle = dlopen(strLib.toUtf8().data(),RTLD_NOW|RTLD_GLOBAL);
        if (!pHandle)
        {
            qDebug()<<"Failed in load "<<strLib;
            qDebug()<<"Error:"<<strerror(errno);
            return ;
        }
        pdc_getver         = GetProcAddr(pHandle,dc_getver        );
        pdc_readdevsnr     = GetProcAddr(pHandle,dc_readdevsnr    );
        pdc_init           = GetProcAddr(pHandle,dc_init          );
        pdc_exit           = GetProcAddr(pHandle,dc_exit          );
        pdc_reset          = GetProcAddr(pHandle,dc_reset         );
        pdc_setcpu         = GetProcAddr(pHandle,dc_setcpu        );
        //pdc_cpureset       = GetProcAddr(pHandle,dc_cpureset      );
        pdc_cpureset_hex   = GetProcAddr(pHandle,dc_cpureset_hex  );
        pdc_cpuapduInt     = GetProcAddr(pHandle,dc_cpuapduInt    );
        pdc_cpuapduInt_hex = GetProcAddr(pHandle,dc_cpuapduInt_hex);
        pdc_cpudown        = GetProcAddr(pHandle,dc_cpudown       );
        pdc_beep           = GetProcAddr(pHandle,dc_beep);
        pdc_GetBankAccountNumber= GetProcAddr(pHandle,dc_GetBankAccountNumber);
//        phex_a             = GetProcAddr(pHandle,hex_a);
//        pa_hex             = GetProcAddr(pHandle,a_hex);
    }

    virtual ~Readerdecard()
    {
        CloseDev();
    }

    virtual bool DetectDev()
    {
       hReader = pdc_init(100,115200);
       if (hReader == -1)
           return false;
       else
       {
//           this_thread::sleep_for(chrono::milliseconds(100));
//           pdc_exit(hReader);
//           this_thread::sleep_for(chrono::milliseconds(100));
//           hReader = -1;
           return true;
       }
    }
    virtual bool OpenDev(char *szPort,int nBaudrate,void *pParam = nullptr)
    {
        if (hReader != -1)
            CloseDev();
        hReader = pdc_init(100,115200);
        if (hReader < 0 )
            return false;
        else
        {
            pdc_beep(hReader,20);
            return true;
        }
    }
    virtual void CloseDev()
    {
        if (hReader != -1)
        {
            pdc_exit(hReader);
            hReader = -1;
        }
    }
    virtual void  Reset(int nTimeout)
    {
        if (hReader != -1)
            pdc_reset(hReader,(unsigned short)nTimeout);
    }

    virtual int  SetCardSlot(int nSlot =  0x0C)
    {
       return 0;
    }

    virtual int  PowerOn(char *szArtInfo,int &nRetLen)
    {
        int nResult = pdc_setcpu(hReader,0x0C);
        if (nResult < 0)
            return nResult;

        return pdc_cpureset_hex(hReader, (unsigned char*)&nRetLen, szArtInfo);
    }

    virtual int  PowerOff()
    {
        return pdc_cpudown(hReader);
    }

    virtual int  ApduInt(const uchar *szSrc,uint nSrcLen,uchar *szDst,uint &nDstLen)
    {
        // dc_cpuapduInt(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer)
        return pdc_cpuapduInt(hReader,nSrcLen,(char *)szSrc,&nDstLen,(char *)szDst);
    }

    virtual int  ApduIntHex(const char *szSrc,uint nSrcLen,char *szDst,uint &nDstLen)
    {
        // dc_cpuapduInt_hex(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);
        return pdc_cpuapduInt_hex(hReader,nSrcLen,(char *)szSrc,&nDstLen,szDst);
    }

    virtual int  GetDevSN(unsigned char* szSN)
    {
        return pdc_readdevsnr(hReader,szSN);
    }

    virtual int  GetDevVer(unsigned char* szVer)
    {
        return pdc_getver(hReader,szVer);
    }

    virtual int  GetBankcardNo(unsigned char* szBankCard)
    {
        // 1 = contact card
        // 0 = contactless card
        return pdc_GetBankAccountNumber(hReader,0,(char *)szBankCard);
    }
    virtual int  GetErrorMsg(std::string &strErrorMsg)
    {
        strErrorMsg = "N/A";
        return 0;
    }

};

#endif // Readerdecard_H
