#include "Syntax.h"

Syntax::Syntax() {

    tree = new AST;
}

void Syntax::run() {

    if (position < tokens.size()) {
        currentToken = tokens[position];
    }

    parseFunction();
}

std::vector<std::string> Syntax::getErrors() {
    return errors;
}

void Syntax::printToFile() {

    tree->printToFileTree();


}

void Syntax::printToConsole() {
    tree->printToConsole();
}

AST* Syntax::getTree()
{
    return tree;
}

void Syntax::processToken(Token token) {

    tokens.push_back(token);

    if (tokens.size() == 1) 
        return;

    currentToken = tokens[0];
    position = 1;

    switch (currentState)
    {
    case Begin: {
        bool res = parseBegin();
        if (res) {
            currentState = Descr;
            clearBuffer();
        }
    }
        break;
    case Descr: {
        bool res = parseDescriptions();
        if (res) {
            currentState = Operators;
            clearBuffer();
        }
    }
        break;
    case Operators: {
        bool res = parseOperators();
        if (res) {
            currentState = End;
            clearBuffer();
        }
    }
        break;
    case End: {
        bool res = parseEnd();
        if (res) {
            currentState = Finished;
            clearBuffer();
        }
    }
        break;
    case Finished: break;
    default: break;
    }

    errors.clear();

}

