#pragma once
#include <fstream>

///Class for logging information.
/// 
/// Used as a static class, automatically initializes on first call to log.
/// close() should be called before exitting to properly release file handle.
class Log {
public:
	///Can be used to manually open the log file. Allows to specify the log file name.
	static bool open(const char* filename);

	///Log message
	static void logMessage(const char* message);
	
	///Log warning
	static void logWarning(const char* warning);

	///Log error
	static void logError(const char* error);

	///Close the log
	static void close();

private:
	static bool isOpen;
	static std::ofstream file;

	static void printTimestamp();
};