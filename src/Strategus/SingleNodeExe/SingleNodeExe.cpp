// SingleNodeExe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "../StrategusClient/ClientManager.h"
#include "../StrategusServer/Scheduler.h"

namespace fs = std::filesystem;

class LocalConnection : public IClientConnection {
public:
    Scheduler* scheduler;

    bool connect(std::string& hostname) {
        if (fs::exists("scheduler"))
            scheduler = new Scheduler("scheduler");
        else
            scheduler = new Scheduler();
        return true;
    }

    TaskInfo* requestTask(ClientInfo* ci, ClientFileManager& fileMan) {
        TaskInfo* ti = scheduler->requestTask(ci);

        if (ti == nullptr)
            return nullptr;

        for (int i = 0; i < ti->getInputFileCount(); i++) {
            std::string srcFile = ti->getInputFile(i);
            std::string dstFile = fileMan.getTaskInputFile(ti, i);
            fs::copy_file(srcFile, dstFile);
        }
        //TODO: cpy job files
        return ti;
    }

    bool reportTaskFinished(TaskInfo* task, ClientFileManager& fileMan) {
        for (int i = 0; i < task->getOutputFileCount(); i++) {
            std::string srcFile = fileMan.getTaskOutputFile(task, i);
            std::string dstFile = scheduler->getTaskOutputFile(task, i);
            fs::copy_file(srcFile, dstFile);
        }
        scheduler->taskFinished(task, nullptr);
        return true;
    }
};

LocalConnection loc;

int main()
{
    std::string hostname = "doesn't matter";
    loc.connect(hostname);

    Config cfg;
    if (!cfg.read("client.cfg")) {
        std::cout << "Couldn't read config file.";
        return -1;
    }

    ClientManager cm(&cfg, &loc);
    cm.start();
    std::cout << "Client started. Awaiting input.\n";
    std::string buf;
    std::getline(std::cin, buf);
    while (true) {
        if (buf.find("exit") == 0)
            break;

        if (buf.find("loadnew") == 0) {
            ID_t newID;
            if (loc.scheduler->loadNewJob(buf.substr(8), newID))
                std::cout << "Loaded new job.\n";
            else
                std::cout << "Couldn't load new job.\n";
        }

        if (buf.find("save") == 0) {
            if (loc.scheduler->saveStatus(buf.substr(5)))
                std::cout << "Saved scheduler data.\n";
            else
                std::cout << "Couldn't save scheduler data.\n";
        }

        if (buf.find("status") == 0) {
            if (loc.scheduler->allJobsComplete())
                std::cout << "All jobs are complete.\n";
            else
                std::cout << "Jobs are still running.\n";
        }

        std::getline(std::cin, buf);
    }
    std::cout << "Exiting!\n";
    cm.stop();
}

