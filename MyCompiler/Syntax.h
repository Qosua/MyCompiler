#pragma once

#include "HashTable.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

class AST {

public:
    AST() {};
    AST(const AST& obj) {
        *this = obj;
    };

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

    void operator=(const AST& obj);

    std::string BeginName;
    std::string beginType;
    HashTable* table;
    std::vector<
        std::pair<
            std::string, // goal var
            AST::Expr
        >
    > operations;
    std::string endVariable;
    std::fstream globalOutput;

    void printToConsole();
    void printExpr(AST::Expr* expr, int level = 0);
    void printSimpleExpr(AST::SimpleExpr* expr, int level = 0);
    void put(int level);

    void printToFileTree();
    void printExprFile(AST::Expr* expr, int level = 0);
    void printSimpleExprFile(AST::SimpleExpr* expr, int level = 0);
    void putFile(int level);

    std::string findVarType(std::string name);
    std::string findConstType(std::string name);
    void setTable(HashTable* table) {
        this->table = table;
    }

};

enum SyntaxState {
    Begin,
    Descr,
    Operators,
    End,
    Finished,
};

class Syntax {

public:
    Syntax();

    void run();
    std::vector<std::string> getErrors();
    void printToFile();
    void printToConsole();
    void setTable(HashTable* table) {
        this->table = table;
        this->tree->table = table;
    }
    AST* getTree();
    void processToken(Token token);

private:
    SyntaxState currentState = SyntaxState::Begin;
    std::vector<Token> tokens;
    std::vector<std::string> errors;
    size_t position = 0;
    Token currentToken;
    AST* tree;
    HashTable* table;
    std::string lastVarType;
    std::string lastVarName;

    bool match(LexemType type, std::string val = "", bool toMoveIfFalse = true);
    void error(std::string errorText);

    void parseFunction();
    bool parseBegin();
    bool parseEnd();
    bool parseDescriptions();
    bool parseOperators();
    bool parseDescr();
    bool parseVarList();
    bool parseType();
    bool parseOp();
    bool parseExpr(AST::Expr* parent = nullptr);
    bool parseSimpleExpr(AST::SimpleExpr* parent = nullptr);

    bool isTypeAhead();
    bool isIDAhead();

    void clearBuffer();

};

