QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS +=  mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RESOURCES += \
    Z390Sample.qrc

unix:!macx: LIBS +=  -ldl

INCLUDEPATH += $$PWD/SDK/include
DEPENDPATH += $$PWD/SDK/include

unix:!macx: LIBS += -L$$PWD/../build-Evolis_Z390_LithographPrinter-unknown-Debug/ -lEvolis_Z390_LithographPrinter

INCLUDEPATH += $$PWD/../build-Evolis_Z390_LithographPrinter-unknown-Debug
DEPENDPATH += $$PWD/../build-Evolis_Z390_LithographPrinter-unknown-Debug
