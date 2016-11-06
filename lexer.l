%{
    #include <iostream>
    #include <string>

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

LP     "("
RP     ")"
UNIT   "{}"|"()"
NL     "\n"
WS     (" "|"\t")
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

%%

{LP}   { return LP; }
{RP}   { return RP; }

{CBEG} { return CBEG; }
{CEND} { return CEND; }

{LET}  { printf("lexer sees LET\n"); return LET; }
{IN}   { return IN; }
{END}  { return END; }

{IF}   { return IF; }
{THEN} { return THEN; }
{ELSE} { return ELSE; }

{EQ}   { return EQ; }
{NE}   { return NE; }
{LT}   { return LT; }
{LE}   { return LE; }
{GT}   { return GT; }
{GE}   { return GE; }

{BOOL} { yylval = new BoolNode(yytext == "true"); return BOOL; }
{AND}  { return AND; }
{OR}   { return OR; }
{NOT}  { return NOT; }

{ADD} { return ADD; }
{SUB} { return SUB; }
{MUL} { return MUL; }
{DIV} { return DIV; }

{BIND} { return BIND; }
{VAL}  { return VAL; }
{FUN}  { return FUN; }
{FN}   { return FN; }
{UNIT} { return UNIT; }
{CONS} { return CONS; }

{RBEG} { return RBEG; }
{REND} { return REND; }
{LBEG} { return LBEG; }
{LEND} { return LEND; }
{SEP}  { return SEP; }
{GET}  { return GET; }

{CHR}  { if (*yytext == '/') yylval = new CharNode(atoc(yytext));
         else yylval = new CharNode(*yytext); return CHR; }

{STR}  { std::string *str = new std::string(yytext);
  yylval = new StringNode(str->substr(1, str->length()-2)); return STR; }

{ID}   { yylval = new IdentifierNode(yytext); return ID; }
{REAL} { yylval = new RealNode(atof(yytext)); return REAL; }
{INT}  { yylval = new IntNode(atoi(yytext)); return INT; }

{NL}   { return NL; }
{WS}   { }

%%

extern int main();
