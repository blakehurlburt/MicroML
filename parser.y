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
      UNIT LP RP CHR STR ID REAL INT BOOL CBEG CEND CONS LET IN END
      IF THEN ELSE
      BIND VAL FUN FN RBEG REND LBEG LEND SEP GET ERR
*/

%}

%token UNIT LP RP CHR STR ID REAL INT CBEG CEND CONS LET IN END
       IF THEN ELSE EQ NE GT GE LT LE BOOL AND OR NOT ADD SUB MUL DIV MOD NEG
       BIND VAL FUN FN RBEG REND LBEG LEND SEP GET ERR

%right BIND
%left ADD, SUB
%left MUL, DIV, MOD

%%


state: VAL ID BIND exp {blocks.top().push_back((StatementNode*) ($$ = new ValNode((IdentifierNode*) $2, (ExpressionNode*) $4)));}
    |  FUN ID ID BIND exp {blocks.top().push_back((StatementNode*) ($$ = new FunDeclNode((IdentifierNode*) $2, (IdentifierNode*) $3, (ExpressionNode*) $5)));}

exp:  expn          {$$ = $1;}
    | expb          {$$ = $1;}

expn: INT           {$$ = $1;}
    | REAL          {$$ = $1;}
    | ID            {$$ = $1;}
    | expn ADD expn {$$ = new BinOpNode(new IdentifierNode("_add_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn SUB expn {$$ = new BinOpNode(new IdentifierNode("_sub_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn MUL expn {$$ = new BinOpNode(new IdentifierNode("_mul_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn DIV expn {$$ = new BinOpNode(new IdentifierNode("_div_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn MOD expn {$$ = new BinOpNode(new IdentifierNode("_mod_"), (ExpressionNode*) $1, (ExpressionNode*) $3);}
    | expn NEG expn {$$ = new InvokeNode(new IdentifierNode("_neg_"), (ExpressionNode*) $1); }
    | IF expb THEN expn ELSE expn {$$ = new IfNode((ExpressionNode*) $2, (ExpressionNode*) $4, (ExpressionNode*) $6); }


expb: BOOL          {$$ = $1;}
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



%%

int yyerror(std::string s)
{
  std::cerr << s << std::endl;
}

BlockNode* absyn_root;

int main() {
  blocks.push(std::list<StatementNode*>());
  if (yyparse() == 0) //parsing worked
    std::cout << BlockNode(blocks.top()).toString() << std::endl;
  return 0;
}
