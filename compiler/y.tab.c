/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140715

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "parser.y"

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

#line 62 "y.tab.c"

#if ! defined(YYSTYPE) && ! defined(YYSTYPE_IS_DECLARED)
/* Default: YYSTYPE is the semantic value type. */
typedef int YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define UNIT 257
#define LP 258
#define RP 259
#define NL 260
#define CHR 261
#define STR 262
#define ID 263
#define REAL 264
#define INT 265
#define CBEG 266
#define CEND 267
#define CONS 268
#define IF 269
#define THEN 270
#define ELSE 271
#define EQ 272
#define NE 273
#define GT 274
#define GE 275
#define LT 276
#define LE 277
#define BOOL 278
#define AND 279
#define OR 280
#define NOT 281
#define ADD 282
#define SUB 283
#define MUL 284
#define DIV 285
#define MOD 286
#define NEG 287
#define BIND 288
#define VAL 289
#define FUN 290
#define FN 291
#define RBEG 292
#define REND 293
#define LBEG 294
#define LEND 295
#define SEP 296
#define GET 297
#define LET 298
#define IN 299
#define END 300
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    1,    1,    4,    4,    4,    5,    5,    5,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    3,    3,
};
static const YYINT yylen[] = {                            2,
    2,    1,    5,    6,    5,    3,    0,    3,    1,    0,
    3,    3,    2,    2,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    6,    4,    4,
    1,    1,    1,    1,    1,    3,    3,    2,    1,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    0,    0,    0,    0,    1,    0,    0,    0,
   35,    0,   32,   31,    0,   33,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    3,    0,
   11,    0,    0,    0,    0,   36,    0,    0,   37,   38,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   12,    4,   29,    0,    0,    0,    0,
    8,    0,    0,    0,    0,
};
static const YYINT yydgoto[] = {                          3,
    4,   32,   49,   31,   33,
};
static const YYINT yysindex[] = {                      -249,
 -259, -257,    0, -249, -274, -247,    0,  281, -268,  281,
    0, -214,    0,    0,  281,    0,  281,  281, -215, -213,
  281,   51,  281, -189,  281,  304, -248, -248, -237, -236,
 -288,  336, -234, -196,  281,  281,  281,  281,  281,  281,
  281,  281,  281,  281,  281,  281,  281, -192,    0,   51,
    0, -170,  281,  281,  281,    0, -191,  281,    0,    0,
  410,  410, -161, -161, -161, -161,  394,  378, -273, -273,
 -248, -248, -248,    0,    0,    0,  320,  362,  362, -211,
    0,  281,  281,  362,  362,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,   73,    0,    0,    0,    0,    0,    0,
    0, -217,    0,    0,    0,    0,    0,    0,    0, -275,
 -203,    0,    0,    0,    0,    0, -142, -114,    0,    0,
    0, -197,    0,    1,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -203,    0,    0,
  187,  191,   79,  106,  133,  160,  218,  232,   -1,   26,
  -86,  -58,  -30,    0,    0,    0,    0,  236, -271,    0,
    0,    0,    0,  245, -270,
};
static const YYINT yygindex[] = {                        77,
    0,   -8,  -31,    0,   24,
};
#define YYTABLESIZE 707
static const YYINT yytable[] = {                         22,
   39,   24,   60,    5,   56,    6,   26,   57,   27,   28,
   45,   46,   47,    8,   50,    9,   52,    7,   75,   23,
    7,    6,    5,   48,    6,    5,   61,   62,   63,   64,
   65,   66,   67,   68,   69,   70,   71,   72,   73,    1,
    2,   34,   34,   25,   77,   78,   79,   29,   48,   30,
   54,   55,   34,   34,   34,   34,   34,   34,   34,   34,
   59,   34,   34,   34,   34,   34,   34,   34,   34,   51,
   74,   80,    2,   84,   85,   34,   83,   34,   34,   34,
    7,   81,   35,   36,   37,   38,   39,   40,   76,   41,
   42,   10,   43,   44,   45,   46,   47,    9,    0,    0,
    0,   35,   36,   37,   38,   39,   40,   48,   41,   42,
    0,   43,   44,   45,   46,   47,   14,   14,    0,    0,
   43,   44,   45,   46,   47,    0,   48,   14,   14,   14,
   14,   14,   14,   14,   14,   48,   14,   14,    0,   14,
   14,   14,   14,   14,   13,   13,    0,    0,    0,    0,
   14,    0,   14,   14,    0,   13,   13,   13,   13,   13,
   13,   13,   13,    0,   13,   13,    0,   13,   13,   13,
   13,   13,   19,   19,    0,    0,    0,    0,   13,    0,
   13,   13,    0,   19,   19,   19,   19,   19,   19,   19,
   19,    0,   19,   19,    0,   19,   19,   19,   19,   19,
   20,   20,    0,    0,    0,    0,   19,    0,   19,   19,
    0,   20,   20,   20,   20,   20,   20,   20,   20,    0,
   20,   20,    0,   20,   20,   20,   20,   20,   21,   21,
    0,    0,    0,    0,   20,    0,   20,   20,    0,   21,
   21,   21,   21,   21,   21,   21,   21,    0,   21,   21,
    0,   21,   21,   21,   21,   21,    0,   17,   17,    0,
    0,    0,   21,    0,   21,   21,    0,    0,   17,   17,
   17,   17,   17,   17,   17,   17,    0,   17,   17,    0,
   17,   17,    0,    0,   18,   18,    0,    0,    0,   39,
   39,   17,    0,   17,   17,   18,   18,   18,   18,   18,
   18,   18,   18,    0,   18,   18,    0,   18,   18,    0,
   34,    0,    0,    0,    0,    0,    0,    0,   18,    0,
   18,   18,   35,   36,   37,   38,   39,   40,    0,   41,
   42,    0,   43,   44,   45,   46,   47,   26,   26,    0,
    0,    0,    0,    0,    0,    0,    0,   48,   26,   26,
   26,   26,   26,   26,   26,   26,    0,   26,   26,    0,
    0,    0,    0,    0,   27,   27,    0,    0,    0,    0,
    0,   26,    0,   26,   26,   27,   27,   27,   27,   27,
   27,   27,   27,    0,   27,   27,    0,    0,    0,    0,
    0,   24,   24,    0,    0,    0,    0,    0,   27,    0,
   27,   27,   24,   24,   24,   24,   24,   24,   24,   24,
    0,   24,   24,    0,    0,    0,    0,    0,   25,   25,
    0,    0,    0,    0,    0,   24,    0,   24,   24,   25,
   25,   25,   25,   25,   25,   25,   25,    0,   25,   25,
    0,    0,    0,    0,    0,   22,   22,    0,    0,   23,
   23,    0,   25,    0,   25,   25,   22,   22,   22,   22,
   23,   23,   23,   23,    0,   22,   22,    0,    0,   23,
   23,    0,    0,    0,    0,    0,   15,   15,    0,   22,
    0,   22,   22,   23,    0,   23,   23,   15,   15,    0,
   16,   16,    0,    0,   30,   30,   15,   15,    0,    0,
    0,   16,   16,   28,   28,   30,   30,    0,    0,    0,
   15,   16,   15,   15,   28,   28,    0,    0,    0,    0,
    0,    0,    0,    0,   16,    0,   16,   16,   30,    0,
   30,   30,    0,    0,    0,    0,    0,   28,   10,   28,
   28,    0,   11,   12,   13,   14,    0,    0,    0,   15,
    0,    0,    0,    0,    0,    0,    0,    0,   16,    0,
    0,   17,    0,    0,    0,    0,    0,   18,    0,    0,
    0,   19,   20,   53,   21,   35,   36,   37,   38,   39,
   40,    0,   41,   42,    0,   43,   44,   45,   46,   47,
   82,   35,   36,   37,   38,   39,   40,    0,   41,   42,
   48,   43,   44,   45,   46,   47,    0,   35,   36,   37,
   38,   39,   40,    0,   41,   42,   48,   43,   44,   45,
   46,   47,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   58,   48,   35,   36,   37,   38,   39,   40,    0,
   41,   42,    0,   43,   44,   45,   46,   47,    0,   35,
   36,   37,   38,   39,   40,    0,   41,    0,   48,   43,
   44,   45,   46,   47,    0,   35,   36,   37,   38,   39,
   40,    0,    0,    0,   48,   43,   44,   45,   46,   47,
    0,    0,    0,   37,   38,   39,   40,    0,    0,    0,
   48,   43,   44,   45,   46,   47,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   48,
};
static const YYINT yycheck[] = {                          8,
    0,   10,   34,  263,  293,  263,   15,  296,   17,   18,
  284,  285,  286,  288,   23,  263,   25,  293,   50,  288,
  296,  293,  293,  297,  296,  296,   35,   36,   37,   38,
   39,   40,   41,   42,   43,   44,   45,   46,   47,  289,
  290,  259,  260,  258,   53,   54,   55,  263,  297,  263,
  288,  288,  270,  271,  272,  273,  274,  275,  276,  277,
  295,  279,  280,  260,  282,  283,  284,  285,  286,  259,
  263,  263,    0,   82,   83,  293,  288,  295,  296,  297,
    4,   58,  272,  273,  274,  275,  276,  277,  259,  279,
  280,  295,  282,  283,  284,  285,  286,  295,   -1,   -1,
   -1,  272,  273,  274,  275,  276,  277,  297,  279,  280,
   -1,  282,  283,  284,  285,  286,  259,  260,   -1,   -1,
  282,  283,  284,  285,  286,   -1,  297,  270,  271,  272,
  273,  274,  275,  276,  277,  297,  279,  280,   -1,  282,
  283,  284,  285,  286,  259,  260,   -1,   -1,   -1,   -1,
  293,   -1,  295,  296,   -1,  270,  271,  272,  273,  274,
  275,  276,  277,   -1,  279,  280,   -1,  282,  283,  284,
  285,  286,  259,  260,   -1,   -1,   -1,   -1,  293,   -1,
  295,  296,   -1,  270,  271,  272,  273,  274,  275,  276,
  277,   -1,  279,  280,   -1,  282,  283,  284,  285,  286,
  259,  260,   -1,   -1,   -1,   -1,  293,   -1,  295,  296,
   -1,  270,  271,  272,  273,  274,  275,  276,  277,   -1,
  279,  280,   -1,  282,  283,  284,  285,  286,  259,  260,
   -1,   -1,   -1,   -1,  293,   -1,  295,  296,   -1,  270,
  271,  272,  273,  274,  275,  276,  277,   -1,  279,  280,
   -1,  282,  283,  284,  285,  286,   -1,  259,  260,   -1,
   -1,   -1,  293,   -1,  295,  296,   -1,   -1,  270,  271,
  272,  273,  274,  275,  276,  277,   -1,  279,  280,   -1,
  282,  283,   -1,   -1,  259,  260,   -1,   -1,   -1,  289,
  290,  293,   -1,  295,  296,  270,  271,  272,  273,  274,
  275,  276,  277,   -1,  279,  280,   -1,  282,  283,   -1,
  260,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  293,   -1,
  295,  296,  272,  273,  274,  275,  276,  277,   -1,  279,
  280,   -1,  282,  283,  284,  285,  286,  259,  260,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  297,  270,  271,
  272,  273,  274,  275,  276,  277,   -1,  279,  280,   -1,
   -1,   -1,   -1,   -1,  259,  260,   -1,   -1,   -1,   -1,
   -1,  293,   -1,  295,  296,  270,  271,  272,  273,  274,
  275,  276,  277,   -1,  279,  280,   -1,   -1,   -1,   -1,
   -1,  259,  260,   -1,   -1,   -1,   -1,   -1,  293,   -1,
  295,  296,  270,  271,  272,  273,  274,  275,  276,  277,
   -1,  279,  280,   -1,   -1,   -1,   -1,   -1,  259,  260,
   -1,   -1,   -1,   -1,   -1,  293,   -1,  295,  296,  270,
  271,  272,  273,  274,  275,  276,  277,   -1,  279,  280,
   -1,   -1,   -1,   -1,   -1,  259,  260,   -1,   -1,  259,
  260,   -1,  293,   -1,  295,  296,  270,  271,  272,  273,
  270,  271,  272,  273,   -1,  279,  280,   -1,   -1,  279,
  280,   -1,   -1,   -1,   -1,   -1,  259,  260,   -1,  293,
   -1,  295,  296,  293,   -1,  295,  296,  270,  271,   -1,
  259,  260,   -1,   -1,  259,  260,  279,  280,   -1,   -1,
   -1,  270,  271,  259,  260,  270,  271,   -1,   -1,   -1,
  293,  280,  295,  296,  270,  271,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  293,   -1,  295,  296,  293,   -1,
  295,  296,   -1,   -1,   -1,   -1,   -1,  293,  258,  295,
  296,   -1,  262,  263,  264,  265,   -1,   -1,   -1,  269,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  278,   -1,
   -1,  281,   -1,   -1,   -1,   -1,   -1,  287,   -1,   -1,
   -1,  291,  292,  270,  294,  272,  273,  274,  275,  276,
  277,   -1,  279,  280,   -1,  282,  283,  284,  285,  286,
  271,  272,  273,  274,  275,  276,  277,   -1,  279,  280,
  297,  282,  283,  284,  285,  286,   -1,  272,  273,  274,
  275,  276,  277,   -1,  279,  280,  297,  282,  283,  284,
  285,  286,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  296,  297,  272,  273,  274,  275,  276,  277,   -1,
  279,  280,   -1,  282,  283,  284,  285,  286,   -1,  272,
  273,  274,  275,  276,  277,   -1,  279,   -1,  297,  282,
  283,  284,  285,  286,   -1,  272,  273,  274,  275,  276,
  277,   -1,   -1,   -1,  297,  282,  283,  284,  285,  286,
   -1,   -1,   -1,  274,  275,  276,  277,   -1,   -1,   -1,
  297,  282,  283,  284,  285,  286,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  297,
};
#define YYFINAL 3
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 300
#define YYUNDFTOKEN 308
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"UNIT","LP","RP","NL","CHR","STR",
"ID","REAL","INT","CBEG","CEND","CONS","IF","THEN","ELSE","EQ","NE","GT","GE",
"LT","LE","BOOL","AND","OR","NOT","ADD","SUB","MUL","DIV","MOD","NEG","BIND",
"VAL","FUN","FN","RBEG","REND","LBEG","LEND","SEP","GET","LET","IN","END",0,0,0,
0,0,0,0,"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : block",
"block : state block",
"block : state",
"state : VAL ID BIND exp nl",
"state : FUN ID ID BIND exp nl",
"bindings : bindings SEP ID BIND exp",
"bindings : ID BIND exp",
"bindings :",
"listelems : exp SEP listelems",
"listelems : exp",
"listelems :",
"exp : LP exp RP",
"exp : exp GET ID",
"exp : NEG exp",
"exp : NOT exp",
"exp : exp AND exp",
"exp : exp OR exp",
"exp : exp ADD exp",
"exp : exp SUB exp",
"exp : exp MUL exp",
"exp : exp DIV exp",
"exp : exp MOD exp",
"exp : exp EQ exp",
"exp : exp NE exp",
"exp : exp LT exp",
"exp : exp LE exp",
"exp : exp GT exp",
"exp : exp GE exp",
"exp : IF exp THEN exp ELSE exp",
"exp : ID LP exp RP",
"exp : FN ID BIND exp",
"exp : INT",
"exp : REAL",
"exp : BOOL",
"exp : ID",
"exp : STR",
"exp : RBEG bindings REND",
"exp : LBEG listelems LEND",
"nl : NL nl",
"nl : NL",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 116 "parser.y"

