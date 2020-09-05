#include "pch.h"
#include "ServerFileManager.h"

namespace fs = std::filesystem;

#define SEPARATOR ((char)fs::path::preferred_separator)

std::string ServerFileManager::statFilePath(ID_t jobID, std::string& dir) {
	//Make sure the directory exists
	if (!fs::exists(dir))
		if (!fs::create_directory(dir))
			throw std::exception("Can't create directory to save Scheduler.");

	return dir + SEPARATOR + std::to_string(jobID) + ".stat";
}

std::string ServerFileManager::schedulerDescFilePath(std::string& dir) {
	//Make sure the directory exists
	if (!fs::exists(dir))
		if (!fs::create_directory(dir))
			throw std::exception("Can't create directory to save Scheduler.");

	return dir + SEPARATOR + "scheduler.desc";
}

std::string ServerFileManager::getTaskOutputFile(const std::string& jobDir, TaskInfo* ti, uint16 fileIndex) {
	std::string dirPath = jobDir + SEPARATOR + std::to_string(ti->getTaskID());
	//Make sure the directory exists
	if (!fs::exists(dirPath))
		if (!fs::create_directories(dirPath))
			throw std::exception("Can't create directory to save taskFile.");

	return dirPath + SEPARATOR + ti->getOutputFile(fileIndex);
}
