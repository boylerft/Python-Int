//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <vector>
#include <map>
#include "TypeDesc.hpp"
#include <stack>
//#include "ArithExpr.hpp"
//#include "ArithExpr.hpp"
//#include "Statements.hpp"
//#include "Statements.hpp"
class FuncStatement;
// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

class SymTab {
public:
    void setValueFor(std::string vName, int valueD);
    void setValueForString(std::string vName, std::string valueD);
    void setValueForArray(std::string vName, std::vector<int> intarray, std::vector<std::string> strarray);
    void setValueForRet(int valueD);
    void setValueForRet(std::string valueD);
    bool isDefined(std::string vName);
    TypeDescriptor *getValueFor(std::string vName);
    //std::map<std::string, TypeDescriptor *> openScope();
    void openScope();
    void closeScope();
    
    TypeDescriptor *getReturnVal();
    
    void addFunc(std::string fName, FuncStatement* func);
    FuncStatement *findFunc(std::string fName);
    
    bool getReturnBool();
    void setReturnBool();

private:
    //std::map<std::string, TypeDescriptor *> symTab;
    std::stack <std::map<std::string, TypeDescriptor *>> scope;
    std::map<std::string, FuncStatement *> funcTab;
    bool _isReturn;
};

//#include "Statements.hpp"

#endif //EXPRINTER_SYMTAB_HPP

