#pragma once
#include "pch.h"
#include <boost/property_tree/ptree.hpp>
#include "../StrategusCore/DataClassFactories.h"
#include "../StrategusCore/MemoryManager/IMemoryManager.h"
#include "UserManager/IUserManager.h"

//Type for representing the status of a task.
typedef uint32 taskStatus_t;
//Unless it's one of the special values, represents time by which the task has to be completed.

//Task ready to be assigned
#define STATUS_UNASSIGNED 0
//Task completed
#define STATUS_COMPLETE 1

/// Scheduler type for storing job info.
class Job {
public:
	/// Creates a new Job from its descriptor file and assigns it an ID.
	/// <param name="descFile">Path to descriptor file.</param>
	/// <param name="id">ID of the Job.</param>
	/// <param name="memoryManager">Memory manager to use for storing data.</param>
	/// <param name="userManager">User manager to use for username translation.</param>
	Job(const char* descFile, ID_t id, IMemoryManager* memoryManager, IUserManager* userManager);

	/// Loads an already existing Job from its descriptor and status file.
	/// <param name="descFile">Path to descriptor file.</param>
	/// <param name="statusFile">Path to status file.</param>
	/// <param name="id">ID of the Job.</param>
	/// <param name="memoryManager">Memory manager to use for storing data.</param>
	/// <param name="userManager">User manager to use for username translation.</param>
	Job(const char* descFile, const char* statusFile, ID_t id, IMemoryManager* memoryManager, IUserManager* userManager);

	/// Returns job information.
	JobInfo* getJobInfo();

	/// Checks all tasks for expired deadlines.
	bool update();

	/// Returns true if all tasks are completed.
	bool isComplete();

	/// Sets task's status.
	void setTaskStatus(ID_t task, taskStatus_t status);

	/// Returns the number of tasks in this Job.
	size_t getTaskCount();

	/// Returns a task by id. If the id is invalid, returns nullptr.
	TaskInfo* getTaskByID(ID_t id);

	/// Returns task's status. If the id is invalid, returns STATUS_COMPLETE.
	taskStatus_t getTaskStatus(ID_t id);

	/// Returns a pointer to a task that hasn't been yet assigned and flags it as assigned.
	TaskInfo* getUnassignedTask();

	/// Returns path to a directory to use for saving finished task's data.
	const std::string& getOutputDirectory();

	/// Saves the status of this Job to a file.
	void saveStatus(const char* file);

	~Job();

private:
	//Contents of descriptor file (except for existing ID).
	JobInfo* jobInfo;
	size_t taskCount;
	TaskInfo** tasks;
	std::string outputDirectory;

	//Contents of status file (plus existing ID).
	taskStatus_t* taskStatus;

	IMemoryManager* memMan;
	JobInfoFactory jif;
	TaskInfoFactory tif;

	/// Time clients have to finish tasks in seconds.
	uint32 taskLength = 24 * 3600;

	/// Loads data from descriptor file. Creates the jobInfo instance.
	void readDescriptorFile(const char* descriptorFile, ID_t jobID, IUserManager* userManager);

	/// Reads TaskInfo from a ptree node.
	TaskInfo* readTaskInfo(boost::property_tree::ptree& node, ID_t taskId);

	/// Loads data from status file. Returns loaded job ID.
	void readStatusFile(const char* statusFile);
};