int main() {
  blocks.push(new BlockNode());
 if (yyparse() == 0) //parsing was successful
     for (StatementNode* s : blocks.top()->statements) {
//       std::cout << blocks.top()->toString() << std::endl;
     }
  std::cout << generateCode(pgrmTranslate(blocks.top())) << std::endl;


 //while(yylex()) std::cout << ((yylval == 0) ? "" : yylval->toString()) << std::endl;
  return 0;
}
#line 459 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    YYERROR_CALL("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 62 "parser.y"
	{ }
break;
case 2:
#line 63 "parser.y"
	{ }
break;
case 3:
#line 65 "parser.y"
	{blocks.top()->add(new ValNode((IdentifierNode*) yystack.l_mark[-3], (ExpressionNode*) yystack.l_mark[-1])); yyval = yystack.l_mark[-4];}
break;
case 4:
#line 66 "parser.y"
	{blocks.top()->add(new FunDeclNode((IdentifierNode*) yystack.l_mark[-4], (IdentifierNode*) yystack.l_mark[-3], (ExpressionNode*) yystack.l_mark[-1])); yyval = yystack.l_mark[-5];}
break;
case 5:
#line 68 "parser.y"
	{ yyval = yystack.l_mark[-4];  ((RecordNode*)yyval)->add(new ValNode((IdentifierNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0])); }
