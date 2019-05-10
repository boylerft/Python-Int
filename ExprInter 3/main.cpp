#include <iostream>
#include<vector>


#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {

    if( argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }

    std::ifstream inputStream;

    inputStream.open(argv[1], std::ios::in);
    if( ! inputStream.is_open()) {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        perror("Error when attempting to open the input file.");
        exit(2);
    }

    Tokenizer tokenizer(inputStream);
    Parser parser(tokenizer);
    SymTab symTab;
    Statements *statements = parser.statements(symTab);
    //SymTab symTab;
    symTab.openScope();

    std::cout << "Processed tokens" << std::endl;
    tokenizer.printProcessedTokens();
    statements->print();
    statements->evaluate(symTab);
    
    return 0;
}
