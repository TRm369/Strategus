#include "pch.h"
#include "Config.h"
#include "../StrategusCore/Utils.h"
#include <algorithm>

namespace pt = boost::property_tree;

std::vector<uint64> Config::flags;

void Config::readConfig(std::string& fileName) {
	pt::ptree tree;
	pt::read_info(fileName, tree);

	//Read flags
	BOOST_FOREACH(pt::ptree::value_type& val, tree.get_child("flags")) {
		flags.push_back(Utils::flagToUint64(val.first.data()));
	}
	std::sort(flags.begin(), flags.end());
}
