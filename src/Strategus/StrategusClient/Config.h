#pragma once
#include "pch.h"
#include <vector>
#include "../StrategusCore/Definitions.h"

/// Configuration class for the client.
class Config {
public:
	///Reads the config from a file
	Config(std::string fileName);

	///Client flags
	std::vector<uint64> flags;

	///Number of (single-slot) tasks that can be executed in parallel
	uint32 availableSlots;
};