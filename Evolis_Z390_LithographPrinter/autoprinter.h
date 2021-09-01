#ifndef AUTOPRINTER_H
#define AUTOPRINTER_H
#include <dlfcn.h>
#include <string.h>
#include <QtDebug>
#include <LithographPrinter.h>
#include <string>
#include <memory>


//class AutoPrinter
//{
//public:

//    static  list<PrinterLibInfo> Printerlist;
//    CLithographPrinter* pPrinter = nullptr;
//    void *pLibhandle = nullptr;
//    pCreateInstance pCreateInst = nullptr;
//    pFreeInstance pFreeInst = nullptr;
//    char szError[1024];
//    AutoPrinter();
//    ~AutoPrinter();
//    CLithographPrinter *GetPrinter(string &strPrinter);
//    void FreeLibHandle()
//    {
//        if (pLibhandle)
//        {
//            dlclose(pLibhandle);
//            pLibhandle = nullptr;
//        }
//    }
//};

//using AutoPrinterPtr = shared_ptr<AutoPrinter>;
#endif // AUTOPRINTER_H
