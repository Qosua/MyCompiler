#include "Semantics.h"

void Semantics::setTree(AST _tree)  {
	tree = _tree;
}

void Semantics::run() {

	bool result = true;

	result *= checkMultipleVarInit();
	result *= checkProgramReturnVarTypeToBeSureThatEveryThigIsOkay();
	result *= checkTypesInTheExpresions();

	if (result)
		madePrefixString();
	else
		errors.push_back("Semantics error: prefix form can not be made");

}

void Semantics::printToFile(std::string str) {

	std::fstream stream;
	stream.open(str, std::ios_base::out);

	stream << prefix;

}

void Semantics::printErrors() {

	for (auto& elem : errors)
		std::cout << elem << std::endl;

}

bool Semantics::checkMultipleVarInit() {

	std::map<std::string,std::vector<std::string>> variables;

	for (int i = 0; i < tree.intVars.size(); ++i)
		variables[tree.intVars[i]].push_back("int");

	for (int i = 0; i < tree.floatVars.size(); ++i)
		variables[tree.floatVars[i]].push_back("float");

	bool ans = true;

	for (auto& elem : variables)
		if (elem.second.size() > 1) {
			errors.push_back("Semantics error: multiple initialization of the variable - " + elem.first);

			ans = false;

			for (int i = 1; i < elem.second.size(); ++i)
				if (elem.second[0] != elem.second[i]) 
					errors.push_back("Semantics error: uncertain variable type for - " + elem.first);
			
		}

	return ans;

}

bool Semantics::checkProgramReturnVarTypeToBeSureThatEveryThigIsOkay() {

	bool isInt = (
		std::find(tree.intVars.begin(), 
				  tree.intVars.end(),
				  tree.endVariable
				  ) != tree.intVars.end());

	bool isFloat = (
		std::find(tree.floatVars.begin(),
				  tree.floatVars.end(),
				  tree.endVariable
				  ) != tree.floatVars.end());

	if (isInt and isFloat) {
		errors.push_back("Semantics error: uncertain variable type for the program return");
		return false;
	}

	if(!isInt and !isFloat) {
		errors.push_back("Semantics error: return variable does not exist");
		return false;
	}

	std::string type = (isInt ? "int" : "float");

	if (type != tree.beginType) {

		errors.push_back("Semantics error: return var must the be same type as main function");
		return false;

	}

	return true;
}

bool Semantics::checkTypesInTheExpresions() {

	bool result = true;

	for (auto& operation : tree.operations) {

		result *= checkOperation(operation.first, &operation.second);

	}

	return result;
}

void Semantics::madePrefixString() {

	std::string result;

	result += tree.beginType + " " + tree.BeginName + " 2 CALL\n";

	result += "int";
	for (int i = 0; i < tree.intVars.size(); ++i) {

		result += " " + tree.intVars[i];

	}
	result += std::string(" ") + std::to_string(tree.intVars.size()) + " DECL\n";

	result += "float";
	for (int i = 0; i < tree.floatVars.size(); ++i) {

		result += " " + tree.floatVars[i];

	}
	result += std::string(" ") + std::to_string(tree.intVars.size()) + " DECL\n";

	result += writeExpresions();

	result += "return " + tree.endVariable + " 2 CALL\n";

	prefix = result;

}

std::string Semantics::writeExpresions() {

	std::string result;
	std::stack<std::string> st;

	for (auto& operation : tree.operations) {

		st.push("=");

		result += operation.first;
		result += writeExpr(&operation.second, st);

		while (st.size() > 0) {
			result += " " + st.top();
			st.pop();
		}


		result += " \n";

	}

	return result;
}

std::string Semantics::writeExpr(AST::Expr* expr, std::stack<std::string>& st) {

	std::string result;

	if (expr->sign == 0) {

		result = writeSimpleExpr(expr->expr1, st);

	}
	else {

		if(expr->sign == 1)
			st.push("+");
		else
			st.push("-");

		result = writeSimpleExpr(expr->expr1, st);
		result += writeExpr(expr->expr2, st);

	}

	return result;
}

