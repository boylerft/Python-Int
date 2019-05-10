.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++17
objects =  Token.o Tokenizer.o Parser.o ArithExpr.o SymTab.o TypeDesc.o Statements.o main.o

statement.x: $(objects)
	g++ $(CFLAGS) -o statement.x $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@


Token.o:  Token.cpp Token.hpp
TypeDesc.o: TypeDesc.cpp TypeDesc.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
ArithExpr.o: ArithExpr.cpp ArithExpr.hpp Token.hpp SymTab.hpp Statements.hpp
SymTab.o: SymTab.cpp SymTab.hpp TypeDesc.hpp
Parser.o: Parser.cpp Token.hpp Parser.hpp Tokenizer.hpp SymTab.hpp ArithExpr.hpp TypeDesc.hpp Statements.hpp
Statements.o: Statements.cpp Statements.hpp ArithExpr.hpp Token.hpp SymTab.hpp TypeDesc.hpp
main.o: main.cpp Token.hpp Tokenizer.hpp Parser.hpp SymTab.hpp TypeDesc.hpp ArithExpr.hpp Statements.hpp

clean:
	rm -fr *.o *~ *.x
