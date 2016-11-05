%{
    #include <stdio.h>
%}

%option noyywrap

/* helpers */
DIGIT  [0-9]
LETTER [a-zA-Z]
UNIT   "{}"|"()"
LP     "("
RP     ")"
WS     (" "|"\n"|"\t")
CHR    \'(\\.|[^\'])\'
STR    \"(\\.|[^\"])*\"

/* basics */
ID     ({LETTER}|"_")({LETTER}|{DIGIT}|"_")*
REAL   ({DIGIT}*"."{DIGIT}+)|({DIGIT}+"."{DIGIT}*)
INT    {DIGIT}+
CBEG   "(*"
CEND   "*)"
CONS   "::"

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

/* booleans and boolean operators */
BOOL   "true"|"false"
AND    "&&"
OR     "||"
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

NEGI   "~"
NEGR   "~."

/* values and functions */
BIND   "="
VAL    "val"
FUN    "fun"
FN     "fn"

/* records and lists */
RBEG   "{"
REND   "}"
LBEG   "["
LEND   "]"
SEP    ","
GET    "."

ERR    . /* things between two errors may also be erroneous */

%%
  printf("\nTokenizer running, ^D to exit\n");


{CBEG} { printf("CBEG\n"); }
{CEND} { printf("CEND\n"); }

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

{BOOL} { printf("BOOL{%s}\n", yytext); }
{AND}  { printf("AND\n"); }
{OR}   { printf("OR\n"); }
{NOT}  { printf("NOT\n"); }

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
{UNIT} { printf("UNIT\n"); }
{CONS} { printf("CONS\n"); }

{RBEG} { printf("RBEG\n"); }
{REND} { printf("REND\n"); }
{LBEG} { printf("LBEG\n"); }
{LEND} { printf("LEND\n"); }
{SEP}  { printf("SEP\n"); }
{GET}  { printf("GET\n"); }

{STR}  { printf("STR{%s}\n", yytext); }
{CHR}  { printf("CHR{%s}\n", yytext); }

{ID}   { printf("ID{%s}\n", yytext); }
{REAL} { printf("REAL{%s}\n", yytext); }
{INT}  { printf("INT{%s}\n", yytext); }

{LP}   { printf("LP\n"); }
{RP}   { printf("RP\n"); }
{WS}   { }


%%s

int main() {
  yylex();
  return 0;
}
