#pragma once
#include "pch.h"
#include <vector>
#include "../StrategusCore/Definitions.h"

class Config {
public:
	///Reads the config from a file
	static void readConfig(std::string& fileName);

	///Client flags
	static std::vector<uint64> flags;
};