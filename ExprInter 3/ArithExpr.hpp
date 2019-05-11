//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_ARITHEXPR_HPP
#define EXPRINTER_ARITHEXPR_HPP

#include "Token.hpp"
#include "SymTab.hpp"
#include <vector>

// Classes in this file define the internal representation of arithmetic expressions.
//class FuncStatement;

// An ExprNode serves as the base class (super class) for arithmetic expression.
// It forces the derived classes (subclasses) to implement two functions, print and
// evaluate.
class ExprNode {
public:
    ExprNode(Token token);
    Token token();
    virtual void print() = 0;
    virtual int evaluate(SymTab &symTab) = 0;
    virtual std::string evaluateStr(SymTab &symTab) = 0;

private:
    Token _token;
};


// An InfixExprNode is useful to represent binary arithmetic operators.
class InfixExprNode: public ExprNode {  // An expression tree node.

public:
    InfixExprNode(Token tk);

    ExprNode *&left();
    ExprNode *&right();
    bool checkType(SymTab &symTab);
    virtual void print();
    virtual int evaluate(SymTab &symTab);
    virtual std::string evaluateStr(SymTab &symTab);

private:
    ExprNode *_left, *_right;
};

// WholeNumber is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class WholeNumber: public ExprNode {
public:
    WholeNumber(Token token);
    virtual void print();
    virtual int evaluate(SymTab &symTab);
    virtual std::string evaluateStr(SymTab &symTab);
};

// Varialbe is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class Variable: public ExprNode {
public:
    Variable(Token token);
    virtual void print();
    virtual int evaluate(SymTab &symTab);
    virtual std::string evaluateStr(SymTab &symTab);
};

class String: public ExprNode {
    public:
        String(Token token);
        virtual void print();
        virtual int evaluate(SymTab &symTab);
        //std::string evaluateStr(SymTab &symTab);
        virtual std::string evaluateStr(SymTab &symTab);
        //virtual std::string evaluate(SymTab &symTab);
};

class forRange: public ExprNode {
    public:
        forRange(Token token);
        
        void setRange(std::vector<ExprNode*> itr, SymTab &symTab);
        // function to tell you to continue iteration
        bool rangeCont();
        // function that gets the next value
        int getNext();
    
        int &getStart();
        int &getStop();
        int &getStep();
        virtual void print();
        virtual int evaluate(SymTab &symTab);
        virtual std::string evaluateStr(SymTab &symTab);
    private:
        std::vector<ExprNode*> _itr;
        int _start;
        int _stop;
        int _step;
};

class CallExpr: public ExprNode {
    public:
        CallExpr(Token token);
    
        std::string &getFuncName();
        std::vector<ExprNode*> &getArgList();
        TypeDescriptor *&getReturn();
        TypeDescriptor *evalFunc(SymTab &symTab);
        void evalParams(SymTab &symTab, std::vector<TypeDescriptor*> &storageList, ExprNode *value);
    
        virtual void print();
        virtual int evaluate(SymTab &symTab);
        virtual std::string evaluateStr(SymTab &symTab);
    private:
        std::string _funcName;
        std::vector<ExprNode*> _args;
        TypeDescriptor *_returnVal;
        
};

class LenArray: public ExprNode {
    public:
        LenArray(Token token);
        virtual void print();
        virtual int evaluate(SymTab &symTab);
        virtual std::string evaluateStr(SymTab &symTab);
        
        std::string &arrID();
    private:
        std::string _arrayId;
};

#include "Statements.hpp"
#endif //EXPRINTER_ARITHEXPR_HPP
