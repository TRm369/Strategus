#pragma once
#include "IMemoryManager.h"
class DummyMemoryManager : public IMemoryManager{
public:
	///Allocates a block of memory of given size
	uint8* getMemoryBlock(size_t size) { return new uint8[size]; };
	///Releases a previously allocated block of memory.
	virtual void releaseMemoryBlock(uint8* block) {	delete[] block;	};
};

