#include "CLithographPrinter.h"

#include <cstdio>

void example(CLithographPrinter* lp, const char* type, const char* device)
{
    LPVOID reply = NULL;
    LPVOID input = NULL;
    char   ec[5];
    int    r;
#define TIMEOUT 30000

    r = lp->Print_Open((char*) type, (char*) device, ec);
    printf("> Print_Open: %d\n", r); fflush(stdout);
    if (r > 0)
        return ;

    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "getPrintingMode",
            input, reply, ec);
    printf("> getPrintingMode: %d\n", r);
    if(r == 0) {
        printf("> Printing mode: %s\n", (char*) reply);
    }
    // getPrintingMode to get printing mode of current printer:
    //   - For Avansia printers:
    //     * DUPLEX -> only duplex printing is available on Avansia

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
