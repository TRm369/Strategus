#include "TaskInfo.h"

ID_t TaskInfo::getTaskID() {
	return FIRST_ID(ID);
}

ID_t TaskInfo::getJobID() {
	return SECOND_ID(ID);
}

doubleID_t TaskInfo::getFullID() {
	return ID;
}

const char* TaskInfo::getName() {
	return name;
}

const char* TaskInfo::getCommand() {
	return command;
}

uint16 TaskInfo::getInputFileCount() {
	return inputFileCount;
}

uint16 TaskInfo::getOutputFileCount() {
	return outputFileCount;
}

const char* TaskInfo::getInputFile(uint16 fileIndex) {
	if (fileIndex >= inputFileCount)
		return nullptr;

	return inputFiles[fileIndex];
}

const char* TaskInfo::getOutputFile(uint16 fileIndex) {
	if (fileIndex >= outputFileCount)
		return nullptr;

	return outputFiles[fileIndex];
}

uint32 TaskInfo::getSize() {
	return size;
}

void TaskInfo::recalculatePointers() {
	char* ptr = (char*)this;
	ptr += sizeof(TaskInfo);

	//For layout of data appended to the object se DataClassFactories.cpp

	//Name and command
	name = ptr;
	while (*ptr != 0) { ptr++; } ptr++;
	command = ptr;
	while (*ptr != 0) { ptr++; } ptr++;

	//Input files
	inputFiles = (char**)ptr;
	ptr += sizeof(char*) * inputFileCount;
	for (int i = 0; i < inputFileCount; i++) {
		inputFiles[i] = ptr;
		while (*ptr != 0) { ptr++; } ptr++;
	}

	//Output files
	outputFiles = (char**)ptr;
	ptr += sizeof(char*) * outputFileCount;
	for (int i = 0; i < outputFileCount; i++) {
		outputFiles[i] = ptr;
		while (*ptr != 0) { ptr++; } ptr++;
	}
}

doubleID_t TaskInfo::IDfromJobAndTask(ID_t jobID, ID_t taskID) {
	return ((doubleID_t)jobID << 32) + taskID;
}

