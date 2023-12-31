//
// Created by ndraey on 03.09.23.
//

#include <iostream>
#include "../include/Directory.hpp"

Directory::Directory(const std::string &path)
: path(path) {
	directory = opendir(this->path.c_str());

	if(!directory) {
		throw std::runtime_error("Could not open directory: " + path);
	}
}

std::vector<std::string> Directory::list_file_names() {
	std::vector<std::string> result = {};

	// If we want to call this method multiple times.
	rewinddir(directory);

	dirent* entry = readdir(directory);

	while(entry) {
		result.emplace_back(entry->d_name);

		entry = readdir(directory);
	}

	return result;
}

Directory::~Directory() {
	closedir(directory);
}

std::string Directory::get_path() {
	return path;
}
