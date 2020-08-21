#pragma once
class StrategusClientInternal {
public:
	///Initializes the client.
	///<param name="configFilePath">Path to a file containing config information.</param>
	///<returns>Returns true if initialization was successful.</returns>
	bool initialize(std::string configFilePath);

	///Connects the client to a server
	///<returns>Returns true if connection was made successfully.</returns>
	bool connectToServer(std::string hostname);

	///Shutdowns the client.
	void shutdown();
};

