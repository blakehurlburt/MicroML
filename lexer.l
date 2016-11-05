%{
    #include <stdio.h>
%}

%option noyywrap

DIGIT  [0-9]
LETTER [a-zA-Z]

ID ({LETTER}|"_")({LETTER}|{DIGIT}|"_")*
REAL ({DIGIT}*"."{DIGIT}+)|({DIGIT}+"."{DIGIT}*)
INT {DIGIT}+



%%
  printf("\nTokenizer running - ^D to exit\n");

{ID}   { printf("%s is an identifier\n", yytext); }
{REAL} { printf("%s is a real\n", yytext); }
{INT}  { printf("%s is an int\n", yytext); }

%%

int main() {
  yylex();
  return 0;
}
