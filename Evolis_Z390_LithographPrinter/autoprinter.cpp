#include "autoprinter.h"
#include <algorithm>
#include <QDir>
#include <error.h>

list<PrinterLibInfo> AutoPrinter::Printerlist =
{
    {"Evolis Zenius","libEvolis_Zenius_LithographPrinter.so"},
    {"Evolis KC200","libEvolis_KC200_LithographPrinter.so"},
    {"Evolis Elypso","libEvolis_Elypso_LithographPrinter.so"},
    {"Evolis Avansia","libEvolis_Avansia_LithographPrinter.so"}
};

AutoPrinter::AutoPrinter()
{

}
AutoPrinter::~AutoPrinter()
{
    if (pPrinter)
    {
        char szCode[1024] = {0};
        pPrinter->Print_Close(szCode);
        pFreeInst(pPrinter);
        pPrinter = nullptr;
    }
   FreeLibHandle();
}

CLithographPrinter *AutoPrinter::GetPrinter(string &strPrinter)
{
    if (pPrinter)
        return pPrinter;
    auto itFind = find_if(Printerlist.begin(),Printerlist.end(), [&](PrinterLibInfo &Ele)
    {
        return (Ele.strPrinter == strPrinter);
    });
    if (itFind == Printerlist.end())
    {
        return nullptr;
    }
    memset(szError,0,1024);
    QString strCurrentPath = QDir::currentPath();
    QString strLibPath = strCurrentPath + "/" + itFind->strLib.c_str();

    qDebug()<<"Prepare to loading "<<strLibPath.toStdString().c_str();
    pLibhandle = dlopen(strLibPath.toStdString().c_str(),RTLD_NOW);
    if (pLibhandle)
    {
        pCreateInst = (pCreateInstance) dlsym(pLibhandle,"CreateInstance");
        pFreeInst = (pFreeInstance)dlsym(pLibhandle,"FreeInstance");
        if (!pCreateInst || !pFreeInst)
        {
            strcpy(szError,"Failed in loading Function CreateInstance!");
            qDebug()<<szError;
            FreeLibHandle();
            return nullptr;
        }
        pPrinter = (CLithographPrinter*)pCreateInst(nullptr);
        if (!pPrinter)
        {
            strcpy(szError,"Failed in Creating Evolis_Z390_LithographPrinter Instance!");
            qDebug()<<szError;
            FreeLibHandle();
            return nullptr;
        }
    }
    else
    {
        sprintf(szError,"Failed in loading %s,error:%s,%s",strLibPath.toStdString().c_str(),strerror(errno),dlerror());
        qDebug()<<szError;
        FreeLibHandle();
        return nullptr;
    }
    return pPrinter;
}
