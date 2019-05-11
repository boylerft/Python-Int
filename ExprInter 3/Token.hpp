//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
    Token();

    bool &eof()  { return _eof; }
    bool &eol()  { return _eol; }

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }
    bool isOpenBracket() const { return _symbol == '{'; }
    bool isClosedBracket() const { return _symbol == '}'; }
    bool isOpenBrack() const { return _symbol == '['; }
    bool isCloseBrack() const { return _symbol == ']'; }
    bool isComma() const { return _symbol == ','; }
    bool isIndent() const { return _indent; }
    bool isDedent() const { return _dedent; }
    bool isColon() const { return _symbol == ':'; }
    
    bool &indent() { return _indent; }
    bool &dedent() { return _dedent; }
    
    //bool indent() const { return _indent; }
    //bool dedent() const { return _dedent; }
    
    //void indent() {_indent = true; }
    //void dedent() {_dedent = true; }
    void indentInc() { _indentLvl++; }
    void dedentDec() { _indentLvl--; }
    int indentLvl() { return _indentLvl; }
    
    
    //bool isOr() const { return _cond == "or"; }
    //bool isAnd() const { return _cond == "and"; }
    //bool isNot() const { return _cond == "not"; }
    
    //bool isNewLine() const { return _symbol == '\n'; }

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }
    
    void reOp(std::string r) { _reOp = r; }
    std::string reOp() { return _reOp; }
    
    bool isPeriod() const {return _symbol == '.'; }
    bool isSemiColon() const { return _symbol == ';'; }
    bool isApostrophe() const { return _symbol == '\''; }
    bool isQuote() const { return _symbol == '"'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }
    
    //void reOp(std::string r) { _reOp = r; }
    //std::string reOp() { return _reOp; }
    
    bool isEqualOperator() const {return _reOp == "=="; }
    bool isNotEqualOperator() const {return _reOp == "!="; }
    bool isLessThanOperator() const {return _reOp == "<"; }
    bool isGreaterThanOperator() const {return _reOp == ">"; }
    bool isLessThanOrEqualOperator() const {return _reOp == "<="; }
    bool isGreaterThanOrEqualOperator() const {return _reOp == ">="; }
    bool isRelationalOperator() const {
        return isEqualOperator() ||
                isNotEqualOperator() ||
                isLessThanOperator() ||
                isGreaterThanOperator() ||
                isLessThanOrEqualOperator() ||
                isGreaterThanOrEqualOperator();
    }
    bool isOr() const { return _name == "or"; }
    bool isAnd() const { return _name == "and"; }
    bool isNot() const { return _name == "not"; }
    
    bool isDef() const { return _name == "def"; }
    bool isReturn() const { return _name == "return"; }
    
    bool isBooleanExpression() const {
        return isOr() || isAnd() || isNot();
    }
    
    bool isName() const                   { return _name.length() > 0; }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }
    
    bool isKeyword() const { return _name == "for" || _name == "print" || _name == "or" || _name == "and" || _name == "not" || _name == "if" || _name == "def" || _name == "return"; }
    
    bool isElseKey() const { return _name == "else"; }
    bool isElifKey() const { return _name == "elif"; }
    bool isInKey() const { return _name == "in"; }
    bool isRangeKey() const { return _name == "range"; }
    
    bool isPopKey() const { return _name == "pop"; }
    bool isPushKey() const {return _name == "push"; }
    bool isArrOp() const {
        return isPopKey() || isPushKey();
    }
    
    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }
    
   // bool &isStringValue() { return _isStringValue; }
   // bool isStringValue() const { return _isStringValue; }
    bool isStringValue() const { return _stringValue.length() > 0; }
    std::string getStringValue() const { return _stringValue; }
    void setStringValue(std::string s) {
        _stringValue = s;
        //isStringValue() = true;
    }

    void print() const;

private:
    int _indentLvl;
    std::string _name;
    bool _eof, _eol;
    bool _indent;
    bool _dedent;
    bool _isWholeNumber;
    bool _isStringValue;
    char _symbol;
    std::string _reOp;
    std::string _cond;
    int _wholeNumber;
    std::string _stringValue;
};

#endif //EXPRINTER_TOKEN_HPP
