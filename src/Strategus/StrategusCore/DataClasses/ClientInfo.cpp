#include "ClientInfo.h"

ID_t ClientInfo::getID() {
    return clientID;
}

uint16 ClientInfo::getFlagCount() {
    return flagCount;
}

const uint64* ClientInfo::getFlags() {
    return flags;
}

uint32 ClientInfo::getSize() {
    return size;
}

void ClientInfo::recalculatePointers() {
    uint8* ptr = (uint8*)this;
    ptr += sizeof(ClientInfo);
    flags = (uint64*)ptr;
}
