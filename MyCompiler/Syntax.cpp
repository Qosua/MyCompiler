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

void Syntax::parseType() {

    tree->children.push_back(new ASTNode);
    tree = tree->children.back();

    if (match(LexemType::KeyWord, "int", false)) {
        tree->Name = "Type: int";
        return;
    }
    if (match(LexemType::KeyWord, "float", false)) {
        tree->Name = "Type: float";
        return;
    }
    error("Expected type. Int or float");
}

void Syntax::error(std::string errorText) {

    errors.push_back("Syntax error: \n\t" + errorText + "\n");

}

void Syntax::parseFunction() {

    tree = new ASTNode;
    tree->Name = "Function";

    parseBegin(); // +
    parseDescriptions(); // +
    parseOperators(); // +
    parseEnd(); // +
}

void Syntax::parseBegin() {

    parseType();

    if (!match(LexemType::ID)) 
        error("Expected function name");
    else {

        tree->children.push_back(new ASTNode);
        tree = tree->children.back();
        tree->Name = "Begin";
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
    if (!match(LexemType::ID)) error("Expected some ID at the end");
    if (!match(LexemType::Separator, ";")) error("Expected ';'");
    if (!match(LexemType::Separator, "}")) error("Expected '}'");
}

void Syntax::parseDescriptions() {

    tree->children.push_back(new ASTNode);
    tree = tree->children.back();
    tree->Name = "Descriptions";

    while (isTypeAhead()) { 
        parseDescr();
    }
}

void Syntax::parseOperators() {
    while (isIDAhead()) {
        parseOp();
    }
}

void Syntax::parseOp() {
    if (!match(LexemType::ID)) 
        error("Expected ID in operator");

    if (!match(LexemType::Operator, "=")) 
        error("Expected '='");

    parseExpr();

    if (!match(LexemType::Separator, ";")) 
        error("Expected ';'");
}

void Syntax::parseDescr() {

    tree->children.push_back(new ASTNode);
    tree = tree->children.back();
    tree->Name = "Descr";

    parseType();
    parseVarList();

    if (!match(LexemType::Separator, ";")) 
        error("Expected ';'");
}

void Syntax::parseVarList() {

    tree->children.push_back(new ASTNode);
    tree = tree->children.back();
    tree->Name = "VarList";

    if (!match(LexemType::ID)) error("Expected some ID");

    if (currentToken.lexem == ";")
        return;

    if (match(LexemType::Separator, ",")) {
        parseVarList();
    }
}

void Syntax::parseExpr() {

    parseSimpleExpr();

    while (match(LexemType::Operator, "+", false) or match(LexemType::Operator, "-", false)) {
        parseSimpleExpr();
    }
    
}

void Syntax::parseSimpleExpr() {

    if (match(LexemType::ID, "", false))
        return;

    if (match(LexemType::Constant, "", false))
        return;

    if (match(LexemType::Separator, "(", false)) {

        parseExpr();

        if (!match(LexemType::Separator, ")", false))
            error("Expected ')'");

        return;
    }

    if (match(LexemType::KeyWord, "itof", false)) {
        if (!match(LexemType::Separator, "("))
            error("Expected '('");

        parseExpr();

        if (!match(LexemType::Separator, ")"))
            error("Expected ')'");

        return;
    }

    if (match(LexemType::KeyWord, "ftoi", false)) {
        if (!match(LexemType::Separator, "("))
            error("Expected '('");

        parseExpr();

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
