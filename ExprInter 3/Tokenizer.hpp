//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include "Token.hpp"
#include <stack>

class Tokenizer {

public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
    void printProcessedTokens();
    int findIndentLen(bool &parsingNewLine);

private:
    Token lastToken;
    bool ungottenToken;
    std::ifstream &inStream;
    std::vector<Token> _tokens; 
    bool parsingNewLine;
    bool parsingMultDedents;
    int multDedentsIndentCount;

private:
    std::string readName();
    int readInteger();
    std::string readString(char d);
    std::stack <int> stack;
};

#endif //EXPRINTER_TOKENIZER_HPP
