#include "FileManager.h"
#include <filesystem> //C++17

namespace fs = std::filesystem;

bool FileManager::fileExists(std::string& file) {
    return fs::exists(file);
}

bool FileManager::directoryExists(std::string& directory) {
    return fs::exists(directory);
}

bool FileManager::createDirectory(std::string& directory) {
    return fs::create_directory(directory);
}

bool FileManager::remove(std::string& path) {
    return fs::remove_all(path);
}
