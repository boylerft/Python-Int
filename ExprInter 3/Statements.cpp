//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }
/*
void Statements::addFunc(std::string funcName, Statement *func) {
    std::cout << "Adding " << funcName << "to the function list" << std::endl;
    symFunc[funcName] = func;
}

Statement* Statements::findFunc(std::string funcName) {
    return symFunc.find(funcName)->second;
}
*/
void Statements::print() {
    for (auto s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
    //bool retFlag = false;
    /*
    for (auto s: _statements) {
        if (retFlag) {
            continue;
        }
        else {
            ReturnStatement *ret = dynamic_cast<ReturnStatement*>(s);
            if (ret == nullptr) {
                std::cout << "no return" << std::endl;
                s->evaluate(symTab);
            }
            else {
                std::cout << "return" << std::endl;
                s->evaluate(symTab);
                retFlag = true;
            }
        }
    }
    */
    for (auto s: _statements) {
        if (symTab.getReturnBool())
            break;
        else 
            s->evaluate(symTab);
    }
}

// AssignmentStatement
//AssignmentStatement::AssignmentStatementArray() : _lhsVariable{""},
//_rhsExpressionArray{nullptr} {}

//AssignmentStatements::AssignmentStatementArray(std::string _lhsVar, std::vector<ExprNode*>):
 //       _lhsVariable{lhsVar}, _rhsExpression{rhsExprArray} {}

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr}, _arrayContent{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr, std::vector<ExprNode*> arrCont):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr}, _arrayContent{arrCont} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    // Handles variables
    //std::cout << "printitnitn" << std::endl;
    /*
    if (rhsExpression() == nullptr) {
        
        std::vector<ExprNode*> arr = arrayContent();
        // need to add checks for int or string array, and 
        // create the array based off that
        std::vector<int> temp;
        // SymTab set value for array
        for (int i = 0; i < arr.size(); i++) {
            int element = arr[i]->evaluate(symTab);
            temp.push_back(element);
        }
        symTab.setValueForArr(lhsVariable(), temp);
    }
    
    else {
    */
        if (rhsExpression()->token().isName()) {
            CallExpr *tempCall = dynamic_cast<CallExpr*>(rhsExpression());
            if (tempCall != nullptr) {
                TypeDescriptor *funcCall = tempCall->evalFunc(symTab);
                if (funcCall == nullptr) {
                    std::cout << "Error with function:: printing with no return value " << std::endl;
                    exit(2);
                }
                else if (funcCall->checkIfInt()) {
                    //int rhs = l[i]->evaluate(symTab);
                    NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(funcCall);
                    int rhs = desc->value.intValue;
                    symTab.setValueFor(lhsVariable(), rhs);
                    //consolePrint += std::to_string(rhs);
                }
                else {
                    //std::string rhs = l[i]->evaluateStr(symTab);
                    StringDescriptor *desc = dynamic_cast<StringDescriptor*>(funcCall);
                    std::string rhs = desc->value.stringValue;
                    symTab.setValueForString(lhsVariable(), rhs);
                    //consolePrint += rhs;
                }
            }
            else {
                TypeDescriptor *desc = symTab.getValueFor(rhsExpression()->token().getName());
                if (desc->checkIfInt()){
                    int rhs = rhsExpression()->evaluate(symTab);
                    symTab.setValueFor(lhsVariable(), rhs);
                }
                else {
                    std::string rhs = rhsExpression()->evaluateStr(symTab);
                    symTab.setValueForString(lhsVariable(), rhs);
                }
            }
        } // Handles string values
        else if (rhsExpression()->token().isStringValue()) {
            std::string rhs = rhsExpression()->evaluateStr(symTab);
            symTab.setValueForString(lhsVariable(), rhs);
        } // Handles integer values
        else if (rhsExpression()->token().isWholeNumber()) {
            int rhs = rhsExpression()->evaluate(symTab);
            symTab.setValueFor(lhsVariable(), rhs);
        } // Handles infix expressions for strings and ints
        else if (rhsExpression()->token().isArithmeticOperator()){
            InfixExprNode *lNode = dynamic_cast<InfixExprNode*>(rhsExpression());
            lNode->right()->token().print();
            while(lNode->right()->token().isArithmeticOperator()) {
                lNode = dynamic_cast<InfixExprNode*>(lNode->right());
            }
            if (lNode->right()->token().isName()){
                TypeDescriptor *desc = symTab.getValueFor(lNode->right()->token().getName());
                    if (desc->checkIfInt()){
                        int rhs = rhsExpression()->evaluate(symTab);
                        symTab.setValueFor(lhsVariable(), rhs);
                    }
                    else {
                        std::string rhs = rhsExpression()->evaluateStr(symTab);
                        symTab.setValueForString(lhsVariable(), rhs);
                    }
            }
            else if (lNode->right()->token().isWholeNumber()) {
                int rhs = rhsExpression()->evaluate(symTab);
                symTab.setValueFor(lhsVariable(), rhs);
            }
            else {
                rhsExpression()->token().print();
                std::string rhs = rhsExpression()->evaluateStr(symTab);
                symTab.setValueForString(lhsVariable(), rhs);
            }

        }
        else if (rhsExpression()->token().isRelationalOperator()) {
            std::cout << std::endl;
            std::cout << std::endl;
            int rhs = rhsExpression()->evaluate(symTab);
            symTab.setValueFor(lhsVariable(), rhs);
        }
    }


