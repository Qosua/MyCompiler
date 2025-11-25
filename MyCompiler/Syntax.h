#pragma once

#include "HashTable.h"

#include <vector>
#include <string>

struct ASTNode {

    std::string Name;
    std::vector<ASTNode*> children;

};

class AST {

public:
    AST();

private:

    struct SimpleExpr;
    struct Expr;
    
    struct Expr {
        int sign = 0; // 0 - nothing, 1 - plus, 2 - minus
        SimpleExpr* simpleExpr;
        Expr* expr = nullptr;
    };

    struct SimpleExpr {
        int state; // -2 - var, -1 - constant, 
                   // 0 - scobes, 1 - itof, 2 - ftoi
        std::string varName;
        std::string Constant;
        Expr* expr;
    };

    std::string BeginName;
    std::vector<std::string> intVars;
    std::vector<std::string> floatVars;
    std::vector<
        std::pair<
            std::string, // goal var
            AST::Expr
        >
    > operations;
    std::string endVariable;

};



class Syntax {

public:
    Syntax();

    void run();
    void setTokens(std::vector<Token> tokens);
    std::vector<std::string> getErrors();
    void printToFile(std::string filePath);

private:
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size_t position = 0;
    Token currentToken;
    ASTNode* tree;

    bool match(LexemType type, std::string val = "", bool toMoveIfFalse = true);
    void error(std::string errorText);

    void parseFunction();
    void parseBegin();
    void parseEnd();
    void parseDescriptions();
    void parseOperators();
    void parseDescr();
    void parseVarList();
    void parseType();
    void parseOp();
    void parseExpr();
    void parseSimpleExpr();

    bool isTypeAhead();
    bool isIDAhead();

};

