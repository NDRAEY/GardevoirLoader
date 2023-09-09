//
// Created by ndraey on 03.09.23.
//

#pragma once

#include <string>
#include <vector>
#include <dirent.h>

class Directory {
public:

	// Main constructor
	// Throws a std::runtime_error if directory is not exist.
	Directory(const std::string& path);
	~Directory();

	std::vector<std::string> list_file_names();
	std::string get_path();
private:

	const std::string path;
	DIR* directory = nullptr;
};
