#include "SSP_Packet.h"

SSP_Packet::SSP_Packet(uint8 category, uint8 command, uint16 payloadSize) : identifier(convertToIdentifier(category, command)), payloadSize(payloadSize) {}
SSP_Packet::SSP_Packet(Identifier_t identifier, uint16 payloadSize) : identifier(identifier), payloadSize(payloadSize) {}

void* SSP_Packet::getPayloadPrt() {
	return (void*)(this+1);
}

uint8 SSP_Packet::getCategory() {
	return (uint8)(identifier >> 8);
}

uint8 SSP_Packet::getCommand() {
	return uint8(identifier);
}

Identifier_t SSP_Packet::getIdentifier() {
	return identifier;
}

uint16 SSP_Packet::getPayloadSize() {
	return payloadSize;
}

inline uint16 SSP_Packet::convertToIdentifier(uint8 category, uint8 command) {
	return ((uint16)category << 8) + command;
}
