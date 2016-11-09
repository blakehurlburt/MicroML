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
  std::cerr << s << ' ' << yylval->toString() << std::endl;
}

extern int yylex();

StatementNode* s;

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

state: fun { $$ = $1; }

lelems: exp SEP lelems { $$ = new RecordNode();
                        ((RecordNode*) $$)->add(new ValNode(new IdentifierNode("head"), (ExpressionNode*) $1));
                        ((RecordNode*) $$)->add(new ValNode(new IdentifierNode("tail"), (RecordNode*) $3));
                       }
      | exp            { $$ = new RecordNode();
                        ((RecordNode*) $$)->add(new ValNode(new IdentifierNode("head"), (ExpressionNode*) $1));
                        ((RecordNode*) $$)->add(new ValNode(new IdentifierNode("tail"), new RecordNode()));
                       }
      |  /*Empty*/     { $$ = new RecordNode(); }

exp: base       { $$ = $1; }
  | LP exp RP   { $$ = $2; }
  | NEG exp     { $$ = new InvokeNode(new IdentifierNode("_neg_"), (ExpressionNode*) $2); }
  | NOT exp     { $$ = new InvokeNode(new IdentifierNode("_not_"), (ExpressionNode*) $2); }
  | exp MUL exp { $$ = new BinOpNode(new IdentifierNode("_mul_"), (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp DIV exp { $$ = new BinOpNode(new IdentifierNode("_div_"), (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp MOD exp { $$ = new BinOpNode(new IdentifierNode("_mod_"), (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp ADD exp { $$ = new BinOpNode(new IdentifierNode("_add_"), (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp SUB exp { $$ = new BinOpNode(new IdentifierNode("_sub_"), (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp GE  exp { $$ = new BinOpNode(new IdentifierNode("_ge_"),  (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp GT  exp { $$ = new BinOpNode(new IdentifierNode("_gt_"),  (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp LE  exp { $$ = new BinOpNode(new IdentifierNode("_le_"),  (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp LT  exp { $$ = new BinOpNode(new IdentifierNode("_lt_"),  (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp EQ  exp { $$ = new BinOpNode(new IdentifierNode("_eq_"),  (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp NE  exp { $$ = new BinOpNode(new IdentifierNode("_ne_"),  (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp AND exp { $$ = new BinOpNode(new IdentifierNode("_and_"), (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | exp OR  exp { $$ = new BinOpNode(new IdentifierNode("_or_"),  (ExpressionNode*) $1, (ExpressionNode*) $3); }
  | LBEG lelems LEND { $$ = $2; }
  | RBEG relems REND { $$ = $2; }

params: params ID { ((ParamListNode*) $1)->add((IdentifierNode*) $2); }
      | ID        { $$ = new ParamListNode((IdentifierNode*) $1); }

fun: FUN ID params BIND exp { s = new FunDeclNode((IdentifierNode*) $2, (ParamListNode*) $3, (ExpressionNode*) $5); }

relems: relems SEP ID BIND exp    { $$ = $1; ((RecordNode*)$$)->add(new ValNode((IdentifierNode*) $3, (ExpressionNode*) $5)); }
      | ID BIND exp { $$ = new RecordNode(); ((RecordNode*)$$)->add(new ValNode((IdentifierNode*) $1, (ExpressionNode*) $3)); }
      |  /*Empty*/  { $$ = new RecordNode(); }

base: INT   { $$ = $1; }
    | REAL  { $$ = $1; }
    | BOOL  { $$ = $1; }
    | ID    { $$ = $1; }
    | STR   { $$ = $1; }

%%

int main() {
  if (yyparse() == 0)
    std::cout << s->toString() << std::endl;
  return 0;
}
