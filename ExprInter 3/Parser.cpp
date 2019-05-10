
/*  The majority of the work is done by the class 'convert'.
    This class builds an expression tree using the input infix
    expression.  A post-order traversal of the expression tree 'dumps'
    it into an array in postfix form.  The iterator copies the token
    from this array to user's arrays.

*/

#include<vector>
#include<iostream>
#include<stack>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"


// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}
// need to change to handle assignmeent statements and array calls
Statements *Parser::statements(SymTab &symTab) {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    //std::cout << "Starting new parser" << std::endl;
    Statements *stmts = new Statements();
    // make the function list to store functions
    //FuncList *funcList = new FuncList();
    Token tok = tokenizer.getToken();
    //tok.print();
    //std::cout << "Starting new parser" << std::endl;
    while (tok.isName() || tok.eol()) {
        //tok.print();
        //std::cout << std::endl;
        if(tok.isKeyword()){
            if (tok.getName() == "print"){
                tokenizer.ungetToken();
                PrintStatement *printStmt = printStatement();
                stmts->addStatement(printStmt);
                tok = tokenizer.getToken();
            }
            if (tok.getName() == "for"){
                tokenizer.ungetToken();
                ForStatement *forStmt = forStatement(symTab);
                stmts->addStatement(forStmt);
                tok = tokenizer.getToken(); 
            }
            if (tok.getName() == "if") {
                tokenizer.ungetToken();
                IfStatement *ifStmt = ifStatement(symTab);
                stmts->addStatement(ifStmt);
                tok = tokenizer.getToken();
            }
            if (tok.getName() == "def") {
                tokenizer.ungetToken();
                FuncStatement *func = funcStatement(symTab);
                //std::cout << "function: "<<func << std::endl;
                //std::cout << "function list: "<<funcList << std::endl;
                //stmts->addFunc(func->functionName(), func);
                //std::cout << "Name: " << func->functionName() << std::endl;
                //funcList->addFunc(func->functionName(), func);
                symTab.addFunc(func->functionName(), func);
                tok = tokenizer.getToken();
            }
            if (tok.getName() == "return") {
                tokenizer.ungetToken();
                ReturnStatement *ret = returnStatement();
                stmts->addStatement(ret);
                tok = tokenizer.getToken();
            }
        }
        else if (tok.eol()) {
            tok = tokenizer.getToken();
            continue;
        }
        // change here for statements that have names tied to them
        // change assignment statement so that it only returns the expr.
        // then back here in the main parser, set the name value using
        // setter function
        else {
            std::string name = tok.getName();
            Token fName = tok;//tokenizer.getToken();
            tok = tokenizer.getToken();
            //tok.print();
            if (tok.isAssignmentOperator()) {
                tokenizer.ungetToken();
                AssignmentStatement *assignStmt = assignStatement();
                
                assignStmt->lhsVariable() = name;
                stmts->addStatement(assignStmt);
                tok = tokenizer.getToken();
            }
            else if (tok.isOpenParen()) {
                //tokenizer.ungetToken();
                //tokenizer.ungetToken();
                CallFunc *caller = callStatement(fName);
                //std::cout << "func name: " << name << std::endl;
                //std::cout << "Checking caller parser: "<< funcList->findFunc(caller->getFuncId()) << std::endl;
                //std::cout << "FunctionList: " << funcList << std::endl;
                //caller->getFuncId() = name;
                //caller->thisFunc() = funcList->findFunc(caller->getFuncId());
                //caller->getFuncId() = name;
                stmts->addStatement(caller);
                tok = tokenizer.getToken();
                // call function here
                //tok = tokenizer.getToken();
            }
            //tok = tokenizer.getToken();
        }
    }
    tokenizer.ungetToken();
    return stmts;
}

