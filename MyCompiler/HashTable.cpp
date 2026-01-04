#include "HashTable.h"

HashTable::HashTable() {

	arr = new MyVector<MyVector<Token>>;

	arr->reserve(512);

	for (int i = 0; i < 512; ++i) {
		MyVector<Token> temp;
		temp.reserve(128);

		arr->pushBack(temp);

		/*arr->pushBack(MyVector<Token>());
		arr[i].reserve(128);*/
	}

}

HashTable::HashTable(const HashTable& obj)
{
	this->arr = obj.arr;
}

HashTable::~HashTable() {}

void HashTable::operator=(const HashTable& obj)
{
	this->arr = obj.arr;
}

void HashTable::insert(const std::string& lexem,const LexemType& type) {

	int index = hashFunction(lexem);

	for (int i = 0; i < (*arr)[index].size(); ++i) {
		if ((*arr)[index][i].lexem == lexem)
			return;
	}

	(*arr)[index].pushBack(Token(lexem, type));

}

void HashTable::erase(const std::string& lexem) {

}

Token& HashTable::operator[](const std::string& lexem) {

	int index = hashFunction(lexem);

	for (int i = 0; i < (*arr)[index].size(); ++i) {

		if ((*arr)[index][i].lexem == lexem)
			return (*arr)[index][i];
	}

	Token token;
	return token;
}

MyVector<Token>& HashTable::operator[](const int& index) {
	return (*arr)[index];
}

std::ostream& HashTable::operator<<(std::ostream& stream) {

	for (int i = 0; i < (*arr).size(); ++i) {
		for (int j = 0; j < (*arr)[i].size(); ++j) {

			std::string buffer;

			buffer = "[" + std::to_string(i) + "," + std::to_string(j) + "] (";
			buffer += (*arr)[i][j].lexem + "|";
			
			switch ((*arr)[i][j].type) {
			case LexemType::Empty: buffer += "EMPTY - ERROR)"; break;
			case LexemType::KeyWord: buffer += "KeyWord)"; break;
			case LexemType::ID: buffer += "ID)"; break;
			case LexemType::Separator: buffer += "Separator)"; break;
			case LexemType::Constant: buffer += "Constant)"; break;
			case LexemType::Operator: buffer += "Operator)"; break;
			case LexemType::Error: buffer += "ERROR)"; break;
			}
			buffer += "\n";
			stream << buffer;
		}
	}

	return stream;
}

bool HashTable::exists(const std::string& lexem) {

	int hash = hashFunction(lexem);

	for (int i = 0; i < (*arr)[hash].size(); ++i) {
		
		if ((*arr)[hash][i].lexem == lexem)
			return true;

	}

	return false;
}

int HashTable::hashFunction(const std::string& lexem) {
	int hashCode = 0;
	for (int i = 0; i < lexem.size(); i++) {
		hashCode += lexem[i] * pow(prime_const, i);
	}
	return hashCode % (*arr).size();
}

size_t HashTable::size() {
	return arr->size();
}