std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}
std::vector<ExprNode*> &AssignmentStatement::arrayContent() {
    return _arrayContent;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}

PrintStatement::PrintStatement() : _printExpression{nullptr} {}

PrintStatement::PrintStatement(std::vector<ExprNode*> printExpr) : 
        _printExpression{printExpr} {} 

void PrintStatement::evaluate(SymTab &symTab) {
    std::cout << "Print function found" << std::endl;
    std::string consolePrint;
    std::vector<ExprNode*> l = printExpression();
    
    //std::cout << "Printing to see return: " << 
    // Vector of all the ExprNodes in test list
    for (int i = 0; i < l.size(); i++) {
        
        // Handles printing variables
        if (l[i]->token().isName()) {
            
            CallExpr *tempCall = dynamic_cast<CallExpr*>(l[i]);
            if (tempCall != nullptr) {
                TypeDescriptor *funcCall = tempCall->evalFunc(symTab);
                if (funcCall == nullptr) {
                    std::cout << "Error with function:: printing with no return value " << std::endl;
                    exit(2);
                }
                else if (funcCall->checkIfInt()) {
                    //int rhs = l[i]->evaluate(symTab);
                    NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(funcCall);
                    int rhs = desc->value.intValue;
                    consolePrint += std::to_string(rhs);
                }
                else {
                    //std::string rhs = l[i]->evaluateStr(symTab);
                    StringDescriptor *desc = dynamic_cast<StringDescriptor*>(funcCall);
                    std::string rhs = desc->value.stringValue;
                    consolePrint += rhs;
                }
            }
            else {
                TypeDescriptor *desc = symTab.getValueFor(l[i]->token().getName());
                if (desc->checkIfInt()) {
                    int rhs = l[i]->evaluate(symTab);
                    consolePrint += std::to_string(rhs);
                }
                else {
                    std::string rhs = l[i]->evaluateStr(symTab);
                    consolePrint += rhs;
                }
            }
        }   // Handles string variable
        else if (l[i]->token().isStringValue()) {
            std::string rhs = l[i]->evaluateStr(symTab);
            consolePrint += rhs;
        }   // Handles interger variable
        else if (l[i]->token().isWholeNumber()) {
            int rhs = l[i]->evaluate(symTab);
            consolePrint += std::to_string(rhs);
        }
        // Handles infix expressions for strings integers and variables
        else if (l[i]->token().isArithmeticOperator()){
            
            InfixExprNode *lNode = dynamic_cast<InfixExprNode*>(l[i]);
            
            while(lNode->right()->token().isArithmeticOperator()) {
                lNode = dynamic_cast<InfixExprNode*>(lNode->right());
            }
            // Variable
            if (lNode->right()->token().isName()) {
                TypeDescriptor *desc = symTab.getValueFor(lNode->right()->token().getName());
                // Int Variable
                if (desc->checkIfInt()){
                    int rhs = l[i]->evaluate(symTab);
                    consolePrint += std::to_string(rhs);
                }
                // String Variable
                else {
                    std::string rhs = l[i]->evaluateStr(symTab);
                    consolePrint += rhs;
                }
            }
            // Integer
            else if (lNode->right()->token().isWholeNumber()) {
                int rhs = l[i]->evaluate(symTab);
                consolePrint += std::to_string(rhs);
            } // String
            else {
                std::string rhs = l[i]->evaluateStr(symTab);
                consolePrint += rhs;
            }
        }
        else if (l[i]->token().isRelationalOperator()) {
            int rhs = l[i]->evaluate(symTab);
            consolePrint += std::to_string(rhs);
        }
        consolePrint += " ";
    }
    std::cout << std::endl;
    std::cout << "Output from Terminal: " << std::endl;
    std::cout << std::setw(50) << consolePrint << std::endl;
}


