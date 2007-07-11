#ifndef __DEFINES_H
#define __DEFINES_H

#define APPTITLE "The Outcast"
#define PI 3.14159
#ifndef min
    #define min(a,b) (a > b ? b : a)
    #define max(a,b) (a < b ? b : a)
#endif
#define FINGERPRINT_TIBIADAT 0
#define FINGERPRINT_TIBIASPR 1
#define FINGERPRINT_TIBIAPIC 2


// FIXME (Khaos#5#) Perhaps this one should move to "utils.h" or sth ... but it'd suck if it'd be there alone. So find some more functions to go there
#include <stdio.h>
inline bool fileexists(const char* filename) {
    FILE *f;
    if ((f = fopen(filename, "r"))) {
        fclose(f);
        return true;
    } else
        return false;
}

#endif
