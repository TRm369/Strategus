#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "../ClientFileManager.h"
#include "../../StrategusCore/DataClasses/TaskInfo.h"
#include <filesystem>

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
}