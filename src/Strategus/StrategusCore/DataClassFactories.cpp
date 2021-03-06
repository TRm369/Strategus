#include "DataClassFactories.h"
#include <string>

//////////////////
// TaskInfoFactory

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

/////////////////
// JobInfoFactory

JobInfoFactory::JobInfoFactory(IMemoryManager* memMan) {
    memManager = memMan;
}

/*Structure of JobInfo object in memory:
* Part:   | JobInfo class   | name             | file* arr                 | files arr      |
* Size:   | sizeof(JobInfo) | strlen(name) + 1 | sizeof(char*) * fileCount | sum of strlens |
*/

JobInfo* JobInfoFactory::createJobInfo(ID_t ID, ID_t userID, const char* name, uint16 fileCount, const char** files) {
    uint32 objectSize = sizeof(JobInfo);

    //Calculate size of the final object
    //strlen returns the lenght of the string without the null terminator, but it has to be included too. So +1 has to be added for all strings.
    objectSize += strlen(name) + 1;
    objectSize += sizeof(char*) * fileCount;
    for (int i = 0; i < fileCount; i++) {
        objectSize += strlen(files[i]) + 1;
    }

    //Allocate memory for it
    uint8* ptr = memManager->getMemoryBlock(objectSize);
    JobInfo* ji = (JobInfo*)ptr;

    //Copy the data
    ji->jobID = ID;
    ji->userID = userID;
    ji->fileCount = fileCount;
    ji->size = objectSize;

    size_t size = 0;
    //Copy name
    ptr += sizeof(JobInfo);
    size = strlen(name) + 1;
    memcpy(ptr, name, size);
    ji->name = (char*)ptr;
    ptr += size;

    //Copy file names
    char** fileArr = (char**)ptr;
    ptr += sizeof(char*) * fileCount;
    for (int i = 0; i < fileCount; i++) {
        size = strlen(files[i]) + 1;
        fileArr[i] = (char*)ptr;
        memcpy(ptr, files[i], size);
        ptr += size;
    }
    ji->files = fileArr;

    //Return pointer to the instance
    return ji;
}

void JobInfoFactory::destroyJobInfo(JobInfo* ji) {
    memManager->releaseMemoryBlock((uint8*)ji);
}

////////////////////
// ClientInfoFactory

ClientInfoFactory::ClientInfoFactory(IMemoryManager* memMan) {
    memManager = memMan;
}

/*Structure of ClientInfo object in memory:
* Part:   | ClientInfo class   | flags arr                  |
* Size:   | sizeof(ClientInfo) | flagCount * sizeof(uint64) |
*/

ClientInfo* ClientInfoFactory::createClientInfo(ID_t ID, uint16 flagCount, uint64* flags) {
    size_t size = sizeof(ClientInfo) + flagCount * sizeof(uint64);
    uint8* ptr = memManager->getMemoryBlock(size);
    ClientInfo* ci = (ClientInfo*)ptr;

    ptr += sizeof(ClientInfo);
    memcpy(ptr, flags, flagCount * sizeof(uint64));

    ci->clientID = ID;
    ci->flagCount = flagCount;
    ci->flags = (uint64*)ptr;
    ci->size = size;

    return ci;
}

void ClientInfoFactory::destroyClientInfo(ClientInfo* ci) {
    memManager->releaseMemoryBlock((uint8*)ci);
}
