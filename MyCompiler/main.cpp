#pragma once

#include "Lexer.h"
#include "HashTable.h"

int main() {

    Lexer lexer("Program.txt");
    lexer.run();

    HashTable table;
    table.insert("text", LexemType::KeyWord);
    table.insert("text", LexemType::KeyWord);
    table.insert("text", LexemType::KeyWord);
    table.insert("test", LexemType::KeyWord);
    table.insert("asd", LexemType::KeyWord);
    table.insert("sadsa", LexemType::KeyWord);

    table << std::cout;
    std::cout << "---";

    return 0;

}