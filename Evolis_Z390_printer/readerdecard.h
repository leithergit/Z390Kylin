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
using dc_pro_resetInt_hex= short  (*)(DEVHANDLE icdev, unsigned char *rlen, char *receive_data);
using dc_card_n_hex      = short  (*)(DEVHANDLE icdev, unsigned char _Mode, unsigned int *SnrLen, unsigned char *_Snr);
using dc_config_card     = short  (*)(DEVHANDLE icdev, unsigned char cardtype);
using dc_cpudown         = short  (*)(DEVHANDLE icdev);
using dc_cpuapduInt      = short  (*)(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);
using dc_cpuapduInt_hex  = short  (*)(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);
//    using hex_a              = short  (*)(unsigned char *hex, unsigned char *a, short length);
//    using a_hex              = short  (*)(unsigned char *a, unsigned char *hex, short len);
using dc_readdevsnr      = short  (*)(DEVHANDLE icdev,unsigned char *szSN);
using dc_getver          = short  (*)(DEVHANDLE icdev,unsigned char *szVer);
using dc_GetBankAccountNumber = short(*)(DEVHANDLE icdev,int nType,char *szNumber);
using dc_beep            = short (*)(DEVHANDLE iddev,unsigned short _Msec);
using dc_pro_commandlinkInt = short (*)(DEVHANDLE icdev, unsigned int slen, unsigned char *sendbuffer, unsigned int *rlen, unsigned char *databuffer, unsigned char timeout);
using dc_pro_commandlinkInt_hex = short (*)(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer, unsigned char timeout);

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
    dc_config_card    pdc_config_card    = nullptr;
    dc_card_n_hex     pdc_card_n_hex     = nullptr;
    dc_pro_resetInt_hex pdc_pro_resetInt_hex = nullptr;
    dc_pro_commandlinkInt pdc_pro_commandlinkInt = nullptr;
    dc_pro_commandlinkInt_hex pdc_pro_commandlinkInt_hex = nullptr;
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
        pdc_config_card    = GetProcAddr(pHandle,dc_config_card);
        pdc_card_n_hex     = GetProcAddr(pHandle,dc_card_n_hex);
        pdc_pro_resetInt_hex = GetProcAddr(pHandle,dc_pro_resetInt_hex);
        pdc_pro_commandlinkInt = GetProcAddr(pHandle,dc_pro_commandlinkInt);
        pdc_pro_commandlinkInt_hex = GetProcAddr(pHandle,dc_pro_commandlinkInt_hex);
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

    virtual int  SetCardSlot(int nPos = Pos_Contact)
    {// 0 for contact,1 for contactless
        nCardPos = (CardPostion)nPos;

        switch (nCardPos)
        {
        default:
        case Pos_Non         :
        case Pos_Bezel       :
        case Pos_Print       :
        case Pos_OutofEntrance:
        case Pos_Blocked     :
        case Pos_Unknow      :
            this->nSlot = -1;
             break;
        case Pos_Contactless :
            this->nSlot = 'A';    // contactless
             break;
        case Pos_Contact     :
            this->nSlot = 0x0C;
             break;
        }
        return 0;
    }

    virtual int  PowerOn(char *szArtInfo,int &nRetLen)
    {
        int nRet  = 0;
        if (nSlot == 'A')
        {
            int nCount = 1;
            while(nCount++ < 5)
            {
                pdc_reset(hReader,10);
                nRet = pdc_config_card(hReader,(uchar)nSlot);
                if (!nRet)
                    break;
            }
            if (nRet)
                return nRet;
            uint nLen;
            nCount = 1;
            uchar Snr[32] = {0};
            while(nCount++ < 5)
            {
                nRet = pdc_card_n_hex(hReader,0,&nLen,(uchar *)Snr);
                if (!nRet)
                    break;
            }
            if (nRet)
                return nRet;
            nCount = 1;
            while(nCount++ < 5)
            {
                nRet = pdc_pro_resetInt_hex(hReader,(uchar *)&nRetLen,(char *)szArtInfo);
                if (!nRet)
                    break;
            }
            return nRet;
        }
        else
        {
            int nResult = pdc_setcpu(hReader,(uchar)nSlot);
            if (nResult < 0)
                return nResult;

            return pdc_cpureset_hex(hReader, (unsigned char*)&nRetLen, szArtInfo);
        }
    }

    virtual int  PowerOff()
    {
        if (nSlot)
            pdc_reset(hReader,10);
        else
            pdc_cpudown(hReader);
        pdc_exit(hReader);
        hReader = -1;
        return 0;
    }

    virtual int  ApduInt(const uchar *szSrc,uint nSrcLen,uchar *szDst,uint &nDstLen)
    {
        if (nSlot == 'A')
           return pdc_pro_commandlinkInt(hReader,nSrcLen,(uchar *)szSrc,&nDstLen,(uchar *)szDst,10);
        else
           return pdc_cpuapduInt(hReader,nSrcLen,(char *)szSrc,&nDstLen,(char *)szDst);
    }

    virtual int  ApduIntHex(const char *szSrc,uint nSrcLen,char *szDst,uint &nDstLen)
    {
        if (nSlot == 'A')
            return pdc_pro_commandlinkInt_hex(hReader,nSrcLen,(char *)szSrc,&nDstLen,(char *)szDst,10);
        else
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
