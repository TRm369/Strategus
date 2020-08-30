#include "Utils.h"
#include <stdexcept>
#include <ctime>
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

uint32 Utils::getUTCtime() {
    time_t timer;
    struct tm y2k20 = { 0 };
    uint32 seconds;

    //00:00, 1/1/2020
    y2k20.tm_hour = 0;   y2k20.tm_min = 0; y2k20.tm_sec = 0;
    y2k20.tm_year = 120; y2k20.tm_mon = 0; y2k20.tm_mday = 1;

    //Current UTC time
    gmtime_s(NULL, &timer);

    //Difference
    //difftime returns as double. Resolution below 1 second is not needed and because
    //the warranty of this code DOES NOT extend beyond the year 2120, uint32 is large enough.
    seconds = (uint32)difftime(timer, mktime(&y2k20));

    return seconds;
}
