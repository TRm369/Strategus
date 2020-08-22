#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "../Process/WindowsProcess.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClientTests {
TEST_CLASS(ProcessTests) {
public:
	TEST_METHOD(Execute) {
		//Cleanup in case there is a leftover after earlier test.
		std::remove("test_execute.txt");
		
		WindowsProcess prc;
		Assert::IsTrue(prc.execute("echo Hello world> test_execute.txt"));
		//Assert::IsTrue(prc.execute("tst.bat"));
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
		Assert::IsTrue(prc.execute("timeout /t 1"));
		Assert::IsTrue(prc.isRunning());
		Sleep(1200);
		Assert::IsTrue(prc.isRunning() == false);
	}
};
}