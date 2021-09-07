#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFontDialog>
#include <qfiledialog.h>

#include <string>
#include <QtDebug>
#include <QStandardPaths>
#include <memory>
#include <QDir>
#include <QList>
#include <QFileInfoList>
#include <QDebug>
#include <string>
#include <QColorDialog>
#include <QEvent>
#include <QMouseEvent>
#include <QTime>


//#include "./Z390/include/evolis_z390_lithographprinter.h"

using namespace std;

#define GetPrinterInstace(x)     (Z390Ptr)x->PrinterInstance();
#define CheckPriner(x)     if (!x)    {    OutputMsg("Please Open printer!");    return;}

#define CreatePrinter(x)     AutoPrinter PrintInstance; CLithographPrinter* x = PrintInstance.Printer();
#define Result(x,f)  x = f; if (x != 0) {OutputMsg("Failed in %s:%d",#f,x);return;}else OutputMsg("Succeed in %s",#f);

struct ExtraCommand
{
    QString strCommand;
    QStringList listData;
};
ExtraCommand cmdlist[] =
    {
        {"ReadBankNo",{}},
        {"WriteCard:1",{}},
        {"WriteCard:2",{"RESULT1","RESULT2"}},
        {"WriteCard:3",{"RESULT","SHBZHM","XM","XB","MZ","CSRQ"}},
        {"WriteCard:4",{"KSBM","KLB","GFBB","CSHJGBM","FKRQ","KYXQ","KH","RESULT"}},
        {"WriteCard:5",{"RESULT1","RESULT2"}},
        {"WriteCard:6",{"<ROOT><USERPIN>123456</USERPIN></ROOT>"}},
        {"WriteCard:7",{"USERPIN","QMZS","RESULT","JMZS","JMMY","OLDGLYPIN","GLYPIN","OLDZKMY","ZKMY"}},
   };
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //setOrientation(SCREEN_ORIENTATION_LANDSCAPE);
    ui->setupUi(this);

    ui->pushButton_PrinterOpen->setEnabled(false);
    QList<QPushButton *> buttonList = findChildren<QPushButton *>();
    for (auto var:buttonList)
        var->setEnabled(false);
    ui->pushButton_PrinterOpen->setEnabled(true);

    for(auto var:cmdlist)
    {
        ui->comboBox_Extracommand->addItem(var.strCommand);
    }
    ui->comboBox_Extracommand->setCurrentIndex(0);
   //pConvas = new QImage(1024, 650, QImage::Format_ARGB32);
   //pConvas->fill(Qt::white);
    //ui->label_Image->setPixmap(QPixmap::fromImage(*pConvas));
    //ui->lineEdit_text_Color->installEventFilter(this);
    //ui->textEdit->document()->setMaximumBlockCount(512);
    QString strCurPath = QDir::currentPath();
    QString strFont = strCurPath + "/resources/SIMSUN.ttf";
    QFileInfo fi(strFont);
    if (fi.isFile())
    {
        ui->lineEdit_Font->setText(strFont);
    }

    connect(this,&MainWindow::RestoreButtons,this,&MainWindow::OnRestoreButtons);
    connect(this,&MainWindow::AppendText,this,&MainWindow::OnAppendText);
    connect(this,&MainWindow::ScrollToEnd,this,&MainWindow::OnScrollToEnd);

}

 void MainWindow::OnRestoreButtons()
 {
//     QList<QPushButton *>buttonlist = ui->findChildren<QPushButton *>();
//     for (auto var:buttonlist)
//         var->setEnabled(true);
 }

 void  MainWindow::OnAppendText(QString strText)
 {
     ui->textEdit->appendPlainText(strText);
     QTextCursor cursor = ui->textEdit->textCursor();
     cursor.movePosition(QTextCursor::End);
     ui->textEdit->setTextCursor(cursor);
 }

 void  MainWindow::OnScrollToEnd()
 {

 }


