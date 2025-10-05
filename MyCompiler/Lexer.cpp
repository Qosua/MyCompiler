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

}

Lexer::~Lexer() {}

void Lexer::run() {

	std::string tempLexem;
	tempLexem.reserve(1024);

	char c;
	file.get(c);

	while (!file.eof()) {
		
		//related to the Program.txt
		switch (c) {
			case '{': processRelatedInput(tempLexem, c); break;
			case '}': processRelatedInput(tempLexem, c); break;
			case '(': processRelatedInput(tempLexem, c); break;
			case ')': processRelatedInput(tempLexem, c); break;
			case '-': processRelatedInput(tempLexem, c); break;
			case '+': processRelatedInput(tempLexem, c); break;
			case '*': processRelatedInput(tempLexem, c); break;
			case '/': processRelatedInput(tempLexem, c); break;
			case '=': processRelatedInput(tempLexem, c); break;
			case ',': processRelatedInput(tempLexem, c); break;
			case ';': processRelatedInput(tempLexem, c); break;
			case ':': processRelatedInput(tempLexem, c); break;
			//case '.': processRelatedInput(tempLexem, c); break;
		}
		//unrelated to the Program.txt
		switch (c) {
			case ' ': processUnRelatedInput(tempLexem, c); break;
			case '~': processUnRelatedInput(tempLexem, c); break;
			case '<': processUnRelatedInput(tempLexem, c); break;
			case '>': processUnRelatedInput(tempLexem, c); break;
			case '`': processUnRelatedInput(tempLexem, c); break;
			case '!': processUnRelatedInput(tempLexem, c); break;
			case '@': processUnRelatedInput(tempLexem, c); break;
			case '"': processUnRelatedInput(tempLexem, c); break;
			case '#': processUnRelatedInput(tempLexem, c); break;
			case '¹': processUnRelatedInput(tempLexem, c); break;
			case '$': processUnRelatedInput(tempLexem, c); break;
			case '%': processUnRelatedInput(tempLexem, c); break;
			case '^': processUnRelatedInput(tempLexem, c); break;
			case '&': processUnRelatedInput(tempLexem, c); break;
			case '?': processUnRelatedInput(tempLexem, c); break;
			case '\n': processUnRelatedInput(tempLexem, c); break;
			case '\t': processUnRelatedInput(tempLexem, c); break;
			case '\b': processUnRelatedInput(tempLexem, c); break;
			case '\0': processUnRelatedInput(tempLexem, c); break;
			case '\'': processUnRelatedInput(tempLexem, c); break;
			case '\\': processUnRelatedInput(tempLexem, c); break;
			default: {
				tempLexem += c;
			} break;
		}

		file.get(c);
	}

}

void Lexer::processRelatedInput(std::string& lexem, char& c) {

	std::cout << " |+|" << lexem << "|" << (c == '\n' or c == '\t' or c == '\0' ? 'N' : c) << "\n";

	lexem.clear();
	c = char();
	return;
}

void Lexer::processUnRelatedInput(std::string& lexem, char& c) {

	std::cout << " |-|" << lexem << "|" << (c == '\n' or c == '\t' or c == '\0' ? 'N' : c) << "\n";

	lexem.clear();
	c = char();
	return;
}
