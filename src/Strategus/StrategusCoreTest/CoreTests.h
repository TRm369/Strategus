#pragma once
#include <iostream>
#include "DummyMemoryManager.h"

//Global vars
extern IMemoryManager* memMan;
extern char* expected;
extern char* returned;

//Test components
int runTaskInfoTests();
int runPacketTests();

//Test funcs
bool testUint64(const char* name, const char* sub, uint64 exp, uint64 value);
bool testString(const char* name, const char* sub, const char* exp, const char* value);

//Output funcs
void printSucc(const char* name);
void printFail(const char* name, const char* sub);