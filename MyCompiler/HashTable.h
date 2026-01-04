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
	int rowNumber = 0;
	int isInt = 0;

};

class HashTable {

public:
	HashTable();
	HashTable(const HashTable& obj);
	~HashTable();

	void operator= (const HashTable& obj);

	void insert(const std::string& lexem, const LexemType& type);
	void erase(const std::string& lexem);

	Token& operator[](const std::string& lexem);
	MyVector<Token>& operator[](const int& index);
	std::ostream& operator<<(std::ostream& stream);

	bool exists(const std::string& lexem);

	int hashFunction(const std::string& lexem);
	size_t size();

private:
	MyVector<MyVector<Token>>* arr;
	const int prime_const = 3;

};