break;
case 6:
#line 69 "parser.y"
	{ yyval = new RecordNode(); ((RecordNode*)yyval)->add(new ValNode((IdentifierNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0])); }
break;
case 7:
#line 70 "parser.y"
	{yyval = new RecordNode(); }
break;
case 8:
#line 73 "parser.y"
	{RecordNode* r = new RecordNode();
             r->add(new ValNode(new IdentifierNode("head"), (ExpressionNode*) yystack.l_mark[-2]));
             r->add(new ValNode(new IdentifierNode("tail"), (RecordNode*) yystack.l_mark[0])); yyval = r;}
break;
case 9:
#line 76 "parser.y"
	{RecordNode* r = new RecordNode();
                  r->add(new ValNode(new IdentifierNode("head"), (ExpressionNode*) yystack.l_mark[0]));
                  r->add(new ValNode(new IdentifierNode("tail"), new RecordNode())); yyval = r;}
break;
case 10:
#line 79 "parser.y"
	{yyval = new RecordNode(); }
break;
case 11:
#line 81 "parser.y"
	{yyval = yystack.l_mark[-1];}
break;
case 12:
#line 82 "parser.y"
	{yyval = new GetNode((ExpressionNode*) yystack.l_mark[-2], (IdentifierNode*) yystack.l_mark[0]); }
