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

	/// Variadic log message
	template<typename... Ts>
	static void logMessage(Ts... args);
	
	///Log warning
	static void logWarning(const char* warning);

	///Log error
	static void logError(const char* error);

	/// Variadic log error
	template<typename... Ts>
	static void logError(Ts... args);

	/// Appends argument to message started by start...() method.
	template<typename T>
	Log& operator<<(T message);

	/// Appends argument to message started by start...() method.
	template<typename T>
	static void append(T message);

	/// Prints all data appended after last end...() call as message.
	static void endMessage();

	/// Prints all data appended after last end...() call as message.
	static void endWarning();

	/// Prints all data appended after last end...() call as message.
	static void endError();

	///Close the log
	static void close();

private:
	static bool isOpen;
	static std::ofstream file;

	static std::string entry;

	static void printTimestamp();

	template<typename... Ts> static void pass(Ts... args) {};
	template<typename T> static bool prepend(T arg);
};

template<typename ...Ts>
inline void Log::logMessage(Ts ...args) {
	entry.clear();
	pass(prepend(args)...);
	endMessage();
}

template<typename ...Ts>
inline void Log::logError(Ts ...args) {
	entry.clear();
	pass(prepend(args)...);
	endError();
}

template<typename T>
inline Log& Log::operator<<(T message) {
	entry += message;
	return *this;
}

template<typename T>
inline void Log::append(T message) {
	entry += message;
}

template<typename T>
inline bool Log::prepend(T arg) {
	entry = arg + entry;
	return true;
}
