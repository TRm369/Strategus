#pragma once
#include "IProcess.h"
#include <windows.h> 

///Windows implementation of IProcess
class WindowsProcess : public IProcess {
public:
	/// Executes the command.
	bool execute(std::string command);

	/// Returns true if the subprocess is running.
	/// 
	/// In case the status can't be retrieved, returns false.
	bool isRunning();

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