ReturnStatement *Parser::returnStatement() {
    Token returnName = tokenizer.getToken();
    if (!returnName.isReturn())
        die("Parser::returnStatement", "Expected return name, instead got", returnName);
    
    Token checkForEnd = tokenizer.getToken();
    ExprNode *values = nullptr;
    
    if (!checkForEnd.eol()) {
        tokenizer.ungetToken();
        values = test();
    }
    else
        tokenizer.ungetToken();
    return new ReturnStatement(values);
}

CallFunc *Parser::callStatement(Token fName) {
    std::cout << "Here" << std::endl;
    // parse the call statement here
    //Token callName = tokenizer.getToken();
    //if (!callName.isName())
    //    die("Parser::callStatement", "Expected a name token, instead got", callName);
    //Token openParen = tokenizer.getToken();
    //if (!openParen.isOpenParen())
        //die("Parser::callStatement", "Expected an open parenthesis, instead got", openParen);
    ExprNode *call = callExpr(fName);
    /*
    Token checkParen = tokenizer.getToken();
    std::vector<ExprNode*> argList;
    if (!checkParen.isCloseParen()) {
        tokenizer.ungetToken();
        argList = testList();
    }
    Token closeParen = tokenizer.getToken();
    if (!closeParen.isCloseParen())
        die("Parser::callStatement", "Expected a close parenthesis, instead got", closeParen);
    std::string temp = ""; //callName.getName()
    */
    //return new CallFunc(temp, argList);
    return new CallFunc(call);
}

ExprNode *Parser::callExpr(Token fName) {
    Token checkParen = tokenizer.getToken();
    std::vector<ExprNode*> argList;
    if (!checkParen.isCloseParen()) {
        tokenizer.ungetToken();
        argList = testList();
        Token closeParen = tokenizer.getToken();
        if (!closeParen.isCloseParen())
            die("Parser::callExpr", "Expected a close parenthesis, instead got", closeParen);
        CallExpr *c = new CallExpr(fName);
        c->getArgList() = argList;
        c->getFuncName() = fName.getName();
        //std::string temp = ""; //callName.getName()
        return c;     
    }
    else {
        //Token closeParen = tokenizer.getToken();
        if (!checkParen.isCloseParen())
            die("Parser::callExpr", "Expected a close parenthesis, instead got", checkParen);
        CallExpr *c = new CallExpr(fName);
        c->getArgList() = argList;
        c->getFuncName() = fName.getName();
        //std::string temp = ""; //callName.getName()
        return c;
    }
}

AssignmentStatement *Parser::assignStatement() {
    //Token varName = tokenizer.getToken();
    //if (!varName.isName())
     //   die("Parser::assignStatement", "Expected a name token, instead got", varName);
    //Token assignOp = tokenizer.getToken();
    //if (!assignOp.isAssignmentOperator())
    //    die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = nullptr;
    std::vector<ExprNode*> arrayContent;
    Token openBrack = tokenizer.getToken();
    if (openBrack.isOpenBrack()) {
        tokenizer.ungetToken();
        arrayContent = arrayInit();
    }
    else
        rightHandSideExpr = test();
    Token endOfLine = tokenizer.getToken();
    if (!endOfLine.eol())
        die("Parser::assignStatement", "Expected an end of line character, instead got", endOfLine);
    
    std::string temp = "";
        //varName.getName()
    return new AssignmentStatement(temp, rightHandSideExpr, arrayContent);
}



PrintStatement *Parser::printStatement() {
    Token printName = tokenizer.getToken();
    if (!printName.isKeyword())
        die("Parser::printStatement", "Expected print key word token, instead got", printName);
    
    std::vector<ExprNode*> list = testList();
    
    Token endOfLine = tokenizer.getToken();
    if (!endOfLine.eol())
        die("Parser::printStatement", "Expected end of line, instead got", endOfLine);
    
    return new PrintStatement(list);
    
}

