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

#include "CPPAST.h"
#include "CodeGen.h"
#include "Translate.h"

#define YYSTYPE ASTNode*

#include "lex.yy.c"

int yyerror(std::string s)
{
  std::cerr << "syntax error: " << yylval << std::endl;
  return 0;
}

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
       BIND VAL FUN FN RBEG REND LBEG LEND SEP GET LET IN END

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

block: state block { }
     | state       { }

state: VAL ID BIND exp nl {blocks.top()->add(new ValNode((IdentifierNode*) $2, (ExpressionNode*) $4)); $$ = $1;}
    | FUN ID ID BIND exp nl {blocks.top()->add(new FunDeclNode((IdentifierNode*) $2, (IdentifierNode*) $3, (ExpressionNode*) $5)); $$ = $1;}

bindings: bindings SEP ID BIND exp { $$ = $1;  ((RecordNode*)$$)->add(new ValNode((IdentifierNode*) $3, (ExpressionNode*) $5)); }
        | ID BIND exp { $$ = new RecordNode(); ((RecordNode*)$$)->add(new ValNode((IdentifierNode*) $1, (ExpressionNode*) $3)); }
        |  /*Empty*/ {$$ = new RecordNode(); }
        ;

elems: exp SEP elems {RecordNode* r = new RecordNode();
             r->add(new ValNode(new IdentifierNode("head"), (ExpressionNode*) $1));
             r->add(new ValNode(new IdentifierNode("tail"), (RecordNode*) $3)); $$ = r;}
     | exp       {RecordNode* r = new RecordNode();
                  r->add(new ValNode(new IdentifierNode("head"), (ExpressionNode*) $1));
                  r->add(new ValNode(new IdentifierNode("tail"), new RecordNode())); $$ = r;}
     | /*Empty*/ {$$ = new RecordNode(); }

exp: LP exp RP {$$ = $2;}
  | exp GET ID {$$ = new GetNode((ExpressionNode*) $1, (IdentifierNode*) $3); }
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
  | STR {$$ = $1; }
  | RBEG bindings REND {$$ = $2;}
  | LBEG elems LEND {$$ = $2;}
  ;

nl: NL nl { }
  | NL { }

%%

int main() {
  blocks.push(new BlockNode());
   if (yyparse() == 0) //parsing worked
   //  for (StatementNode* s : blocks.top()->statements)
      // std::cout << blocks.top()->toString() << std::endl;
      std::cout << generateCode(pgrmTranslate(blocks.top())) << std::endl;

 //while(yylex());
  return 0;
}
