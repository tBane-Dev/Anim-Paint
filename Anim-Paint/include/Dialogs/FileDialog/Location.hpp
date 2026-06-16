#pragma once
#include <filesystem>


class Location {
public:
	std::filesystem::path _path;
	std::vector<std::shared_ptr<Location>> _children;

	bool _isOpen = false;
	bool _hasChildren = false;
	int _depth = 0;

	Location(std::filesystem::path path, int depth = 0);
	~Location();

	void open();
	void close();
};