void MainWindow::on_comboBox_Extracommand_currentIndexChanged(int index)
{
    WaitCursor();
    if (index != -1)
    {
        ui->comboBox_ExtracommandData->clear();
        auto var = cmdlist[index];
        ui->comboBox_ExtracommandData->addItems(var.listData);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

#include <QTime>
#define __countof(array) (sizeof(array)/sizeof(array[0]))
void MainWindow::OutputMsg(const char *pFormat, ...)
{
    if (pThreadPrint)
        return ;
    va_list args;
    va_start(args, pFormat);
    int nBuff;
    CHAR szBuffer[256] = {0};
    nBuff = vsnprintf(szBuffer, __countof(szBuffer), pFormat, args);
    qDebug()<<szBuffer;
    QTime tNow = QTime::currentTime();
    QString strText = tNow.toString();
    strText += ' ';
    strText += szBuffer;
    va_end(args);
    emit AppendText(strText);
}

void MainWindow::OpenPrinter()
{
    if (!pAutoPrinter)
    {
        string strPrinterModel = ui->comboBox_Printer->currentText().toStdString();
        string strPort = ui->comboBox_Port->currentText().toStdString();
        string strAddress = ui->lineEdit_Address->text().toStdString();
        pAutoPrinter = new Printerlib();

        pPrinterInstance = GetPrinterInstace(pAutoPrinter);
        if (!pPrinterInstance)
        {
            OutputMsg("Failed in loading Printer drivers:%s!",strPrinterModel.c_str());
            delete pAutoPrinter;
            pAutoPrinter = nullptr;
            return;
        };
        LPVOID reply;
        char   ec[5];

        long nTimeout = 5000;
        int x = pPrinterInstance->Print_Open((char *)strPort.c_str(),(char *)strAddress.c_str(),ec);
        if (x != 0)
        {
            OutputMsg("Failed in Print_Open:%d",x);
            delete pAutoPrinter;
            pAutoPrinter = nullptr;
            return;
        }
        else
        {
//           int r = pPrinterInstance->Print_ExtraCommand(nTimeout, (char*) "setInputTray",(char*) "FEEDER", reply, ec);
//           OutputMsg("> setInputTray: %d\n", r);
           // setInputTray to configure card insertion, it accepts:
           //   - For Avansia printers:
           //     * FEEDER     -> top feeder
           //     * MANUALNCMD -> right side of the printer + standby

//           r = pPrinterInstance->Print_ExtraCommand(nTimeout, (char*) "setOutputTray", (char*) "STD_STANDBY", reply, ec);
//           OutputMsg("> setOutputTray: %d\n", r);
           // setOutputTray to configure ejection, it accepts:
           //   - For Avansia printers:
           //     * STD            -> left side of the printer
           //     * STD_STANDBY    -> left side of the printer + standby
           //     * ERR            -> right side of the printer
           //     * ERR_STANDBY    -> right side of the printer

//           r = pPrinterInstance->Print_ExtraCommand(nTimeout, (char*) "setErrorTray", (char*) "ERR", reply, ec);
//           OutputMsg("> setErrorTray: %d\n", r);
           // setErrorTray to configure rejection, it accepts:
           //   - For Avansia printers:
           //     * STD            -> left side of the printer
           //     * STD_STANDBY    -> left side of the printer + standby
           //     * ERR            -> right side of the printer
           //     * ERR_STANDBY    -> right side of the printer


            OutputMsg("Succeed in Print_Open:%d",x);
            ui->pushButton_PrinterOpen->setEnabled(false);
            QList<QPushButton *> buttonList = findChildren<QPushButton *>();
            for (auto var:buttonList)
                var->setEnabled(true);
            ui->pushButton_PrinterOpen->setEnabled(false);
            ui->comboBox_Printer->setEnabled(false);
        }
    }
    else
    {
        OutputMsg("Printer has been Opened!");
        return;
    }
}

void MainWindow::Printer_SetOverlayer()
{
    CheckPriner(pPrinterInstance);

    char szRCode[1024] = {0};
    long lTimeout = 2000;
    char szCommandout[128] = {0};

    if (pPrinterInstance->Print_ExtraCommand(lTimeout,"Set Overlayer",(void *)"assets:/overlayer.bmp",(LPVOID &)szCommandout,szRCode))
    {
        OutputMsg("Print_GetBoxStatus Failed!");
    }
}

void MainWindow::ClosePrinter()
{
    if (pAutoPrinter)
    {
        char szRCode[1024] = {0};
        pPrinterInstance->Print_Close(szRCode);
        pPrinterInstance = nullptr;
        pAutoPrinter->pPrinterInstance = nullptr;
        delete pAutoPrinter;
        pAutoPrinter = nullptr;

        ui->pushButton_PrinterOpen->setEnabled(true);
        QList<QPushButton *> buttonList = findChildren<QPushButton *>();
        for (auto var:buttonList)
            var->setEnabled(false);
        ui->pushButton_PrinterOpen->setEnabled(true);
        ui->comboBox_Printer->setEnabled(true);
    }
}
void MainWindow::Printer_GetBoxStatus()
{
    CheckPriner(pPrinterInstance);

    char szRCode[1024] = {0};

    long lTimeout = 2000;
    Lithograph::LITHOGRAPHBOXUNIT boxesUnit[10];
    Lithograph::LITHOGRAPHBOXINFO boxesInfo = {10, boxesUnit};
    Lithograph::LPLITHOGRAPHBOXINFO lpBoxInfo = &boxesInfo;
    if (pPrinterInstance->Print_GetBoxStatus(lTimeout,lpBoxInfo,szRCode))
    {
        OutputMsg("Print_GetBoxStatus Failed!");
    }
    else
    {
        if (lpBoxInfo->nCount)
        {
            const char *szBoxType[] = {"未知","发卡箱","回收箱"};
            const char *szBoxStatus[] = {"正常", "卡少","无卡", "不可操作", "不存在", "高(快满)","满","未知"};
            const char *szSense[] = {"否","是"};

            OutputMsg("Print_GetBoxStatus Succeed,Boxes Count:%d",lpBoxInfo->nCount);
            OutputMsg("Card boxes status:");
            OutputMsg("\tBoxNo\tType\tStatus\tSensorSupported");
            QString strLine ;
            for(int i = 0;i < lpBoxInfo->nCount;i++)
            {
                strLine = QString("\t%1\t%2\t%3\t%4").arg(lpBoxInfo->lpplist[i].byBoxNumber).arg(szBoxType[lpBoxInfo->lpplist[i].byType]).arg(szBoxStatus[lpBoxInfo->lpplist[i].byStatus]).arg(szSense[lpBoxInfo->lpplist[i].bHardwareSensor]);
                OutputMsg("%s",strLine.toStdString().c_str());
            }
        }
        else
        {
            OutputMsg("There is no cardbox.");
        }

    }
}

void MainWindow::Printer_InsertCard()
{
    CheckPriner(pPrinterInstance);

    char szRCode[1024] = {0};

    long lTimeout = 2000;
    int nBox = ui->comboBox_cardbox->currentIndex();
    int nDispPos = ui->comboBox_DispPOS->currentIndex();

    if (pPrinterInstance->Print_Dispense(lTimeout,nBox,nDispPos,szRCode))
        OutputMsg("Failed in Print_Dispense:%s",szRCode);
    else
        OutputMsg("Print_Dispense Succeed:%s.",szRCode);
}

void MainWindow::Printer_EjectCard()
{
    CheckPriner(pPrinterInstance);

    char szRCode[1024] = {0};
    long lTimeout = 2000;
    if (pPrinterInstance->Print_Eject(lTimeout,szRCode))
        OutputMsg("Failed in Print_Eject:%s",szRCode);
    else
        OutputMsg("Print_Eject Succeed.",szRCode);
}

 void MainWindow::Printer_Retract()
 {

     CheckPriner(pPrinterInstance);

     char szRCode[1024] = {0};
     long lTimeout = 2000;
     if (pPrinterInstance->Print_Retract(lTimeout,0,szRCode))
         OutputMsg("Failed in Print_Retract:%s",szRCode);
     else
         OutputMsg("Print_Retract Succeed.",szRCode);
 }

void MainWindow::Printer_GetStatus()
{
    CheckPriner(pPrinterInstance);
    long lTimeout = 2000;
    char szRCode[1024] = {0};
    Lithograph::LPLITHOGRAPHSTATUS lpStatus = new Lithograph::LITHOGRAPHSTATUS;

    char *szDevice[] = {"在线","忙","不在线","故障"};
    char *szMedia[] = {"无卡","卡在门口","卡在内部","卡在上电位","卡在闸门外","堵卡","卡片未知"};
    char *szToner[] = {"FLLL","LOW","OUT","NOTSUPP","UNKNOW"};
    if (pPrinterInstance->Print_Status(lTimeout,lpStatus,szRCode))
    {
        OutputMsg("Print_Status Failed!");
        return ;
    }
    OutputMsg("Device = %s\tMedia = %s\tToner = %s.",szDevice[lpStatus->fwDevice],szMedia[lpStatus->fwMedia],szToner[lpStatus->fwToner]);
}

void MainWindow::Printer_Start()
{

    CheckPriner(pPrinterInstance);
    long lTimeout = 2000;
    char szRCode[1024] = {0};

    LPVOID lpCmdOut = nullptr;
    QString strTempFile = QDir::currentPath();
    strTempFile += "/PrintPriew.bmp";
    pPrinterInstance->Print_ExtraCommand(lTimeout,"Set SavePreview",(LPVOID)strTempFile.toStdString().c_str(),lpCmdOut,szRCode);

    if (pPrinterInstance->Print_StartPrint(lTimeout,szRCode))
    {
        OutputMsg("Print_StartPrint Failed!");
        return ;
    }
}

void MainWindow::Printer_Reset()
{
    CheckPriner(pPrinterInstance);
    long lTimeout = 2000;
    char szRCode[1024] = {0};
    nResetOption = ui->comboBox_cardpos->currentIndex() + 1;

    if (pPrinterInstance->Print_Reset(lTimeout,nResetOption,szRCode))
    {
        OutputMsg("Print_Reset Failed!");
        return ;
    }
    else
    {
         OutputMsg("Print_Reset Succeed!");
    }
}


void MainWindow::Printer_ICPowerOn()
{
    CheckPriner(pPrinterInstance);
    char szRCode[1024] = {0};
    int nArtlen = 0;
    long lTimeout = 2000;
    unsigned char szUID[1024] = {0};
    int nUidLen = 0;
    unsigned char szout[1024] = {0};
    if (pPrinterInstance->Print_IcPowerOn(lTimeout,szout,nArtlen,(unsigned char *)szUID,nUidLen,szRCode))
    {
        OutputMsg("Print_IcPowerOn Failed!");
    }
    else
    {
        OutputMsg("Print_IcPowerOn Succeed!");
    }
}

void MainWindow::Printer_ICPowerOff()
{
    CheckPriner(pPrinterInstance);
    char szRCode[1024] = {0};
    long lTimeout = 2000;
    if (pPrinterInstance->Print_IcPowerOff(lTimeout,szRCode))
    {
        OutputMsg("Print_IcPowerOn Failed!");
    }
    else
    {
        OutputMsg("Print_IcPowerOn Succeed!");
    }
}

void MainWindow::Printer_ICExchange(const char *szCmd)
{
    CheckPriner(pPrinterInstance);
    OutputMsg("IC Command = %s\tlength = %d.",szCmd,strlen(szCmd));
    char szRCode[1024] = {0};
    long lTimeout = 2000;
    char szOut[1024] = {0};
    int nOutLen = 1024;
    if (pPrinterInstance->Print_IcExchange(lTimeout,(unsigned char *)szCmd,strlen(szCmd),(unsigned char *)szOut,nOutLen,szRCode))
        OutputMsg("Print_IcExchange Failed!");
    else
        OutputMsg("Print_IcExchange Succeed %d Byte:%s!",nOutLen,szOut);
}


void MainWindow::Printer_Depense()
{
    CheckPriner(pPrinterInstance);
    char szRCode[1024] = {0};

    long lTimeout = 2000;
    if (pPrinterInstance->Print_Dispense(lTimeout,1,2,szRCode))
    {
        OutputMsg("Print_Dispense Failed!");
        return ;
    }
    else
        OutputMsg("Print_Dispense Succeed!");
}

void MainWindow::on_pushButton_ExtraCommand_clicked()
{
    WaitCursor();
    CheckPriner(pPrinterInstance);
    char szRCode[1024] = {0};
    int x;
    int nArtlen = 0;
    long lTimeout = 2000;
    unsigned char szUID[1024] = {0};
    int nUidLen = 0;
    unsigned char szout[1024] = {0};
    //pPrinterInstance->Print_IcPowerOff(lTimeout, szRCode);
    //Result(x,pPrinterInstance->Print_IcPowerOn(lTimeout,szout,nArtlen,(unsigned char *)szUID,nUidLen,szRCode));

    QString qstrCommand = ui->comboBox_Extracommand->currentText();
    QByteArray baCommand = qstrCommand.toLatin1();

    QString qstrData = ui->comboBox_ExtracommandData->currentText();
    QByteArray baData = qstrData.toLatin1();

    LPVOID szCommandout = nullptr;
    if (pPrinterInstance->Print_ExtraCommand(lTimeout,baCommand.data(),baData.data(),szCommandout,szRCode))
    {
        OutputMsg("Print_ExtraCommand Failed!:%s",szRCode);
    }
    else
    {
        OutputMsg("Print_ExtraCommand Succeed:");
        QString strmsg = QString::fromLocal8Bit((char *)szCommandout);
        OutputMsg("%s.\n",strmsg.toStdString().c_str());
    }
    //Result(x,pPrinterInstance->Print_IcPowerOff(lTimeout, szRCode));
}

enum Printer_Func
{
    Open_Printer,
    Set_Overlayer,
    Close_Printer,
    CardBox_status,
    Insert_Card,
    Eject_Card,
    Retract,
    Printer_status,
    Start_Print,
    Depense,
    OpenReader
};

void MainWindow::on_pushButton_browsegraph_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        QString strFile = fileName;
        qDebug()<<strFile;
        ui->lineEdit_graphpath->setText(strFile);

        QImage Convas(1024, 650, QImage::Format_RGB888);
        Convas.fill(Qt::white);
        QPainter painter(&Convas);
        painter.setCompositionMode(QPainter::CompositionMode_Source);

        QImage ImageLoad;
        painter.setRenderHint(QPainter::Antialiasing, true);
        float fAngle = ui->lineEdit_graph_angle->text().toFloat();

        /* 坐标转换公式
         * MM_w=Pixel_x*25.4/dpi
         * MM_h=Pixel_y*25.4/dpi
         * Pixel_x=MM_w*dpi/25.4
         * Pixel_y=MM_h*dpi/25.4
        */
        const double nDPI = 300;
        const double fScale = nDPI/25.4;
        if (ImageLoad.load(strFile))
        {
            double nX = ui->lineEdit_graphX->text().toDouble();
            double nY = ui->lineEdit_graphY->text().toDouble();
            double nW = ui->lineEdit_graphW->text().toDouble();
            double nH = ui->lineEdit_graphH->text().toDouble();

            double fPixel_x = nX*fScale;
            double fPixel_y = nY*fScale;
            double fW = nW*fScale;
            double fH = nH*fScale;

            painter.save();
            int nConvasWidth = Convas.width();
            int nConvasHeight = Convas.height();
            painter.translate(nConvasWidth/2,nConvasHeight/2);

            painter.rotate(fAngle);
            painter.translate(-nConvasWidth/2,-nConvasHeight/2);
            painter.drawImage(QRectF(fPixel_x,fPixel_y,fW,fH),ImageLoad);
            painter.restore();

            QFileInfo fi(strFile);
            QString strImageSave = fi.absoluteDir().absolutePath() + "/Sample.bmp";
            OutputMsg(strImageSave.toStdString().c_str());
            Convas.save(strImageSave);
        }
    }
}

