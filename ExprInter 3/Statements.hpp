//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>
//#include <map>

#include "ArithExpr.hpp"
//#include "StringNode.hpp"
#include "SymTab.hpp"



// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;

};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    //void addFunc(std::string ID, Statement *func);
    //Statement* findFunc(std::string ID);
    void evaluate(SymTab &symTab);

    void print();

private:
    std::vector<Statement *> _statements;
    //std::map<std::string, Statement *> symFunc;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    // possibly change from string to expr
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr, std::vector<ExprNode*> arrayContent);
    
    //AssignmentStatementArray(std::string lhsVar, std::vector<ExprNode> rhsExpr);
    
    

    std::string &lhsVariable();
    ExprNode *&rhsExpression();
    std::vector<ExprNode*> &arrayContent();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
    std::vector<ExprNode*> _arrayContent;
};

// PrintStatement represents the notion of a print statement having the keyword print, and an 
// expression or variable to print
class PrintStatement : public Statement {
    public:
        PrintStatement();
        PrintStatement(std::vector<ExprNode*> printExpr);
        //PrintStatement(std::string printVar);
        
        //ExprNode *&printExpression();
        //std::string &printVariable();
        std::vector<ExprNode*> &printExpression();
    
        //void printValue(TypeDescriptor *desc);
        
        virtual void evaluate(SymTab &symTab);
        virtual void print();
    
    private:
        std::vector<ExprNode*> _printExpression;
        //std::string _printVariable;
};

// ForStatement represents the notion of running a for statement, with the 
// nested statements in the for loop

class ForStatement : public Statement {
    public:
        ForStatement();
        //ForStatement(AssignmentStatement *expr1, ExprNode *cond, AssignmentStatement *expr3, Statements *stmts);
        ForStatement(std::string var, forRange * r, std::vector<ExprNode*> list, Statements *stmts);
        std::string &forLoopVar();
        //std::vector<ExprNode*> TestList();
        std::vector<ExprNode*> &rangeList();
        forRange *&forLoopR();
        //AssignmentStatement();
        //AssignmentStatement *&expression1();
        //AssignmentStatement *&expression2();
        //ExprNode *&condition();
        Statements *&statements();
        //AssignmentStatement();
        
        virtual void evaluate(SymTab &symTab);
        virtual void print();
    
    private:
        // Assign 1
        //AssignmentStatement *_expr1;
        // Assign 2
        //AssignmentStatement *_expr2;
        // Condition
        //ExprNode *_condition;
        std::string _varName;
        std::vector<ExprNode*> _list;
        forRange *_r;
        Statements *_statements;
        
};

class IfStatement : public Statement {
    public:
        IfStatement();
        // Initializers 
        IfStatement(ExprNode *cond, Statements *stmts, Statements *elseStmts, std::vector<Statements*> stmtList, std::vector<ExprNode*> condList);
        
        ExprNode *&conditions();
        Statements *&statements();
        Statements *&elseStatements();
    
        std::vector<Statements*> &elifStmtList();
        std::vector<ExprNode*> &elifCondList();
        
        virtual void evaluate(SymTab &symTab);
        virtual void print();
    private:
        ExprNode *_conditions;
        Statements *_statements;
        Statements *_elseStatements;
        std::vector<Statements*> _elifStmtList;
        std::vector<ExprNode*> _elifCondList;
};

class ReturnStatement : public Statement {
    public:
        ReturnStatement();
        ReturnStatement(ExprNode *expr);
    
        ExprNode *&returnExpr();
    
        virtual void evaluate(SymTab &symTab);
        virtual void print();
    
    private:
        ExprNode *_returnExpr;
        
};


class FuncStatement : public Statement {
    public:
        FuncStatement();
        // func properties go here ie statements, parameters, if there is a return statement, func name
        FuncStatement(std::string funcName, std::vector<std::string> funcParms, Statements *funcStmts);
        
        std::vector<std::string> &funcParams();
        Statements *&funcStatements();
        std::string &functionName();
    
        virtual void evaluate(SymTab &symTab);
        virtual void print();
        
    private:
        Statements *_funcStmts;
        std::vector<std::string> _params;
        std::string _functionName;
        
};


class FuncList {
    public:
        FuncList();
        //FuncList(FuncStatement *func);
    
        //FuncStatement *&func();
    
        void addFunc(std::string funcName, FuncStatement *func);
        FuncStatement* findFunc(std::string funcName);
        
        //virtual void evaluate(SymTab &funcTab);
        //virtual void print();
    private:
        //FuncStatement _func;
        std::map<std::string, FuncStatement *> symFunc;
        //symTabFuncs
};


// Call object

class CallFunc : public Statement {
    public:
        CallFunc();
        // ID of function and args to the function
        //CallFunc(std::string ID, std::vector<ExprNode*> arg_list);
        CallFunc(ExprNode *call);
        // Evaluate the params
        void evalParams(SymTab &symTab, std::vector<TypeDescriptor*> &storageList, ExprNode *value);
        
        // Getter functions
        std::string &getFuncId();
        std::vector<ExprNode*> &getArgList();
        FuncStatement *&thisFunc();
        TypeDescriptor *&returnValue();
        ExprNode *&callOb();
    
        virtual void evaluate(SymTab &symTab);
        virtual void print();
    private:
        std::string _id;
        std::vector<ExprNode*> _args;
        FuncStatement *_thisFunc;
        ExprNode *_callExpr;
        //ReturnStatement *_returnVal;
        TypeDescriptor *_returnVal;
    
};

#endif //EXPRINTER_STATEMENTS_HPP
