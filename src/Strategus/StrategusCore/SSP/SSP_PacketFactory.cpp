#include "SSP_PacketFactory.h"
#include <cstring> //For memcpy

SSP_PacketFactory::SSP_PacketFactory(IMemoryManager* mm) {
	memMan = mm;
	currPtr = nullptr;
	inPlace = true;
	ptr = nullptr;
}

bool SSP_PacketFactory::constructInPlace(uint16 maxSize) {
	//Only one packet can be constructed at one time
	if (conStarted)
		return false;

	//Set state
	conStarted = true;
	inPlace = true;
	currSize = sizeof(SSP_Packet);
	this->maxSize = maxSize;

	//Allocate memory
	ptr = memMan->getMemoryBlock(maxSize);
	currPtr = ptr + sizeof(SSP_Packet);
	return true;
}

bool SSP_PacketFactory::constructAndCopy(uint16 maxSize) {
	//Only one packet can be constructed at one time
	if (conStarted)
		return false;

	//Set state
	conStarted = true;
	inPlace = false;
	currSize = sizeof(SSP_Packet);
	this->maxSize = maxSize;

	//Allocate memory
	ptr = memMan->getMemoryBlock(maxSize);
	currPtr = ptr + sizeof(SSP_Packet);
	return true;
}

bool SSP_PacketFactory::appendData(void* data, uint16 size) {
	//Check if we have space
	if (currSize + size > maxSize)
		return false;

	memcpy(currPtr, data, size);
	currPtr += size;
	currSize += size;
	return true;
}

bool SSP_PacketFactory::appendUint32(uint32 data) {
	//Check if we have space
	if (currSize + sizeof(uint32) > maxSize)
		return false;

	*(uint32*)currPtr = data;
	currPtr += sizeof(uint32);
	currSize += sizeof(uint32);
	return true;
}

bool SSP_PacketFactory::appendUint64(uint64 data) {
	//Check if we have space
	if (currSize + sizeof(uint64) > maxSize)
		return false;

	*(uint64*)currPtr = data;
	currPtr += sizeof(uint64);
	currSize += sizeof(uint64);
	return true;
}

SSP_Packet* SSP_PacketFactory::finishPacket(uint8 category, uint8 command) {
	return finishPacket(SSP_Packet::convertToIdentifier(category, command));
}

SSP_Packet* SSP_PacketFactory::finishPacket(Identifier_t identifier) {
	SSP_Packet* packetPtr = (SSP_Packet*)ptr;
	
	if (inPlace == false) {
		//Copy the data to new location and clean up
		packetPtr = (SSP_Packet*)memMan->getMemoryBlock(currSize);
		memcpy(packetPtr, ptr, currSize);
		memMan->releaseMemoryBlock(ptr);
	}

	//Initialize the packet instance
	packetPtr->identifier = identifier;
	packetPtr->payloadSize = currSize - sizeof(SSP_Packet);

	conStarted = false;
	return packetPtr;
}

void SSP_PacketFactory::destroyPacket(SSP_Packet* packet) {
	memMan->releaseMemoryBlock((uint8*)packet);
}
