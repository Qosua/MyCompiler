#include "Lexer.h"

Lexer::Lexer(const std::string& programmFilePath) {

	file.open(programmFilePath);

	nfa.addLexem("return", LexemType::KeyWord);
	nfa.addLexem("float", LexemType::KeyWord);
	nfa.addLexem("int", LexemType::KeyWord);
	nfa.addLexem("ftoi", LexemType::KeyWord);
	nfa.addLexem("itof", LexemType::KeyWord);
	nfa.addLexem("{", LexemType::Separator);
	nfa.addLexem("}", LexemType::Separator);
	nfa.addLexem("(", LexemType::Separator);
	nfa.addLexem(")", LexemType::Separator);
	nfa.addLexem(",", LexemType::Separator);
	nfa.addLexem(";", LexemType::Separator);
	nfa.addLexem("+", LexemType::Operator);
	nfa.addLexem("-", LexemType::Operator);
	nfa.addLexem("=", LexemType::Operator);

	errors = new std::vector<std::string>;
	table = new HashTable;

}

Lexer::~Lexer() {}

void Lexer::run() {

	std::string tempLexem;
	tempLexem.reserve(1024);

	char c;
	file.get(c);

	while (!file.eof()) {
		switch (c) {
			case '{': processInput(tempLexem, c); break;
			case '}': processInput(tempLexem, c); break;
			case '(': processInput(tempLexem, c); break;
			case ')': processInput(tempLexem, c); break;
			case '-': processInput(tempLexem, c); break;
			case '+': processInput(tempLexem, c); break;
			case '*': processInput(tempLexem, c); break;
			case '/': processInput(tempLexem, c); break;
			case '=': processInput(tempLexem, c); break;
			case ',': processInput(tempLexem, c); break;
			case ';': processInput(tempLexem, c); break;
			case ':': processInput(tempLexem, c); break;
			case ' ': processInput(tempLexem, c); break;
			//case '.': processInput(tempLexem, c); break;
			case '\n': processInput(tempLexem, c); break;
			case '\t': processInput(tempLexem, c); break;
			case '\b': processInput(tempLexem, c); break;
			case '\0': processInput(tempLexem, c); break;
			case '\'': processInput(tempLexem, c); break;
			case '\"': processInput(tempLexem, c); break;
			case '\\': processInput(tempLexem, c); break;
			default: {
				tempLexem += c;
			} break;
		}

		file.get(c);
	}

}

void Lexer::processInput(std::string& lexem, char& c) {

	//String processing
	if (lexem.size() != 0) {

		LexemType type = numNfa.detectLexem(lexem);
		if (type == LexemType::Error) {

			if (isdigit(lexem[0]))
				errors->push_back("Lexem type detection error:\n\t _wrong_constant_symbols_(" + lexem + ")");
			else {

				type = nfa.detectLexem(lexem);
				if (type == LexemType::Error) {

					type = idNfa.detectLexem(lexem);
					if (type == LexemType::Error) {

						errors->push_back("Lexem type detection error:\n\t _wrong_identifier_symbols_(" + lexem + ")");

					}
				}
			}
		}

		if (type != LexemType::Error) {

			std::string temp;
			temp += "(" + lexem + " | ";

			switch (type)
			{
			case LexemType::Empty: temp += "ERROR)\n";
				break;
			case LexemType::KeyWord: temp += "KeyWord)\n";
				break;
			case LexemType::ID: temp += "ID)\n";
				break;
			case LexemType::Separator: temp += "ERROR)\n";
				break;
			case LexemType::Constant: temp += "Constant)\n";
				break;
			case LexemType::Operator: temp += "ERROR)\n";
				break;
			case LexemType::Error: temp += "ERROR)\n";
				break;
			default:
				break;
			}
			table->insert(lexem, type);
			std::cout << temp;
		}
	}

	//Char processing
	lexem = c;
	if (lexem[0] != '\n' and lexem[0] != '\t' and lexem[0] != '\0' and lexem[0] != ' ') {

		LexemType type = nfa.detectLexem(lexem);
		if (type == LexemType::Error) {

			errors->push_back("Lexem type detection error:\n\t _wrong_separator_symbol_(" + lexem + ")");
		}

		if (type != LexemType::Error) {

			std::string temp;
			temp += "(" + lexem + " | ";

			switch (type)
			{
			case LexemType::Empty: temp += "ERROR)\n";
				break;
			case LexemType::KeyWord: temp += "ERROR)\n";
				break;
			case LexemType::ID:temp += "ERROR)\n";
				break;
			case LexemType::Separator: temp += "Separator)\n";
				break;
			case LexemType::Constant: temp += "ERROR)\n";
				break;
			case LexemType::Operator: temp += "Operator)\n";
				break;
			case LexemType::Error: temp += "ERROR)\n";
				break;
			default:
				break;
			}
			table->insert(lexem, type);
			std::cout << temp;
		}
	}

	lexem.clear();
	c = char();
}

HashTable* Lexer::getHashTable() {
	return table;
}

std::vector<std::string>* Lexer::getErrors() {
	return errors;
}

void Lexer::printToFile(const std::string& filePath) {

	std::fstream output;
	output.open(filePath);

	for (int i = 0; i < table->size(); ++i) {
		for (int j = 0; j < (*table)[i].size(); ++j) {

			std::string buffer;

			buffer = "[" + std::to_string(i) + "," + std::to_string(j) + "] (";
			buffer += (*table)[i][j].lexem + "|";

			switch ((*table)[i][j].type) {
				case LexemType::Empty: buffer += "EMPTY - ERROR)"; break;
				case LexemType::KeyWord: buffer += "KeyWord)"; break;
				case LexemType::ID: buffer += "ID)"; break;
				case LexemType::Separator: buffer += "Separator)"; break;
				case LexemType::Constant: buffer += "Constant)"; break;
				case LexemType::Operator: buffer += "Operator)"; break;
				case LexemType::Error: buffer += "ERROR)"; break;
			}
			buffer += "\n";
			output << buffer;
		}
	}

}
