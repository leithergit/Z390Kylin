#include "mainwindow.h"
#include <QApplication>
//#include "./Z390/include/evolis_z390_lithographprinter.h"

#include <QScreen>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonParseError>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <string>
#include <QImage>
using namespace std;


int TestJson()
{

    QFile fJson("/home/gwi/git/Z390Kylin/T.json");
    fJson.open(QIODevice::ReadOnly);
    QByteArray baJson = fJson.readAll();
    QJsonParseError jsErr;
    QJsonDocument jsdoc = QJsonDocument::fromJson(baJson, &jsErr);
    if (jsdoc.isNull())
    {
        qDebug("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
        QString strJson = QString::fromLocal8Bit(baJson);
        jsdoc = QJsonDocument::fromJson(strJson.toLatin1(), &jsErr);
        if (jsdoc.isNull())
        {
            qDebug("Error in parser json:%s",jsErr.errorString().toStdString().c_str());
            return -1;
        }
        QByteArray baRaw = jsdoc.toBinaryData();
        QFile fJson1("/home/gwi/git/Z390Kylin/T2.json");
        fJson1.open(QIODevice::WriteOnly);
        fJson1.write(baRaw);
        fJson1.close();
        QJsonObject jsobj = jsdoc.object();
        if (!jsobj.contains("RESULT") ||
            !jsobj.contains("ADDR") ||
            !jsobj.contains("PHONE") ||
            !jsobj.contains("ADDRCODE"))
        {
            qDebug("Can't find 'RESULT','ADDR','ADDRCODE' or 'PHONE' object!");

            return -1;
        }

        string strResult     = jsobj.value("RESULT").toString().toStdString();
        string strAddr       = jsobj.value("ADDR").toString().toLocal8Bit().data();
        string strAddrCode   = jsobj.value("ADDRCODE").toString().toStdString();
        string strPhone      = jsobj.value("PHONE").toString().toStdString();
        qDebug("Result = %s\tAddr = %s\tAddrCode = %s\tPhone = %s",strResult.c_str(),strAddr.c_str(),strAddrCode.c_str(),strPhone.c_str());

    }
    return 0;
}

int main(int argc, char *argv[])
{
    QString strPath = QDir::currentPath();
    QString strEnv = "LD_LIBRARY_PATH:"+strPath;
    qputenv("LD_LIBRARY_PATH",strEnv.toUtf8());
    strEnv = "PATH:"+ strPath;
    qputenv("PATH",strEnv.toUtf8());
    QApplication a(argc, argv);
    //TestJson();
    QString strJpeg ="/home/gwi/git/Z390Kylin/build/signCardHead_2.jpg";
    QImage image(strJpeg);
    if (image.isNull())
    {
        qCritical()<<"Failed in load file "<< strJpeg;
    }
    MainWindow w;

    w.show();
    return a.exec();
}
