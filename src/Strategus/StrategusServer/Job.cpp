#include "pch.h"
#include "Job.h"
#include "../StrategusCore/Utils.h"

//TODO: task managing in a more intelligent way.

namespace pt = boost::property_tree;

Job::Job(const char* descFile, ID_t id, IMemoryManager* memoryManager, IUserManager* userManager)
	: tif(memoryManager), jif(memoryManager) {
	memMan = memoryManager;

	//Contents of descriptor file get loaded
	if (!readDescriptorFile(descFile, id, userManager))
		throw std::exception("Failed to create Job.");

	//Contents of status file get created
	taskStatus = (taskStatus_t*)memMan->allocateArray(taskCount, sizeof(taskStatus_t));
	for (size_t i = 0; i < taskCount; i++) {
		taskStatus[i] = STATUS_UNASSIGNED;
	}
}

Job::Job(const char* descFile, const char* statusFile, ID_t id, IMemoryManager* memoryManager, IUserManager* userManager)
	: tif(memoryManager), jif(memoryManager) {
	memMan = memoryManager;

	//Contents of descriptor file get loaded
	if (!readDescriptorFile(descFile, id, userManager))
		throw std::exception("Failed to create Job.");
	
	//Contents of status file get loaded
	readStatusFile(statusFile);
}

JobInfo* Job::getJobInfo() {
	return jobInfo;
}

bool Job::update() {
	//TODO: optimize
	uint32 currTime = Utils::getUTCtime();
	bool complete = true;
	for (size_t i = 0; i < taskCount; i++) {
		//If the status isn't special code and is expired, mark it unassigned.
		if (taskStatus[i] > STATUS_COMPLETE && currTime > taskStatus[i])
			taskStatus[i] = STATUS_UNASSIGNED;

		complete &= taskStatus[i] == STATUS_COMPLETE;
	}

	return complete;
}

bool Job::isComplete() {
	//TODO: optimize
	bool complete = true;
	for (size_t i = 0; i < taskCount; i++) {
		complete &= taskStatus[i] == STATUS_COMPLETE;
	}
	return complete;
}

void Job::setTaskStatus(ID_t task, taskStatus_t status) {
	taskStatus[task] = status;
}

size_t Job::getTaskCount() {
	return taskCount;
}

TaskInfo* Job::getTaskByID(ID_t id) {
	if (id >= taskCount)
		return nullptr;
	return tasks[id];
}

taskStatus_t Job::getTaskStatus(ID_t id) {
	if (id >= taskCount)
		return STATUS_COMPLETE;

	return taskStatus[id];
}

TaskInfo* Job::getUnassignedTask() {
	//TODO: optimize
	for (size_t i = 0; i < taskCount; i++) {
		if (taskStatus[i] == STATUS_UNASSIGNED) {
			taskStatus[i] = Utils::getUTCtime() + taskLength;
			return tasks[i];
		}
	}

	return nullptr;
}

const std::string& Job::getOutputDirectory() {
	return outputDirectory;
}

void Job::saveStatus(const char* file) {
	std::ofstream statusFile(file, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

	if (!statusFile)
		throw std::exception("Can't open status file for Job.");

	//Write task status
	statusFile.write((char*)taskStatus, taskCount * sizeof(taskStatus_t));

	if (!statusFile)
		throw std::exception("Can't write status file for Job.");
	statusFile.close();
}

Job::~Job() {
	//Clen-up ...Info objects
	jif.destroyJobInfo(jobInfo);
	for (size_t i = 0; i < taskCount; i++) {
		tif.destroyTaskInfo(tasks[i]);
	}

	//Deallocate arrays
	memMan->releaseArray((uint8*)tasks);
	memMan->releaseArray((uint8*)taskStatus);
}

bool Job::readDescriptorFile(const char* descriptorFile, ID_t jobID, IUserManager* userManager) {
	Log::logMessage("Reading job descriptor file ", descriptorFile);

	//Read descriptor file.
	pt::ptree tree;
	try {
		pt::read_xml(descriptorFile, tree);
		tree = tree.get_child("job");
	} catch (std::exception ex) {
		Log::logError("Failed to read job descriptor file ", descriptorFile);
		return false;
	}

	std::string name;
	std::string username;
	size_t fileCount;
	const char** fileNames;
	size_t readCount;
	pt::ptree fileTree;
	try {
		//Read info
		name = tree.get<std::string>("name");
		username = tree.get<std::string>("user");
		outputDirectory = tree.get<std::string>("outputDirectory");

		//Read file list
		fileTree = tree.get_child("jobFiles");
		fileCount = fileTree.size();
		fileNames = new const char* [fileCount];
		readCount = 0;
		for (pt::ptree::iterator i = fileTree.begin(); i != fileTree.end(); i++) {
			fileNames[readCount++] = i->second.data().c_str();
		}
	} catch (std::exception ex) {
		Log::logError("Failed to parse job descriptor file ", descriptorFile);
		return false;
	}

	//Create JobInfo
	jobInfo = jif.createJobInfo(jobID, userManager->usernameToID(username), name.c_str(), fileCount, fileNames);

	//Read tasks
	try {
		pt::ptree taskTree = tree.get_child("taskList");
		taskCount = taskTree.size();
		tasks = (TaskInfo**)memMan->allocateArray(taskCount, sizeof(TaskInfo*));
		readCount = 0;
		for (pt::ptree::iterator i = taskTree.begin(); i != taskTree.end(); i++) {
			tasks[readCount] = readTaskInfo(i->second, readCount);
			readCount++;
		}
	} catch (std::exception ex) {
		Log::logError("Failed to parse tasks from job descriptor file ", descriptorFile);
		return false;
	}

	//Clen-up
	delete[] fileNames;
	return true;
}

TaskInfo* Job::readTaskInfo(pt::ptree& node, ID_t taskId) {
	doubleID_t id = TaskInfo::IDfromJobAndTask(jobInfo->getID(), taskId);
	std::string name = node.get<std::string>("name");
	std::string cmd = node.get<std::string>("command");

	//Read input files
	pt::ptree inFilesTree = node.get_child("inputFiles");
	size_t inFileCount = inFilesTree.size();
	const char** inFileNames = new const char* [inFileCount];
	size_t readCount = 0;
	for (pt::ptree::iterator i = inFilesTree.begin(); i != inFilesTree.end(); i++) {
		inFileNames[readCount++] = i->second.data().c_str();
	}

	//Read output files
	pt::ptree outFilesTree = node.get_child("outputFiles");
	size_t outFileCount = outFilesTree.size();
	const char** outFileNames = new const char* [outFileCount];
	readCount = 0;
	for (pt::ptree::iterator i = outFilesTree.begin(); i != outFilesTree.end(); i++) {
		outFileNames[readCount++] = i->second.data().c_str();
	}

	TaskInfo* ti = tif.createTaskInfo(id, name.c_str(), cmd.c_str(), inFileCount, inFileNames, outFileCount, outFileNames);

	//Clean-up
	delete[] inFileNames;
	delete[] outFileNames;

	return ti;
}

void Job::readStatusFile(const char* filename) {
	std::ifstream statusFile(filename, std::ifstream::in | std::ifstream::binary);

	if (!statusFile.is_open())
		throw std::exception("Can't open status file for Job.");

	//Read task status
	taskStatus = (taskStatus_t*)memMan->allocateArray(taskCount, sizeof(taskStatus_t));
	statusFile.read((char*)taskStatus, taskCount * sizeof(taskStatus_t));

	if (!statusFile)
		throw std::exception("Can't read status file for Job.");
	statusFile.close();
}
