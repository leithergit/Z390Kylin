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
    if (r > 0)
        return ;
    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setInputTray", (char*) "FEEDER", reply, ec);
    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setOutputTray", (char*) "STD", reply, ec);

    printf("> Please insert a card...\n"); fflush(stdout);
    r = lp->Print_EnableCard(TIMEOUT, 0, ec);
    r = lp->Print_InitPrint(TIMEOUT, ec);
    printf("> Print_PrintText: "); fflush(stdout);
    r = lp->Print_PrintText(TIMEOUT, (char*) "With SIMPLEX", 0, 200, 200, (char*) "SIMPLEX", 7, 2, 0x52c0ff, ec);
    printf("%d\n> Print_PrintText: ", r); fflush(stdout);
    r = lp->Print_PrintText(TIMEOUT, (char*) "With COMPLEX", 45, 1200, 500, (char*) "COMPLEX", 3, 5, 0xeb0707, ec);
    printf("%d\n> Print_PrintText: ", r); fflush(stdout);
    r = lp->Print_PrintText(TIMEOUT, (char*) "With SIMSUN.ttf 你 好", 0, 200, 400, (char*) "resources/SIMSUN.ttf", 125, 5, 0xeb0707, ec);
    printf("%d\n> Print_StartPrint: ", r); fflush(stdout);
    r = lp->Print_StartPrint(TIMEOUT * 4, ec);
    printf("%d\n> Print_Eject: ", r); fflush(stdout);
    r = lp->Print_Eject(TIMEOUT, ec);
    printf("%d\n", r);

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
