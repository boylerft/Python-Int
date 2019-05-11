//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    std::string name;
    char c;
    while( inStream.get(c) && (isalnum(c) || c == '_')) {
            name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

std::string Tokenizer::readString(char d) {
    // This function is called when we know that 
    // we are about to parse in a string
    // The function reads and returns all characters of the string.
    
    std::string value;
    char c;
    while( inStream.get(c) && c != d) {
        value += c;
    }
    std::cout << value << std::endl;
    if(!inStream.good())
        exit(1);
    return value;
}

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        intValue = intValue * 10 + c - '0';
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

int Tokenizer::findIndentLen(bool &parsingNewLine) {
    char s;
    int indentCount = 0;
    while( inStream.get(s) && isspace(s))
        indentCount++;
    inStream.putback(s);
    parsingNewLine = false;
    return indentCount;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}, parsingNewLine{false}, parsingMultDedents{false}, multDedentsIndentCount{0} {
    //stack.push(0);
}

Token Tokenizer::getToken() {
    
    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }
    
    

    char c;
    char p;
    std::string s = "";
    
    Token token;
    
    if (parsingMultDedents) {
        //std::cout << "Indent 2count: " << multDedentsIndentCount << std::endl;
        //std::cout << "Stack 2top: " << stack.top() << std::endl;
        if (stack.size() == 0 && parsingMultDedents) {
                std::cout << "no matching indent, throwing error " << std::endl;
                exit(1);
            }
        else if (stack.top() == multDedentsIndentCount) {
            parsingMultDedents = false;
            multDedentsIndentCount = 0;
        }
        else {
            std::cout << "dedent2" << std::endl;
            stack.pop();
            token.dedent() = true;
            _tokens.push_back(token);
            return lastToken = token;
        }
    }
    
    while( inStream.get(c) && isspace(c) ) {// Skip spaces but not new-line chars.
        if (c == '#')
            break;
        if (c == '\n') {
            break;
        }
        if (parsingNewLine) {
            inStream.putback(c);
            int indentCount = findIndentLen(parsingNewLine);
            if (indentCount > 0 && isspace(c) ) {
                // Case 1
                if (stack.empty()){
                    stack.push(indentCount);
                    token.indent() = true;
                    //_tokens.push_back(token);
                }
                // Case 2
                else {
                    // 1
                    if (stack.top() == indentCount){
                        inStream.get(c);
                        break;
                    }
                    // 2
                    else if (stack.top() < indentCount) {
                        stack.push(indentCount);
                        token.indent() = true;
                        //_tokens.push_back(token);
                    }
                    // 3
                    else {
                            std::cout << "Indent count: " << indentCount << std::endl;
                            std::cout << "Stack top: " << stack.top() << std::endl;
                            if (stack.top() == indentCount) {
                                //token.dedent() = true;
                                parsingMultDedents = false;
                                multDedentsIndentCount = 0;
                                //break;
                            }
                            else {
                                std::cout << "dedent" << std::endl;
                                stack.pop();
                                token.dedent() = true;
                                parsingMultDedents = true;
                                multDedentsIndentCount = indentCount;
                                //_tokens.push_back(token);
                                //_tokens.push_back(token);
                                //return lastToken = token;
                                //break;
                            }
                        if (stack.size() == 0 && indentCount > 0){
                            std::cout << "no matching indent, throwing error " << std::endl;
                            exit(1);
                        }
                    }

                }
                _tokens.push_back(token);
                return lastToken = token;
            }

        }

    }

    if (c == '#') {
        while(c != '\n') {
            inStream.get(c);
        }
    }
    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }
    if (parsingNewLine && !inStream.eof()) {
        if (!stack.empty()) {
            inStream.putback(c);
            token.dedent() = true;
            stack.pop();
            _tokens.push_back(token);
            return lastToken = token;
        }
        else
            parsingNewLine = false;
    }
    
    if( inStream.eof()) {
        if (stack.size() > 0) {
            while (stack.size() > 0) {
                token.dedent() = true;
                stack.pop();
                _tokens.push_back(token);
                return lastToken = token;
            }
        }
        else
            token.eof() = true;
    } else if( c == '\n' ) {
        token.eol() = true;
        parsingNewLine = true;
    } else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber( readInteger() );

    } else if( c == '=' ){
        if((inStream.peek()) == '='){
            inStream.get(p);
            s = "==";
            token.reOp(s);
        }
        else{
            token.symbol(c);
        }
    }
    else if( c == '!'){
        inStream.get(p);
        s = "!=";
        token.reOp(s);
    }
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if( c == '<' || c == '>') {
        if((inStream.peek()) == '='){
            inStream.get(p);
            s.push_back(c);
            s.push_back(p);
            token.reOp(s);
        }
        else{
            s = c;
            token.reOp(s);
        }
    }
    else if( c == ';' )
        token.symbol(c);
    else if( c== ':' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if( c == '{' || c == '}')
        token.symbol(c);
    else if( c == '[' || c == ']')
        token.symbol(c);
    else if( c == ',')
        token.symbol(c);
    else if( c == '\'' || c == '"') {
        token.setStringValue( readString(c) );
    }
    else if(isalpha(c) || c == '_') {  // an identifier?
        inStream.putback(c);
        token.setName( readName() );
    } 
    else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
