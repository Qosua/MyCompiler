#pragma once

#include "HashTable.h"

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

class Lexer {

public:
	Lexer(const std::string& programmFilePath);
	~Lexer();

	void run();
	HashTable* getHashTable();

private:
	std::vector<std::unordered_map<char, int>> transitionsTable;
	int startTransitionIndex = 0;
	std::fstream file;
	
};
