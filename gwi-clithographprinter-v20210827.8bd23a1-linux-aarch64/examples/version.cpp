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

    char version[32] = { 0 };
    r = lp->Print_GetVersion(TIMEOUT, version, ec);
    printf("> Version: %s\n", version);

    lp->Print_Close(ec);
}
