#ifndef EXPRINTER_STRINGEXPR_HPP
#define EXPRINTER_STRINGEXPR_HPP

#include <string>
#include "Token.hpp"
#include "SymTab.hpp"

/*
class StringNode {
    public:
        StringNode(Token token);
        Token token();
        virtual void print() = 0;
        virtual std::string evaluate(SymTab &symTab) = 0;
    private:
        Token _token;
};
*/

class String: public ExprNode {
    public:
        String(Token token);
        std::string evaluate(SymTab &symTab);
        virtual void print();
        //virtual std::string evaluate(SymTab &symTab);
};

class VariableString: public ExprNode {
    public:
        VariableString(Token token);
        std::string evaluate(SymTab &symTab);
        virtual void print();
        //virtual std::string evaluate(SymTab &symTab);
};

#endif