#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "../Task.h"
#include "../../StrategusCore/TaskInfoFactory.h"
#include"../../StrategusCoreTest/DummyMemoryManager.h"
#include <filesystem>
#include "../../StrategusCore/Utils.h"
#include "../Process/WindowsProcess.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace fs = std::filesystem;

namespace ClientTests {
TEST_CLASS(FileManagerTests) {
public:
	TEST_METHOD(DirectoryPaths) {
		fs::remove_all("./client");

		ClientFileManager cfm = ClientFileManager("./client");
		Assert::IsTrue(cfm.getJobDirectory(123456789) == "./client/data/123456789");
		
		doubleID_t taskID = TaskInfo::IDfromJobAndTask(123456, 3141592);
		Assert::IsTrue(cfm.getTaskDirectory(taskID) == "./client/data/123456/3141592");

		Assert::IsTrue(fs::exists("./client/data"));
		Assert::IsTrue(fs::exists("./client/data/123456789"));
		Assert::IsTrue(fs::exists("./client/data/123456/3141592"));

		cfm.removeTaskData(taskID);
		Assert::IsTrue(fs::exists("./client/data/123456"));
		Assert::IsFalse(fs::exists("./client/data/123456/3141592"));

		cfm.removeJobData(SECOND_ID(taskID));
		Assert::IsFalse(fs::exists("./client/data/123456"));
	}
};

TEST_CLASS(TaskTests) {
public:
	ClientFileManager cfm;
	IMemoryManager* memMan;
	TaskInfoFactory tif;
	TaskTests() : cfm("./client"), memMan(new DummyMemoryManager()), tif(memMan) {}

	TEST_METHOD(Execution) {
		TaskInfo* ti = tif.createTaskInfo(0, "tst", "timeout /t 1", 0, nullptr, 0, nullptr);
		Task task = Task(ti, cfm);
		Assert::IsTrue(task.isFinished() >= 0); //Just to test it doesn't crash, not sure what it should return yet.
		Assert::IsTrue(task.start());
		Assert::IsFalse(task.isFinished());
		Utils::sleep(1100);
		Assert::IsTrue(task.isFinished());
		tif.destroyTaskInfo(ti);
	}

	TEST_METHOD(OutputFile) {
		const char** oFiles = new const char* {"out.txt"};
		TaskInfo* ti = tif.createTaskInfo((uint64(1) << 32) + 1, "tst", "echo Hello world>out.txt", 0, nullptr, 1, oFiles);
		cfm.removeJobData(ti->getJobID()); //Clean-up from previous runs
		Task task = Task(ti, cfm);
		Assert::IsTrue(task.start());
		Sleep(10);
		Assert::IsTrue(task.verifyOutputFiles());
	}
};

TEST_CLASS(ProcessTests) {
public:
	TEST_METHOD(Execute) {
		//Cleanup in case there is a leftover after earlier test.
		std::remove("test_execute.txt");

		WindowsProcess prc;
		Assert::IsTrue(prc.execute("echo Hello world> test_execute.txt", ""));
		Sleep(1000); //Sleep to let the process finish

		std::ifstream file("test_execute.txt");
		Assert::IsTrue(file.is_open());
		std::string line;
		std::getline(file, line);
		Assert::IsTrue(line == "Hello world");
	}

	TEST_METHOD(IsRunning) {
		WindowsProcess prc;
		Assert::IsTrue(prc.isRunning() == false);
		Assert::IsTrue(prc.execute("timeout /t 1", ""));
		Assert::IsTrue(prc.isRunning());
		Sleep(1200);
		Assert::IsTrue(prc.isRunning() == false);
	}

	TEST_METHOD(ExecuteDirectory) {
		if (!fs::exists("./prcDir"))
			fs::create_directory("./prcDir");
		fs::remove("./prcDir/out.txt");

		WindowsProcess prc;
		Assert::IsTrue(prc.execute("echo Hello world> out.txt", "./prcDir"));
		Sleep(10);
		Assert::IsTrue(fs::exists("./prcDir/out.txt"));
	}
};
}