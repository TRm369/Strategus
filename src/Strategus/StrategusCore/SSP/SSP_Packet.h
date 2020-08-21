#pragma once
#include "../Definitions.h"

///Packet identifiers
enum PacketIdentifiers {
	Positive = 0x0000,
	Negative = 0x0001
};

typedef uint16 Identifier_t;

///Base class for SSP packets
class SSP_Packet {
	friend class SSP_PacketFactory;
public:
	///Constructor using separate category and command
	SSP_Packet(uint8 category, uint8 command, uint16 payloadSize);

	///Constructor using identifier
	SSP_Packet(Identifier_t identifier, uint16 payloadSize);

	///Returns a pointer to the payload of this packet
	void* getPayloadPrt();

	///Returns packet category
	uint8 getCategory();

	///Returns packet category
	uint8 getCommand();

	///Returns packet identifier (first byte category, second command)
	Identifier_t getIdentifier();

	///Size of the payload
	uint16 getPayloadSize();

	///Converts category and command to identifier
	static uint16 convertToIdentifier(uint8 category, uint8 command);

private:
	Identifier_t identifier;
	uint16 payloadSize;
};