void MainWindow::on_pushButton_Font_clicked()
{
    //setOrientation(SCREEN_ORIENTATION::SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
}


bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
//    if(obj == ui->lineEdit_text_Color && e->type() == QEvent::MouseButtonPress)
//    {
//        QMouseEvent *me = (QMouseEvent*)e;
//        if(me->button() == Qt::LeftButton)
//        {
//            QColor color = QColorDialog::getColor(Qt::red,this, tr("Color Dialog"));
//            QString strColor = QString::number((uint)color.rgb(),16);
//            ui->lineEdit_text_Color->setText(strColor);
//        }
//    }
    return QMainWindow::eventFilter(obj,e);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ClosePrinter();
}

void MainWindow::on_pushButton_PrinterOpen_clicked()
{
    WaitCursor();
    OpenPrinter();
}

void MainWindow::on_pushButton_PrinterInsert_clicked()
{
    WaitCursor();
    Printer_InsertCard();
}

void MainWindow::on_pushButton_PrinterEject_clicked()
{
    WaitCursor();
    Printer_EjectCard();
}

//void MainWindow::on_pushButton_PrinterReject_clicked()
//{
//    WaitCursor();
//    Printer_Retract();
//}

void MainWindow::on_pushButton_PrinterClose_clicked()
{
    WaitCursor();
    ClosePrinter();
}

