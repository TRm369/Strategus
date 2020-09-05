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

	/// Prints the beggining of a message entry. Use operator << to append data to the message and endEntry() to append it to the log.
	/// If there is a previous entry that wasn't ended, it's ended by this function.
	static void startMessage();

	/// Prints the beggining of a warning entry. Use operator << to append data to the message and endEntry() to append it to the log.
	/// If there is a previous entry that wasn't ended, it's ended by this function.
	static void startWarning();

	/// Prints the beggining of a error entry. Use operator << to append data to the message and endEntry() to append it to the log.
	/// If there is a previous entry that wasn't ended, it's ended by this function.
	static void startError();

	/// Appends argument to message started by start...() method.
	template<typename T>
	Log& operator<<(T message);

	/// Appends argument to message started by start...() method.
	template<typename T>
	static void append(T message);

	/// Ends the entry started by start...() method and writes it to the log.
	static void endEntry();

	///Close the log
	static void close();

private:
	static bool isOpen;
	static std::ofstream file;

	static std::string entry;

	static void printTimestamp();
};

template<typename T>
inline Log& Log::operator<<(T message) {
	entry += message;
	return *this;
}

template<typename T>
inline void Log::append(T message) {
	entry += message;
}
