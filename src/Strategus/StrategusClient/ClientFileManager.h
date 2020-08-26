#pragma once
#include <pch.h>
#include "../StrategusCore/FileManager.h"

/// Class handling storage of job and task files and corresponding directories.
/// Functions throw errors when unable to create required directories.
class ClientFileManager : FileManager {
public:
	/// Constructor. rootDirectory is the directory in which data is stored.
	ClientFileManager(std::string rootDirectory);

	/// Returns the directory for job files.
	/// If that directory doesn't exist, it's created.
	std::string getJobDirectory(ID_t jobID);

	/// Returns the directory for task files.
	/// If that directory doesn't exist, it's created.
	std::string getTaskDirectory(doubleID_t taskID);

	/// Removes data associated with a task
	void removeTaskData(doubleID_t taskID);

	/// Removes data associated with a job and all its tasks.
	void removeJobData(ID_t jobID);
private:
	//Root directory
	std::string rootDir;

	//Directory for storing job data
	std::string dataDir;
};

