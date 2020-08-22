#pragma once
#include <string>

///Contains all exports of the client library
#define DLLexport __declspec(dllexport)

//Class for internal functionality. Allows to hide all of internal headers and expose just the relevant interface.
class ClientManager;

/// C++ interface for the client
class DLLexport StrategusClient {
public:
	/// Starts the client.
	/// <param name="configFileName">Path to configuration file for the client.</param>
	/// <param name="serverHostname">Hostname of the server to connect to.</param>
	/// <returns>Returns 0 if initialization was successful, 1 if reading config failed and 2 if connection to server failed.</returns>
	int startClient(std::string configFileName, std::string serverHostname);

	///Shutdowns the client.
	void shutdown();
private:
	ClientManager* man;
};