void MainWindow::on_pushButton_PrinterStatus_clicked()
{
    WaitCursor();
    Printer_GetStatus();
}

void MainWindow::on_pushButton_PrinterBoxStatus_clicked()
{
    WaitCursor();
    Printer_GetBoxStatus();
}

void MainWindow::on_pushButton_PrinterInit_clicked()
{
    WaitCursor();
        CheckPriner(pPrinterInstance);
    long lTimeout = 2000;
    char szRCode[1024] = {0};

    if (pPrinterInstance->Print_InitPrint(lTimeout,szRCode))
    {
        OutputMsg("Print_InitPrint Failed!");
        return ;
    }
    else
         OutputMsg("Print_InitPrint Succeed!");
}

void MainWindow::on_pushButton_PrinterSetImage_clicked()
{
    WaitCursor();

    //if (ui->checkBox_CheckPrinter->isChecked())
        CheckPriner(pPrinterInstance);
    long lTimeout = 2000;
    char szRCode[1024] ={0};

    QString strFileImage = ui->lineEdit_graphpath->text();
    if (strFileImage.size() <= 0)
    {
        OutputMsg("Plz Select a Image to Print!");
        return;
    }
    int nX = ui->lineEdit_graphX->text().toInt();
    int nY = ui->lineEdit_graphY->text().toInt();
    int nW = ui->lineEdit_graphW->text().toInt();
    int nH = ui->lineEdit_graphH->text().toInt();
    int nAngle = ui->lineEdit_graph_angle->text().toInt();

    QImage Convas(1024, 650, QImage::Format_RGB888);
    Convas.fill(Qt::white);
    QPainter painter(&Convas);

    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QImage ImageLoad;
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (ImageLoad.load(strFileImage))
    {
        painter.save();
        int nConvasWidth = Convas.width();
        int nConvasHeight = Convas.height();
        painter.translate(nConvasWidth/2,nConvasHeight/2);
        if (nAngle)
            painter.rotate(nAngle);
        painter.translate(-nConvasWidth/2,-nConvasHeight/2);
        painter.drawImage(QRectF(nX,nY,nH,nW),ImageLoad);
        painter.restore();
    }

//    if (ui->checkBox_CheckPrinter->isChecked())
//    {
        if (pPrinterInstance->Print_PrintImage(lTimeout,(char *)strFileImage.toStdString().c_str(),nAngle,nX,nY,nH,nW,szRCode))
            OutputMsg("Print_PrintImage Failed!");
        else
            OutputMsg("Print_PrintImage Succeed!");
//    }
//    else
//        OutputMsg("Picture has input!\n");
}

