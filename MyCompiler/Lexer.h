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
	void processRelatedInput(std::string& lexem, char& c);
	void processUnRelatedInput(std::string& lexem, char& c);
	HashTable* getHashTable();

private:
	std::fstream file;

	NFA nfa;
	IdNFA idNfa;
	NumNFA numNfa;
	
};
