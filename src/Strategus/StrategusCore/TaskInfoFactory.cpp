#include "TaskInfoFactory.h"
#include <string>

TaskInfoFactory::TaskInfoFactory(IMemoryManager* memManager) {
    this->memManager = memManager;
}

/*Structure of TaskInfo object in memory:
* Part:   | TaskInfo class   | name             | command           | inputFile* arr                 | inputFiles arr | outputFile* arr                 | outputFiles arr |
* Size:   | sizeof(TaskInfo) | strlen(name) + 1 | strlen(command)+1 | sizeof(char*) * inputFileCount | sum of strlens | sizeof(char*) * outputFileCount | sum of strlens  |
*/
TaskInfo* TaskInfoFactory::createTaskInfo(doubleID_t ID, const char* name, const char* command, uint16 inputFileCount, const char** inputFiles,
                                          uint16 outputFileCount, const char** outputFiles) {
    uint32 objectSize = sizeof(TaskInfo);
    
    //Calculate size of the final object
    //strlen returns the lenght of the string without the null terminator, but it has to be included too. So +1 has to be added for all strings.
    objectSize += strlen(name) + 1;
    objectSize += strlen(command) + 1;
    objectSize += sizeof(char*) * inputFileCount;
    for (int i = 0; i < inputFileCount; i++) {
        objectSize += strlen(inputFiles[i]) + 1;
    }
    objectSize += sizeof(char*) * outputFileCount;
    for (int i = 0; i < outputFileCount; i++) {
        objectSize += strlen(outputFiles[i]) + 1;
    }

    //Allocate memory for it
    uint8* ptr = memManager->getMemoryBlock(objectSize);
    TaskInfo* ti = (TaskInfo*)ptr;

    //Copy the data
    ti->ID = ID;
    ti->inputFileCount = inputFileCount;
    ti->outputFileCount = outputFileCount;
    ti->size = objectSize;

    size_t size = 0;
    //Copy name and command
    ptr += sizeof(TaskInfo);
    size = strlen(name) + 1;
    memcpy(ptr, name, size);
    ti->name = (char*)ptr;
    ptr += size;
    size = strlen(command) + 1;
    memcpy(ptr, command, size);
    ti->command = (char*)ptr;
    ptr += size;

    //Copy input file names
    char** inputFileArr = (char**)ptr;
    ptr += sizeof(char*) * inputFileCount;
    for (int i = 0; i < inputFileCount; i++) {
        size = strlen(inputFiles[i]) + 1;
        inputFileArr[i] = (char*)ptr;
        memcpy(ptr, inputFiles[i], size);
        ptr += size;
    }
    ti->inputFiles = inputFileArr;

    //Copy output file names
    char** outputFileArr = (char**)ptr;
    ptr += sizeof(char*) * outputFileCount;
    for (int i = 0; i < outputFileCount; i++) {
        size = strlen(outputFiles[i]) + 1;
        outputFileArr[i] = (char*)ptr;
        memcpy(ptr, outputFiles[i], size);
        ptr += size;
    }
    ti->outputFiles = outputFileArr;

    //Return pointer to the instance
    return ti;
}

void TaskInfoFactory::destroyTaskInfo(TaskInfo* ti) {
    memManager->releaseMemoryBlock((uint8*)ti);
}
