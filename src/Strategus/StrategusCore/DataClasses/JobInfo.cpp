#include "JobInfo.h"

ID_t JobInfo::getID() {
	return jobID;
}

ID_t JobInfo::getUserID() {
	return userID;
}

const char* JobInfo::getName() {
	return name;
}

uint16 JobInfo::getFileCount() {
	return fileCount;
}

const char* JobInfo::getFileName(uint16 index) {
	if (index >= fileCount)
		return nullptr;

	return files[index];
}

uint32 JobInfo::getSize() {
	return size;
}

void JobInfo::recalculatePointers() {
	char* ptr = (char*)this;
	ptr += sizeof(JobInfo);

	//For layout of data appended to the object se DataClassFactories.cpp

	//Name and command
	name = ptr;
	while (*ptr != 0) { ptr++; } ptr++;

	//Input files
	files = (char**)ptr;
	ptr += sizeof(char*) * fileCount;
	for (int i = 0; i < fileCount; i++) {
		files[i] = ptr;
		while (*ptr != 0) { ptr++; } ptr++;
	}
}