#pragma once
#include <string>

///Contains all exports of the client library
#define DLLexport __declspec(dllexport)

//Class for internal functionality. Allows to hide all of internal headers and expose just the relevant interface.
class StrategusClientInternal;

/// C++ interface
class DLLexport StrategusClient {
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
private:
	StrategusClientInternal* intern;
};