#pragma once
#include "DataClasses/TaskInfo.h"
#include "IMemoryManager.h"

class TaskInfoFactory {
public:
	TaskInfoFactory(IMemoryManager* memManager);

	/// <summary>
	/// Creates a TaskInfo object from given data.
	/// </summary>
	/// <param name="ID">IDs of the task (first) and the corresponding job (second)</param>
	/// <param name="name">Name of the task.</param>
	/// <param name="command">Unformatted command that should be executed.</param>
	/// <param name="inputFileCount">Number of input files.</param>
	/// <param name="inputFiles">Array of input file names.</param>
	/// <param name="outputFileCount">Number of output files.</param>
	/// <param name="outputFiles">Array of output file names.</param>
	/// <returns></returns>
	TaskInfo* createTaskInfo(doubleID_t ID, const char* name, const char* command, uint16 inputFileCount, const char** inputFiles,
		                     uint16 outputFileCount, const char** outputFiles);

	///Destroys an instance of TaskInfo previously created by this Factory.
	void destroyTaskInfo(TaskInfo* ti);

private:
	IMemoryManager* memManager;
};

