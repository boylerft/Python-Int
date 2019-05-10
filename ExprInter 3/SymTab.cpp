//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"


// parameter changed to auto from int
void SymTab::setValueFor(std::string vName, int valueD) {
    // Define a variable by setting its initial value.
    std::map<std::string, TypeDescriptor *> symTab = scope.top();
    scope.pop();
    std::cout << vName << " <- " << valueD << std::endl;
    NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
    desc->value.intValue = valueD;
    symTab[vName] = desc;
    scope.push(symTab);
}

void SymTab::setValueForString(std::string vName, std::string valueD){
    std::map<std::string, TypeDescriptor *> symTab = scope.top();
    scope.pop();
    std::cout << vName << " <- " << valueD << std::endl;
    StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
    desc->value.stringValue = valueD;
    symTab[vName] = desc;
    scope.push(symTab);
}

void SymTab::setValueForArr(std::string vName, std::vector<int> elems) {
    std::map<std::string, TypeDescriptor *> symTab = scope.top();
    scope.pop();
    std::cout << vName << " <- array " << std::endl;
    ArrayDescriptor *desc = new ArrayDescriptor(TypeDescriptor::ARRAY);
    desc->value.intArr = elems;
    symTab[vName] = desc;
    scope.push(symTab);
}

void SymTab::setValueForRet(int valueD) {
    std::map<std::string, TypeDescriptor *> symTab = scope.top();
    scope.pop();
    std::cout << "return <- " << valueD << std::endl;
    NumberDescriptor *desc = new NumberDescriptor(TypeDescriptor::INTEGER);
    desc->value.intValue = valueD;
    std::string vName = "return";
    _isReturn = true;
    symTab[vName] = desc;
    scope.push(symTab);
}

void SymTab::setValueForRet(std::string valueD) {
    std::map<std::string, TypeDescriptor *> symTab = scope.top();
    scope.pop();
    std::cout << "return <- " << valueD << std::endl;
    StringDescriptor *desc = new StringDescriptor(TypeDescriptor::STRING);
    desc->value.stringValue = valueD;
    std::string vName = "return";
    _isReturn = true;
    symTab[vName] = desc;
    scope.push(symTab);
}


bool SymTab::isDefined(std::string vName) {
    std::map<std::string, TypeDescriptor *> symTab = scope.top();
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor *SymTab::getValueFor(std::string vName) {
    std::map<std::string, TypeDescriptor *> symTab = scope.top();
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    NumberDescriptor *desc = dynamic_cast<NumberDescriptor *>(symTab.find(vName)->second);
    if (desc == nullptr){
        StringDescriptor *sDesc = dynamic_cast<StringDescriptor *>(symTab.find(vName)->second);
        std::cout << "SymTab::getValueForStr: " << vName << " contains " << sDesc->value.stringValue << std::endl;
        return sDesc;
    }
    else {
        std::cout << "SymTab::getValueFor: " << vName << " contains " << desc->value.intValue << std::endl;
        return desc;
    }
}

//std::map<std::string, TypeDescriptor *> openScope()

void SymTab::openScope() {
    std::map<std::string, TypeDescriptor *> newScope;
    scope.push(newScope);
    //return newScope;
}

void SymTab::closeScope() {
    scope.pop();
}

TypeDescriptor *SymTab::getReturnVal() {
    if (_isReturn) {
        std::map<std::string, TypeDescriptor *> symTab = scope.top();
        NumberDescriptor *desc = dynamic_cast<NumberDescriptor *>(symTab.find("return")->second);
        //_isReturn = false;
        return desc;
    }
    else {
        TypeDescriptor *fake = nullptr;
        return fake;
    }
}

void SymTab::addFunc(std::string fName, FuncStatement* func) {
    funcTab[fName] = func;
}

FuncStatement *SymTab::findFunc(std::string fName) {
    return funcTab.find(fName)->second;
}

bool SymTab::getReturnBool() {
    return _isReturn;
}

void SymTab::setReturnBool() {
    _isReturn = false;
}

