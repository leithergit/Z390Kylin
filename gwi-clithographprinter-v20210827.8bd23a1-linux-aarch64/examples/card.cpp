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

    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setInputTray",
            (char*) "FEEDER", reply, ec);
    printf("> setInputTray: %d\n", r);
    // setInputTray to configure card insertion, it accepts:
    //   - For Avansia printers:
    //     * FEEDER     -> top feeder
    //     * MANUALNCMD -> right side of the printer + standby

    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setOutputTray",
            (char*) "STD_STANDBY", reply, ec);
    printf("> setOutputTray: %d\n", r);
    // setOutputTray to configure ejection, it accepts:
    //   - For Avansia printers:
    //     * STD            -> left side of the printer
    //     * STD_STANDBY    -> left side of the printer + standby
    //     * ERR            -> right side of the printer
    //     * ERR_STANDBY    -> right side of the printer

    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setErrorTray",
            (char*) "ERR", reply, ec);
    printf("> setErrorTray: %d\n", r);
    // setErrorTray to configure rejection, it accepts:
    //   - For Avansia printers:
    //     * STD            -> left side of the printer
    //     * STD_STANDBY    -> left side of the printer + standby
    //     * ERR            -> right side of the printer
    //     * ERR_STANDBY    -> right side of the printer

    r = lp->Print_ExtraCommand(TIMEOUT, (char*) "setCardSideEject",
            (char*) "FRONT_SIDE_UP", reply, ec);
    printf("> setCardSideEject: %d\n", r);
    // setCardSideEject to configure card orientation, it accepts:
    //   - For Avansia printers:
    //     * DEFAULT         -> default
    //     * FRONT_SIDE_UP   -> front side up
    //     * FRONT_SIDE_DOWN -> front side down

    if (r == 0) {
        r = lp->Print_EnableCard(TIMEOUT, 0, ec);
        r = lp->Print_InitPrint(TIMEOUT, ec);
        r = lp->Print_PrintImage(TIMEOUT, (char*) "resources/front.bmp", 0, 0, 0, 0, 0, ec);
        printf("> Print_StartPrint: "); fflush(stdout);
        r = lp->Print_StartPrint(TIMEOUT * 4, ec);
        printf("%d\n> Print_Eject: ", r); fflush(stdout);
        r = lp->Print_Eject(TIMEOUT, ec);
        printf("%d\n", r);
    }

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
