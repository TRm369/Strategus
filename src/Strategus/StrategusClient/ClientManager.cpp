#include "pch.h"
#include "ClientManager.h"
#include "../StrategusCore/Utils.h"
#include "../StrategusCore/MemoryManager/DummyMemoryManager.h"

ClientManager::ClientManager(Config* cfg, IClientConnection* conn) :
	cfm(cfg->rootDirectory), cif(memMan = new DummyMemoryManager()) {
	config = cfg;
	connection = conn;
	CI = cif.createClientInfo(0, config->flags.size(), config->flags.data());

	slotsUsed = 0;
	stopCtl = false;
}

void ClientManager::start() {
	std::thread::id nonJoinable;
	if (controlThread.get_id() != nonJoinable)
		return; //Already running

	//Start a thread running the control cycle
	controlThread = std::thread(controlCycle, this);
}

void ClientManager::stop() {
	//TODO: better (stop subprocesses)

	std::thread::id nonJoinable;
	if (controlThread.get_id() == nonJoinable)
		return; //Already stopped

	//Signal stop and wait for the thread to exit
	{
		std::lock_guard lock(mutex);
		stopCtl = true;
	}
	controlThread.join();
}

void ClientManager::controlCycle(ClientManager* cliMan) {
	//Periodically check for available slots and finished tasks
	//TODO: multi-slot tasks

	while (true) {
		{
			//Lock
			std::lock_guard guard(cliMan->mutex);

			//Control signals
			if (cliMan->stopCtl) {
				cliMan->stopCtl = false;
				break;
			}


			Task* finishedTask = nullptr;
			std::vector<Task*>::iterator it;
			//Look for finished task
			for (it = cliMan->running.begin(); it < cliMan->running.end(); it++) {
				if ((*it)->isFinished()) {
					finishedTask = *it;
					break;
				}
			}
			//If there is one, report it, remove it from running list and release the slot
			if (finishedTask != nullptr) {
				submitTask(cliMan, finishedTask);
				cliMan->running.erase(it);
				delete finishedTask;
				cliMan->slotsUsed--;
			}

			//Request new task if slot available
			if (cliMan->config->availableSlots > cliMan->slotsUsed) {
				TaskInfo* ti = cliMan->connection->requestTask(cliMan->CI, cliMan->cfm);
				if (ti != nullptr) {
					createTask(cliMan, ti);
				}
				//TODO: incremental wait on no new task
			}

			//Unlock (guard goes out of scope)
		}

		//Wait
		Utils::sleep(cliMan->config->pollRate);
	}
}

void ClientManager::submitTask(ClientManager* cliMan, Task* task) {
	//Report the task to the server and upload output files
	cliMan->connection->reportTaskFinished(task->getTaskInfo(), cliMan->cfm);

	//Clean-up it's files
	cliMan->cfm.removeTaskData(task->getTaskInfo()->getFullID());
}

void ClientManager::createTask(ClientManager* cliMan, TaskInfo* ti) {
	Task* newTask = new Task(ti, cliMan->cfm);
	cliMan->running.push_back(newTask);
	cliMan->slotsUsed++;
	newTask->start();
}
