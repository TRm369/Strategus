#define _CRT_SECURE_NO_WARNINGS
#include "CoreTests.h"
#include "../StrategusCore/Log.h"

IMemoryManager* memMan = new DummyMemoryManager();
char* expected = new char[1024];
char* returned = new char[1024];

void printSucc(const char* name) {
    std::cout << "    Test " << name << " succeded.\n";
    Log::logMessage("    Test succeded.");
}
void printFail(const char* name, const char* sub) {
    std::cout << "    Test " << name << " failed (sub-test " << sub << ": expected " << expected << ", returned " << returned << ").\n";
    Log::logError("    Test failed");
}

bool testUint64(const char* name, const char* sub, uint64 exp, uint64 value) {
    if (value != exp) {
        sprintf(expected, "%u", exp);
        sprintf(returned, "%u", value);
        printFail(name, sub);
        return false;
    }
    return true;
}

bool testString(const char* name, const char* sub, const char* exp, const char* value) {
    if (strcmp(exp, value) != 0) {
        sprintf(expected, "%s", exp);
        sprintf(returned, "%s", value);
        printFail(name, sub);
        return false;
    }
    return true;
}

int main()
{
    Log::logMessage("Beginning tests.");
    std::cout << "StrategusCore tests\n\n";
    int fails = 0;
    fails += runTaskInfoTests();
    fails += runPacketTests();

    std::cout << "Tests finished with " << fails << " fails.";
    Log::close();
}