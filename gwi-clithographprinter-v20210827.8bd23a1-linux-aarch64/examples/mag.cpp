#include "CLithographPrinter.h"

#include <cstdio>
#include <cstring>

void example(CLithographPrinter* lp, const char* type, const char* device)
{
    char ec[5];
    int  r;
#define TIMEOUT 30000

    r = lp->Print_Open((char*) type, (char*) device, ec);
    printf("> Print_Open: %d\n", r); fflush(stdout);
    if (r > 0)
        return ;

    Lithograph::LITHOGRAPHMAGINFO tracks;

    printf("> Please insert a card...\n"); fflush(stdout);

    r = lp->Print_EnableCard(TIMEOUT, 0, ec);
    printf("> Print_WriteTrack: "); fflush(stdout);
    memset(&tracks, 0, sizeof(tracks));
    tracks.wTrack1Method = tracks.wTrack2Method = tracks.wTrack3Method = 0x0004;
    strcpy((char*) tracks.track1_data, "0123456789AZERTYUIOPMLKJHGFDSQWXCVBN12345678912345123456789012345ABKIYTRLM");
    strcpy((char*) tracks.track2_data, "1234567891234567891234567891234567891");
    strcpy((char*) tracks.track3_data, "1234567891234567891234567891234567891234567891231234567891234567891234567891234567891234567891231234");
    tracks.nTrack1Len = strlen((char*) tracks.track1_data);
    tracks.nTrack2Len = strlen((char*) tracks.track2_data);
    tracks.nTrack3Len = strlen((char*) tracks.track3_data);
    r = lp->Print_WriteTrack(TIMEOUT, 0x000E, &tracks, ec);
    printf("%d\n", r); fflush(stdout);
    if (r > 0)
        return ;

    printf("> Print_ReadTrack: "); fflush(stdout);
    memset(&tracks, 0, sizeof(tracks));
    tracks.wTrack1Method = tracks.wTrack2Method = tracks.wTrack3Method = 0x0004;
    r = lp->Print_ReadTrack(TIMEOUT, 0x000E, &tracks, ec);
    printf("%d\n", r); fflush(stdout);

    printf("> Print_Eject: "); fflush(stdout);
    r = lp->Print_Eject(TIMEOUT, ec);
    printf("%d\n", r); fflush(stdout);

    lp->Print_Close(ec);
}
