#pragma once
#include "pch.h"
#include "Job.h"
#include "ServerFileManager.h"

#include "../StrategusCore/MemoryManager/DummyMemoryManager.h"
#include "UserManager/DummyUserManager.h"


// Job and path to its descFile.
typedef std::pair<Job*, std::string> JobEntry;

/// Class responsible for managing Jobs and scheduling tasks.
class Scheduler {
public:
	/// Creates an empty scheduler.
	Scheduler();

	/// Creates a scheduler and loads Jobs from a directory.
	Scheduler(std::string directory);

	/// Loads a new job.
	/// <param name="descFile">Path to the Job descriptor file.</param>
	/// <param name="jobID">If sucessful, is set to the job's ID.</param>
	/// <returns>True iff sucessfully loaded.</returns>
	bool loadNewJob(std::string descFile, ID_t& jobID);

	/// Loads a Job with its status file.
	/// <param name="descFile">Path to the Job descriptor file.</param>
	/// <param name="statusFile">Path to the Job status file.</param>
	/// <param name="jobID">If sucessful, is set to the job's ID.</param>
	/// <returns>True iff sucessfully loaded.</returns>
	bool loadExistingJob(std::string descFile, std::string statusFile, ID_t& jobID);

	///  Removes a Job.
	/// <returns>True iff sucessful.</returns>
	bool removeJob(ID_t jobID);

	/// Requests a task for a client (compute node).
	/// <param name="clientInfo">Client's ClientInfo object.</param>
	/// <returns>Pointer to a TaskInfo object if sucessful, nullptr otherwise.</returns>
	TaskInfo* requestTask(ClientInfo* clientInfo);

	/// Reports a finished task (including file transfer).
	/// <param name="task">Pointer to TaskInfo of the finished task.</param>
	/// <param name="client">Clietn that finished the task.</param>
	void taskFinished(TaskInfo* task, ClientInfo* client);

	/// Returns true iff all Jobs assigned to this scheduler are complete.
	bool allJobsComplete();

	/// Returns the path to a output file of a task. On fail returns empty string.
	/// <param name="ti">Pointer to a TaskInfo instance.</param>
	/// <param name="fileIndex">Index of the file to use.</param>
	std::string getTaskOutputFile(TaskInfo* ti, uint16 fileIndex);

	/// Saves the status of the scheduler and its Jobs.
	/// <param name="directory">Path to a directory to save the data into.</param>
	/// <returns>True iff sucessful.</returns>
	bool saveStatus(std::string directory);

private:
	std::vector<JobEntry> jobs;
	DummyMemoryManager memMan;
	DummyUserManager userMan;
	ServerFileManager sfm;

	ID_t nextID;
};