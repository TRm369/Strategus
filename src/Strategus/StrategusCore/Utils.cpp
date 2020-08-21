#include "Utils.h"

uint64 Utils::flagToUint64(const char* flag) {
    uint64 flagUint = 0;
    for (int i = 0; i < 8; i++) {
        //Check if we're at the end of the string
        if (flag[i] == 0)
            break;

        //Convert character to corresponding value
        flagUint += (uint64)flag[i] << (8 * i);
    }
    return flagUint;
}
