#pragma once
#include "../Definitions.h"
/**
* Handles memory allocations.
* 
* Used to control how memory is allocated to prevent memory segmentation.
*/
class IMemoryManager {
public:
	/// Allocates a block of memory of given size
	virtual uint8* getMemoryBlock(size_t size) = 0;

	/// Releases a previously allocated block of memory.
	virtual void releaseMemoryBlock(uint8* block) = 0;

	///Alocates a block of memory for an array.
	
};