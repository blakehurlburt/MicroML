%{
    #include <stdio.h>
%}

%option noyywrap

/* helpers */
DIGIT  [0-9]
LETTER [a-zA-Z]

/* basics */
ID     ({LETTER}|"_")({LETTER}|{DIGIT}|"_")*
REAL   ({DIGIT}*"."{DIGIT}+)|({DIGIT}+"."{DIGIT}*)
INT    {DIGIT}+

/* scope delimiters */
LET    "let"
IN     "in"
END    "end"

/* conditionals */
IF     "if"
THEN   "then"
ELSE   "else"

/* logical operators */
EQ     "=="
NE     "!="
GT     ">"
GE     ">="
LT     "<"
LE     "<="

NOT    "!"

/* arithmetic operators */
ADDI   "+"
ADDR   "+."
SUBI   "-"
SUBR   "-."
MULI   "*"
MULR   "*."
DIVI   "/"
DIVR   "/."
MODI   "%"

NEGI  "~"
NEGR  "~."

/* values and functions */
BIND  "="
VAL   "val"
FUN   "fun"
FN    "fn"


%%
  printf("\nTokenizer running, ^D to exit\n");

{LET}  { printf("LET\n"); }
{IN}   { printf("IN\n"); }
{END}  { printf("END\n"); }

{IF}   { printf("IF\n"); }
{THEN} { printf("THEN\n"); }
{ELSE} { printf("ELSE\n"); }

{EQ}   { printf("EQ\n"); }
{NE}   { printf("NE\n"); }
{LT}   { printf("LT\n"); }
{LE}   { printf("LE\n"); }
{GT}   { printf("GT\n"); }
{GE}   { printf("GE\n"); }

{ADDI} { printf("ADDI\n"); }
{ADDR} { printf("ADDR\n"); }
{SUBI} { printf("SUBI\n"); }
{SUBR} { printf("SUBR\n"); }
{MULI} { printf("MULI\n"); }
{MULR} { printf("MULR\n"); }
{DIVI} { printf("DIVI\n"); }
{DIVR} { printf("DIVR\n"); }

{BIND} { printf("BIND\n"); }
{VAL}  { printf("VAL\n"); }
{FUN}  { printf("FUN\n"); }
{FN}   { printf("FN\n"); }

{ID}   { printf("ID{%s}\n", yytext); }
{REAL} { printf("REAL{%s}\n", yytext); }
{INT}  { printf("INT{%s}\n", yytext); }

%%

int main() {
  yylex();
  return 0;
}
