#include "pch.h"

//Mostly stolen from https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output

#ifdef OS_WINDOWS
#include "WindowsProcess.h"

bool WindowsProcess::execute(std::string command) {
	//Setup for starting child process
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
		return false;

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		return false;

	// Create a pipe for the child process's STDIN. 
	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
		return false;

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
		return false;

	// Create the child process.

	//Setup
	//start the command with the cmd executable so that the complete process is "cmd.exe /c [command]"
	std::string fullCmd = "C:\\Windows\\System32\\cmd.exe /c " + command;
	size_t length = fullCmd.length();
	TCHAR* wideCommand = new TCHAR[length+1];
	mbstowcs_s(nullptr, wideCommand, length+1, fullCmd.c_str(), length);
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 
	bSuccess = CreateProcess(NULL,
		wideCommand,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION

	if (!bSuccess)
		return false;
	else {
		// Close handles to the stdin and stdout pipes no longer needed by the child process.
		// If they are not explicitly closed, there is no way to recognize that the child process has ended.
		CloseHandle(g_hChildStd_OUT_Wr);
		CloseHandle(g_hChildStd_IN_Rd);
	}
}

bool WindowsProcess::isRunning() {
	return true;
}

WindowsProcess::~WindowsProcess() {
	// Close handles to the child process and its primary thread.
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);

	//Close the rest of the handles to prevent resource leak.
	CloseHandle(g_hChildStd_IN_Wr);
	CloseHandle(g_hChildStd_OUT_Rd);
}

#endif