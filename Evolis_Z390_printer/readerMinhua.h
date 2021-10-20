#ifndef ReaderMinhua_H
#define ReaderMinhua_H
#include "dynamiclib.h"
#include "readerbase.h"
#include <QDebug>

using mwDevGetUsbList       = int  (*)(char *list);
using mwDevOpen             = int  (*)(const char *port, const char *paras, DEVHANDLE *handle);
using mwDevClose            = int  (*)(DEVHANDLE icdev);
using mwDevGetHardwareVer   = int  (*)(DEVHANDLE icdev, char *strHardwareVer);
using mwDevGetSerialNumber  = int  (*)(DEVHANDLE icdev, char *strSerialNumber);
using mwSmartCardPowerDown  = int  (*)(DEVHANDLE icdev, int slotNumber);
using mwSmartCardCommand    = int  (*)(DEVHANDLE icdev, int slotNumber, const unsigned char srcData[], int srcLen, unsigned char dstInfo[]);
using mwSmartCardCommand_HEX= int  (*)(DEVHANDLE icdev, int slotNumber, const char *strSrc, char *strDst);
using SIReaderBankCard      = int  (*)(DEVHANDLE icdev, char *cBankCard);
using mwSmartCardReset      = int  (*)(DEVHANDLE icdev, int slotNumber, unsigned char atrInfo[], int opFlag);
using mwSmartCardReset_HEX  = int  (*)(DEVHANDLE icdev, int slotNumber, char *strArtInfo, int opFlag);
using mwDevBeep             = int  (*)(DEVHANDLE icdev, int beepTimes, int interval, int time);
using getErrDescription     = int  (*)(int errcode,int language,char *message);
class ReaderMinhua :public ReaderBase,DynamicLib
{

private:
    int  nSlot = -1;
    mwDevGetUsbList         pDevGetUsbList          = nullptr;
    mwDevOpen               pDevOpen                = nullptr;
    mwDevClose              pDevClose               = nullptr;
    mwDevGetHardwareVer     pDevGetHardwareVer      = nullptr;
    mwDevGetSerialNumber    pDevGetSerialNumber     = nullptr;
    mwSmartCardPowerDown    pSmartCardPowerDown     = nullptr;
    mwSmartCardCommand      pSmartCardCommand       = nullptr;
    mwSmartCardCommand_HEX  pSmartCardCommand_HEX   = nullptr;
    SIReaderBankCard        pSIReaderBankCard       = nullptr;
    mwSmartCardReset        pSmartCardReset         = nullptr;
    mwSmartCardReset_HEX    pSmartCardReset_HEX     = nullptr;
    mwDevBeep               pDevBeep                = nullptr;
    getErrDescription       pgetErrDescription      = nullptr;
public:
    ReaderMinhua()
    {
        nVenderID = Vender::Minhua;
        QString strLib = QDir::currentPath();
        strLib += "/libmwReader.so";
        pHandle = dlopen(strLib.toUtf8().data(),RTLD_NOW|RTLD_GLOBAL);
        if (!pHandle)
        {
            qDebug()<<"Failed in load "<<strLib;
            qDebug()<<"Error:"<<strerror(errno);
            return ;
        }

        pDevGetUsbList     = GetProcAddr(pHandle,mwDevGetUsbList    );
        pDevOpen           = GetProcAddr(pHandle,mwDevOpen          );
        pDevClose          = GetProcAddr(pHandle,mwDevClose          );
        pDevGetHardwareVer = GetProcAddr(pHandle,mwDevGetHardwareVer);
        pDevGetSerialNumber= GetProcAddr(pHandle,mwDevGetSerialNumber);
        pSmartCardPowerDown= GetProcAddr(pHandle,mwSmartCardPowerDown);
        pSmartCardCommand  = GetProcAddr(pHandle,mwSmartCardCommand );
        pSmartCardCommand_HEX= GetProcAddr(pHandle,mwSmartCardCommand_HEX );
        pSIReaderBankCard  = GetProcAddr(pHandle,SIReaderBankCard);
        pSmartCardReset    = GetProcAddr(pHandle,mwSmartCardReset );
        pSmartCardReset_HEX= GetProcAddr(pHandle,mwSmartCardReset_HEX );
        pDevBeep           = GetProcAddr(pHandle,mwDevBeep );
        pgetErrDescription = GetProcAddr(pHandle,getErrDescription );
//        phex_a             = GetProcAddr(pHandle,hex_a);
//        pa_hex             = GetProcAddr(pHandle,a_hex);
    }

