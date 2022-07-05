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
    mainwindow.ui \
    mainwindow_cn.ui

QMAKE_LFLAGS += -Wl,--rpath=./

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Z390Sample.qrc

unix:!macx: LIBS +=  -ldl
#unix:!macx: LIBS += -L$$PWD/../SDK/Lib/ -lwlt
#unix:!macx: LIBS += -L$$PWD/../SDK/Lib/ -ldcrf32

INCLUDEPATH += $$PWD/../build-Release
DEPENDPATH += $$PWD/../build-Release
TRANSLATIONS += chs.ts
