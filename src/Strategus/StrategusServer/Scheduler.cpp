#include "pch.h"
#include "Scheduler.h"
#include <filesystem> //C++17

namespace pt = boost::property_tree;
namespace fs = std::filesystem;

#define SEPARATOR ((char)fs::path::preferred_separator)

typedef std::vector<JobEntry>::iterator JobEntryIterator;

Scheduler::Scheduler() {
	nextID = 0;
}

Scheduler::Scheduler(std::string directory) : Scheduler() {
	pt::ptree tree;
	pt::read_xml(schedulerDescFilePath(directory), tree);

	//Load properties
	nextID = tree.get<ID_t>("nextID");

	//Load jobs
	pt::ptree jobTree = tree.get_child("jobs");
	for (pt::ptree::iterator i = jobTree.begin(); i != jobTree.end(); i++) {
		//Load the job described by ptree *i
		Job* job;
		std::string descFile = i->second.get<std::string>("descFile");
		ID_t id = i->second.get<ID_t>("ID");
		std::string statFile = statFilePath(id, directory);
		job = new Job(descFile.c_str(), statFile.c_str(), id, &memMan, &userMan);

		//Add it to jobs vector.
		jobs.push_back(JobEntry(job, descFile));
	}
}

bool Scheduler::loadNewJob(std::string descFile, ID_t& jobID) {
	Job* job;
	jobID = nextID++;
	job = new Job(descFile.c_str(), jobID, &memMan, &userMan);

	//Add it to jobs vector.
	jobs.push_back(JobEntry(job, descFile));

	return true;
}

bool Scheduler::loadExistingJob(std::string descFile, std::string statusFile, ID_t& jobID) {
	Job* job;
	jobID = nextID++;
	job = new Job(descFile.c_str(), statusFile.c_str(), jobID, &memMan, &userMan);

	//Add it to jobs vector.
	jobs.push_back(JobEntry(job, descFile));

	return true;
}

bool Scheduler::removeJob(ID_t jobID) {
	for (JobEntryIterator i = jobs.begin(); i != jobs.end(); i++) {
		if (i->first->getJobInfo()->getID() == jobID) {
			jobs.erase(i);
			return true;
		}
	}

	return false;
}

TaskInfo* Scheduler::requestTask(ClientInfo* clientInfo) {
	//TODO: flags

	for (JobEntryIterator i = jobs.begin(); i != jobs.end(); i++) {
		TaskInfo* task = i->first->getUnassignedTask();
		if (task != nullptr)
			return task;
	}

	return nullptr;
}

void Scheduler::taskFinished(TaskInfo* task, ClientInfo* client) {
	//Find the job.
	Job* job = nullptr;
	for (JobEntryIterator i = jobs.begin(); i != jobs.end(); i++) {
		if (i->first->getJobInfo()->getID() == task->getJobID()) {
			job = i->first;
			break;
		}
	}

	if (job != nullptr) {
		job->setTaskStatus(task->getTaskID(), STATUS_COMPLETE);
		job->update(); //TODO: think about this.
	}
}

bool Scheduler::allJobsComplete() {
	for (JobEntryIterator i = jobs.begin(); i != jobs.end(); i++) {
		if (i->first->isComplete() == false)
			return false;
	}
	return true;
}

bool Scheduler::saveStatus(std::string directory) {
	//Make sure the directory exists
	if (!fs::exists(directory))
		if (!fs::create_directory(directory))
			throw std::exception("Can't create directory to save Scheduler.");

	pt::ptree tree;

	tree.put<ID_t>("nextID", nextID);

	for (JobEntryIterator i = jobs.begin(); i != jobs.end(); i++) {
		//Add the job to jobs vector
		pt::ptree jobTree = pt::ptree();
		ID_t jobID = i->first->getJobInfo()->getID();
		jobTree.put("descFile", i->second);
		jobTree.put("ID", jobID);
		tree.add_child("jobs.job", jobTree);

		//Save job status
		i->first->saveStatus(statFilePath(jobID, directory).c_str());
	}

	pt::write_xml(schedulerDescFilePath(directory), tree);
	return true;
}

inline std::string Scheduler::statFilePath(ID_t jobID, std::string& dir) {
	return dir + SEPARATOR + std::to_string(jobID) + ".stat";
}

inline std::string Scheduler::schedulerDescFilePath(std::string& dir) {
	return dir + SEPARATOR + "scheduler.desc";
}
