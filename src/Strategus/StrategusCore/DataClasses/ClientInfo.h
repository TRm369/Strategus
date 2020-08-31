#pragma once
#include "../Definitions.h"

/// Description of a client (compute node).
class ClientInfo {
	friend class ClientInfoFactory;
public:
	/// Returns the client's ID.
	ID_t getID();

	/// Returns the number of flags.
	uint16 getFlagCount();

	/// Returns the pointer to flag array.
	const uint64* getFlags();

	///Returns the size of this object (including all data appended to it).
	uint32 getSize();

	/// Recalculates all pointers after the object has been moved to a different address in memory (eg. after a network transmit).
	void recalculatePointers();
private:
	//Shouldn't be used. Use ClientInfoFactory instead.
	ClientInfo();
	~ClientInfo();
	void* operator new(size_t size);
	void* operator new[](size_t size);
	void operator delete(void* ptr);
	void operator delete[](void* ptr);

	ID_t clientID;

	uint16 flagCount;
	uint64* flags;

	//Size of this instance (including all data appended to it)
	uint32 size;
};

