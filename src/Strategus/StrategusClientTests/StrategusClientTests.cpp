#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "Config.h"
#include "Task.h"
#include "../StrategusCore/DataClassFactories.h"
#include"../StrategusCore/MemoryManager/DummyMemoryManager.h"
#include <filesystem>
#include "../StrategusCore/Utils.h"
#include "Process/WindowsProcess.h"
#include <fstream>
#include "ClientManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace fs = std::filesystem;

namespace ClientTests {

	class DummyConnection : public IClientConnection {
	public:
		IMemoryManager* memMan;
		TaskInfoFactory tif;
		int taskCount = 0;
		const char** cmds = new const char* [8]{ "echo done>out0.txt", "echo done>out1.txt", "echo done>out2.txt", "echo done>out3.txt", "echo done>out4.txt", "echo done>out5.txt", "echo done>out6.txt", "echo done>out7.txt" };
		bool done[8];

		DummyConnection() : memMan(new DummyMemoryManager()), tif(memMan) {}

		bool connect(std::string& hostname) {
			return true;
		}

		TaskInfo* requestTask(ClientInfo* ci, ClientFileManager& fileMan) {
			if (taskCount >= 8)
				return nullptr;
			TaskInfo* ti = tif.createTaskInfo(taskCount, "", cmds[taskCount], 0, nullptr, 0, nullptr);
			taskCount++;
			return ti;
		}

		bool reportTaskFinished(TaskInfo* task, ClientFileManager& fileMan) {
			done[task->getTaskID()] = true;
			tif.destroyTaskInfo(task);
			return true;
		}

		bool verifyAllDone() {
			bool ok = true;
			for (int i = 0; i < 8; i++) {
				ok &= done[i];
			}
			return ok;
		}
	};

	TEST_CLASS(ConfigTests) {
	public:
		Config cfg;

		ConfigTests() { cfg.read("strategus.cfg"); }

		TEST_METHOD(configFlags) {
			int expectedCount = 6;
			uint64 flags[] = { 0, 0x41, 0x47414c46, 0x26205f4021, 0x554f594f4c4c4548 , 0x6161616161616161 };

			Assert::AreEqual(expectedCount, (int)cfg.flags.size());
			for (int i = 0; i < expectedCount; i++) {
				Assert::AreEqual<uint64>(flags[i], cfg.flags[i]);
			}
		}

		TEST_METHOD(configValues) {
			Assert::AreEqual<uint32>(cfg.availableSlots, 2);
		}
	};

	TEST_CLASS(FileManagerTests) {
	public:
		TEST_METHOD(DirectoryPaths) {
			fs::remove_all(".\\client");

			ClientFileManager cfm = ClientFileManager(".\\client");
			Assert::IsTrue(cfm.getJobDirectory(123456789) == ".\\client\\data\\123456789");

			doubleID_t taskID = TaskInfo::IDfromJobAndTask(123456, 3141592);
			Assert::IsTrue(cfm.getTaskDirectory(taskID) == ".\\client\\data\\123456\\3141592");

			Assert::IsTrue(fs::exists(".\\client\\data"));
			Assert::IsTrue(fs::exists(".\\client\\data\\123456789"));
			Assert::IsTrue(fs::exists(".\\client\\data\\123456\\3141592"));

			cfm.removeTaskData(taskID);
			Assert::IsTrue(fs::exists(".\\client\\data\\123456"));
			Assert::IsFalse(fs::exists(".\\client\\data\\123456\\3141592"));

			cfm.removeJobData(SECOND_ID(taskID));
			Assert::IsFalse(fs::exists(".\\client\\data\\123456"));
		}
	};

	TEST_CLASS(TaskTests) {
	public:
		ClientFileManager cfm;
		IMemoryManager* memMan;
		TaskInfoFactory tif;
		TaskTests() : cfm(fs::current_path().string() + "\\client"), memMan(new DummyMemoryManager()), tif(memMan) {}

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
			const char** oFiles = new const char* { "out.txt" };
			TaskInfo* ti = tif.createTaskInfo((uint64(1) << 32) + 1, "tst", "echo Hello world>out.txt", 0, nullptr, 1, oFiles);
			cfm.removeJobData(ti->getJobID()); //Clean-up from previous runs
			Task task = Task(ti, cfm);
			Assert::IsTrue(task.start());
			Sleep(10);
			Assert::IsTrue(task.verifyOutputFiles());
			delete oFiles;
			tif.destroyTaskInfo(ti);
		}

		TEST_METHOD(NameResolution) {
			TaskInfo* ti = tif.createTaskInfo(0, "", "type $JOB\\in.txt>out.txt", 0, nullptr, 0, nullptr);
			cfm.removeTaskData(ti->getFullID()); //Clean-up from previous runs
			std::ofstream testFile((cfm.getJobDirectory(ti->getJobID()) + "\\in.txt").c_str());
			testFile << "Hello world";
			testFile.flush();
			testFile.close();
			Task task(ti, cfm);
			task.start();
			Sleep(100);
			Assert::IsTrue(fs::exists(".\\client\\data\\0\\0\\out.txt"));

			std::ifstream file(".\\client\\data\\0\\0\\out.txt");
			Assert::IsTrue(file.is_open());
			std::string line;
			std::getline(file, line);
			Assert::IsTrue(line == "Hello world");
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

	TEST_CLASS(ClientManagerTests) {
		TEST_METHOD(TasksExecute) {
			fs::remove_all(".\\client\\data\\0");
			Config cfg;
			cfg.read("strategus.cfg");
			DummyConnection* conn = new DummyConnection();
			ClientManager cm(&cfg, conn);

			cm.start();
			Sleep(5000);
			cm.stop();
			Assert::IsTrue(conn->verifyAllDone());
		}
	};
}