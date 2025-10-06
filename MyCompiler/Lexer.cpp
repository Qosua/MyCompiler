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

	std::cout << "\n";
	for (std::string& str : *errors) {

		std::cout << str << "\n";

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

			std::cout << "(" + lexem + " | ";

			switch (type)
			{
			case LexemType::Empty: std::cout << "ERROR)\n";
				break;
			case LexemType::KeyWord: std::cout << "KeyWord)\n";
				break;
			case LexemType::ID: std::cout << "ID)\n";
				break;
			case LexemType::Separator: std::cout << "ERROR)\n";
				break;
			case LexemType::Constant: std::cout << "Constant)\n";
				break;
			case LexemType::Operator: std::cout << "ERROR)\n";
				break;
			case LexemType::Error: std::cout << "ERROR)\n";
				break;
			default:
				break;
			}
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

			std::cout << "(" + lexem + " | ";

			switch (type)
			{
			case LexemType::Empty: std::cout << "ERROR)\n";
				break;
			case LexemType::KeyWord: std::cout << "ERROR)\n";
				break;
			case LexemType::ID: std::cout << "ERROR)\n";
				break;
			case LexemType::Separator: std::cout << "Separator)\n";
				break;
			case LexemType::Constant: std::cout << "ERROR)\n";
				break;
			case LexemType::Operator: std::cout << "Operator)\n";
				break;
			case LexemType::Error: std::cout << "ERROR)\n";
				break;
			default:
				break;
			}
		}
	}

	lexem.clear();
	c = char();
}