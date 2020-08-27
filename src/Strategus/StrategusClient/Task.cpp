#include "pch.h"
#include "Task.h"
#include "../StrategusCore/Utils.h"

#ifdef OS_WINDOWS
#include "Process/WindowsProcess.h"
#endif

#define JOB_DIR "$JOB"
#define TASK_DIR "$TASK"

Task::Task(TaskInfo* ti, ClientFileManager& cfm) : cfm(cfm) {
	taskInfo = ti;
	jobDir = cfm.getJobDirectory(taskInfo->getJobID());
	taskDir = cfm.getTaskDirectory(taskInfo->getFullID());
	resCommand = taskInfo->getCommand();
	resolveCommand();
}

bool Task::start() {
#ifdef OS_WINDOWS
	proc = new WindowsProcess();
#elif
	return false;
#endif
	
	return proc->execute(resCommand, taskDir);
}

bool Task::isFinished() {
	//If for some reason this task wasn't yet started, treat it as unfinished.
	if (proc == nullptr)
		return false;

	return !proc->isRunning();
}

bool Task::verifyOutputFiles() {
	bool verified = true;

	//Loop through task;s output files and check if they're present
	for (int i = 0; i < taskInfo->getOutputFileCount(); i++) {
		verified = verified && cfm.checkTaskFile(taskInfo->getFullID(), taskInfo->getOutputFile(i));
	}

	return verified;
}

TaskInfo* Task::getTaskInfo() {
	return taskInfo;
}

Task::~Task() {
	//If there's a process associated with this task, kill it and delete the object
	if (proc != nullptr) {
		if (!proc->kill()) {
			//Can't kill the subprocess.
			throw std::exception("Can't kill subprocess.");
		}
		delete proc;
	}
}

void Task::resolveCommand() {
	Utils::replaceAll(resCommand, JOB_DIR, jobDir);
	Utils::replaceAll(resCommand, TASK_DIR, taskDir);
}
