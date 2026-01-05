#pragma once

#include <windows.h>

#include "Lexer.h"
#include "Syntax.h"
#include "Semantics.h"
#include "HashTable.h"

int main() {

    Lexer lexerAndSyntax("Program.txt");
    Semantics semantics;

    lexerAndSyntax.run();

    semantics.setTree(lexerAndSyntax.syntax.getTree());
    semantics.run();
    semantics.printToFile("MainOutput.txt");
    semantics.printErrors();


    return 0;

}