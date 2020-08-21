#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "../Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClientTests {
TEST_CLASS(ConfigTests) {
public:
	ConfigTests() {
		std::string filename = "strategus.cfg";
		
		Config::readConfig(filename);
	}

	TEST_METHOD(configFlags) {
		int expectedCount = 6;
		uint64 flags[] = { 0, 0x41, 0x47414c46, 0x26205f4021, 0x554f594f4c4c4548 , 0x6161616161616161 };

		Assert::AreEqual(expectedCount, (int)Config::flags.size());
		for (int i = 0; i < expectedCount; i++) {
			Assert::AreEqual<uint64>(flags[i], Config::flags[i]);
		}
	}
};
}