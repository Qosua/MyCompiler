#pragma once

#include "Lexer.h"
#include "HashTable.h"

int main() {

    Lexer lexer("Program.txt");
    lexer.run();

    std::cout << "\n-----------------------------------------------------------------\n\n";

    *lexer.getHashTable() << std::cout;

    std::cout << "\n-----------------------------------------------------------------\n\n";

    for (auto elem : *lexer.getErrors()) {
        std::cout << elem << "\n";
    }
    std::cout << "\n";

    lexer.printToFile("lexemes.txt");

    return 0;

}