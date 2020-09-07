#include "pch.h"
#include "Config.h"
#include "../StrategusCore/Utils.h"
#include <algorithm>

namespace pt = boost::property_tree;

bool Config::read(const char* fileName) {
	Log::logMessage("Reading config file ", fileName);

	pt::ptree tree;
	//Open the file
	try {
		pt::read_info(fileName, tree);
	} catch (std::exception ex) {
		Log::logError("Error opening config file ", fileName);
		return false;
	}

	try {
		//Read values
		availableSlots = tree.get<uint32>("availableSlots");
		pollRate = tree.get<uint32>("pollRate");
		rootDirectory = tree.get<std::string>("rootDirectory");

		//Read flags
		BOOST_FOREACH(pt::ptree::value_type & val, tree.get_child("flags")) {
			flags.push_back(Utils::flagToUint64(val.first.data()));
		}
		std::sort(flags.begin(), flags.end());
	} catch (std::exception ex) {
		Log::logError("Error reading config file ", fileName);
		return false;
	}

	return true;
}
