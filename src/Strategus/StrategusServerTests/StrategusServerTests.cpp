#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "../StrategusServer/Job.h"
#include "../StrategusCore/MemoryManager/DummyMemoryManager.h"
#include "../StrategusServer/UserManager/DummyUserManager.h"

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
	};
}