void MainWindow::on_pushButton_PrinterSetText_clicked()
{
    WaitCursor();
    QString strFont = ui->lineEdit_Font->text();
    if (!strFont.size())
    {
        OutputMsg("The font can't be empty,plz select a font file.");
        return ;
    }
    CheckPriner(pPrinterInstance);
    long lTimeout = 2000;

    char szRCode[1024] ={0};
//    float x = ui->lineEdit_x->text().toFloat();
//    float y = ui->lineEdit_y->text().toFloat();
//    int nAngle = ui->lineEdit_text_angle->text().toInt();

//    int size = ui->lineEdit_text_size->text().toInt();
//    QString imageText = ui->lineEdit_text->text();

//    QPainter painter(pConvas);
//    /* 设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
//    改变组合模式和上面的填充方式可以画出透明的图片。*/
//    painter.setCompositionMode(QPainter::CompositionMode_Source);
//    //图片上的字符串，例如 HELLO
//    QFont font("宋体");
//    //设置显示字体的大小
//    font.setPixelSize(size);
//    QFontMetrics fm(font);
//    int charWidth = fm.horizontalAdvance(imageText);
//    auto TextRect = fm.boundingRect(imageText);
//    charWidth = fm.boundingRect(imageText).width();
//    painter.setFont(font);
//    QPen pen = painter.pen();
//    pen.setColor(QColor(0, 0, 0));
//    painter.setPen(pen);
//    int nConvasWidth = pConvas->width();
//    int nConvasHeight = pConvas->height();

//    painter.translate(nConvasWidth,nConvasHeight);
//    painter.save();
//    painter.rotate(nAngle);
//    painter.translate(-nConvasWidth,-nConvasHeight);
//    painter.drawText(QRectF(x, y, TextRect.width(), TextRect.height()), Qt::AlignLeft | Qt::AlignTop, imageText);

//    painter.restore();
////    ui->label_Image->setPixmap(QPixmap::fromImage(*pConvas));

////    if (ui->checkBox_CheckPrinter->isChecked())
////    {

     int nAngle = 180;
     int size = 8;
      pPrinterInstance->Print_PrintText(lTimeout,"姓名  测试用户",nAngle,28,14.5,(char *)strFont.toStdString().c_str(),size,1,0,szRCode);
      pPrinterInstance->Print_PrintText(lTimeout,"社会保障号码  123456789012345678",nAngle,28,19,(char *)strFont.toStdString().c_str(),size,1,0,szRCode);
      pPrinterInstance->Print_PrintText(lTimeout,"社会保障卡号  ABCDEFGHIJKLMN",nAngle,28,23.5,(char *)strFont.toStdString().c_str(),size,1,0,szRCode);
      pPrinterInstance->Print_PrintText(lTimeout,"发卡日期  2019年9月27日",nAngle,28,28,(char *)strFont.toStdString().c_str(),size,1,0,szRCode);

      OutputMsg("Print_PrintText Succeed!");

}

