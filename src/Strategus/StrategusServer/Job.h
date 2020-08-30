#pragma once
#include "../StrategusCore/DataClasses/TaskInfo.h"

//Type for representing the status of a task.
typedef uint32 taskStatus_t;
//Unless it's one of the special values, represents time by which the task has to be completed.

//Task ready to be assigned
#define STATUS_NEW 0
//Task completed
#define STATUS_COMPLETE 1

/// Scheduler type for storing job info.
class Job {
public:
	/// Creates a new Job from its descriptor file and assigns it an ID.
	Job(const char* descFile, ID_t id);

	/// Loads an already existing Job from its descriptor and status file.
	Job(const char* descFile, const char* statusFile);

	/// Checks all tasks for expired deadlines.
	bool update();

	/// Returns true if all tasks are completed.
	bool isComplete();

	/// Sets task's status.
	void setTaskStatus(ID_t task, taskStatus_t status);

	/// Returns a pointer to a task that hasn't been yet assigned and flags it as assigned.
	TaskInfo* getUnassignedTask();

	/// Saves the status of this Job to a file.
	void saveStatus(const char* file);

private:
	
	size_t taskCount;
	taskStatus_t* taskStatus;
	TaskInfo** tasks;
};

