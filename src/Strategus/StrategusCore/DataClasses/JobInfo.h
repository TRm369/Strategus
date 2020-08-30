#pragma once
#include "../Definitions.h"

/// Description of a job shared between compute node and scheduler.
class JobInfo {
	friend class JobInfoFactory;
public:
	/// Returns the job's ID.
	ID_t getID();

	/// Returns the ID of this job's owner.
	ID_t getUserID();

	/// Returns the job's name.
	const char* getName();

	/// Return the count of files associated with this job.
	uint16 getFileCount();

	/// Returns the name of a file associated with this job. For invalid index returns nullptr.
	const char* getFileName(uint16 index);

	///Returns the size of this object (including all data appended to it).
	uint32 getSize();

	/// Recalculates all pointers after the object has been moved to a different address in memory (eg. after a network transmit).
	void recalculatePointers();

private:
	//Shouldn't be used. Use JobInfoFactory instead.
	JobInfo();
	~JobInfo();
	void* operator new(size_t size);
	void* operator new[](size_t size);
	void operator delete(void* ptr);
	void operator delete[](void* ptr);

	ID_t jobID;
	ID_t userID;
	const char* name;
	
	//Info about files associated with the job.
	char** files;
	uint16 fileCount;

	//Size of this instance (including all data appended to it)
	uint32 size;
};

