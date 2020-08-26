#pragma once
#include "IProcess.h"
#include <windows.h> 

///Windows implementation of IProcess
class WindowsProcess : public IProcess {
public:
	/// Executes the command.
	/// <param name="command">Command to be executed.</param>
	/// <param name="directory">Directory in which the command should be executed. If an empty string is passed, uses current application's directory.</param>
	/// <returns>True iff successfully executed.</returns>
	bool execute(const std::string& command, const std::string& directory);

	/// Returns true if the subprocess is running.
	/// 
	/// In case the status can't be retrieved, returns false.
	bool isRunning();

	/// Kills the process.
	/// <returns>True if successful.</returns>
	bool kill();

	~WindowsProcess();

private:
	//Handles for child process IO
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;

	//Info about the child process
	PROCESS_INFORMATION piProcInfo;
};