std::vector<ExprNode*> &PrintStatement::printExpression() {
    return _printExpression;
}

void PrintStatement::print() {
    std::cout << "print function will print : " << std::endl;
    //_printVariable->print();
    //printExpression()->token().print();
    std::cout << std::endl;
}

// ForStatement

ForStatement::ForStatement() : _varName{""}, _r{nullptr}, _list{nullptr}, _statements{nullptr} {}

ForStatement::ForStatement(std::string var, forRange *r, std::vector<ExprNode*> list, Statements *stmts):
    _varName{var}, _r{r}, _list{list}, _statements{stmts} {}

void ForStatement::evaluate(SymTab &symTab) {
    forLoopR()->setRange(rangeList(), symTab);
    int varVal = forLoopR()->getStart();
    symTab.setValueFor(forLoopVar(), varVal);
    std::cout << "Stop: " << forLoopR()->getStop() << std::endl;
    while(!forLoopR()->rangeCont()){
        //std::cout << "Made it here" << std::endl;
        statements()->evaluate(symTab);
        varVal = forLoopR()->getNext();
        symTab.setValueFor(forLoopVar(), varVal);
    }
    std::cout << "forloop reset" << std::endl;
    forLoopR()->setRange(rangeList(), symTab);
    
}
std::string &ForStatement::forLoopVar() {
    return _varName;
}
std::vector<ExprNode*> &ForStatement::rangeList() {
    return _list;
}
Statements *&ForStatement::statements(){
    return _statements;
}
forRange *&ForStatement::forLoopR() {
    return _r;
}

void ForStatement::print() {
    std::cout << "Statements that have been initialized for this for loop are: " << std::endl;
    statements()->print();
    std::cout << std::endl;
}

// IfStatement

IfStatement::IfStatement() : _conditions{nullptr}, _statements{nullptr}, _elseStatements{nullptr}, 
    _elifStmtList{nullptr}, _elifCondList{nullptr} {}

IfStatement::IfStatement(ExprNode *cond, Statements *stmts, Statements *elseStmts, std::vector<Statements*> stmtList,
                        std::vector<ExprNode*> condList): 
    _conditions{cond}, _statements{stmts}, _elseStatements{elseStmts}, _elifStmtList{stmtList}, _elifCondList{condList} {}

void IfStatement::evaluate(SymTab &symTab) {
    std::vector<Statements*> elif_s = elifStmtList();
    std::vector<ExprNode*> elif_c = elifCondList();
    bool elifCaught = false;
    if (elif_s.empty() && elif_c.empty()) {
        if (elseStatements() == nullptr) {

            if (conditions()->evaluate(symTab)){
                statements()->evaluate(symTab);
            }
        }
        else {
            if (conditions()->evaluate(symTab)){
                statements()->evaluate(symTab);
            }
            else {
                elseStatements()->evaluate(symTab);
            }
        }
    }
    else {
        if (conditions()->evaluate(symTab)){
                statements()->evaluate(symTab);
        }
        else {
            for (int i = 0; i < elif_s.size(); i++) {
                if (elif_c[i]->evaluate(symTab)) {
                    elif_s[i]->evaluate(symTab);
                    elifCaught = true;
                    break;
                }
            }
            if (!elifCaught && elseStatements() != nullptr) {
                elseStatements()->evaluate(symTab);
            }
        }
    }
}


