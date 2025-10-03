#pragma once

#include "NFA.h"

int main() {
    
    NumNFA nfa;

    std::vector<std::string> tests = {
        "12123", 
        "123123.", 
        "0123", 
        "0.123", 
        "0.1123,123",
        "0 1123 123",
        "01a",
        "-01",//
        "00004",
        "\n00004",
        "",
        " ",
        "2131.123123.1233",
        ".",//
    };
    
    for (std::string test : tests) {

        LexemType tupe = nfa.detectLexem(test);

        if (tupe != LexemType::Error)
            std::cout << test << " - OKAY\n";
        else
            std::cout << test << " - NOT OKAY\n";

    }

    return 0;

}