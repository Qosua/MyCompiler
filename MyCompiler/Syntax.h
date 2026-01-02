#pragma once

#include "HashTable.h"

#include <vector>
#include <iostream>
#include <string>

class AST {

public:
    AST() {};

    struct SimpleExpr;
    struct Expr;
    
    struct Expr {
        int sign = 0; // 0 - only expr1, 1 - plus, 2 - minus
        SimpleExpr* expr1 = nullptr;
        Expr* expr2 = nullptr;
    };

    struct SimpleExpr {
        int state; // -2 - var, -1 - constant, 
                   // 0 - scobes, 1 - itof, 2 - ftoi
        std::string varName;
        std::string constant;
        std::string type;
        Expr* expr = nullptr;
    };

    std::string BeginName;
    std::string beginType;
    std::vector<std::string> intVars;
    std::vector<std::string> floatVars;
    std::vector<
        std::pair<
            std::string, // goal var
            AST::Expr
        >
    > operations;
    std::string endVariable;

    void printToConsole();
    void printExpr(AST::Expr* expr, int level = 0);
    void printSimpleExpr(AST::SimpleExpr* expr, int level = 0);
    void put(int level);
    std::string findVarType(std::string name);
    std::string findConstType(std::string name);

};



class Syntax {

public:
    Syntax();

    void run();
    void setTokens(std::vector<Token> tokens);
    std::vector<std::string> getErrors();
    void printToFile(std::string filePath);
    void printToConsole();
    AST getTree();

private:
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size_t position = 0;
    Token currentToken;
    AST tree;
    std::string lastVarType;
    std::string lastVarName;

    bool match(LexemType type, std::string val = "", bool toMoveIfFalse = true);
    void error(std::string errorText);

    void parseFunction();
    void parseBegin();
    void parseEnd();
    void parseDescriptions();
    void parseOperators();
    void parseDescr();
    void parseVarList();
    bool parseType();
    void parseOp();
    void parseExpr(AST::Expr* parent = nullptr);
    void parseSimpleExpr(AST::SimpleExpr* parent = nullptr);

    bool isTypeAhead();
    bool isIDAhead();

};

