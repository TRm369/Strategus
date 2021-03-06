#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "../StrategusServer/Job.h"
#include "../StrategusCore/MemoryManager/DummyMemoryManager.h"
#include "../StrategusServer/UserManager/DummyUserManager.h"
#include <fstream>
#include "../StrategusServer/Scheduler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StrategusServerTests
{
	TEST_CLASS(JobTests)
	{
	public:
		IMemoryManager* memMan;
		IUserManager* userMan;

		JobTests() : memMan(new DummyMemoryManager()), userMan(new DummyUserManager()) {}

		TEST_METHOD(readDescriptorFile) {
			Job job = Job("testJob.xml", 0xB00B1E5, memMan, userMan);

			//Test jobInfo
			JobInfo* ji = job.getJobInfo();
			Assert::AreEqual<ID_t>(ji->getID(), 0xB00B1E5);
			Assert::IsTrue(strcmp(ji->getName(), "Job name") == 0);
			Assert::IsTrue(ji->getFileCount() == 1);
			Assert::IsTrue(strcmp(ji->getFileName(0), "Path/to/job.file") == 0);

			//Test tasks
			Assert::AreEqual<size_t>(job.getTaskCount(), 1);
			TaskInfo* ti = job.getTaskByID(0);
			Assert::IsTrue(strcmp(ti->getName(), "Task name") == 0);
			Assert::IsTrue(strcmp(ti->getCommand(), "Task command") == 0);
			Assert::AreEqual<uint32>(ti->getInputFileCount(), 1);
			Assert::AreEqual<uint32>(ti->getOutputFileCount(), 1);
			Assert::IsTrue(strcmp(ti->getInputFile(0), "Path/to/input.file") == 0);
			Assert::IsTrue(strcmp(ti->getOutputFile(0), "output.file") == 0);
		}

		TEST_METHOD(readStatusFile) {
			Job job("testJob.xml", "testStatus.bin", 0, memMan, userMan);

			//Test jobInfo
			JobInfo* ji = job.getJobInfo();
			Assert::AreEqual<ID_t>(ji->getID(), 0);
			Assert::IsTrue(strcmp(ji->getName(), "Job name") == 0);
			Assert::IsTrue(ji->getFileCount() == 1);
			Assert::IsTrue(strcmp(ji->getFileName(0), "Path/to/job.file") == 0);

			//Test tasks
			Assert::AreEqual<size_t>(job.getTaskCount(), 1);
			TaskInfo* ti = job.getTaskByID(0);
			Assert::IsTrue(strcmp(ti->getName(), "Task name") == 0);
			Assert::IsTrue(strcmp(ti->getCommand(), "Task command") == 0);
			Assert::AreEqual<uint32>(ti->getInputFileCount(), 1);
			Assert::AreEqual<uint32>(ti->getOutputFileCount(), 1);
			Assert::IsTrue(strcmp(ti->getInputFile(0), "Path/to/input.file") == 0);
			Assert::IsTrue(strcmp(ti->getOutputFile(0), "output.file") == 0);
			Assert::AreEqual<uint32>(job.getTaskStatus(0), 0x11111111);
		}

		TEST_METHOD(writeStatusFile) {
			Job job("testJob.xml", "testStatus.bin", 0, memMan, userMan);
			job.setTaskStatus(0, 0xAABBCCDD);
			job.saveStatus("outputStatus.bin");

			std::ifstream testFile("outputStatus.bin");

			char buf[4];
			testFile.read(buf, 4);
			Assert::IsTrue((bool)testFile);
			char* expected = new char[4]{ (char)0xDD, (char)0xCC, (char)0xBB, (char)0xAA };
			bool same = true;
			for (int i = 0; i < 4; i++) {
				same &= buf[i] == expected[i];
			}
			Assert::IsTrue(same);
		}

		TEST_METHOD(UpdateAndComplete) {
			Job job("testJob.xml", "testStatus.bin", 0, memMan, userMan);

			Assert::IsFalse(job.isComplete());
			job.setTaskStatus(0, 0xFFFFFFFF);
			Assert::IsFalse(job.update());
			job.setTaskStatus(0, 2);
			job.update();
			Assert::IsTrue(job.getTaskStatus(0) == STATUS_UNASSIGNED);
			job.setTaskStatus(0, STATUS_COMPLETE);
			Assert::IsTrue(job.isComplete());
		}
	};

	TEST_CLASS(SchedulerTests) {
		TEST_METHOD(Empty) {
			Scheduler sch;
			Assert::IsTrue(sch.requestTask(nullptr) == nullptr);
			ID_t id = 1234;
			sch.loadNewJob("testJob.xml", id);
			Assert::IsTrue(id == 0);
			sch.loadNewJob("testJob.xml", id);
			Assert::IsTrue(id == 1);
		}

		TEST_METHOD(SingleJob) {
			Scheduler sch;
			ID_t id = 1234;
			sch.loadNewJob("8xTimeout.xml", id);

			for (int i = 0; i < 8; i++) {
				TaskInfo* task = sch.requestTask(nullptr);
				Assert::IsNotNull(task);
				sch.taskFinished(task, nullptr);
			}

			Assert::IsTrue(sch.allJobsComplete());
		}

		TEST_METHOD(SingleJobMultithread) {
			Scheduler sch;
			ID_t id = 1234;
			sch.loadNewJob("8xTimeout.xml", id);
			TaskInfo** tasks = new TaskInfo* [8];

			for (int i = 0; i < 8; i++) {
				tasks[i] = sch.requestTask(nullptr);
				Assert::IsNotNull(tasks[i]);
			}
			for (int i = 0; i < 8; i++) {
				sch.taskFinished(tasks[i], nullptr);
			}

			Assert::IsTrue(sch.allJobsComplete());
		}

		TEST_METHOD(MulitJob) {
			Scheduler sch;
			ID_t id = 1234;
			sch.loadNewJob("8xTimeout.xml", id);
			sch.loadNewJob("8xTimeout.xml", id);

			for (int i = 0; i < 16; i++) {
				TaskInfo* task = sch.requestTask(nullptr);
				Assert::IsNotNull(task);
				sch.taskFinished(task, nullptr);
			}

			Assert::IsTrue(sch.allJobsComplete());
		}

		TEST_METHOD(MultiJobMultithread) {
			Scheduler sch;
			ID_t id = 1234;
			sch.loadNewJob("8xTimeout.xml", id);
			sch.loadNewJob("8xTimeout.xml", id);
			TaskInfo** tasks = new TaskInfo * [16];

			for (int i = 0; i < 16; i++) {
				tasks[i] = sch.requestTask(nullptr);
				Assert::IsNotNull(tasks[i]);
			}
			for (int i = 0; i < 16; i++) {
				sch.taskFinished(tasks[i], nullptr);
			}

			Assert::IsTrue(sch.allJobsComplete());
		}

		TEST_METHOD(Saving) {
			Scheduler sch;
			ID_t id = 1234;
			sch.loadNewJob("8xTimeout.xml", id);
			sch.loadNewJob("8xTimeout.xml", id);

			for (int i = 0; i < 8; i++) {
				TaskInfo* task = sch.requestTask(nullptr);
				Assert::IsNotNull(task);
				sch.taskFinished(task, nullptr);
			}

			Assert::IsTrue(sch.allJobsComplete() == false);

			sch.saveStatus("savingTest");

			Scheduler sch2("savingTest");
			for (int i = 0; i < 8; i++) {
				TaskInfo* task = sch2.requestTask(nullptr);
				Assert::IsNotNull(task);
				sch2.taskFinished(task, nullptr);
			}

			Assert::IsTrue(sch2.allJobsComplete());
		}
	};
}