void MainWindow::on_pushButton_PrinterICOn_clicked()
{
    Printer_ICPowerOn();
}

void MainWindow::on_pushButton_PrinterICExchange_clicked()
{
    WaitCursor();
    QString strICCommand = ui->comboBox_ICcommand->currentText();

    Printer_ICExchange(strICCommand.toStdString().c_str());
}

void MainWindow::on_pushButton_PrinterICOff_clicked()
{
    WaitCursor();
    Printer_ICPowerOff();
}

void MainWindow::on_pushButton_PrinterStart_clicked()
{
    WaitCursor();
    Printer_Start();
}

void MainWindow::on_pushButton_PrinterReset_clicked()
{
    WaitCursor();
    Printer_Reset();
}

void MainWindow::on_pushButton_PrinterConfigure_Get_clicked()
{
    WaitCursor();
}

void MainWindow::on_pushButton_PrinterConfigure_Set_clicked()
{
    WaitCursor();

}

bool MainWindow::GetCardBoxStatus()
{
    char szRCode[1024] = {0};

    long lTimeout = 2000;
    Lithograph::LPLITHOGRAPHBOXINFO lpBoxInfo = new Lithograph::LITHOGRAPHBOXINFO;
    if (pPrinterInstance->Print_GetBoxStatus(lTimeout,lpBoxInfo,szRCode))
    {
        OutputMsg("Print_GetBoxStatus Failed!");
        return false;
    }
    else
    {
        for(int i = 0;i < lpBoxInfo->nCount;i++)
        {
            if (lpBoxInfo->lpplist[i].byType == 1 &&  //  发卡箱
                lpBoxInfo->lpplist[i].byStatus == 2)  // 无卡
                return false;
        }
    }
    return true;
}
void MainWindow::PrintJos()
{


}


