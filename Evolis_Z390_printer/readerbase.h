#ifndef READERBASE_H
#define READERBASE_H

#include "dynamiclib.h"
#include <memory>
#include <chrono>
#include <thread>
using namespace std;

enum CardPostion
{
    Pos_Non         = 0,    
    Pos_Mr          = 1,    
    Pos_Contact     = 2,
    Pos_Contactless = 3,
    Pos_Print       = 4,
    Pos_Bezel       = 5,
    Pos_OutofEntrance,
    Pos_Blocked     ,
    Pos_Unknow
};

//把a转换为Hex字符
#define TOHEXA(a, b) {*b++ = chHexTableA[a >> 4]; *b++ = chHexTableA[a & 0xf];}
#define		Char2DigitA(ch)	(ch > '9'?(ch - 'A' + 10):ch - '0')
#define		UpcasecharA(ch)	( ch >= 'A'?ch:(ch - 'a' + 'A'))
using   DEVHANDLE = int;
int Binary2Hexstring(unsigned char *pBinary,int nHexLen, unsigned char *szHexString,int nBuffLen,char chSeperator);
int HexString2Binary(unsigned char* pHexBuffer, int nHexBuffLen,unsigned char* szBinary, int nBinaryLen,  char chSeperator);

enum class Vender
{
    Unkonw = -1,
    Decard = 1,
    Minhua = 2
};
class ReaderBase
{
public:
    int     hReader = -1;
    int     nError  = 0;
    CardPostion   nCardPos = Pos_Unknow;
    Vender     nVenderID = Vender::Unkonw;
public:
    ReaderBase()
    {
    }
    virtual ~ReaderBase() {}
    virtual bool DetectDev() = 0;
    virtual bool OpenDev(char *szPort,int nBaudrate,void *pParam = nullptr) = 0;
    virtual void CloseDev() = 0;
    virtual void Reset(int nTimeout) = 0;
    virtual int  SetCardSlot(int nSlot) = 0;
    virtual int  PowerOn(char *szArtInfo,int &nRetLen) = 0;
    virtual int  PowerOff()  = 0;
    virtual int  ApduInt(const uchar *szSrc,uint nSrcLen,uchar *szDst,uint &nDstLen) = 0;
    virtual int  ApduIntHex(const char *szSrc,uint nSrcLen,char *szDst,uint &nDstLen) = 0;
    virtual int  GetDevSN(unsigned char* szSN) = 0;
    virtual int  GetDevVer(unsigned char* szVer)  = 0;
    virtual int  GetBankcardNo(unsigned char* szBankCard)  = 0;
    virtual Vender GetVender() {return nVenderID;}
    virtual int  GetErrorMsg(std::string &strErrorMsg) = 0;

    // binary to hex string
    short hex_a(unsigned char *szHex,unsigned char *szAsc,short nInputLen)
    {
        return Binary2Hexstring(szHex,nInputLen, szAsc,1024,0);
    }

    // hex string to binary
    short a_hex(unsigned char* szAsc,unsigned char *szHex,short nInputLen)
    {
        return HexString2Binary(szAsc,nInputLen,szHex,1024,0);
    }
};

using ReaderPtr = shared_ptr<ReaderBase> ;
ReaderPtr CreateReader();

#endif // READERBASE_H