ForStatement *Parser::forStatement(SymTab &symTab) {
    // For Keyword
    Token forName = tokenizer.getToken();
    if(!forName.isKeyword())
        die("Parser::forStatement", "Expected for key word token, instead got", forName);
    
    // forVar
    Token forVar = tokenizer.getToken();
    if(!forVar.isName())
        die("Parser::forStatement", "Expected for loop variable, instead got", forVar);
    
    // For in keyword
    Token forInKey = tokenizer.getToken();
    if(!forInKey.isInKey())
        die("Parser::forStatement", "Expected in keyword, instead got", forInKey);
    
    // Range keyword
    Token rangeKeyword = tokenizer.getToken();
    if(!rangeKeyword.isRangeKey())
        die("Parser::forStatement", "Expected range keyword, instead got", rangeKeyword);
    
    // First parenthesis
    Token firstParen = tokenizer.getToken();
    if(!firstParen.isOpenParen())
        die("Parser::forStatement", "Expected open parenthesis, instead got", firstParen);
    
    // Call testlist to parse the conditions of the range function
    std::vector<ExprNode*> testlist = testList();
    forRange *r = new forRange(rangeKeyword);
    
    // Closing parenthesis
    Token secParen = tokenizer.getToken();
    if(!secParen.isCloseParen())
        die("Parser::forStatement", "Expected closing parenthesis, instead got", secParen);
    
    // For colon
    Token colon = tokenizer.getToken();
    if(!colon.isColon())
        die("Parser::forStatement", "Expected colon, instead got", colon);
    Statements *forstmts = suite(symTab);

    return new ForStatement(forVar.getName(), r, testlist, forstmts);
        
}

IfStatement *Parser::ifStatement(SymTab &symTab) {
    // If keyword
    Token ifKey = tokenizer.getToken();
    if (ifKey.getName() != "if")
        die("Parser::IfStatment", "Expected if keyword, instead got", ifKey);
    
    // condition
    ExprNode *cond = test();
    
    // Colon
    Token colonKey = tokenizer.getToken();
    if (!colonKey.isColon())
        die("Parser::IfStatement", "Expected colon key, instead got", colonKey);
    
    // Statements
    Statements *ifstmts = suite(symTab);
    
    // elif
    
    Statements *elifStmts = nullptr;
    std::vector<ExprNode*> elifCond;
    std::vector<Statements*> elifList;
    Token elifKey = tokenizer.getToken();
    while(elifKey.isElifKey()) {
        // get elif cond
        ExprNode *elifcond = test();
        elifCond.push_back(elifcond);
        
        // check for colon
        Token elifColon = tokenizer.getToken();
        if (!elifColon.isColon())
            die("Parser::IfStatement elif", "Expected colon key, instead got", elifColon);
        // get elif statements
        elifStmts = suite(symTab);
        elifList.push_back(elifStmts);
        
        // get next token to see if there are more elifs
        elifKey = tokenizer.getToken();
        
    }
    // not an elif, putting token back
    tokenizer.ungetToken();
    
    Statements *elseStmts = nullptr;
    // else
    Token elseKey = tokenizer.getToken();
    if (!elseKey.isElseKey()) {
        std::cout << "Parser::ifStatement Else: No else key found, putting token back" << std::endl;
        tokenizer.ungetToken();
    }
    else {
        // colon check
        Token elseColon = tokenizer.getToken();
        if (!elseColon.isColon())
            die("Parser::IfStatement Else", "Expected colon key, instead got", elseColon);
        elseStmts = suite(symTab);
        
    }
    return new IfStatement(cond, ifstmts, elseStmts, elifList, elifCond);
}

