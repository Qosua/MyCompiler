#include "NFA.h"

NFA::NFA() {
	newTransiotion();
}

NFA::~NFA() {}

void NFA::addLexem(const std::string& lexem, const LexemType& lexemType) {

	int current = 0;

	for (int i = 0; i < lexem.size();++i) {

		if (transitionsTable[current].find(lexem[i]) == transitionsTable[current].end()) {

			Node node;
			node.nodeNumber = newTransiotion();
			node.lexemToDetectOnNode = lexemType;

			if (i == lexem.size() - 1)
				node.isFinishNode = true;

			transitionsTable[current][lexem[i]] = node;
		}
		current = transitionsTable[current][lexem[i]].nodeNumber;
	}

	return;
}

LexemType NFA::detectLexem(const std::string& lexem) {

	int current = 0;
	bool nodeState = true;
	LexemType ans = LexemType::Error;

	for (char c : lexem) {

		if (transitionsTable[current].find(c) == transitionsTable[current].end())
			return LexemType::Error;

		nodeState = transitionsTable[current][c].isFinishNode;
		ans = transitionsTable[current][c].lexemToDetectOnNode;
		current = transitionsTable[current][c].nodeNumber;
	}

	if(nodeState)
		return ans;
	else
		return LexemType::Error;
}

int NFA::newTransiotion() {

	transitionsTable.emplace_back();
	return (int)(transitionsTable.size() - 1);
}

IdNFA::IdNFA() {
	newTransiotion();

	Node node;
	node.nodeNumber = 0;
	node.lexemToDetectOnNode = LexemType::ID;
	node.isFinishNode = true;

	for (int i = 65; i <= 90; ++i) 
		transitionsTable[0][char(i)] = node;
	for (int i = 97; i <= 122; ++i)
		transitionsTable[0][char(i)] = node;

	return;
}

IdNFA::~IdNFA() {}

NumNFA::NumNFA() {
	newTransiotion();
	newTransiotion();
	newTransiotion();
	newTransiotion();

	Node node;
	node.nodeNumber = 1;
	node.lexemToDetectOnNode = LexemType::Constant;
	node.isFinishNode = true;

	transitionsTable[0]['0'] = node;

	node.nodeNumber = 2;
	transitionsTable[1]['.'] = node;

	for (int i = 0; i < 10; ++i)
		transitionsTable[2][char('0' + i)] = node;

	node.nodeNumber = 3;
	for (int i = 0; i < 9; ++i)
		transitionsTable[0][char('1' + i)] = node;

	for (int i = 0; i < 10; ++i)
		transitionsTable[3][char('0' + i)] = node;

	node.nodeNumber = 4;
	transitionsTable[3]['.'] = node;

	for (int i = 0; i < 10; ++i)
		transitionsTable[4][char('0' + i)] = node;

	return;
}

NumNFA::~NumNFA() {}