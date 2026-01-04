#pragma once

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <stack>

#include "Syntax.h"

class Semantics {

public:
	void setTree(AST* _tree);
	void run();
	void printToFile(std::string str);
	void printErrors();

private:
	bool checkMultipleVarInit();
	bool checkProgramReturnVarTypeToBeSureThatEveryThigIsOkay();
	bool checkTypesInTheExpresions();

	void madePrefixString();
	std::string writeExpresions();
	std::string writeExpr(AST::Expr* expr, std::stack<std::string>& st);
	std::string writeExprIF(AST::Expr* expr, std::stack<std::string>& st);
	std::string writeSimpleExpr(AST::SimpleExpr* simpleExpr, std::stack<std::string>& st);

	bool checkOperation(std::string varName, AST::Expr* expr);
	std::string checkExpr(AST::Expr* expr);
	std::string checkSimpleExpr(AST::SimpleExpr* simpleExpr);

	std::string getVarType(std::string name);

	AST* tree;
	std::string prefix;
	std::vector<std::string> errors;

};

