#pragma once

#include "HashTable.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

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

class AST {

public:
    AST() {};
    AST(const AST& obj) {
        *this = obj;
    };

    void operator=(const AST& obj);

    std::string BeginName;
    std::string beginType;
    HashTable* table;
    std::vector<
        std::pair<
            std::string, // goal var
            Expr
        >
    > operations;
    std::string endVariable;
    std::fstream globalOutput;

    void printToConsole();
    void printExpr(Expr* expr, int level = 0);
    void printSimpleExpr(SimpleExpr* expr, int level = 0);
    void put(int level);

    void printToFileTree();
    void printExprFile(Expr* expr, int level = 0);
    void printSimpleExprFile(SimpleExpr* expr, int level = 0);
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
    std::vector<Token> tokens;
    std::vector<std::string> errors;

    SyntaxState currentState = SyntaxState::Begin;
    size_t position = 0;

    AST* tree;
    // -------------------tree parts-------------------
    std::string beginName;
    std::string beginType;
    HashTable* table;
    std::vector<
        std::pair<
        std::string, // goal var
        Expr
        >
    > operations;
    std::string endVariable;
    std::fstream globalOutput;
    // -------------------tree parts-------------------

    std::string lastVarType;
    std::string lastVarName;

    std::string kostyl;

    bool match(LexemType type, std::string val = "", bool toMoveIfFalse = true);
    void error(std::string errorText);
    std::string idType(std::string name);
    std::string constantType(std::string name);

    bool parseBegin();
    bool parseEnd();
    bool parseDescriptions();
    bool parseOperators();
    bool parseDescr();
    bool parseVarList();
    bool parseType();
    bool parseOp();
    bool parseExpr(Expr* parent = nullptr);
    bool parseSimpleExpr(SimpleExpr* parent = nullptr);

    std::string& previousLexem();

    void assembleTree();
    void clearBuffer();

};

