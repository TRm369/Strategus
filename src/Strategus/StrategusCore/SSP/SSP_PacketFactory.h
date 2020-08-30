#pragma once
#include "../MemoryManager/IMemoryManager.h"
#include "SSP_Packet.h"

class SSP_PacketFactory {
public:
	SSP_PacketFactory(IMemoryManager* mm);

	/// Begins in-place construction of a packet. If the specified size isn't filled up, the memory stay allocated. Unused space won't be transmitted with the packet.
	/// <param name="maxSize">Size of the packet with payload.</param>
	/// <returns>Returns if construction of a new packet was started.</returns>
	bool constructInPlace(uint16 maxSize);

	/// Begins construction of a packet. After the construction is complete, data is copied to a new location and unused space is freed.
	/// <param name="maxSize">Maximum size of the packet with the payload.</param>
	/// <returns>Returns if construction of a new packet was started.</returns>
	bool constructAndCopy(uint16 maxSize);

	bool appendData(void* data, uint16 size);
	bool appendUint32(uint32 data);
	bool appendUint64(uint64 data);

	/// Finishes construction and returns a pointer to the constructed packet.
	/// 
	/// This packet has to be released using the method destroyPacket of this class.
	SSP_Packet* finishPacket(uint8 category, uint8 command);

	/// Finishes construction and returns a pointer to the constructed packet.
	/// 
	/// This packet has to be released using the method destroyPacket of this class.
	SSP_Packet* finishPacket(Identifier_t identifier);

	/// Frees the memory used by a packet.
	void destroyPacket(SSP_Packet* packet);

private:
	IMemoryManager* memMan;

	//Is a packet currently being constructed?
	bool conStarted = false;

	//Are we building in-place?
	bool inPlace;

	//Maximum payload size for this packet
	uint16 maxSize = 0;

	//Size of current payload
	uint16 currSize = 0;

	//Pointer to memory block being used
	uint8* ptr;

	//Pointer to beginning of free mem inside our block.
	uint8* currPtr;
};

