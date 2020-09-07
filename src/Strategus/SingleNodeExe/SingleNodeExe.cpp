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
        
        //Copy job files (if necessary)
        JobInfo* ji = scheduler->getJobInfo(ti->getJobID());
        for (int i = 0; i < ji->getFileCount(); i++) {
            std::string dstFile = fileMan.getJobFile(ji, i);
            if (!fs::exists(dstFile)) {
                std::string srcFile = ji->getFileName(i);
                fs::copy_file(srcFile, dstFile);
            }
        }

        //Delete task files from previous runs
        fs::remove_all(fileMan.getTaskDirectory(ti->getFullID()));
        //Copy task files
        for (int i = 0; i < ti->getInputFileCount(); i++) {
            std::string srcFile = ti->getInputFile(i);
            std::string dstFile = fileMan.getTaskInputFile(ti, i);
            fs::copy_file(srcFile, dstFile);
        }

        return ti;
    }

    bool reportTaskFinished(TaskInfo* task, ClientFileManager& fileMan) {
        //Copy files
        for (int i = 0; i < task->getOutputFileCount(); i++) {
            std::string srcFile = fileMan.getTaskOutputFile(task, i);
            std::string dstFile = scheduler->getTaskOutputFile(task, i);
            //Delete task files from previous runs
            fs::remove_all(dstFile);
            fs::copy_file(srcFile, dstFile);
        }
        scheduler->taskFinished(task, nullptr);
        return true;
    }
};

LocalConnection loc;

void printHelp() {
    std::cout <<
        "Use following command to control the scheduler:\n"
        "  loadnew <job descriptor file>: loads a new job from its descriptor file and starts execution.\n"
        "  save <path to save dir>: saves the scheduler status to a directory.\n"
        "  status: prints the status of the scheduler.\n"
        "  help: shows this help.\n"
        "  exit: stops the execution of the client and exits.\n\n";
}

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
    std::cout << "Single executable version of Strategus scheduler\n\n";
    std::cout << "Use command help to show help.\n";
    std::cout << "Client started. Awaiting input.\n";
    std::string buf;
    std::getline(std::cin, buf);
    while (true) {
        if (buf.find("exit") == 0)
            break;

        else if (buf.find("help") == 0)
            printHelp();

        else if (buf.find("loadnew") == 0) {
            ID_t newID;
            if (loc.scheduler->loadNewJob(buf.substr(8), newID))
                std::cout << "Loaded new job.\n";
            else
                std::cout << "Couldn't load new job.\n";
        }

        else if (buf.find("save") == 0) {
            if (loc.scheduler->saveStatus(buf.substr(5)))
                std::cout << "Saved scheduler data.\n";
            else
                std::cout << "Couldn't save scheduler data.\n";
        }

        else if (buf.find("status") == 0) {
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

