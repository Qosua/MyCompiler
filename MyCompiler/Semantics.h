#pragma once

#include <vector>
#include <string>
#include <map>

#include "Syntax.h"

class Semantics {

public:
	void setTree(AST _tree);
	void run();
	void printToFile(std::string str);
	void printErrors();

private:
	bool checkMultipleVarInit();
	bool checkProgramReturnVarTypeToBeSureThatEveryThigIsOkay();
	bool checkTypesInTheExpresions();

	void madePrefixString();

	bool checkOperation(std::string varName, AST::Expr* expr);
	std::string checkExpr(AST::Expr* expr);
	std::string checkSimpleExpr(AST::SimpleExpr* simpleExpr);

	std::string getVarType(std::string name);

	AST tree;
	std::vector<std::string> errors;

};

