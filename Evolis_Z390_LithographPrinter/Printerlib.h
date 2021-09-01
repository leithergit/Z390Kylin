#ifndef AUTOPRINTER_H
#define AUTOPRINTER_H
#include <dlfcn.h>
#include <string.h>
#include <QtDebug>
#include <LithographPrinter.h>
#include <string>
#include <memory>


class Printerlib
{
public:
    CLithographPrinter* pPrinterInstance = nullptr;
    void *pLibhandle = nullptr;
    pCreateInstance pCreateInst = nullptr;
    pFreeInstance pFreeInst = nullptr;
    char szError[1024];
    Printerlib()
    {
        memset(szError,0,1024);
        pLibhandle = dlopen("libEvolis_Z390_LithographPrinter.so",RTLD_NOW);
        if (pLibhandle)
        {
            pCreateInst = (pCreateInstance) dlsym(pLibhandle,"CreateInstance");
            pFreeInst = (pFreeInstance)dlsym(pLibhandle,"FreeInstance");
            if (!pCreateInst || !pFreeInst)
            {
                strcpy(szError,"Failed in loading Function CreateInstance!");
                qDebug()<<szError;
                return ;
            }
            pPrinterInstance = (CLithographPrinter*)pCreateInst(nullptr);
            if (!pPrinterInstance)
            {
                strcpy(szError,"Failed in Creating Evolis_Z390_LithographPrinter Instance!");
                qDebug()<<szError;
                return;
            }
        }
        else
        {
            strcpy(szError,"Failed in loading libEvolis_Z390_LithographPrinter_armeabi-v7a.so!");
            qDebug()<<szError;
        }
    }
    ~Printerlib()
    {
        if (pPrinterInstance)
        {
            char szCode[1024] = {0};
            pPrinterInstance->Print_Close(szCode);
            pFreeInst(pPrinterInstance);
            pPrinter = nullptr;
        }
        if (pLibhandle)
            dlclose(pLibhandle);
    }

    CLithographPrinter *PrinterInstance()
    {
        return pPrinterInstance;
    }
};

using PrinterlibPtr = shared_ptr<Printerlib>;
#endif // AUTOPRINTER_H
