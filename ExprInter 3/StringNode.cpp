#include<iostream>
#include "StringNode.hpp"

// StringNode
StringNode::StringNode(Token token): _token{token} {}

Token StringNode::token() { return _token; }

// String
String::String(Token token): StringNode{token} {}

void String::print() {
    token().print();
}

std::string String::evaluate(SymTab &symTab) {
    std::cout << "String::evaluate: returning " << token().getStringValue() << std::endl;
    return token().getStringValue();
}   

VariableString::VariableString(Token token): StringNode{token} {}

void VariableString::print() {
    token().print();
}

std::string VariableString::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValueFor(token().getName());
}