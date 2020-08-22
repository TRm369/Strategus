#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestAssert.h>
#include "../Config.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClientTests {
TEST_CLASS(ConfigTests) {
public:
	Config cfg = Config("strategus.cfg");

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
}