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
    tinyxml/tinyxmlparser.cpp

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
    ../SDK/include/ava-printers.h \
    ../SDK/include/dcrf32.h \
    ../SDK/include/evo-printers.h \
    ../SDK/include/evolis.h \
    evolis_z390_printer.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    Z390.qrc


unix:!macx: LIBS += -lfreetype
unix:!macx: LIBS += -lharfbuzz
unix:!macx: LIBS += -lopencv_core
unix:!macx: LIBS += -lopencv_imgproc
unix:!macx: LIBS += -lopencv_freetype
unix:!macx: LIBS += -lopencv_imgcodecs

unix:!macx: LIBS +=  -ldl
unix:!macx: LIBS += -L$$PWD/../SDK/Lib/ -lwlt
unix:!macx: LIBS += -L$$PWD/../SDK/Lib/ -ldcrf32
unix:!macx: LIBS += -L$$PWD/../SDK/Lib/ -levolis

INCLUDEPATH += $$PWD/../SDK/Lib
DEPENDPATH += $$PWD/../SDK/Lib

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
