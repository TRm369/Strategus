#pragma once
#include "Definitions.h"
#include <string>
class Utils {
public:
	static uint64 flagToUint64(const char* flag);
	static void replaceAll(std::string& str, const std::string& toReplace, const std::string& replaceBy);
	static void sleep(uint32 miliseconds);
};

