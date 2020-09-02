#pragma once
#include <pch.h>
#include "../StrategusCore/DataClasses/TaskInfo.h"
#include "../StrategusCore/DataClasses/ClientInfo.h"
#include "ClientFileManager.h"

///Interface representing connection to the scheduling server
class IClientConnection {
public:
	/// Connects to the specified server.
	/// <param name="hostname">Hostname of the server.</param>
	/// <returns>Returns true if successful.</returns>
	virtual bool connect(std::string& hostname) = 0;

	/// Requests a task from the scheduler.
	/// 
	/// This function is also responsible for retrieving all necessary files,
	/// meaning once it returns the TaskInfo pointer, all of it's input files must already be in place ready for execution.
	/// <param name="ci">Description of this client.</param>
	/// <param name="fileMan">ClientFileManager used to find directory to save task files.</param>
	/// <returns>Pointer to a TaskInfo object, nullptr on fail.</returns>
	virtual TaskInfo* requestTask(ClientInfo* ci, ClientFileManager& fileMan) = 0;

	/// Reports a finished task to the scheduler.
	/// 
	/// This function is also responsible for submitting all the output files for the task.
	/// <param name="task">TaskInfo of task that was completed.</param>
	/// <param name="fileMan">ClientFileManager used to find directory with files to report.</param>
	/// <returns>Returns true if task was submitted successfully.</returns>
	virtual bool reportTaskFinished(TaskInfo* task, ClientFileManager& fileMan) = 0;
};