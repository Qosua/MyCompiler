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

    auto table = lexer.getHashTable();

    syntax.setTable(table);
    syntax.setTokens(lexer.getTokens());
    syntax.run();
    syntax.printToFile();

    auto tree = syntax.getTree();

    semantics.setTree(tree);
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