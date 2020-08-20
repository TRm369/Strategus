#define _CRT_SECURE_NO_WARNINGS
#include "CoreTests.h"

IMemoryManager* memMan = new DummyMemoryManager();
char* expected = new char[1024];
char* returned = new char[1024];

void printSucc(const char* name) {
    std::cout << "    Test " << name << " succeded.\n";
}
void printFail(const char* name, const char* sub) {
    std::cout << "    Test " << name << " failed (sub-test " << sub << ": expected " << expected << ", returned " << returned << ").\n";
}

int main()
{
    std::cout << "StrategusCore tests\n\n";
    int fails = 0;
    fails += runTaskInfoTests();

    std::cout << "Tests finished with " << fails << " fails.";
}