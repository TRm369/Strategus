#include <stdexcept>
#include "pch.h"
#include "ClientFileManager.h"

#define DATA_SUBDIR "/data"

ClientFileManager::ClientFileManager(std::string rootDirectory) {
	rootDir = rootDirectory;

	//If the root directory doesn't exist, create it
	if (!directoryExists(rootDir))
		if (!createDirectory(rootDir))
			//It can't be created
			throw std::invalid_argument("Root directory for ClinetFileManager doesn't exist and can't be created.");

	//Create data directory
	dataDir = rootDir + DATA_SUBDIR;
	if (!directoryExists(dataDir))
		if (!createDirectory(dataDir))
			//It can't be created
			throw std::invalid_argument("Data directory for ClinetFileManager doesn't exist and can't be created.");

}

std::string ClientFileManager::getJobDirectory(ID_t jobID) {
	std::string dir = dataDir + "/" + std::to_string(jobID);
	if (!directoryExists(dir))
		if (!createDirectory(dir))
			throw std::exception("Can't create directory for a job.");

	return dir;
}

std::string ClientFileManager::getTaskDirectory(doubleID_t taskID) {
	std::string dir = getJobDirectory(SECOND_ID(taskID)) + "/" + std::to_string(FIRST_ID(taskID));

	if (!directoryExists(dir))
		if (!createDirectory(dir))
			throw std::exception("Can't create directory for a task.");

	return dir;
}

void ClientFileManager::removeTaskData(doubleID_t taskID) {
	std::string dir = getTaskDirectory(taskID);
	remove(dir);
}

void ClientFileManager::removeJobData(ID_t jobID) {
	std::string dir = getJobDirectory(jobID);
	remove(dir);
}
