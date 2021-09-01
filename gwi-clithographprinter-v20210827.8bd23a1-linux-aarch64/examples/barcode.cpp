#include "CLithographPrinter.h"

#include <cstdio>

void example(CLithographPrinter* lp, const char* type, const char* device)
{
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
    printf("> Print_PrintBarcode: "); fflush(stdout);
    r = lp->Print_PrintBarcode(TIMEOUT, 0, (char*) "c128", 200, 200, 4, 1, true, (char*) "0123456789", ec);
    r = lp->Print_PrintBarcode(TIMEOUT, 45, (char*) "qrCode", 1100, 800, 4, 1, true, (char*) "0123456789", ec);
    r = lp->Print_PrintBarcode(TIMEOUT, 0, (char*) "c39", 300, 500, 4, 20, false, (char*) "0123456789", ec);
    printf("%d\n> Print_StartPrint: ", r); fflush(stdout);
    r = lp->Print_StartPrint(TIMEOUT * 4, ec);
    printf("%d\n> Print_Eject: ", r); fflush(stdout);
    r = lp->Print_Eject(TIMEOUT, ec);
    printf("%d\n", r);

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
