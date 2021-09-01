#include "CLithographPrinter.h"

#include <cstdio>
#include <thread>

void example(CLithographPrinter* lp, const char* type, const char* device)
{
    char ec[5];
    int  r, n;
#define TIMEOUT 30000

    r = lp->Print_Open((char*) type, (char*) device, ec);
    printf("> Print_Open: %d\n", r); fflush(stdout);
    if (r > 0)
        return ;

    Lithograph::LITHOGRAPHSTATUS ls;
    Lithograph::LPLITHOGRAPHSTATUS pls = &ls;

    for (int i = 0; i < 60; ++i) {
        const char* device;
        const char* media;
        const char* toner;

        ///
        /// Print_Status()
        r = lp->Print_Status(TIMEOUT, pls, ec);
        switch (ls.fwDevice) {
            case Lithograph::DEVICE_ONLINE: device = "ONLINE"; break;
            case Lithograph::DEVICE_BUSY: device = "BUSY"; break;
            case Lithograph::DEVICE_OFFLINE: device = "OFFLINE"; break;
            case Lithograph::DEVICE_HWERR: device = "HWERR"; break;
            default: device = "UNK";
        }
        switch (ls.fwMedia) {
            case Lithograph::NO_CARD: media = "NO_CARD"; break;
            case Lithograph::CARD_GATE: media = "GATE"; break;
            case Lithograph::CARD_INNER: media = "INNER"; break;
            case Lithograph::CARD_IC_POS: media = "IC_POS"; break;
            case Lithograph::CARD_RF_POS: media = "RF_POS"; break;
            case Lithograph::CARD_JAM: media = "JAM"; break;
            default: media = "UNK"; break;
        }
        switch (ls.fwToner) {
            case Lithograph::TONNER_FULL: toner = "FULL"; break;
            case Lithograph::TONNER_LOW: toner = "LOW"; break;
            case Lithograph::TONNER_OUT: toner = "OUT"; break;
            case Lithograph::TONNER_NOTSUPP: toner = "NOTSUPP"; break;
            default: toner = "UNK"; break;
        }

        ///
        /// Print_GetBoxStatus()
        Lithograph::LITHOGRAPHBOXUNIT boxesUnit[10];
        Lithograph::LITHOGRAPHBOXINFO boxesInfo = {10, boxesUnit};
        Lithograph::LPLITHOGRAPHBOXINFO lpBoxesInfo = &boxesInfo;

        n = lp->Print_GetBoxStatus(TIMEOUT, lpBoxesInfo, ec);

        ///
        /// Show result then sleep a while
        printf("r=%d device=%s media=%s toner=%s / n=%d boxesCount=%d\n",
               r, device, media, toner, n, boxesInfo.nCount);
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    r = lp->Print_Close(ec);
    printf("> Print_Close: %d\n", r); fflush(stdout);
}
