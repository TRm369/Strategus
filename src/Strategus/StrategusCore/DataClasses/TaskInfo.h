#pragma once
#include "../Definitions.h"

/// Description of a task shared between compute node and scheduler.
class TaskInfo {
	friend class TaskInfoFactory;
public:
	///Returns ID of this task within its job.
	ID_t getTaskID();

	///Returns ID of the job this task belongs to.
	ID_t getJobID();

	/// Returns the full ID of the task.
	doubleID_t getFullID();

	///Returns task's name.
	const char* getName();

	///Returns unformatted command.
	const char* getCommand();

	///Returns the number of input files for this task.
	uint16 getInputFileCount();

	///Returns the number of output files for this task.
	uint16 getOutputFileCount();

	/**Returns the name of specified input file.
	* 
	* For invalid index returns nullptr.
	*/
	const char* getInputFile(uint16 fileIndex);

	/**Returns the name of specified output file.
	*
	* For invalid index returns nullptr.
	*/
	const char* getOutputFile(uint16 fileIndex);

	///Returns the size of this object (including all data appended to it).
	uint32 getSize();

	/// Recalculates all pointers after the object has been moved to a different address in memory (eg. after a network transmit).
	void recalculatePointers();
	
	/// Creates a doubleID from separate job and task ID.
	static doubleID_t IDfromJobAndTask(ID_t jobID, ID_t taskID);

private:
	//Shouldn't be used. Use TaskInfoFactory instead.
	TaskInfo();
	~TaskInfo();
	void* operator new(size_t size);
	void* operator new[](size_t size);
	void operator delete(void* ptr);
	void operator delete[](void* ptr);

	//Task (first ID) and job (second ID) IDs.
	doubleID_t ID;
	char* name;
	char* command;
	uint16 inputFileCount;
	uint16 outputFileCount;
	char** inputFiles;
	char** outputFiles;

	//Size of this instance (including all data appended to it)
	uint32 size;
};