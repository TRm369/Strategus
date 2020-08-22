#include "pch.h"
#include "Config.h"
#include "../StrategusCore/Utils.h"
#include <algorithm>

namespace pt = boost::property_tree;

Config::Config(std::string fileName) {
	pt::ptree tree;
	pt::read_info(fileName, tree);

	//Read values
	availableSlots = tree.get<uint32>("availableSlots");

	//Read flags
	BOOST_FOREACH(pt::ptree::value_type& val, tree.get_child("flags")) {
		flags.push_back(Utils::flagToUint64(val.first.data()));
	}
	std::sort(flags.begin(), flags.end());
}
