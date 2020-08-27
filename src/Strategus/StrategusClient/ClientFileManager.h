#pragma once
#include <pch.h>
#include "../StrategusCore/FileManager.h"

/// Class handling storage of job and task files and corresponding directories.
/// Functions throw errors when unable to create required directories.
/// Functions returning paths use system's preffered seperator ('\' for win, '/' for linux)
class ClientFileManager : public FileManager {
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

	/// Checks if a file exists in task's data.
	/// <param name="taskID">ID of the task</param>
	/// <param name="fileName">Name of the file (without directories).</param>
	/// <returns>True iff the file exists.</returns>
	bool checkTaskFile(doubleID_t taskID, const std::string& fileName);

private:
	//Root directory
	std::string rootDir;

	//Directory for storing job data
	std::string dataDir;
};

