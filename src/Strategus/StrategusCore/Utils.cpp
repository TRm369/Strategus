#include "Utils.h"
#include <stdexcept>
#ifdef OS_WINDOWS
#include <Windows.h>
#endif

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

void Utils::replaceAll(std::string& str, const std::string& toReplace, const std::string& replaceBy) {
    size_t pos = str.find(toReplace);
    while (pos != std::string::npos) {
        str.replace(pos, toReplace.length(), replaceBy);
        pos = str.find(toReplace);
    }
}

void Utils::sleep(uint32 miliseconds) {
#ifdef OS_WINDOWS
    Sleep(miliseconds);
#else
    throw std::exception("Sleep not implemented.");
#endif
}
