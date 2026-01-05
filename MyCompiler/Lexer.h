#pragma once

#include "HashTable.h"
#include "NFA.h"
#include "Syntax.h"

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
	std::vector<std::string> getErrors();
	void printToFile(const std::string& filePath);

	Syntax syntax;

private:
	std::fstream file;
	HashTable* table;
	std::vector<std::string>* errors;
	int rowCount = 1;

	NumNFA numNfa;
	NFA nfa;
	IdNFA idNfa;
	
};
