QT +=  core

TEMPLATE = lib
DEFINES += EVOLIS_Z390_LITHOGRAPHPRINTER_LIBRARY\
        TIXML_USE_STL

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AES.cpp \
    QEvolisPrinter.cpp \
    evolis_z390_printer.cpp \
    tinyxml/ZBase64.cpp \
    tinyxml/tables.cpp \
    tinyxml/tinystr.cpp \
    tinyxml/tinyxml.cpp \
    tinyxml/tinyxmlerror.cpp \
    tinyxml/tinyxmlparser.cpp \
    readerbase.cpp

HEADERS += \
    AES.h \
    Evolis_Z390_Printer_global.h \
    LithographPrinter.h \
    QEvolisPrinter.h \
    tinyxml/ZBase64.h \
    tinyxml/tables.h \
    tinyxml/targetver.h \
    tinyxml/tinystr.h \
    tinyxml/tinyxml.h \
    evolis_z390_printer.h \    
    dynamiclib.h \
    readerbase.h \
    readerMinhua.h \
    readerdecard.h


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target


RESOURCES +=


unix:!macx: LIBS +=  -ldl


INCLUDEPATH += $$PWD/../SDK/Lib
DEPENDPATH += $$PWD/../SDK/Lib

INCLUDEPATH += $$PWD/opencv/include

QMAKE_CXXFLAGS += -Wl,-rpath=./
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable


unix:!macx: LIBS += -L$$PWD/../3rdparty/lib/ -lopencv_world

INCLUDEPATH += $$PWD/../3rdparty
DEPENDPATH += $$PWD/../3rdparty

unix:!macx: PRE_TARGETDEPS += $$PWD/../3rdparty/lib/libopencv_world.a
