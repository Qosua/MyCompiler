#pragma once

#include "HashTable.h"

#include <string>
#include <vector>
#include <unordered_map>

struct Node {
	int nodeNumber;
	LexemType lexemToDetectOnNode;
};

//It is General NFA for any finite words set
class NFA {

public:
	NFA();
	~NFA();

	void addLexem(const std::string& lexem, const LexemType& lexemType);
	LexemType detectLexem(const std::string& lexem);

	int newTransiotion();

protected:
	std::vector<std::unordered_map<char, Node>> transitionsTable;

};

//I defenitly not fish anymore
//It is NFA only for IDs
class IdNFA : public NFA {

public:
	IdNFA();
	~IdNFA();

};

//It is NFA only for Number detection
class NumNFA : public NFA {

public:
	NumNFA();
	~NumNFA();

};