    virtual ~ReaderMinhua()
    {
        CloseDev();
    }

    virtual bool DetectDev()
    {
        char list[128];
        nError = pDevGetUsbList(list);
        if (nError < 0)
           return false;
        else
            return true;
    }

    virtual bool OpenDev(char *szPort,int nBaudrate,void *pParam = nullptr)
    {
        if (hReader != -1)
            CloseDev();

        nError = pDevOpen("USB", nullptr, &hReader);
        if (nError < 0)
            return false;
        else
        {
            pDevBeep(hReader, 2, 2, 2);
            return true;
        }
    }

    virtual int  GetDevSN(unsigned char* szSN)
    {
        nError = pDevGetSerialNumber(hReader, (char *)szSN);
        if (nError < 0)
            return nError;
         else
            return 0;
    }

    virtual int  GetDevVer(unsigned char* szVer)
    {
        char version[20];
        nError = pDevGetHardwareVer(hReader, version); //读设备版本号
        if (nError < 0)
            return nError;
        else
           return 0;
    }

    virtual int  GetBankcardNo(unsigned char* szBankCard)
    {
        // 1 = contact card
        // 0 = contactless card
        nError = pSIReaderBankCard(hReader,(char *)szBankCard);
        if (nError > 0)
            return 0;
        else
            return -1;
    }

    virtual void CloseDev()
    {
        if (hReader != -1)
        {
            pDevClose(hReader);
            hReader = -1;
        }
    }

    virtual void  Reset(int nTimeout){}

    virtual int  SetCardSlot(int nPos)
    {
        nCardPos = (CardPostion)nPos;
        Q_UNUSED(nSlot);
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
            this->nSlot = 1;
             break;
        case Pos_Contact     :
            this->nSlot = 0;
             break;
        }
        return 0;
    }

    virtual int  PowerOn(char *szArtInfo,int &nRetLen)
    {
        int nOpFlag = 115200;
        if (nSlot == 1)
            nOpFlag = 1;
        nError = pSmartCardReset_HEX(hReader, nSlot, szArtInfo,nOpFlag);
        if (nError < 0)
            return nError;
        else
        {
            nRetLen = nError;
            return 0;
        }
    }

    virtual int  PowerOff()
    {
        nError = pSmartCardPowerDown(hReader,nSlot);
        if (nError < 0)
            return nError;
        else
            return 0;
    }

    virtual int  ApduInt(const uchar *szSrc,uint nSrcLen,uchar *szDst, uint &nDstLen)
    {
        // SmartCardCommand(DEVHANDLE icdev, int slotNumber, const unsigned char srcData[], int srcLen, unsigned char dstInfo[]);
        nError = pSmartCardCommand(hReader,nSlot, szSrc,(int)nSrcLen,szDst);
        if (nError < 0)
            return nError;
        else
        {
            nDstLen = nError;
            return 0;
        }
    }

    virtual int  ApduIntHex(const char *szSrc,uint nSrcLen,char *szDst,uint &nDstLen)
    {
        //SmartCardCommand_HEX(DEVHANDLE icdev, int slotNumber, const char *strSrc, char *strDst)
        nError = pSmartCardCommand_HEX(hReader,nSlot, szSrc,szDst);
        if (nError < 0)
            return nError;
        else
        {
            nDstLen = nError;
            return 0;
        }
    }

    virtual int  GetErrorMsg(std::string &strErrorMsg)
    {
        char szError[1024] = {0};
        pgetErrDescription(nError, 0, szError);
        strErrorMsg = szError;
        return 0;
    }
};

#endif // ReaderMinhua_H
