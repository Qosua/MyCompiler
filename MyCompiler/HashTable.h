#pragma once

#include "MyVector.h"

class HashTable {

public:


};

enum struct LexemType {

	Empty,
	KeyWord,
	ID,
	Separator,
	Constant,
	Operator,
	Error,

};

struct Token {

	LexemType type;
	std::string lexem;

};
