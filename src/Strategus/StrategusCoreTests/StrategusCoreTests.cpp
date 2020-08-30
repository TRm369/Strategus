#include "pch.h"
#include "CppUnitTest.h"

#include "../StrategusCore/DataClassFactories.h"
#include "DummyMemoryManager.h"
#include "../StrategusCore/SSP/SSP_PacketFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StrategusCoreTests
{
	TEST_CLASS(TaskInfoTests)
	{
	public:
		TaskInfoFactory tif;
		IMemoryManager* memMan;

		TaskInfoTests() : tif(memMan = new DummyMemoryManager()) {}

		TEST_METHOD(Create) {
			doubleID_t id = 0x10B00000B00B1E50;
			const char* name = "taskName";
			const char* command = "command";
			const char** inFiles = new const char* [2]{ "inFile1", "inFile2" };
			const char** outFiles = new const char* [2]{ "outFile1", "outFile2" };

			TaskInfo* ti = tif.createTaskInfo(id, name, command, 2, inFiles, 2, outFiles);

			Assert::AreEqual<uint32>(ti->getTaskID(), 0xB00B1E50);
			Assert::AreEqual<uint32>(ti->getJobID(), 0x10B00000);
			Assert::IsTrue(strcmp(ti->getName(), "taskName") == 0);
			Assert::IsTrue(strcmp(ti->getCommand(), "command") == 0);
			Assert::AreEqual<uint32>(ti->getInputFileCount(), 2);
			Assert::AreEqual<uint32>(ti->getOutputFileCount(), 2);
			Assert::IsTrue(strcmp(ti->getInputFile(0), "inFile1") == 0);
			Assert::IsTrue(strcmp(ti->getInputFile(1), "inFile2") == 0);
			Assert::IsTrue(strcmp(ti->getOutputFile(0), "outFile1") == 0);
			Assert::IsTrue(strcmp(ti->getOutputFile(1), "outFile2") == 0);

			tif.destroyTaskInfo(ti);
		}

		TEST_METHOD(MemoryCopy) {
			doubleID_t id = 0x10B00000B00B1E50;
			const char* name = "taskName";
			const char* command = "command";
			const char** inFiles = new const char* [2]{ "inFile1", "inFile2" };
			const char** outFiles = new const char* [2]{ "outFile1", "outFile2" };

			TaskInfo* orig = tif.createTaskInfo(id, name, command, 2, inFiles, 2, outFiles);
			size_t size = orig->getSize();
			TaskInfo* ti = (TaskInfo*)memMan->getMemoryBlock(size);
			memcpy(ti, orig, size);
			tif.destroyTaskInfo(orig);
			ti->recalculatePointers();

			Assert::AreEqual<uint32>(ti->getTaskID(), 0xB00B1E50);
			Assert::AreEqual<uint32>(ti->getJobID(), 0x10B00000);
			Assert::IsTrue(strcmp(ti->getName(), "taskName") == 0);
			Assert::IsTrue(strcmp(ti->getCommand(), "command") == 0);
			Assert::AreEqual<uint32>(ti->getInputFileCount(), 2);
			Assert::AreEqual<uint32>(ti->getOutputFileCount(), 2);
			Assert::IsTrue(strcmp(ti->getInputFile(0), "inFile1") == 0);
			Assert::IsTrue(strcmp(ti->getInputFile(1), "inFile2") == 0);
			Assert::IsTrue(strcmp(ti->getOutputFile(0), "outFile1") == 0);
			Assert::IsTrue(strcmp(ti->getOutputFile(1), "outFile2") == 0);

			tif.destroyTaskInfo(ti);
		}
	};

	TEST_CLASS(JobInfoTests) {
	public:
		JobInfoFactory jif;
		IMemoryManager* memMan;

		JobInfoTests() : jif(memMan = new DummyMemoryManager()) {}

		TEST_METHOD(Create) {
			ID_t ID = 0xB00B1E5;
			ID_t userID = 0xDECAFBAD;
			const char* name = "jobName";
			uint16 fileCount = 2;
			const char** files = new const char*[] {"file1", "file2"};

			JobInfo* ji = jif.createJobInfo(ID, userID, name, fileCount, files);

			Assert::AreEqual<uint32>(ji->getID(), ID);
			Assert::AreEqual<uint32>(ji->getUserID(), userID);
			Assert::AreEqual<uint32>(ji->getFileCount(), fileCount);
			Assert::IsTrue(strcmp(ji->getFileName(0), files[0]) == 0);
			Assert::IsTrue(strcmp(ji->getFileName(1), files[1]) == 0);

			jif.destroyJobInfo(ji);
		}

		TEST_METHOD(Copy) {
			ID_t ID = 0xB00B1E5;
			ID_t userID = 0xDECAFBAD;
			const char* name = "jobName";
			uint16 fileCount = 2;
			const char** files = new const char* [] {"file1", "file2"};

			JobInfo* orig = jif.createJobInfo(ID, userID, name, fileCount, files);
			size_t size = orig->getSize();
			JobInfo* ji = (JobInfo*)memMan->getMemoryBlock(size);
			memcpy(ji, orig, size);
			jif.destroyJobInfo(orig);
			ji->recalculatePointers();

			Assert::AreEqual<uint32>(ji->getID(), ID);
			Assert::AreEqual<uint32>(ji->getUserID(), userID);
			Assert::AreEqual<uint32>(ji->getFileCount(), fileCount);
			Assert::IsTrue(strcmp(ji->getFileName(0), files[0]) == 0);
			Assert::IsTrue(strcmp(ji->getFileName(1), files[1]) == 0);

			jif.destroyJobInfo(ji);
		}
	};

	TEST_CLASS(PacketTests) {
	public:
		IMemoryManager* memMan;
		SSP_PacketFactory pf;

		PacketTests() : pf(memMan = new DummyMemoryManager()) {}

		TEST_METHOD(CreateInPlace) {
			pf.constructInPlace(1024);
			pf.appendUint32(1);
			pf.appendUint64(2);
			pf.appendData((void*)"Hello world", 12);
			pf.appendUint32(3);
			SSP_Packet* packet = pf.finishPacket(0);

			Assert::AreEqual<uint32>(packet->getIdentifier(), 0);
			Assert::AreEqual<uint32>(packet->getPayloadSize(), 28);

			uint32* int1 = (uint32*)packet->getPayloadPrt();
			Assert::AreEqual<uint32>(*int1, 1);
			uint64* int2 = (uint64*)(int1 + 1);
			Assert::AreEqual<uint64>(*int2, 2);
			char* str = (char*)(int2 + 1);
			Assert::IsTrue(strcmp(str, "Hello world") == 0);
			uint32* int3 = (uint32*)(str + 12);
			Assert::AreEqual<uint32>(*int3, 3);
		}

		TEST_METHOD(CreateAndCopy) {
			pf.constructAndCopy(1024);
			pf.appendUint32(1);
			pf.appendUint64(2);
			pf.appendData((void*)"Hello world", 12);
			pf.appendUint32(3);
			SSP_Packet* packet = pf.finishPacket(0);

			Assert::AreEqual<uint32>(packet->getIdentifier(), 0);
			Assert::AreEqual<uint32>(packet->getPayloadSize(), 28);

			uint32* int1 = (uint32*)packet->getPayloadPrt();
			Assert::AreEqual<uint32>(*int1, 1);
			uint64* int2 = (uint64*)(int1 + 1);
			Assert::AreEqual<uint64>(*int2, 2);
			char* str = (char*)(int2 + 1);
			Assert::IsTrue(strcmp(str, "Hello world") == 0);
			uint32* int3 = (uint32*)(str + 12);
			Assert::AreEqual<uint32>(*int3, 3);
		}
	};
}
