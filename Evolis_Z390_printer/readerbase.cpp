#include "readerdecard.h"
#include "readerMinhua.h"
#include <list>
using namespace std;

ReaderPtr CreateReader()
{
    list<ReaderPtr> listReader = {shared_ptr<ReaderBase>(new Readerdecard()),
                                  shared_ptr<ReaderBase>(new ReaderMinhua())};
    for (auto var:listReader)
        if (var->DetectDev())
            return var;
    return nullptr;
}

/*
 功能描述		内存数据转换为16进制ASCII字符串
 pHex			输入数据流
 nHexLen		输入数据流长度
 szAscString	输出16进制ASCII字符串缓冲区
 nBuffLen		输出缓冲区最大长度
 返回值		<0时 输入参数不合法
                >0 返回转换后的ASCII符串的长度
*/
int Binary2Hexstring(unsigned char *pBinary,int nHexLen, unsigned char *szHexString,int nBuffLen,char chSeperator)
{
    static const  char chHexTableA[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    if (!pBinary ||
        !szHexString||
        !nBuffLen)
        return -1;
    unsigned char nMult = 3;
    if (chSeperator == '\0')
        nMult = 2;
    if (nHexLen*nMult > nBuffLen)
        return -1;
    unsigned char *p = &szHexString[0];

    int n;
    for (n = 0; n < nHexLen ; n++)
    {
        TOHEXA(pBinary[n], p);
        if (nMult == 2)
            continue;
        *p++ = chSeperator;
    }
    return n*nMult;
}
/*
 功能描述			16进制ASCII字符串转为用内存数据
 szAscString		输入16进制ASCII字符串
 nAscStringLen	输入数据长度
 pHex				输出缓冲区
 nBuffLen			输出缓冲区最大长度
 返回值			<0时 输入参数不合法
                    >0 返回转换后pHex数据的长度
*/
int HexString2Binary(unsigned char* pHexBuffer, int nHexBuffLen,unsigned char* szBinary, int nBinaryLen,  char chSeperator)
{
    if (!szBinary ||
        !pHexBuffer ||
        !nHexBuffLen)
        return -1;
    unsigned char nMult = 3;
    if (chSeperator == '\0')
        nMult = 2;

    if (nBinaryLen * nMult < nHexBuffLen)
        return -1;

    int nCount = 0;
    char ch;
    for (int i = 0; i < nHexBuffLen; i += nMult)
    {
        ch = UpcasecharA(pHexBuffer[i]);
        unsigned char nHi = Char2DigitA(ch);
        ch = UpcasecharA(pHexBuffer[i + 1]);
        unsigned char nLo = Char2DigitA(ch);
        szBinary[nCount++] = (nHi & 0x0F) << 4 | (nLo & 0x0F);
    }
    return nCount;
}
