#pragma once
#include "DataClasses/TaskInfo.h"
#include "DataClasses/JobInfo.h"
#include "DataClasses/ClientInfo.h"
#include "MemoryManager/IMemoryManager.h"

/// Factory class for TaskInfo objects.
class TaskInfoFactory {
public:
	TaskInfoFactory(IMemoryManager* memManager);

	/// <summary>
	/// Creates a TaskInfo object from given data. To delete the object use destroyTaskInfo().
	/// </summary>
	/// <param name="ID">IDs of the task (first) and the corresponding job (second)</param>
	/// <param name="name">Name of the task.</param>
	/// <param name="command">Unformatted command that should be executed.</param>
	/// <param name="inputFileCount">Number of input files.</param>
	/// <param name="inputFiles">Array of input file names.</param>
	/// <param name="outputFileCount">Number of output files.</param>
	/// <param name="outputFiles">Array of output file names.</param>
	TaskInfo* createTaskInfo(doubleID_t ID, const char* name, const char* command, uint16 inputFileCount, const char** inputFiles,
		                     uint16 outputFileCount, const char** outputFiles);

	///Destroys an instance of TaskInfo previously created by this Factory.
	void destroyTaskInfo(TaskInfo* ti);

private:
	IMemoryManager* memManager;
};

/// Factory class for JobInfo objects.
class JobInfoFactory {
public:
	JobInfoFactory(IMemoryManager* memManager);

	/// <summary>
	/// Creates a JobInfo object from given data. To delete the object use destroyJobInfo().
	/// </summary>
	/// <param name="ID">ID of the job.</param>
	/// <param name="userID">ID of the job's owner.</param>
	/// <param name="name">Name of the job.</param>
	/// <param name="fileCount">Number of files associated with the job.</param>
	/// <param name="files">Names of files associated with the job.</param>
	JobInfo* createJobInfo(ID_t ID, ID_t userID, const char* name, uint16 fileCount, const char** files);

	///Destroys an instance of JobInfo previously created by this Factory.
	void destroyJobInfo(JobInfo* ji);

private:
	IMemoryManager* memManager;
};

/// Factory class for ClientInfo objects.
class ClientInfoFactory {
public:
	ClientInfoFactory(IMemoryManager* memManager);

	/// <summary>
	/// Creates a ClientInfo object from given data. To delete the object use destroyClientInfo().
	/// </summary>
	/// <param name="flagCount">Number of client's flags.</param>
	/// <param name="flags">Client's flags.</param>
	ClientInfo* createClientInfo(ID_t ID, uint16 flagCount, uint64* flags);

	///Destroys an instance of JobInfo previously created by this Factory.
	void destroyClientInfo(ClientInfo* ci);

private:
	IMemoryManager* memManager;
};