#pragma once
#include "pch.h"
#include "Job.h"

/// Class responsible for managing Jobs and scheduling tasks.
class Scheduler {
public:
	/// Creates an empty scheduler.
	Scheduler();

	/// Creates a scheduler and loads Jobs from decriptor file.
	Scheduler(const char* descFile);

	/// Loads a new job.
	/// <param name="descFile">Path to the Job descriptor file.</param>
	/// <param name="jobID">If sucessful, is set to the job's ID.</param>
	/// <returns>True iff sucessfully loaded.</returns>
	bool loadNewJob(const char* descFile, ID_t& jobID);

	/// Loads a Job with its status file.
	/// <param name="descFile">Path to the Job descriptor file.</param>
	/// <param name="statusFile">Path to the Job status file.</param>
	/// <param name="jobID">If sucessful, is set to the job's ID.</param>
	/// <returns>True iff sucessfully loaded.</returns>
	bool loadExistingJob(const char* descFile, const char* statusFile, ID_t& jobID);

	///  Removes a Job.
	/// <returns>True iff sucessful.</returns>
	bool removeJob(ID_t jobID);

	/// Requests a task for a client (compute node).
	/// <param name="clientInfo">Client's ClientInfo object.</param>
	/// <returns>Pointer to a TaskInfo object if sucessful, nullptr otherwise.</returns>
	TaskInfo* requestTask(ClientInfo* clientInfo);

	/// Reports a finished task.
	/// <param name="task"></param>
	/// <param name="client"></param>
	void taskFinished(TaskInfo* task, ClientInfo* client);
};