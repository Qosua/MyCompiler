#pragma once

#include "Lexer.h"
#include "Syntax.h"
#include "HashTable.h"

int main() {

    Lexer lexer("Program.txt");
    Syntax syntax;

    lexer.run();

    syntax.setTokens(lexer.getTokens());
    syntax.run();

    std::cout << "\n-----------------------------------------------------------------\n\n";

    for (auto elem : lexer.getErrors())
        std::cout << elem << "\n";
    
    std::cout << "\n";

    for (auto elem : syntax.getErrors())
        std::cout << elem << "\n";
    
    std::cout << "\n";

    lexer.printToFile("lexemes.txt");
    //syntax.printToFile("AST.txt");

    return 0;

}