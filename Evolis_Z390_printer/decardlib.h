#ifndef DECARDLIB_H
#define DECARDLIB_H
#include "dynamiclib.h"
#include <QDebug>

//#include "../SDK/include/dcrf32.h"
using   DEVHANDLE = int;

using dc_init            = int   (*)(short port, int baud);
using dc_exit            = short  (*)(DEVHANDLE icdev);
using dc_reset           = short  (*)(DEVHANDLE icdev, unsigned short _Msec);
using dc_setcpu          = short  (*)(DEVHANDLE icdev, unsigned char _Byte);
using dc_cpureset        = short  (*)(DEVHANDLE icdev, unsigned char *rlen, unsigned char *databuffer);
using dc_cpureset_hex    = short  (*)(DEVHANDLE icdev, unsigned char *rlen, char *databuffer);
using dc_cpudown         = short  (*)(DEVHANDLE icdev);
using dc_cpuapduInt      = short  (*)(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);
using dc_cpuapduInt_hex  = short  (*)(DEVHANDLE icdev, unsigned int slen, char *sendbuffer, unsigned int *rlen, char *databuffer);
using hex_a              = short  (*)(unsigned char *hex, unsigned char *a, short length);
using a_hex              = short  (*)(unsigned char *a, unsigned char *hex, short len);


class decardLib :public DynamicLib
{
private:
    void *pHandle = nullptr;

public:
    dc_init           pdc_init           = nullptr;
    dc_exit           pdc_exit           = nullptr;
    dc_reset          pdc_reset          = nullptr;
    dc_setcpu         pdc_setcpu         = nullptr;
    dc_cpureset       pdc_cpureset       = nullptr;
    dc_cpureset_hex   pdc_cpureset_hex   = nullptr;    
    dc_cpudown        pdc_cpudown        = nullptr;
    dc_cpuapduInt     pdc_cpuapduInt     = nullptr;
    dc_cpuapduInt_hex pdc_cpuapduInt_hex = nullptr;
    hex_a             phex_a             = nullptr;
    a_hex             pa_hex             = nullptr;
    decardLib()
    {
        QString strLib = QDir::currentPath();
        strLib += "/libdcrf32.so";
        pHandle = dlopen(strLib.toUtf8().data(),RTLD_NOW|RTLD_GLOBAL);
        if (!pHandle)
        {
            qDebug()<<"Failed in load "<<strLib;
            qDebug()<<"Error:"<<strerror(errno);
            return ;
        }
        pdc_init           = GetProcAddr(pHandle,dc_init          );
        pdc_exit           = GetProcAddr(pHandle,dc_exit          );
        pdc_reset          = GetProcAddr(pHandle,dc_reset         );
        pdc_setcpu         = GetProcAddr(pHandle,dc_setcpu        );
        pdc_cpureset       = GetProcAddr(pHandle,dc_cpureset      );
        pdc_cpureset_hex   = GetProcAddr(pHandle,dc_cpureset_hex  );
        pdc_cpuapduInt     = GetProcAddr(pHandle,dc_cpuapduInt    );
        pdc_cpuapduInt_hex = GetProcAddr(pHandle,dc_cpuapduInt_hex);
        pdc_cpudown        = GetProcAddr(pHandle,dc_cpudown       );
        phex_a             = GetProcAddr(pHandle,hex_a);
        pa_hex             = GetProcAddr(pHandle,a_hex);
    }

};

#endif // DECARDLIB_H
