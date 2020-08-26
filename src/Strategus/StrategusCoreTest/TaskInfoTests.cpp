#include "CoreTests.h"
#include "../StrategusCore/TaskInfoFactory.h"

//Test TaskInfoFactory
bool test_createTaskInfo() {
	TaskInfoFactory tif(memMan);

	doubleID_t id = 0x10B00000B00B1E50;
	const char* name = "taskName";
	const char* command = "command";
	const char** inFiles = new const char*[2] { "inFile1", "inFile2" };
	const char** outFiles = new const char* [2] { "outFile1", "outFile2" };

	TaskInfo* ti = tif.createTaskInfo(id, name, command, 2, inFiles, 2, outFiles);

	if (ti->getTaskID() != 0xB00B1E50) {
		sprintf(expected, "%d", 0xB00B1E50);
		sprintf(returned, "%d", ti->getTaskID());
		printFail("createTaskInfo", "taskID");
		return false;
	}
	if (ti->getJobID() != 0x10B00000) {
		sprintf(expected, "%d", 0x10B00000);
		sprintf(returned, "%d", ti->getJobID());
		printFail("createTaskInfo", "jobID");
		return false;
	}
	if (strcmp(ti->getName(), "taskName") != 0) {
		sprintf(expected, "%s", "taskName");
		sprintf(returned, "%s", ti->getName());
		printFail("createTaskInfo", "taskName");
		return false;
	}
	if (strcmp(ti->getCommand(), "command") != 0) {
		sprintf(expected, "%s", "command");
		sprintf(returned, "%s", ti->getCommand());
		printFail("createTaskInfo", "command");
		return false;
	}
	if (ti->getInputFileCount() !=2) {
		sprintf(expected, "%d", 2);
		sprintf(returned, "%d", ti->getInputFileCount());
		printFail("createTaskInfo", "getInputFileCount");
		return false;
	}
	if (ti->getOutputFileCount() != 2) {
		sprintf(expected, "%d", 2);
		sprintf(returned, "%d", ti->getOutputFileCount());
		printFail("createTaskInfo", "getOutputFileCount");
		return false;
	}
	if (strcmp(ti->getInputFile(0), "inFile1") != 0) {
		sprintf(expected, "%s", "inFile1");
		sprintf(returned, "%s", ti->getInputFile(0));
		printFail("createTaskInfo", "inFile1");
		return false;
	}
	if (strcmp(ti->getInputFile(1), "inFile2") != 0) {
		sprintf(expected, "%s", "inFile2");
		sprintf(returned, "%s", ti->getInputFile(1));
		printFail("createTaskInfo", "inFile2");
		return false;
	}
	if (strcmp(ti->getOutputFile(0), "outFile1") != 0) {
		sprintf(expected, "%s", "outFile1");
		sprintf(returned, "%s", ti->getOutputFile(0));
		printFail("createTaskInfo", "outFile1");
		return false;
	}
	if (strcmp(ti->getOutputFile(1), "outFile2") != 0) {
		sprintf(expected, "%s", "outFile1");
		sprintf(returned, "%s", ti->getOutputFile(1));
		printFail("createTaskInfo", "outFile2");
		return false;
	}

	printSucc("createTaskInfo");
	return true;
}

//Test TaskInfoFactory
bool test_memcpyTaskInfo() {
	TaskInfoFactory tif(memMan);

	doubleID_t id = 0x10B00000B00B1E50;
	const char* name = "taskName";
	const char* command = "command";
	const char** inFiles = new const char* [2] { "inFile1", "inFile2" };
	const char** outFiles = new const char* [2] { "outFile1", "outFile2" };

	TaskInfo* tiOrig = tif.createTaskInfo(id, name, command, 2, inFiles, 2, outFiles);
	TaskInfo* ti = (TaskInfo*)(new uint8[tiOrig->getSize()]);
	memcpy(ti, tiOrig, tiOrig->getSize());

	if (ti->getTaskID() != 0xB00B1E50) {
		sprintf(expected, "%d", 0xB00B1E50);
		sprintf(returned, "%d", ti->getTaskID());
		printFail("memcpyTaskInfo", "taskID");
		return false;
	}
	if (ti->getJobID() != 0x10B00000) {
		sprintf(expected, "%d", 0x10B00000);
		sprintf(returned, "%d", ti->getJobID());
		printFail("memcpyTaskInfo", "jobID");
		return false;
	}
	if (strcmp(ti->getName(), "taskName") != 0) {
		sprintf(expected, "%s", "taskName");
		sprintf(returned, "%s", ti->getName());
		printFail("memcpyTaskInfo", "taskName");
		return false;
	}
	if (strcmp(ti->getCommand(), "command") != 0) {
		sprintf(expected, "%s", "command");
		sprintf(returned, "%s", ti->getCommand());
		printFail("memcpyTaskInfo", "command");
		return false;
	}
	if (ti->getInputFileCount() != 2) {
		sprintf(expected, "%d", 2);
		sprintf(returned, "%d", ti->getInputFileCount());
		printFail("memcpyTaskInfo", "getInputFileCount");
		return false;
	}
	if (ti->getOutputFileCount() != 2) {
		sprintf(expected, "%d", 2);
		sprintf(returned, "%d", ti->getOutputFileCount());
		printFail("memcpyTaskInfo", "getOutputFileCount");
		return false;
	}
	if (strcmp(ti->getInputFile(0), "inFile1") != 0) {
		sprintf(expected, "%s", "inFile1");
		sprintf(returned, "%s", ti->getInputFile(0));
		printFail("memcpyTaskInfo", "inFile1");
		return false;
	}
	if (strcmp(ti->getInputFile(1), "inFile2") != 0) {
		sprintf(expected, "%s", "inFile2");
		sprintf(returned, "%s", ti->getInputFile(1));
		printFail("memcpyTaskInfo", "inFile2");
		return false;
	}
	if (strcmp(ti->getOutputFile(0), "outFile1") != 0) {
		sprintf(expected, "%s", "outFile1");
		sprintf(returned, "%s", ti->getOutputFile(0));
		printFail("memcpyTaskInfo", "outFile1");
		return false;
	}
	if (strcmp(ti->getOutputFile(1), "outFile2") != 0) {
		sprintf(expected, "%s", "outFile1");
		sprintf(returned, "%s", ti->getOutputFile(1));
		printFail("memcpyTaskInfo", "outFile2");
		return false;
	}

	printSucc("memcpyTaskInfo");
	return true;
}

int runTaskInfoTests() {
	std::cout << "Starting tests for TaskInfo class.\n";
	int fails = 0;
	if (!test_createTaskInfo()) fails++;
	if (!test_memcpyTaskInfo()) fails++;

	std::cout << "Tests for class TaskInfo ended with " << fails << " fails.\n\n";

	return fails;
}