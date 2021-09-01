QT +=  core
#android {
#QT += androidextras
#}


TEMPLATE = lib
DEFINES += EVOLIS_Z390_LITHOGRAPHPRINTER_LIBRARY\
        TIXML_USE_STL

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    AES.cpp \
    evolis_z390_lithographprinter.cpp \
    tinyxml/ZBase64.cpp \
    tinyxml/tables.cpp \
    tinyxml/tinystr.cpp \
    tinyxml/tinyxml.cpp \
    tinyxml/tinyxmlerror.cpp \
    tinyxml/tinyxmlparser.cpp
INCLUDEPATH +=$$PWD/SDK/decard/include

DEPENDPATH+= $$PWD/SDK/decard/lib
HEADERS += \
    AES.h \
    Evolis_Z390_LithographPrinter_global.h \
    LithographPrinter.h \
    SDK/decard/include/dcrf32.h \
    evolis_z390_lithographprinter.h \
    tinyxml/ZBase64.h \
    tinyxml/tables.h \
    tinyxml/targetver.h \
    tinyxml/tinystr.h \
    tinyxml/tinyxml.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target



RESOURCES += \
    Z390.qrc


QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable


INCLUDEPATH += $$PWD/SDK/decard/Lib
DEPENDPATH += $$PWD/SDK/decard/Lib

unix:!macx: LIBS += -L$$PWD/SDK/decard/Lib/ -lwlt

INCLUDEPATH += $$PWD/SDK/decard/Lib
DEPENDPATH += $$PWD/SDK/decard/Lib

unix:!macx: LIBS += -L$$PWD/SDK/decard/Lib/ -ldcrf32

INCLUDEPATH += $$PWD/SDK/decard/Lib
DEPENDPATH += $$PWD/SDK/decard/Lib