std::string Semantics::writeExprIF(AST::Expr* expr, std::stack<std::string>& st) {

	std::string result;

	if (expr->sign == 0) {

		result = writeSimpleExpr(expr->expr1, st);

	}
	else {

		std::stack<std::string> anotherStack;

		result += writeSimpleExpr(expr->expr1, anotherStack);
		result += writeExpr(expr->expr2, anotherStack);

		if (expr->sign == 1)
			anotherStack.push("+");
		else
			anotherStack.push("-");

		while (anotherStack.size() > 0) {
			result += " " + anotherStack.top();
			anotherStack.pop();
		}

	}

	return result;
}

std::string Semantics::writeSimpleExpr(AST::SimpleExpr* simpleExpr, std::stack<std::string>& st) {

	// struct SimpleExpr 
	// -2 - var, -1 - constant, 
	// 0 - scobes, 1 - itof, 2 - ftoi

	std::string result;

	if (simpleExpr->state == -2) {

		result = " " + simpleExpr->varName;

	}
	if (simpleExpr->state == -1) {

		result = " " + simpleExpr->constant;

	}
	if (simpleExpr->state == 0) {

		result = writeExpr(simpleExpr->expr, st);

	}
	if (simpleExpr->state == 1 or simpleExpr->state == 2) {

		std::stack<std::string> st;

		result += writeExprIF(simpleExpr->expr, st);

		if (simpleExpr->state == 1)
			result += (" itof 2 CALL");
		else
			result += (" ftoi 2 CALL");

	}

	return result;
}

bool Semantics::checkOperation(std::string varName, AST::Expr* expr) {

	std::string varType = getVarType(varName);
	std::string exprType = checkExpr(expr);

	if ((varType != exprType) or (exprType == "TYPE ERROR")) {

		errors.push_back("Semantics error: expresion types do not match or variable does not exist");
		return false;
	}

	return true;
}

std::string Semantics::checkExpr(AST::Expr* expr) {

	// struct Expr  
	// 0 - nothing, 1 - plus, 2 - minus

	std::string type;

	if (expr->sign == 0) {

		type = checkSimpleExpr(expr->expr1);

	}
	else {

		std::string tempType1 = checkSimpleExpr(expr->expr1);
		std::string tempType2 = checkExpr(expr->expr2);

		if ((tempType1 == tempType2) and 
			(tempType1 != "TYPE ERROR") and 
			(tempType2 != "TYPE ERROR"))

			type = tempType1;
		else
			type = "TYPE ERROR";

	}
	
	return type;
}

std::string Semantics::checkSimpleExpr(AST::SimpleExpr* simpleExpr) {

	// struct SimpleExpr 
	// -2 - var, -1 - constant, 
	// 0 - scobes, 1 - itof, 2 - ftoi

	std::string type;

	if (simpleExpr->state == -2) {

		if (std::find(tree.intVars.begin(), tree.intVars.end(), simpleExpr->varName) == tree.intVars.end()) {
			if (std::find(tree.floatVars.begin(), tree.floatVars.end(), simpleExpr->varName) == tree.floatVars.end()) {
				
				type = "TYPE ERROR";
				errors.push_back("Semantics error: variable - " + simpleExpr->varName + " was not declared");
			}
			else
				type = simpleExpr->type;
		}
		else
			type = simpleExpr->type;

	}
	if (simpleExpr->state == -1) {

		type = simpleExpr->type;

	}
	if (simpleExpr->state == 0) {

		type = checkExpr(simpleExpr->expr);

	}
	if (simpleExpr->state == 1) {

		type = checkExpr(simpleExpr->expr);
		if (type != "int") {
			type = "TYPE ERROR";
			errors.push_back("Semantics error: itof must get int");
		}
		else
			type = "float";

	}
	if (simpleExpr->state == 2) {

		type = checkExpr(simpleExpr->expr);
		if (type != "float") {
			type = "TYPE ERROR";
			errors.push_back("Semantics error: ftoi must get float");
		}
		else
			type = "int";

	}

	return type;
}

std::string Semantics::getVarType(std::string name) {

	if (std::find(tree.intVars.begin(), tree.intVars.end(), name) != tree.intVars.end())
		return "int";

	if (std::find(tree.floatVars.begin(), tree.floatVars.end(), name) != tree.floatVars.end())
		return "float";

	return "TYPE ERROR";
}