void MainWindow::on_radioButton_EnableCardOnce_clicked()
{
    WaitCursor();
    CheckPriner(pPrinterInstance);
    long lTimeout = ui->lineEdit_EnableCardTimeout->text().toInt();
    char szRCode[1024] = {0};
    OutputMsg("准备从出卡口进卡,请在%d秒内插入卡片.");
    if (pPrinterInstance->Print_EnableCard(lTimeout,1,szRCode))
    {
        OutputMsg("Print_EnableCard Failed!");
        return ;
    }
    OutputMsg("Print_EnableCard Succeed.");
}


void MainWindow::on_radioButton_EnableCardAlways_clicked()
{
    WaitCursor();
    CheckPriner(pPrinterInstance);
    long lTimeout = ui->lineEdit_EnableCardTimeout->text().toInt();
    char szRCode[1024] = {0};
    OutputMsg("准备从出卡口进卡,若要从卡箱进卡,请点击禁‘禁止出卡口进卡’.\n",lTimeout/1000);
    if (pPrinterInstance->Print_EnableCard(lTimeout,0,szRCode))
    {
        OutputMsg("Print_EnableCard Failed!");
        return ;
    }
    OutputMsg("Print_EnableCard Succeed.");
}


void MainWindow::on_radioButton_DisableCard_clicked()
{
    WaitCursor();
    CheckPriner(pPrinterInstance);
    long lTimeout = ui->lineEdit_EnableCardTimeout->text().toInt();
    char szRCode[1024] = {0};
    OutputMsg("禁止出卡口进卡,此后将从卡箱进卡.");
    if (pPrinterInstance->Print_DisableCard(lTimeout,szRCode))
    {
        OutputMsg("Print_DisableCard Failed!");
        return ;
    }
    OutputMsg("Print_DisableCard Succeed.");
}


