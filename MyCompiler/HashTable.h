#pragma once

#include "MyVector.h"

#include <string>
#include <ostream>

enum LexemType {
	Empty,
	KeyWord,
	ID,
	Separator,
	Constant,
	Operator,
	Error,
};

struct Token {

	Token(const std::string& str, LexemType typ) {
		type = typ;
		lexem = str;
	}
	Token() {};
	~Token() {};

	Token operator=(const Token& token) {
		this->lexem = token.lexem;
		this->type = token.type;
		return *this;
	}

	LexemType type = LexemType::Empty;
	std::string lexem = "__error__";

};

class HashTable {

public:
	HashTable();
	~HashTable();

	void insert(const std::string& lexem, const LexemType& type);
	void erase(const std::string& lexem);

	Token& operator[](const std::string& lexem);
	std::ostream& operator<<(std::ostream& stream);

	int hashFunction(const std::string& lexem);

private:
	MyVector<MyVector<Token>>* arr;
	const int prime_const = 19;

};
