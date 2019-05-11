//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false} , _indent{false}, _dedent{false} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isOpenBracket() ) std::cout << "{" ;
    else if( isClosedBracket() ) std::cout << "}" ;
    else if( isOpenBrack() ) std::cout << "[" ;
    else if( isCloseBrack() ) std::cout << "]" ;
    else if( isComma() ) std::cout << ",";
    else if( isPeriod() )   std::cout << ".";
    else if( isApostrophe() ) std::cout << '\'' ;
    else if( isQuote() ) std::cout << '"' ;
    else if( isIndent() ) std::cout << "Indent found" ;
    else if( isDedent() ) std::cout << "Dedent found" ;
    else if( isInKey() ) std::cout << "in keyword found" ;
    else if( isRangeKey() ) std::cout << "range keyword found" ;
    else if( isDef() ) std::cout << "function found" ;
    else if( isLenKey() ) std::cout << "len keyword found" ;
    else if( isPopKey() ) std::cout << "pop keyword found" ;
    else if( isPushKey() )std::cout << "push keyword found";
    else if( isReturn() ) std::cout << "Return statement found" ;
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isColon() ) std::cout << ":" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isEqualOperator() ) std::cout << " == " ;
    else if( isNotEqualOperator() ) std::cout << " != " ;
    else if( isLessThanOperator() ) std::cout << " < " ;
    else if( isGreaterThanOperator() ) std::cout << " > " ;
    else if( isLessThanOrEqualOperator() ) std::cout << " <= " ;
    else if( isGreaterThanOrEqualOperator() ) std::cout << " >= " ;
    else if( isKeyword() ) std::cout << "Key word " << getName() << " was found. \n"; 
    else if( isName() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isStringValue() ) std::cout << getStringValue();
    else std::cout << "Uninitialized token.\n";
}
