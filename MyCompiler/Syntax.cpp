#include "Syntax.h"

Syntax::Syntax() {

    tree = new AST;
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

    position = 0;

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
            tokens.push_back(Token(kostyl, LexemType::ID));
        }
    }
        break;
    case Operators: {
        bool res = parseOperators();
        if (res) {
            currentState = End;
            clearBuffer();
            tokens.push_back(Token(kostyl, LexemType::KeyWord));
        }
        else
            operations.clear();
    }
        break;
    case End: {
        bool res = parseEnd();
        if (res) {
            assembleTree();
            currentState = Finished;
            clearBuffer();
        }
    }
        break;
    case Finished:  break;
    default: break;
    }

    errors.clear();

}

bool Syntax::match(LexemType type, std::string val, bool toMoveIfFalse) {

    if(position >= tokens.size())
        return false;

    if (tokens[position].type == type and (val == "" or tokens[position].lexem == val)) {

        if (position < tokens.size())
            position += 1;
        

        return true;
    }

    if (toMoveIfFalse and position < tokens.size())
        position += 1;
    
    return false;
}

bool Syntax::parseType() {

    if (match(LexemType::KeyWord, "int", false)) {
        return true;
    }
    
    if (match(LexemType::KeyWord, "float", false)) {
        return true;
    }

    position += 1;

    error("Expected type. int or float");
    return false;
}

void Syntax::error(std::string errorText) {

    errors.push_back("Syntax error \n\t" + errorText + "\n");

}

std::string Syntax::idType(std::string name) {

    int isInt = (*table)[name].isInt;

    if (isInt == 1)
        return "int";
    if (isInt == 2)
        return "float";

    return "TYPE ERROR";

}

std::string Syntax::constantType(std::string name) {

    for (int i = 0; i < name.size(); ++i)
        if (name[i] == '.')
            return "float";

    return "int";

}

bool Syntax::parseBegin() {

    bool res = true;

    if (parseType())
        beginType = previousLexem();
    else
        res = false;

    if (!match(LexemType::ID)) {
        error("Expected function name");
        res = false;
    }
    else {
        beginName = previousLexem();
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
        endVariable = previousLexem();

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

    while (parseType())
        res *= parseDescr();

    position -= 1;

    if (match(LexemType::ID)) {
        kostyl = previousLexem();
        return res;
    }
    else
        error("Wrong transition between Descriptions and Operators");

    return false;
}

bool Syntax::parseOperators() {

    bool res = true;

    while (match(LexemType::ID))
        res *= parseOp();

    position -= 1;

    if (match(LexemType::KeyWord, "return")) {
        kostyl = previousLexem();
        return res;
    }
    else
        error("Wrong transition between Descriptions and Operators");

    return false;
}

bool Syntax::parseDescr() {

    bool res = true;

    position -= 1;

    if (parseType())
        lastVarType = previousLexem();
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
            (*table)[tokens[position - 1].lexem].isInt = 1;
        }
        else
            if (lastVarType == "float") {
                (*table)[tokens[position - 1].lexem].isInt = 2;
            }
            else
                res = false;

    }

    if (position < tokens.size() and tokens[position].lexem == ";")
        return res;

    if (match(LexemType::Separator, ",")) {
        parseVarList();
    }
    else
        res = false;

    return res;
}

bool Syntax::parseOp() {

    bool res = true;

    position -= 1;

    if (!match(LexemType::ID)) {
        error("Expected ID in operator");
        res = false;
    }
    else {
        operations.emplace_back();
        operations.back().first = previousLexem();
    }

    if (!match(LexemType::Operator, "=")) {
        error("Expected '='");
        res = false;
    }

    res *= parseExpr(&operations.back().second);

    if (!match(LexemType::Separator, ";")) {
        error("Expected ';'");
        res = false;
    }

    return res;
}

bool Syntax::parseExpr(Expr* parent) {

    bool res = true;

    SimpleExpr* simpleExpr1 = new SimpleExpr;
    parent->expr1 = simpleExpr1;
    res *= parseSimpleExpr(simpleExpr1);

    while (match(LexemType::Operator, "+", false) or 
           match(LexemType::Operator, "-", false)) {

        parent->sign = (previousLexem() == "+" ? 1 : 2);
        Expr* expr = new Expr;
        parent->expr2 = expr;
        res *= parseExpr(expr);
    }

    return res;
    
}

bool Syntax::parseSimpleExpr(SimpleExpr* parent) {

    bool res = true;

    if (match(LexemType::ID, "", false)) {
        parent->state = -2;
        parent->varName = previousLexem();
        parent->type = idType(parent->varName);
        return true;
    }

    if (match(LexemType::Constant, "", false)) {
        parent->state = -1;
        parent->constant = previousLexem();
        parent->type = constantType(parent->constant);
        return true;
    }

    if (match(LexemType::Separator, "(", false)) {

        parent->expr = new Expr;
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

        parent->expr = new Expr;
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

        parent->expr = new Expr;
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

std::string& Syntax::previousLexem() {
    return tokens[position - 1].lexem;
}

void Syntax::assembleTree() {
    
    tree->BeginName = beginName;
    tree->beginType = beginType;
    tree->operations = operations;
    tree->endVariable = endVariable;
    tree->table = table;

}

void Syntax::clearBuffer() {

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

void AST::printExpr(Expr* expr, int level) {

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

void AST::printSimpleExpr(SimpleExpr* expr, int level) {

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

void AST::printExprFile(Expr* expr, int level)
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

void AST::printSimpleExprFile(SimpleExpr* expr, int level)
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