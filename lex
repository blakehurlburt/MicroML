lex lexer.l && cc lex.yy.c -o lex.out && ([ [ -n $1 ] ] && ./lex.out || cat $1 | ./lex.out);

rm lex.out;
rm lex.yy.c;