bool Syntax::match(LexemType type, std::string val, bool toMoveIfFalse) {

    if(position >= tokens.size())
        return false;

    if (currentToken.type == type and (val == "" or currentToken.lexem == val)) {

        if (position + 1 <= tokens.size()) {
            currentToken = tokens[position++];
        }

        return true;
    }

    if (toMoveIfFalse and position + 1 <= tokens.size()) {
        currentToken = tokens[position++];
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

    errors.push_back("Syntax error \n\t" + errorText + "\n");

}

void Syntax::parseFunction() {

    parseBegin();
    parseDescriptions();
    parseOperators();
    parseEnd();
}

bool Syntax::parseBegin() {

    bool res = true;

    if (parseType())
        tree->beginType = tokens[position - 1].lexem;
    else
        res = false;

    if (!match(LexemType::ID)) {
        error("Expected function name");
        res = false;
    }
    else {
        tree->BeginName = tokens[position - 1].lexem;
    }
    
    if (!match(LexemType::Separator, "(")) {
        error("Expected '('");
        res = false;
    }
    if (!match(LexemType::Separator, ")")) {
        error("Expected ')'");
        res = false;
}
    if (!match(LexemType::Separator, "{")) {
        error("Expected '{'");
        res = false;
    }
    return res;
}

bool Syntax::parseEnd() {

    bool res = true;

    if (!match(LexemType::KeyWord, "return")) {
        error("Expected return key word");
        res = false;
    }
    
    if (!match(LexemType::ID)) {
        error("Expected some ID at the end");
        res = false;
    }
    else
        tree->endVariable = tokens[position - 1].lexem;

    if (!match(LexemType::Separator, ";")) {
        error("Expected ';'");
        res = false;
    }
    if (!match(LexemType::Separator, "}")) {
        error("Expected '}'");
        res = false;
    }

    return res;
}

bool Syntax::parseDescriptions() {

    bool res = true;

    while (isTypeAhead())
        res *= parseDescr();

    return res;
}

bool Syntax::parseOperators() {

    bool res = true;

    while (isIDAhead())
        res *= parseOp();
    
    return res;
}

bool Syntax::parseDescr() {

    bool res = true;

    if (parseType())
        lastVarType = tokens[position - 2].lexem;
    else
        res = false;
    
    res *= parseVarList();

    if (!match(LexemType::Separator, ";")) {
        error("Expected ';'");
        res = false;
    }
    
    return res;
}

bool Syntax::parseVarList() {

    bool res = true;

    if (!match(LexemType::ID)) {
        error("Expected some ID");
        res = false;
    }
    else {

        if (lastVarType == "int") {
            (*tree->table)[tokens[position - 1].lexem].isInt = 1;
        }
        else
            if (lastVarType == "float") {
                (*tree->table)[tokens[position - 1].lexem].isInt = 2;
            }
            else
                res = false;

    }

    if (currentToken.lexem == ";") {
        return res;
        position -= 1;
    }

    if (match(LexemType::Separator, ",")) {
        parseVarList();
    }
    else
        res = false;

    return res;
}

bool Syntax::parseOp() {

    bool res = true;

    if (!match(LexemType::ID)) {
        error("Expected ID in operator");
        res = false;
    }
    else {
        tree->operations.emplace_back();
        tree->operations.back().first = tokens[position - 1].lexem;
    }

    if (!match(LexemType::Operator, "=")) {
        error("Expected '='");
        res = false;
    }

    parseExpr(&tree->operations.back().second);

    if (!match(LexemType::Separator, ";")) {
        error("Expected ';'");
        res = false;
    }

    return res;
}

bool Syntax::parseExpr(AST::Expr* parent) {

    bool res = true;

    AST::SimpleExpr* simpleExpr1 = new AST::SimpleExpr;
    parent->expr1 = simpleExpr1;
    res *= parseSimpleExpr(simpleExpr1);

    while (match(LexemType::Operator, "+", false) or 
           match(LexemType::Operator, "-", false)) {

        parent->sign = (tokens[position - 1].lexem == "+" ? 1 : 2);
        AST::Expr* expr = new AST::Expr;
        parent->expr2 = expr;
        res *= parseExpr(expr);
    }

    return res;
    
}

bool Syntax::parseSimpleExpr(AST::SimpleExpr* parent) {

    bool res = true;

    if (match(LexemType::ID, "", false)) {
        parent->state = -2;
        parent->varName = tokens[position - 1].lexem;
        parent->type = tree->findVarType(parent->varName);
        return true;
    }

    if (match(LexemType::Constant, "", false)) {
        parent->state = -1;
        parent->constant = tokens[position - 1].lexem;
        parent->type = tree->findConstType(parent->constant);
        return true;
    }

    if (match(LexemType::Separator, "(", false)) {

        parent->expr = new AST::Expr;
        parent->state = 0;
        res *= parseExpr(parent->expr);

        if (!match(LexemType::Separator, ")", false)) {
            error("Expected ')'");
            res = false;
        }

        return res;
    }

    if (match(LexemType::KeyWord, "itof", false)) {
        if (!match(LexemType::Separator, "(")) {
            error("Expected '('");
            res = false;
        }

        parent->expr = new AST::Expr;
        parent->state = 1;
        res *= parseExpr(parent->expr);

        if (!match(LexemType::Separator, ")")) {
            error("Expected ')'");
            res = false;
        }

        return res;
    }

    if (match(LexemType::KeyWord, "ftoi", false)) {
        if (!match(LexemType::Separator, "(")) {
            error("Expected '('");
            res = false;
        }

        parent->expr = new AST::Expr;
        parent->state = 2;
        res *= parseExpr(parent->expr);

        if (!match(LexemType::Separator, ")")) {
            error("Expected ')'");
            res = false;
        }

        return res;
    }

    error("Error in the arithmetic expresion syntax");
    return false;

}

bool Syntax::isTypeAhead() {

    if (position < tokens.size())
        if (tokens[position - 1].lexem == "int" or tokens[position - 1].lexem == "float")
            return true;

    return false;
}

bool Syntax::isIDAhead() {
    if (position < tokens.size())
        if (tokens[position].type == LexemType::ID)
            return true;

    return false;
}

void Syntax::clearBuffer() {

    /*delete tree;
    tree = new AST;
    tree->table = table;*/
    tokens.clear();
    position = 0;

}

void AST::operator=(const AST& obj)
{
    BeginName = obj.BeginName;
    beginType = obj.beginType;
    operations = obj.operations;
    endVariable = obj.endVariable;
    table = obj.table;
}

void AST::printToConsole() {

    std::cout << "FUNCTION:\n";

    std::cout << "\tBEGIN:\n";
    std::cout << "\t\tFUNC TYPE: " << beginType << "\n";
    std::cout << "\t\tFUNC NAME: " << BeginName << "\n";

    std::cout << "\tDESCRIPTION:\n";
    std::cout << "\t\tINT VARS: ";
    for (int i = 0; i < table->size(); ++i) {
        for (int j = 0; j < (*table)[i].size(); ++j) {

            if ((*table)[i][j].isInt == 1) {
                std::cout << (*table)[i][j].lexem + ", ";
            }
        }

    }
    std::cout << ";\n";

    std::cout << "\t\tFLOAT VARS:";
    for (int i = 0; i < table->size(); ++i) {
        for (int j = 0; j < (*table)[i].size(); ++j) {

            if ((*table)[i][j].isInt == 2) {
                std::cout << (*table)[i][j].lexem + ", ";
            }
        }

    }
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
    printExpr(expr->expr2, level + 1);
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

void AST::printToFileTree()
{
    globalOutput.open("AST.txt");
    globalOutput << "FUNCTION:\n";

    globalOutput << "\tBEGIN:\n";
    globalOutput << "\t\tFUNC TYPE: " << beginType << "\n";
    globalOutput << "\t\tFUNC NAME: " << BeginName << "\n";

    globalOutput << "\tDESCRIPTION:\n";
    globalOutput << "\t\tINT VARS: ";
    for (int i = 0; i < table->size(); ++i) {
        for (int j = 0; j < (*table)[i].size(); ++j) {

            if ((*table)[i][j].isInt == 1) {
                globalOutput << (*table)[i][j].lexem + ", ";
            }
        }

    }
    globalOutput << ";\n";

    globalOutput << "\t\tFLOAT VARS:";
    for (int i = 0; i < table->size(); ++i) {
        for (int j = 0; j < (*table)[i].size(); ++j) {

            if ((*table)[i][j].isInt == 2) {
                globalOutput << (*table)[i][j].lexem + ", ";
            }
        }

    }
    globalOutput << ";\n";

    globalOutput << "\tOPERATORS:\n";
    for (auto elem : operations) {
        globalOutput << "\t\tOP:\n";
        globalOutput << "\t\t\tGOAL VAR: " << elem.first << "\n";
        printExprFile(&elem.second, 2);
    }

    globalOutput << "\tEND:\n";
    globalOutput << "\t\tPROGRAM END VAR: " << endVariable << "\n\n";


}

void AST::printExprFile(AST::Expr* expr, int level)
{
    if (!expr)
        return;

    putFile(level);
    globalOutput << "EXPR:\n";

    putFile(level);
    globalOutput << "\tSIGN: ";
    switch (expr->sign) {
    case 0: {
        globalOutput << "no sign\n";
    } break;
    case 1: {
        globalOutput << "+\n";
    } break;
    case 2: {
        globalOutput << "-\n";
    } break;
    }
    printSimpleExprFile(expr->expr1, level + 1);
    printExprFile(expr->expr2, level + 1);
}

void AST::printSimpleExprFile(AST::SimpleExpr* expr, int level)
{
    if (!expr)
        return;

    putFile(level);
    globalOutput << "SIMPLE EXPR:\n";

    switch (expr->state) {
    case -2: {

        putFile(level);
        globalOutput << "\tVAR: " << expr->varName << "\n";
        putFile(level);
        globalOutput << "\tTYPE: " << expr->type << "\n";
    } break;
    case -1: {
        putFile(level);
        globalOutput << "\tCONSTANT: " << expr->constant << "\n";
        put(level);
        globalOutput << "\tTYPE: " << expr->type << "\n";
    } break;
    case 0: {
        putFile(level);
        globalOutput << "\tSCOBES - (EXPR): \n";
        printExprFile(expr->expr, level + 1);
    } break;
    case 1: {
        putFile(level);
        globalOutput << "\tITOF - (EXPR): \n";
        printExprFile(expr->expr, level + 1);
    } break;
    case 2: {
        putFile(level);
        globalOutput << "\tFTOI - (EXPR): \n";
        printExprFile(expr->expr, level + 1);
    } break;
    }
}

void AST::putFile(int level)
{
    for (int i = 0; i < level; ++i)
        globalOutput << '\t';
}

std::string AST::findVarType(std::string name) {

    int isInt = (*table)[name].isInt;

    if (isInt == 1)
        return "int";

    if (isInt == 2)
        return "float";

    return "TYPE ERROR";
}

std::string AST::findConstType(std::string name) {

    return (std::find(name.begin(), name.end(), '.') != name.end() ? "float" : "int");

}