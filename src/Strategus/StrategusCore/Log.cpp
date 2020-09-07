#include "Log.h"
#include <time.h>

#define DEFAULT_FILENAME "strategus.log"

#define MESSAGE "         "
#define WARNING "warning: "
#define ERROR "ERROR:   "

bool Log::isOpen = false;
std::ofstream Log::file;
std::string Log::entry;

bool Log::open(const char* filename) {
	if (file.is_open())
		return false;

	file.open(filename);
	isOpen = true;

	file << "==========  STRATEGUS LOG FILE ==========\n";
	printTimestamp();
	file << "Log opened\n";
}

void Log::logMessage(const char* message) {
	if (!isOpen)
		open(DEFAULT_FILENAME);

	printTimestamp();
	file << MESSAGE << message << "\n";
}

void Log::logWarning(const char* warning) {
	if (!isOpen)
		open(DEFAULT_FILENAME);

	printTimestamp();
	file << WARNING << warning << "\n";
}

void Log::logError(const char* error) {
	if (!isOpen)
		open(DEFAULT_FILENAME);

	printTimestamp();
	file << ERROR << error << "\n";
}

void Log::endMessage() {
	if (entry.empty())
		return;
	if (!isOpen)
		open(DEFAULT_FILENAME);

	printTimestamp();
	file << MESSAGE << entry << "\n";
	entry.clear();
	file.flush();
}

void Log::endWarning() {
	if (entry.empty())
		return;
	if (!isOpen)
		open(DEFAULT_FILENAME);

	printTimestamp();
	file << WARNING << entry << "\n";
	entry.clear();
	file.flush();
}

void Log::endError() {
	if (entry.empty())
		return;
	if (!isOpen)
		open(DEFAULT_FILENAME);

	printTimestamp();
	file << ERROR << entry << "\n";
	entry.clear();
	file.flush();
}

void Log::close() {
	printTimestamp();
	file << "Closing log.\n";
	file.flush();
	file.close();
	isOpen = false;
}

void Log::printTimestamp() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	//tstruct = *localtime(&now);
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	file << "[" << buf << "] ";
}