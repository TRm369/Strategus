#pragma once
#include "Config.h"
#include "IClientConnection.h"
#include "Task.h"
#include "ClientFileManager.h"
#include <thread>
#include <mutex>

/// Contains logic controlling the client.
class ClientManager {
public:
	/// Initializes the manager
	ClientManager(Config* config, IClientConnection* connection);

	/// Begins the execution of control cycle and task execution.
	void start();

	/// Stops execution of control cycle and waits for the thread to exit.
	void stop();

private:
	Config* config;
	IClientConnection* connection;
	ClientFileManager cfm;

	//Number of slots currently being used
	uint32 slotsUsed;
	
	//Running tasks
	std::vector<Task*> running;

	//Thread responsible for managing the client.
	std::thread controlThread;

	//Mutex for accessing data
	std::mutex mutex;

	/// Control flags for the thread.
	bool stopCtl;

	/// Method responsible for managing the client.
	static void controlCycle(ClientManager* cliMan);

	/// Submits finished task and removes it's data.
	static void submitTask(ClientManager* cliMan, Task* task);

	/// Creates a new Task and starts its execution
	static void createTask(ClientManager* cliMan, TaskInfo* ti);
};

