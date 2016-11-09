%{

  extern "C" {
    int yyparse();
    int yylex();
    int yywrap() { return 1; }
  }


#include <iostream>
#include <string>

#include "AST.h"
#include "CPPAST.h"
#include "Translate.h"

#define YYSTYPE ASTNode*

#include "lex.yy.c"

void yyerror(std::string s = "") {
  std::cerr << "syntax error: " << s << std::endl;
}

extern int yylex();

%}

%token UNIT LP RP NL CHR STR ID REAL INT CBEG CEND CONS
       IF THEN ELSE EQ NE GT GE LT LE BOOL AND OR NOT ADD SUB MUL DIV MOD NEG
       BIND VAL FUN FN RBEG REND LBEG LEND SEP GET LET IN END

%left SEP
%right BIND IF THEN ELSE
%left OR
%left AND
%left EQ NE
%left GE GT LE LT
%left ADD SUB
%left MUL DIV MOD
%right NEG NOT
%right GET
%left LP RP

%%

exp: op        { $$ = $1; }

op: fun        { $$ = $1; }
  | op MUL fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op DIV fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op MOD fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op ADD fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op SUB fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op GE  fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op GT  fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op LE  fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op LT  fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op EQ  fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op NE  fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op AND fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | op OR  fun { $$ = new BinOpNode((IdentifierNode*) $2, (ExpressionNode*) $1, (ExpressionNode*) $3); }

fun: base      { $$ = $1; }
  | ID fun     { $$ = new InvokeNode((ExpressionNode*) $1, (ExpressionNode*) $2); }

base: INT      { $$ = $1; }
  | REAL       { $$ = $1; }
  | BOOL       { $$ = $1; }
  | LP exp RP  { $$ = $2; }

%%

int main() {
  BlockNode* b = new BlockNode();
  if (yyparse() == 0)
    std::cout << b->toString() << std::endl;
  return 0;
}