ExprNode *&IfStatement::conditions() {
    return _conditions;
}

Statements *&IfStatement::statements() {
    return _statements;
}

Statements *&IfStatement::elseStatements() {
    return _elseStatements;
}

std::vector<Statements*> &IfStatement::elifStmtList() {
    return _elifStmtList;
}

std::vector<ExprNode*> &IfStatement::elifCondList() {
    return _elifCondList;
}

void IfStatement::print() {
    std::cout << "If statements that will be evaluated: " << std::endl;
    statements()->print();
    std::cout << "if condition: " << std::endl;
    conditions()->print();
    std::cout << std::endl;
    std::cout << "Is true" << std::endl;
}

// Function List

FuncList::FuncList() {}

//void FuncList::evaluate(SymTab &funcTab) {
//    std::cout << "This does nothing" << std::endl;
//}

void FuncList::addFunc(std::string funcName, FuncStatement *func) {
    std::cout << "Adding " << func << " to the function list" << std::endl;
    symFunc[funcName] = func;
    std::cout << "Checking pointers "<< symFunc.find(funcName)->second << std::endl;
}

FuncStatement* FuncList::findFunc(std::string funcName) {
    std::cout << symFunc.find(funcName)->second << std::endl;
    return symFunc.find(funcName)->second;
}

//void FuncList::print() {
//    std::cout << "This is our function list" << std::endl;
//}


// Function statement

FuncStatement::FuncStatement() : _functionName{""}, _params{nullptr}, _funcStmts{nullptr} {}

FuncStatement::FuncStatement(std::string funcName, std::vector<std::string> funcParams, Statements *funcStmts ):
    _functionName{funcName}, _params{funcParams}, _funcStmts{funcStmts} {}

void FuncStatement::evaluate(SymTab &symTab) {
    //evaluate the function statements
    std::cout << "need to add" << std::endl;
    funcStatements()->evaluate(symTab);
}

std::vector<std::string> &FuncStatement::funcParams() {
    return _params;
}

std::string &FuncStatement::functionName() {
    return _functionName;
}

Statements *&FuncStatement::funcStatements() {
    return _funcStmts;
}

void FuncStatement::print() {
    std::cout << "Function print" << std::endl;
}

// Return object

ReturnStatement::ReturnStatement() : _returnExpr{nullptr} {}

ReturnStatement::ReturnStatement(ExprNode *expr):
    _returnExpr{expr} {}
