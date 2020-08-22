#pragma once
#include <pch.h>

/// Interface abstraction for classes representing subprocesses executing a command in the command line.
class IProcess {
public:
	/// Executes the command.
	virtual bool execute(std::string command) = 0;

	/// Returns true if the subprocess is running.
	virtual bool isRunning() = 0;
};