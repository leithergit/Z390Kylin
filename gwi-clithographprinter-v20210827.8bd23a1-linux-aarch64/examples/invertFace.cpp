#include "CLithographPrinter.h"

#include <cstdio>

void example(CLithographPrinter* lp, const char* type, const char* device)
{
    LPVOID reply;
    char   ec[5];
    int    r;
#define TIMEOUT 30000

    r = lp->Print_Open((char*) type, (char*) device, ec);
    printf("> Print_Open: %d\n", r); fflush(stdout);

    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setInvertFaces",
            (char*) "false", reply, ec);
    printf("> setInvertFaces: %d\n", r);
    // setInvertFaces to invert front and back faces, it accepts
    // "true" to invert the faces or false otherwise.

    printf("> Please insert a card...\n"); fflush(stdout);
    r = lp->Print_EnableCard(TIMEOUT, 0, ec);
    r = lp->Print_InitPrint(TIMEOUT, ec);
    r = lp->Print_PrintImage(TIMEOUT, (char*) "resources/front.bmp", 0, 0, 0, 0, 0, ec);
    r = lp->Print_Rotate(TIMEOUT, 0, ec);
    r = lp->Print_PrintImage(TIMEOUT, (char*) "resources/back.bmp", 0, 0, 0, 0, 0, ec);
    printf("> Start printing...\n"); fflush(stdout);
    r = lp->Print_StartPrint(TIMEOUT * 4, ec);
    r = lp->Print_Rotate(TIMEOUT, 0, ec);
    printf("> Card ejection...\n"); fflush(stdout);
    r = lp->Print_Eject(TIMEOUT, ec);

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