// need to implement strings, variables, infix expression, and function calls
void ReturnStatement::evaluate(SymTab &symTab) {
    //std::cout << "doees nothing yet" << std::endl;
    //auto val = returnExpr()->evaluate(symTab);
    //symTab.setValueForRet(val);
    if (returnExpr()->token().isName()) {
            CallExpr *tempCall = dynamic_cast<CallExpr*>(returnExpr());
            if (tempCall != nullptr) {
                TypeDescriptor *funcCall = tempCall->evalFunc(symTab);
                if (funcCall == nullptr) {
                    std::cout << "Error with function:: printing with no return value " << std::endl;
                    exit(2);
                }
                else if (funcCall->checkIfInt()) {
                    //int rhs = l[i]->evaluate(symTab);
                    NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(funcCall);
                    int rhs = desc->value.intValue;
                    symTab.setValueForRet(rhs);
                    //consolePrint += std::to_string(rhs);
                }
                else {
                    //std::string rhs = l[i]->evaluateStr(symTab);
                    StringDescriptor *desc = dynamic_cast<StringDescriptor*>(funcCall);
                    std::string rhs = desc->value.stringValue;
                    symTab.setValueForRet(rhs);
                    //consolePrint += rhs;
                }
            }
            else {
                TypeDescriptor *desc = symTab.getValueFor(returnExpr()->token().getName());
                if (desc->checkIfInt()){
                    int rhs = returnExpr()->evaluate(symTab);
                    symTab.setValueForRet(rhs);
                }
                else {
                    std::string rhs = returnExpr()->evaluateStr(symTab);
                    symTab.setValueForRet(rhs);
                }
            }
        } // Handles string values
        else if (returnExpr()->token().isStringValue()) {
            std::string rhs = returnExpr()->evaluateStr(symTab);
            symTab.setValueForRet(rhs);
        } // Handles integer values
        else if (returnExpr()->token().isWholeNumber()) {
            int rhs = returnExpr()->evaluate(symTab);
            symTab.setValueForRet(rhs);
        } // Handles infix expressions for strings and ints
        else if (returnExpr()->token().isArithmeticOperator()){
            InfixExprNode *lNode = dynamic_cast<InfixExprNode*>(returnExpr());
            //lNode->right()->token().print();
            while(lNode->right()->token().isArithmeticOperator()) {
                lNode = dynamic_cast<InfixExprNode*>(lNode->right());
            }
            if (lNode->right()->token().isName()){
                TypeDescriptor *desc = symTab.getValueFor(lNode->right()->token().getName());
                    if (desc->checkIfInt()){
                        int rhs = returnExpr()->evaluate(symTab);
                        symTab.setValueForRet(rhs);
                    }
                    else {
                        std::string rhs = returnExpr()->evaluateStr(symTab);
                        symTab.setValueForRet(rhs);
                    }
            }
            else if (lNode->right()->token().isWholeNumber()) {
                int rhs = returnExpr()->evaluate(symTab);
                symTab.setValueForRet(rhs);
            }
            else {
                //rhsExpression()->token().print();
                std::string rhs = returnExpr()->evaluateStr(symTab);
                symTab.setValueForRet(rhs);
            }

        }
        else if (returnExpr()->token().isRelationalOperator()) {
            std::cout << std::endl;
            std::cout << std::endl;
            int rhs = returnExpr()->evaluate(symTab);
            symTab.setValueForRet(rhs);
        }
}


ExprNode *&ReturnStatement::returnExpr() {
    return _returnExpr;
}

void ReturnStatement::print() {
    std::cout << "return print" << std::endl;
}

// Call object

CallFunc::CallFunc() : _id{""}, _args{nullptr} {}
//std::string ID, std::vector<ExprNode*> arg_list
//_id{ID}, _args{arg_list}
CallFunc::CallFunc(ExprNode *call):
    _callExpr{call} {}

void CallFunc::evalParams(SymTab &symTab, std::vector<TypeDescriptor*> &storageList, ExprNode *value) {
    if (value->token().isName()) {
            TypeDescriptor *desc = symTab.getValueFor(value->token().getName());
            storageList.push_back(desc);
            /*
            if (desc->checkIfInt()){
                int rhs = value->evaluate(symTab);
                //symTab.setValueFor(lhsVariable(), rhs);
            }
            else {
                std::string rhs = value->evaluateStr(symTab);
                symTab.setValueForString(lhsVariable(), rhs);
            }
            */
        } // Handles string values
        else if (value->token().isStringValue()) {
            std::string rhs = value->evaluateStr(symTab);
            StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
            desc->value.stringValue = rhs;
            storageList.push_back(desc);
            //symTab.setValueForString(lhsVariable(), rhs);
        } // Handles integer values
        else if (value->token().isWholeNumber()) {
            int rhs = value->evaluate(symTab);
            NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            desc->value.intValue = rhs;
            storageList.push_back(desc);
            //symTab.setValueFor(lhsVariable(), rhs);
        } // Handles infix expressions for strings and ints
        else if (value->token().isArithmeticOperator()){
            InfixExprNode *lNode = dynamic_cast<InfixExprNode*>(value);
            lNode->right()->token().print();
            while(lNode->right()->token().isArithmeticOperator()) {
                lNode = dynamic_cast<InfixExprNode*>(lNode->right());
            }
            if (lNode->right()->token().isName()){
                TypeDescriptor *desc = symTab.getValueFor(lNode->right()->token().getName());
                storageList.push_back(desc);
                    /*
                    if (desc->checkIfInt()){
                        int rhs = value->evaluate(symTab);
                        symTab.setValueFor(lhsVariable(), rhs);
                    }
                    else {
                        std::string rhs = value->evaluateStr(symTab);
                        symTab.setValueForString(lhsVariable(), rhs);
                    }
                    */
            }
            else if (lNode->right()->token().isWholeNumber()) {
                int rhs = value->evaluate(symTab);
                NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
                desc->value.intValue = rhs;
                storageList.push_back(desc);
                //symTab.setValueFor(lhsVariable(), rhs);
            }
            else {
                //value->token().print();
                std::string rhs = value->evaluateStr(symTab);
                StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
                desc->value.stringValue = rhs;
                storageList.push_back(desc);
                //symTab.setValueForString(lhsVariable(), rhs);
            }

        }
        else if (value->token().isRelationalOperator()) {
            //std::cout << std::endl;
            //std::cout << std::endl;
            int rhs = value->evaluate(symTab);
            NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
            desc->value.intValue = rhs;
            storageList.push_back(desc);
            //symTab.setValueFor(lhsVariable(), rhs);
        }
}

