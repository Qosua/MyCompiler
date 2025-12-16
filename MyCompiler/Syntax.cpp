#include "Syntax.h"

Syntax::Syntax() {}

void Syntax::run() {

    if (position < tokens.size()) {
        currentToken = tokens[position];
    }

    parseFunction();
}

void Syntax::setTokens(std::vector<Token> tokens) {

    this->tokens = tokens;

}

std::vector<std::string> Syntax::getErrors() {
    return errors;
}

void Syntax::printToFile(std::string filePath) {

}

void Syntax::printToConsole() {
    tree.printToConsole();
}

AST Syntax::getTree()
{
    return tree;
}

bool Syntax::match(LexemType type, std::string val, bool toMoveIfFalse) {
    if (currentToken.type == type and (val == "" or currentToken.lexem == val)) {

        if (position + 1 < tokens.size()) {
            currentToken = tokens[++position];
        }

        return true;
    }

    if (toMoveIfFalse and position + 1 < tokens.size()) {
        currentToken = tokens[++position];
    }
    return false;
}

bool Syntax::parseType() {

    if (match(LexemType::KeyWord, "int", false))
        return true;
    
    if (match(LexemType::KeyWord, "float", false))
        return true;
    
    error("Expected type. int or float");
    return false;
}

void Syntax::error(std::string errorText) {

    errors.push_back("Syntax error - row:" + std::to_string(tokens[position].rowNumber) + " \n\t" + errorText + "\n");

}

void Syntax::parseFunction() {

    parseBegin(); // +
    parseDescriptions(); // +
    parseOperators(); // +
    parseEnd(); // +
}

void Syntax::parseBegin() {

    if (parseType()) 
        tree.beginType = tokens[position - 1].lexem;

    if (!match(LexemType::ID)) 
        error("Expected function name");
    else {
        tree.BeginName = tokens[position - 1].lexem;
    }
    
    if (!match(LexemType::Separator, "(")) 
        error("Expected '('");
    if (!match(LexemType::Separator, ")")) 
        error("Expected ')'");
    if (!match(LexemType::Separator, "{")) 
        error("Expected '{'");
}

void Syntax::parseEnd() {

    if (!match(LexemType::KeyWord, "return")) error("Expected return key word");
    
    if (!match(LexemType::ID))
        error("Expected some ID at the end");
    else
        tree.endVariable = tokens[position - 1].lexem;

    if (!match(LexemType::Separator, ";")) error("Expected ';'");
    if (!match(LexemType::Separator, "}")) error("Expected '}'");
}

void Syntax::parseDescriptions() {

    while (isTypeAhead())
        parseDescr();
    
}

void Syntax::parseOperators() {

    while (isIDAhead())
        parseOp();
    
}

void Syntax::parseDescr() {

    if (parseType())
        lastVarType = tokens[position - 1].lexem;
    
    parseVarList();

    if (!match(LexemType::Separator, ";")) 
        error("Expected ';'");
}

void Syntax::parseVarList() {

    if (!match(LexemType::ID)) 
        error("Expected some ID");
    else {

        if (lastVarType == "int")
            tree.intVars.push_back(tokens[position - 1].lexem);
        if (lastVarType == "float")
            tree.floatVars.push_back(tokens[position - 1].lexem);

    }

    if (currentToken.lexem == ";")
        return;

    if (match(LexemType::Separator, ",", false)) {
        parseVarList();
    }
}

void Syntax::parseOp() {

    if (!match(LexemType::ID))
        error("Expected ID in operator");
    else {
        tree.operations.emplace_back();
        tree.operations.back().first = tokens[position - 1].lexem;
    }

    if (!match(LexemType::Operator, "="))
        error("Expected '='");

    parseExpr(&tree.operations.back().second);

    if (!match(LexemType::Separator, ";"))
        error("Expected ';'");
}

void Syntax::parseExpr(AST::Expr* parent) {

    AST::SimpleExpr* simpleExpr1 = new AST::SimpleExpr;
    parent->expr1 = simpleExpr1;
    parseSimpleExpr(simpleExpr1);

    while (match(LexemType::Operator, "+", false) or match(LexemType::Operator, "-", false)) {

        parent->sign = (tokens[position - 1].lexem == "+" ? 1 : 2);
        AST::SimpleExpr* simpleExpr2 = new AST::SimpleExpr;
        parent->expr2 = simpleExpr2;
        parseSimpleExpr(simpleExpr2);
    }
    
}

