%{

extern "C"
{
  int yyparse(void);
  int yylex(void);
  int yywrap() { return 1; }
}

#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <stdexcept>

#include "AST.h"

#define YYSTYPE ASTNode*

#include "lex.yy.c"


std::stack<BlockNode*> blocks;

extern int yylex();

/*
      UNIT CHR STR ID REAL INT BOOL CBEG CEND CONS LET IN END
      IF THEN ELSE
      FN RBEG REND LBEG LEND SEP GET ERR
*/

%}

%token UNIT LP RP NL CHR STR ID REAL INT CBEG CEND CONS
       IF THEN ELSE EQ NE GT GE LT LE BOOL AND OR NOT ADD SUB MUL DIV MOD NEG
       BIND VAL FUN FN RBEG REND LBEG LEND SEP GET LET IN END ERR

%left SEP
%right BIND, IF, THEN, ELSE
%left OR
%left AND
%left EQ, NE
%left GE, GT, LE, LT
%left ADD, SUB
%left MUL, DIV, MOD
%right NEG, NOT
%right GET
%left LP, RP

%%

block:
     | block state

state: LP state RP {$$ = $2;}
    | VAL ID BIND exp {blocks.top()->add(new ValNode((IdentifierNode*) $2, (ExpressionNode*) $4)); $$ = new CharNode('@');}
    | FUN ID ID BIND exp {blocks.top()->add(new FunDeclNode((IdentifierNode*) $2, (IdentifierNode*) $3, (ExpressionNode*) $5)); $$ = new CharNode('@');}
    | state state {$$ = (((CharNode*)$1)->value == '@' && ((CharNode*)$2)->value == '@')? new CharNode('@') : new CharNode('!');}

exp: LP exp RP {$$ = $2;}
  | NEG exp {$$ = new InvokeNode(new IdentifierNode("_neg_"), (ExpressionNode*) $2);}
  | NOT exp {$$ = new InvokeNode(new IdentifierNode("_not_"), (ExpressionNode*) $2);}
  | exp ADD exp {$$ = new BinOpNode(new IdentifierNode("_add_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp SUB exp {$$ = new BinOpNode(new IdentifierNode("_sub_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp MUL exp {$$ = new BinOpNode(new IdentifierNode("_mul_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp DIV exp {$$ = new BinOpNode(new IdentifierNode("_div_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp MOD exp {$$ = new BinOpNode(new IdentifierNode("_mod_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp EQ exp {$$ = new BinOpNode(new IdentifierNode("_eq_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp NE exp {$$ = new BinOpNode(new IdentifierNode("_ne_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp LT exp {$$ = new BinOpNode(new IdentifierNode("_lt_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp LE exp {$$ = new BinOpNode(new IdentifierNode("_le_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp GT exp {$$ = new BinOpNode(new IdentifierNode("_gt_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | exp GE exp {$$ = new BinOpNode(new IdentifierNode("_ge_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
  | IF exp THEN exp ELSE exp {$$ = new IfNode((ExpressionNode*) $2, (ExpressionNode*) $4, (ExpressionNode*) $6);}
  | ID LP exp RP {$$ = new InvokeNode((IdentifierNode*) $1, (ExpressionNode*) $3);}
  | FN ID BIND exp {$$ = new LambdaNode((IdentifierNode*) $2, (ExpressionNode*) $4);}
  | INT {$$ = $1;}
  | REAL {$$ = $1;}
  | BOOL {$$ = $1;}
  | ID {$$ = $1;}
  | LET block IN exp END {$$ }
  ;

other: LET {blocks.push(new BlockNode()); } block IN exp END {$$ = new LetNode(blocks.top(), (ExpressionNode*) $2); blocks.pop();}


%%

int yyerror(std::string s)
{
  std::cerr << "syntax error: " << yylval << std::endl;
  return 0;
}


int main() {
  blocks.push(new BlockNode());
  if (yyparse() == 0) //parsing worked
  //  for (StatementNode* s : blocks.top()->statements)
      std::cout << blocks.top()->toString() << std::endl;

 //while(yylex());
  return 0;
}
