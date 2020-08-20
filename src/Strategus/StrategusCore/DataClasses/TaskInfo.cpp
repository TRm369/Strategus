#include "TaskInfo.h"

ID_t TaskInfo::getTaskID() {
	return FIRST_ID(ID);
}

ID_t TaskInfo::getJobID() {
	return SECOND_ID(ID);
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