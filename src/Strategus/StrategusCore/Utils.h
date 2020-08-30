#pragma once
#include "Definitions.h"
#include <string>

/// Provides various utility functions.
class Utils {
public:
	/// Converts flag represented as a string (only up to first 8 characters are considered) to a uint64 value.
	static uint64 flagToUint64(const char* flag);

	/// Replaces all occurances of toReplace in str with replaceBy.
	static void replaceAll(std::string& str, const std::string& toReplace, const std::string& replaceBy);

	/// Sleeps current thread.
	static void sleep(uint32 miliseconds);

	/// Returns current UTC time as the number of seconds since 00:00 hours, Jan 1, 2020 UTC
	static uint32 getUTCtime();
};