void Syntax::parseSimpleExpr(AST::SimpleExpr* parent) {

    if (match(LexemType::ID, "", false)) {
        parent->state = -2;
        parent->varName = tokens[position - 1].lexem;
        parent->type = tree.findVarType(parent->varName);
        return;
    }

    if (match(LexemType::Constant, "", false)) {
        parent->state = -1;
        parent->constant = tokens[position - 1].lexem;
        parent->type = tree.findConstType(parent->constant);
        return;
    }

    if (match(LexemType::Separator, "(", false)) {

        parent->expr = new AST::Expr;
        parent->state = 0;
        parseExpr(parent->expr);

        if (!match(LexemType::Separator, ")", false))
            error("Expected ')'");

        return;
    }

    if (match(LexemType::KeyWord, "itof", false)) {
        if (!match(LexemType::Separator, "("))
            error("Expected '('");

        parent->expr = new AST::Expr;
        parent->state = 1;
        parseExpr(parent->expr);

        if (!match(LexemType::Separator, ")"))
            error("Expected ')'");

        return;
    }

    if (match(LexemType::KeyWord, "ftoi", false)) {
        if (!match(LexemType::Separator, "("))
            error("Expected '('");

        parent->expr = new AST::Expr;
        parent->state = 2;
        parseExpr(parent->expr);

        if (!match(LexemType::Separator, ")"))
            error("Expected ')'");

        return;
    }

    error("Error in the arithmetic expresion syntax");

}

bool Syntax::isTypeAhead() {

    if (position < tokens.size())
        if (tokens[position].lexem == "int" or tokens[position].lexem == "float")
            return true;

    return false;
}

bool Syntax::isIDAhead() {
    if (position < tokens.size())
        if (tokens[position].type == LexemType::ID)
            return true;

    return false;
}

void AST::printToConsole() {

    std::cout << "FUNCTION:\n";

    std::cout << "\tBEGIN:\n";
    std::cout << "\t\tFUNC TYPE: " << beginType << "\n";
    std::cout << "\t\tFUNC NAME: " << BeginName << "\n";

    std::cout << "\tDESCRIPTION:\n";
    std::cout << "\t\tINT VARS: ";
    for (auto elem : intVars)
        std::cout << elem << ", ";
    std::cout << ";\n";
    std::cout << "\t\tFLOAT VARS:";
    for (auto elem : floatVars)
        std::cout << elem << ", ";
    std::cout << ";\n";
    
    std::cout << "\tOPERATORS:\n";
    for (auto elem : operations) {
        std::cout << "\t\tOP:\n";
        std::cout << "\t\t\tGOAL VAR: " << elem.first << "\n";
        printExpr(&elem.second, 2);
    }

    std::cout << "\tEND:\n";
    std::cout << "\t\tPROGRAM END VAR: " << endVariable << "\n\n";



}

void AST::printExpr(AST::Expr* expr, int level) {

    if (!expr)
        return;

    put(level);
    std::cout << "EXPR:\n";

    put(level);
    std::cout << "\tSIGN: ";
    switch (expr->sign) {
    case 0: {
        std::cout << "no sign\n"; 
    } break;
    case 1: {
        std::cout << "+\n"; 
    } break;
    case 2: { 
        std::cout << "-\n"; 
    } break;
    }
    printSimpleExpr(expr->expr1, level + 1);
    printSimpleExpr(expr->expr2, level + 1);
}

void AST::printSimpleExpr(AST::SimpleExpr* expr, int level) {

    if (!expr)
        return;

    put(level);
    std::cout << "SIMPLE EXPR:\n";

    switch (expr->state) {
    case -2: {

        put(level);
        std::cout << "\tVAR: " << expr->varName << "\n";
        put(level);
        std::cout << "\tTYPE: " << expr->type << "\n";
    } break;
    case -1: {
        put(level);
        std::cout << "\tCONSTANT: " << expr->constant << "\n";
        put(level);
        std::cout << "\tTYPE: " << expr->type << "\n";
    } break;
    case 0: {
        put(level);
        std::cout << "\tSCOBES - (EXPR): \n";
        printExpr(expr->expr, level + 1);
    } break;
    case 1: {
        put(level);
        std::cout << "\tITOF - (EXPR): \n";
        printExpr(expr->expr, level + 1);
    } break;
    case 2: {
        put(level);
        std::cout << "\tFTOI - (EXPR): \n";
        printExpr(expr->expr, level + 1);
    } break;
    }
}

void AST::put(int level) {
    for (int i = 0; i < level; ++i)
        std::cout << '\t';
}

std::string AST::findVarType(std::string name) {

    if (std::find(intVars.begin(), intVars.end(), name) != intVars.end())
        return "int";

    if (std::find(floatVars.begin(), floatVars.end(), name) != floatVars.end())
        return "float";

    return "TYPE ERROR";
}

std::string AST::findConstType(std::string name) {

    return (std::find(name.begin(), name.end(), '.') != name.end() ? "float" : "int");

}