FuncStatement *Parser::funcStatement(SymTab &symTab) {
    // Def
    Token defTok = tokenizer.getToken();
    if(defTok.getName() != "def")
        die("Parser::funcStatement", "Expected def for function, instead got", defTok);
    
    // Function name
    Token funcName = tokenizer.getToken();
    if(!funcName.isName())
        die("Parser::funcStatement", "Expected function name, instead got", funcName);

    // Open parenthesis
    Token openParen = tokenizer.getToken();
    if(!openParen.isOpenParen())
        die("Parser::funcStatement", "Expected open parenthesis, instead got", openParen);
    
    Token checkParen = tokenizer.getToken();
    std::vector<std::string> paramList;
    if(!checkParen.isCloseParen()) {
        tokenizer.ungetToken();
        paramList = parameter_list();
    }
    tokenizer.ungetToken();
    // Parameter list
    //std::vector<std::string> paramList = parameter_list();
    
    // Close parenthesis
    Token closeParen = tokenizer.getToken();
    if(!closeParen.isCloseParen())
        die("Parser::funcStatement", "Expected closing parenthesis, instead got", closeParen);
    
    // Colon
    Token funcColon = tokenizer.getToken();
    if(!funcColon.isColon())
        die("Parser::funcStatement", "Expected colon, instead got", funcColon);
    
    // Function statements
    Statements *funcStatements = func_suite(symTab);
    
    // Got all we need for a funciont, now return new function statement
    return new FuncStatement(funcName.getName(), paramList, funcStatements);
}

Statements *Parser::func_suite(SymTab &symTab) {
    // New Line/End of line tok
    Token newLine = tokenizer.getToken();
    if(!newLine.eol())
        die("Parser::func_suite", "Expected new line char, instead got", newLine);
    
    // Indent check
    Token indentTok = tokenizer.getToken();
    if(!indentTok.isIndent())
        die("Parser::func_suite", "Expected indent, instead got", indentTok);
    
    // Parse statements or return statement
    Statements *func_suiteStmts = statements(symTab);
    
    // Dedent
    Token dedentTok = tokenizer.getToken();
    if(!dedentTok.isDedent())
        die("Parser::func_suite", "Expected dedent, instead got", dedentTok);
    return func_suiteStmts;
}

Statements *Parser::suite(SymTab &symTab) {
    // For end of line
    Token endOfLine = tokenizer.getToken();
    if(!endOfLine.eol())
        die("Parser::suite", "Expected end of line, instead got", endOfLine);
    
    // Statements of the for loop
    // Call suite
    Token id = tokenizer.getToken();
    if(!id.isIndent())
        die("Parser::suite", "Expected indent, instead got", id);
    Statements *suitestmts = statements(symTab);
    
    // Dedent
    Token dd = tokenizer.getToken();
    if(!dd.isDedent()) {
        die("Parser::suite", "Expected dedent, instead got", dd);
    }
    return suitestmts;
    
}

std::vector<ExprNode*> Parser::arrayInit() {
    // Check for open bracket
    Token openBracket = tokenizer.getToken();
    if(!openBracket.isOpenBrack())
        die("Parser::arrayInit", "Expected open brack, instead got", openBracket);
    // get array elements
    std::vector<ExprNode*> arrayContents = testList();
    
    // Check for close bracket
    Token closeBracket = tokenizer.getToken();
    if(!closeBracket.isCloseBrack())
        die("Parser::arrayInit", "Expected close brack, instead got", closeBracket);
    return arrayContents;
}
// are we parsing ID or Expr?
std::vector<std::string> Parser::parameter_list() {
    Token ID = tokenizer.getToken();
    std::string id = ID.getName();
    Token tok = tokenizer.getToken();
    std::vector<std::string> pList;
    while(tok.isComma()){
        pList.push_back(id);
        ID = tokenizer.getToken();
        id = ID.getName();
        tok = tokenizer.getToken();
    }
    pList.push_back(id);
    tokenizer.ungetToken();
    return pList;
}

std::vector<ExprNode*> Parser::testList() {
    ExprNode *left = test();
    Token tok = tokenizer.getToken();
    std::vector<ExprNode*> testlist;
    while(tok.isComma()){
        testlist.push_back(left);
        left = test();
        tok = tokenizer.getToken();
    }
    testlist.push_back(left);
    tokenizer.ungetToken();
    return testlist;
}

ExprNode *Parser::test() {
    // This function passes test onto or_test
    ExprNode *left = or_test();
    return left;
}

