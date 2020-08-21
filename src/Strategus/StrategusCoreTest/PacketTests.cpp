#include "CoreTests.h"
#include "../StrategusCore/SSP/SSP_PacketFactory.h"
SSP_PacketFactory pf(nullptr);

//Test in-place creation
bool test_inPlaceCreate() {
	pf.constructInPlace(1024);
	pf.appendUint32(1);
	pf.appendUint64(2);
	pf.appendData((void*)"Hello world", 12);
	pf.appendUint32(3);
	SSP_Packet* packet = pf.finishPacket(0);

	if (!testUint64("inPlaceCreate", "identifier", 0, packet->getIdentifier())) return false;
	if (!testUint64("inPlaceCreate", "payloadSize", 28, packet->getPayloadSize())) return false;

	uint32* int1 = (uint32*)packet->getPayloadPrt();
	if (!testUint64("inPlaceCreate", "int1", 1, *int1)) return false;
	uint64* int2 = (uint64*)(int1+1);
	if (!testUint64("inPlaceCreate", "int2", 2, *int2)) return false;
	char* str = (char*)(int2 + 1);
	if (!testString("inPlaceCreate", "str", "Hello world", str)) return false;
	uint32* int3 = (uint32*)(str + 12);
	if (!testUint64("inPlaceCreate", "int3", 3, *int3)) return false;

	printSucc("inPlaceCreate");
	return true;
}

//Test create and copy
bool test_createAndCopy() {
	pf.constructAndCopy(1024);
	pf.appendUint32(1);
	pf.appendUint64(2);
	pf.appendData((void*)"Hello world", 12);
	pf.appendUint32(3);
	SSP_Packet* packet = pf.finishPacket(0);

	if (!testUint64("createAndCopy", "identifier", 0, packet->getIdentifier())) return false;
	if (!testUint64("createAndCopy", "payloadSize", 28, packet->getPayloadSize())) return false;

	uint32* int1 = (uint32*)packet->getPayloadPrt();
	if (!testUint64("createAndCopy", "int1", 1, *int1)) return false;
	uint64* int2 = (uint64*)(int1 + 1);
	if (!testUint64("createAndCopy", "int2", 2, *int2)) return false;
	char* str = (char*)(int2 + 1);
	if (!testString("createAndCopy", "str", "Hello world", str)) return false;
	uint32* int3 = (uint32*)(str + 12);
	if (!testUint64("inPlaceCreate", "int3", 3, *int3)) return false;

	printSucc("createAndCopy");
	return true;
}

int runPacketTests() {
	std::cout << "Starting tests for SSP_Packet class.\n";
	pf = SSP_PacketFactory(memMan);
	int fails = 0;
	if (!test_inPlaceCreate()) fails++;
	if (!test_createAndCopy()) fails++;

	std::cout << "Tests for class SSP_Packet ended with " << fails << " fails.\n\n";

	return fails;
}