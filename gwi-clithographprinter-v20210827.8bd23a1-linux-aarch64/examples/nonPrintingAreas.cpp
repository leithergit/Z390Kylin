#include "CLithographPrinter.h"

#include <cstdio>

void example(CLithographPrinter* lp, const char* type, const char* device)
{
    LPVOID reply;
    char ec[5];
    int  r;
#define TIMEOUT 30000

    r = lp->Print_Open((char*) type, (char*) device, ec);
    printf("> Print_Open: %d\n", r); fflush(stdout);
    if (r > 0)
        return ;

    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setInputTray", (char*) "FEEDER", reply, ec);
    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setOutputTray", (char*) "STD", reply, ec);

    printf("> Please insert a card...\n"); fflush(stdout);
    r = lp->Print_EnableCard(TIMEOUT, 0, ec);
    r = lp->Print_InitPrint(TIMEOUT, ec);

    // setNonPrintingAreas to configure non printing areas (5 areas maximum),
    // only available for YMCKI ribbon:
    //      An area is defined with the coordinate of the its top-left (x0, y0) corner and its
    //      bottom-right corner (x1, y1).
    //      Coordinates of the area are seperated by 'x' and areas are seperated by ':'.
    // Examples of valid areas:
    //      - 1 area : 300x550x900x800
    //      - 2 areas : 300x550x900x800:200x250x800x700
    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setNonPrintingAreas", (char*) "300x550x1100x800", reply, ec);
    r = lp->Print_PrintImage(TIMEOUT, (char*) "resources/tigre.bmp", 0, 0, 0, 0, 0, ec);
    printf("> Print_StartPrint: "); fflush(stdout);

    // Same we can define NonPrintingAreas for back face after Print_Rotate() call.
    r = lp->Print_Rotate(TIMEOUT, 0, ec);
    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setNonPrintingAreas", (char*) "300x550x900x800", reply, ec);
    r = lp->Print_PrintImage(TIMEOUT, (char*) "resources/landscape.jpg", 0, 0, 0, 0, 0, ec);

    r = lp->Print_StartPrint(TIMEOUT * 4, ec);
    printf("%d\n> Print_Eject: ", r); fflush(stdout);
    r = lp->Print_Eject(TIMEOUT, ec);
    printf("%d\n", r);

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