void MainWindow::on_comboBox_Port_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "USB")
        ui->lineEdit_Address->setText("/dev/usb/lp0");
    else if (arg1 == "IP")
        ui->lineEdit_Address->setText("192.168.1.199");
    else if (arg1 == "NISCA")
        ui->lineEdit_Address->setText("Evolis Avansia");
}

void MainWindow::on_pushButton_browseFont_clicked()
{
    const QString fileName = QFileDialog::getOpenFileName(this);

     if (!fileName.isEmpty())
     {
        QString strFile = fileName;
        qDebug()<<strFile;
        ui->lineEdit_Font->setText(strFile);
    }
}

void MainWindow::on_MainWindow_destroyed()
{
     ClosePrinter();
}

void MainWindow::on_pushButton_Printer_clicked()
{
    WaitCursor();
    Printer_Retract();
}

void MainWindow::on_pushButton_SetPrinterOption_clicked()
{
    char *szOption[2][2] = {
        {"FColorBrightness","FColorContrast"},
        {"BColorBrightness","BColorContrast"},
    };
    int nCardSide = ui->comboBox_cardside->currentIndex();
    int nOption = ui->comboBox_PrinterOption->currentIndex();
    CheckPriner(pPrinterInstance);
    int nVal = ui->lineEdit_OptioinVal->text().toInt();
    char szVal[16] = {0};
    sprintf(szVal,"VAL%d",nVal);
    char szCommandout[128] = {0};

    char szRCode[1024] = {0};
    long lTimeout = 2000;

    if (!pPrinterInstance->Print_ExtraCommand(lTimeout,szOption[nCardSide][nOption],(void *)szVal,(LPVOID &)szCommandout,szRCode))
        OutputMsg("Set Printer Option[%s] to %s Succeed.",szOption[nCardSide][nOption],szVal);
}
