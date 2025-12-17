#pragma once

#include <windows.h>

#include "Lexer.h"
#include "Syntax.h"
#include "Semantics.h"
#include "HashTable.h"

int main() {

    Lexer lexer("Program.txt");
    Syntax syntax;
    Semantics semantics;

    lexer.run();

    syntax.setTokens(lexer.getTokens());
    syntax.run();
    syntax.printToConsole();

    semantics.setTree(syntax.getTree());
    semantics.run();
    semantics.printToFile("MainOutput.txt");
    semantics.printErrors();

    /*std::cout << "\n-----------------------------------------------------------------\n\n";

    for (auto elem : lexer.getErrors())
        std::cout << elem << "\n";
    
    std::cout << "\n";

    for (auto elem : syntax.getErrors())
        std::cout << elem << "\n";
    
    std::cout << "\n";*/

    //lexer.printToFile("lexemes.txt");
    //syntax.printToConsole();


    return 0;

}