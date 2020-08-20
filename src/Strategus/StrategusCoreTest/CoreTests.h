#pragma once
#include <iostream>
#include "DummyMemoryManager.h"

//Global vars
extern IMemoryManager* memMan;
extern char* expected;
extern char* returned;

//Test components
int runTaskInfoTests();

//Output funcs
void printSucc(const char* name);
void printFail(const char* name, const char* sub);