//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include "ArithExpr.hpp"

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }



// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

// checks type of the right node of expr. Returns true if its 
// and int, false if it is a string
bool InfixExprNode::checkType(SymTab &symTab) {
    InfixExprNode *rNode = dynamic_cast<InfixExprNode*>(right());
    InfixExprNode *lNode = dynamic_cast<InfixExprNode*>(left());
    
    ExprNode *node = nullptr;
    if (rNode == nullptr) {
        node = dynamic_cast<ExprNode*>(right());
        if (node->token().isName()) {
        TypeDescriptor *desc = symTab.getValueFor(node->token().getName());
        if (desc->checkIfStr())
            return true;
        else 
            return false;
        }
        else if (node->token().isStringValue()) {
            return true;
        }
        else 
            return false;
    }
    else if (lNode == nullptr) {
        node = dynamic_cast<ExprNode*>(left());
        if (node->token().isName()) {
        TypeDescriptor *desc = symTab.getValueFor(node->token().getName());
        if (desc->checkIfStr())
            return true;
        else 
            return false;
        }
        else if (node->token().isStringValue()) {
            return true;
        }
        else 
            return false;
    }
    else
        return false;
}


int InfixExprNode::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
        
    if (!checkType(symTab)) {
        int lValue = left()->evaluate(symTab);
        int rValue = right()->evaluate(symTab);

        std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
        if( token().isAdditionOperator() )
            return lValue + rValue;
        else if(token().isSubtractionOperator())
            return lValue - rValue;
        else if(token().isMultiplicationOperator())
            return lValue * rValue;
        else if(token().isDivisionOperator())
            return lValue / rValue; // division by zero?
        else if( token().isModuloOperator() )
            return lValue % rValue;
        else if ( token().isEqualOperator() )
            return lValue == rValue;
        else if ( token().isNotEqualOperator() )
            return lValue != rValue;
        else if ( token().isLessThanOperator() )
            return lValue < rValue;
        else if ( token().isGreaterThanOperator() )
            return lValue > rValue;
        else if ( token().isLessThanOrEqualOperator() )
            return lValue <= rValue;
        else if ( token().isGreaterThanOrEqualOperator() )
            return lValue >= rValue;
        else if (token().isOr())
            return lValue || rValue;
        else if (token().isAnd())
            return lValue && rValue;
        else if (token().isNot())
            return 0;
        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }
    else {
        std::string lValue = left()->evaluateStr(symTab);
        std::string rValue = right()->evaluateStr(symTab);
        
        if ( token().isEqualOperator() )
            return lValue == rValue;
        else if ( token().isNotEqualOperator() )
            return lValue != rValue;
        else if ( token().isLessThanOperator() )
            return lValue < rValue;
        else if ( token().isGreaterThanOperator() )
            return lValue > rValue;
        else if ( token().isLessThanOrEqualOperator() )
            return lValue <= rValue;
        else if ( token().isGreaterThanOrEqualOperator() )
            return lValue >= rValue;
        else {
            std::cout << "InfixExprNode::evaluateStr RelOp: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }
    
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

std::string InfixExprNode::evaluateStr(SymTab &symTab) {
    std::string lValue = left()->evaluateStr(symTab);
    std::string rValue = right()->evaluateStr(symTab);
    std::cout << "InfixExprNode::evaluateStr: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
    
    if(token().isAdditionOperator())
        return lValue + rValue;
    else {
        std::cout << "InfixExprNode::evaluateStr: invalid operator for string evaluation\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
        
}

// Range functions
forRange::forRange(Token token) : ExprNode{token}, _start{0}, _stop{0}, _step{1}, _itr{nullptr} {}

void forRange::setRange(std::vector<ExprNode*> itr, SymTab &symTab) {
    if (itr.size() == 3) {
        for (int i = 0; i < itr.size(); i++) {
            int rhs;
            CallExpr *tempCall = dynamic_cast<CallExpr*>(itr[i]);
            if (tempCall != nullptr) {
                TypeDescriptor *funcCall = tempCall->evalFunc(symTab);
                if (funcCall == nullptr) {
                    std::cout << "Error with function:: returning with no return value " << std::endl;
                    exit(2);
                }
                else if (funcCall->checkIfInt()) {
                    //int rhs = l[i]->evaluate(symTab);
                    NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(funcCall);
                    rhs = desc->value.intValue;
                    //consolePrint += std::to_string(rhs);
                }
            }
            else {
                rhs = itr[i]->evaluate(symTab);
            }
            if (i == 0)
                getStart() = rhs;
            else if (i == 1)
                getStop() = rhs;
            else if (i == 2)
                getStep() = rhs;
        }
        /*
        getStart() = itr[0]->evaluate(symTab);
        getStop() = itr[1]->evaluate(symTab);
        getStep() = itr[2]->evaluate(symTab);
        */
    }
    else if (itr.size() == 2) {
        std::cout << "Here for range" << std::endl;
        for (int i = 0; i < itr.size(); i++) {
            int rhs;
            CallExpr *tempCall = dynamic_cast<CallExpr*>(itr[i]);
            if (tempCall != nullptr) {
                TypeDescriptor *funcCall = tempCall->evalFunc(symTab);
                if (funcCall == nullptr) {
                    std::cout << "Error with function:: returning with no return value " << std::endl;
                    exit(2);
                }
                else if (funcCall->checkIfInt()) {
                    //int rhs = l[i]->evaluate(symTab);
                    NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(funcCall);
                    rhs = desc->value.intValue;
                    //consolePrint += std::to_string(rhs);
                }
            }
            else {
                rhs = itr[i]->evaluate(symTab);
            }
            if (i == 0)
                getStart() = rhs;
            else if (i == 1)
                getStop() = rhs;
        }
        getStep() = 1;
        /*
        getStart() = itr[0]->evaluate(symTab);
        getStop() = itr[1]->evaluate(symTab);
        getStep() = 1;
        */
    }
    else if (itr.size() == 1){
            int rhs;
            CallExpr *tempCall = dynamic_cast<CallExpr*>(itr[0]);
            if (tempCall != nullptr) {
                TypeDescriptor *funcCall = tempCall->evalFunc(symTab);
                if (funcCall == nullptr) {
                    std::cout << "Error with function:: returning with no return value " << std::endl;
                    exit(2);
                }
                else if (funcCall->checkIfInt()) {
                    //int rhs = l[i]->evaluate(symTab);
                    NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(funcCall);
                    rhs = desc->value.intValue;
                    //consolePrint += std::to_string(rhs);
                }
            }
            else {
                rhs = itr[0]->evaluate(symTab);
            }
        getStop() = rhs;
        getStart() = 0;
        getStep() = 1;
        /*
        getStop() = itr[0]->evaluate(symTab);
        getStart() = 0;
        getStep() = 1;
        */
    }
}
int &forRange::getStart() {
    return _start;
}
int &forRange::getStop() {
    return _stop;
}
int &forRange::getStep() {
    return _step;
}
// Might need to use the declared var instead of getStart
// getStart might just be used to assign the for variable
bool forRange::rangeCont() {
    if (getStart() < 0 || getStop() < 0) {
        if (getStart() > getStop())
            return false;
        else
            return true;
    }
    else if (getStart() < getStop() )
        return false;
    else if (getStart() > getStop())
        return true;
    else
        return true;
}

int forRange::getNext() {
    getStart() += getStep();
    return getStart();
}

void forRange::print() {
    std::cout << "forRange" << std::endl;
}
int forRange::evaluate(SymTab &symTab) {
    return 0;
}
std::string forRange::evaluateStr(SymTab &symTab) {
    return "0";
}

// WHoleNumber
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

int WholeNumber::evaluate(SymTab &symTab) {
    std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return token().getWholeNumber();
}

std::string WholeNumber::evaluateStr(SymTab &symTab) {
    return "0";
}

// String
String::String(Token token): ExprNode{token} {}

void String::print() {
    token().print();
}

int String::evaluate(SymTab &symTab) {
    return 0;
}

std::string String::evaluateStr(SymTab &symTab) {
    std::cout << "String::evaluate: returning " << token().getStringValue() << std::endl;
    return token().getStringValue();
}

// Variable

Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

int Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    NumberDescriptor *desc = dynamic_cast<NumberDescriptor *>(symTab.getValueFor(token().getName()));
    std::cout << "Variable::evaluate: returning " << desc->value.intValue << std::endl;
    return desc->value.intValue;
}

std::string Variable::evaluateStr(SymTab &symTab) {
    if ( ! symTab.isDefined(token().getName())){
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    StringDescriptor *desc = dynamic_cast<StringDescriptor *>(symTab.getValueFor(token().getName()));
    std::cout << "Variable::evaluateStr: returning " << desc->value.stringValue << std::endl;
    return desc->value.stringValue;
}

// Call object

CallExpr::CallExpr(Token token): ExprNode{token}, _args{nullptr}, _funcName{""}, _returnVal{nullptr} {}

void CallExpr::print() {
    token().print();
}

int CallExpr::evaluate(SymTab &symTab) {
    // should never be called
    return 0;
}

std::string CallExpr::evaluateStr(SymTab &symTab) {
    // shoule never be called
    return "0";
}

TypeDescriptor *CallExpr::evalFunc(SymTab &symTab) {
    std::cout << "Starting eval on func: " << getFuncName() << std::endl;
    std::vector<ExprNode*> tempArgs;
    std::vector<TypeDescriptor*> evaluatedArgs;
    std::vector<std::string> tempParams;
    
    // Get func for call object
    FuncStatement *currentFunc = symTab.findFunc(getFuncName());
    //std::cout << "func name: " << getFuncName() << std::endl;
    
    tempArgs = getArgList();
    // Evaluate passed args
    if (!tempArgs.empty()) {
        for (int i = 0; i < tempArgs.size(); i++) {
            //evaluatedArgs.push_back(tempArgs[i]->evaluate(symTab));
            evalParams(symTab, evaluatedArgs, tempArgs[i]);
        }
    }
    // get func params
    tempParams = currentFunc->funcParams();
    std::cout << "Came here" << std::endl;
    // open new scope
    symTab.openScope();
    
    // add new args to the scope with the func params
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
    // Evaluate the func statements
    currentFunc->evaluate(symTab);
    
    // handle return values
    getReturn() = symTab.getReturnVal();
    //std::cout << "hhhdhdhdhdhd" << std::endl;
    // close the scope
    symTab.closeScope();
    symTab.setReturnBool();
    //TypeDescriptor *dummy = nullptr;
    return getReturn();
    
}
void CallExpr::evalParams(SymTab &symTab, std::vector<TypeDescriptor*> &storageList, ExprNode *value) {
    if (value->token().isName()) {
            CallExpr *tempCall = dynamic_cast<CallExpr*>(value);
            if (tempCall != nullptr) {
                TypeDescriptor *funcCall = tempCall->evalFunc(symTab);
                if (funcCall == nullptr) {
                    std::cout << "Error with function:: printing with no return value " << std::endl;
                    exit(2);
                }
                else if (funcCall->checkIfInt()) {
                    //int rhs = l[i]->evaluate(symTab);
                    NumberDescriptor *desc = dynamic_cast<NumberDescriptor*>(funcCall);
                    storageList.push_back(desc);
                    //int rhs = desc->value.intValue;
                    //consolePrint += std::to_string(rhs);
                }
                else {
                    //std::string rhs = l[i]->evaluateStr(symTab);
                    StringDescriptor *desc = dynamic_cast<StringDescriptor*>(funcCall);
                    storageList.push_back(desc);
                    //std::string rhs = desc->value.stringValue;
                    //consolePrint += rhs;
                }
            }
            else {
                TypeDescriptor *desc = symTab.getValueFor(value->token().getName());
                storageList.push_back(desc);
            }
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

std::string &CallExpr::getFuncName() {
    return _funcName;
}

std::vector<ExprNode*> &CallExpr::getArgList() {
    return _args;
}

TypeDescriptor *&CallExpr::getReturn() {
    return _returnVal;
}