break;
case 13:
#line 83 "parser.y"
	{yyval = new InvokeNode(new IdentifierNode("_neg_"), (ExpressionNode*) yystack.l_mark[0]);}
break;
case 14:
#line 84 "parser.y"
	{yyval = new InvokeNode(new IdentifierNode("_not_"), (ExpressionNode*) yystack.l_mark[0]);}
break;
case 15:
#line 85 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_and_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 16:
#line 86 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_or_"),  (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 17:
#line 87 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_add_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 18:
#line 88 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_sub_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 19:
#line 89 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_mul_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 20:
#line 90 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_div_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 21:
#line 91 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_mod_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 22:
#line 92 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_eq_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 23:
#line 93 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_ne_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 24:
#line 94 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_lt_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 25:
#line 95 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_le_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 26:
#line 96 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_gt_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 27:
#line 97 "parser.y"
	{yyval = new BinOpNode(new IdentifierNode("_ge_"), (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 28:
#line 98 "parser.y"
	{yyval = new IfNode((ExpressionNode*) yystack.l_mark[-4], (ExpressionNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 29:
#line 99 "parser.y"
	{yyval = new InvokeNode((IdentifierNode*) yystack.l_mark[-3], (ExpressionNode*) yystack.l_mark[-1]);}
break;
case 30:
#line 100 "parser.y"
	{yyval = new LambdaNode((IdentifierNode*) yystack.l_mark[-2], (ExpressionNode*) yystack.l_mark[0]);}
break;
case 31:
#line 101 "parser.y"
	{yyval = yystack.l_mark[0];}
break;
case 32:
#line 102 "parser.y"
	{yyval = yystack.l_mark[0];}
break;
case 33:
#line 103 "parser.y"
	{yyval = yystack.l_mark[0];}
break;
case 34:
#line 104 "parser.y"
	{yyval = yystack.l_mark[0];}
break;
case 35:
#line 105 "parser.y"
	{yyval = yystack.l_mark[0]; }
break;
case 36:
#line 106 "parser.y"
	{yyval = yystack.l_mark[-1];}
break;
case 37:
#line 107 "parser.y"
	{yyval = yystack.l_mark[-1];}
break;
case 38:
#line 110 "parser.y"
	{ }
break;
case 39:
#line 111 "parser.y"
	{ }
break;
#line 821 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    YYERROR_CALL("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
