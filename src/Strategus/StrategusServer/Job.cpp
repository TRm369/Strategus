#include "pch.h"
#include "Job.h"

namespace pt = boost::property_tree;

Job::Job(const char* descFile, ID_t id, IMemoryManager* memoryManager, IUserManager* userManager)
	: tif(memoryManager), jif(memoryManager) {
	memMan = memoryManager;

	//Contents of descriptor file get loaded
	readDescriptorFile(descFile, id, userManager);

	//Contents of status file get created
	taskStatus = new taskStatus_t[taskCount];
	//TODO: init taskStatus
}

Job::Job(const char* descFile, const char* statusFile, IMemoryManager* memoryManager, IUserManager* userManager)
	: tif(memoryManager), jif(memoryManager) {
	memMan = memoryManager;

	//Contents of status file get loaded
	ID_t jobID = readStatusFile(statusFile);

	//Contents of descriptor file get loaded
	readDescriptorFile(descFile, jobID, userManager);
}

JobInfo* Job::getJobInfo() {
	return jobInfo;
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

void Job::saveStatus(const char* file) {
	std::ofstream statusFile(file, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

	if (!statusFile)
		throw std::exception("Can't open status file for Job.");

	//Write job ID
	ID_t jobID = jobInfo->getID();
	statusFile.write((char*)&jobID, sizeof(ID_t));

	//Write task count
	uint64 taskCnt = taskCount;
	statusFile.write((char*)&taskCnt, sizeof(uint64));

	//Write task status
	statusFile.write((char*)taskStatus, taskCount * sizeof(taskStatus_t));

	if (!statusFile)
		throw std::exception("Can't write status file for Job.");
	statusFile.close();
}

void Job::readDescriptorFile(const char* descriptorFile, ID_t jobID, IUserManager* userManager) {
	pt::ptree tree;
	pt::read_xml(descriptorFile, tree);
	tree = tree.get_child("job");
	
	//Read info
	std::string name = tree.get<std::string>("name");
	std::string username = tree.get<std::string>("user");
	
	//Read file list
	pt::ptree fileTree = tree.get_child("jobFiles");
	size_t fileCount = fileTree.size();
	const char** fileNames = new const char* [fileCount];
	size_t readCount = 0;
	for (pt::ptree::iterator i = fileTree.begin(); i != fileTree.end(); i++) {
		fileNames[readCount++] = i->second.data().c_str();
	}

	//Create JobInfo
	jobInfo = jif.createJobInfo(jobID, userManager->usernameToID(username), name.c_str(), fileCount, fileNames);

	//Read tasks
	pt::ptree taskTree = tree.get_child("taskList");
	taskCount = taskTree.size();
	tasks = new TaskInfo* [taskCount];
	readCount = 0;
	for (pt::ptree::iterator i = taskTree.begin(); i != taskTree.end(); i++) {
		tasks[readCount] = readTaskInfo(i->second, readCount);
		readCount++;
	}

	//Clen-up
	delete[] fileNames;
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

ID_t Job::readStatusFile(const char* filename) {
	std::ifstream statusFile(filename, std::ifstream::in | std::ifstream::binary);

	if (!statusFile.is_open())
		throw std::exception("Can't open status file for Job.");

	//Read job ID
	ID_t jobID;
	statusFile.read((char*)&jobID, sizeof(jobID));

	//Read task count
	uint64 taskCnt;
	statusFile.read((char*)&taskCnt, sizeof(uint64));
	taskCount = taskCnt;

	//Read task status
	taskStatus = new taskStatus_t[taskCount];
	statusFile.read((char*)taskStatus, taskCount * sizeof(taskStatus_t));

	if (!statusFile)
		throw std::exception("Can't read status file for Job.");
	statusFile.close();

	return jobID;
}
