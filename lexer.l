%{
    #include <stdio.h>
    #include <string.h>

    #include "y.tab.h"

    extern YYSTYPE yylval;

    char atoc(const char* str) {
      switch(str[1]) {
        case '\"': return '\"';
        case '\'': return '\'';
        case '\\': return '\\';
        case '0':  return '\0';
        case 'a':  return '\a';
        case 'b':  return '\b';
        case 'f':  return '\f';
        case 'n':  return '\n';
        case 'r':  return '\r';
        case 't':  return '\t';
        case 'v':  return '\v';
        default:   return 0;
      }
    }
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
ADD   "+"
SUB   "-"
MUL   "*"
DIV   "/"
MOD   "%"
NEG   "~"

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



{CBEG} { ECHO; return CBEG; }
{CEND} { ECHO; return CEND; }

{LET}  { ECHO; return LET; }
{IN}   { ECHO; return IN; }
{END}  { ECHO; return END; }

{IF}   { ECHO; return IF; }
{THEN} { ECHO; return THEN; }
{ELSE} { ECHO; return ELSE; }

{EQ}   { ECHO; return EQ; }
{NE}   { ECHO; return NE; }
{LT}   { ECHO; return LT; }
{LE}   { ECHO; return LE; }
{GT}   { ECHO; return GT; }
{GE}   { ECHO; return GE; }

{BOOL} { yylval = new BoolNode(yytext == "true"); ECHO; return BOOL; }
{AND}  { ECHO; return AND; }
{OR}   { ECHO; return OR; }
{NOT}  { ECHO; return NOT; }

{ADD} { ECHO; return ADD; }
{SUB} { ECHO; return SUB; }
{MUL} { ECHO; return MUL; }
{DIV} { ECHO; return DIV; }

{BIND} { ECHO; return BIND; }
{VAL}  { ECHO; return VAL; }
{FUN}  { ECHO; return FUN; }
{FN}   { ECHO; return FN; }
{UNIT} { ECHO; return UNIT; }
{CONS} { ECHO; return CONS; }

{RBEG} { ECHO; return RBEG; }
{REND} { ECHO; return REND; }
{LBEG} { ECHO; return LBEG; }
{LEND} { ECHO; return LEND; }
{SEP}  { ECHO; return SEP; }
{GET}  { ECHO; return GET; }


{CHR}  { if (*yytext == '/') yylval = new CharNode(atoc(yytext));
         else yylval = new CharNode(*yytext); ECHO; return CHR; }

{ID}   { yylval = new IdentifierNode(yytext); ECHO; return ID; }
{REAL} { yylval = new RealNode(atof(yytext)); ECHO; return REAL; }
{INT}  { yylval = new IntNode(atoi(yytext)); ECHO; return INT; }

{LP}   { ECHO; return LP; }
{RP}   { ECHO; return RP; }
{WS}   { }


%%

extern int main();
