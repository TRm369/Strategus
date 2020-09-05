#pragma once
#include "../StrategusCore/FileManager.h"
#include "../StrategusCore/DataClasses/TaskInfo.h"

class ServerFileManager : private FileManager {
public:
	std::string statFilePath(ID_t jobID, std::string& dir);
	std::string schedulerDescFilePath(std::string& dir);
	std::string getTaskOutputFile(const std::string& jobDir, TaskInfo* ti, uint16 fileIndex);
};

