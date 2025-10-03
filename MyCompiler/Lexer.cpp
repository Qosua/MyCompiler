#include "Lexer.h"

Lexer::Lexer(const std::string& programmFilePath) {
	file.open(programmFilePath);

	transitionsTable.emplace_back();

	//addKeyWord("return");

}

Lexer::~Lexer() {}

void Lexer::run() {

	/*std::string tempLexem;
	tempLexem.reserve(1024);

	while (!file.eof()) {

		char c = ' ';
		file.get(c);

		switch (c) {
			case '{': recogniseLexem(tempLexem, c); break;
			case '}': recogniseLexem(tempLexem, c); break;
			case '(': recogniseLexem(tempLexem, c); break;
			case ')': recogniseLexem(tempLexem, c); break;
			case '[': recogniseLexem(tempLexem, c); break;
			case ']': recogniseLexem(tempLexem, c); break;
			case '/': recogniseLexem(tempLexem, c); break;
			case '\\': recogniseLexem(tempLexem, c); break;
			case '\n': recogniseLexem(tempLexem, c); break;
			case ',': recogniseLexem(tempLexem, c); break;
			case '<': recogniseLexem(tempLexem, c); break;
			case '>': recogniseLexem(tempLexem, c); break;
			case '&': recogniseLexem(tempLexem, c); break;
			case '*': recogniseLexem(tempLexem, c); break;
			case '?': recogniseLexem(tempLexem, c); break;
			case ':': recogniseLexem(tempLexem, c); break;
			case ';': recogniseLexem(tempLexem, c); break;
			case '~': recogniseLexem(tempLexem, c); break;
			case '`': recogniseLexem(tempLexem, c); break;
			case '@': recogniseLexem(tempLexem, c); break;
			case '#': recogniseLexem(tempLexem, c); break;
			case '$': recogniseLexem(tempLexem, c); break;
			case '^': recogniseLexem(tempLexem, c); break;
			case '%': recogniseLexem(tempLexem, c); break;
		default: {
			tempLexem += c;
		}break;
		}

		tempLexem += c;

	}*/

}
