#pragma once
#include <pch.h>
#include "ClientFileManager.h"
#include "../StrategusCore/DataClasses/TaskInfo.h"
#include "Process/IProcess.h"

/// Class representing a task running on the client.
class Task {
public:
	/// Creates Task from TaskInfo.
	/// <param name="ti">TaskInfo object describing the task.</param>
	/// <param name="cfm">ClientFileManager used to resolve names.</param>
	Task(TaskInfo* ti, ClientFileManager& cfm);

	/// Starts execution of the task.
	/// <returns>True if task started successfully.</returns>
	bool start();

	/// Returns whether the task has finished execution.
	bool isFinished();

	/// Returns true if all output files are present.
	bool verifyOutputFiles();

	~Task();

private:
	//ClientFileManager for name resolving and file checking
	ClientFileManager& cfm;

	//Description of the task
	TaskInfo* taskInfo;
	//Directory with task data
	std::string taskDir;
	//Directory with job data
	std::string jobDir;
	//Task command with resolved names
	std::string resCommand;

	//Process running the task
	IProcess* proc;

	/// Takes value in resCommand as a command without resolved names and resolves them.
	void resolveCommand();
};

