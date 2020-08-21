#include "pch.h"
#include "StrategusClient.h"
#include "StrategusClientInternal.h"

bool StrategusClient::initialize(std::string configFilePath) {
    return intern->initialize(configFilePath);
}

bool StrategusClient::connectToServer(std::string hostname) {
    return intern->connectToServer(hostname);
}

void StrategusClient::shutdown() {
    intern->shutdown();
}
