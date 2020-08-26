#pragma once
#include <string>

/// Base class providing basic functionality for client and server versions.
class FileManager {
protected:
	/// Checks if a file exists.
	bool fileExists(std::string& file);

	/// Checks if a directory exists.
	bool directoryExists(std::string& directory);

	/// Creates a directory.
	/// <returns>True if the directory was created.</returns>
	bool createDirectory(std::string& directory);

	/// Removes a file or directory. Contents of a directory are also deleted.
	/// <returns>True if the file/directory was deleted.</returns>
	bool remove(std::string& path);
};