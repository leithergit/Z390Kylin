#include "mainwindow.h"
#include <QApplication>
//#include "./Z390/include/evolis_z390_lithographprinter.h"

#include <QScreen>
#include <QDebug>
#include <QString>
#include <QDir>


int main(int argc, char *argv[])
{
    QString strPath = QDir::currentPath();
    QString strEnv = "LD_LIBRARY_PATH:"+strPath;
    qputenv("LD_LIBRARY_PATH",strEnv.toUtf8());
    strEnv = "PATH:"+ strPath;
    qputenv("PATH",strEnv.toUtf8());
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
