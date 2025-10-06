#pragma once

#include "HashTable.h"
#include "NFA.h"

#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

class Lexer {

public:
	Lexer(const std::string& programmFilePath);
	~Lexer();

	void run();
	void processInput(std::string& lexem, char& c);
	HashTable* getHashTable();

private:
	std::fstream file;
	HashTable* table;
	std::vector<std::string>* errors;

	NumNFA numNfa;
	NFA nfa;
	IdNFA idNfa;
	
};