ExprNode *Parser::or_test() {
    // This function parses the grammar rules:
    // or_test: and_test {'or' and_test}*
    
    ExprNode *left = and_test();
    Token tok = tokenizer.getToken();
    //tok.getName() == "or"
    while (tok.isOr()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::and_test() {
    // This function parses the grammar rules:
    // and_test: not_test {'and' not_test}*
    
    ExprNode *left = not_test();
    Token tok = tokenizer.getToken();
    //tok.getName() == "and"
    while (tok.isAnd()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = not_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}
/*
ExprNode *Parser::not_test() {
    ExprNode *left = rel_expr()
    Token tok = tokenizer.getToken();
    
    if (tok.isNot()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = rel_expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}
*/
// imnplement not, just like factor for negatives
ExprNode *Parser::not_test() {
    // This function parses the grammar rules:
    // not_test: 'not' not_test | comparison
    // comparison = rel_expr() ?
    ExprNode *left = rel_expr();
    Token tok = tokenizer.getToken();
    
    // might not need isRelOp, needed for later grammer rule
    //tok.getName() == "not"
    while (tok.isNot()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = rel_expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::rel_expr(){
    //This function parses the grammar rules: 
    
    // <rel_expr> -> <rel_term> {<rel_op> <rel_term>}
    // <rel_op> -> == | !=
    
    // However, it makes the <rel_op> left associative
    
    ExprNode *left = rel_primary();
    Token tok = tokenizer.getToken();
    while (tok.isEqualOperator() || tok.isGreaterThanOperator() ||
           tok.isLessThanOperator() || tok.isGreaterThanOrEqualOperator() ||
           tok.isLessThanOrEqualOperator() || tok.isNotEqualOperator()){
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = rel_primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::rel_primary(){
    // This function parses the grammar rules:
    
    // <rel_primary> -> <arith_expr>
    
    ExprNode *left = expr();
    return left;
}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    //ExprNode *left = primary();
    ExprNode *left = factor();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = factor();
        //p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::factor() {
    // This function parses the grammar rules:
    
    // factor -> {'-'} factor | atom
    
    //ExprNode *left = primary();
    Token tok = tokenizer.getToken();
    if (tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        // Sets up token with the value of zero
        Token zero;
        zero.setWholeNumber(0);
        // Sets left side of infix expr to the 
        // zero token
        p->left() = new WholeNumber(zero);
        p->right() = factor();
        ExprNode *left = p;
        return left;
    }
    
    else if (tok.isName()) {
        auto tempName = tok;
        tok = tokenizer.getToken();
        if (tok.isOpenParen()) {
            // call statement
            return callExpr(tempName);
        }
        else {
            tokenizer.ungetToken();
            return new Variable(tempName);
        }
        //tokenizer.ungetToken();
        
        /*
        Token tempName = tok;
        if (tok.getName() == "len") {
            // go to len statement
            std::cout << "Should not be heree" << std::endl;
        }
        else {
            return callExpr(tempName);
            
            tok = tokenizer.getToken();
            if (tok.isOpenParen()) {
                // call statement
                return callExpr(tempName);
            }
            
            else if (tok.isOpenBracket()) {
                // subscription statement
                std::cout << "Should not be here either" << std::endl;
            }
            
        }
        std::cout << "Printing brokeen tok: " << std::endl;
        tok.print();
        std::cout << std::endl;
        tokenizer.ungetToken();
        tokenizer.ungetToken();
        tok = tokenizer.getToken();
        std::cout << "Printing brokeen tik: " << std::endl;
        tok.print();
        std::cout << std::endl;
        */
    }
    
    else {
        tokenizer.ungetToken();
        ExprNode *left = primary();
        return left;
    }
    
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();
    //tok.print();
    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if( tok.isStringValue() )
        return new String(tok); // added
    else if (tok.isOpenParen()) {
        ExprNode *p = test();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
