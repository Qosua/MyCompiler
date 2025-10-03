#include "NFA.h"

NFA::NFA() {
	newTransiotion();
}

NFA::~NFA() {}

void NFA::addLexem(const std::string& lexem, const LexemType& lexemType) {

	int current = 0;

	for (char c : lexem) {

		if (transitionsTable[current].find(c) == transitionsTable[current].end()) {
			Node node;
			node.nodeNumber = newTransiotion();
			node.lexemToDetectOnNode = lexemType;

			transitionsTable[current][c] = node;
		}
		current = transitionsTable[current][c].nodeNumber;
	}

	return;
}

LexemType NFA::detectLexem(const std::string& lexem) {

	int current = 0;
	LexemType ans = LexemType::Error;

	for (char c : lexem) {

		if (transitionsTable[current].find(c) == transitionsTable[current].end())
			return LexemType::Error;

		ans = transitionsTable[current][c].lexemToDetectOnNode;
		current = transitionsTable[current][c].nodeNumber;
	}

	return ans;
}

int NFA::newTransiotion() {

	transitionsTable.emplace_back();
	return (int)(transitionsTable.size() - 1);
}

IdNFA::IdNFA() {
	newTransiotion();

	Node node;
	node.nodeNumber = 1;
	node.lexemToDetectOnNode = LexemType::ID;

	for (int i = 65; i <= 90; ++i) 
		transitionsTable[0][char(i)] = node;
	for (int i = 97; i <= 122; ++i)
		transitionsTable[0][char(i)] = node;

	for (int i = 65; i <= 90; ++i)
		transitionsTable[1][char(i)] = node;
	for (int i = 97; i <= 122; ++i)
		transitionsTable[1][char(i)] = node;
	for (int i = 0; i < 10; ++i)
		transitionsTable[1][char('0' + i)] = node;

	return;
}

IdNFA::~IdNFA() {}

NumNFA::NumNFA() {
	newTransiotion();

	Node node;
	node.nodeNumber = 0;
	node.lexemToDetectOnNode = LexemType::Constant;

	for (int i = 0; i < 10; ++i)
		transitionsTable[0][char('0' + i)] = node;

	node.nodeNumber = 1;
	transitionsTable[0]['.'] = node;

	for (int i = 0; i < 10; ++i)
		transitionsTable[1][char('0' + i)] = node;

	return;
}

NumNFA::~NumNFA() {}