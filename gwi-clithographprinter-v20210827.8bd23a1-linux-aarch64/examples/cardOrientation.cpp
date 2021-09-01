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

    printf("> Please insert a card...\n"); fflush(stdout);
    r = lp->Print_EnableCard(TIMEOUT, 0, ec);
    r = lp->Print_InitPrint(TIMEOUT, ec);
    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setCardOrientation", (char*) "LANDSCAPE_CC90", reply, ec);
    printf("> setCardOrientation: %d\n", r);
    // setCardOrientation to configure card orientation, it accepts:
    //     * LANDSCAPE_CC90 -> landscape
    //     * PORTRAIT       -> portrait
    r = lp->Print_PrintImage(TIMEOUT, (char*) "resources/front.bmp", 0, 0, 0, 0, 0, ec);
    printf("> Print_StartPrint: "); fflush(stdout);
    r = lp->Print_StartPrint(TIMEOUT * 4, ec);
    printf("%d\n> Print_Eject: ", r); fflush(stdout);
    r = lp->Print_Eject(TIMEOUT, ec);
    printf("%d\n", r);

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
