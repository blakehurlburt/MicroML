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

#include "AST.h"

#define YYSTYPE ASTNode*

#include "lex.yy.c"


std::stack<std::list<StatementNode*>> blocks;

extern int yylex();

/*
      UNIT CHR STR ID REAL INT BOOL CBEG CEND CONS LET IN END
      IF THEN ELSE
      FN RBEG REND LBEG LEND SEP GET ERR
*/

%}

%token UNIT LP RP NL CHR STR ID REAL INT CBEG CEND CONS LET IN END
       IF THEN ELSE EQ NE GT GE LT LE BOOL AND OR NOT ADD SUB MUL DIV MOD NEG
       BIND VAL FUN FN RBEG REND LBEG LEND SEP GET ERR

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
%left ID EXPN
%left LP, RP

%%



block: state nl block  {blocks.top().push_front((StatementNode*) $1); $$ = nullptr;}
     | state nl      {blocks.top().push_front((StatementNode*) $1); $$ = nullptr;}
     | state  {blocks.top().push_front((StatementNode*) $1); $$ = nullptr;}

spec: LET           {std::cout << "parser sees LET" << std::endl; blocks.push(std::list<StatementNode*>()); $$ = nullptr;}

state: VAL ID BIND exp {$$ = new ValNode((IdentifierNode*) $2, (ExpressionNode*) $4);}
    |  FUN ID ID BIND exp {$$ = new FunDeclNode((IdentifierNode*) $2, (IdentifierNode*) $3, (ExpressionNode*) $5);}

exp:  expn          {$$ = $1;}
    | expb          {$$ = $1;}

expn: INT           {$$ = $1;}
    | REAL          {$$ = $1;}
    | ID            {$$ = $1;}
    | LP expn RP    {$$ = $2;}
    | expn ADD expn {$$ = new BinOpNode(new IdentifierNode("_add_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn SUB expn {$$ = new BinOpNode(new IdentifierNode("_sub_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn MUL expn {$$ = new BinOpNode(new IdentifierNode("_mul_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn DIV expn {$$ = new BinOpNode(new IdentifierNode("_div_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn MOD expn {$$ = new BinOpNode(new IdentifierNode("_mod_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn NEG expn {$$ = new InvokeNode(new IdentifierNode("_neg_"), (ExpressionNode*) $1); }
    | IF expb THEN expn ELSE expn {$$ = new IfNode((ExpressionNode*) $2, (ExpressionNode*) $4, (ExpressionNode*) $6); }
    | ID expn       {$$ = new InvokeNode((IdentifierNode*) $1, (ExpressionNode*) $2);}
    | FN ID BIND expn {$$ = new LambdaNode((IdentifierNode*) $2, (ExpressionNode*) $4);}
    | spec block IN expn END {$$ = new LetNode(new BlockNode(blocks.top()), (ExpressionNode*) $4); blocks.pop(); }

expb: BOOL          {$$ = $1;}
    | ID            {$$ = $1;}
    | LP expb RP    {$$ = $2;}
    | expb AND expb {$$ = new BinOpNode(new IdentifierNode("_and_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expb OR  expb {$$ = new BinOpNode(new IdentifierNode("_or_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | NOT expb      {$$ = new InvokeNode(new IdentifierNode("_not_"), (ExpressionNode*) $1); }
    | expn EQ expn  {$$ = new BinOpNode(new IdentifierNode("_eq_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expb EQ expb  {$$ = new BinOpNode(new IdentifierNode("_eq_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn NE expn  {$$ = new BinOpNode(new IdentifierNode("_ne_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expb NE expb  {$$ = new BinOpNode(new IdentifierNode("_ne_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn LT expn  {$$ = new BinOpNode(new IdentifierNode("_lt_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn GT expn  {$$ = new BinOpNode(new IdentifierNode("_gt_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn LE expn  {$$ = new BinOpNode(new IdentifierNode("_le_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn GE expn  {$$ = new BinOpNode(new IdentifierNode("_ge_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | IF expb THEN expb ELSE expb {$$ = new IfNode((ExpressionNode*) $2, (ExpressionNode*) $4, (ExpressionNode*) $6); }
    | ID expb       {$$ = new InvokeNode((IdentifierNode*) $1, (ExpressionNode*) $2);}
    | FN ID BIND expb {$$ = new LambdaNode((IdentifierNode*) $2, (ExpressionNode*) $4);}
    | spec block IN expb END {std::cout << "spec block in expb end" << std::endl; $$ = new LetNode(new BlockNode(blocks.top()), (ExpressionNode*) $4); blocks.pop(); }

nl: NL nl {}
  | NL {}

%%

int yyerror(std::string s)
{
  std::cerr << s << std::endl;
}

int main() {
  blocks.push(std::list<StatementNode*>());
  if (yyparse() == 0) //parsing worked
    std::cout << BlockNode(blocks.top()).toString() << std::endl;
  return 0;
}
