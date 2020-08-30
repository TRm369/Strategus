#pragma once
#include "IMemoryManager.h"
class DummyMemoryManager : public IMemoryManager{
public:
	///Allocates a block of memory of given size
	uint8* getMemoryBlock(size_t size) { return new uint8[size]; };
	
	///Releases a previously allocated block of memory.
	void releaseMemoryBlock(uint8* block) {	delete[] block;	};

	///Alocates a block of memory for an array.
	uint8* allocateArray(size_t arraySize, size_t objectSize) { return new uint8[arraySize * objectSize]; };
};