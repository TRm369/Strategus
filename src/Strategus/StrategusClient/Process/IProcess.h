#pragma once
#include <pch.h>

/// Interface abstraction for classes representing subprocesses executing a command in the command line.
class IProcess {
public:
	/// Executes the command.
	/// <param name="command">Command to be executed.</param>
	/// <param name="directory">Directory in which the command should be executed.</param>
	/// <returns>True iff successfully executed.</returns>
	virtual bool execute(const std::string& command, const std::string& directory) = 0;

	/// Returns true if the subprocess is running.
	virtual bool isRunning() = 0;

	/// Kills the process.
	/// <returns>True if successful.</returns>
	virtual bool kill() = 0;
};