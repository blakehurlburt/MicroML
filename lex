lex lexer.l && cc lex.yy.c -o lex.out && ./lex.out $1;

rm lex.out;
rm lex.yy.c;