void CallFunc::evaluate(SymTab &symTab) {
    
    CallExpr *cal = dynamic_cast<CallExpr*>(callOb());
    if (cal == nullptr) {
        std::cout << "Error in storing call node" << std::endl;
        exit(2);
    }
    //callOb()->evalFunc(symTab);
    cal->evalFunc(symTab);
    /*
    std::vector<ExprNode*> tempArgs;
    std::vector<TypeDescriptor*> evaluatedArgs;
    std::vector<std::string> tempParams;
    
    // Find function that is called
    //FuncStatement *currentFunc = findFunc(getFuncId());
    //FuncStatement *currentFunc = thisFunc();
    FuncStatement *currentFunc = symTab.findFunc(getFuncId());
    //std::cout << currentFunc << std::endl;
    //currentFunc->print();
    
    // Evaluate arglist expressions
    tempArgs = getArgList();
    //std::cout << "Here2" << std::endl;
    if (!tempArgs.empty()) {
        for (int i = 0; i < tempArgs.size(); i++) {
            //evaluatedArgs.push_back(tempArgs[i]->evaluate(symTab));
            evalParams(symTab, evaluatedArgs, tempArgs[i]);
        }
    }
    // store called function parameters
    tempParams = currentFunc->funcParams();
    // Call open scope
    symTab.openScope();
    // std::map<std::string, TypeDescriptor *> funcTab = symTab.openScope();
    // should return the symtab for the new scope (func symtab)
    
    // add arg expression for parameters to symtab
    //std::cout << "Here2" << std::endl;
    if (!tempArgs.empty()) {
        for (int i = 0; i < evaluatedArgs.size(); i++) {
            if (evaluatedArgs[i]->checkIfInt()) {
                NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(evaluatedArgs[i]);
                symTab.setValueFor(tempParams[i], desc->value.intValue);
            }
            else {
                StringDescriptor *desc = dynamic_cast<StringDescriptor*>(evaluatedArgs[i]);
                symTab.setValueForString(tempParams[i], desc->value.stringValue);
            }
        }
    }
    // evaluate the called function
    //std::cout << "Here3" << std::endl;
    currentFunc->evaluate(symTab);
    // handle return values
    returnValue() = symTab.getReturnVal();
    // close the scope
    symTab.closeScope();
    */
}

std::string &CallFunc::getFuncId() {
    return _id;
}

std::vector<ExprNode*> &CallFunc::getArgList(){
    return _args;
}

FuncStatement *&CallFunc::thisFunc() {
    return _thisFunc;
}

TypeDescriptor *&CallFunc::returnValue() {
    return _returnVal;
}

ExprNode *&CallFunc::callOb() {
    return _callExpr;
}

void CallFunc::print() {
    std::cout << "Called function" << getFuncId() << std::endl;
}







