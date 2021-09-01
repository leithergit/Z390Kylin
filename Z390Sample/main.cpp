#include "mainwindow.h"
#include <QApplication>
//#include "./Z390/include/evolis_z390_lithographprinter.h"

#include <QScreen>
#include <QDebug>
#include <QString>
#include <QDir>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    QString strCurrentPath = QDir::currentPath();
    qDebug()<<strCurrentPath;
    w.show();
    return a.exec();
}
