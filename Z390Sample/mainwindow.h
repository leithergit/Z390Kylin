﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMessageBox>
#include <stdio.h>
#include <QImage>
#include <QtDebug>
#include <dlfcn.h>
#include <QObject>
#include <QDir>

#include "../Evolis_Z390_LithographPrinter/LithographPrinter.h"
#include <QApplication>
#include <thread>
#include <chrono>
using namespace std;

class QWaitCursor
{
public:
    QWaitCursor()
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    }
    ~QWaitCursor()
    {
        QApplication::restoreOverrideCursor();
    }
};

#define WaitCursor()  QWaitCursor qWait;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using pCreateInstance =  LPVOID  (*)(LPVOID lpReserve);
using pFreeInstance = void (*)(LPVOID lpDev);

using Z390Ptr = CLithographPrinter*;

enum SCREEN_ORIENTATION
{
    SCREEN_ORIENTATION_LANDSCAPE = 0,
    SCREEN_ORIENTATION_REVERSE_LANDSCAPE = 8,
    SCREEN_ORIENTATION_SENSOR_LANDSCAPE = 6,
    SCREEN_ORIENTATION_PORTRAIT = 1,
    SCREEN_ORIENTATION_REVERSE_PORTRAIT = 9,
    SCREEN_ORIENTATION_SENSOR_PORTRAIT = 7
};

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
        QString strCurrentPath = QDir::currentPath();
        QString strLibPath = strCurrentPath + "/libEvolis_Z390_Printer.so";
        //QString strLibPath = strCurrentPath + "/libEvolis_Z390.so";
        pLibhandle = dlopen(strLibPath.toStdString().c_str(),RTLD_NOW|RTLD_GLOBAL);
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
            sprintf(szError,"Failed in loading %s,std error:%s,dlerror:%s",strLibPath.toStdString().c_str(),strerror(errno),dlerror());
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
            pPrinterInstance = nullptr;
        }
        if (pLibhandle)
            dlclose(pLibhandle);
    }

    CLithographPrinter *PrinterInstance()
    {
        return pPrinterInstance;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void OutputMsg(const char *pFormat, ...);
    //QImage *pConvas = nullptr;
    void OpenPrinter();
    void Printer_SetOverlayer();
    void ClosePrinter();
    Printerlib *pAutoPrinter = nullptr;
    Z390Ptr pPrinterInstance = nullptr;
    std::thread *pThreadPrint = nullptr;

    void Printer_GetBoxStatus();

    void Printer_InsertCard();

    void Printer_EjectCard();

    void Printer_Retract();

    void Printer_GetStatus();

    void Printer_Start();

    void Printer_Reset();

    void Printer_ICPowerOn();

    void Printer_ICPowerOff();

    void Printer_ICExchange(const char *szCmd);

    void Printer_Depense();
    void PrintJos();

    bool GetCardBoxStatus();
    QString strLogText;
protected:
    bool eventFilter(QObject *obj, QEvent *e);
    void closeEvent(QCloseEvent *event);
signals:
    void RestoreButtons();
    void AppendText(QString strTxt);
    void ScrollToEnd();


private slots:

    void OnRestoreButtons();
    void OnAppendText(QString strText);
    void OnScrollToEnd();

    void on_pushButton_ExtraCommand_clicked();

    void on_comboBox_Extracommand_currentIndexChanged(int index);

    void on_pushButton_browsegraph_clicked();

    void on_pushButton_Font_clicked();

    void on_pushButton_PrinterOpen_clicked();

    void on_pushButton_PrinterInsert_clicked();

    void on_pushButton_PrinterEject_clicked();

    void on_pushButton_PrinterClose_clicked();

    void on_pushButton_PrinterStatus_clicked();

    void on_pushButton_PrinterBoxStatus_clicked();

    void on_pushButton_PrinterInit_clicked();

    void on_pushButton_PrinterSetImage_clicked();

    void on_pushButton_PrinterSetText_clicked();

    void on_pushButton_PrinterICOn_clicked();

    void on_pushButton_PrinterICExchange_clicked();

    void on_pushButton_PrinterICOff_clicked();

    void on_pushButton_PrinterStart_clicked();

    void on_pushButton_PrinterReset_clicked();

    void on_pushButton_PrinterConfigure_Get_clicked();

    void on_pushButton_PrinterConfigure_Set_clicked();

    void on_radioButton_EnableCardOnce_clicked();

    void on_radioButton_EnableCardAlways_clicked();

    void on_radioButton_DisableCard_clicked();

    void on_comboBox_Port_currentIndexChanged(const QString &arg1);

    void on_pushButton_browseFont_clicked();

    void on_MainWindow_destroyed();

    void on_pushButton_Printer_clicked();

    void on_pushButton_SetPrinterOption_clicked();

private:
    Ui::MainWindow *ui;
    int  nResetOption = 1;
};
#endif // MAINWINDOW_H
